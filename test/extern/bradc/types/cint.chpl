_extern type c_int;

var my_c_int: c_int;

_extern def set_c_int(inout lhs: c_int, rhs: int(32));

_extern def print_c_int(x: c_int);

set_c_int(my_c_int, 32);
print_c_int(my_c_int);
