**PROCEDURE: bar_
**BEGIN: Three Address Code Statements
a_ = 10

**END: Three Address Code Statements
**PROCEDURE: foo_
**BEGIN: Three Address Code Statements
temp0 = y_ && bb_
temp1 = x_ > 1
temp2 = z_ < 10.00
temp3 = temp1 && temp2
temp4 = temp0 || temp3
temp6 = ! temp4
if(temp6) goto Label2
stemp1 = 10.00
goto Label3
Label2: 
temp5 = z_ * 4.00
stemp1 = temp5
Label3: 
stemp0 = stemp1
goto Label0
Label0: 
return stemp0

**END: Three Address Code Statements
**PROCEDURE: foobar_
**BEGIN: Three Address Code Statements
stemp0 = 69
goto Label1
Label1: 
return stemp0

**END: Three Address Code Statements
**PROCEDURE: main
**BEGIN: Three Address Code Statements
temp1 = 1 < 2
temp0 = foo_(1, temp1, 20.00)
f_ = temp0
temp3 = 1 + x_
temp4 = x_ < 2
temp5 = f_ + 20.25
temp2 = foo_(temp3, temp4, temp5)
f_ = temp2
temp6 = 1 + x_
temp7 = x_ < 2
temp8 = f_ + 20.25
bar_(temp6, temp7, temp8)
temp10 = 1 + x_
temp11 = x_ < 2
temp12 = f_ + 20.25
temp9 = foobar_(temp10, temp11, temp12)
x_ = temp9

**END: Three Address Code Statements
