use UtilReplicatedVar, IO;

/************************* Parallel Lines Iterator *************************/
// NOTE: we are assuming that we have at least one "good" line per stripe/block.
// (i.e. we only have to buffer one stripe and we won't get multiple buffering on the
// stripes)

// false: tack on the front
// true: tack on the back
// We need atomic access to the string (hence the locks)
proc write_tuple(str:string, i:int, ref lock: atomic int, arr: [?D] (atomic int,
      string, /* front or back */ bool, /* overlap */ bool), put_on_front_of_str:bool):bool {

  var full : atomic bool;
  full.write(false);

  while(!lock.compareExchange(0, 1)) {
    lock.waitFor(0);
  }

  // second one here, let's go and see which way we need to tack
  if ((arr[i](1).read() == 2)) {
    if (arr[i](3)) {
      arr[i](2) += str;
    } else {
      arr[i](2) = str + arr[i](2);
    }
    full.write(true);
  } else {
  // first one here, put our string on, and say whether we're front or back
    arr[i](1).write(1);
    arr[i](2) = str;
    arr[i](3) = put_on_front_of_str;
    // write whether or not there was any overlap
    full.write(arr[i](4));
    arr[i](1).add(1);
  }

  lock.write(0);
  return full.read();
}

iter file.splitAt(delim:string = '\n', start:int(64) = 0, end:int(64) = max(int(64)), hints:iohints =
    IOHINT_NONE, style:iostyle = this._style, locales=Locales) {

  // We cannot set record fields in iterators currently
  var local_style = style;
  local_style.string_format = QIO_STRING_FORMAT_TOEND;
  local_style.string_end = ascii(delim):style_char_t;

  var reader = this.reader( kind=iokind.dynamic, locking=false,
      start=start, end=end,
      hints=hints, style=local_style);

  var len:ssize_t;
  var tx:c_string;
  var x:string;
  var err:syserr = ENOERR;

  while true {
    on this.home {
      err = qio_channel_scan_string(false, reader._channel_internal, tx, len, -1);
      x = toString(tx);
    }
    if err then break;
    yield x;
  }
  reader.close();
}

iter file.splitAt(param tag: iterKind, delim:string = '\n', start:int(64) = 0, end:int(64) = max(int(64)),
    hints:iohints = IOHINT_NONE, style:iostyle = this._style, locales=Locales) where
tag == iterKind.leader {

  extern var qbytes_iobuf_size:size_t;
  var mylocs:domain(locale);
  var bufchunk = qbytes_iobuf_size:int;
  var chunk = this.getchunk(start, end);
  var local_style = style;
  local_style.string_format = QIO_STRING_FORMAT_TOEND;
  local_style.string_end = ascii(delim):style_char_t;

  // If the file is distributed, get the chunk/block size of distribution
  // and make sure that it is a multiple of mychunk.
  // We'll assume that the file is made of chunks of the same size
  // as the first.
  if (chunk(1) == 0 && chunk(2) == 0) ||
    chunk(1) > start {
      var startchunk = start / bufchunk;
      var endchunk = startchunk + 1;
      chunk = (startchunk*bufchunk,endchunk*bufchunk);
    }

  var chunksz = chunk(2) - chunk(1);
  var theend = end;
  var sz = this.length();
  var boundary = chunk(1);

  if chunksz <= 0 then chunksz = bufchunk;
  if theend > sz then theend = sz;

  // Now compute the start and end rounded out
  // to chunk sizes.
  var startchunk = (start - boundary) / chunksz;
  var endchunk = 1 + (theend - 1 - boundary)/chunksz;

  // Now set up the array that will hold the partial strings that cross block
  // boundaries
  var overlap_buf: [startchunk..endchunk-1] (atomic int, string, bool, bool);
  var arr_lock: [0..1] atomic int;

  // We now need to replicate our files across the locales
  var files: [rcDomain] file;

  // get the filesystem type (lustre, hdfs, none etc.)
  var ftype = this.fstype();

  // XXX/XXX HACK HACK!!! -- see future on this
  //--------------------------------------------
  proc file.open(out error:syserr, path:string="", mode:iomode, hints:iohints=IOHINT_NONE,
      style:iostyle = defaultIOStyle(), url:string=""):file {
    // hdfs paths are expected to be of the form:
    // hdfs://<host>:<port>/<path>
    proc parse_hdfs_path(path:string): (string, int, string) {

      var hostidx_start = path.indexOf("//");
      var new_str = path.substring(hostidx_start+2..path.length);
      var hostidx_end = new_str.indexOf(":");
      var host = new_str.substring(0..hostidx_end-1);

      new_str = new_str.substring(hostidx_end+1..new_str.length);

      var portidx_end = new_str.indexOf("/");
      var port = new_str.substring(0..portidx_end-1);

      //the file path is whatever we have left
      var file_path = new_str.substring(portidx_end+1..new_str.length);

      return (host, port:int, file_path);
    }

    var local_style = style;
    var ret:file;
    ret.home = here;
    if (url != "") {
      if (url.startsWith("hdfs://")) { // HDFS
        var (host, port, file_path) = parse_hdfs_path(url);
        var fs:c_void_ptr;
        error = hdfs_connect(fs, host.c_str(), port);
        if error then ioerror(error, "Unable to connect to HDFS", host);
        error = qio_file_open_access_usr(ret._file_internal, file_path.c_str(), _modestring(mode).c_str(), hints, local_style, fs, hdfs_function_struct_ptr);
        // Since we don't have an auto-destructor for this, we actually need to make
        // the reference count 1 on this FS after we open this file so that we will
        // disconnect once we close this file.
        hdfs_do_release(fs);
        if error then ioerror(error, "Unable to open file in HDFS", url);
      } else if (url.startsWith("http://", "https://", "ftp://", "ftps://", "smtp://", "smtps://", "imap://", "imaps://"))  { // Curl
        error = qio_file_open_access_usr(ret._file_internal, url.c_str(), _modestring(mode).c_str(), hints, local_style, c_nil, curl_function_struct_ptr);
        if error then ioerror(error, "Unable to open URL", url);
      } else {
        ioerror(ENOENT:syserr, "Invalid URL passed to open");
      }
    } else {
      if (path == "") then
        ioerror(ENOENT:syserr, "in open: Both path and url were path");

      error = qio_file_open_access(ret._file_internal, path.c_str(), _modestring(mode).c_str(), hints, local_style);
    }

    return ret;
  }
  //--------------------------------------------

  forall loc in locales {
    mylocs+=loc;
    on loc {
      var err:syserr = ENOERR;
      var llocal_style = local_style;
      // String local hack. See Sung's comment in internal/MemTracking.chpl
      var hack = this.path.locale.id;
      const lpath = this.path;
      if ftype == FTYPE_HDFS {
        rcLocal(files) = this.open(error=err, path="", mode=iomode.r, hints=hints, style=llocal_style, url=lpath);
      } else {
        rcLocal(files) = this.open(error=err, path=lpath, mode=iomode.r, hints=hints, style=llocal_style, url="");
      }
    }
  }

  coforall loc in locales do on loc {
    forall chunk in startchunk..endchunk-1 {
      var followstart = boundary + chunk*chunksz;
      var followend = boundary + (chunk+1)*chunksz;
      // and then handle the boundaries correctly.
      if (followstart > theend ||
          followend < start) {
        // do nothing, it's outside our region
      } else {
        if followstart < start then followstart = start;
        if followend > theend then followend = theend;
        var chosenloc = here;
        var gotloc = false;
        // Now, did the returned locs array have here as the first
        // locale in uselocs? Did it have any locale in uselocs?
        for loc in rcLocal(files).localesForRegion(followstart, followend) {
          if mylocs.member(loc) {
            gotloc = true;
            chosenloc = loc;
            break;
          }
        }
        // If we have all of the locales, we should round robin
        if (!gotloc) {
          // No overlap. Deterministically select a loc from locales.
          var id = (chunk % locales.size) + locales.domain.low;
          chosenloc = locales[id];
        }
        if (chosenloc == here) { // Do we need this?
          yield (((followstart-start)..(followend-start-1), overlap_buf, arr_lock, chunk), files);
        }
      }
    }
  }

  forall loc in locales {
    on loc {
      rcLocal(files).close();
    }
  }
}

iter file.splitAt(param tag: iterKind, delim:string = '\n', start:int(64) = 0, end:int(64) = max(int(64)),
    hints:iohints = IOHINT_NONE, style:iostyle = this._style, locales=Locales, followThis)
where tag == iterKind.follower {
  const lowBasedIters = followThis(1)(1).translate(start);

  assert(lowBasedIters.stride == 1);

  var mystart = lowBasedIters.low;
  var myend = lowBasedIters.high + 1;
  var local_style = style;
  local_style.string_format = QIO_STRING_FORMAT_TOEND;
  local_style.string_end = ascii(delim):style_char_t;

  if mystart < start then mystart = start;
  if myend > end then myend = end;

  var reader = rcLocal(followThis(2)).reader( kind=iokind.dynamic, locking=false,
      start=mystart, end=myend,
      hints=hints, style=local_style);

  var len:ssize_t;
  var tx:c_string;
  var x:string;
  var err:syserr = ENOERR;
  var total = mystart;
  var once = true;

  // We are assuming that lines are "reasonable". i.e., that we have at least one
  // line per stripe/block.
  while true {
    len = 0;
    err = qio_channel_scan_string(false, reader._channel_internal, tx, len, -1);
    x = toString(tx);

    // Now see if we have any stuff from the last block to take care of for the first
    // part of the file.
    if (once) {
      once = false;
      if (followThis(1)(4) > 0) {
        if (write_tuple(x, followThis(1)(4)-1, followThis(1)(3)(0), followThis(1)(2), false)) {
          x = (followThis(1)(2))[followThis(1)(4)-1](2);
        } else {
          // else, we gave up this line since we're waiting on the other follower to
          // append its overlap onto this.
          total += len; // we have to account for the stuff we read above!
          len = 0;
          // Now, read again, and we'll return this out. This way, this iterator will
          // return the same # of lines as the serial iterator.
          err = qio_channel_scan_string(false, reader._channel_internal, tx, len, -1);
          x = toString(tx);
        }
      }
    }

    // We have some stuff left, but we don't have an entire line (on the boundary)
    // so mark it, and add to the pool. Note: we dont't care about
    // ordering of the lines, since we are inherently parallel, and therefore as long
    // as we return "contiguous lines" we are fine.
    if ((len == 0) && (total < myend)) {
      // retry
      err = ENOERR; // Reset error from above
      // Just get whatever is left, it's definitely not EOF or a line
      err = qio_channel_read_string(false, local_style.byteorder, myend-total, reader._channel_internal, tx, len, -1);
      x = toString(tx);

      // Now, put this in the shared array so that we can go and splice up the lines
      if (write_tuple(x, followThis(1)(4), followThis(1)(3)(0), followThis(1)(2), true)) {
        x = (followThis(1)(2))[followThis(1)(4)](2); // other guy was here first
        yield x;
        break;
      } else break; // We got here first, and we've read all that remains. So break.
    }

    // We have nicely split on a boundary.
    if ((total == myend)  && (len == 0)) {
      // Now, put this in the shared array so that we can go and splice up the lines
      // we do a "nop" here. We simply say that we hit this point in the file.
      if (write_tuple("", followThis(1)(4), followThis(1)(3)(0), followThis(1)(2), true)) {
        x = (followThis(1)(2))[followThis(1)(4)](2); // other guy was here first
        yield x;
        break;
      }
    }

    total += len;
    if err then break;
    yield x;
  }
  reader.close();
}

