// Testing various aspects of 1-d BlockCyclic.

use d, f, u;

config const verbose = false;
config const nopfx = false;
no_pfx = nopfx;

config const s1 = 1;
config const s2 = 3;
setupLocales(s1, s2);

var phase = 0;
proc leapphase() { phase += 20; fphase(phase); }

proc test(d) {
  leapphase();

  hd("domain = ", d);
  tl();

  const vd = d._value.dom2;
  if verbose {
    hd("vd = ", vd, "  : ", typeToString(vd.type));
    tl();
  } else {
    nextphase();
  }

  hd("storage");
  for (ix, locDdesc) in (d._value.dist.targetIds, d._value.localDdescs) do
    msg(" ", ix, "   ", locDdesc.myStorageDom);
  tl();

  proc t1(param k) {
    hd("dsiAccess1d dim", k);
    const subdom = if k==1 then d._value.dom1 else d._value.dom2;
    for i in d.dim(k) do
      msg(" ", i, "   ", subdom.dsiAccess1d(i));
    tl();
  }

  t1(1);
  t1(2);

  hd("serial iterator over the domain");
  msgserial(d);
  tl();

  hd("parallel iterator over the domain");
  forall i in d do msg(i);
  tl();

  hd("creating an array");
  var a: [d] int;
  tl();

  hd("initializing the array with explicit indexing");
  for ix in d do
    a[ix] = ( ix(1)*1000 + ix(2) ): a.eltType;
  tl();

  hd("serial iterator over the array");
  msgserial(a);
  tl();

  hd("zippered iterator over (domain, array)");
  forall (ix,a) in (d,a) do msg(ix, "  ", a);
  tl();

  hd("zippered iterator over (array, domain)");
  forall (a,ix) in (a,d) do msg(ix, "  ", a);
  tl();

} // test()

proc testsuite(type T, initphase) {
//compilerWarning("testsuite -- ", typeToString(T), 0);
  phase = initphase; leapphase();
  hd("testsuite(", typeToString(T), ")");
  tl();

  const df8 = new idist(lowIdx=-100, blockSize=7, numLocales=s1, name="D1");
  const df9 = new idist(lowIdx=-10, blockSize=5, numLocales=s2, name="D2");
  const dm = new dmap(new DimensionalDist(mylocs, df8, df9, "dm", idxType=T));

  proc tw(a, b, c, d) { test([a:T..b:T, c:T..d:T] dmapped dm); }
  inline
  proc t2(a,b) { tw(5,5,a,b); }
  proc t22(a,b,st,al) { test([5:T..5:T, a:T..b:T by st align al:T] dmapped dm); }

  t2(7,7);   // 1,0
  t2(12,12); // 1,1
  t2(18,18); // 1,2

  t2(5,9);   // 1,0
  t2(10,14); // 1,1
  t2(15,19); // 1,2

  t2(3,7);   // 0,2 - 1,0

  t2(3,12);  // 0,2 - 1,1
  t2(3,15);  // 0,2 - 1,2
  t2(3,16);  // 0,2 - 1,2
  t2(3,17);  // 0,2 - 1,2
  t2(3,18);  // 0,2 - 1,2
  t2(3,19);  // 0,2 - 1,2

  t2(0,16);  // 0,2 - 1,2
  t2(1,16);  // 0,2 - 1,2
  t2(2,16);  // 0,2 - 1,2
  t2(3,16);  // 0,2 - 1,2
  t2(4,16);  // 0,2 - 1,2

  t22(0,19,3,0);
  t22(0,19,3,1);
  t22(0,19,3,2);

  t22(0,19,5,1);
  t22(0,19,7,1);
  t22(0,19,8,1);
  t22(0,19,9,1);
  t22(0,19,10,1);
  t22(0,19,15,1);
  t22(0,19,18,1);
  t22(0,19,19,1);
  t22(0,19,20,1);

  tw(1,1,  7,7);  // 14,0 | 1,0
  tw(1,1, 12,12); // 14,0 | 1,1
  tw(1,1, 18,18); // 14,0 | 1,2

  tw(5,11, 5,9);
  tw(5,11, 10,14);
  tw(5,11, 16,19);

  test([ 5:T..11:T, 12:T..12:T] dmapped dm);

  test([ 1:T..1:T, 0:T..9:T       ] dmapped dm);
  test([ 1:T..1:T, 0:T..9:T by  1 ] dmapped dm);
  test([ 1:T..1:T, 0:T..9:T by  2 ] dmapped dm);
  test([ 1:T..1:T, 0:T..9:T by  3 ] dmapped dm);
  test([ 1:T..1:T, 0:T..9:T by -1 ] dmapped dm);
  test([ 1:T..1:T, 0:T..9:T by -2 ] dmapped dm);
  test([ 1:T..1:T, 0:T..9:T by -3 ] dmapped dm);

  tw(5,11, 47,42);
  tw(11,5, 42,47);
  tw(11,5, 47,42);
}

testsuite(int,        0);
testsuite(int(64), 1000);
//testsuite(uint,     2000);
//testsuite(uint(64), 3000);