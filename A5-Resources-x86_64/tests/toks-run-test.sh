cd ../src
make
cd ../tests
../src/sclp --show-tokens "$1"
mv "$1".toks output.toks
../reference-implementations/A5-sclp --show-tokens "$1"
mv "$1".toks expected.toks
diff -Bw output.toks expected.toks
