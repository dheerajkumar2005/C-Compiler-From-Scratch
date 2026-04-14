cd ../src
make
cd ../tests
../src/sclp --show-ast "$1" --sa-parse
mv "$1".ast output.ast
../reference-implementations/A5-sclp --show-ast "$1" --sa-parse
mv "$1".ast expected.ast
diff -Bw output.ast expected.ast
