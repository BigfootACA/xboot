/*
 * driver/g2d/g2d.c
 *
 * Copyright(c) 2007-2023 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <xboot.h>
#include <g2d/g2d.h>

struct g2d_t * search_g2d(const char * name)
{
	struct device_t * dev;

	dev = search_device(name, DEVICE_TYPE_G2D);
	if(!dev)
		return NULL;
	return (struct g2d_t *)dev->priv;
}

struct g2d_t * search_first_g2d(void)
{
	struct device_t * dev;

	dev = search_first_device(DEVICE_TYPE_G2D);
	if(!dev)
		return NULL;
	return (struct g2d_t *)dev->priv;
}

struct device_t * register_g2d(struct g2d_t * g, struct driver_t * drv)
{
	struct device_t * dev;

	if(!g || !g->name)
		return NULL;

	dev = malloc(sizeof(struct device_t));
	if(!dev)
		return NULL;

	dev->name = strdup(g->name);
	dev->type = DEVICE_TYPE_G2D;
	dev->driver = drv;
	dev->priv = g;
	dev->kobj = kobj_alloc_directory(dev->name);

	if(!register_device(dev))
	{
		kobj_remove_self(dev->kobj);
		free(dev->name);
		free(dev);
		return NULL;
	}
	return dev;
}

void unregister_g2d(struct g2d_t * g)
{
	struct device_t * dev;

	if(g && g->name)
	{
		dev = search_device(g->name, DEVICE_TYPE_G2D);
		if(dev && unregister_device(dev))
		{
			kobj_remove_self(dev->kobj);
			free(dev->name);
			free(dev);
		}
	}
}
