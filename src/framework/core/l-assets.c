/*
 * framework/core/l-assets.c
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

#include <core/l-assets.h>

static const char assets_lua[] = X(
local Xfs = Xfs
local Font = Font
local Image = Image
local DisplayImage = DisplayImage

local M = Class()

function M:init()
	self._images = {}
end

function M:loadImage(name)
	if type(name) == "string" then
		if not self._images[name] and Xfs.isfile(name) then
			self._images[name] = Image.new(name)
		end
		return self._images[name]
	end
	return nil
end

function M:loadDisplay(name)
	if type(name) == "string" and Xfs.isfile(name) then
		if string.lower(string.sub(name, -4)) == ".png" then
			return DisplayImage.new(self:loadImage(name))
		elseif string.lower(string.sub(name, -4)) == ".jpg" then
			return DisplayImage.new(self:loadImage(name))
		elseif string.lower(string.sub(name, -5)) == ".jpeg" then
			return DisplayImage.new(self:loadImage(name))
		end
	else
		return name
	end
end

function M:clear()
	self._images = {}
end

return M
);

int luaopen_assets(lua_State * L)
{
	if(luaL_loadbuffer(L, assets_lua, sizeof(assets_lua) - 1, "Assets.lua") == LUA_OK)
		lua_call(L, 0, 1);
	return 1;
}
