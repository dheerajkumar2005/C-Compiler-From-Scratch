cd ../src
make
cd ../tests
../src/sclp --show-tac "$1"
mv "$1".tac output.tac
../reference-implementations/A5-sclp --show-tac "$1"
mv "$1".tac expected.tac
diff -Bw output.tac expected.tac
