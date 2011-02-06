_extern proc chpl_malloc(number, size, description, userCode=true, lineno=-1, filename=""): opaque;
_extern proc chpl_realloc(ptr, number, size, description, lineno=-1, filename=""): opaque;
_extern proc chpl_free(ptr, userCode=true, lineno=-1, filename="");

_extern proc resetMemStat();
_extern proc printMemStat(lineno=-1, filename="");

resetMemStat();

var i = chpl_malloc(1, numBytes(int(64)), "int(64)", true, -1, "");
writeln("malloc'd an int");
printMemStat();

// numBytes(bool) == 0 which seems wrong
// but I'm not sure one should be able to query numBytes(bool anyway,
// and believe that we're going to want to make the width vary from
// implementation to implementation, so am changing the following
// from a bool to an int(8)
//
var b = chpl_malloc(1, numBytes(int(8)), "fake bool", true, -1, "");
writeln("malloc'd a bool");
printMemStat();

var f = chpl_malloc(1, numBytes(real), "real", true, -1, "");
writeln("malloc'd a real");
printMemStat();

chpl_free(i);
chpl_free(b);
writeln("freed the int and the bool");
printMemStat();

f = chpl_realloc(f, 10, numBytes(real), "_real64", -1, "");
writeln("realloc'd 10 times the real");
printMemStat();

chpl_free(f);
writeln("freed the real");
printMemStat();
