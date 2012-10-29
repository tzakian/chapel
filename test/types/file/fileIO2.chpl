config var n = 10,
           filename = "arr2.out";

const ADom = {1..n, 1..n};

var A: [ADom] real = [(i,j) in ADom] (i-1) + ((j-1)/10.0);

writeArray(n, A, filename);
var B = readArray(filename);

const numErrors = + reduce [i in ADom] (A(i) != B(i));

writeln("B is:\n", B);

if (numErrors > 0) {
  writeln("FAILURE");
} else {
  writeln("SUCCESS");
}


proc writeArray(n, X, filename) {
  var outfile = open(filename, iomode.cw).writer();
  outfile.writeln(n, " ", n);
  outfile.writeln(X);
  outfile.close();
}


proc readArray(filename) {
  var m, n: int;

  var infile = open(filename, iomode.r).reader();
  infile.read(m);
  infile.read(n);

  const XDom = {1..m, 1..n};
  var X: [XDom] real;

  for ij in XDom do infile.read(X(ij));

  infile.close();

  return X;
}


