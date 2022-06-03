
	.equ SFR_BASE_HIGH,0xBF88
	.equ TRISE,0x6100
	.equ TRISD,0x60C0
	.equ LATE,0x6120
	.equ LATD,0x60E0
	.equ PORTE,0x6110
	.equ PORTD,0x60D0
	.data
	.text
	.globl main
main:


	#-----------------------configure ports I/O-------------------------
	lui $t0,SFR_BASE_HIGH	
	lw $t1,TRISE($t0)
	andi $t1,$t1,0xFFFE	#RE0 :out -> 0
	sw $t1,TRISE($t0)


	lw $t1,TRISD($t0)
	ori $t1,$t1,0x0100 	#RD8: in ->1
	sw $t1,TRISD($t0)	
	#-----------------------end config I/O ----------------------------------------
while:
	lw $t1,PORTD($t0)
	andi $t1,$t1,0x0100	#reset all bits except 8
	srl $t1,$t1,8
	
	lw $t2,LATE($t0)
	andi $t2,$t2,0xFFFE	#reset bit 8 to write
	or $t2,$t2,$t1
	
	sw $t2,LATE($t0)	#write LATe

	j while

	li $v0,0
	jr $ra
