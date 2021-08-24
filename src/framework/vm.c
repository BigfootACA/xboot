/*
 * framework/vm.c
 *
 * Copyright(c) 2007-2021 Jianjun Jiang <8192542@qq.com>
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

#include <xfs/xfs.h>
#include <luahelper.h>
#include <core/l-application.h>
#include <core/l-assets.h>
#include <core/l-class.h>
#include <core/l-color.h>
#include <core/l-display-icon.h>
#include <core/l-display-image.h>
#include <core/l-display-ninepatch.h>
#include <core/l-display-object.h>
#include <core/l-display-pager.h>
#include <core/l-display-scroll.h>
#include <core/l-display-text.h>
#include <core/l-dobject.h>
#include <core/l-easing.h>
#include <core/l-event.h>
#include <core/l-event-dispatcher.h>
#include <core/l-i18n.h>
#include <core/l-icon.h>
#include <core/l-image.h>
#include <core/l-matrix.h>
#include <core/l-ninepatch.h>
#include <core/l-printr.h>
#include <core/l-setting.h>
#include <core/l-spring.h>
#include <core/l-stage.h>
#include <core/l-stopwatch.h>
#include <core/l-text.h>
#include <core/l-timer.h>
#include <core/l-vision.h>
#include <core/l-window.h>
#include <core/l-xfs.h>
#include <codec/l-codec.h>
#include <hardware/l-hardware.h>
#include <vm.h>

static void luaopen_glblibs(lua_State * L)
{
	const luaL_Reg glblibs[] = {
		{ "Class",					luaopen_class },
		{ "Printr",					luaopen_printr },
		{ "Setting",				luaopen_setting },
		{ "Xfs",					luaopen_xfs },
		{ "Window",					luaopen_window },
		{ "I18n",					luaopen_i18n },
		{ "Easing",					luaopen_easing },
		{ "Spring",					luaopen_spring },
		{ "Stopwatch",				luaopen_stopwatch },
		{ "Color",					luaopen_color },
		{ "Matrix",					luaopen_matrix },
		{ "Image",					luaopen_image },
		{ "Ninepatch",				luaopen_ninepatch },
		{ "Text",					luaopen_text },
		{ "Icon",					luaopen_icon },
		{ "Dobject",				luaopen_dobject },
		{ "Event",					luaopen_event },
		{ "EventDispatcher",		luaopen_event_dispatcher },
		{ "DisplayObject",			luaopen_display_object },
		{ "DisplayPager",			luaopen_display_pager },
		{ "DisplayScroll",			luaopen_display_scroll },
		{ "DisplayImage",			luaopen_display_image },
		{ "DisplayNinepatch",		luaopen_display_ninepatch },
		{ "DisplayText",			luaopen_display_text },
		{ "DisplayIcon",			luaopen_display_icon },
		{ "Timer",					luaopen_timer },
		{ "Vision",					luaopen_vision },
		{ "Stage",					luaopen_stage },
		{ "Assets",					luaopen_assets },
		{ "Application",			luaopen_application },
		{ NULL,	NULL },
	};
	const luaL_Reg * lib;

	for(lib = glblibs; lib->func; lib++)
	{
		luaL_requiref(L, lib->name, lib->func, 1);
		lua_pop(L, 1);
	}
}

static void luaopen_prelibs(lua_State * L)
{
	const luaL_Reg prelibs[] = {
		{ "codec.base64",			luaopen_base64 },
		{ "codec.json",				luaopen_cjson_safe },

		{ "hardware.adc",			luaopen_hardware_adc },
		{ "hardware.battery",		luaopen_hardware_battery },
		{ "hardware.buzzer",		luaopen_hardware_buzzer },
		{ "hardware.compass",		luaopen_hardware_compass },
		{ "hardware.dac",			luaopen_hardware_dac },
		{ "hardware.gmeter",		luaopen_hardware_gmeter },
		{ "hardware.gpio",			luaopen_hardware_gpio },
		{ "hardware.gyroscope",		luaopen_hardware_gyroscope },
		{ "hardware.hygrometer",	luaopen_hardware_hygrometer },
		{ "hardware.i2c",			luaopen_hardware_i2c },
		{ "hardware.led",			luaopen_hardware_led },
		{ "hardware.ledstrip",		luaopen_hardware_ledstrip },
		{ "hardware.ledtrigger",	luaopen_hardware_ledtrigger },
		{ "hardware.light",			luaopen_hardware_light },
		{ "hardware.motor",			luaopen_hardware_motor },
		{ "hardware.nvmem",			luaopen_hardware_nvmem },
		{ "hardware.pressure",		luaopen_hardware_pressure },
		{ "hardware.proximity",		luaopen_hardware_proximity },
		{ "hardware.pwm",			luaopen_hardware_pwm },
		{ "hardware.servo",			luaopen_hardware_servo },
		{ "hardware.spi",			luaopen_hardware_spi },
		{ "hardware.stepper",		luaopen_hardware_stepper },
		{ "hardware.thermometer",	luaopen_hardware_thermometer },
		{ "hardware.uart",			luaopen_hardware_uart },
		{ "hardware.vibrator",		luaopen_hardware_vibrator },
		{ "hardware.watchdog",		luaopen_hardware_watchdog },

		{ NULL, NULL },
	};
	const luaL_Reg * lib;

	for(lib = prelibs; lib->func; lib++)
	{
		luahelper_preload(L, lib->name, lib->func);
	}
}

static const char boot_lua[] = X(
	stage = Stage.new()
	assets = Assets.new()
	T = I18n.new(Setting.get("language", "en-US"))
	if require("main") then
		stage:loop()
	end
);

static int luaopen_boot(lua_State * L)
{
	if(luaL_loadbuffer(L, boot_lua, sizeof(boot_lua) - 1, "Boot.lua") == LUA_OK)
		lua_call(L, 0, 0);
	return 0;
}

struct reader_data_t
{
	struct xfs_file_t * file;
	char buffer[LUAL_BUFFERSIZE];
};

static const char * reader(lua_State * L, void * data, size_t * size)
{
	struct reader_data_t * rd = (struct reader_data_t *)data;
	s64_t ret;

	ret = xfs_read(rd->file, rd->buffer, LUAL_BUFFERSIZE);
	if(ret < 0)
	{
		lua_error(L);
		return NULL;
	}

	*size = (size_t)ret;
	return rd->buffer;
}

static int l_loadfile(lua_State * L)
{
	struct xfs_context_t * ctx = ((struct vmctx_t *)luahelper_vmctx(L))->xfs;
	const char * filename = luaL_optstring(L, 1, NULL);
	struct reader_data_t * rd;

	rd = malloc(sizeof(struct reader_data_t));
	if(!rd)
	{
		lua_pushnil(L);
		lua_pushfstring(L, "cannot malloc memory", filename);
		return 2;
	}

	rd->file = xfs_open_read(ctx, filename);
	if(!rd->file)
	{
		free(rd);
		lua_pushnil(L);
		lua_pushfstring(L, "cannot open %s", filename);
		return 2;
	}

	if(lua_load(L, reader, rd, filename, NULL))
	{
		free(rd);
		lua_pushnil(L);
		lua_pushfstring(L, "cannot read %s", filename);
		return 2;
	}

	xfs_close(rd->file);
	free(rd);
	return 1;
}

static int dofilecont(lua_State * L, int d1, lua_KContext d2)
{
	return lua_gettop(L) - 1;
}

static int l_dofile(lua_State * L)
{
	lua_settop(L, 1);
	if(l_loadfile(L) != 1)
		return lua_error(L);
	lua_callk(L, 0, LUA_MULTRET, 0, dofilecont);
	return dofilecont(L, 0, 0);
}

static int l_search_package_lua(lua_State * L)
{
	struct xfs_context_t * ctx = ((struct vmctx_t *)luahelper_vmctx(L))->xfs;
	const char * filename = lua_tostring(L, -1);
	char * buf;
	size_t len, i;

	len = strlen(filename);
	buf = malloc(len + 16);
	if(!buf)
		return lua_error(L);

	strcpy(buf, filename);
	for(i = 0; i < len; i++)
	{
		if(buf[i] == '.')
			buf[i] = '/';
	}

	if(xfs_isdir(ctx, buf))
		strcat(buf, "/init.lua");
	else
		strcat(buf, ".lua");

	if(xfs_isfile(ctx, buf))
	{
		lua_pop(L, 1);
		lua_pushcfunction(L, l_loadfile);
		lua_pushstring(L, buf);
		lua_call(L, 1, 1);
	}
	else
	{
		lua_pushfstring(L, "\r\n\tno file '%s' in application directories", buf);
	}

	free(buf);
	return 1;
}

static int l_xboot_version(lua_State * L)
{
	lua_pushstring(L, xboot_version_string());
	return 1;
}

static int l_xboot_banner(lua_State * L)
{
	struct machine_t * mach = get_machine();
	char buf[SZ_4K];
	sprintf(buf, "%s - [%s][%s]", xboot_banner_string(), mach->name, mach->desc);
	lua_pushstring(L, buf);
	return 1;
}

static int l_xboot_shutdown(lua_State * L)
{
	machine_shutdown();
	return 0;
}

static int l_xboot_reboot(lua_State * L)
{
	machine_reboot();
	return 0;
}

static int l_xboot_sleep(lua_State * L)
{
	machine_sleep();
	return 0;
}

static int l_xboot_uniqueid(lua_State * L)
{
	lua_pushstring(L, machine_uniqueid());
	return 1;
}

static int l_xboot_keygen(lua_State * L)
{
	const char * msg = luaL_optstring(L, 1, "");
	char key[32];
	if(machine_keygen(msg, key))
		lua_pushlstring(L, key, 32);
	else
		lua_pushnil(L);
	return 1;
}

static int pmain(lua_State * L)
{
	luaL_openlibs(L);
	luaopen_glblibs(L);
	luaopen_prelibs(L);

	lua_pushcfunction(L, l_loadfile);
	lua_pushvalue(L, -1);
	lua_setglobal(L, "loadfile");

	lua_pushcfunction(L, l_dofile);
	lua_pushvalue(L, -1);
	lua_setglobal(L, "dofile");

	luahelper_package_searcher(L, l_search_package_lua, 2);
	luahelper_package_path(L, "./?/init.lua;./?.lua");
	luahelper_package_cpath(L, "./?.so");

	lua_getglobal(L, "xboot");
	if(!lua_istable(L, -1))
	{
		lua_pop(L, 1);
		lua_newtable(L);
		lua_pushvalue(L, -1);
		lua_setglobal(L, "xboot");
	}
	lua_pushcfunction(L, l_xboot_version);
	lua_setfield(L, -2, "version");
	lua_pushcfunction(L, l_xboot_banner);
	lua_setfield(L, -2, "banner");
	lua_pushcfunction(L, l_xboot_shutdown);
	lua_setfield(L, -2, "shutdown");
	lua_pushcfunction(L, l_xboot_reboot);
	lua_setfield(L, -2, "reboot");
	lua_pushcfunction(L, l_xboot_sleep);
	lua_setfield(L, -2, "sleep");
	lua_pushcfunction(L, l_xboot_uniqueid);
	lua_setfield(L, -2, "uniqueid");
	lua_pushcfunction(L, l_xboot_keygen);
	lua_setfield(L, -2, "keygen");

	luaopen_boot(L);
	return 0;
}

static void * l_alloc(void * ud, void * ptr, size_t osize, size_t nsize)
{
	if(nsize == 0)
	{
		free(ptr);
		return NULL;
	}
	else
	{
		return realloc(ptr, nsize);
	}
}

static int l_panic(lua_State *L)
{
	lua_writestringerror("PANIC: unprotected error in call to Lua API (%s)\r\n", lua_tostring(L, -1));
	return 0;
}

static lua_State * l_newstate(void * ud)
{
	lua_State * L = lua_newstate(l_alloc, ud);
	if(L)
		lua_atpanic(L, &l_panic);
	return L;
}

static struct vmctx_t * vmctx_alloc(const char * path, const char * fb, const char * input, void * data)
{
	struct vmctx_t * ctx;

	ctx = malloc(sizeof(struct vmctx_t));
	if(!ctx)
		return NULL;

	ctx->xfs = xfs_alloc(path, 1);
	ctx->f = font_context_alloc();
	ctx->w = window_alloc(fb, input);
	ctx->priv = data;

	return ctx;
}

static void vmctx_free(struct vmctx_t * ctx)
{
	if(!ctx)
		return;

	xfs_free(ctx->xfs);
	font_context_free(ctx->f);
	window_free(ctx->w);
	free(ctx);
}

static void vm_task(struct task_t * task, void * data)
{
	struct task_data_t * td = (struct task_data_t *)data;
	struct vmctx_t * ctx;
	lua_State * L;

	if(td)
	{
		ctx = vmctx_alloc(task->name, td->fb, td->input, td);
		if(ctx)
		{
			L = l_newstate(ctx);
			if(L)
			{
				lua_pushcfunction(L, &pmain);
				if(luahelper_pcall(L, 0, 0) != LUA_OK)
				{
					lua_writestringerror("%s: ", task->name);
					lua_writestringerror("%s\r\n", lua_tostring(L, -1));
					lua_pop(L, 1);
				}
				lua_close(L);
			}
			vmctx_free(ctx);
		}
		task_data_free(td);
	}
}

int vmexec(const char * path, const char * fb, const char * input)
{
	if(!is_absolute_path(path))
		return -1;
	task_create(NULL, path, vm_task, task_data_alloc(fb, input, NULL), 0, 0);
	return 0;
}
