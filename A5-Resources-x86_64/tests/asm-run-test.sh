cd ../src
make
cd ../tests

../src/sclp --show-asm "$1"
grep -v '^[[:space:]]*$' "$1".spim > output.spim

../reference-implementations/A5-sclp --show-asm "$1"
sed -i 's/;;.*//' "$1".spim
grep -v '^[[:space:]]*$' "$1".spim > expected.spim

diff -Bw output.spim expected.spim
