**PROCEDURE: foo_
**BEGIN: Three Address Code Statements
	stemp0 = 10
	goto Label0
Label0:
	 return stemp0
**END: Three Address Code Statements
**PROCEDURE: main
**BEGIN: Three Address Code Statements
	temp0 = foo_()
	a_ = temp0
**END: Three Address Code Statements
