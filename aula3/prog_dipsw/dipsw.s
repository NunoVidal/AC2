
	.equ SFR_BASE_HIGH,0xBF88
	.equ TRISE,0x6100
	.equ TRISB,0x6040
	.equ LATE,0x6120
	.equ LATB,0x6060
	.equ PORTE,0x6110
	.equ PORTB,0x6050
	.data
	.text
	.globl main
main:


	#-----------------------configure ports I/O-------------------------
	lui $t0,SFR_BASE_HIGH	
	lw $t1,TRISE($t0)
	andi $t1,$t1,0xFFFE	#RE0 :out -> 0
	sw $t1,TRISE($t0)


	lw $t1,TRISB($t0)
	ori $t1,$t1,0x0001 	#RB0: in ->1
	sw $t1,TRISB($t0)	
	#-----------------------end config I/O ----------------------------------------
while:
	lw $t1,PORTB($t0)
	andi $t1,$t1,0x0001	#reset all bits except 0
	xori $t1,$t1,1 #negar rb0

	lw $t2,LATE($t0)
	andi $t2,$t2,0xFFFE	#reset bit 0 to write
	or $t2,$t2,$t1
	
	sw $t2,LATE($t0)	#write LATe

	j while

	li $v0,0
	jr $ra
