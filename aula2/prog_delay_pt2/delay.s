
	.equ READ_CORE_TIMER,11
	.equ RESET_CORE_TIMER,12
	.equ PUT_CHAR,3
	.equ PRINT_INT, 6
	.equ IN_KEY,1
	.data
	.text
	.globl main
main:	


	addiu $sp,$sp,-32
	sw $s6,-28($sp)
	sw $s5, -24($sp)
	sw $s4,-20($sp)
	sw $s3,-16($sp)
	sw $s0,-12($sp)
	sw $s1,-8($sp)
	sw $s2,-4($sp)
	sw $ra,0($sp)
	
	li $s0,0 #cnt10 = 0
	li $s1,0 #cnt5 = 0
	li $s2,0 #cnt1 = 0
	
	li $s3,100
	li $s4,100
	li $s5, 800
	li $s6,1 #flag para contar
while:

	li $v0,IN_KEY
	syscall
if: bne $v0,'A',else 
	div $s3,$s3,2
	div $s4,$s4,2
	div $s5,$s5,2
	
	j endif
else: bne $v0,'N',elsus
	li $s3,100
	li $s4,100
	li $s5, 800
	j endif
elsus: bne $v0,'S',elsret
	li $s6,0	
	j endif
elsret: bne $v0,'R',endif
	li $s6,1
endif:

ifflag:	bne $s6,1,endifflag
	move $a0,$s3
	jal delay
	
	move $a0,$s0
	li $t2,4
	sll $t2,$t2,16
        ori $t2,$t2,10
	move $a1,$t2    #10 | 4 <<16
	li $v0,PRINT_INT	
	syscall
	addi $s0,$s0,1 #cnt ++

	li $a0,' '
	li $v0,PUT_CHAR
	syscall
	
	move $a0,$s4
	jal delay

	move $a0,$s1
	li $t2,4
	sll $t2,$t2,16
        ori $t2,$t2,10
	move $a1,$t2    #10 | 4 <<16
	li $v0,PRINT_INT	
	syscall
	addi $s1,$s1,1 #cnt ++


	li $a0,' '
	li $v0,PUT_CHAR
	syscall

	move $a0,$s5
	jal delay

	move $a0,$s2
	li $t2,4
	sll $t2,$t2,16
        ori $t2,$t2,10
	move $a1,$t2    #10 | 4 <<16
	li $v0,PRINT_INT	
	syscall
	addi $s2,$s2,1 #cnt ++


	li $a0,'\r'
	li $v0,PUT_CHAR
	syscall
	
endifflag:

	j while
endwhile:

	lw $s6,-28($sp)
	lw $s5, -24($sp)
	lw $s4,-20($sp)
	lw $s3,-16($sp)
	lw $s0,-12($sp)
	lw $s1,-8($sp)
	lw $s2,-4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,32
	
	li $v0,0
	jr $ra


delay:
	li $v0,RESET_CORE_TIMER
	syscall
whiledelay: li $v0,READ_CORE_TIMER
	    syscall
	   move $t0,$v0
	   li $t1,20000
	   multu $t1,$a0	#K*ms
	  mflo $t1  
	bgeu $t0,$t1,endwhiledelay #readcoretimer() < K*ms

	j whiledelay
endwhiledelay:

	jr $ra
