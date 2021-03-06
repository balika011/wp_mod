/*
 * drivers/w1/slaves/w1_bq2022.c
 *
 * Copyright (C) 2014 Aaron Segaert (flar2)
 * Copyright (C) 2014 Mohammad Abu-Garbeyyeh (MohammadAG)
 * Copyright (C) 2015 Balázs Triszka (balika011) <balika011@protonmail.ch>
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/kallsyms.h>
#include <asm/mmu_writeable.h>

void hook_func(const char* name, const void *new)
{
	int i;
	unsigned long addr = kallsyms_lookup_name(name);
	unsigned long hook_arm_asm[3] = {
		0xE59FF000,
		0x00000000,
		0x00000000
	};
    	hook_arm_asm[1] = hook_arm_asm[2] = (unsigned long)new;

	for(i = 0; i < sizeof(hook_arm_asm) / 4; ++i)
		mem_text_write_kernel_word((unsigned long *)(addr + i * 4), hook_arm_asm[i]);
}

static int sony_ric_enabled(void)
{
	return 0;
}

static int __init wp_mod_init(void)
{
	hook_func("sony_ric_enabled", &sony_ric_enabled);

	return 0;
}
module_init(wp_mod_init)

MODULE_AUTHOR("flar2, MohammadAG, balika011");
MODULE_DESCRIPTION("Defeat system write protect");
MODULE_VERSION("2.0");
MODULE_LICENSE("GPL");
