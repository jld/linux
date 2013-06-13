/*
 *  linux/arch/arm/include/asm/perf_event.h
 *
 *  Copyright (C) 2009 picoChip Designs Ltd, Jamie Iles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#ifndef __ARM_PERF_EVENT_H__
#define __ARM_PERF_EVENT_H__

#include <asm/ptrace.h>

/*
 * The ARMv7 CPU PMU supports up to 32 event counters.
 */
#define ARMPMU_MAX_HWEVENTS		32

#define HW_OP_UNSUPPORTED		0xFFFF
#define C(_x)				PERF_COUNT_HW_CACHE_##_x
#define CACHE_OP_UNSUPPORTED		0xFFFF

#ifdef CONFIG_HW_PERF_EVENTS
struct pt_regs;
extern unsigned long perf_instruction_pointer(struct pt_regs *regs);
extern unsigned long perf_misc_flags(struct pt_regs *regs);
#define perf_misc_flags(regs)	perf_misc_flags(regs)
#endif

/*
 * We can't actually get the caller's registers here; the saved PC and
 * SP values have to be consistent or else EHABI unwinding won't work,
 * and the only way to find the matching SP for the return address is
 * to unwind the current function.  So we save the current state
 * instead.
 *
 * Note that the ARM Exception Handling ABI allows unwinding to depend
 * on the contents of any core register, but our unwinder is limited
 * to the ones in struct stackframe (which are the only ones we expect
 * GCC to need for kernel code), so we just record those.
 */
#ifdef CONFIG_THUMB2_KERNEL
#define perf_arch_fetch_caller_regs(regs, ip)				\
	do {								\
		__u32 _cpsr, _pc;					\
		__asm__ __volatile__("str r7, [%[_regs], #(7 * 4)]\n\t" \
				     "str r13, [%[_regs], #(13 * 4)]\n\t" \
				     "str r14, [%[_regs], #(14 * 4)]\n\t" \
				     "mov %[_pc],  r15\n\t"		\
				     "mrs %[_cpsr], cpsr\n\t"		\
				     : [_cpsr] "=r" (_cpsr),		\
				       [_pc] "=r" (_pc)			\
				     : [_regs] "r" (&(regs)->uregs)	\
				     : "memory");			\
		(regs)->ARM_pc = _pc;					\
		(regs)->ARM_cpsr = _cpsr;				\
	} while (0)
#else
#define perf_arch_fetch_caller_regs(regs, ip)				\
	do {								\
		__u32 _cpsr;						\
		__asm__ __volatile__("stmia %[_regs11], {r11 - r15}\n\t" \
				     "mrs %[_cpsr], cpsr\n\t"		\
				     : [_cpsr] "=r" (_cpsr)		\
				     : [_regs11] "r" (&(regs)->uregs[11]) \
				     : "memory");			\
		(regs)->ARM_cpsr = _cpsr;				\
	} while (0)
#endif

#endif /* __ARM_PERF_EVENT_H__ */
