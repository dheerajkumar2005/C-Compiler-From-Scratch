**PROCEDURE: main
**BEGIN: Three Address Code Statements
	a_ = 20
	temp0 = a_ / 2
	temp1 = 5 * temp0
	temp2 = temp1 - 30
	b_ = temp2
	temp3 = a_ > b_
	temp6 = ! temp3
	if(temp6) goto Label0
	temp4 = a_ - b_
	stemp0 = temp4
	goto Label1
Label0:
	temp5 = a_ + b_
	stemp0 = temp5
Label1:
	c_ = stemp0
**END: Three Address Code Statements
