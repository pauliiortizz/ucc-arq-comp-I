.text

.global bucle

.extern Olas
.extern Parpadeo



mov r0, #2
mov r1, #1
mov r2, #2
mov r3, #3
mov r4, #4
bucle 	CMP r0,r1
		beq op1
		CMP r0,r2
		beq op2
		cmp r0,r3
		bne exit
op1 
;Olas

BL ping_pong

op2
;Parpadeo
BL pila


exit MOV PC, LR