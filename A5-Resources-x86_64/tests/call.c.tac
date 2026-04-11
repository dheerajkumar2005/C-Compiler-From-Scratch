**PROCEDURE: foo_
**BEGIN: Three Address Code Statements
	stemp0 = 10.00
	goto Label0
Label0:
	 return stemp0
**END: Three Address Code Statements
**PROCEDURE: main
**BEGIN: Three Address Code Statements
	temp1 = 1 + 2
	temp2 = 1 < 2
	temp3 = 10.00 + 20.00
	temp0 = foo_(temp1, temp2, temp3)
	f_ = temp0
**END: Three Address Code Statements
