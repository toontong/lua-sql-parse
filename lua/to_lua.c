#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "../src/sqliteInt.h"

#include "to_lua.h"
#include "table_util.h"


int insert_to_lua_table(lua_State *L, const Insert *s){
    return 1;
}


int select_to_lua_table(lua_State *L, const Select *s){
    lua_newtable(L); /* create a new table for Select Obj */

    set_table_field_bool(L, "isDistinct", s->isDistinct);
    set_table_field_bool(L, "isResolved", s->isResolved);
    set_table_field_bool(L, "isAgg", s->isAgg);
    set_table_field_bool(L, "usesVirt", s->usesVirt);
    set_table_field_bool(L, "disallowOrderBy", s->disallowOrderBy);
    set_table_field_n(L, "nLimit", s->iLimit);
    set_table_field_n(L, "nOffset", s->iOffset);

    if(s->pWhere)
        expr_to_lua_table(L,"where", s->pWhere);
    
    if(s->pLimit)
        expr_to_lua_table(L,"limit", s->pLimit);

    if(s->pOffset)
        expr_to_lua_table(L,"offset", s->pOffset); 
    if(s->pHaving)
        expr_to_lua_table(L,"having", s->pHaving);

    if(s->pSrc){
        SrcList_to_lua_array(L,"fromList", s->pSrc); 
    }

    if (s->pEList ){
        ExprList_to_lua_array(L, "fields", s->pEList);
    }
    if (s->pGroupBy){
        ExprList_to_lua_array(L, "groupBy", s->pGroupBy);
    }
    if (s->pOrderBy){
        ExprList_to_lua_array(L, "orderBy", s->pOrderBy);
    }
   
    if (s->pPrior) {
        select_to_lua_table(L, s->pPrior);
        make_filed_to_table(L, "prior");
    }

    if (s->pRightmost) {
        select_to_lua_table(L, s->pRightmost);
        make_filed_to_table(L, "rightmost");
    }

    assert(lua_istable(L, -1));
    return 1;
}

int update_to_lua_table(lua_State *L, const Update *s){return 1;}
int delete_to_lua_table(lua_State *L, const Delete *s){return 1;}

int expr_to_lua_table(lua_State *L, const char *field, const Expr *s){
    lua_newtable(L); /* create a new table for Expr Obj */

    //set_table_field_n(L, "op", s->op);           // op 是位标识
    // set_table_field_n(L, "flags", s->flags)  // igrone field
    set_table_field_s(L, "token", s->token.z, s->token.n);    /* An operand token */
    set_table_field_s(L, "span", s->span.z, s->span.n);      /* Complete text of the expression */
    
    if (ExprHasProperty(s, EP_Agg)) {
        set_table_field_n(L, "iAgg", s->iAgg);      /* Which entry in pAggInfo->aCol[] or ->aFunc[] */
        //TODO:AggInfo s->pAggInfo
    }
    if (ExprHasProperty(s, EP_FromJoin)) {
        set_table_field_n(L, "iRightJoinTable", s->iRightJoinTable); /* If op&EP_FromJoin, the right table of the join */
    }
    if (s->pSelect != NULL){
        printf("expr->pSelect make as table\n");
        select_to_lua_table(L, s->pSelect);
        make_filed_to_table(L, "pSelect");
    }
    if(s->pLeft != NULL && s->pLeft->pRight != s){ // 如果pRight就是s，就会死循环
        printf("s->pLeft != NULL && s->pLeft->pRight != s, -->expr_to_lua_table(s->pLeft)\n");
        expr_to_lua_table(L, "pLeft", s->pLeft);
    }

    if(s->pRight != NULL && s->pRight->pLeft != s){
        printf("expr_to_lua_table(s-pRight)\n");
        expr_to_lua_table(L, "pRight", s->pRight);
    }
    // igrone pParent
    // if(s->pParent != NULL){
    //     expr_to_lua_table(L, "pParent", s->pParent);
    // }

    if(s->pTab != NULL && s->pTab->zName != NULL){
        set_table_field_s(L, "table", s->pTab->zName, strlen(s->pTab->zName));
    }
    if(s->pSchema != NULL){
        set_table_field_s(L,"pSchema", "TODO:schema", 11);
    }
   
    assert(lua_istable(L, -1));
    make_filed_to_table(L, field);
    return 1;
}

void SrcList_to_lua_array(lua_State *L, const char *field, const SrcList *l){
   assert(l != NULL);
   if(l->nSrc<=0)
       return;

   lua_pushstring(L, field);
   lua_newtable(L);
   int i=0;
   for(i=0; i<l->nSrc; i++){
        lua_pushnumber(L, i);
        lua_newtable(L);
        
        if(l->a[i].zDatabase)
            set_table_field_s(L, "db", l->a[i].zDatabase, strlen(l->a[i].zDatabase));
        if(l->a[i].zName)
            set_table_field_s(L, "table", l->a[i].zName, strlen(l->a[i].zName));
        if(l->a[i].zAlias)
            set_table_field_s(L, "alias", l->a[i].zAlias, strlen(l->a[i].zAlias));
        /* if(l->a[i].dbToken.z)
            set_table_field_s(L, "dbToken", l->a[i].dbToken.z, l->a[i].dbToken.n);
        if(l->a[i].tableToken.z)
            set_table_field_s(L, "tbToken", l->a[i].tableToken.z, l->a[i].tableToken.n); */

        if(l->a[i].pSelect){
           printf("TODO:A SELECT statement used in place of a table name\n");
        }
        if(l->a[i].pOn){
          expr_to_lua_table(L, "pOn", l->a[i].pOn);
        } 
        if(l->a[i].pUsing){
          printf("TODO: parse the pUsing.\n");
        }
        set_table_field_n(L, "joinType", l->a[i].jointype);
        
        lua_settable(L, -3);
   }
   lua_settable(L, -3);
}


void ExprList_to_lua_array(lua_State *L, const char *field, const ExprList *l){

   assert(l != NULL);
   if(l->nExpr <= 0)
       return;

   lua_pushstring(L, field);
   lua_newtable(L);
   int i=0;
   for(i=0; i<l->nExpr; i++){
       lua_pushnumber(L, i);
       lua_newtable(L);
       if(l->a[i].zName){
           set_table_field_s(L, "name", l->a[i].zName, strlen(l->a[i].zName));
       }
       if(l->a[i].pExpr){
           //printf("ExprList of [%s].expr->[%s]\n", field, l->a[i].pExpr->token.z);
           expr_to_lua_table(L, "expr", l->a[i].pExpr); 
       }
       if (l->a[i].sortOrder == 1){
          set_table_field_s(L, "orderBy", "DESC", 4);
       }
       if (l->a[i].sortOrder == 0){
          set_table_field_s(L, "orderBy", "ASC", 3);
       }
       set_table_field_bool(L, "isAgg", l->a[i].isAgg);
       lua_settable(L, -3);
   }
   lua_settable(L, -3);
}
