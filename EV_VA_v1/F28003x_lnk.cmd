#if defined(_FLASH)
	#define BEGIN		FLASH_BEGIN
	#define CODE		FLASH_BANK0_SEC
	#define DATA		RAMM0 | RAMLS56
#else
	#define BEGIN		RAM_BEGIN
	#define CODE		RAMM0 | RAMGS123 | RAMLS34
	#define	DATA		RAMLS12
#endif
	#define STACK		RAMM1
	#define	CLA			RAMLS0
	#define CLA_DATA	RAMLS12

MEMORY
{
   RAM_BEGIN		: origin = 0x00000000, length = 0x00000002
   FLASH_BEGIN      : origin = 0x00080000, length = 0x00000002
   BOOT_RSVD        : origin = 0x00000002, length = 0x00000126

   RAMM0            : origin = 0x00000128, length = 0x000002D8
   RAMM1            : origin = 0x00000400, length = 0x000003F8
   // RAMM1_RSVD       : origin = 0x000007F8, length = 0x00000008 /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

   RAMLS0           : origin = 0x00008000, length = 0x00000800
   RAMLS12          : origin = 0x00008800, length = 0x00001000
   RAMLS34			: origin = 0x00009800, length = 0x00001000
   RAMLS56          : origin = 0x0000A800, length = 0x00001000
   RAMLS7           : origin = 0x0000B800, length = 0x00000800

   RAMGS0           : origin = 0x0000C000, length = 0x00001000
   RAMGS123         : origin = 0x0000D000, length = 0x00002FF8
   // RAMGS3_RSVD      : origin = 0x0000FFF8, length = 0x00000008 /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */

   BOOTROM          : origin = 0x003F8000, length = 0x00007FC0
   SECURE_ROM       : origin = 0x003F2000, length = 0x00006000

   RESET            : origin = 0x003FFFC0, length = 0x00000002

   /* Flash sectors */
   FLASH_BANK0_SEC	: origin = 0x00080002, length = 0x00FFFE
   // FLASH_BANK0_SEC15_RSVD     : origin = 0x0AFFF0, length = 0x000010  /* Reserve and do not use for code as per the errata advisory "Memory: Prefetching Beyond Valid Memory" */
   FLASH_BANK1_SEC  : origin = 0x00090000, length = 0x00FFFF
   FLASH_BANK2_SEC  : origin = 0x000A0000, length = 0x00FFF0

   CLA1_MSGRAMLOW   : origin = 0x00001480, length = 0x000080
   CLA1_MSGRAMHIGH  : origin = 0x00001580, length = 0x000080
}

SECTIONS
{
	codestart		: >		BEGIN,	ALIGN(4)
	.stack			: >		STACK
	.reset          : >		RESET,	TYPE = DSECT /* not used, */

   	ramgs0			: >		RAMGS0
	ramgs1			: >		RAMGS0

	Cla1ToCpuMsgRAM	: >		CLA1_MSGRAMLOW
	CpuToCla1MsgRAM	: >		CLA1_MSGRAMHIGH

	.cinit			: >		CODE,	ALIGN(4)
	.switch			: >		CODE,	ALIGN(4)
	.text			: >		CODE,	ALIGN(4)

	dclfuncs		: >		CODE

	#if defined(__TI_EABI__)
		.init_array	: >		CODE,	ALIGN(4)
		.const		: >		CODE,	ALIGN(4)

		.sysmem		: >		DATA
		.bss		: >		DATA
		.bss:output	: >		DATA
		.bss:cio	: >		DATA
		.data		: >		DATA
	#else
		.pinit		: >		CODE,	ALIGN(4)
		.econst		: >		CODE,	ALIGN(4)

		.esysmem	: >		DATA
		.ebss		: >		DATA
		.cio		: >		DATA
	#endif

	.scratchpad		: >		CLA_DATA
	.bss_cla		: >		CLA_DATA
	Cla1DataRam		: >		CLA_DATA
	cla_shared		: >		CLA_DATA
	CLADataLS1		: >		CLA_DATA

	#if defined(_FLASH)
	/* FLASH sections */
   .TI.ramfunc      : LOAD = CODE,
					  RUN = DATA,
					  RUN_START(RamfuncsRunStart),
					  RUN_SIZE(RamfuncsRunSize),
              	      RUN_END(RamfuncsRunEnd),
					  LOAD_START(RamfuncsLoadStart),
                 	  LOAD_SIZE(RamfuncsLoadSize),
                	  LOAD_END(RamfuncsLoadEnd),
                  	  ALIGN(4)

    Cla1Prog        : LOAD = CODE,
                      RUN = CLA,
                      RUN_START(Cla1ProgRunStart),
                      LOAD_START(Cla1ProgLoadStart),
                      LOAD_SIZE(Cla1ProgLoadSize),
                      ALIGN(4)

   .const_cla       : LOAD = CODE,
                      RUN = CLA_DATA,
                      RUN_START(Cla1ConstRunStart),
                      LOAD_START(Cla1ConstLoadStart),
                      LOAD_SIZE(Cla1ConstLoadSize),
                      ALIGN(4)

    CLA1mathTables	: LOAD = CODE,
                      RUN = CLA_DATA,
                      RUN_START(CLA1mathTablesRunStart),
                      RUN_SIZE(CLA1mathTablesRunSize),
                      LOAD_START(CLA1mathTablesLoadStart),
                      LOAD_SIZE(CLA1mathTablesLoadSize),
                      ALIGN(4)
	#else
	/* RAM sections */
	.TI.ramfunc		: >		CODE,	ALIGN(4)

	Cla1Prog		: >		CLA
	.const_cla		: >		CLA_DATA
    CLA1mathTables :	LOAD = CLA_DATA,
                    	RUN = CLA_DATA,
                    	LOAD_START(CLA1mathTablesLoadStart),
                   		RUN_START(CLA1mathTablesRunStart),
                    	LOAD_SIZE(CLA1mathTablesLoadSize)
	#endif
}
