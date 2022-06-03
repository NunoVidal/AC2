
	.equ READ_CORE_TIMER,11
	.equ RESET_CORE_TIMER,12
	.equ PUT_CHAR,3
	.equ PRINT_INT, 6
	.data
	.text
	.globl main
main:	


	addiu $sp,$sp,-8
	sw $s0,-4($sp)
	sw $ra,0($sp)
	
	li $s0,0 #counter = 0

while:
	li $v0,RESET_CORE_TIMER
	syscall
	

	li $a0,214747
	jal delay

	move $a0,$s0
	li $t2,4
	sll $t2,$t2,16
        ori $t2,$t2,10
	move $a1,$t2    #10 | 4 <<16
	li $v0,PRINT_INT	
	syscall
	
	addi $s0,$s0,1 #counter ++

	li $a0,'\r'
	li $v0,PUT_CHAR
	syscall
	
	j while
endwhile:

	lw $s0,-4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,8
	
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
