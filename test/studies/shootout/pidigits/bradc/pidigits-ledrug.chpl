/* The Computer Language Benchmarks Game
   http://shootout.alioth.debian.org/

   contributed by Tom Hildebrandt, Brad Chamberlain, Lydia Duncan
   derived from the GNU C version by Ledrug
*/

use GMP;

config const n = 50;                // Compute n digits of Pi, 50 by default

proc main() {
  for (d,i) in zip(gen_digits(n), 1..) {
    write(d);
    if i % 10 == 0 then
      writeln("\t:",i);
  }
}


iter gen_digits(numDigits) {
  var tmp1, tmp2, acc, den, num: mpz_t;

  mpz_init(tmp1);                          // init tmp1
  mpz_init(tmp2);                          // init tmp2
  mpz_init_set_ui(acc, 0);                 // acc = 0
  mpz_init_set_ui(den, 1);                 // den = 1
  mpz_init_set_ui(num, 1);                 // num = 1

  var d, k: c_ulong;

  for i in 1..numDigits {
    do {
      do {
        k += 1;
        const k2 = 2 * k + 1;

        //
        // Compute the next term
        //
        mpz_addmul_ui(acc, num, 2);        // acc += num * 2
        mpz_mul_ui(acc, acc, k2);	   // acc *= (2k+1)
        mpz_mul_ui(den, den, k2);          // den *= (2k+1)
        mpz_mul_ui(num, num, k);           // num *= k

        //
        // Continue looping until the digit is ready
        //
      } while (mpz_cmp(num, acc) > 0);     // while num > acc

      d = extract_digit(3);
    } while (d != extract_digit(4));       // while the 3rd digit, d != the 4th

    yield d;                               // once it differs, yield it

    //
    // eliminate digit d
    //
    mpz_submul_ui(acc, den, d);            // acc = (acc - den) * d
    mpz_mul_ui(acc, acc, 10);              // acc *= 10
    mpz_mul_ui(num, num, 10);              // num *= 10
  }

  //
  // Helper function to extract the nth digit
  //
  proc extract_digit(nth: c_ulong) {
    mpz_mul_ui(tmp1, num, nth);              // tmp1 = num * nth
    mpz_add(tmp2, tmp1, acc);                // tmp2 = tmp1 + acc
    mpz_tdiv_q(tmp1, tmp2, den);             // tmp1 = tmp2 / den
    
    return mpz_get_ui(tmp1);                 // convert tmp1 to a c_uint
  }
}


