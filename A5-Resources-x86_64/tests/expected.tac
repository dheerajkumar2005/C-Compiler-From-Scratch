**PROCEDURE: foo_
**BEGIN: Three Address Code Statements
	temp0 = ! y_
	if(temp0) goto Label1
	stemp1 = 10.00
	goto Label2
Label1:
	stemp1 = 20.00
Label2:
	stemp0 = stemp1
	goto Label0
Label0:
	 return stemp0
**END: Three Address Code Statements
**PROCEDURE: main
**BEGIN: Three Address Code Statements
	temp1 = 1 < 2
	temp0 = foo_(1, temp1, 20.00)
	f_ = temp0
**END: Three Address Code Statements
