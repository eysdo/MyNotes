# Lua&C++交互篇(精髓)

Author : Dejan       QQ: 673008865

## 一. 开发编译环境&第一个Lua程序

### 1. 开发编译环境准备

1. 下载&安装Lua
    下载Lua 5.1  http://www.lua.org/versions.html
    (为什么用5.1? 因为 5.3版本性能有所下降出于稳定的考虑还是使用稳定的5.1版本)

2. Lua编译
    编译Lua为静态库 (不用静态库, 影响C++编译速度)
    建立Lua编译器项目
    编译Lua库为了以后对其拓展 (也可以直接下载库)

  Windows编译步骤(VS2015):
  	1) 在src下建立vs c++控制台dll程序
  		添加新项目 -> Visual C++ -> Win32控制台应用程序 -> DLL -> 空项目

  2) 拷贝lua5.1 src目录下的所有 .h .c 代码文件到项目目录下并添加现有项

  3) 设置编译输出路径为 ../../../bin
  	项目属性配置:
  		常规:
  			输出目录:../../../bin/$(Platform)/$(Configuration)
  			中间目录:../../../temp/$(Platform)/$(Configuration)/$(ProjectName)

  4) 设置输出 导入库路径为 ../../../lib
  	项目属性配置:
  		链接器:
  			高级 - 导入库:../../../lib/$(TargetName).lib

  5) 添加LUA_BUILD_AS_DLL宏
  	项目属性配置:
  		C/C++:
  			预处理器 - 预处理器定义(在最前面添加):LUA_BUILD_AS_DLL;

3. 编译报错问题解决
    导致原因是由于lua.c luac.c 两个文件中重复定义了main()主函数. 
    lua.c 是lua解析器可执行程序源码
    luac.c 是lua编译器可执行程序源码
    解决方法:由于我们要把lua打包成dll动态库, 这两个文件已经不需要了可以删除或者把把这两个文件的main()函数注释掉.

### 2. 开始第一个Lua程序

```cpp
// lua.h
#ifndef _LUA_H_
#define _LUA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>

#ifdef __cplusplus
}
#endif


#endif // !_LUA_H_
```

```cpp
// main.cpp
#include <lua.h>
#pragma comment(lib, "lua.lib")

int main(int argc, char **argv)
{
	lua_State *L = lua_open();
	luaopen_base(L);
	luaL_loadfile(L, "main.lua");
	lua_pcall(L, 0, 0, 0);

	getchar();
	return 0;
}
```

```lua
-- main.lua
print("Hello Lua!");
```

## 二. Lua脚本基础语法

### 1. lua代码注释

```lua
-- main.lua 单行注释

--[[
	多行注释
]]
```

### 2. Lua 基础数据类型和变量

​	全局变量
​		b = 2

​	本地变量(尽量用本地, 保证即时的垃圾回收)
​		local a = 1

​	数据类型:
​		Nil
​		Booleans
​		Numbers
​		Strings

​	Nil 空类型:
​		用于区分具有一些数据或没有(nil)数据的值

​		全局变量设置为nil会交给垃圾回收
​			local a = nil
​			print(type(a))  --> nil

​	Booleans 类型:
​		但要注意Lua中所有的值都可以作为条件
​		除了false和nil为假, 其他值都为真, 0为真

​	Numbers 类型:
​		Lua中没有整数, 都是用浮点数运算
​		对应C的double类型
​		新版Lua中有基于64位的整型
​		tonumber() 转换格式, 仅支持字符串转换

​	Strings 类型:
​		tostring() 转换格式
​		[[]]多行字符串赋值
​		"与C一样的转义\""
​		..字符串拼接, 不能拼接 nil

​	String字符串处理:
​		字符串长度 string.len
​		字符串子串 string.sub(str, 3, 5)
​		字符串查找 local b, e = string.find(str, "HEAD")支持正则, 一次可以返回两个变量
​		字符串替换 string.gsub(str, "TEXT", "HDJ")支持正则

代码示例:

```cpp
// main.cpp
extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#pragma comment(lib, "lua.lib")

int main()
{
	lua_State *lua = lua_open();
	// 打开基本库
	luaopen_base(lua);
    // 打开string库
	luaopen_string(lua);

	// 载入脚本
	if (luaL_loadfile(lua, "main.lua"))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua load error:%s\n", error);
		return -1;
	}

	// 执行脚本
	if (lua_pcall(lua, 0, 0, 0))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua call error:%s\n", error);
		return -1;
	}

	getchar();
	return 0;
}
```

```lua
-- main.lua

-- a = 10 全局变量, 文件外可访问调用
-- local a = 10 本地变量, 文件外不可访问调用
local a = 10
print(a)

-- String字符串处理: 获取字符串长度 string.len()
local str = "Dejan"
print(string.len(str))

-- String字符串处理: 字符串子串 string.sub(str, 3, 5)
str = "HelloLua!"
print(string.sub(str, 3, 5))

-- String字符串处理: 字符串查找 local b, e = string.find(str, "html")支持正则, 一次可以返回两个变量
str = [[
<html>
</html>
]]
local s_start, s_end = string.find(str, "html")
print("s_start = "..s_start.." s_end = "..s_end)

-- String字符串处理: 字符串替换 string.gsub(str, "html", "body")支持正则
local res = string.gsub(str, "html", "body")
print(res)
print(str)
```

### 3. 文件引用(包含)

```cpp
// cpp
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#pragma comment(lib, "lua.lib")

int main(int argc, char **argv)
{
	lua_State *L = lua_open();
	luaopen_base(L);
	luaL_loadfile(L, "main.lua");
	lua_pcall(L, 0, 0, 0);

	getchar();
	return 0;
}
```

```lua
-- main.lua
-- a = 10
local a = 10
dofile("common.lua")
print(a)
```

```lua
-- common.lua
print("Hello Lua!")
print(a)
```

### 4. Lua 控制结构语句

```
if     条件语句
while  循环语句
repeat 循环语句(类似C的do while)
for    循环语句
```

#### 1) if条件判断语句

	代码示例:
		if conditions then
			then-part
		elseif conditions then
			elseif-part
		else
			else-part
		end
	
	逻辑运算:
		and    or    not
		
		if(a==1 or a==3)
		if(a==q and b==1)
		if(not(a==1)) 注意加括号, 解决逻辑次序问题
代码示例:

```lua
-- main.lua

-- if条件语句
if((1==1 and 1==3) or 3==3) then
	print("1==1")
elseif(2==2) then
	print("2==2")
else
	print("in else")
end
```

#### 2) while循环语句

```
代码示例:
	while condition do
		statements
	end

break跳出循环, 没有 continue
```

代码示例:

```lua
-- main.lua

-- while循环语句
local i = 100
while( not(i < 0) ) do
	print("i = "..i)
	i = i - 1
	if(i == 90) then
		print("break while")
		break
	end
end
```

#### 3) repeat循环语句

```
代码示例:
	repeat
		statements
	until conditions

break退出循环
```

代码示例:

```lua
-- main.lua

-- repeat循环语句
local i = 100
repeat
	i = i - 1
	print("i = "..i)
until i<100
```

#### 4) for循环语句

```
数值循环
	for var=from,to,step do
		loop-part
	end

范型循环: pairs遍历全部 ipairs遍历数组
	for i,v in ipairs(a) do print(v) end
```

代码示例:

```lua
-- main.lua

-- for循环语句 - 数值循环
-- 一次跳2步
for var = 1,5,2 do
	print("var = "..var)
end
print("-----------------")
-- 可以省略最后的步数,默认一次跳1步
for var = 1,5 do
	print("var = "..var);
end
```

```lua
-- main.lua

-- for循环语句 - 范型循环
local days = {"Sun","Mon","Tue"}
for i,v in ipairs(days) do
	print(i..":"..v)
end
for i,v in pairs(days) do
	print(i.."=="..v)
end
local tab = { [1] = "A", [2] = "B", [3] = "C", [5] = "E" }
for i,v in pairs(tab) do
	print(i.."=>"..v)
end
for i,v in ipairs(tab) do
	print(i.."--"..v)
end
```

### 5. Lua 表和函数

#### 1) Lua表

```
常用操作:
	获取表的大小  table.getn(t1)
	插入        table.insert(a, pos, line) 不传pos相当于push_back
	删除        table.remove(a, pos) 返回这次删除的值, 不传pos相当于pop_back
```

代码示例:

```cpp
// main.cpp
extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#pragma comment(lib, "lua.lib")

int main()
{
	lua_State *lua = lua_open();
	// 打开基本库
	luaopen_base(lua);
	luaopen_string(lua);
	luaopen_table(lua);

	// 载入脚本
	if (luaL_loadfile(lua, "main.lua"))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua load error:%s\n", error);
		return -1;
	}

	// 执行脚本
	if (lua_pcall(lua, 0, 0, 0))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua call error:%s\n", error);
		return -1;
	}

	getchar();
	return 0;
}
```

```lua
-- main.lua

-- 数组表
local tab1 = {"001","002","003"}
for i,v in ipairs(tab1) do
	print("value = "..v)
end

print("========= insert =========")

table.insert(tab1, 3, "002-2")
for i,v in ipairs(tab1) do
	print("value = "..v)
end

print("========= push_back =========")

table.insert(tab1, "004")
for i,v in ipairs(tab1) do
	print("value = "..v)
end

print("========= remove =========")

table.remove(tab1, 3)
for i,v in ipairs(tab1) do
	print("value = "..v)
end

print("========= pop_back =========")

table.remove(tab1)
for i,v in ipairs(tab1) do
	print("value = "..v)
end
```

```lua
-- main.lua

-- 哈希表
local tab2 = {id=2, arg=20}
tab2["name"] = "dejan"
tab2["id"] = nil
for k,v in pairs(tab2) do
	print(k.." : "..v)
end
```

```lua
-- main.lua

-- 二维表
local tab3 = {}
tab3[1] = {"name1", "name2"}
tab3[2] = {"value1", "value2"}

for k,v in pairs(tab3) do
	for k2,v2 in pairs(v) do
		print("k2 = "..k2, " v2 = "..v2)
	end
end
```

#### 2) Lua函数

```
函数语法(支持可变参数)
function func_name(arguments-list)
	statements-list
end

return 返回值, 可以返回多个值
```

代码示例:

```lua
-- main.lua

-- 普通传参
function test(p1, p2)
	if(p1 == nil) then
		p1 = "001"
	end
	if(p2 == nil) then
		p2 = "002"
	end
	
	print("p1="..p1.." p2="..p2)
	print("in test function")
end

test(01, "Dejan")
```

```lua
-- main.lua

-- 可变参数
function test(...)
	local len = table.getn(arg)
	print("arg len is "..len)
	for a = 1,len do
		print("arg["..a.."]="..arg[a])
	end
	return 1, "name"
end

test(01, "Dejan", 22)
```

```lua
-- main.lua

-- 函数传变量
function test(id, name)
	return id, name
end
local func = test
local event = function(p1)
	print("event = "..p1)
end
event("key")
local id,name = func(01, "Dejan")
print("ID : "..id..", "..name)
```

```lua
-- main.lua

-- 函数重载
function test()
	print("not arg")
end
function test(...)
	print("new test")
end
function test(...)
	print("test ...")
end
test()
```

## 三. Lua交互访问C++

### 1. Lua调用c++函数

代码示例:

```cpp
// main.cpp
extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#pragma comment(lib, "lua.lib")

int CTest(lua_State *lua)
{
	printf("int CTest");
	return 0;
}

int main()
{
	lua_State *lua = lua_open();
	// 打开基本库
	luaopen_base(lua);
	lua_register(lua, "CTest", CTest);

	// 载入脚本
	if (luaL_loadfile(lua, "main.lua"))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua load error:%s\n", error);
		return -1;
	}

	// 执行脚本
	if (lua_pcall(lua, 0, 0, 0))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua call error:%s\n", error);
		return -1;
	}

	getchar();
	return 0;
}
```

```lua
-- main.lua

CTest()
```



### 2. Lua给C++函数传递普通类型参数

代码示例:

```cpp
// main.cpp
extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#pragma comment(lib, "lua.lib")

int CTest(lua_State *L)
{
	size_t len;
	const char *name = lua_tolstring(L, 1, &len);
	int age = lua_tonumber(L, 2);
	bool is = lua_toboolean(L, 3);
	printf("name = %s, age = %d is %d\n", name, age, is);
	return 0;
}

int main()
{
	lua_State *lua = lua_open();
	// 打开基本库
	luaopen_base(lua);
	lua_register(lua, "CTest", CTest);

	// 载入脚本
	if (luaL_loadfile(lua, "main.lua"))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua load error:%s\n", error);
		return -1;
	}

	// 执行脚本
	if (lua_pcall(lua, 0, 0, 0))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua call error:%s\n", error);
		return -1;
	}

	getchar();
	return 0;
}
```

```lua
-- main.lua
CTest("Lua", 123, false)
```



### 3. Lua给C++函数传递数组参数

代码示例:

```cpp
// main.cpp
extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#pragma comment(lib, "lua.lib")

int CTestArr(lua_State *L)
{
	int len = luaL_getn(L, 1); // 5.1版本以上luaL_getn改为lua_objlen，luaL_setn废弃
	for (int i = 1; i <= len; i++)
	{
        // 将key放到栈顶，这个key为i。如果key是字符串，那就用lua_pushstring
		lua_pushnumber(L, i); // 当前i在栈顶-1(2)位置
		lua_gettable(L, 1); // 当前表在key的下方索引为-2(1)位置
		size_t size;
		printf("%s\n", lua_tolstring(L, -1, &size));
		lua_pop(L, 1); // 弹出栈顶-1 key
        //--------------------------
        // 正索引    值     负索引
        //   2      i       -1
        //   1     table    -2
        //--------------------------
        // lua_pop(L,1)之后,相当于恢复状
        // 正索引    值     负索引
        //   1     table    -1
        //--------------------------
	}
	return 0;
}

int main()
{
	lua_State *lua = lua_open();
	// 打开基本库
	luaopen_base(lua);
	lua_register(lua, "CTestArr", CTestArr);

	// 载入脚本
	if (luaL_loadfile(lua, "main.lua"))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua load error:%s\n", error);
		return -1;
	}

	// 执行脚本
	if (lua_pcall(lua, 0, 0, 0))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua call error:%s\n", error);
		return -1;
	}

	getchar();
	return 0;
}
```

```lua
-- main.lua
local arr = {"A001", "A002", "A003"}
CTestArr(arr)
```



### 4. Lua给C++函数传递表参数

代码示例:

```cpp
// main.cpp
extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#pragma comment(lib, "lua.lib")

/* 获取所有 */
int CTestTable(lua_State *L)
{
	lua_pushnil(L);
    //------------------
	// 正索引  值    负索引
	//   2    nil      -1
	//   1    table    -2
	//------------------
	while (lua_next(L, 1) != 0) // 同 lua_next(L, -2),读到末尾会自动把栈索引-1回收掉
	{
		//------------------
		// 正索引   值    负索引
		//   3     value    -1
		//   2     key      -2
		//   1     table    -3
		//------------------
		printf("key = %s ", lua_tostring(L, -2));
		printf("value = %s \n", lua_tostring(L, -1));
		lua_pop(L, 1);
        //------------------
		// 正索引   值    负索引
		//   2     key      -1
		//   1     table    -2
		//------------------
	}
	//------------------
	// 正索引   值    负索引
	//   1     table    -1
	//------------------
	return 0;
}

/* 获取一个 */
int CTestTableOne(lua_State *L)
{
    //------------------
	// 正索引   值    负索引
	//   1     table    -1
	//------------------
	lua_getfield(L, 1, "name"); // 同 lua_getfield(L, -1, "name"),获取到value并push到栈顶
    //------------------
	// 正索引   值    负索引
    //   2     value    -1
	//   1     table    -2
	//------------------
	size_t len;
	printf("CTestTableOne: name = %s\n", lua_tolstring(L, -1, &len));
	return 0;
}

int main()
{
	lua_State *lua = lua_open();
	// 打开基本库
	luaopen_base(lua);
	lua_register(lua, "CTestTable", CTestTable);
	lua_register(lua, "CTestTableOne", CTestTableOne);

	// 载入脚本
	if (luaL_loadfile(lua, "main.lua"))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua load error:%s\n", error);
		return -1;
	}

	// 执行脚本
	if (lua_pcall(lua, 0, 0, 0))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua call error:%s\n", error);
		return -1;
	}

	getchar();
	return 0;
}
```

```lua
-- main.lua
local tab = {name = "Dejan", age = "20", id = "008"}
CTestTable(tab)
CTestTableOne(tab)
```



### 5. Lua给C++函数的参数类型检查

代码示例:

```cpp
// main.cpp
extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#pragma comment(lib, "lua.lib")

/* 获取所有 */
int CTestTable(lua_State *L)
{
    //--------------------
	// 正索引   值       负索引
    //   2     param2    -1
    //   1     param1    -2
    //--------------------
	luaL_checktype(L, 1, LUA_TTABLE); // 同 luaL_checktype(L,-2,LUA_TTABLE);
	if (lua_type(L, 2) != LUA_TNUMBER) // 同 lua_type(L, -1)
	{
		printf("param 2 is not number!!\n");
	}
    //---------------------------
	// 正索引   值            负索引
    //   2     param2         -1
    //   1     param1(table)  -2
    //---------------------------
	lua_getfield(L, 1, "name"); // 获取到value并push到栈顶
    //---------------------------
	// 正索引   值            负索引
    //   3     value          -1
    //   2     param2         -2
    //   1     param1(table)  -3
    //---------------------------
	printf("name = %s\n", lua_tostring(L, -1));

	return 0;
}

int main()
{
	lua_State *lua = lua_open();
	// 打开基本库
	luaopen_base(lua);
	lua_register(lua, "CTestTable", CTestTable);

	// 载入脚本
	if (luaL_loadfile(lua, "main.lua"))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua load error:%s\n", error);
		return -1;
	}

	// 执行脚本
	if (lua_pcall(lua, 0, 0, 0))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua call error:%s\n", error);
		return -1;
	}

	getchar();
	return 0;
}
```

```lua
-- main.lua

local tab = {name = "Lua", age = "20", id = "008"}
local size = 108
--CTestTable(tab, size)
CTestTable(tab, "test")
```



### 6. Lua调用C++函数获取返回值

代码示例:

```cpp
// main.cpp
extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#pragma comment(lib, "lua.lib")

int CTestRe(lua_State *L)
{
	//lua_pushstring(L, "return value");
    
	lua_newtable(L);
    //----------------------
	// 正索引   值    负索引
	//   1     table    -1
	//----------------------
    
	// 插入key value
	lua_pushstring(L, "name");
	lua_pushstring(L, "Dejan");
    //----------------------
	// 正索引    值     负索引
	//   3     "Dejan"   -1
	//   2     "name"    -2
	//   1      table    -3
	//----------------------
	lua_settable(L, -3); // 同 lua_settable(L,1),添加到表后会自动清理索引-1、-2
    //----------------------
	// 正索引    值       负索引
	//   1      table[1]    -1
	//----------------------
	lua_pushstring(L, "age");
	lua_pushnumber(L, 20);
    //----------------------
	// 正索引    值     负索引
	//   3       20       -1
	//   2      "age"     -2
	//   1     table[1]   -3
	//----------------------
	lua_settable(L, -3); // 同 lua_settable(L,1),添加到表后会自动清理索引-1、-2
    //----------------------
	// 正索引    值       负索引
	//   1      table[2]    -1
	//----------------------

	return 1;
}

int main()
{
	lua_State *lua = lua_open();
	// 打开基本库
	luaopen_base(lua);
	lua_register(lua, "CTestRe", CTestRe);

	// 载入脚本
	if (luaL_loadfile(lua, "main.lua"))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua load error:%s\n", error);
		return -1;
	}

	// 执行脚本
	if (lua_pcall(lua, 0, 0, 0))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua call error:%s\n", error);
		return -1;
	}

	getchar();
	return 0;
}
```

```lua
-- main.lua
local re = CTestRe()
print("name = "..re["name"])
print("age = "..re["age"])
```



### 7. C++访问Lua的变量

代码示例:

```cpp
// main.cpp
extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#pragma comment(lib, "lua.lib")

int main()
{
	lua_State *lua = lua_open();
	// 打开基本库
	luaopen_base(lua);

	// 载入脚本
	if (luaL_loadfile(lua, "main.lua"))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua load error:%s\n", error);
		return -1;
	}

	// 执行脚本
	if (lua_pcall(lua, 0, 0, 0))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua call error:%s\n", error);
		return -1;
	}

	lua_getglobal(lua, "width"); // 读取lua脚本中全局变量的值并push到栈顶 -1
	int width = lua_tonumber(lua, -1);
	lua_pop(lua, 1);
	printf("width = %d\n", width);

	lua_close(lua);

	getchar();
	return 0;
}
```

```lua
-- main.lua
width = 128
```



### 8. C++调用Lua表变量

代码示例:

```cpp
// main.cpp
extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#pragma comment(lib, "lua.lib")

int main()
{
	lua_State *lua = lua_open();
	// 打开基本库
	luaopen_base(lua);

	// 载入脚本
	if (luaL_loadfile(lua, "main.lua"))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua load error:%s\n", error);
		return -1;
	}

	// 执行脚本
	if (lua_pcall(lua, 0, 0, 0))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua call error:%s\n", error);
		return -1;
	}

	lua_getglobal(lua, "width"); // 读取lua脚本中全局变量width的值并push到栈顶 -1
	int width = lua_tonumber(lua, -1);
	lua_pop(lua, 1); // 弹出一个栈顶元素 -1
	printf("width = %d\n", width);

	lua_getglobal(lua, "conf"); // 读取lua脚本中全局变量conf的值并push到栈顶 -1
	lua_getfield(lua, -1, "titlename"); // 读取conf["titlename"]值并push到栈顶 -1
    //---------------------
    // 正索引   值      负索引
    //  ...   value     -1
    // 	...   conf      -2
    //  ...   ...       ...
    //---------------------
	printf("title = %s\n", lua_tostring(lua, -1)); // conf["titlename"]值
	lua_pop(lua, 1);
	lua_getfield(lua, -1, "height");
	printf("height = %d\n", (int)lua_tonumber(lua, -1));
	lua_pop(lua, 2); // 弹出2个栈顶元素

	lua_close(lua);

	getchar();
	return 0;
}
```

```lua
-- main.lua
width = 128

conf = 
{
	titlename = "first lua",
	height = 180
}
```



### 9. C++给Lua传递变量

代码示例:

```cpp
// main.cpp
extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#pragma comment(lib, "lua.lib")

int main()
{
	lua_State *lua = lua_open();
	// 打开基本库
	luaopen_base(lua);

	// 设置一个lua全局变量
	lua_pushstring(lua, "Hello"); // push 到栈顶 -1
	lua_setglobal(lua, "test"); // 获取到栈顶-1索引的值设置成lua全局变量,并自动回收栈顶-1元素

	// 载入脚本
	if (luaL_loadfile(lua, "main.lua"))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua load error:%s\n", error);
		return -1;
	}

	// 执行脚本
	if (lua_pcall(lua, 0, 0, 0))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua call error:%s\n", error);
		return -1;
	}


	lua_close(lua);

	getchar();
	return 0;
}
```

```lua
-- main.lua
print(test)
```



### 10. C++给Lua传递表

代码示例:

```cpp
// main.cpp
extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#pragma comment(lib, "lua.lib")

int main()
{
	lua_State *lua = lua_open();
	// 打开基本库
	luaopen_base(lua);

	// C++给lua传递表
	lua_newtable(lua); // 栈顶创建一个表 -1
    //-------------------
    // 正索引   值    负索引
    //  ...   table   -1
    //-------------------
	lua_pushstring(lua, "name");
	lua_pushstring(lua, "Dejan");
    //-------------------
    // 正索引    值     负索引
    //  ...   "Dejan"   -1
    //  ...   "name"    -2
    //  ...    table    -3
    //-------------------
	lua_settable(lua, -3); // 将 "name"=>"Dejan" 添加到索引-3表中,并自动清理-1、-2索引元素
    //------------------------
    // 正索引    值        负索引
    //  ...    table[1]    -1
	//------------------------

	lua_pushstring(lua, "age");
	lua_pushnumber(lua, 20);
    //----------------------
    // 正索引   值       负索引
    //  ...    20        -1
    //  ...   "age"      -2
    //  ...    table[1]  -3
    //----------------------
	lua_settable(lua, -3); // 将 "age"=>20 添加到索引-3表中,并自动清理-1、-2索引元素
    //------------------------
    // 正索引     值       负索引
    //  ...    table[2]    -1
	//------------------------

	lua_setglobal(lua, "user"); // 获取到栈顶-1索引元素设置为lua全局变量,并自动回收栈顶-1元素

	// 载入脚本
	if (luaL_loadfile(lua, "main.lua"))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua load error:%s\n", error);
		return -1;
	}

	// 执行脚本
	if (lua_pcall(lua, 0, 0, 0))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua call error:%s\n", error);
		return -1;
	}


	lua_close(lua);

	getchar();
	return 0;
}
```

```lua
-- main.lua
print("name:"..user["name"])
print("age:"..user["age"])
```



### 11. C++调用Lua函数

代码示例:

```cpp
// main.cpp
extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#pragma comment(lib, "lua.lib")

int main()
{
	lua_State *lua = lua_open();
	// 打开基本库
	luaopen_base(lua);

	// 载入脚本
	if (luaL_loadfile(lua, "main.lua"))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua load error:%s\n", error);
		return -1;
	}

	// 执行脚本
	if (lua_pcall(lua, 0, 0, 0))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua call error:%s\n", error);
		return -1;
	}

	lua_getglobal(lua, "event"); // 获取lua脚本函数event(),并push到栈顶 -1
	lua_pcall(lua, 0, 0, 0); // 执行函数event(),并自动回收栈顶-1元素

	lua_close(lua);
	getchar();
	return 0;
}
```

```lua
-- main.lua
function event()
	print("c++ call lua function")
end
```



### 12. C++调用Lua函数错误显示和Lua堆栈清理

代码示例:

```cpp
// main.cpp
extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#pragma comment(lib, "lua.lib")

int main()
{
	lua_State *lua = lua_open();
	// 打开基本库
	luaopen_base(lua);

	// 载入脚本
	if (luaL_loadfile(lua, "main.lua"))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua load error:%s\n", error);
		return -1;
	}

	// 执行脚本
	if (lua_pcall(lua, 0, 0, 0))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua call error:%s\n", error);
		return -1;
	}

	printf("top is %d\n", lua_gettop(lua));
	lua_getglobal(lua, "event2");
	if (lua_pcall(lua, 0, 0, 0) != 0)
	{
        // lua错误信息会push到栈顶 -1
		printf("call event failed %s\n", lua_tostring(lua, -1));
		lua_pop(lua, 1); // 获取完错误信息后也需要弹出栈顶 -1
	}
	printf("top is %d\n", lua_gettop(lua));

	lua_close(lua);
	getchar();
	return 0;
}
```

```lua
-- main.lua
function event()
	print("c++ call lua function")
end
```



### 13. C++调用Lua函数传递参数

代码示例:

```cpp
// main.cpp
extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#pragma comment(lib, "lua.lib")

int main()
{
	lua_State *lua = lua_open();
	// 打开基本库
	luaopen_base(lua);

	// 载入脚本
	if (luaL_loadfile(lua, "main.lua"))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua load error:%s\n", error);
		return -1;
	}

	// 执行脚本
	if (lua_pcall(lua, 0, 0, 0))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua call error:%s\n", error);
		return -1;
	}

	printf("top is %d\n", lua_gettop(lua));
	lua_getglobal(lua, "event"); // 获取lua脚本函数event(),并push到栈顶 -1
	lua_pushstring(lua, "key");
    //---------------------
    // 正索引     值      负索引
    //  ...     "key"     -1
    //  ...   function    -2
    //---------------------
	if (lua_pcall(lua, 1, 1, 0) != 0) // lua_pcall(lua, [参数个数], [返回值个数], [错误处理函数索引]),会自动回收索引-1、-2元素
	{
		printf("call event failed %s\n", lua_tostring(lua, -1));
		lua_pop(lua, 1);
	}
	else
	{
        // 执行成功, push返回值到栈顶 -1
		printf("lua return:%s\n", lua_tostring(lua, -1));
		lua_pop(lua, 1);
	}
	printf("top is %d\n", lua_gettop(lua));

	lua_close(lua);
	getchar();
	return 0;
}
```

```lua
-- main.lua
function event(e)
	print("c++ call lua function")
	print(e)
	return "lua event return"
end
```



### 14. C++调用Lua函数错误处理

代码示例:

```cpp
// main.cpp
extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#pragma comment(lib, "lua.lib")

int main()
{
	lua_State *lua = lua_open();
	// 打开基本库
	luaopen_base(lua);

	// 载入脚本
	if (luaL_loadfile(lua, "main.lua"))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua load error:%s\n", error);
		return -1;
	}

	// 执行脚本
	if (lua_pcall(lua, 0, 0, 0))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua call error:%s\n", error);
		return -1;
	}

	printf("top is %d\n", lua_gettop(lua));
	int errfunc = lua_gettop(lua); // 获得当前栈顶元素正数索引 2
	lua_getglobal(lua, "ferror"); // 获取lua脚本函数ferror(),并push到栈顶 -1
	errfunc++; // 得到ferror()在lua堆栈中的正数索引 3
	lua_getglobal(lua, "event"); // 获取lua脚本函数event(),并push到栈中
	lua_pushstring(lua, "key");
    //------------------------
    // 正索引      值      负索引
    //   5      "key"      -1
    //   4      event()    -2
    //   3     ferror()    -3
    //   ...      ...      ...
    //------------------------
    // lua_pcall(lua, [参数个数], [返回值个数], [错误处理函数索引]),会自动回收索引-1、-2元素
	if (lua_pcall(lua, 1, 1, errfunc) != 0)
	{
		printf("call event failed %s\n", lua_tostring(lua, -1));
		lua_pop(lua, 1);
	}
	else
	{
        // 执行成功, push返回值到栈顶 -1
		printf("lua return:%s\n", lua_tostring(lua, -1));
		lua_pop(lua, 1);
	}
	lua_pop(lua, 1); // 弹出栈顶元素 ferror()
	printf("top is %d\n", lua_gettop(lua));

	lua_close(lua);
	getchar();
	return 0;
}
```

```lua
-- main.lua
function ferror(e)
	print("My error:"..e)
	return "lua change error"
end

function event2(e)
	print("c++ call lua function")
	print(e)
	return "lua event return"
end
```



### 15. C++调用Lua函数获取返回值(表)

代码示例:

```cpp
// main.cpp
extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#pragma comment(lib, "lua.lib")

int main()
{
	lua_State *lua = lua_open();
	// 打开基本库
	luaopen_base(lua);

	// 载入脚本
	if (luaL_loadfile(lua, "main.lua"))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua load error:%s\n", error);
		return -1;
	}

	// 执行脚本
	if (lua_pcall(lua, 0, 0, 0))
	{
		const char *error = lua_tostring(lua, -1);
		printf("lua call error:%s\n", error);
		return -1;
	}

	printf("top is %d\n", lua_gettop(lua));
	int errfunc = lua_gettop(lua); // 获得当前栈顶元素正数索引 2
	lua_getglobal(lua, "ferror"); // 获取lua脚本函数ferror(),并push到栈顶 -1
	errfunc++; // 得到ferror()在lua堆栈中的正数索引 3
	lua_getglobal(lua, "event"); // 获取lua脚本函数event(),并push到栈中
	lua_pushstring(lua, "key");
    //------------------------
    // 正索引      值      负索引
    //   5      "key"      -1
    //   4      event()    -2
    //   3     ferror()    -3
    //   ...      ...      ...
    //------------------------

	lua_newtable(lua);
	lua_pushstring(lua, "name");
	lua_pushstring(lua, "Dejan");
    //------------------------
    // 正索引      值      负索引
    //   7      "Dejan"    -1
    //   6      "name"     -2
    //   6      table      -3
    //   5      "key"      -4
    //   4      event()    -5
    //   3     ferror()    -6
    //   ...      ...      ...
    //------------------------
	lua_settable(lua, -3); // 将 "name"=>"Dejan"添加到表 -3,并自动回收索引-1、-2元素
    //------------------------
    // 正索引      值      负索引
    //   6      table[1]   -1
    //   5      "key"      -2
    //   4      event()    -3
    //   3     ferror()    -4
    //   ...      ...      ...
    //------------------------

    // lua_pcall(lua, [参数个数], [返回值个数], [错误处理函数索引]),会自动回收索引-1、-2、-3元素
	if (lua_pcall(lua, 2, 1, errfunc) != 0)
	{
		printf("call event failed %s\n", lua_tostring(lua, -1));
		lua_pop(lua, 1);
	}
	else
	{
        // 执行成功, push返回值到栈顶 -1
		lua_getfield(lua, -1, "id"); // 获得 table["id"]的值并push到栈顶 -1
        //---------------------
        // 正索引    值     负索引
        //  ...    value    -1
        //  ...    table    -2
        //---------------------
		printf("return table id is %d\n", (int)lua_tonumber(lua, -1));
		lua_pop(lua, 2); // 弹出栈顶元素 -1、-2
	}
	lua_pop(lua, 1); // 弹出栈顶元素 ferror()
	printf("top is %d\n", lua_gettop(lua));

	lua_close(lua);
	getchar();
	return 0;
}
```

```lua
-- main.lua
function ferror(e)
	print("My error:"..e)
	return "lua change error"
end

function event(e, obj)
	print("c++ call lua function")
	print(e)
	print(obj.name)
	local re = {id = 123}
	return re
end
```













