**PROCEDURE: main
**BEGIN: Three Address Code Statements
Label0: 
temp0 = x_ + y_
temp1 = temp0 >= z_
temp2 = y_ <= z_
temp3 = temp1 && temp2
temp8 = ! temp3
if(temp8) goto Label1
temp4 = x_ - 1
x_ = temp4
temp5 = y_ + 3
temp6 = x_ / 10
temp7 = temp5 - temp6
y_ = temp7
goto Label0
Label1: 

**END: Three Address Code Statements
