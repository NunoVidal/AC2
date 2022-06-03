
	.equ printStr,8
	.equ printInt10,7
	.equ printInt,6
	.equ readInt10,5
	.data
msg1: 	.asciiz "\n Introduza um inteiro (sinal e modulo): "
msg2:	.asciiz "\n Valor em base 10(signed): "
msg3:	.asciiz "\n Valor em base 2: "
msg4:	.asciiz "\n Valor em base 16: "
msg5:	.asciiz "\n Valor em base 10 unsigned"
msg6:	.asciiz "\n Valor em base 10, unsigned formatado"
	.text
	.globl main
main:

while:
	la $a0,msg1 #printStr("Introduza um inteiro")
	li $v0,printStr
	syscall
	
	li $v0,readInt10
	syscall
	
	move $t1,$v0	#value = $t1
	
	la $a0,msg2 #printStr("Valor em base 10 (signed)")
	li $v0,printStr
	syscall

	move $a0,$t1
	li $v0,printInt10
	syscall


	la $a0,msg3 #printStr("valor em base 2")
	li $v0,printStr
	syscall

	move $a0,$t1
	li $a1,2
	li $v0,printInt
	syscall	


	la $a0,msg4 #printStr(Valor em base 16)
	li $v0,printStr
	syscall
	
	move $a0,$t1
	li $a1,16
	li $v0,printInt
	syscall

	la $a0,msg5 #printStr(Valor base 10 unsigned)
	li $v0,printStr
	syscall
	
	move $a0,$t1
	li $a1,10
	li $v0,printInt
	syscall
	
	la $a0,msg6 #printStr unsigned 10 formatado
	li $v0,printStr
	syscall

	li $t2,5
	sll $t2,$t2,16
	ori $t2,$t2,10

	move $a0,$t1
	move $a1, $t2
	li $v0,printInt
	syscall
	j while
endwhile:


	li $v0,0
	jr $ra
