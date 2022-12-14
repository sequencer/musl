#define a_barrier a_barrier
static inline void a_barrier()
{
	__asm__ __volatile__ ("fence rw,rw" : : : "memory");
}

#define a_ll a_ll
static inline int a_ll(volatile int *p)
{
	int v;
	__asm__ __volatile__ ("lr.w %0, (%1)" : "=&r"(v) : "r"(p));
	return v;
}

#define a_sc a_sc
static inline int a_sc(volatile int *p, int v)
{
	int r;
	__asm__ __volatile__ ("sc.w %0, %2, (%1)" : "=&r"(r) : "r"(p), "r"(v) : "memory");
	return !r;
}

#define a_cas a_cas
static inline int a_cas(volatile int *p, int t, int s)
{
	int old, tmp;
	__asm__ __volatile__ (
		"\n1:	lr.w.aqrl %0, (%2)\n"
		"	bne %0, %3, 1f\n"
		"	sc.w.aqrl %1, %4, (%2)\n"
		"	bnez %1, 1b\n"
		"1:"
		: "=&r"(old), "=&r"(tmp)
		: "r"(p), "r"((long)t), "r"((long)s)
		: "memory");
	return old;
}

#define a_cas_p a_cas_p
static inline void *a_cas_p(volatile void *p, void *t, void *s)
{
	void *old;
	int tmp;
	__asm__ __volatile__ (
		"\n1:	lr.w.aqrl %0, (%2)\n"
		"	bne %0, %3, 1f\n"
		"	sc.w.aqrl %1, %4, (%2)\n"
		"	bnez %1, 1b\n"
		"1:"
		: "=&r"(old), "=&r"(tmp)
		: "r"(p), "r"(t), "r"(s)
		: "memory");
	return old;
}
