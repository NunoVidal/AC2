

	.equ SR_BASE,0xBF88
	.equ TRISB,0x6040
	.equ PORTB,0x6050
	.equ TRISE,0x6100
	.equ LATE,0x6120
	.data
	.text
	.globl main
main:
	lui $s0,SR_BASE
	
	lw $t1,TRISB($s0)
	ori $t1,$t1,0x000F #3..0 a ser 1
	sw $t1,TRISB($s0)
	

	lw $t1,TRISE($s0)
	andi $t1,$t1,0xFF7F #bit 7 é 0
	sw $t1,TRISE($s0)

while:

	lw $t1,PORTB($s0)
	andi $t1,$t1,0x000F 
	
	xori $t1,$t1,0x000F	#inverte bits
	
	li $v0,6
	move $a0,$t1
	li $a1,16 	
	syscall

	lw $t1,LATE($s0)
	xori $t1,$t1,0x0080
	sw $t1,LATE($s0)
	

	j while	

	jr $ra



delay:
	li $v0, 12 #reset Coretimer
	syscall 

while_delay:
	li $v0,11
	syscall
	mul $t0,$a0,20000

	bge $v0,$t0,endwhile_delay


	j while_delay
endwhile_delay:


	jr $ra
