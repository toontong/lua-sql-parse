
#ifndef HEADER_TABLE_UTILS_H
#define  HEADER_TABLE_UTILS_H 1

#include <stdio.h>
#include "lua.h"

void set_table_field_n(lua_State *L, const char *key, int value);
void set_table_field_bool(lua_State *L, const char *key, int b);
void set_table_field_s(lua_State *L, const char *key, const char *value, size_t length);
void set_array_field_n(lua_State *L, const int index, const int value);
void set_array_field_s(lua_State *L, const int index, const char *value, size_t length);

void make_filed_to_table(lua_State *L, const char *key);

#endif
