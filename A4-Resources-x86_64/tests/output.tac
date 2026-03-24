**PROCEDURE: main
**BEGIN: Three Address Code Statements
temp2 = ! b1_
if(temp2) goto Label1
temp0 = x_ * 2
x_ = temp0
goto Label0
Label1: 
temp1 = 2 * y_
z_ = temp1
Label0: 

**END: Three Address Code Statements
