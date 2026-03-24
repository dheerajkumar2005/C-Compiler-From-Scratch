**PROCEDURE: main
**BEGIN: Three Address Code Statements
	temp0 = x_ < 20
	b1_ = temp0
	temp1 = b1_ && b2_
	temp2 = temp1 || b3_
	temp5 = ! temp2
	if(temp5) goto Label1
	temp3 = x_ * 2
	x_ = temp3
	temp4 = x_ - 9
	x_ = temp4
	goto Label0
Label1:
	temp6 = y_ - 236
	z_ = temp6
	temp7 = 2 * y_
	z_ = temp7
Label0:
**END: Three Address Code Statements
