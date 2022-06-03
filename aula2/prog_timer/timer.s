
	.equ READ_CORE_TIMER,11
	.equ RESET_CORE_TIMER,12
	.equ PUT_CHAR,3
	.equ PRINT_INT, 6
	.data
	.text
	.globl main
main:	li $t0,0 #counter = 0



while:
	li $v0,RESET_CORE_TIMER
	syscall
whiletimer: li $v0,READ_CORE_TIMER
	    syscall
	    move $t1,$v0
	#   bge $t1,200000,endwhiletimer 100Hz
	#   bge $t1,2000000,endwhiletimer #10Hz
	# bge $t1,4000000,endwhiletimer #5Hz
	bge $t1,20000000,endwhiletimer #1Hz
	    
	   j whiletimer
endwhiletimer:	

	move $a0,$t0
	li $t2,4
	sll $t2,$t2,16
        ori $t2,$t2,10
	move $a1,$t2    #10 | 4 <<16
	li $v0,PRINT_INT	
	syscall
	
	addi $t0,$t0,1 #counter ++

	li $a0,'\r'
	li $v0,PUT_CHAR
	syscall
	
	j while
endwhile:

	li $v0,0
	jr $ra

