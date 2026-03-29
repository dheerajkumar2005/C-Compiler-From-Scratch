**PROCEDURE: main
**BEGIN: Three Address Code Statements
Label15:
	temp0 = ! ba_
	if(temp0) goto Label0
	a_ = b_
	goto Label0
Label0:
	temp3 = ! ba_
	if(temp3) goto Label1
	temp1 = c_ + d_
	stemp0 = temp1
	goto Label2
Label1:
	temp2 = c_ - d_
	stemp0 = temp2
Label2:
	a_ = stemp0
	temp4 = a_ < b_
	temp13 = ! temp4
	if(temp13) goto Label8
	temp5 = e_ < f_
	temp9 = ! temp5
	if(temp9) goto Label3
	temp6 = b_ + g_
	stemp1 = temp6
	goto Label4
Label3:
	temp7 = c_ * g_
	temp8 = temp7 + d_
	stemp1 = temp8
Label4:
	temp10 = i_ * stemp1
	temp11 = - temp10
	write  temp11
	temp12 = ! ba_
	if(temp12) goto Label5
	stemp2 = b_
	goto Label6
Label5:
	stemp2 = c_
Label6:
	a_ = stemp2
	write  1
	goto Label7
Label8:
	temp14 = c_ < d_
	temp15 = bb_ || temp14
	temp21 = ! temp15
	if(temp21) goto Label14
	temp19 = ! bc_
	if(temp19) goto Label9
	temp16 = e_ + f_
	stemp3 = temp16
	goto Label10
Label9:
	temp17 = e_ * j_
	temp18 = d_ - temp17
	stemp3 = temp18
Label10:
	g_ = stemp3
	temp20 = ! bb_
	if(temp20) goto Label11
	stemp4 = c_
	goto Label12
Label11:
	stemp4 = d_
Label12:
	b_ = stemp4
	goto Label13
Label14:
	h_ = i_
Label13:
Label7:
	temp22 = a_ < b_
	temp23 = temp22 || bd_
	temp24 = bc_ && temp23
	temp25 = bb_ || temp24
	if(temp25) goto Label15
**END: Three Address Code Statements
