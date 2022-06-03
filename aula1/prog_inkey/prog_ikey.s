	.equ inkey,1
	.equ putChar,3
	.data
	.text
	.globl main
main:

do:
	li $v0,inkey
	syscall
	move $t1,$v0

if: beq $t1,0,else
	move $a0,$t1
	j endif
else:
	li $a0,'.'
endif:
	
	li $v0,putChar
	syscall

while: bne $t1,'\n',do 

	li $v0,0 #return 0
	jr $ra
