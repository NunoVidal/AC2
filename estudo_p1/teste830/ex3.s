	
	.equ TRISE,0x6100
	.equ BASE,0xBF88
	.equ LATE,0x6120
	.data
	.text
	.globl main
main:

	lui $s1,BASE

	lw $t0,TRISE($s1)
	andi $t0,$t0,0xFFE1
	sw $t0,TRISE($s1)
	
	lw $t0,LATE($s1)
	andi $t0,$t0,0xFFE1
	li $t1,18
	or $t0,$t0,$t1
	sw $t0,LATE($s1)

while:
	lw $t0,LATE($s1)
	xori $t1,$t0,0x001E
	sw $t1,LATE($s1)
	
	move $a0,$t1
	li $a1,2
	li $t2,4
	sll $t2,$t2,16
	or $a1,$a1,$t2
	li $v0,6
	syscall



	li $a0,142
	jal delay


	j while

	jr $ra

delay:
	li $v0,12
	syscall

whiledelay:
	li $v0,11
	syscall
	mul $t0,$a0,20000
	bgeu $v0,$t0,endwhiledelay

	j whiledelay
endwhiledelay:


	jr $ra
