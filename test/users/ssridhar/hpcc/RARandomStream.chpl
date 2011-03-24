//
// A helper module for the RA benchmark that defines the random stream
// of values
//
module RARandomStream {
  param randWidth = 64;              // the bit-width of the random numbers
  type randType = uint(randWidth);   // the type of the random numbers

  //
  // m2 is a table (tuple) of helper values used to fast-forward
  // through the random stream.
  //
  const m2: randWidth*randType = computeM2Vals();

  proc whichRNG(): string {
    return "DEF";
  }

  //
  // A serial iterator for the random stream that resets the stream
  // to its 0th element and yields values endlessly.
  //
  iter RAStream(seed: randType) {
    var val = getNthRandom(0, seed);
    while (1) {
      getNextRandom(val);
      yield val;
    }
  }

  //
  // A "follower" iterator for the random stream that takes a range of
  // 0-based indices (follower) and yields the pseudo-random values
  // corresponding to those indices.  Follower iterators like these
  // are required for parallel zippered iteration.
  //
  iter RAStream(param tag: iterator, follower, seed: randType) where tag == iterator.follower {
    if follower.size != 1 then
      halt("RAStream cannot use multi-dimensional iterator");
    var val = getNthRandom(follower(1).low, seed);
    for follower {
      getNextRandom(val);
      yield val;
    }
  }

  //
  // A helper function for "fast-forwarding" the random stream to
  // position n in O(log2(n)) time
  //
  proc getNthRandom(in n: uint(64), seed: randType) {
    param period = 0x7fffffffffffffff/7;

    n %= period;
    if (n == 0) then return 0x1;
    var ran: randType = seed;
    for i in 0..log2(n)-1 by -1 {
      var val: randType = 0;
      for j in 0..#randWidth do
	if ((ran >> j) & 1) then val ^= m2(j+1);
      ran = val;
      if ((n >> i) & 1) then getNextRandom(ran);
    }
    return ran;
  }

  //
  // A helper function for advancing a value from the random stream,
  // x, to the next value
  //
  proc getNextRandom(inout x) {
    param POLY = 0x7;
    param hiRandBit = 0x1:randType << (randWidth-1);
    
    x = (x << 1) ^ (if (x & hiRandBit) then POLY else 0);
  }

  //
  // A helper function for computing the values of the helper tuple, m2
  //
  proc computeM2Vals() {
    var m2tmp: randWidth*randType;
    var nextVal = 0x1: randType;
    for i in 1..randWidth {
      m2tmp(i) = nextVal;
      getNextRandom(nextVal);
      getNextRandom(nextVal);
    }
    return m2tmp;
  }
  
  // note: power operator will result in a while loop
  proc indexMask(r: randType, n): randType {
    return r & ((1 << n) - 1);
  }
}
