cd ../src
make
cd ../tests
../src/sclp --show-asm "$1"
cp "$1".spim output.spim
../reference-implementations/A5-sclp --show-asm -s "$1"
mv "$1".spim expected.spim
diff -Bw output.spim expected.spim
