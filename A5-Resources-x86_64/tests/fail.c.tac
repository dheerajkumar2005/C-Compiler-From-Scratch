**PROCEDURE: f_
**BEGIN: Three Address Code Statements
c_ = 5
temp0 = a_ + b_
temp1 = temp0 + c_
write temp1

**END: Three Address Code Statements
**PROCEDURE: g_
**BEGIN: Three Address Code Statements
temp0 = x_ + y_
z_ = temp0
stemp0 = z_
goto Label0
Label0: 
return stemp0

**END: Three Address Code Statements
**PROCEDURE: main
**BEGIN: Three Address Code Statements
temp0 = g_(10, 20)

**END: Three Address Code Statements
