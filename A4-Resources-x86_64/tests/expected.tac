**PROCEDURE: main
**BEGIN: Three Address Code Statements
	temp1 = ! b1_
	if(temp1) goto Label1
	temp0 = x_ * 2
	x_ = temp0
	goto Label0
Label1:
	temp2 = 2 * y_
	z_ = temp2
Label0:
**END: Three Address Code Statements
