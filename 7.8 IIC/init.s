	AREA |DATA|,CODE,READONLY
	ENTRY
	ldr r13,=0x1000
	IMPORT iicMain
	b iicMain
	END