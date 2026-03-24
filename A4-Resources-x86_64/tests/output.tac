**PROCEDURE: main
**BEGIN: Three Address Code Statements
temp0 = x_ < 20
b1_ = temp0

temp1 = b1_ && b2_
temp2 = temp1 || b3_
temp7 = ! temp2
if(temp7) goto Label0
temp3 = x_ * 2
x_ = temp3
temp4 = x_ - 9
x_ = temp4
goto Label1
Label0: 
temp5 = y_ - 236
z_ = temp5
temp6 = 2 * y_
z_ = temp6
Label1: 

**END: Three Address Code Statements
