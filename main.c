#include <stdio.h>
#include <stdlib.h>

#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"

// custom allocator for lua runtime, use luaL_newstate() instead if you don't care
void *lua_alloc(void *custom_arg, void *old_pointer, size_t old_size, size_t new_size) {
    (void)custom_arg; (void)old_size;

    if (new_size == 0) {
        free(old_pointer);
        return NULL;
    }
    return realloc(old_pointer, new_size);
}

// will be called from lua
static int lua_get_third_number(lua_State *L) {
    int value = 20;
    lua_pushinteger(L, value);
    return 1;
}

int main(void) {
    lua_State *lua = lua_newstate(lua_alloc, /* custom_arg: */ NULL);
    luaL_openlibs(lua);

    lua_register(lua, "get_third_number", lua_get_third_number);

    const char *lua_code =
        "function add_numbers(a, b)"  "\n"
        "  c = get_third_number()"    "\n"
        "  print('Adding:', a, b, c)" "\n"
        "  return a + b + c"          "\n"
        "end"                         "\n";

    if (luaL_dostring(lua, lua_code) != LUA_OK) {
        printf("Error running code: %s\n", lua_tostring(lua, -1));
        return 1;
    }

    // push function on the stack
    lua_getglobal(lua, "add_numbers");

    // (optional) verify function
    if (!lua_isfunction(lua, -1)) {
        printf("last stack value (should be 'add') is not a function\n");
        return 1;
    }
    
    // push function arguments
    lua_pushinteger(lua, 25); // a
    lua_pushinteger(lua, 24); // b

    if (lua_pcall(lua, 2, 1, /* error_function: */ 0) != LUA_OK) {
        printf("Error calling function: %s\n", lua_tostring(lua, -1));
        return 1;
    }

    // (optional) verify the result value
    if (!lua_isinteger(lua, -1)) {
        printf("Function 'add' did not return a number\n");
        return 1;
    }

    int result = lua_tonumber(lua, -1);
    printf("Result of add is %d.\n", result);

    lua_pop(lua, 1);
    lua_close(lua);
    return 0;
}
