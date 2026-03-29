cd ../src
make
cd ../tests
../src/sclp --show-ast "$1"
mv "$1".ast output.ast
../reference-implementations/A4-sclp --show-ast "$1"
mv "$1".ast expected.ast
diff -Bw output.ast expected.ast
