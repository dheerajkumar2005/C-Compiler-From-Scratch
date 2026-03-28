**PROCEDURE: main
**BEGIN: Three Address Code Statements
	temp0 = x_ < y_
	temp1 = ! temp0
	if(temp1) goto Label1
	z_ = 10
	goto Label0
Label1:
	temp2 = w_ < x_
	temp3 = ! temp2
	if(temp3) goto Label3
	z_ = 20
	goto Label2
Label3:
	z_ = 30
Label2:
Label0:
**END: Three Address Code Statements
