_extern class C { var x: int; }
proc my_foo(c: C, x: int) {
  c.x = x;
}
var myC = new C(5);

writeln(myC);

my_foo(myC, 3);

writeln(myC);
