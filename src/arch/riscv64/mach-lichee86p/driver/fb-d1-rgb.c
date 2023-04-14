/*
 * driver/fb-d1-rgb.c
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
#include <dma/dma.h>
#include <clk/clk.h>
#include <reset/reset.h>
#include <gpio/gpio.h>
#include <led/led.h>
#include <framebuffer/framebuffer.h>
#include <d1-gpio.h>
#include <d1/reg-de.h>
#include <d1/reg-tconlcd.h>

struct fb_d1_rgb_pdata_t
{
	virtual_addr_t virt_de;
	virtual_addr_t virt_tconlcd;

	char * clk_de;
	char * clk_tconlcd;
	int rst_de;
	int rst_tconlcd;
	int width;
	int height;
	int pwidth;
	int pheight;
	int bits_per_pixel;
	int bytes_per_pixel;
	int pixlen;
	int index;
	void * vram[2];
	struct region_list_t * nrl, * orl;

	struct {
		int pixel_clock_hz;
		int h_front_porch;
		int h_back_porch;
		int h_sync_len;
		int v_front_porch;
		int v_back_porch;
		int v_sync_len;
		int h_sync_active;
		int v_sync_active;
		int den_active;
		int clk_active;
	} timing;

	struct led_t * backlight;
	int brightness;
};

static void inline d1_de_enable(struct fb_d1_rgb_pdata_t * pdat)
{
	struct de_glb_t * glb = (struct de_glb_t *)(pdat->virt_de + D1_DE_MUX_GLB);
	write32((virtual_addr_t)&glb->dbuff, 1);
}

static inline void d1_de_set_address(struct fb_d1_rgb_pdata_t * pdat, void * vram)
{
	struct de_ui_t * ui = (struct de_ui_t *)(pdat->virt_de + D1_DE_MUX_CHAN + 0x1000 * 1);
	write32((virtual_addr_t)&ui->cfg[0].top_laddr, (u32_t)(unsigned long)vram);
}

static inline void d1_de_set_mode(struct fb_d1_rgb_pdata_t * pdat)
{
	struct de_clk_t * clk = (struct de_clk_t *)(pdat->virt_de);
	struct de_glb_t * glb = (struct de_glb_t *)(pdat->virt_de + D1_DE_MUX_GLB);
	struct de_bld_t * bld = (struct de_bld_t *)(pdat->virt_de + D1_DE_MUX_BLD);
	struct de_ui_t * ui = (struct de_ui_t *)(pdat->virt_de + D1_DE_MUX_CHAN + 0x1000 * 1);
	u32_t size = (((pdat->height - 1) << 16) | (pdat->width - 1));
	u32_t val;
	int i;

	val = read32((virtual_addr_t)&clk->rst_cfg);
	val |= 1 << 0;
	write32((virtual_addr_t)&clk->rst_cfg, val);

	val = read32((virtual_addr_t)&clk->gate_cfg);
	val |= 1 << 0;
	write32((virtual_addr_t)&clk->gate_cfg, val);

	val = read32((virtual_addr_t)&clk->bus_cfg);
	val |= 1 << 0;
	write32((virtual_addr_t)&clk->bus_cfg, val);

	val = read32((virtual_addr_t)&clk->sel_cfg);
	val &= ~(1 << 0);
	write32((virtual_addr_t)&clk->sel_cfg, val);

	write32((virtual_addr_t)&glb->ctl, (1 << 0));
	write32((virtual_addr_t)&glb->status, 0);
	write32((virtual_addr_t)&glb->dbuff, 1);
	write32((virtual_addr_t)&glb->size, size);

	for(i = 0; i < 4; i++)
	{
		void * chan = (void *)(pdat->virt_de + D1_DE_MUX_CHAN + 0x1000 * i);
		memset(chan, 0, i == 0 ? sizeof(struct de_vi_t) : sizeof(struct de_ui_t));
	}
	memset(bld, 0, sizeof(struct de_bld_t));

	write32((virtual_addr_t)&bld->fcolor_ctl, 0x00000101);
	write32((virtual_addr_t)&bld->route, 1);
	write32((virtual_addr_t)&bld->premultiply, 0);
	write32((virtual_addr_t)&bld->bkcolor, 0xff000000);
	write32((virtual_addr_t)&bld->bld_mode[0], 0x03010301);
	write32((virtual_addr_t)&bld->bld_mode[1], 0x03010301);
	write32((virtual_addr_t)&bld->output_size, size);
	write32((virtual_addr_t)&bld->out_ctl, 0);
	write32((virtual_addr_t)&bld->ck_ctl, 0);
	for(i = 0; i < 4; i++)
	{
		write32((virtual_addr_t)&bld->attr[i].fcolor, 0xff000000);
		write32((virtual_addr_t)&bld->attr[i].insize, size);
	}

	write32(pdat->virt_de + D1_DE_MUX_VSU, 0);
	write32(pdat->virt_de + D1_DE_MUX_GSU1, 0);
	write32(pdat->virt_de + D1_DE_MUX_GSU2, 0);
	write32(pdat->virt_de + D1_DE_MUX_GSU3, 0);
	write32(pdat->virt_de + D1_DE_MUX_FCE, 0);
	write32(pdat->virt_de + D1_DE_MUX_BWS, 0);
	write32(pdat->virt_de + D1_DE_MUX_LTI, 0);
	write32(pdat->virt_de + D1_DE_MUX_PEAK, 0);
	write32(pdat->virt_de + D1_DE_MUX_ASE, 0);
	write32(pdat->virt_de + D1_DE_MUX_FCC, 0);
	write32(pdat->virt_de + D1_DE_MUX_DCSC, 0);

	write32((virtual_addr_t)&ui->cfg[0].attr, (1 << 0) | (4 << 8) | (1 << 1) | (0xff << 24));
	write32((virtual_addr_t)&ui->cfg[0].size, size);
	write32((virtual_addr_t)&ui->cfg[0].coord, 0);
	write32((virtual_addr_t)&ui->cfg[0].pitch, 4 * pdat->width);
	write32((virtual_addr_t)&ui->cfg[0].top_laddr, (u32_t)(unsigned long)pdat->vram[pdat->index]);
	write32((virtual_addr_t)&ui->ovl_size, size);
}

static void d1_tconlcd_enable(struct fb_d1_rgb_pdata_t * pdat)
{
	struct d1_tconlcd_reg_t * tcon = (struct d1_tconlcd_reg_t *)pdat->virt_tconlcd;
	u32_t val;

	val = read32((virtual_addr_t)&tcon->gctrl);
	val |= (1 << 31);
	write32((virtual_addr_t)&tcon->gctrl, val);
}

static void d1_tconlcd_disable(struct fb_d1_rgb_pdata_t * pdat)
{
	struct d1_tconlcd_reg_t * tcon = (struct d1_tconlcd_reg_t *)pdat->virt_tconlcd;
	u32_t val;

	val = read32((virtual_addr_t)&tcon->dclk);
	val &= ~(0xf << 28);
	write32((virtual_addr_t)&tcon->dclk, val);

	write32((virtual_addr_t)&tcon->gctrl, 0);
	write32((virtual_addr_t)&tcon->gint0, 0);
}

static void d1_tconlcd_set_timing(struct fb_d1_rgb_pdata_t * pdat)
{
	struct d1_tconlcd_reg_t * tcon = (struct d1_tconlcd_reg_t *)pdat->virt_tconlcd;
	int bp, total;
	u32_t val;

	val = (pdat->timing.v_front_porch + pdat->timing.v_back_porch + pdat->timing.v_sync_len) / 2;
	write32((virtual_addr_t)&tcon->ctrl, (1 << 31) | (0 << 24) | (0 << 23) | ((val & 0x1f) << 4) | (0 << 0));
	val = clk_get_rate(pdat->clk_tconlcd) / pdat->timing.pixel_clock_hz;
	write32((virtual_addr_t)&tcon->dclk, (0xf << 28) | ((val / 2) << 0));
	write32((virtual_addr_t)&tcon->timing0, ((pdat->width - 1) << 16) | ((pdat->height - 1) << 0));
	bp = pdat->timing.h_sync_len + pdat->timing.h_back_porch;
	total = pdat->width + pdat->timing.h_front_porch + bp;
	write32((virtual_addr_t)&tcon->timing1, ((total - 1) << 16) | ((bp - 1) << 0));
	bp = pdat->timing.v_sync_len + pdat->timing.v_back_porch;
	total = pdat->height + pdat->timing.v_front_porch + bp;
	write32((virtual_addr_t)&tcon->timing2, ((total * 2) << 16) | ((bp - 1) << 0));
	write32((virtual_addr_t)&tcon->timing3, ((pdat->timing.h_sync_len - 1) << 16) | ((pdat->timing.v_sync_len - 1) << 0));

	val = (0 << 31) | (1 << 28);
	if(!pdat->timing.h_sync_active)
		val |= (1 << 25);
	if(!pdat->timing.v_sync_active)
		val |= (1 << 24);
	if(!pdat->timing.den_active)
		val |= (1 << 27);
	if(!pdat->timing.clk_active)
		val |= (1 << 26);
	write32((virtual_addr_t)&tcon->io_polarity, val);
	write32((virtual_addr_t)&tcon->io_tristate, 0);
}

static void d1_tconlcd_set_dither(struct fb_d1_rgb_pdata_t * pdat)
{
	struct d1_tconlcd_reg_t * tcon = (struct d1_tconlcd_reg_t *)pdat->virt_tconlcd;

	if((pdat->bits_per_pixel == 16) || (pdat->bits_per_pixel == 18))
	{
		write32((virtual_addr_t)&tcon->frm_seed[0], 0x11111111);
		write32((virtual_addr_t)&tcon->frm_seed[1], 0x11111111);
		write32((virtual_addr_t)&tcon->frm_seed[2], 0x11111111);
		write32((virtual_addr_t)&tcon->frm_seed[3], 0x11111111);
		write32((virtual_addr_t)&tcon->frm_seed[4], 0x11111111);
		write32((virtual_addr_t)&tcon->frm_seed[5], 0x11111111);
		write32((virtual_addr_t)&tcon->frm_table[0], 0x01010000);
		write32((virtual_addr_t)&tcon->frm_table[1], 0x15151111);
		write32((virtual_addr_t)&tcon->frm_table[2], 0x57575555);
		write32((virtual_addr_t)&tcon->frm_table[3], 0x7f7f7777);

		if(pdat->bits_per_pixel == 16)
			write32((virtual_addr_t)&tcon->frm_ctrl, (1 << 31) | (1 << 6) | (0 << 5)| (1 << 4));
		else if(pdat->bits_per_pixel == 18)
			write32((virtual_addr_t)&tcon->frm_ctrl, (1 << 31) | (0 << 6) | (0 << 5)| (0 << 4));
	}
}

static void fb_d1_cfg_gpios(int base, int n, int cfg, enum gpio_pull_t pull, enum gpio_drv_t drv)
{
	for(; n > 0; n--, base++)
	{
		gpio_set_cfg(base, cfg);
		gpio_set_pull(base, pull);
		gpio_set_drv(base, drv);
	}
}

static void st7701s_write_command(struct fb_d1_rgb_pdata_t * pdat, uint8_t val)
{
	int i;

	gpio_set_value(D1_GPIOE14, 0);
	gpio_set_value(D1_GPIOE12, 0);
	gpio_set_value(D1_GPIOE15, 0);
	gpio_set_value(D1_GPIOE15, 1);
	for(i = 0; i < 8; i++)
	{
		if(val & (0x80 >> i))
			gpio_set_value(D1_GPIOE12, 1);
		else
			gpio_set_value(D1_GPIOE12, 0);
		gpio_set_value(D1_GPIOE15, 0);
		gpio_set_value(D1_GPIOE15, 1);
	}
	gpio_set_value(D1_GPIOE14, 1);
}

static void st7701s_write_data(struct fb_d1_rgb_pdata_t * pdat, uint8_t val)
{
	int i;

	gpio_set_value(D1_GPIOE14, 0);
	gpio_set_value(D1_GPIOE12, 1);
	gpio_set_value(D1_GPIOE15, 0);
	gpio_set_value(D1_GPIOE15, 1);
	for(i = 0; i < 8; i++)
	{
		if(val & (0x80 >> i))
			gpio_set_value(D1_GPIOE12, 1);
		else
			gpio_set_value(D1_GPIOE12, 0);
		gpio_set_value(D1_GPIOE15, 0);
		gpio_set_value(D1_GPIOE15, 1);
	}
	gpio_set_value(D1_GPIOE14, 1);
}

static void st7701s_init(struct fb_d1_rgb_pdata_t * pdat)
{
	gpio_set_cfg(D1_GPIOE15, 1);
	gpio_set_pull(D1_GPIOE15, GPIO_PULL_UP);
	gpio_direction_output(D1_GPIOE15, 1);

	gpio_set_cfg(D1_GPIOE12, 1);
	gpio_set_pull(D1_GPIOE12, GPIO_PULL_UP);
	gpio_direction_output(D1_GPIOE12, 1);

	gpio_set_cfg(D1_GPIOE14, 1);
	gpio_set_pull(D1_GPIOE14, GPIO_PULL_UP);
	gpio_direction_output(D1_GPIOE14, 1);

	gpio_set_cfg(D1_GPIOG13, 1);
	gpio_set_pull(D1_GPIOG13, GPIO_PULL_UP);
	gpio_direction_output(D1_GPIOG13, 0);
	mdelay(10);
	gpio_direction_output(D1_GPIOG13, 1);
	mdelay(10);

	st7701s_write_command(pdat, 0xff);
	st7701s_write_data(pdat, 0x77);
	st7701s_write_data(pdat, 0x01);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x10);

	st7701s_write_command(pdat, 0xc0);
	st7701s_write_data(pdat, 0x3b);
	st7701s_write_data(pdat, 0x00);

	st7701s_write_command(pdat, 0xc1);
	st7701s_write_data(pdat, 0x0d);
	st7701s_write_data(pdat, 0x02);

	st7701s_write_command(pdat, 0xc2);
	st7701s_write_data(pdat, 0x21);
	st7701s_write_data(pdat, 0x08);

	st7701s_write_command(pdat, 0xcd);
	st7701s_write_data(pdat, 0x18);

	st7701s_write_command(pdat, 0xb0);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x11);
	st7701s_write_data(pdat, 0x18);
	st7701s_write_data(pdat, 0x0e);
	st7701s_write_data(pdat, 0x11);
	st7701s_write_data(pdat, 0x06);
	st7701s_write_data(pdat, 0x07);
	st7701s_write_data(pdat, 0x08);
	st7701s_write_data(pdat, 0x07);
	st7701s_write_data(pdat, 0x22);
	st7701s_write_data(pdat, 0x04);
	st7701s_write_data(pdat, 0x12);
	st7701s_write_data(pdat, 0x0f);
	st7701s_write_data(pdat, 0xaa);
	st7701s_write_data(pdat, 0x31);
	st7701s_write_data(pdat, 0x18);

	st7701s_write_command(pdat, 0xb1);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x11);
	st7701s_write_data(pdat, 0x19);
	st7701s_write_data(pdat, 0x0e);
	st7701s_write_data(pdat, 0x12);
	st7701s_write_data(pdat, 0x07);
	st7701s_write_data(pdat, 0x08);
	st7701s_write_data(pdat, 0x08);
	st7701s_write_data(pdat, 0x08);
	st7701s_write_data(pdat, 0x22);
	st7701s_write_data(pdat, 0x04);
	st7701s_write_data(pdat, 0x11);
	st7701s_write_data(pdat, 0x11);
	st7701s_write_data(pdat, 0xa9);
	st7701s_write_data(pdat, 0x32);
	st7701s_write_data(pdat, 0x18);

	st7701s_write_command(pdat, 0xff);
	st7701s_write_data(pdat, 0x77);
	st7701s_write_data(pdat, 0x01);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x11);

	st7701s_write_command(pdat, 0xb0);
	st7701s_write_data(pdat, 0x60);

	st7701s_write_command(pdat, 0xb1);
	st7701s_write_data(pdat, 0x30);

	st7701s_write_command(pdat, 0xb2);
	st7701s_write_data(pdat, 0x87);

	st7701s_write_command(pdat, 0xb3);
	st7701s_write_data(pdat, 0x80);

	st7701s_write_command(pdat, 0xb5);
	st7701s_write_data(pdat, 0x49);

	st7701s_write_command(pdat, 0xb7);
	st7701s_write_data(pdat, 0x85);

	st7701s_write_command(pdat, 0xb8);
	st7701s_write_data(pdat, 0x21);

	st7701s_write_command(pdat, 0xc1);
	st7701s_write_data(pdat, 0x78);

	st7701s_write_command(pdat, 0xc2);
	st7701s_write_data(pdat, 0x78);
	mdelay(10);

	st7701s_write_command(pdat, 0xe0);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x1b);
	st7701s_write_data(pdat, 0x02);

	st7701s_write_command(pdat, 0xe1);
	st7701s_write_data(pdat, 0x08);
	st7701s_write_data(pdat, 0xa0);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x07);
	st7701s_write_data(pdat, 0xa0);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x44);
	st7701s_write_data(pdat, 0x44);

	st7701s_write_command(pdat, 0xe2);
	st7701s_write_data(pdat, 0x11);
	st7701s_write_data(pdat, 0x11);
	st7701s_write_data(pdat, 0x44);
	st7701s_write_data(pdat, 0x44);
	st7701s_write_data(pdat, 0xed);
	st7701s_write_data(pdat, 0xa0);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0xec);
	st7701s_write_data(pdat, 0xa0);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x00);

	st7701s_write_command(pdat, 0xe3);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x11);
	st7701s_write_data(pdat, 0x11);

	st7701s_write_command(pdat, 0xe4);
	st7701s_write_data(pdat, 0x44);
	st7701s_write_data(pdat, 0x44);

	st7701s_write_command(pdat, 0xe5);
	st7701s_write_data(pdat, 0x0a);
	st7701s_write_data(pdat, 0xe9);
	st7701s_write_data(pdat, 0xd8);
	st7701s_write_data(pdat, 0xa0);
	st7701s_write_data(pdat, 0x0c);
	st7701s_write_data(pdat, 0xeb);
	st7701s_write_data(pdat, 0xd8);
	st7701s_write_data(pdat, 0xa0);
	st7701s_write_data(pdat, 0x0e);
	st7701s_write_data(pdat, 0xed);
	st7701s_write_data(pdat, 0xd8);
	st7701s_write_data(pdat, 0xa0);
	st7701s_write_data(pdat, 0x10);
	st7701s_write_data(pdat, 0xef);
	st7701s_write_data(pdat, 0xd8);
	st7701s_write_data(pdat, 0xa0);

	st7701s_write_command(pdat, 0xe6);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x11);
	st7701s_write_data(pdat, 0x11);

	st7701s_write_command(pdat, 0xe7);
	st7701s_write_data(pdat, 0x44);
	st7701s_write_data(pdat, 0x44);

	st7701s_write_command(pdat, 0xe8);
	st7701s_write_data(pdat, 0x09);
	st7701s_write_data(pdat, 0xe8);
	st7701s_write_data(pdat, 0xd8);
	st7701s_write_data(pdat, 0xa0);
	st7701s_write_data(pdat, 0x0b);
	st7701s_write_data(pdat, 0xea);
	st7701s_write_data(pdat, 0xd8);
	st7701s_write_data(pdat, 0xa0);
	st7701s_write_data(pdat, 0x0d);
	st7701s_write_data(pdat, 0xec);
	st7701s_write_data(pdat, 0xd8);
	st7701s_write_data(pdat, 0xa0);
	st7701s_write_data(pdat, 0x0f);
	st7701s_write_data(pdat, 0xee);
	st7701s_write_data(pdat, 0xd8);
	st7701s_write_data(pdat, 0xa0);

	st7701s_write_command(pdat, 0xeb);
	st7701s_write_data(pdat, 0x02);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0xe4);
	st7701s_write_data(pdat, 0xe4);
	st7701s_write_data(pdat, 0x88);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x40);

	st7701s_write_command(pdat, 0xec);
	st7701s_write_data(pdat, 0x3c);
	st7701s_write_data(pdat, 0x00);

	st7701s_write_command(pdat, 0xed);
	st7701s_write_data(pdat, 0xab);
	st7701s_write_data(pdat, 0x89);
	st7701s_write_data(pdat, 0x76);
	st7701s_write_data(pdat, 0x54);
	st7701s_write_data(pdat, 0x02);
	st7701s_write_data(pdat, 0xff);
	st7701s_write_data(pdat, 0xff);
	st7701s_write_data(pdat, 0xff);
	st7701s_write_data(pdat, 0xff);
	st7701s_write_data(pdat, 0xff);
	st7701s_write_data(pdat, 0xff);
	st7701s_write_data(pdat, 0x20);
	st7701s_write_data(pdat, 0x45);
	st7701s_write_data(pdat, 0x67);
	st7701s_write_data(pdat, 0x98);
	st7701s_write_data(pdat, 0xba);

	st7701s_write_command(pdat, 0xff);
	st7701s_write_data(pdat, 0x77);
	st7701s_write_data(pdat, 0x01);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x00);
	st7701s_write_data(pdat, 0x00);

	st7701s_write_command(pdat, 0x3a);
	st7701s_write_data(pdat, 0x66);

	st7701s_write_command(pdat, 0x36);
	st7701s_write_data(pdat, 0x00);

	st7701s_write_command(pdat, 0x21);
	st7701s_write_command(pdat, 0x11);
	st7701s_write_command(pdat, 0x29);
}

static void fb_d1_rgb_init(struct fb_d1_rgb_pdata_t * pdat)
{
	st7701s_init(pdat);
	if(pdat->bits_per_pixel == 16)
	{
		fb_d1_cfg_gpios(D1_GPIOD1, 5, 0x2, GPIO_PULL_NONE, GPIO_DRV_STRONG);
		fb_d1_cfg_gpios(D1_GPIOD6, 6, 0x2, GPIO_PULL_NONE, GPIO_DRV_STRONG);
		fb_d1_cfg_gpios(D1_GPIOD13, 5, 0x2, GPIO_PULL_NONE, GPIO_DRV_STRONG);
		fb_d1_cfg_gpios(D1_GPIOD18, 4, 0x2, GPIO_PULL_NONE, GPIO_DRV_STRONG);
	}
	else if(pdat->bits_per_pixel == 18)
	{
		fb_d1_cfg_gpios(D1_GPIOD0, 6, 0x2, GPIO_PULL_NONE, GPIO_DRV_STRONG);
		fb_d1_cfg_gpios(D1_GPIOD6, 6, 0x2, GPIO_PULL_NONE, GPIO_DRV_STRONG);
		fb_d1_cfg_gpios(D1_GPIOD12, 6, 0x2, GPIO_PULL_NONE, GPIO_DRV_STRONG);
		fb_d1_cfg_gpios(D1_GPIOD18, 4, 0x2, GPIO_PULL_NONE, GPIO_DRV_STRONG);
	}
	d1_tconlcd_disable(pdat);
	d1_tconlcd_set_timing(pdat);
	d1_tconlcd_set_dither(pdat);
	d1_tconlcd_enable(pdat);
	d1_de_set_mode(pdat);
	d1_de_enable(pdat);
	d1_de_set_address(pdat, pdat->vram[pdat->index]);
	d1_de_enable(pdat);
}

static void fb_setbl(struct framebuffer_t * fb, int brightness)
{
	struct fb_d1_rgb_pdata_t * pdat = (struct fb_d1_rgb_pdata_t *)fb->priv;
	led_set_brightness(pdat->backlight, brightness);
}

static int fb_getbl(struct framebuffer_t * fb)
{
	struct fb_d1_rgb_pdata_t * pdat = (struct fb_d1_rgb_pdata_t *)fb->priv;
	return led_get_brightness(pdat->backlight);
}

static struct surface_t * fb_create(struct framebuffer_t * fb)
{
	struct fb_d1_rgb_pdata_t * pdat = (struct fb_d1_rgb_pdata_t *)fb->priv;
	return surface_alloc(pdat->width, pdat->height, NULL);
}

static void fb_destroy(struct framebuffer_t * fb, struct surface_t * s)
{
	surface_free(s);
}

static void fb_present(struct framebuffer_t * fb, struct surface_t * s, struct region_list_t * rl)
{
	struct fb_d1_rgb_pdata_t * pdat = (struct fb_d1_rgb_pdata_t *)fb->priv;
	struct region_list_t * nrl = pdat->nrl;

	region_list_clear(nrl);
	region_list_merge(nrl, pdat->orl);
	region_list_merge(nrl, rl);
	region_list_clone(pdat->orl, rl);

	pdat->index = (pdat->index + 1) & 0x1;
	if(nrl->count > 0)
		present_surface(pdat->vram[pdat->index], s, nrl);
	else
		memcpy(pdat->vram[pdat->index], s->pixels, s->pixlen);
	dma_cache_sync(pdat->vram[pdat->index], pdat->pixlen, DMA_TO_DEVICE);
	d1_de_set_address(pdat, pdat->vram[pdat->index]);
	d1_de_enable(pdat);
}

static struct device_t * fb_d1_rgb_probe(struct driver_t * drv, struct dtnode_t * n)
{
	struct fb_d1_rgb_pdata_t * pdat;
	struct framebuffer_t * fb;
	struct device_t * dev;
	virtual_addr_t virt = phys_to_virt(dt_read_address(n));
	char * clk_de = dt_read_string(n, "clock-name-de", NULL);
	char * clk_tconlcd = dt_read_string(n, "clock-name-tconlcd", NULL);

	if(!search_clk(clk_de) || !search_clk(clk_tconlcd))
		return NULL;

	pdat = malloc(sizeof(struct fb_d1_rgb_pdata_t));
	if(!pdat)
		return NULL;

	fb = malloc(sizeof(struct framebuffer_t));
	if(!fb)
	{
		free(pdat);
		return NULL;
	}

	pdat->virt_de = virt;
	pdat->virt_tconlcd = phys_to_virt(D1_TCONLCD_BASE);
	pdat->clk_de = strdup(clk_de);
	pdat->clk_tconlcd = strdup(clk_tconlcd);
	pdat->rst_de = dt_read_int(n, "reset-de", -1);
	pdat->rst_tconlcd = dt_read_int(n, "reset-tconlcd", -1);
	pdat->width = dt_read_int(n, "width", 800);
	pdat->height = dt_read_int(n, "height", 480);
	pdat->pwidth = dt_read_int(n, "physical-width", 216);
	pdat->pheight = dt_read_int(n, "physical-height", 135);
	pdat->bits_per_pixel = dt_read_int(n, "bits-per-pixel", 18);
	pdat->bytes_per_pixel = 4;
	pdat->pixlen = pdat->width * pdat->height * pdat->bytes_per_pixel;
	pdat->index = 0;
	pdat->vram[0] = dma_alloc_noncoherent(pdat->pixlen);
	pdat->vram[1] = dma_alloc_noncoherent(pdat->pixlen);
	pdat->nrl = region_list_alloc(0);
	pdat->orl = region_list_alloc(0);
	memset(pdat->vram[0], 0, pdat->pixlen);
	memset(pdat->vram[1], 0, pdat->pixlen);

	pdat->timing.pixel_clock_hz = dt_read_long(n, "clock-frequency", 33000000);
	pdat->timing.h_front_porch = dt_read_int(n, "hfront-porch", 40);
	pdat->timing.h_back_porch = dt_read_int(n, "hback-porch", 87);
	pdat->timing.h_sync_len = dt_read_int(n, "hsync-len", 1);
	pdat->timing.v_front_porch = dt_read_int(n, "vfront-porch", 13);
	pdat->timing.v_back_porch = dt_read_int(n, "vback-porch", 31);
	pdat->timing.v_sync_len = dt_read_int(n, "vsync-len", 1);
	pdat->timing.h_sync_active = dt_read_bool(n, "hsync-active", 0);
	pdat->timing.v_sync_active = dt_read_bool(n, "vsync-active", 0);
	pdat->timing.den_active = dt_read_bool(n, "den-active", 0);
	pdat->timing.clk_active = dt_read_bool(n, "clk-active", 0);
	pdat->backlight = search_led(dt_read_string(n, "backlight", NULL));

	fb->name = alloc_device_name(dt_read_name(n), -1);
	fb->width = pdat->width;
	fb->height = pdat->height;
	fb->pwidth = pdat->pwidth;
	fb->pheight = pdat->pheight;
	fb->setbl = fb_setbl;
	fb->getbl = fb_getbl;
	fb->create = fb_create;
	fb->destroy = fb_destroy;
	fb->present = fb_present;
	fb->priv = pdat;

	clk_enable(pdat->clk_de);
	clk_enable(pdat->clk_tconlcd);
	if(pdat->rst_de >= 0)
		reset_deassert(pdat->rst_de);
	if(pdat->rst_tconlcd >= 0)
		reset_deassert(pdat->rst_tconlcd);
	fb_d1_rgb_init(pdat);

	if(!(dev = register_framebuffer(fb, drv)))
	{
		clk_disable(pdat->clk_de);
		clk_disable(pdat->clk_tconlcd);
		free(pdat->clk_de);
		free(pdat->clk_tconlcd);
		dma_free_noncoherent(pdat->vram[0]);
		dma_free_noncoherent(pdat->vram[1]);
		region_list_free(pdat->nrl);
		region_list_free(pdat->orl);
		free_device_name(fb->name);
		free(fb->priv);
		free(fb);
		return NULL;
	}
	return dev;
}

static void fb_d1_rgb_remove(struct device_t * dev)
{
	struct framebuffer_t * fb = (struct framebuffer_t *)dev->priv;
	struct fb_d1_rgb_pdata_t * pdat = (struct fb_d1_rgb_pdata_t *)fb->priv;

	if(fb)
	{
		unregister_framebuffer(fb);
		clk_disable(pdat->clk_de);
		clk_disable(pdat->clk_tconlcd);
		free(pdat->clk_de);
		free(pdat->clk_tconlcd);
		dma_free_noncoherent(pdat->vram[0]);
		dma_free_noncoherent(pdat->vram[1]);
		region_list_free(pdat->nrl);
		region_list_free(pdat->orl);
		free_device_name(fb->name);
		free(fb->priv);
		free(fb);
	}
}

static void fb_d1_rgb_suspend(struct device_t * dev)
{
	struct framebuffer_t * fb = (struct framebuffer_t *)dev->priv;
	struct fb_d1_rgb_pdata_t * pdat = (struct fb_d1_rgb_pdata_t *)fb->priv;

	pdat->brightness = led_get_brightness(pdat->backlight);
	led_set_brightness(pdat->backlight, 0);
}

static void fb_d1_rgb_resume(struct device_t * dev)
{
	struct framebuffer_t * fb = (struct framebuffer_t *)dev->priv;
	struct fb_d1_rgb_pdata_t * pdat = (struct fb_d1_rgb_pdata_t *)fb->priv;

	led_set_brightness(pdat->backlight, pdat->brightness);
}

static struct driver_t fb_d1_rgb = {
	.name		= "fb-d1-rgb",
	.probe		= fb_d1_rgb_probe,
	.remove		= fb_d1_rgb_remove,
	.suspend	= fb_d1_rgb_suspend,
	.resume		= fb_d1_rgb_resume,
};

static __init void fb_d1_rgb_driver_init(void)
{
	register_driver(&fb_d1_rgb);
}

static __exit void fb_d1_rgb_driver_exit(void)
{
	unregister_driver(&fb_d1_rgb);
}

driver_initcall(fb_d1_rgb_driver_init);
driver_exitcall(fb_d1_rgb_driver_exit);
