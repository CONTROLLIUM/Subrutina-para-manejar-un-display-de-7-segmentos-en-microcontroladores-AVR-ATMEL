;***************************************************************************
; SUBRUTINA DE ESCRITURA EN DISPLAY DE 7 SEGMENTOS, UN DIGITO (DE $00 A $0F)
;
; REALIZADA POR ING. C.N. JOSUE NORBERTO MARTINEZ F. (MAYO 2020)
;				produccionesrek@gmail.com
;				http://prdiagramasyelectronica.blogspot.com/
;
; rev. 1 (sep. 2020)
; rev. 2 (ene. 2021) libera el bit 7 del registro DDRX correspondiente a PORTX_7SEG 
;***************************************************************************
;
; EL CODIGO DE ESTAS SUBRUTINAS PUEDE SER AGREGADO AL CODIGO DEL PROGRAMA 
; PRINCIPAL O PUEDE SER INCLUIDO UTILIZANDO EL COMANDO ".INCLUDE"
;
;  EJEMPLO:
;  .INCLUDE "SUBRUTINA_7SEGMENTOS_1DIGITO.INC"
;
;
; PARA EJECUTAR ESTA SUBRUTINA SE DEBEN SEGUIR LOS SIGUIENTES PASOS:
;
;	1.-	DEFINIR EL REGISTRO "NUMERO_7SEG" QUE SERÁ UTILIZADO POR LA SUBRUTINA
;
;	2.-	DEFINIR "PORTX_7SEG" QUE CORRESPONDEN AL PUERTO EN 
;		EL QUE SE ENCUENTRA CONECTADO EL DISPLAY DE 7 SEGMENTOS:	
;
;	3.-	DEFINIR "LIMITE_7SEG" QUE INDICA EL MAXIMO VALOR QUE MOSTRARÁ EL DISPLAY
;		EL VALOR MAXIMO PERMITIDO ES $0F					
;
;	4.-	DEFINIR EL NÚMERO QUE SE QUIERE MOSTRAR A TRAVÉS DEL REGISTRO "NUMERO_7SEG"
;
;	5.-	UTILIZAR EL COMANDO "RCALL" PARA EJECUTAR LA SUB-RUTINA
;
;	6.-	NO ES NECESARIO CONFIGURAR EL PUERTO DDR, YA QUE LA CONFIGURACION DE ENTRADAS Y SALIDAS
;		PARA EL DISPLAY DE 7 SEGMENTOS SE ENCUENTRA INCLUIDA EN ESTA SUBRUTINA.
;
; NOTAS:
;
; SI SE COLOCA UN NUMERO FUERA DEL RANGO DE 0 A "LIMITE_7SEG", SE MOSTRARÁ UNA "H" EN EL DISPLAY 
; 
; ESTA  SUBRUTINA  NO ALTERA LOS VALORES ALMACENADOS EN LOS REGISTROS
; DE PROPÓSITO GENERAL (R0 A R31)
;
;---------------------------------------------------------------------
;--------------- EJEMPLO DE IMPLEMENTACION:---------------------------
;---------------------------------------------------------------------
;
;	.DEF NUMERO_7SEG	=	R16		;DEFINE EL REGISTRO QUE UTILIZARÁ LA SUBRUTINA
;	.EQU PORTX_7SEG		=	PORTD	;DEFINE EL PUERTO (PORTX) DONDE SE ENCUENTRA EL DISPLAY
;	.EQU LIMITE_7SEG	=	15		;VALOR MAXIMO DEL DISPLAY = 15 ($0F)
;
;	RCALL INICIALIZA_7SEG		;INICIALIZA LA SUBRUTINA
;
;	MAIN:
;	
;	LDI NUMERO_7SEG, 3			;ASIGNA EL NÚMERO (del 0 al 15, o del $00 al $0F) PARA MOSTRAR EN EL DISPLAY DE 7 SEGMENTOS
;	RCALL DISPLAY_7SEG			;EJECUTA LA SUBRUTINA, MUESTRA EL NUMERO EN EL DISPLAY
;	
;	RJMP MAIN
;
;	.INCLUDE "SUBRUTINA_7SEGMENTOS_1DIGITO.INC"
;
;****************************************************************************************************
; SEGMENTO	|	PUERTO AVR
; -------------------------
;	a	|	PX0
;	b	|	PX1
;	c	|	PX2
;	d	|	PX3
;	e	|	PX4
;	f	|	PX5
;	g	|	PX6

;****************************************************************************************************

TABLA_7SEG:
;DÍGITOS	0	1	2	3	4	5	6	7	8	9	A	b	C	d	E	F	H
.DB		$3F, $06, $5B, $4F, $66, $6D, $7D, $07, $7F, $6F, $77, $7C, $39, $5E, $79, $71, $76, 0

;****************************************************************************************************
;ADVERTENCIA PARA EL COMPILADOR

.IF LIMITE_7SEG > 15
.ERROR " *LIMITE_7SEG* Debe ser igual o menor a 15"
.ENDIF

;****************************************************************************************************
;CODIGO


INICIALIZA_7SEG:
	
	PUSH NUMERO_7SEG
	
	IN NUMERO_7SEG, PORTX_7SEG - 1
	SBR NUMERO_7SEG, 0b0111_1111
	OUT PORTX_7SEG - 1, NUMERO_7SEG

	POP NUMERO_7SEG

RET


DISPLAY_7SEG:

	PUSH ZL
	PUSH ZH
	PUSH R0
	PUSH NUMERO_7SEG
	
	CLR R0

	CPI NUMERO_7SEG, LIMITE_7SEG + 1
	BRLO MUESTRA_DIGITO
	
	LDI NUMERO_7SEG, 16
		
	MUESTRA_DIGITO:

		LDI ZH, HIGH(TABLA_7SEG<<1)
		LDI ZL, LOW(TABLA_7SEG<<1)
		ADD ZL, NUMERO_7SEG
		ADC ZH, R0
		LPM NUMERO_7SEG, Z+		
		OUT PORTX_7SEG, NUMERO_7SEG

	POP NUMERO_7SEG
	POP R0
	POP ZH
	POP ZL

RET

;****************************************************************************************************
