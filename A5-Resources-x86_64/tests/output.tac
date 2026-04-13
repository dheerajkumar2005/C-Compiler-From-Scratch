**PROCEDURE: foo_
**BEGIN: Three Address Code Statements
temp0 = ! y_
if(temp0) goto Label1
stemp0 = 10.00
goto Label2
Label1: 
stemp0 = 20.00
Label2: 
stemp0 = stemp0
goto Label0
Label0: 
return stemp0

**END: Three Address Code Statements
**PROCEDURE: main
**BEGIN: Three Address Code Statements
temp2 = 1 < 2
temp1 = foo_(1, temp2, 20.00)
f_ = temp1

**END: Three Address Code Statements
