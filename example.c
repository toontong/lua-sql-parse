#include "src/sqliteInt.h"
#include "cstring/cstring.h"
#include <string.h>

int main(int argc, char* argv[]) {
		Parse parseObj;
		char *errMsg = 0;
		u32 i;
		int j;
		ParsedResultItem *itm;
		Select *pSrc;
		memset(&parseObj, 0, sizeof(parseObj));

		sqlite3RunParser(&parseObj, "select * from test where id > 100 and id < 200 or id between 300 and 400 or id = 1000;", &errMsg);
		//sqlite3RunParser(&parseObj, "select count(*) from test union select id from test where id IN (select id from test1);", &errMsg);
		if( sqlite3MallocFailed()  ){
				parseObj.rc = SQLITE_NOMEM;
		}
		if( parseObj.rc==SQLITE_DONE ) parseObj.rc = SQLITE_OK;
		if (errMsg != NULL) {
				printf("error: %s, error_code:%d \n", errMsg, parseObj.rc);
				return 1;
		}

		//printf("error seen=%d\n", parseObj.nErr);
		printf("rc=%d, ", parseObj.rc);
		//printf("memory cells=%d\n", parseObj.nMem);
		//printf("used sets=%d\n", parseObj.nSet);
		printf("explain=%d\n", parseObj.explain);
		if (parseObj.sLastToken.z != NULL)
			printf("sLastToken=%s, dyn=%d, n=%d\n", parseObj.sLastToken.z, parseObj.sLastToken.dyn, parseObj.sLastToken.n);

		for (i = 0; i < parseObj.parsed.curSize; i++) {
				itm = &(parseObj.parsed.array[i]);

				if (SQLTYPE_SELECT == itm->sqltype){
						printf("parsed.item[%d].sqltype=[SQLTYPE_SELECT]\n",i);
						pSrc = itm->result.selectObj;

						printf("FOMR.nSrc=[%d]:\n", pSrc->pSrc->nSrc);

						for (j = 0; j< pSrc->pSrc->nSrc; j++){
								printf("\tFOMR list of [%d]-->db[%s].tb[%s]",  j,
												pSrc->pSrc->a[j].zDatabase,
												pSrc->pSrc->a[j].zName );
								if ( pSrc->pSrc->a[j].zAlias != NULL ){
										printf(".alias=>[%s]", pSrc->pSrc->a[j].zAlias );
								}
								printf("\n");
						}
						if(pSrc->pWhere != NULL && pSrc->pWhere->pList != NULL){
								printf("WHERE-> pList -> nExpr = [%d]\n", pSrc->pWhere->pList->nExpr);
						}
				}else{
						printf("unhandle---->_< parsed.item[%d].sqltype=[%d]\n",i, itm->sqltype);
				}
		}


	printf("\tparseObj.zSql:%s\n", parseObj.zSql);
		 
		return 0;
}
