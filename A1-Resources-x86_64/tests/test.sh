cd ../src
make
./sclp ../example-programs/Level-2-invalid-test-cases/l2-invalid-exmp4.c 2>| error.log
echo $?
diff -Bw error.log ../tests/empty.log
