

	.equ getChar,2
	.equ putChar,3
	.data
	.text
	.globl main
main:
#c = $t1
do:
	li $v0,getChar
	syscall
	move $t1,$v0

	addiu $t2,$t1,1
	move $a0,$t2
	li $v0,putChar
	syscall

while: bne $t1,'\n', do
	


	li $v0,0 #return 0
	jr $ra


