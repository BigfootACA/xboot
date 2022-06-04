/*
 * kernel/xui/xui.c
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
#include <xui/xui.h>

static const char style_default[] = X({
	"primary": {
		"normal-bakcground-color": "#536de6ff",
		"normal-foreground-color": "#ffffffff",
		"normal-border-color": "#00000000",

		"hover-bakcground-color": "#3a57e2ff",
		"hover-foreground-color": "#ffffffff",
		"hover-border-color": "#00000000",

		"active-bakcground-color": "#2647e0ff",
		"active-foreground-color": "#ffffffff",
		"active-border-color": "#536de660",
	},
	"secondary": {
		"normal-bakcground-color": "#6c757dff",
		"normal-foreground-color": "#ffffffff",
		"normal-border-color": "#00000000",

		"hover-bakcground-color": "#5a6268ff",
		"hover-foreground-color": "#ffffffff",
		"hover-border-color": "#00000000",

		"active-bakcground-color": "#545b62ff",
		"active-foreground-color": "#ffffffff",
		"active-border-color": "#6c757d60",
	},
	"success": {
		"normal-bakcground-color": "#10c469ff",
		"normal-foreground-color": "#ffffffff",
		"normal-border-color": "#00000000",

		"hover-bakcground-color": "#0da156ff",
		"hover-foreground-color": "#ffffffff",
		"hover-border-color": "#00000000",

		"active-bakcground-color": "#0c9550ff",
		"active-foreground-color": "#ffffffff",
		"active-border-color": "#10c46960",
	},
	"info": {
		"normal-bakcground-color": "#35b8e0ff",
		"normal-foreground-color": "#ffffffff",
		"normal-border-color": "#00000000",

		"hover-bakcground-color": "#20a6cfff",
		"hover-foreground-color": "#ffffffff",
		"hover-border-color": "#00000000",

		"active-bakcground-color": "#1e9dc4ff",
		"active-foreground-color": "#ffffffff",
		"active-border-color": "#35b8e060",
	},
	"warning": {
		"normal-bakcground-color": "#f9c851ff",
		"normal-foreground-color": "#ffffffff",
		"normal-border-color": "#00000000",

		"hover-bakcground-color": "#f8bc2cff",
		"hover-foreground-color": "#ffffffff",
		"hover-border-color": "#00000000",

		"active-bakcground-color": "#f7b820ff",
		"active-foreground-color": "#ffffffff",
		"active-border-color": "#f9c85160",
	},
	"danger": {
		"normal-bakcground-color": "#ff5b5bff",
		"normal-foreground-color": "#ffffffff",
		"normal-border-color": "#00000000",

		"hover-bakcground-color": "#ff3535ff",
		"hover-foreground-color": "#ffffffff",
		"hover-border-color": "#00000000",

		"active-bakcground-color": "#ff2828ff",
		"active-foreground-color": "#ffffffff",
		"active-border-color": "#ff5b5b60",
	},
	"cancel": {
		"normal-bakcground-color": "#6c757dff",
		"normal-foreground-color": "#ffffffff",
		"normal-border-color": "#00000000",

		"hover-bakcground-color": "#5a6268ff",
		"hover-foreground-color": "#ffffffff",
		"hover-border-color": "#00000000",

		"active-bakcground-color": "#545b62ff",
		"active-foreground-color": "#ffffffff",
		"active-border-color": "#6c757d60",
	},

	"icon-family": "material-icons",
	"font-family": "roboto-regular",
	"text-color": "#6c757dff",
	"font-size": 16,

	"layout-width": 64,
	"layout-height": 16,
	"layout-padding": 4,
	"layout-spacing": 4,
	"layout-indent": 24,

	"window-border-radius": 4,
	"window-border-width": 4,
	"window-title-height": 24,
	"window-background-color": "#f4f5f8ff",
	"window-border-color": "#2647e0ff",
	"window-title-color": "#2647e0ff",
	"window-text-color": "#ffffffff",

	"panel-border-radius": 8,
	"panel-border-width": 0,
	"panel-background-color": "#ffffffff",
	"panel-border-color": "#00000000",

	"scroll-width": 4,
	"scroll-radius": 2,
	"scroll-color": "#b1b6ba7f",

	"collapse-border-radius": 2,
	"collapse-border-width": 0,

	"tree-border-radius": 2,
	"tree-border-width": 0,

	"button-border-radius": 4,
	"button-border-width": 4,
	"button-outline-width": 2,

	"checkbox-border-radius": 4,
	"checkbox-border-width": 4,
	"checkbox-outline-width": 2,

	"radio-border-width": 4,
	"radio-outline-width": 2,

	"toggle-border-width": 4,
	"toggle-outline-width": 2,

	"tabbar-border-radius": 2,
	"tabbar-border-width": 0,

	"slider-invalid-color": "#eef2f7ff",
	"slider-border-width": 4,

	"number-border-radius": 4,
	"number-border-width": 4,
	"number-outline-width": 2,

	"textedit-border-radius": 4,
	"textedit-border-width": 4,
	"textedit-outline-width": 2,

	"badge-border-radius": 4,
	"badge-border-width": 4,
	"badge-outline-width": 2,

	"progress-invalid-color": "#eef2f7ff",
	"progress-border-radius": 4,

	"radialbar-invalid-color": "#eef2f7ff",

	"split-width": 2,
});

static struct region_t unlimited_region = {
	.x = 0,
	.y = 0,
	.w = INT_MAX,
	.h = INT_MAX,
};

void xui_begin(struct xui_context_t * ctx)
{
	ktime_t now = ktime_get();

	ctx->cmd_list.idx = 0;
	ctx->root_list.idx = 0;
	ctx->ohover = ctx->hover;
	ctx->hflag = 0;
	ctx->oactive = ctx->active;
	ctx->aflag = 0;
	ctx->hover_root = ctx->next_hover_root;
	ctx->next_hover_root = NULL;
	ctx->scroll_target = NULL;
	ctx->mouse.dx = ctx->mouse.x - ctx->mouse.ox;
	ctx->mouse.dy = ctx->mouse.y - ctx->mouse.oy;
	ctx->mouse.ox = ctx->mouse.x;
	ctx->mouse.oy = ctx->mouse.y;
	ctx->delta = ktime_sub(now, ctx->stamp);
	ctx->stamp = now;
	ctx->frame++;
	if(ktime_to_ns(ctx->delta) > 0)
		ctx->fps = ((ctx->fps * 633) >> 10) + 382000000LL / ktime_to_ns(ctx->delta);
}

static int compare_zindex(const void * a, const void * b)
{
	return (*(struct xui_container_t **)a)->zindex - (*(struct xui_container_t **)b)->zindex;
}

static int xui_cmd_next(struct xui_context_t * ctx, union xui_cmd_t ** cmd)
{
	if(*cmd)
		*cmd = (union xui_cmd_t *)(((char *)*cmd) + (*cmd)->base.len);
	else
		*cmd = (union xui_cmd_t *)ctx->cmd_list.items;
	while((char *)(*cmd) != ctx->cmd_list.items + ctx->cmd_list.idx)
	{
		if((*cmd)->base.type != XUI_CMD_TYPE_JUMP)
			return 1;
		*cmd = (*cmd)->jump.addr;
	}
	return 0;
}

void xui_end(struct xui_context_t * ctx)
{
	union xui_cmd_t * cmd = NULL;
	struct region_t r;
	unsigned int * ncell = ctx->cells[ctx->cindex];
	unsigned int * ocell = ctx->cells[(ctx->cindex = (ctx->cindex + 1) & 0x1)];
	unsigned int h;
	int x1, y1, x2, y2;
	int x, y;
	int i, n;

	assert(ctx->container_stack.idx == 0);
	assert(ctx->clip_stack.idx == 0);
	assert(ctx->id_stack.idx == 0);
	assert(ctx->layout_stack.idx == 0);
	if(ctx->scroll_target && !ctx->scroll_target->noscroll)
	{
		if(ctx->key_down & XUI_KEY_CTRL)
		{
			ctx->scroll_target->scroll_x -= ctx->mouse.zy * 30;
			ctx->scroll_target->scroll_y -= ctx->mouse.zx * 30;
		}
		else
		{
			ctx->scroll_target->scroll_x -= ctx->mouse.zx * 30;
			ctx->scroll_target->scroll_y -= ctx->mouse.zy * 30;
		}
		if(ctx->mouse.state & XUI_MOUSE_LEFT)
		{
			ctx->scroll_target->scroll_x -= ctx->mouse.dx;
			ctx->scroll_target->scroll_y -= ctx->mouse.dy;
			ctx->scroll_target->scroll_vx = 0;
			ctx->scroll_target->scroll_vy = 0;
		}
		if(abs(ctx->mouse.vx) > 50)
			ctx->scroll_target->scroll_vx = ctx->mouse.vx;
		if(abs(ctx->mouse.vy) > 50)
			ctx->scroll_target->scroll_vy = ctx->mouse.vy;
		if(abs(ctx->scroll_target->scroll_vx) > 2)
		{
			float friction = ktime_to_ns(ctx->delta) * (3.0 / 1000000000.0);
			ctx->scroll_target->scroll_vx -= ctx->scroll_target->scroll_vx * friction;
			ctx->scroll_target->scroll_x -= ctx->scroll_target->scroll_vx * friction;
		}
		if(abs(ctx->scroll_target->scroll_vy) > 2)
		{
			float friction = ktime_to_ns(ctx->delta) * (3.0 / 1000000000.0);
			ctx->scroll_target->scroll_vy -= ctx->scroll_target->scroll_vy * friction;
			ctx->scroll_target->scroll_y -= ctx->scroll_target->scroll_vy * friction;
		}
	}
	if(ctx->mouse.down && ctx->next_hover_root && (ctx->next_hover_root->zindex < ctx->last_zindex) && (ctx->next_hover_root->zindex >= 0))
		xui_set_front(ctx, ctx->next_hover_root);
	ctx->mouse.down = 0;
	ctx->mouse.up = 0;
	ctx->mouse.zx = 0;
	ctx->mouse.zy = 0;
	ctx->mouse.vx = 0;
	ctx->mouse.vy = 0;
	ctx->key_pressed = 0;
	ctx->input_text[0] = '\0';
	n = ctx->root_list.idx;
	qsort(ctx->root_list.items, n, sizeof(struct xui_container_t *), compare_zindex);
	for(i = 0; i < n; i++)
	{
		struct xui_container_t * c = ctx->root_list.items[i];
		if(i == 0)
		{
			union xui_cmd_t * cmd = (union xui_cmd_t *)ctx->cmd_list.items;
			cmd->jump.addr = (char *)c->head + sizeof(struct xui_cmd_jump_t);
		}
		else
		{
			struct xui_container_t * prev = ctx->root_list.items[i - 1];
			prev->tail->jump.addr = (char *)c->head + sizeof(struct xui_cmd_jump_t);
		}
		if(i == n - 1)
			c->tail->jump.addr = ctx->cmd_list.items + ctx->cmd_list.idx;
	}
	while(xui_cmd_next(ctx, &cmd))
	{
		if(region_intersect(&r, &ctx->screen, &cmd->base.r))
		{
			h = 5381;
			xui_hash(&h, &cmd->base, cmd->base.len);
			x1 = r.x >> ctx->cpshift;
			y1 = r.y >> ctx->cpshift;
			x2 = (r.x + r.w) >> ctx->cpshift;
			y2 = (r.y + r.h) >> ctx->cpshift;
			for(y = y1; y <= y2; y++)
			{
				for(x = x1; x <= x2; x++)
				{
					xui_hash(&ncell[x + y * ctx->cwidth], &h, sizeof(unsigned int));
				}
			}
		}
	}
	region_list_clear(ctx->w->rl);
	for(y = 0; y < ctx->cheight; y++)
	{
		for(x = 0; x < ctx->cwidth; x++)
		{
			i = x + y * ctx->cwidth;
			if(ncell[i] != ocell[i])
			{
				region_init(&r, x << ctx->cpshift, y << ctx->cpshift, 1 << ctx->cpshift, 1 << ctx->cpshift);
				if(region_intersect(&r, &r, &ctx->screen))
					region_list_add(ctx->w->rl, &r);
			}
			ocell[i] = 5381;
		}
	}
}

const char * xui_format(struct xui_context_t * ctx, const char * fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(ctx->fmtbuf, sizeof(ctx->fmtbuf), fmt, ap);
	va_end(ap);
	return (const char *)ctx->fmtbuf;
}

int xui_pool_init(struct xui_context_t * ctx, struct xui_pool_item_t * items, int len, unsigned int id)
{
	int i, n = 0, f = ctx->frame;
	for(i = 0; i < len; i++)
	{
		if(items[i].last_update < f)
		{
			f = items[i].last_update;
			n = i;
		}
	}
	items[n].id = id;
	items[n].last_update = ctx->frame;
	return n;
}

int xui_pool_get(struct xui_context_t * ctx, struct xui_pool_item_t * items, int len, unsigned int id)
{
	int i;
	for(i = 0; i < len; i++)
	{
		if(items[i].id == id)
			return i;
	}
	return -1;
}

void xui_pool_update(struct xui_context_t * ctx, struct xui_pool_item_t * items, int idx)
{
	items[idx].last_update = ctx->frame;
}

static void xui_get_bound(struct region_t * r, int x, int y)
{
	int x0 = min(r->x, x);
	int y0 = min(r->y, y);
	int x1 = max(r->x + r->w, x);
	int y1 = max(r->y + r->h, y);
	region_init(r, x0, y0, x1 - x0, y1 - y0);
}

static int xui_check_clip(struct xui_context_t * ctx, struct region_t * r)
{
	struct region_t * cr = xui_get_clip(ctx);

	if((r->w <= 0) || (r->h <= 0) || (r->x > cr->x + cr->w) || (r->x + r->w < cr->x) || (r->y > cr->y + cr->h) || (r->y + r->h < cr->y))
		return 0;
	else if((r->x >= cr->x) && (r->x + r->w <= cr->x + cr->w) && (r->y >= cr->y) && (r->y + r->h <= cr->y + cr->h))
		return 1;
	else
		return -1;
}

void xui_draw_line(struct xui_context_t * ctx, struct point_t * p0, struct point_t * p1, int thickness, struct color_t * c)
{
	union xui_cmd_t * cmd;
	struct region_t r;
	int clip;

	region_init(&r, p0->x, p0->y, 1, 1);
	xui_get_bound(&r, p1->x, p1->y);
	if(thickness > 1)
		region_expand(&r, &r, iceil(thickness / 2));
	if((clip = xui_check_clip(ctx, &r)))
	{
		if(clip < 0)
			xui_cmd_push_clip(ctx, xui_get_clip(ctx));
		cmd = xui_cmd_push(ctx, XUI_CMD_TYPE_LINE, sizeof(struct xui_cmd_line_t), &r);
		cmd->line.p0.x = p0->x;
		cmd->line.p0.y = p0->y;
		cmd->line.p1.x = p1->x;
		cmd->line.p1.y = p1->y;
		cmd->line.thickness = thickness;
		memcpy(&cmd->line.c, c, sizeof(struct color_t));
		if(clip < 0)
			xui_cmd_push_clip(ctx, &unlimited_region);
	}
}

void xui_draw_polyline(struct xui_context_t * ctx, struct point_t * p, int n, int thickness, struct color_t * c)
{
	union xui_cmd_t * cmd;
	struct region_t r;
	int len, i;
	int clip;

	region_init(&r, p[0].x, p[0].y, 1, 1);
	for(i = 1; i < n; i++)
		xui_get_bound(&r, p[i].x, p[i].y);
	if(thickness > 1)
		region_expand(&r, &r, iceil(thickness / 2));
	if((clip = xui_check_clip(ctx, &r)))
	{
		if(clip < 0)
			xui_cmd_push_clip(ctx, xui_get_clip(ctx));
		len = sizeof(struct point_t) * n;
		cmd = xui_cmd_push(ctx, XUI_CMD_TYPE_POLYLINE, sizeof(struct xui_cmd_polyline_t) + len, &r);
		cmd->polyline.n = n;
		cmd->polyline.thickness = thickness;
		memcpy(&cmd->polyline.c, c, sizeof(struct color_t));
		memcpy(cmd->polyline.p, p, len);
		if(clip < 0)
			xui_cmd_push_clip(ctx, &unlimited_region);
	}
}

void xui_draw_curve(struct xui_context_t * ctx, struct point_t * p, int n, int thickness, struct color_t * c)
{
	union xui_cmd_t * cmd;
	struct region_t r;
	int len, i;
	int clip;

	region_init(&r, p[0].x, p[0].y, 1, 1);
	for(i = 1; i < n; i++)
		xui_get_bound(&r, p[i].x, p[i].y);
	if(thickness > 1)
		region_expand(&r, &r, iceil(thickness / 2));
	if((clip = xui_check_clip(ctx, &r)))
	{
		if(clip < 0)
			xui_cmd_push_clip(ctx, xui_get_clip(ctx));
		len = sizeof(struct point_t) * n;
		cmd = xui_cmd_push(ctx, XUI_CMD_TYPE_CURVE, sizeof(struct xui_cmd_curve_t) + len, &r);
		cmd->curve.n = n;
		cmd->curve.thickness = thickness;
		memcpy(&cmd->curve.c, c, sizeof(struct color_t));
		memcpy(cmd->curve.p, p, len);
		if(clip < 0)
			xui_cmd_push_clip(ctx, &unlimited_region);
	}
}

void xui_draw_triangle(struct xui_context_t * ctx, struct point_t * p0, struct point_t * p1, struct point_t * p2, int thickness, struct color_t * c)
{
	union xui_cmd_t * cmd;
	struct region_t r;
	int clip;

	region_init(&r, p0->x, p0->y, 1, 1);
	xui_get_bound(&r, p1->x, p1->y);
	xui_get_bound(&r, p2->x, p2->y);
	if(thickness > 1)
		region_expand(&r, &r, iceil(thickness / 2));
	if((clip = xui_check_clip(ctx, &r)))
	{
		if(clip < 0)
			xui_cmd_push_clip(ctx, xui_get_clip(ctx));
		cmd = xui_cmd_push(ctx, XUI_CMD_TYPE_TRIANGLE, sizeof(struct xui_cmd_triangle_t), &r);
		cmd->triangle.p0.x = p0->x;
		cmd->triangle.p0.y = p0->y;
		cmd->triangle.p1.x = p1->x;
		cmd->triangle.p1.y = p1->y;
		cmd->triangle.p2.x = p2->x;
		cmd->triangle.p2.y = p2->y;
		cmd->triangle.thickness = thickness;
		memcpy(&cmd->triangle.c, c, sizeof(struct color_t));
		if(clip < 0)
			xui_cmd_push_clip(ctx, &unlimited_region);
	}
}

void xui_draw_rectangle(struct xui_context_t * ctx, int x, int y, int w, int h, int radius, int thickness, struct color_t * c)
{
	union xui_cmd_t * cmd;
	struct region_t r;
	int clip;

	region_init(&r, x, y, w, h);
	if(thickness > 1)
		region_expand(&r, &r, iceil(thickness / 2));
	if((clip = xui_check_clip(ctx, &r)))
	{
		if(clip < 0)
			xui_cmd_push_clip(ctx, xui_get_clip(ctx));
		cmd = xui_cmd_push(ctx, XUI_CMD_TYPE_RECTANGLE, sizeof(struct xui_cmd_rectangle_t), &r);
		cmd->rectangle.x = x;
		cmd->rectangle.y = y;
		cmd->rectangle.w = w;
		cmd->rectangle.h = h;
		cmd->rectangle.radius = radius;
		cmd->rectangle.thickness = thickness;
		memcpy(&cmd->rectangle.c, c, sizeof(struct color_t));
		if(clip < 0)
			xui_cmd_push_clip(ctx, &unlimited_region);
	}
}

void xui_draw_polygon(struct xui_context_t * ctx, struct point_t * p, int n, int thickness, struct color_t * c)
{
	union xui_cmd_t * cmd;
	struct region_t r;
	int len, i;
	int clip;

	region_init(&r, p[0].x, p[0].y, 1, 1);
	for(i = 1; i < n; i++)
		xui_get_bound(&r, p[i].x, p[i].y);
	if(thickness > 1)
		region_expand(&r, &r, iceil(thickness / 2));
	if((clip = xui_check_clip(ctx, &r)))
	{
		if(clip < 0)
			xui_cmd_push_clip(ctx, xui_get_clip(ctx));
		len = sizeof(struct point_t) * n;
		cmd = xui_cmd_push(ctx, XUI_CMD_TYPE_POLYGON, sizeof(struct xui_cmd_polygon_t) + len, &r);
		cmd->polygon.n = n;
		cmd->polygon.thickness = thickness;
		memcpy(&cmd->polygon.c, c, sizeof(struct color_t));
		memcpy(cmd->polygon.p, p, len);
		if(clip < 0)
			xui_cmd_push_clip(ctx, &unlimited_region);
	}
}

void xui_draw_circle(struct xui_context_t * ctx, int x, int y, int radius, int thickness, struct color_t * c)
{
	union xui_cmd_t * cmd;
	struct region_t r;
	int clip;

	region_init(&r, x - radius, y - radius, radius * 2, radius * 2);
	if(thickness > 1)
		region_expand(&r, &r, iceil(thickness / 2));
	if((clip = xui_check_clip(ctx, &r)))
	{
		if(clip < 0)
			xui_cmd_push_clip(ctx, xui_get_clip(ctx));
		cmd = xui_cmd_push(ctx, XUI_CMD_TYPE_CIRCLE, sizeof(struct xui_cmd_circle_t), &r);
		cmd->circle.x = x;
		cmd->circle.y = y;
		cmd->circle.radius = radius;
		cmd->circle.thickness = thickness;
		memcpy(&cmd->circle.c, c, sizeof(struct color_t));
		if(clip < 0)
			xui_cmd_push_clip(ctx, &unlimited_region);
	}
}

void xui_draw_ellipse(struct xui_context_t * ctx, int x, int y, int w, int h, int thickness, struct color_t * c)
{
	union xui_cmd_t * cmd;
	struct region_t r;
	int clip;

	region_init(&r, x - w, y - h, w * 2, h * 2);
	if(thickness > 1)
		region_expand(&r, &r, iceil(thickness / 2));
	if((clip = xui_check_clip(ctx, &r)))
	{
		if(clip < 0)
			xui_cmd_push_clip(ctx, xui_get_clip(ctx));
		cmd = xui_cmd_push(ctx, XUI_CMD_TYPE_ELLIPSE, sizeof(struct xui_cmd_ellipse_t), &r);
		cmd->ellipse.x = x;
		cmd->ellipse.y = y;
		cmd->ellipse.w = w;
		cmd->ellipse.h = h;
		cmd->ellipse.thickness = thickness;
		memcpy(&cmd->ellipse.c, c, sizeof(struct color_t));
		if(clip < 0)
			xui_cmd_push_clip(ctx, &unlimited_region);
	}
}

void xui_draw_arc(struct xui_context_t * ctx, int x, int y, int radius, int a1, int a2, int thickness, struct color_t * c)
{
	union xui_cmd_t * cmd;
	struct region_t r;
	int clip;

	region_init(&r, x - radius, y - radius, radius * 2, radius * 2);
	if(thickness > 1)
		region_expand(&r, &r, iceil(thickness / 2));
	if((clip = xui_check_clip(ctx, &r)))
	{
		if(clip < 0)
			xui_cmd_push_clip(ctx, xui_get_clip(ctx));
		cmd = xui_cmd_push(ctx, XUI_CMD_TYPE_ARC, sizeof(struct xui_cmd_arc_t), &r);
		cmd->arc.x = x;
		cmd->arc.y = y;
		cmd->arc.radius = radius;
		cmd->arc.a1 = a1;
		cmd->arc.a2 = a2;
		cmd->arc.thickness = thickness;
		memcpy(&cmd->arc.c, c, sizeof(struct color_t));
		if(clip < 0)
			xui_cmd_push_clip(ctx, &unlimited_region);
	}
}

void xui_draw_surface(struct xui_context_t * ctx, struct surface_t * s, struct matrix_t * m, int refresh)
{
	union xui_cmd_t * cmd;
	struct region_t r;
	double x1 = 0;
	double y1 = 0;
	double x2 = surface_get_width(s);
	double y2 = surface_get_height(s);
	int clip;

	matrix_transform_bounds(m, &x1, &y1, &x2, &y2);
	region_init(&r, x1, y1, x2 - x1 + 1, y2 - y1 + 1);
	if((clip = xui_check_clip(ctx, &r)))
	{
		if(clip < 0)
			xui_cmd_push_clip(ctx, xui_get_clip(ctx));
		cmd = xui_cmd_push(ctx, XUI_CMD_TYPE_SURFACE, sizeof(struct xui_cmd_surface_t), &r);
		cmd->surface.s = s;
		memcpy(&cmd->surface.m, m, sizeof(struct matrix_t));
		cmd->surface.refresh ^= refresh ? 1 : 0;
		if(clip < 0)
			xui_cmd_push_clip(ctx, &unlimited_region);
	}
}

void xui_draw_icon(struct xui_context_t * ctx, const char * family, uint32_t code, int x, int y, int w, int h, struct color_t * c)
{
	union xui_cmd_t * cmd;
	struct region_t r;
	int clip;

	region_init(&r, x, y, w, h);
	if((clip = xui_check_clip(ctx, &r)))
	{
		if(clip < 0)
			xui_cmd_push_clip(ctx, xui_get_clip(ctx));
		cmd = xui_cmd_push(ctx, XUI_CMD_TYPE_ICON, sizeof(struct xui_cmd_icon_t), &r);
		cmd->icon.family = family;
		cmd->icon.code = code;
		cmd->icon.x = x;
		cmd->icon.y = y;
		cmd->icon.w = w;
		cmd->icon.h = h;
		memcpy(&cmd->icon.c, c, sizeof(struct color_t));
		if(clip < 0)
			xui_cmd_push_clip(ctx, &unlimited_region);
	}
}

void xui_draw_text(struct xui_context_t * ctx, int x, int y, struct text_t * txt)
{
	union xui_cmd_t * cmd;
	struct region_t region;
	int clip;
	int len;

	region_init(&region, x, y, txt->metrics.width, txt->metrics.height);
	if((clip = xui_check_clip(ctx, &region)))
	{
		if(clip < 0)
			xui_cmd_push_clip(ctx, xui_get_clip(ctx));
		len = strlen(txt->utf8) + 1;
		cmd = xui_cmd_push(ctx, XUI_CMD_TYPE_TEXT, sizeof(struct xui_cmd_text_t) + ((len + 0x3) & ~0x3), &region);
		cmd->text.x = x;
		cmd->text.y = y;
		memcpy(&cmd->text.c, txt->c, sizeof(struct color_t));
		memcpy(cmd->text.utf8, txt->utf8, len);
		cmd->text.utf8[len] = 0;
		memcpy(&cmd->text.txt, txt, sizeof(struct text_t));
		if(clip < 0)
			xui_cmd_push_clip(ctx, &unlimited_region);
	}
}

void xui_draw_text_align(struct xui_context_t * ctx, const char * family, int size, const char * utf8, struct region_t * r, int wrap, struct color_t * c, int opt)
{
	union xui_cmd_t * cmd;
	struct region_t region;
	struct text_t txt;
	int x, y, w, h;
	int clip;
	int len;

	if(!family)
		family = ctx->style.font.font_family;
	if(size <= 0)
		size = ctx->style.font.size;
	if(!c)
		c = &ctx->style.font.color;
	text_init(&txt, utf8, c, wrap, ctx->f, family, size);
	w = txt.metrics.width;
	h = txt.metrics.height;
	switch(opt & (0x7 << 4))
	{
	case XUI_OPT_TEXT_LEFT:
		x = r->x + ctx->style.layout.padding;
		y = r->y + (r->h - h) / 2;
		break;
	case XUI_OPT_TEXT_RIGHT:
		x = r->x + r->w - w - ctx->style.layout.padding;
		y = r->y + (r->h - h) / 2;
		break;
	case XUI_OPT_TEXT_TOP:
		x = r->x + (r->w - w) / 2;
		y = r->y + ctx->style.layout.padding;
		break;
	case XUI_OPT_TEXT_BOTTOM:
		x = r->x + (r->w - w) / 2;
		y = r->y + r->h - h - ctx->style.layout.padding;
		break;
	case XUI_OPT_TEXT_CENTER:
		x = r->x + (r->w - w) / 2;
		y = r->y + (r->h - h) / 2;
		break;
	default:
		x = r->x + ctx->style.layout.padding;
		y = r->y + (r->h - h) / 2;
		break;
	}
	region_init(&region, x, y, w, h);
	xui_push_clip(ctx, r);
	if((clip = xui_check_clip(ctx, &region)))
	{
		if(clip < 0)
			xui_cmd_push_clip(ctx, xui_get_clip(ctx));
		len = strlen(utf8) + 1;
		cmd = xui_cmd_push(ctx, XUI_CMD_TYPE_TEXT, sizeof(struct xui_cmd_text_t) + ((len + 0x3) & ~0x3), &region);
		cmd->text.x = x;
		cmd->text.y = y;
		memcpy(&cmd->text.c, c, sizeof(struct color_t));
		memcpy(cmd->text.utf8, utf8, len);
		cmd->text.utf8[len] = 0;
		cmd->text.txt.utf8 = cmd->text.utf8;
		cmd->text.txt.c = &cmd->text.c;
		cmd->text.txt.wrap = wrap;
		cmd->text.txt.fctx = ctx->f;
		cmd->text.txt.family = family;
		cmd->text.txt.size = size;
		cmd->text.txt.metrics.ox = txt.metrics.ox;
		cmd->text.txt.metrics.oy = txt.metrics.oy;
		cmd->text.txt.metrics.width = txt.metrics.width;
		cmd->text.txt.metrics.height = txt.metrics.height;
		if(clip < 0)
			xui_cmd_push_clip(ctx, &unlimited_region);
	}
	xui_pop_clip(ctx);
}

void xui_draw_glass(struct xui_context_t * ctx, int x, int y, int w, int h, int radius, int refresh)
{
	union xui_cmd_t * cmd;
	struct region_t r;
	int clip;

	region_init(&r, x, y, w, h);
	if((clip = xui_check_clip(ctx, &r)))
	{
		if(clip < 0)
			xui_cmd_push_clip(ctx, xui_get_clip(ctx));
		cmd = xui_cmd_push(ctx, XUI_CMD_TYPE_GLASS, sizeof(struct xui_cmd_glass_t), &r);
		cmd->glass.x = x;
		cmd->glass.y = y;
		cmd->glass.w = w;
		cmd->glass.h = h;
		cmd->glass.radius = radius;
		cmd->glass.refresh ^= refresh ? 1 : 0;
		if(clip < 0)
			xui_cmd_push_clip(ctx, &unlimited_region);
	}
}

void xui_draw_shadow(struct xui_context_t * ctx, int x, int y, int w, int h, int radius, struct color_t * c, int refresh)
{
	union xui_cmd_t * cmd;
	struct region_t r;
	int r2 = radius << 1;
	int r4 = radius << 2;
	int clip;

	region_init(&r, x - r2, y - r2, w + r4, h + r4);
	if((clip = xui_check_clip(ctx, &r)))
	{
		if(clip < 0)
			xui_cmd_push_clip(ctx, xui_get_clip(ctx));
		cmd = xui_cmd_push(ctx, XUI_CMD_TYPE_SHADOW, sizeof(struct xui_cmd_glass_t), &r);
		cmd->shadow.x = x;
		cmd->shadow.y = y;
		cmd->shadow.w = w;
		cmd->shadow.h = h;
		cmd->shadow.radius = radius;
		memcpy(&cmd->shadow.c, c, sizeof(struct color_t));
		cmd->shadow.refresh ^= refresh ? 1 : 0;
		if(clip < 0)
			xui_cmd_push_clip(ctx, &unlimited_region);
	}
}

void xui_draw_gradient(struct xui_context_t * ctx, int x, int y, int w, int h, struct color_t * lt, struct color_t * rt, struct color_t * rb, struct color_t * lb)
{
	union xui_cmd_t * cmd;
	struct region_t r;
	int clip;

	region_init(&r, x, y, w, h);
	if((clip = xui_check_clip(ctx, &r)))
	{
		if(clip < 0)
			xui_cmd_push_clip(ctx, xui_get_clip(ctx));
		cmd = xui_cmd_push(ctx, XUI_CMD_TYPE_GRADIENT, sizeof(struct xui_cmd_gradient_t), &r);
		cmd->gradient.x = x;
		cmd->gradient.y = y;
		cmd->gradient.w = w;
		cmd->gradient.h = h;
		memcpy(&cmd->gradient.lt, lt, sizeof(struct color_t));
		memcpy(&cmd->gradient.rt, rt, sizeof(struct color_t));
		memcpy(&cmd->gradient.rb, rb, sizeof(struct color_t));
		memcpy(&cmd->gradient.lb, lb, sizeof(struct color_t));
		if(clip < 0)
			xui_cmd_push_clip(ctx, &unlimited_region);
	}
}

void xui_draw_checkerboard(struct xui_context_t * ctx, int x, int y, int w, int h)
{
	union xui_cmd_t * cmd;
	struct region_t r;
	int clip;

	region_init(&r, x, y, w, h);
	if((clip = xui_check_clip(ctx, &r)))
	{
		if(clip < 0)
			xui_cmd_push_clip(ctx, xui_get_clip(ctx));
		cmd = xui_cmd_push(ctx, XUI_CMD_TYPE_CHECKERBOARD, sizeof(struct xui_cmd_checkerboard_t), &r);
		cmd->checkerboard.x = x;
		cmd->checkerboard.y = y;
		cmd->checkerboard.w = w;
		cmd->checkerboard.h = h;
		if(clip < 0)
			xui_cmd_push_clip(ctx, &unlimited_region);
	}
}

static void push_layout(struct xui_context_t * ctx, struct region_t * body, int scrollx, int scrolly)
{
	struct xui_layout_t layout;
	memset(&layout, 0, sizeof(layout));
	region_init(&layout.body, body->x - scrollx, body->y - scrolly, body->w, body->h);
	layout.max_width = INT_MIN;
	layout.max_height = INT_MIN;
	xui_push(ctx->layout_stack, layout);
	xui_layout_row(ctx, 1, (int[]){ 0 }, 0);
}

struct xui_container_t * get_container(struct xui_context_t * ctx, unsigned int id, int opt)
{
	struct xui_container_t * c;
	int idx = xui_pool_get(ctx, ctx->container_pool, XUI_CONTAINER_POOL_SIZE, id);
	if(idx >= 0)
	{
		if(ctx->containers[idx].open || (~opt & XUI_OPT_CLOSED))
			xui_pool_update(ctx, ctx->container_pool, idx);
		return &ctx->containers[idx];
	}
	if(opt & XUI_OPT_CLOSED)
		return NULL;
	idx = xui_pool_init(ctx, ctx->container_pool, XUI_CONTAINER_POOL_SIZE, id);
	c = &ctx->containers[idx];
	memset(c, 0, sizeof(struct xui_container_t));
	c->open = 1;
	xui_set_front(ctx, c);
	return c;
}

void push_container_body(struct xui_context_t * ctx, struct xui_container_t * c, struct region_t * body)
{
	struct region_t r;
	region_expand(&r, body, -ctx->style.layout.padding);
	push_layout(ctx, &r, c->scroll_x, c->scroll_y);
	region_clone(&c->body, body);
}

void pop_container(struct xui_context_t * ctx)
{
	struct xui_container_t * c = xui_get_container(ctx);
	struct xui_layout_t * layout = xui_get_layout(ctx);
	c->content_width = layout->max_width - layout->body.x;
	c->content_height = layout->max_height - layout->body.y;
	xui_pop(ctx->container_stack);
	xui_pop(ctx->layout_stack);
	xui_pop_id(ctx);
}

static int in_hover_root(struct xui_context_t * ctx)
{
	int i = ctx->container_stack.idx;
	while(i--)
	{
		if(ctx->container_stack.items[i] == ctx->hover_root)
			return 1;
		if(ctx->container_stack.items[i]->head)
			break;
	}
	return 0;
}

static int xui_mouse_over(struct xui_context_t * ctx, struct region_t * r)
{
	return region_hit(r, ctx->mouse.x, ctx->mouse.y) && region_hit(xui_get_clip(ctx), ctx->mouse.x, ctx->mouse.y) && in_hover_root(ctx);
}

void scroll_begin(struct xui_context_t * ctx, struct xui_container_t * c, int opt)
{
	int maxw, maxh;

	if(opt & XUI_OPT_NOSCROLL)
		c->noscroll = 1;
	else
	{
		maxw = c->content_width + ctx->style.layout.padding * 2 - c->body.w;
		if((maxw > 0) && (c->body.w > 0))
		{
			c->scroll_x = clamp(c->scroll_x, 0, maxw);
			if(xui_mouse_over(ctx, &c->body))
				ctx->scroll_target = c;
			else
			{
				c->scroll_vx = 0;
				c->scroll_vy = 0;
			}
		}
		else
		{
			c->scroll_x = 0;
			c->scroll_vx = 0;
		}
		maxh = c->content_height + ctx->style.layout.padding * 2 - c->body.h;
		if((maxh > 0) && (c->body.h > 0))
		{
			c->scroll_y = clamp(c->scroll_y, 0, maxh);
			if(xui_mouse_over(ctx, &c->body))
				ctx->scroll_target = c;
			else
			{
				c->scroll_vx = 0;
				c->scroll_vy = 0;
			}
		}
		else
		{
			c->scroll_y = 0;
			c->scroll_vy = 0;
		}
		c->noscroll = 0;
	}
}

void scroll_end(struct xui_context_t * ctx, struct xui_container_t * c)
{
	struct region_t r;
	struct color_t * color;
	int width, height;
	int maxw, maxh;

	if(!c->noscroll)
	{
		width = c->content_width + ctx->style.layout.padding * 2;
		maxw = width - c->body.w;
		if((maxw > 0) && (c->body.w > 0))
		{
			r.w = max(24, c->body.w * c->body.w / width);
			r.h = ctx->style.scroll.width;
			r.x = c->body.x + c->scroll_x * (c->body.w - r.w) / maxw;
			r.y = c->body.y + c->body.h - ctx->style.scroll.width;
			color = &ctx->style.scroll.color;
			if(color->a)
				xui_draw_rectangle(ctx, r.x, r.y, r.w, r.h, ctx->style.scroll.radius, 0, color);
		}
		height = c->content_height + ctx->style.layout.padding * 2;
		maxh = height - c->body.h;
		if((maxh > 0) && (c->body.h > 0))
		{
			r.w = ctx->style.scroll.width;
			r.h = max(24, c->body.h * c->body.h / height);
			r.x = c->body.x + c->body.w - ctx->style.scroll.width;
			r.y = c->body.y + c->scroll_y * (c->body.h - r.h) / maxh;
			color = &ctx->style.scroll.color;
			if(color->a)
				xui_draw_rectangle(ctx, r.x, r.y, r.w, r.h, ctx->style.scroll.radius, 0, color);
		}
	}
}

void xui_control_update(struct xui_context_t * ctx, unsigned int id, struct region_t * r, int opt)
{
	if(~opt & XUI_OPT_NOINTERACT)
	{
		if(!ctx->mouse.state && (ctx->active == id) && (~opt & XUI_OPT_HOLDFOCUS))
			ctx->active = 0;
		if(xui_mouse_over(ctx, r))
		{
			if((ctx->mouse.up || ctx->mouse.down) && (!ctx->aflag || !ctx->active))
			{
				ctx->active = id;
				ctx->aflag = 1;
			}
			if(!ctx->mouse.state && (!ctx->hflag || !ctx->hover))
			{
				ctx->hover = id;
				ctx->hflag = 1;
			}
		}
		else
		{
			if((ctx->mouse.up || ctx->mouse.down) && (ctx->active == id))
				ctx->active = 0;
			if(ctx->hover == id)
				ctx->hover = 0;
		}
	}
}

void xui_layout_width(struct xui_context_t * ctx, int width)
{
	xui_get_layout(ctx)->size_width = width;
}

void xui_layout_height(struct xui_context_t * ctx, int height)
{
	xui_get_layout(ctx)->size_height = height;
}

void xui_layout_row(struct xui_context_t * ctx, int items, const int * widths, int height)
{
	struct xui_layout_t * layout = xui_get_layout(ctx);
	if(widths)
	{
		assert(items <= XUI_MAX_WIDTHS);
		memcpy(layout->widths, widths, items * sizeof(int));
	}
	layout->items = items;
	layout->item_index = 0;
	layout->position_x = layout->indent;
	layout->position_y = layout->next_row;
	layout->size_height = height;
}

void xui_layout_begin_column(struct xui_context_t * ctx)
{
	push_layout(ctx, xui_layout_next(ctx), 0, 0);
}

void xui_layout_end_column(struct xui_context_t * ctx)
{
	struct xui_layout_t * a, * b;
	b = xui_get_layout(ctx);
	xui_pop(ctx->layout_stack);
	a = xui_get_layout(ctx);
	a->position_x = max(a->position_x, b->position_x + b->body.x - a->body.x);
	a->next_row = max(a->next_row, b->next_row + b->body.y - a->body.y);
	a->max_width = max(a->max_width, b->max_width);
	a->max_height = max(a->max_height, b->max_height);
}

void xui_layout_set_next(struct xui_context_t * ctx, struct region_t * r, int relative)
{
	struct xui_layout_t * layout = xui_get_layout(ctx);
	region_clone(&layout->next, r);
	layout->next_type = relative ? 1 : 2;
}

struct region_t * xui_layout_next(struct xui_context_t * ctx)
{
	struct xui_layout_t * layout = xui_get_layout(ctx);
	struct xui_style_t * style = &ctx->style;
	struct region_t r;

	if(layout->next_type)
	{
		int type = layout->next_type;
		layout->next_type = 0;
		region_clone(&r, &layout->next);
		if(type == 2)
		{
			region_clone(&ctx->last_rect, &r);
			return &ctx->last_rect;
		}
	}
	else
	{
		if(layout->item_index == layout->items)
			xui_layout_row(ctx, layout->items, NULL, layout->size_height);
		r.x = layout->position_x;
		r.y = layout->position_y;
		r.w = layout->items > 0 ? layout->widths[layout->item_index] : layout->size_width;
		r.h = layout->size_height;
		if(r.w == 0)
			r.w = style->layout.width + style->layout.padding * 2;
		if(r.h == 0)
			r.h = style->layout.height + style->layout.padding * 2;
		if(r.w < 0)
			r.w += layout->body.w - r.x + 1;
		if(r.h < 0)
			r.h += layout->body.h - r.y + 1;
		layout->item_index++;
	}

	layout->position_x += r.w + style->layout.spacing;
	layout->next_row = max(layout->next_row, r.y + r.h + style->layout.spacing);
	r.x += layout->body.x;
	r.y += layout->body.y;
	layout->max_width = max(layout->max_width, r.x + r.w);
	layout->max_height = max(layout->max_height, r.y + r.h);

	region_clone(&ctx->last_rect, &r);
	return &ctx->last_rect;
}

struct xui_context_t * xui_context_alloc(const char * fb, const char * input, void * data)
{
	struct xui_context_t * ctx;
	int len;

	ctx = malloc(sizeof(struct xui_context_t));
	if(!ctx)
		return NULL;

	memset(ctx, 0, sizeof(struct xui_context_t));
	ctx->w = window_alloc(fb, input);
	ctx->f = font_context_alloc();
	ctx->lang = NULL;
	ctx->surface = NULL;
	region_init(&ctx->screen, 0, 0, window_get_width(ctx->w), window_get_height(ctx->w));
	ctx->cpshift = 7;
	ctx->cpsize = 1 << ctx->cpshift;
	ctx->cwidth = (ctx->screen.w >> ctx->cpshift) + 1;
	ctx->cheight = (ctx->screen.h >> ctx->cpshift) + 1;
	len = ctx->cwidth * ctx->cheight * sizeof(unsigned int);
	ctx->cells[0] = malloc(len);
	ctx->cells[1] = malloc(len);
	if(!ctx->cells[0] || !ctx->cells[1])
	{
		if(ctx->cells[0])
			free(ctx->cells[0]);
		if(ctx->cells[1])
			free(ctx->cells[1]);
		free(ctx);
	}
	memset(ctx->cells[0], 0xff, len);
	memset(ctx->cells[1], 0xff, len);
	ctx->cindex = 0;
	ctx->running = 1;
	region_clone(&ctx->clip, &ctx->screen);
	xui_load_style(ctx, style_default, sizeof(style_default));
	ctx->stamp = ktime_get();
	ctx->priv = data;

	return ctx;
}

void xui_context_free(struct xui_context_t * ctx)
{
	if(ctx)
	{
		window_free(ctx->w);
		font_context_free(ctx->f);
		if(ctx->lang)
			hmap_free(ctx->lang);
		if(ctx->surface)
			lru_free(ctx->surface);
		if(ctx->cells[0])
			free(ctx->cells[0]);
		if(ctx->cells[1])
			free(ctx->cells[1]);
		free(ctx);
	}
}

static void style_widget_color(struct json_value_t * v, struct xui_widget_color_t * wc)
{
	struct json_value_t * o;
	int i;

	if(v && (v->type == JSON_OBJECT))
	{
		for(i = 0; i < v->u.object.length; i++)
		{
			o = v->u.object.values[i].value;
			switch(shash(v->u.object.values[i].name))
			{
			case 0x52d1c547: /* "normal-bakcground-color" */
				if(o && (o->type == JSON_STRING))
					color_init_string(&wc->normal.background, o->u.string.ptr);
				break;
			case 0xaf0e03c2: /* "normal-foreground-color" */
				if(o && (o->type == JSON_STRING))
					color_init_string(&wc->normal.foreground, o->u.string.ptr);
				break;
			case 0xa0d36ca5: /* "normal-border-color" */
				if(o && (o->type == JSON_STRING))
					color_init_string(&wc->normal.border, o->u.string.ptr);
				break;

			case 0xd8fbc302: /* "hover-bakcground-color" */
				if(o && (o->type == JSON_STRING))
					color_init_string(&wc->hover.background, o->u.string.ptr);
				break;
			case 0x3538017d: /* "hover-foreground-color" */
				if(o && (o->type == JSON_STRING))
					color_init_string(&wc->hover.foreground, o->u.string.ptr);
				break;
			case 0xf165c4e0: /* "hover-border-color" */
				if(o && (o->type == JSON_STRING))
					color_init_string(&wc->hover.border, o->u.string.ptr);
				break;

			case 0x38d1e5da: /* "active-bakcground-color" */
				if(o && (o->type == JSON_STRING))
					color_init_string(&wc->active.background, o->u.string.ptr);
				break;
			case 0x950e2455: /* "active-foreground-color" */
				if(o && (o->type == JSON_STRING))
					color_init_string(&wc->active.foreground, o->u.string.ptr);
				break;
			case 0x37ac3bb8: /* "active-border-color" */
				if(o && (o->type == JSON_STRING))
					color_init_string(&wc->active.border, o->u.string.ptr);
				break;

			default:
				break;
			}
		}
	}
}

void xui_load_style(struct xui_context_t * ctx, const char * json, int len)
{
	struct json_value_t * v, * o;
	int i;

	if(json && (len > 0))
	{
		v = json_parse(json, len, NULL);
		if(v && (v->type == JSON_OBJECT))
		{
			for(i = 0; i < v->u.object.length; i++)
			{
				o = v->u.object.values[i].value;
				switch(shash(v->u.object.values[i].name))
				{
				case 0xc2cfc789: /* "primary" */
					style_widget_color(o, &ctx->style.primary);
					break;
				case 0x4bad706d: /* "secondary" */
					style_widget_color(o, &ctx->style.secondary);
					break;
				case 0xb04bf9fe: /* "success" */
					style_widget_color(o, &ctx->style.success);
					break;
				case 0x7c9884d1: /* "info" */
					style_widget_color(o, &ctx->style.info);
					break;
				case 0xb6a3487b: /* "warning" */
					style_widget_color(o, &ctx->style.warning);
					break;
				case 0xf83c41d6: /* "danger" */
					style_widget_color(o, &ctx->style.danger);
					break;
				case 0xf5e7082b: /* "cancel" */
					style_widget_color(o, &ctx->style.cancel);
					break;

				case 0xb1c870bd: /* "icon-family" */
					if(o && (o->type == JSON_STRING))
						strlcpy(ctx->style.font.icon_family, o->u.string.ptr, sizeof(ctx->style.font.icon_family));
					break;
				case 0x673faacb: /* "font-family" */
					if(o && (o->type == JSON_STRING))
						strlcpy(ctx->style.font.font_family, o->u.string.ptr, sizeof(ctx->style.font.font_family));
					break;
				case 0x1005def6: /* "text-color" */
					if(o && (o->type == JSON_STRING))
						color_init_string(&ctx->style.font.color, o->u.string.ptr);
					break;
				case 0xf1c88f84: /* "font-size" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.font.size = o->u.integer;
					break;

				case 0x3f791630: /* "layout-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.layout.width = o->u.integer;
					break;
				case 0x0b589fc9: /* "layout-height" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.layout.height = o->u.integer;
					break;
				case 0xd48dc0a7: /* "layout-padding" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.layout.padding = o->u.integer;
					break;
				case 0xde430275: /* "layout-spacing" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.layout.spacing = o->u.integer;
					break;
				case 0x0e4ddf52: /* "layout-indent" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.layout.indent = o->u.integer;
					break;

				case 0xb2771add: /* "window-border-radius" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.window.border_radius = o->u.integer;
					break;
				case 0x05c75415: /* "window-border-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.window.border_width = o->u.integer;
					break;
				case 0xcb989ef2: /* "window-title-height" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.window.title_height = o->u.integer;
					break;
				case 0xc2f19cd6: /* "window-background-color" */
					if(o && (o->type == JSON_STRING))
						color_init_string(&ctx->style.window.background_color, o->u.string.ptr);
					break;
				case 0x0460d5b4: /* "window-border-color" */
					if(o && (o->type == JSON_STRING))
						color_init_string(&ctx->style.window.border_color, o->u.string.ptr);
					break;
				case 0xd74ad5d8: /* "window-title-color" */
					if(o && (o->type == JSON_STRING))
						color_init_string(&ctx->style.window.title_color, o->u.string.ptr);
					break;
				case 0x8111065b: /* "window-text-color" */
					if(o && (o->type == JSON_STRING))
						color_init_string(&ctx->style.window.text_color, o->u.string.ptr);
					break;

				case 0x573ba135: /* "panel-border-radius" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.panel.border_radius = o->u.integer;
					break;
				case 0x96686f6d: /* "panel-border-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.panel.border_width = o->u.integer;
					break;
				case 0xaae0a42e: /* "panel-background-color" */
					if(o && (o->type == JSON_STRING))
						color_init_string(&ctx->style.panel.background_color, o->u.string.ptr);
					break;
				case 0x9501f10c: /* "panel-border-color" */
					if(o && (o->type == JSON_STRING))
						color_init_string(&ctx->style.panel.border_color, o->u.string.ptr);
					break;

				case 0xce6db481: /* "scroll-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.scroll.width = o->u.integer;
					break;
				case 0x8fe988c9: /* "scroll-radius" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.scroll.radius = o->u.integer;
					break;
				case 0xcd073620: /* "scroll-color" */
					if(o && (o->type == JSON_STRING))
						color_init_string(&ctx->style.scroll.color, o->u.string.ptr);
					break;

				case 0xa6c2fb38: /* "collapse-border-radius" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.collapse.border_radius = o->u.integer;
					break;
				case 0xbf9b1510: /* "collapse-border-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.collapse.border_width = o->u.integer;
					break;

				case 0xa2dc6c55: /* "tree-border-radius" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.tree.border_radius = o->u.integer;
					break;
				case 0xfd8c568d: /* "tree-border-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.tree.border_width = o->u.integer;
					break;

				case 0x421d2901: /* "button-border-radius" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.button.border_radius = o->u.integer;
					break;
				case 0xcc122db9: /* "button-border-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.button.border_width = o->u.integer;
					break;
				case 0xf2f7533b: /* "button-outline-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.button.outline_width = o->u.integer;
					break;

				case 0xd513cbcc: /* "checkbox-border-radius" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.checkbox.border_radius = o->u.integer;
					break;
				case 0xe00a2324: /* "checkbox-border-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.checkbox.border_width = o->u.integer;
					break;
				case 0x85edf606: /* "checkbox-outline-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.checkbox.outline_width = o->u.integer;
					break;

				case 0x375f9ccc: /* "radio-border-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.radio.border_width = o->u.integer;
					break;
				case 0xc7f2a4ae: /* "radio-outline-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.radio.outline_width = o->u.integer;
					break;

				case 0x940cdb1f: /* "toggle-border-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.toggle.border_width = o->u.integer;
					break;
				case 0xba47ad61: /* "toggle-outline-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.toggle.outline_width = o->u.integer;
					break;

				case 0x9d23c911: /* "tabbar-border-radius" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.tabbar.border_radius = o->u.integer;
					break;
				case 0x71bd0bc9: /* "tabbar-border-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.tabbar.border_width = o->u.integer;
					break;

				case 0x118b4b08: /* "slider-invalid-color" */
					if(o && (o->type == JSON_STRING))
						color_init_string(&ctx->style.slider.invalid_color, o->u.string.ptr);
					break;
				case 0xa5534dc0: /* "slider-border-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.slider.border_width = o->u.integer;
					break;

				case 0x1110bece: /* "number-border-radius" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.number.border_radius = o->u.integer;
					break;
				case 0xc2d3bde6: /* "number-border-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.number.border_width = o->u.integer;
					break;
				case 0xc1eae908: /* "number-outline-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.number.outline_width = o->u.integer;
					break;

				case 0x1f996970: /* "textedit-border-radius" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.textedit.border_radius = o->u.integer;
					break;
				case 0xb3c09c48: /* "textedit-border-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.textedit.border_width = o->u.integer;
					break;
				case 0xd07393aa: /* "textedit-outline-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.textedit.outline_width = o->u.integer;
					break;

				case 0x903d8118: /* "badge-border-radius" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.badge.border_radius = o->u.integer;
					break;
				case 0xbeec5ef0: /* "badge-border-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.badge.border_width = o->u.integer;
					break;
				case 0x4117ab52: /* "badge-outline-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.badge.outline_width = o->u.integer;
					break;

				case 0x98e7d09a: /* "progress-invalid-color" */
					if(o && (o->type == JSON_STRING))
						color_init_string(&ctx->style.progress.invalid_color, o->u.string.ptr);
					break;
				case 0xcadecf7a: /* "progress-border-radius" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.progress.border_radius = o->u.integer;
					break;

				case 0xc2184287: /* "radialbar-invalid-color" */
					if(o && (o->type == JSON_STRING))
						color_init_string(&ctx->style.radialbar.invalid_color, o->u.string.ptr);
					break;

				case 0xf72f0b7e: /* "split-width" */
					if(o && (o->type == JSON_INTEGER))
						ctx->style.split.width = o->u.integer;
					break;

				default:
					break;
				}
			}
		}
		json_free(v);
	}
}

static void hmap_entry_callback(struct hmap_t * m, struct hmap_entry_t * e)
{
	if(e && e->value)
		free(e->value);
}

void xui_load_lang(struct xui_context_t * ctx, const char * json, int len)
{
	struct json_value_t * v;
	char * key, * value;
	int i;

	if(json && (len > 0))
	{
		if(!ctx->lang)
			ctx->lang = hmap_alloc(0, hmap_entry_callback);
		if(ctx->lang)
		{
			hmap_clear(ctx->lang);
			v = json_parse(json, len, NULL);
			if(v && (v->type == JSON_OBJECT))
			{
				for(i = 0; i < v->u.object.length; i++)
				{
					if(v->u.object.values[i].value->type == JSON_STRING)
					{
						key = v->u.object.values[i].name;
						value = hmap_search(ctx->lang, key);
						if(value)
						{
							hmap_remove(ctx->lang, key);
							free(value);
						}
						hmap_add(ctx->lang, key, strdup(v->u.object.values[i].value->u.string.ptr));
					}
				}
			}
			json_free(v);
		}
	}
}

static void lru_callback(struct lru_t * l, const char * key, int nkey, void * buf, int nbuf)
{
	struct surface_t * s;

	if(nbuf == sizeof(struct surface_t *))
	{
		memcpy(&s, buf, nbuf);
		surface_free(s);
	}
}

struct surface_t * xui_load_surface(struct xui_context_t * ctx, const char * path)
{
	struct surface_t * s = NULL;

	if(!ctx->surface)
		ctx->surface = lru_alloc(0, 0, lru_callback);
	if(lru_get(ctx->surface, path, strlen(path), &s, sizeof(struct surface_t *)) != sizeof(struct surface_t *))
	{
		struct xfs_context_t * xfs;
		xfs = xfs_alloc("/private/framework", 0);
		if(xfs)
		{
			s = surface_alloc_from_xfs(xfs, path);
			if(s)
				lru_set(ctx->surface, path, strlen(path), &s, sizeof(struct surface_t *));
			xfs_free(xfs);
		}
	}
	return s;
}

void xui_add_font(struct xui_context_t * ctx,  const char * family, const char * path)
{
	if(ctx)
		font_add(ctx->f, NULL, family, path);
}

static void xui_draw(struct window_t * w, void * o)
{
	struct xui_context_t * ctx = (struct xui_context_t *)o;
	struct surface_t * s = ctx->w->s;
	struct region_t * r, * clip = &ctx->clip;
	union xui_cmd_t * cmd;
	struct matrix_t m;
	struct icon_t ico;
	int count, size;
	int i;

	if((count = w->rl->count) > 0)
	{
		for(i = 0; i < count; i++)
		{
			r = &w->rl->region[i];
			region_clone(clip, r);
			cmd = NULL;
			while(xui_cmd_next(ctx, &cmd))
			{
				switch(cmd->base.type)
				{
				case XUI_CMD_TYPE_CLIP:
					if(!region_intersect(clip, r, &cmd->clip.r))
						region_init(clip, 0, 0, 0, 0);
					break;
				case XUI_CMD_TYPE_LINE:
					surface_shape_line(s, clip, &cmd->line.p0, &cmd->line.p1, cmd->line.thickness, &cmd->line.c);
					break;
				case XUI_CMD_TYPE_POLYLINE:
					surface_shape_polyline(s, clip, cmd->polyline.p, cmd->polyline.n, cmd->polyline.thickness, &cmd->polyline.c);
					break;
				case XUI_CMD_TYPE_CURVE:
					surface_shape_curve(s, clip, cmd->curve.p, cmd->curve.n, cmd->curve.thickness, &cmd->curve.c);
					break;
				case XUI_CMD_TYPE_TRIANGLE:
					surface_shape_triangle(s, clip, &cmd->triangle.p0, &cmd->triangle.p1, &cmd->triangle.p2, cmd->triangle.thickness, &cmd->triangle.c);
					break;
				case XUI_CMD_TYPE_RECTANGLE:
					surface_shape_rectangle(s, clip, cmd->rectangle.x, cmd->rectangle.y, cmd->rectangle.w, cmd->rectangle.h, cmd->rectangle.radius, cmd->rectangle.thickness, &cmd->rectangle.c);
					break;
				case XUI_CMD_TYPE_POLYGON:
					surface_shape_polygon(s, clip, cmd->polygon.p, cmd->polygon.n, cmd->polygon.thickness, &cmd->polygon.c);
					break;
				case XUI_CMD_TYPE_CIRCLE:
					surface_shape_circle(s, clip, cmd->circle.x, cmd->circle.y, cmd->circle.radius, cmd->circle.thickness, &cmd->circle.c);
					break;
				case XUI_CMD_TYPE_ELLIPSE:
					surface_shape_ellipse(s, clip, cmd->ellipse.x, cmd->ellipse.y, cmd->ellipse.w, cmd->ellipse.h, cmd->ellipse.thickness, &cmd->ellipse.c);
					break;
				case XUI_CMD_TYPE_ARC:
					surface_shape_arc(s, clip, cmd->arc.x, cmd->arc.y, cmd->arc.radius, cmd->arc.a1, cmd->arc.a2, cmd->arc.thickness, &cmd->arc.c);
					break;
				case XUI_CMD_TYPE_SURFACE:
					surface_blit(s, clip, &cmd->surface.m, cmd->surface.s, RENDER_TYPE_GOOD);
					break;
				case XUI_CMD_TYPE_ICON:
					size = min(cmd->icon.w, cmd->icon.h);
					icon_init(&ico, cmd->icon.code, &cmd->icon.c, ctx->f, cmd->icon.family, size);
					matrix_init_translate(&m, cmd->icon.x + (cmd->icon.w - size) / 2, cmd->icon.y + (cmd->icon.h - size) / 2);
					surface_icon(s, clip, &m, &ico);
					break;
				case XUI_CMD_TYPE_TEXT:
					matrix_init_translate(&m, cmd->text.x, cmd->text.y);
					surface_text(s, clip, &m, &cmd->text.txt);
					break;
				case XUI_CMD_TYPE_GLASS:
					surface_effect_glass(s, clip, cmd->glass.x, cmd->glass.y, cmd->glass.w, cmd->glass.h, cmd->glass.radius);
					break;
				case XUI_CMD_TYPE_SHADOW:
					surface_effect_shadow(s, clip, cmd->shadow.x, cmd->shadow.y, cmd->shadow.w, cmd->shadow.h, cmd->shadow.radius, &cmd->shadow.c);
					break;
				case XUI_CMD_TYPE_GRADIENT:
					surface_effect_gradient(s, clip, cmd->gradient.x, cmd->gradient.y, cmd->gradient.w, cmd->gradient.h, &cmd->gradient.lt, &cmd->gradient.rt, &cmd->gradient.rb, &cmd->gradient.lb);
					break;
				case XUI_CMD_TYPE_CHECKERBOARD:
					surface_effect_checkerboard(s, clip, cmd->checkerboard.x, cmd->checkerboard.y, cmd->checkerboard.w, cmd->checkerboard.h);
					break;
				default:
					break;
				}
			}
		}
	}
}

void xui_loop(struct xui_context_t * ctx, void (*func)(struct xui_context_t *))
{
	struct event_t e;
	int64_t delta;
	char utf8[16];
	int l, sz;

	while(ctx->running)
	{
		while(window_pump_event(ctx->w, &e))
		{
			switch(e.type)
			{
			case EVENT_TYPE_KEY_DOWN:
				switch(e.e.key_down.key)
				{
				case KEY_POWER:
					ctx->key_down |= XUI_KEY_POWER;
					ctx->key_pressed |= XUI_KEY_POWER;
					break;
				case KEY_UP:
					ctx->key_down |= XUI_KEY_UP;
					ctx->key_pressed |= XUI_KEY_UP;
					break;
				case KEY_DOWN:
					ctx->key_down |= XUI_KEY_DOWN;
					ctx->key_pressed |= XUI_KEY_DOWN;
					break;
				case KEY_LEFT:
					ctx->key_down |= XUI_KEY_LEFT;
					ctx->key_pressed |= XUI_KEY_LEFT;
					break;
				case KEY_RIGHT:
					ctx->key_down |= XUI_KEY_RIGHT;
					ctx->key_pressed |= XUI_KEY_RIGHT;
					break;
				case KEY_VOLUME_UP:
					ctx->key_down |= XUI_KEY_VOLUME_UP;
					ctx->key_pressed |= XUI_KEY_VOLUME_UP;
					break;
				case KEY_VOLUME_DOWN:
					ctx->key_down |= XUI_KEY_VOLUME_DOWN;
					ctx->key_pressed |= XUI_KEY_VOLUME_DOWN;
					break;
				case KEY_VOLUME_MUTE:
					ctx->key_down |= XUI_KEY_VOLUME_MUTE;
					ctx->key_pressed |= XUI_KEY_VOLUME_MUTE;
					break;
				case KEY_TAB:
					ctx->key_down |= XUI_KEY_TAB;
					ctx->key_pressed |= XUI_KEY_TAB;
					break;
				case KEY_HOME:
					ctx->key_down |= XUI_KEY_HOME;
					ctx->key_pressed |= XUI_KEY_HOME;
					break;
				case KEY_BACK:
					ctx->key_down |= XUI_KEY_BACK;
					ctx->key_pressed |= XUI_KEY_BACK;
					break;
				case KEY_MENU:
					ctx->key_down |= XUI_KEY_MENU;
					ctx->key_pressed |= XUI_KEY_MENU;
					break;
				case KEY_ENTER:
					ctx->key_down |= XUI_KEY_ENTER;
					ctx->key_pressed |= XUI_KEY_ENTER;
					break;
				case KEY_L_CTRL:
				case KEY_R_CTRL:
					ctx->key_down |= XUI_KEY_CTRL;
					ctx->key_pressed |= XUI_KEY_CTRL;
					break;
				case KEY_L_ALT:
				case KEY_R_ALT:
					ctx->key_down |= XUI_KEY_ALT;
					ctx->key_pressed |= XUI_KEY_ALT;
					break;
				case KEY_L_SHIFT:
				case KEY_R_SHIFT:
					ctx->key_down |= XUI_KEY_SHIFT;
					ctx->key_pressed |= XUI_KEY_SHIFT;
					break;
				default:
					if(e.e.key_up.key >= KEY_SPACE)
					{
						ucs4_to_utf8(&e.e.key_up.key, 1, utf8, sizeof(utf8));
						l = strlen(ctx->input_text);
						sz = strlen(utf8) + 1;
						if(l + sz <= sizeof(ctx->input_text))
							memcpy(ctx->input_text + l, utf8, sz);
					}
					break;
				}
				break;
			case EVENT_TYPE_KEY_UP:
				switch(e.e.key_up.key)
				{
				case KEY_POWER:
					ctx->key_down &= ~XUI_KEY_POWER;
					break;
				case KEY_UP:
					ctx->key_down &= ~XUI_KEY_UP;
					break;
				case KEY_DOWN:
					ctx->key_down &= ~XUI_KEY_DOWN;
					break;
				case KEY_LEFT:
					ctx->key_down &= ~XUI_KEY_LEFT;
					break;
				case KEY_RIGHT:
					ctx->key_down &= ~XUI_KEY_RIGHT;
					break;
				case KEY_VOLUME_UP:
					ctx->key_down &= ~XUI_KEY_VOLUME_UP;
					break;
				case KEY_VOLUME_DOWN:
					ctx->key_down &= ~XUI_KEY_VOLUME_DOWN;
					break;
				case KEY_VOLUME_MUTE:
					ctx->key_down &= ~XUI_KEY_VOLUME_MUTE;
					break;
				case KEY_TAB:
					ctx->key_down &= ~XUI_KEY_TAB;
					break;
				case KEY_HOME:
					ctx->key_down &= ~XUI_KEY_HOME;
					break;
				case KEY_BACK:
					ctx->key_down &= ~XUI_KEY_BACK;
					break;
				case KEY_MENU:
					ctx->key_down &= ~XUI_KEY_MENU;
					break;
				case KEY_ENTER:
					ctx->key_down &= ~XUI_KEY_ENTER;
					break;
				case KEY_L_CTRL:
				case KEY_R_CTRL:
					ctx->key_down &= ~XUI_KEY_CTRL;
					break;
				case KEY_L_ALT:
				case KEY_R_ALT:
					ctx->key_down &= ~XUI_KEY_ALT;
					break;
				case KEY_L_SHIFT:
				case KEY_R_SHIFT:
					ctx->key_down &= ~XUI_KEY_SHIFT;
					break;
				default:
					break;
				}
				break;
			case EVENT_TYPE_MOUSE_DOWN:
				ctx->mouse.x = e.e.mouse_down.x;
				ctx->mouse.y = e.e.mouse_down.y;
				ctx->mouse.state |= e.e.mouse_down.button;
				ctx->mouse.down |= e.e.mouse_down.button;
				ctx->mouse.tx = e.e.mouse_down.x;
				ctx->mouse.ty = e.e.mouse_down.y;
				ctx->mouse.tdown = e.timestamp;
				break;
			case EVENT_TYPE_MOUSE_MOVE:
				ctx->mouse.x = e.e.mouse_move.x;
				ctx->mouse.y = e.e.mouse_move.y;
				ctx->mouse.tmove = e.timestamp;
				break;
			case EVENT_TYPE_MOUSE_UP:
				ctx->mouse.x = e.e.mouse_up.x;
				ctx->mouse.y = e.e.mouse_up.y;
				ctx->mouse.state &= ~e.e.mouse_up.button;
				ctx->mouse.up |= e.e.mouse_up.button;
				if(ktime_to_ns(ktime_sub(e.timestamp, ctx->mouse.tmove)) < 50000000LL)
				{
					delta = ktime_to_ns(ktime_sub(e.timestamp, ctx->mouse.tdown));
					if(delta > 0)
					{
						ctx->mouse.vx = (e.e.mouse_up.x - ctx->mouse.tx) * 1000000000LL / delta;
						ctx->mouse.vy = (e.e.mouse_up.y - ctx->mouse.ty) * 1000000000LL / delta;
					}
				}
				break;
			case EVENT_TYPE_MOUSE_WHEEL:
				ctx->mouse.zx += e.e.mouse_wheel.dx;
				ctx->mouse.zy += e.e.mouse_wheel.dy;
				break;
			case EVENT_TYPE_TOUCH_BEGIN:
				if(e.e.touch_begin.id == 0)
				{
					ctx->mouse.ox = ctx->mouse.x = e.e.touch_begin.x;
					ctx->mouse.oy = ctx->mouse.y = e.e.touch_begin.y;
					ctx->mouse.state |= MOUSE_BUTTON_LEFT;
					ctx->mouse.down |= MOUSE_BUTTON_LEFT;
					ctx->mouse.tx = e.e.touch_begin.x;
					ctx->mouse.ty = e.e.touch_begin.y;
					ctx->mouse.tdown = e.timestamp;
				}
				break;
			case EVENT_TYPE_TOUCH_MOVE:
				if(e.e.touch_move.id == 0)
				{
					ctx->mouse.x = e.e.touch_move.x;
					ctx->mouse.y = e.e.touch_move.y;
					ctx->mouse.tmove = e.timestamp;
				}
				break;
			case EVENT_TYPE_TOUCH_END:
				if(e.e.touch_end.id == 0)
				{
					ctx->mouse.x = e.e.touch_end.x;
					ctx->mouse.y = e.e.touch_end.y;
					ctx->mouse.state &= ~MOUSE_BUTTON_LEFT;
					ctx->mouse.up |= MOUSE_BUTTON_LEFT;
					if(ktime_to_ns(ktime_sub(e.timestamp, ctx->mouse.tmove)) < 50000000LL)
					{
						delta = ktime_to_ns(ktime_sub(e.timestamp, ctx->mouse.tdown));
						if(delta > 0)
						{
							ctx->mouse.vx = (e.e.touch_end.x - ctx->mouse.tx) * 1000000000LL / delta;
							ctx->mouse.vy = (e.e.touch_end.y - ctx->mouse.ty) * 1000000000LL / delta;
						}
					}
				}
				break;
			case EVENT_TYPE_SYSTEM_EXIT:
				xui_exit(ctx);
				break;
			default:
				break;
			}
		}
		if(func)
			func(ctx);
		if(window_is_active(ctx->w))
			window_present(ctx->w, ctx, xui_draw);
		task_yield();
	}
}
