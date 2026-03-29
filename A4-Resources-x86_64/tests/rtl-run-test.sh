cd ../src
make
cd ../tests
../src/sclp --show-rtl main.c
mv main.c.rtl output.rtl
../reference-implementations/A4-sclp --show-rtl main.c
mv main.c.rtl expected.rtl
diff -Bw output.rtl expected.rtl
