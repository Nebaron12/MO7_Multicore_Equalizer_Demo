#include "xtmrctr.h"

XTmrCtr_Config XTmrCtr_ConfigTable[] __attribute__ ((section (".drvcfg_sec"))) = {

	{
		"xlnx,axi-timer-2.0", /* compatible */
		0x42800000, /* reg */
		"1e+08", /* clock-frequency */
		0x4021, /* interrupts */
		0xf8f01000 /* interrupt-parent */
	},
	{
		"xlnx,axi-timer-2.0", /* compatible */
		0x42810000, /* reg */
		"1e+08", /* clock-frequency */
		0x4022, /* interrupts */
		0xf8f01000 /* interrupt-parent */
	},
	{
		"xlnx,axi-timer-2.0", /* compatible */
		0x42820000, /* reg */
		"1e+08", /* clock-frequency */
		0x4023, /* interrupts */
		0xf8f01000 /* interrupt-parent */
	},
	 {
		 NULL
	}
};