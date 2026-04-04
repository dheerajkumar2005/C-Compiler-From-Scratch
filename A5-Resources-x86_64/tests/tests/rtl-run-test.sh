cd ../src
make
cd ../tests

../src/sclp --show-rtl "$1"
grep -v '^[[:space:]]*$' "$1".rtl > output.rtl

../reference-implementations/A5-sclp --show-rtl "$1"
sed -i 's/;;.*//' "$1".rtl
grep -v '^[[:space:]]*$' "$1".rtl > expected.rtl

diff -Bw output.rtl expected.rtl
