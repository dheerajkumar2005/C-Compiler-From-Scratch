**PROCEDURE: bar_
**BEGIN: Three Address Code Statements
	temp0 = 1 > 2
	temp1 = ! temp0
	if(temp1) goto Label3
	stemp1 = 10
	goto Label4
Label3:
	stemp1 = 20
Label4:
	stemp0 = stemp1
	goto Label2
Label2:
	 return stemp0
**END: Three Address Code Statements
**PROCEDURE: foo_
**BEGIN: Three Address Code Statements
	temp0 = 1 > 2
	temp1 = ! temp0
	if(temp1) goto Label5
	stemp1 = 20.00
	goto Label6
Label5:
	stemp1 = 15.00
Label6:
	stemp0 = stemp1
	goto Label1
Label1:
	 return stemp0
**END: Three Address Code Statements
**PROCEDURE: main
**BEGIN: Three Address Code Statements
	temp0 = 1 > 2
	temp1 = ! temp0
	if(temp1) goto Label7
	stemp0 = 10
	goto Label8
Label7:
	stemp0 = 20
Label8:
	a_ = stemp0
**END: Three Address Code Statements
**PROCEDURE: xyz_
**BEGIN: Three Address Code Statements
	temp0 = 1 < 2
	temp1 = ! temp0
	if(temp1) goto Label10
	goto Label9
Label10:
	temp2 = 3 < 4
	temp3 = ! temp2
	if(temp3) goto Label11
	stemp0 = 20
	goto Label0
	goto Label11
Label11:
Label9:
Label0:
	 return stemp0
**END: Three Address Code Statements
