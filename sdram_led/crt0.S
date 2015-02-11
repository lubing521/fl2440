	AREA crt0,CODE,READONLY
	ENTRY
	IMPORT disable_watch_dog
	IMPORT memsetup
	IMPORT copy_2th_to_sdram
	IMPORT main
	
	ldr sp,=1024*4
	bl  disable_watch_dog
	bl  memsetup
	bl  copy_2th_to_sdram
	
	ldr pc,=on_sdram	
on_sdram
	ldr sp,=0x34000000
	bl  main
	
halt_loop
	b   halt_loop



	END

