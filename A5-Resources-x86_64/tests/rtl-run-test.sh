cd ../src
make
cd ../tests
../src/sclp --show-rtl "$1"
mv "$1".rtl output.rtl
../reference-implementations/A5-sclp --show-rtl -s "$1"
mv "$1".rtl expected.rtl
diff -Bw output.rtl expected.rtl
