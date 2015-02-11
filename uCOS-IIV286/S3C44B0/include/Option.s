;**********OPTIONS*******************************
;_RAM_STARTADDRESS  EQU   0xc000000
_ISR_STARTADDRESS   EQU   0xc7fff00 ;GCS6:64M DRAM/SDRAM
;_ISR_STARTADDRESS  EQU   0xc1fff00 ;GCS6:16M DRAM


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
PLLCLK		SETA	66000000    ;40000000

	[	PLLCLK = 40000000	;Fin=10MHz Fout=40MHz
M_DIV	EQU	0x48
P_DIV	EQU	0x3
S_DIV	EQU	0x2		
	]

	[	PLLCLK = 50000000	;Fin=10MHz Fout=50MHz
M_DIV	EQU	0x2a
P_DIV	EQU	0x3
S_DIV	EQU	0x1		
	]

	[	PLLCLK = 60000000	;Fin=10MHz Fout=60MHz
M_DIV	EQU	0x34
P_DIV	EQU	0x3
S_DIV	EQU	0x1		
	]

; wwf changed
	[	PLLCLK = 66000000	;Fin=10MHz Fout=66MHz
M_DIV	EQU	0x3a
P_DIV	EQU	0x3
S_DIV	EQU	0x1		
	]


	[	PLLCLK = 75000000	;Fin=10MHz Fout=75MHz
M_DIV	EQU	0x43
P_DIV	EQU	0x3
S_DIV	EQU	0x1		
	]
;************************************************
	END