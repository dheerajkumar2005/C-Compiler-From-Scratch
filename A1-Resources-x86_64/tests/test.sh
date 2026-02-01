cd ../src
make
for i in {1..18}; do
    # echo "TC: {$i}"
    # check 1
    file="../example-programs/Level-2-test-cases/l2-exmp$i.c"
    ./sclp $file 2>| error1.log
    ret1=$?
    ../reference-implementations/A1-sclp $file 2>| error2.log
    ret2=$?
    if [[ "$ret1" == "$ret2" && "$ret1" == 1 ]]; then
        if [ -s error1.log ]; then
            echo "TC: ${i} passed"
        else
            echo "TC: ${i} failed"
            echo "Error.log is empty"
        fi
    elif [[ "$ret1" == "$ret2" && "$ret1" == 0 ]]; then
        ./sclp $file --show-tokens > tok1.txt
        ../reference-implementations/A1-sclp $file --show-tokens > tok2.txt
        diff -Bw tok1.txt tok2.txt > diff.txt
        if [ -s diff.txt ]; then
            echo "TC: ${i} failed"
            cat diff.txt
        else
            echo "TC: ${i} passed"
        fi
    else
        echo "TC: ${i} failed"
        echo "Return values not matching"
    fi


done

