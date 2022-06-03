

	.equ getChar,2
	.equ BASE_HIGH,0xBF88
	.equ TRISE,0x6100
	.equ LATE,0x6120
	.equ readCoreTimer,11
	.equ resetCoreTimer,12
	.data
	.text
	.globl main
main:

	lui $s0,BASE_HIGH
	
	lw $t0,TRISE($s0)
	andi $t0,$t0,0xFFF0
	sw $t0,TRISE($s0)


while:
	
	li $v0,getChar
	syscall
	move $t0,$v0

if: bne $t0,'0',else1
	
	li $t1,1

	j endif
else1: bne $t0,'1',else2
	
	li $t1,2

	j endif
else2: bne $t0,'2',else3

	li $t1,4
	j endif
else3: bne $t0,'3',else
	
	li $t1,8
	
	j endif
else: bne $t0,'4', def
	li $t1,0xF
	j endif
def:
	li $t1,0x0
endif:


	lw $t0,LATE($s0)
	andi $t0,$t0,0xFFF0
	or $t0,$t0,$t1
	sw $t0,LATE($s0)

if_de: bne $t1,0xF,end_de
	li $a0,1000
	jal delay

	lw $t0,LATE($s0)
	andi $t0,$t0,0xFFF0
	sw $t0,LATE($s0)
end_de:	
	

	j while


	jr $ra


delay:
	li $v0, resetCoreTimer
	syscall
	
	li $t0, 20000
	mul $t0,$t0,$a0
	

while_delay:
	 
	li $v0,readCoreTimer
	syscall
	move $t1,$v0
	bge $t1,$t0,end_whiledelay

	j while_delay
end_whiledelay:

	jr $ra
