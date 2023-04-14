/*
 * kernel/xui/xui-window.c
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
#include <xui/window.h>

static inline void root_container_begin(struct xui_context_t * ctx, struct xui_container_t * c)
{
	xui_push(ctx->container_stack, c);
	xui_push(ctx->root_list, c);
	c->head = xui_cmd_push_jump(ctx, NULL);
	if(region_hit(&c->region, ctx->mouse.x, ctx->mouse.y) && (!ctx->next_hover_root || (c->zindex > ctx->next_hover_root->zindex)))
		ctx->next_hover_root = c;
	xui_push(ctx->clip_stack, *(&(struct region_t){0, 0, INT_MAX, INT_MAX}));
}

static inline void root_container_end(struct xui_context_t * ctx)
{
	struct xui_container_t * c = xui_get_container(ctx);
	c->tail = xui_cmd_push_jump(ctx, NULL);
	c->head->jump.addr = ctx->cmd_list.items + ctx->cmd_list.idx;
	xui_pop_clip(ctx);
	pop_container(ctx);
}

int xui_begin_window_ex(struct xui_context_t * ctx, const char * title, struct region_t * r, int opt)
{
	unsigned int id = title ? xui_get_id(ctx, title, strlen(title)) : xui_get_id(ctx, &title, sizeof(title));
	struct xui_container_t * c = get_container(ctx, id, opt);
	struct region_t region, body;

	if(c && c->open)
	{
		xui_push(ctx->id_stack, id);
		if(opt & XUI_WINDOW_FULLSCREEN)
			region_clone(&c->region, &ctx->screen);
		else if(c->region.w == 0)
			region_clone(&c->region, r ? r : &ctx->screen);
		region_clone(&region, &c->region);
		region_clone(&body, &c->region);
		root_container_begin(ctx, c);
		scroll_begin(ctx, c, opt);
		if(opt & XUI_WINDOW_FULLSCREEN)
		{
			if(~opt & XUI_WINDOW_TRANSPARENT)
				xui_draw_rectangle(ctx, region.x, region.y, region.w, region.h, 0, 0, &ctx->style.window.background_color);
			push_container_body(ctx, c, &body);
		}
		else
		{
			if(ctx->style.window.shadow_radius > 0)
				xui_draw_shadow(ctx, region.x, region.y, region.w, region.h, max(ctx->style.window.shadow_radius, ctx->style.window.border_radius), &(struct color_t){0, 0, 0, 255}, 0);
			if(ctx->style.window.border_color.a && (ctx->style.window.border_width > 0))
				xui_draw_rectangle(ctx, region.x, region.y, region.w, region.h, ctx->style.window.border_radius, ctx->style.window.border_width, &ctx->style.window.border_color);
			if(~opt & XUI_WINDOW_TRANSPARENT)
				xui_draw_rectangle(ctx, region.x, region.y, region.w, region.h, ctx->style.window.border_radius, 0, &ctx->style.window.background_color);
			if(~opt & XUI_WINDOW_NOTITLE)
			{
				struct region_t hr, hdr, hcr;
				region_init(&hr, region.x, region.y, region.w, ctx->style.window.title_height);
				if(~opt & XUI_WINDOW_NOCLOSE)
					region_init(&hcr, hr.x + hr.w - hr.h, hr.y, hr.h, hr.h);
				else
					region_init(&hcr, hr.x + hr.w, hr.y, 0, 0);
				region_init(&hdr, hr.x, hr.y, hr.w - hcr.w, hr.h);
				id = xui_get_id(ctx, "!title", 6);
				xui_draw_rectangle(ctx, hr.x, hr.y, hr.w, hr.h, (0xc << 16) | ctx->style.window.border_radius, 0, &ctx->style.window.title_color);
				xui_draw_text_align(ctx, ctx->style.font.font_family, ctx->style.font.size, title, &hdr, 0, &ctx->style.window.text_color, opt);
				xui_control_update(ctx, id, &hdr, opt);
				if((ctx->active == id) && (ctx->mouse.state & XUI_MOUSE_LEFT))
				{
					c->region.x += ctx->mouse.dx;
					c->region.y += ctx->mouse.dy;
				}
				body.y += hr.h;
				body.h -= hr.h;
				if(~opt & XUI_WINDOW_NOCLOSE)
				{
					id = xui_get_id(ctx, "!close", 6);
					int u = (hcr.h * 164) >> 9;
					int v = (hcr.h * 348) >> 9;
					int t = (hcr.h *  42) >> 9;
					xui_draw_circle(ctx, hcr.x + (hcr.h >> 1), hcr.y + (hcr.h >> 1), (hcr.h * 213) >> 9, 0, &ctx->style.window.text_color);
					xui_draw_line(ctx, &(struct point_t){hcr.x + u, hcr.y + u}, &(struct point_t){hcr.x + v, hcr.y + v}, t, &ctx->style.window.title_color);
					xui_draw_line(ctx, &(struct point_t){hcr.x + u, hcr.y + v}, &(struct point_t){hcr.x + v, hcr.y + u}, t, &ctx->style.window.title_color);
					xui_control_update(ctx, id, &hcr, opt);
					if((ctx->active == id) && (ctx->mouse.up & XUI_MOUSE_LEFT))
						c->open = 0;
				}
			}
			if(~opt & XUI_WINDOW_NORESIZE)
			{
				struct region_t sr;
				int sz = ctx->style.window.title_height;
				region_init(&sr, region.x + region.w - sz, region.y + region.h - sz, sz, sz);
				id = xui_get_id(ctx, "!resize", 7);
				xui_control_update(ctx, id, &sr, opt);
				if((ctx->active == id) && (ctx->mouse.state & XUI_MOUSE_LEFT))
				{
					if(ctx->resize_id != id)
					{
						ctx->resize_id = id;
						ctx->resize_cursor_x = ctx->mouse.x - sr.x;
						ctx->resize_cursor_y = ctx->mouse.y - sr.y;
					}
					c->region.w = max(64, ctx->mouse.x - region.x + sz - ctx->resize_cursor_x);
					c->region.h = max(64, ctx->mouse.y - region.y + sz - ctx->resize_cursor_y);
				}
				else if(ctx->resize_id == id)
				{
					ctx->resize_id = 0;
				}
			}
			push_container_body(ctx, c, &body);
			if(opt & XUI_WINDOW_POPUP)
			{
				struct region_t * pr = &xui_get_layout(ctx)->body;
				c->region.w = c->content_width + (c->region.w - pr->w);
				c->region.h = c->content_height + (c->region.h - pr->h);
				if(ctx->mouse.down && (ctx->hover_root != c))
					c->open = 0;
			}
		}
		xui_push_clip(ctx, &c->body);
		return 1;
	}
	return 0;
}

void xui_end_window(struct xui_context_t * ctx)
{
	struct xui_container_t * c = xui_get_container(ctx);
	scroll_end(ctx, c);
	xui_pop_clip(ctx);
	root_container_end(ctx);
}
