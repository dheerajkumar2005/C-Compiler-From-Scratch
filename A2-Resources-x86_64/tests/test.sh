#! /bin/bash

output1=$(../reference-implementations/A2-sclp test.c --show-ast -d)
output2=$(../src/sclp test.c --show-ast -d )

tmp1=$(mktemp)
tmp2=$(mktemp)

echo $output1 > $tmp1
echo $output2 > $tmp2

diff -Bw $tmp1 $tmp2
rm $tmp1 $tmp2
