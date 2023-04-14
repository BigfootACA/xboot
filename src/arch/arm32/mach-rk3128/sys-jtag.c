/*
 * sys-jtag.c
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

void sys_jtag_init(void)
{
	virtual_addr_t addr;
	u32_t val;

	/* Config GPIO1_C4 and GPIO1_C5 to swdclk and swdio */
	addr = 0x20008000 + 0x00b8 + ((20 >> 3) << 2);
	val = (((0x3 << 16) | (0x2 & 0x3)) << ((20 & 0x7) << 1));
	write32(addr, val);
	addr = 0x20008000 + 0x00b8 + ((21 >> 3) << 2);
	val = (((0x3 << 16) | (0x2 & 0x3)) << ((21 & 0x7) << 1));
	write32(addr, val);
}
