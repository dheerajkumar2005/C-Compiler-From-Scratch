cd ../src
make
cd ../tests
../src/sclp "$1"
mv "$1".spim output.spim
../reference-implementations/A5-sclp -s "$1"
mv "$1".spim expected.spim
diff -Bw output.spim expected.spim
