cd ../src
make
cd ../tests
../src/sclp --show-tac main.c
mv main.c.tac output.tac
../reference-implementations/A4-sclp --show-tac main.c
mv main.c.tac expected.tac
diff -Bw output.tac expected.tac
