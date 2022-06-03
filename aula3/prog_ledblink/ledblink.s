

	.equ SR_BASE_HIGH,0xBF88
	.equ TRISE,0x6100
	.equ LATE,0x6120	
	.equ TRISD,0x60C0
	.equ LATD,0x60E0
	.equ RESET_CORE_TIMER,12
	.equ READ_CORE_TIMER,11
	.data
	.text
	.globl main
main:
	li $s0,1 # v = 0

	lui $s1,SR_BASE_HIGH

	lw $t2,TRISE($s1)
	andi $t2,$t2,0xFFFE #TRISE0 = 0
	sw $t2,TRISE($s1)
	
	lw $t2,TRISD($s1)
	andi $t2,$t2,0xFFFE #TRISE0 = 0
	sw $t2,TRISD($s1)
while:
	
	lw $t2,LATE($s1)
	andi $t2,$t2,0xFFFE #reset LATE0 bit
	or $t2,$t2,$s0 #LATE0 = v
	sw $t2,LATE($s1)

	lw $t2,LATD($s1)
	andi $t2,$t2,0xFFFE #reset LATE0 bit
	or $t2,$t2,$s0 #LATE0 = v
	sw $t2,LATD($s1)
	

	li $a0,500
	jal delay	
	#insert delay code
	
	xori $s0,$s0,1 #nega v

	j while

	jr $ra

delay:
	 li $v0,RESET_CORE_TIMER
         syscall
whiledelay: li $v0,READ_CORE_TIMER
	syscall
	move $t0,$v0
	li $t1,20000
	multu $t1,$a0        #K*ms
	mflo $t1  
	bgeu $t0,$t1,endwhiledelay #readcoretimer() < K*ms 
	j whiledelay
endwhiledelay:

	jr $ra
