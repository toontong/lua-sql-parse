#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "table_util.h"
#include "../src/sqliteInt.h"


static int _parse_error(lua_State * L, const char* errMsg){
    lua_pushnil(L);
    lua_pushstring(L, errMsg);
    return 2;
}


static void 
__push_lua_sql_type_result(lua_State *L, const SqlType typ, const ParsedResult *result){
    printf("push sql_type_result: sqltype=%d\n", typ);
    switch (typ){
        case SQLTYPE_SELECT: 
                select_to_lua_table(L, result->selectObj);
                break;
        case SQLTYPE_INSERT: break;
        case SQLTYPE_UPDATE: break;
        case SQLTYPE_DELETE: break;
        case SQLTYPE_SHOW: break;
        default:
                printf("switch-default.....\n");
        break;
    }
   printf("\tend __push_select_obj\n");
   assert(lua_istable(L, -1));
}

static void 
_to_lua_parse_result_array(lua_State *L, const ParsedResultArray *parsed){
    // lua_pushstring(L, "parsed");
    lua_newtable(L);
    
    ParsedResultItem *itm = NULL;

    // make array-table{0~curSize}
    int i = 0;
    for(i = 0; i < parsed->curSize; i++) {
        itm = &parsed->array[i];
        if (itm == NULL) {
            printf("parsed result item[%d] was NULL.", i);
            return;
        }
        lua_pushnumber(L, i);                                    /* table key = index */
        __push_lua_sql_type_result(L, itm->sqltype, &(itm->result));   /* table value */
        assert(lua_istable(L, -1));
        lua_settable(L, -3);                                     /* make a table-arrry */
    }

   assert(lua_istable(L, -1)); // this stack table was an array.
   printf("make table of parsed.\n");
   make_filed_to_table(L, "parsed");
   
   // lua_settable(L, -3); // mean: parsed = {}
}

// Lua 调用入口函数
// 参数：1个，sql string 
// 返回：2个，table_result, errMsg
static int parse(lua_State * L) {
    const char *sql = luaL_checkstring(L, 1);

    if (sql == NULL || strlen(sql)==0){
        return _parse_error(L, "args 1:sql can not be empty string.");
    }

    Parse *p = sqlite3ParseNew();
    resetParseObject(p);
    char *errMsg = 0;

    sqlite3RunParser(p, sql, &errMsg);
    if( sqlite3MallocFailed() ){
        sqlite3ParseDelete(p);
        return _parse_error(L, "malloc memory failed.");
    }
    if ( errMsg != NULL ){
        _parse_error(L, errMsg);
        free(errMsg);
        sqlite3ParseDelete(p);
        return 2;
    }
    if( p->rc == SQLITE_DONE ){
        p->rc = SQLITE_OK;
    }
    if (p->rc != SQLITE_OK){
        sqlite3ParseDelete(p);
        static const int len =1024;
        char buff[len];
        memset(buff, 0, len);
        snprintf(buff, len, "unknown parse result. rc=[%d].", p->rc);
        printf(buff);
        return _parse_error(L, buff);
    }

    lua_newtable(L); // push return 1: result-table

    set_table_field_n(L, "rc", p->rc);
    set_table_field_n(L, "explain", p->explain);
    set_table_field_n(L, "flags", p->flags);
    set_table_field_n(L, "nErr", p->nErr);
    set_table_field_n(L, "nTab", p->nTab);

    set_table_field_s(L, "sql", p->zSql, strlen(p->zSql));
    set_table_field_s(L, "tail", p->zTail, strlen(p->zTail));

    _to_lua_parse_result_array(L, &(p->parsed));


    printf("[lua c-model]return success.\n");

    ///// end set result-table field ////////////////
    lua_pushnil(L); // push return 2: err = nil
    sqlite3ParseDelete(p);
    return 2;
}

static const struct luaL_Reg sql_parser [] = {
    {"parse", parse},
    {NULL, NULL}  /* sentinel */
};

int luaopen_sql_parse (lua_State *L) {
    luaL_register(L, "sql_parse", sql_parser);

    const char sqltype[][64] = {
        "SQLTYPE_UNKNOWN",
        "SQLTYPE_SELECT",
        "SQLTYPE_UPDATE",
        "SQLTYPE_INSERT",
        "SQLTYPE_REPLACE",
        "SQLTYPE_DELETE", 
        "SQLTYPE_TRANSACTION_BEGIN",
        "SQLTYPE_TRANSACTION_START",
        "SQLTYPE_TRANSACTION_COMMIT",
        "SQLTYPE_TRANSACTION_ROLLBACK",
        "SQLTYPE_CREATE_TABLE",
        "SQLTYPE_DROP_TABLE",
        "SQLTYPE_SET",
        "SQLTYPE_SET_NAMES",
        "SQLTYPE_SET_CHARACTER_SET",
        "SQLTYPE_SHOW"
    };
    int i = 0;
    for(i = 0; i<SQLTYPE_SHOW; i++){
        lua_pushnumber(L, i);
        lua_setfield(L, -2, sqltype[i]);
    }
    const char showStmtType [][64] ={
        "SHOWTYPE_SHOW_DATABASES",
        "SHOWTYPE_SHOW_TABLES",
        "SHOWTYPE_SHOW_TABLE_STATUS",
        "SHOWTYPE_SHOW_VARIABLES",
        "SHOWTYPE_SHOW_COLLATION"
    };
    for(i = 0; i<SHOWTYPE_SHOW_COLLATION; i++){
        lua_pushnumber(L, i);
        lua_setfield(L, -2, showStmtType[i]);
    }
    return 1;
}

