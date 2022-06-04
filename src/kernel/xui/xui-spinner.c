/*
 * kernel/xui/xui-spinner.c
 *
 * Copyright(c) 2007-2022 Jianjun Jiang <8192542@qq.com>
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
#include <xui/spinner.h>

void xui_spinner_ex(struct xui_context_t * ctx, int opt)
{
	struct region_t * r = xui_layout_next(ctx);
	struct xui_widget_color_t * wc;
	struct color_t * c;
	int width, radius, angle;
	int x, y;

	switch(opt & (0x7 << 8))
	{
	case XUI_SPINNER_PRIMARY:
		wc = &ctx->style.primary;
		break;
	case XUI_SPINNER_SECONDARY:
		wc = &ctx->style.secondary;
		break;
	case XUI_SPINNER_SUCCESS:
		wc = &ctx->style.success;
		break;
	case XUI_SPINNER_INFO:
		wc = &ctx->style.info;
		break;
	case XUI_SPINNER_WARNING:
		wc = &ctx->style.warning;
		break;
	case XUI_SPINNER_DANGER:
		wc = &ctx->style.danger;
		break;
	default:
		wc = &ctx->style.primary;
		break;
	}
	c = &wc->normal.background;
	width = min(r->w, r->h) >> 4;
	radius = (min(r->w, r->h) - width - (ctx->style.layout.padding << 1)) >> 1;
	x = r->x + (r->w >> 1);
	y = r->y + (r->h >> 1);
	if(c->a)
	{
		angle = (ktime_to_ns(ctx->stamp) * 360 / 750000000) % 360;
		xui_draw_arc(ctx, x, y, radius, angle, angle + 270, width, c);
	}
}
