;**********OPTIONS*******************************
_RAM_STARTADDRESS  EQU   0xc000000
;_ISR_STARTADDRESS   EQU   0xc7fff00 ;GCS6:64M DRAM/SDRAM
_ISR_STARTADDRESS  EQU   0xc1fff00 ;GCS6:16M DRAM


;BUSWIDTH; 16,32
                GBLA    BUSWIDTH
BUSWIDTH	SETA    16


;"DRAM","SDRAM"
                GBLS    BDRAMTYPE
BDRAMTYPE	SETS    "SDRAM"


;This value has to be TRUE on ROM program.
;This value has to be FALSE in RAM program.
                GBLL    PLLONSTART
PLLONSTART      SETL    {TRUE}

	GBLA	PLLCLK
PLLCLK		SETA	40000000

	[	PLLCLK = 40000000	
M_DIV	EQU	0x48	;Fin=10MHz Fout=40MHz
P_DIV	EQU	0x3
S_DIV	EQU	0x2		
	]

	[	PLLCLK = 50000000	
M_DIV	EQU	0x2a	;Fin=10MHz Fout=50MHz
P_DIV	EQU	0x3
S_DIV	EQU	0x1	
	]

	[	PLLCLK = 60000000	
M_DIV	EQU	0x34	;Fin=10MHz Fout=60MHz
P_DIV	EQU	0x3
S_DIV	EQU	0x1
;M_DIV	EQU	0x34	;Fin=4MHz Fout=60MHz
;P_DIV	EQU	0x0
;S_DIV	EQU	0x1
;M_DIV	EQU	0x48	;Fin=3MHz Fout=60MHz
;P_DIV	EQU	0x0
;S_DIV	EQU	0x1
	]

	[	PLLCLK = 75000000	
M_DIV	EQU	0x43	;Fin=10MHz Fout=75MHz
P_DIV	EQU	0x3
S_DIV	EQU	0x1		
	]
;************************************************
	END