**PROCEDURE: foo_
**BEGIN: Three Address Code Statements
	stemp0 = 50
	goto Label2
Label2:
	 return stemp0
**END: Three Address Code Statements
**PROCEDURE: main
**BEGIN: Three Address Code Statements
	temp0 = 1 > 2
	temp1 = ! temp0
	if(temp1) goto Label4
	stemp0 = 10
	goto Label1
	goto Label3
Label4:
	temp2 = 3 < 4
	temp3 = ! temp2
	if(temp3) goto Label6
	stemp0 = 20
	goto Label1
	goto Label5
Label6:
	temp4 = 4 < 6
	temp5 = ! temp4
	if(temp5) goto Label7
	stemp0 = 30
	goto Label1
	goto Label7
Label7:
Label5:
Label3:
Label1:
	 return stemp0
**END: Three Address Code Statements
**PROCEDURE: zar_
**BEGIN: Three Address Code Statements
	stemp0 = 100
	goto Label0
Label0:
	 return stemp0
**END: Three Address Code Statements
