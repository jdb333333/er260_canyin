//
// ASM_MACR.H
// ----------
//
// Contiene la definizione di macro in assembler
// ---------------------------------------------
//






// asm_MemSet()
// ------------
//
// Fa il fill su un buffer near
//
// Note : la lunghezza da trasferire pu\xF2 essere = 0
//Ccr // Note : la lunghezza da trasferire può essere = 0
//        per sicurezza, si dovrebbe salvare i registri usati (togli i commenti)
//
static void asm_MemSet(void *pdest, BYTE value, WORD len);
#pragma __ASMMACRO asm_MemSet(A1,R0L,R3)

#pragma ASM
_asm_MemSet	.macro
;;	pushm	R3,A1
	sstr.b
;;	popm	R3,A1
	.endm
#pragma ENDASM




// asm_MemSet_retd()
// ---------------
//
// Fa il fill su un buffer near, tornando pointer a nex byte destinazione
//
// Note : la lunghezza da trasferire pu\xF2 essere = 0
//Ccr // Note : la lunghezza da trasferire può essere = 0
//        per sicurezza, si dovrebbe salvare i registri usati (togli i commenti)
//
static BYTE* asm_MemSet_retd(void *pdest, BYTE value, WORD len);
#pragma __ASMMACRO asm_MemSet_retd(A1,R0L,R3)

#pragma ASM
_asm_MemSet_retd	.macro
;;	pushm	R3,A1
	sstr.b
	mov.w	A1,R0
;;	popm	R3,A1
	.endm
#pragma ENDASM



// asm_memcpy()
// ------------
//
// Fa la copia near->near, ritorna pointer a primo byte destinazione (come la memcpy standard)
//
// Note : la lunghezza da trasferire pu\xF2 essere = 0
//Ccr // Note : la lunghezza da trasferire può essere = 0
//        per sicurezza, si dovrebbe salvare i registri usati (togli i commenti)
//

static BYTE* asm_memcpy(void *pdest, void *psrc, WORD len);
#pragma __ASMMACRO asm_memcpy(A1,A0,R3)

#pragma ASM
_asm_memcpy	.macro
;;	pushm	R1,R3,A0,A1
	mov.w	A1,R0
	mov.b	#0,R1H
	smovf.b
;;	popm	R1,R3,A0,A1

;;;	jsrs	#200	;<-- in ALTERNATIVA : chiama la _uti_nnmemcpy !!!!

	.endm
#pragma ENDASM







// asm_fnmemcpy()
// ------------
//
// Fa la copia far->near, ritorna pointer a primo byte destinazione (come la memcpy standard)
//
// Note : la lunghezza da trasferire pu\xF2 essere = 0
//Ccr // Note : la lunghezza da trasferire può essere = 0
//        per sicurezza, si dovrebbe salvare i registri usati (togli i commenti)
//
static BYTE* ASM_FNMEMCPY(WORD pdest, BYTE psrch, WORD psrcl, WORD len);
#pragma __ASMMACRO ASM_FNMEMCPY(A1,R1H,A0,R3)

#pragma ASM
_ASM_FNMEMCPY	.macro
;;	pushm	R1,R3,A0,A1
	mov.w	A1,R0
	smovf.b
;;	popm	R1,R3,A0,A1

;;	jsrs	#201	;<-- in ALTERNATIVA : chiama la _uti_fnmemcpy !!!!

	.endm
#pragma ENDASM
#define asm_fnmemcpy(dest, src, size) (ASM_FNMEMCPY((WORD)dest, (BYTE)((ULONG)src >> 16), (WORD)((ULONG)src & 0xFFFF), (WORD)size))




// asm_memcpy_retd()
// -----------------
//
// Fa la copia near->near, ritorna pointer pointer a next byte destinazione
//
// Note : la lunghezza da trasferire pu\xF2 essere = 0
//Ccr // Note : la lunghezza da trasferire può essere = 0
//        per sicurezza, si dovrebbe salvare i registri usati (togli i commenti)
//
static BYTE* asm_memcpy_retd(BYTE *pdest, BYTE *psrc, WORD len);
#pragma __ASMMACRO asm_memcpy_retd(A1,A0,R3)

#pragma ASM
_asm_memcpy_retd	.macro
;;	pushm	R1,R3,A0,A1
	mov.w	#0,R1
	smovf.b
	mov.w	A1,R0
;;	popm	R1,R3,A0,A1
	.endm
#pragma ENDASM



// asm_memcpy_rets()
// -----------------
//
// Fa la copia near->near, ritorna pointer a next byte sorgente
//
// Note : la lunghezza da trasferire pu\xF2 essere = 0
//Ccr // Note : la lunghezza da trasferire può essere = 0
//        per sicurezza, si dovrebbe salvare i registri usati (togli i commenti)
//
static BYTE* asm_memcpy_rets(BYTE *pdest, BYTE *psrc, WORD len);
#pragma __ASMMACRO asm_memcpy_rets(A1,A0,R3)

#pragma ASM
_asm_memcpy_rets	.macro
;;	pushm	R1,R3,A0,A1
	mov.w	#0,R1
	smovf.b
	mov.w	A0,R0
;;	popm	R1,R3,A0,A1
	.endm
#pragma ENDASM



// asm_add()
// --------
//
// Somma (algebricamente) il valore di un totalizzatore ad un altro di uguale lunghezza e #decimali, e
// gestendo l'overflow sul risultato.
// Questa routine \xE8 ottimizzata in velocita' rispetto alla routine generale Calc_Tot()
//
// Inputs :
//               *pdest    =  pointer a totalizzatore risultato/addendo
//               *padd     =  pointer a totalizzatore addendo
//               BYTE len  =  lunghezza dei totalizzatori
//

static BYTE asm_add(void *pdest, void *padd, BYTE len);
#pragma __ASMMACRO asm_add(A1,A0,R0L)

#pragma ASM
_asm_add	.macro

	dec.b	r0l
	jz		ASMADDL12

	fclr	c

ASMADDL10:

	adc.b	[a0],[a1]
	inc.w		a0
	inc.w		a1
	sbjnz.b	#1,r0l,ASMADDL10

ASMADDL12:

	mov.b	[a1],r1l
	exts.b	r1l
	mov.b	[a0],r0l
	exts.b	r0l

	adc.b	[a0],[a1]
	adc.b	r0h,r1h

	cmp.b	#0,r1h
	jnz		ASMADDL14
	cmp.b	#7Eh,[a1]
	jgtu	ASMADDLOVP
	mov.b	#1,r0l
	rts
ASMADDL14:
	cmp.b	#0FFh,r1h
	jnz		ASMADDLOVP
	cmp.b	#81h,[a1]
	jltu	ASMADDLOVN
	mov.b	#1,r0l
	rts

ASMADDLOVP:
ASMADDLOVN:
	mov.b	#7Fh,[a1]
	mov.b	#0,r0l
	rts

	.endm

#pragma ENDASM
