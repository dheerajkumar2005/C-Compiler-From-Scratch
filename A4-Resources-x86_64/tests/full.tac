**PROCEDURE: main
**BEGIN: Three Address Code Statements
	temp0 = x_ < y_
	temp1 = ! temp0
	if(temp1) goto Label1
	z_ = 10
	goto Label0
Label1:
	z_ = 20
Label0:
**END: Three Address Code Statements
