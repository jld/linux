#ifndef __ASM_STACKTRACE_H
#define __ASM_STACKTRACE_H

struct stackframe {
#ifdef CONFIG_THUMB2_KERNEL
	unsigned long r7;
#else
	unsigned long fp;
#endif
	unsigned long sp;
	unsigned long lr;
	unsigned long pc;
};

extern int unwind_frame(struct stackframe *frame);
extern void walk_stackframe(struct stackframe *frame,
			    int (*fn)(struct stackframe *, void *), void *data);

#endif	/* __ASM_STACKTRACE_H */
