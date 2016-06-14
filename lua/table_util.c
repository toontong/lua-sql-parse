/* all function assume that table is at the lua_State top */

#include <assert.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "table_util.h"

void set_table_field_n(lua_State *L, const char *key, int value) {
    lua_pushstring(L, key);
    lua_pushnumber(L, (double)value);
    lua_settable(L, -3); /*mean: table[key] = value */
}

void set_table_field_bool(lua_State *L, const char *key, int b){
    lua_pushstring(L, key);
    lua_pushboolean(L, b);
    lua_settable(L, -3); /*mean: table[key] = value */
}

void set_table_field_s(lua_State *L, const char *key, const char *value, size_t length) {
    lua_pushstring(L, key);
    lua_pushlstring(L, value, length); // 可包含'/0',可不以NULL结束
    lua_settable(L, -3); /*mean: table[key] = value */
}

void set_array_field_n(lua_State *L, const int index, const int value) {
   lua_pushnumber(L, (double)index);
   lua_pushnumber(L, (double)value); 
   lua_settable(L, -3); /*mean: table[key] = value */
}

void set_array_field_s(lua_State *L, const int index, const char *value, size_t length) {
    lua_pushnumber(L, (double)index);
    lua_pushlstring(L, value, length); // 可包含'/0',可不以NULL结束
    lua_settable(L, -3); /*mean: table[key] = value */
}

void make_filed_to_table(lua_State *L, const char *key){
    assert(lua_istable(L, -1));
    lua_pushstring(L, key);
    lua_insert(L, -2);
    assert(lua_istable(L, -1));
    assert(lua_isstring(L, -2));
    assert(lua_istable(L, -3));
    lua_settable(L, -3); /*mean: table[key] = table{} */
}
