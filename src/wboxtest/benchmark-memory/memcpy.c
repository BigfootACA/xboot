/*
 * wboxtest/benchmark-memory/memcpy.c
 */

#include <wboxtest.h>

struct wbt_memcpy_pdata_t
{
	char * src;
	char * dst;
	size_t size;

	ktime_t t1;
	ktime_t t2;
	int calls;
};

static void * memcpy_setup(struct wboxtest_t * wbt)
{
	struct wbt_memcpy_pdata_t * pdat;
	int i;

	pdat = malloc(sizeof(struct wbt_memcpy_pdata_t));
	if(!pdat)
		return NULL;

	pdat->size = SZ_1M;
	pdat->src = malloc(pdat->size);
	pdat->dst = malloc(pdat->size);
	if(!pdat->src || !pdat->dst)
	{
		free(pdat->src);
		free(pdat->dst);
		free(pdat);
		return NULL;
	}
	for(i = 0; i < pdat->size; i++)
	{
		pdat->src[i] = i & 0xff;
		pdat->dst[i] = 0;
	}

	return pdat;
}

static void memcpy_clean(struct wboxtest_t * wbt, void * data)
{
	struct wbt_memcpy_pdata_t * pdat = (struct wbt_memcpy_pdata_t *)data;

	if(pdat)
	{
		free(pdat->dst);
		free(pdat->src);
		free(pdat);
	}
}

static void memcpy_run(struct wboxtest_t * wbt, void * data)
{
	struct wbt_memcpy_pdata_t * pdat = (struct wbt_memcpy_pdata_t *)data;
	char buf[32];

	if(pdat)
	{
		pdat->calls = 0;
		pdat->t2 = pdat->t1 = ktime_get();
		do {
			pdat->calls++;
			memcpy(pdat->dst, pdat->src, pdat->size);
			pdat->t2 = ktime_get();
		} while(ktime_before(pdat->t2, ktime_add_ms(pdat->t1, 2000)));
		wboxtest_print(" Bandwidth: %s/s\r\n", ssize(buf, (double)(pdat->calls * pdat->size) * 1000.0 / ktime_ms_delta(pdat->t2, pdat->t1)));
	}
}

static struct wboxtest_t wbt_memcpy = {
	.group	= "benchmark-memory",
	.name	= "memcpy",
	.setup	= memcpy_setup,
	.clean	= memcpy_clean,
	.run	= memcpy_run,
};

static __init void memcpy_wbt_init(void)
{
	register_wboxtest(&wbt_memcpy);
}

static __exit void memcpy_wbt_exit(void)
{
	unregister_wboxtest(&wbt_memcpy);
}

wboxtest_initcall(memcpy_wbt_init);
wboxtest_exitcall(memcpy_wbt_exit);
