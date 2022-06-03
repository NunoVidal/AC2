
	.equ BASE_HIGH, 0xBF88
	.equ TRISE,0x6100
	.equ TRISB,0x6040
	.equ PORTB,0x6050
	.equ LATE,0x6120
	.data
	.text
	.globl main
main:
	
	lui $s0, BASE_HIGH

	lw $t0,TRISE($s0)
	andi $t0,$t0,0xFFF0 #re3 a r0 como output
	sw $t0,TRISE($s0)


	lw $t0, TRISB($s0)
	ori $t0,$t0,0x000F #rb3 a rb0 como inp
	sw $t0,TRISB($s0)

while:

	lw $t0, PORTB($s0)
	andi $t0,$t0,0x000F
	
	lw $t1,LATE($s0)
	andi $t1,$t1,0xFFF0
	or $t1, $t1,$t0
	sw $t1,LATE($s0)


	j while



	jr $ra
