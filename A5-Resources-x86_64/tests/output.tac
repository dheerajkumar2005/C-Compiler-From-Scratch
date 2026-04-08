**PROCEDURE: foo_
**BEGIN: Three Address Code Statements
a_ = 100
temp0 = 2 * a_
b_ = temp0
temp1 = a_ + b_
c_ = temp1
temp2 = a_ * b_
temp3 = temp2 + c_
stemp1 = temp3
goto Label1
Label1: 
return stemp1

**END: Three Address Code Statements
**PROCEDURE: main
**BEGIN: Three Address Code Statements
temp4 = 1 + 2
a_ = temp4
temp5 = 1 + 2
stemp0 = temp5
goto Label0
Label0: 
return stemp0

**END: Three Address Code Statements
