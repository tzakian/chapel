module desirable_syntax {

  // Proposal for syntax to allow reindexing a formal array argument,
  // preserving shape.

  // This is desirable to permit Fortran and C programmers to mimic their
  // existing styles, and to allow simpler code for array operands whose
  // formal index ranges are dissimilar across dimensions

  config const n = 4, row_base = -4, col_base = 17;

  def main {

    var A : [row_base .. #n, col_base .. #n] 2*int;

    for i in A.domain do
      A (i) = i;

    print_A (A);

    var A_reindexed : [0..#n, 0..#n] => A;

    print_A (A_reindexed);

    print_A_reindexed (A);

  }

  def print_A (A) {
    for i in A.domain.dim(1) do
      writeln (i, A (i, ..) );
    writeln ();
  }

  def print_A_reindexed (A : [0.., 0..]) {
    for i in A.domain.dim(1) do
      writeln (i, A (i, ..) );
  }
  
}
      