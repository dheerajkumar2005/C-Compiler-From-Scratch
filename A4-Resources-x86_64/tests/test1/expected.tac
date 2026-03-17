**PROCEDURE: main
**BEGIN: Three Address Code Statements
	read  x_
	temp0 = - x_
	y_ = temp0
	temp1 = a_ / 2
	temp2 = 5 * temp1
	temp3 = temp2 - 30
	b_ = temp3
	temp4 = a_ > b_
	temp7 = ! temp4
	if(temp7) goto Label0
	temp5 = a_ - b_
	stemp0 = temp5
	goto Label1
Label0:
	temp6 = a_ + b_
	stemp0 = temp6
Label1:
	c_ = stemp0
	temp8 = c_ * c_
	temp9 = temp8 + b_
	write  temp9
	temp10 = a_ < b_
	temp11 = a_ != b_
	temp12 = temp10 || temp11
	r_ = temp12
	temp13 = a_ > b_
	temp14 = a_ == b_
	temp15 = temp13 && temp14
	r_ = temp15
**END: Three Address Code Statements
