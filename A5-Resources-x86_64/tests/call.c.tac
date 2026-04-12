**PROCEDURE: bar_
**BEGIN: Three Address Code Statements
	a_ = 10
**END: Three Address Code Statements
**PROCEDURE: foo_
**BEGIN: Three Address Code Statements
	stemp0 = 10.00
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
	temp1 = 1 + x_
	temp2 = x_ < 2
	temp3 = f_ + 20.25
	temp0 = foo_(temp1, temp2, temp3)
	f_ = temp0
	temp4 = 1 + x_
	temp5 = x_ < 2
	temp6 = f_ + 20.25
	bar_(temp4, temp5, temp6)
	temp8 = 1 + x_
	temp9 = x_ < 2
	temp10 = f_ + 20.25
	temp7 = foobar_(temp8, temp9, temp10)
	x_ = temp7
**END: Three Address Code Statements
