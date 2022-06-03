
	.equ resetCoreTimer, 12
	.equ readCoreTimer, 11
	.equ TRISE,0x6100 
	.equ TRISB,0x6040
	.equ LATE,0x6120
	.equ PORTB,0x6050
	.equ BASE_HIGH, 0xBF88
	.data
	.text
	.globl main
main:
	lui $s0,BASE_HIGH
	li $s1,0x0
	li $s2, 1

	lw $t0,TRISE($s0)
	andi $t0,$t0,0xFFF0 #definir como output		
	sw $t0,TRISE($s0)

	lw $t0,TRISB($s0)
	ori $t0,$t0,0x0008 #rb3 como input
	sw $t0,TRISB($s0)

while:
	lw $t0,PORTB($s0)
	andi $t0,$t0,0x0008
	
if_op: beq $t0,0x8,else_op
	li $s2, -1
	j end_op
else_op:
	li $s2,1
end_op:

if:	beq $s2,1, else #se 1 contador crescente
	bge $s1,0x0, endif
	
	li $s1,0xF
	j endif 
else: 
	ble $s1,0xF,endif

	li $s1,0x0
endif:
	lw $t0, LATE($s0)
	andi $t0,$t0,0xFFF0
	or $t0,$t0,$s1
	sw $t0,LATE($s0)	
	
	li $a0,500
	jal delay	

	add $s1,$s1,$s2
	j while

	jr $ra
delay:
	li $v0,resetCoreTimer
	syscall

while_delay: li $v0,readCoreTimer
	     syscall
	     move $t0, $v0
	     mul $t1, $a0,20000
	     bge $t0,$t1, end_while_delay
	j while_delay
end_while_delay:	

	jr $ra
