// Simple case ///////////////////////////
writeln("x y");
var x = 5;
refvar y = x;
writeln(x, ' ', y);
x += 1;
writeln(x, ' ', y);
y += 1;
writeln(x, ' ', y, '\n');

// Array element modification ////////////
var A = [1, 2, 3, 4, 5];
writeln("A  : ", A);
for i in 1..5 {
  refvar element = A[i];
  element += 1;
}
writeln("A' : ", A, '\n');


// Array of Array element modification ///
var B = [[1, 2], [3, 4]];
writeln("B  : ", B);
for i in 1..2 {
  for j in 1..2 {
    refvar element = B[i][j];
    element += 1;
  }
}
writeln("B' : ", B, '\n');


// Records ///////////////////////////////
record Point {
  var x: real;
  var y: real;
}

record Rect {
  var upLeft: Point;
  var botRight: Point;
}

var rectangles = [
  new Rect(new Point(1.0, 1.0), new Point(4.0, 5.0)),
  new Rect(new Point(2.0, 3.0), new Point(6.0, 7.0))
];

writeln("rectangles:");
for r in rectangles do
  writeln(r);

for r in rectangles {
  refvar upLeft = r.upLeft;
  refvar botRight = r.botRight;
  upLeft.x += 1.0;
  botRight.y -= 1.0;
}

writeln('\n', "rectangles':");
for r in rectangles do
  writeln(r);

// Class ////////////////////////////////
class Foo {
  var x = 1.0;

  proc doubleMe() {
    x *= 2;
  }
}

var myFoo = new Foo();
refvar myrefFoo = myFoo;
writeln('\n', "myFoo     myrefFoo");
writeln(myFoo, ' ', myrefFoo);
myFoo.doubleMe();
writeln(myFoo, ' ', myrefFoo);
myrefFoo.doubleMe();
writeln(myFoo, ' ', myrefFoo);
