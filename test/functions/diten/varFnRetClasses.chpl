class C { }
class D: C { }

var c = new C();
var d = new D();

def f(a: bool) var : C {
  if a then
    return c;
  else
    return d;
}

f(true) = new C();
f(false) = new D();
