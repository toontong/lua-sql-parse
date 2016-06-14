#ifndef __H_OBJ_TO__LUA_HEADER_
#define __H_OBJ_TO__LUA_HEADER_
#include <stdio.h>
#include <assert.h>

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "../src/sqliteInt.h"

int insert_to_lua_table(lua_State *L, const Insert *s);
int select_to_lua_table(lua_State *L, const Select *s);
int update_to_lua_table(lua_State *L, const Update *s);
int delete_to_lua_table(lua_State *L, const Delete *s);
void SrcList_to_lua_array(lua_State *L, const char *field, const SrcList *l);
void ExprList_to_lua_array(lua_State *L, const char *field, const ExprList *l);


#endif
