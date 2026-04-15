#! /bin/bash

cd src
make
cp sclp ../../../A5_Eval_2026/
make clean
cd ..
cp -r src/* JAN2026_group_10
tar cvzf A5-JAN2026_group_10-JAN2026_group_10.tar.gz JAN2026_group_10/
cp A5-JAN2026_group_10-JAN2026_group_10.tar.gz ../../A5_Eval_2026/Submissions/
