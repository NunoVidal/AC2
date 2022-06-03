
	.equ READ_CORE_TIMER,11
	.equ RESET_CORE_TIMER,12
	.equ PUT_CHAR,3
	.equ PRINT_INT, 6
	.data
	.text
	.globl main
main:	

	addiu $sp,$sp,-12
	sw $s1,-8($sp)
	sw $s0,-4($sp)
	sw $ra,0($sp)

	li $s0,0 #counter = 0

while:
	li $v0,RESET_CORE_TIMER
	syscall
whiletimer: li $v0,READ_CORE_TIMER
	    syscall
	    move $t1,$v0
	   bge $t1,200000,endwhiletimer # 100Hz

	  move $a0,40000
	 move $a1,'0'
	jal timedone
 
	   j whiletimer
endwhiletimer:	

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

	sw $s1,-8($sp)
	lw $s0,-4($sp)
	lw $ra,0($sp)
	addiu $sp,$sp,12
	
	li $v0,0
	jr $ra


timedone:
	li $t0,0 # retValue = 0

if: ble $a1,0,else	#reset > 0
	li $v0,RESET_CORE_TIMER
	syscall
	j endif
else:
	li $v0,READ_CORE_TIMER
	syscall
	move $t1,$v0	#curCount = READ_CORE_TIMER
	multu $t3,$a1,20000
ifcur: bleu $t1,$t2,endifcur # if curCount > (K*ms)
	
	divu $t0,$t1,20000
endifcur:

endif:

	move $v0,$t0
	jr $ra







