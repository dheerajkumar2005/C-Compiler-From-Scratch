cd ../src
make
cd ../tests
../src/sclp --show-ast main.c
mv main.c.ast output.ast
../reference-implementations/A4-sclp --show-ast main.c
mv main.c.ast expected.ast
diff -Bw output.ast expected.ast
