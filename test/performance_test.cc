
#include "gtest/gtest.h"
#include "../src/sqliteInt.h"
#include <string.h>
#include <sys/time.h>

long getTime() {
    struct timeval iTime;
    gettimeofday(&iTime, NULL);
    long lTime = ((long) iTime.tv_sec) * 1000000 + (long) iTime.tv_usec;
    return lTime;
}

Token *findKey(Expr *expr, const char *key) {
    if (!expr) { return NULL; }

    if (expr->op == TK_EQ && expr->pLeft != NULL) {
        if (strncmp((const char*)expr->pLeft->token.z, key, expr->pLeft->token.n) == 0) {
            return &expr->pLeft->token;
        }
    }

    if (expr->pLeft != NULL) {
        Token *token = findKey(expr->pLeft, key);
        if (token != NULL) { return token; }
    }

    if (expr->pRight != NULL) {
        Token *token = findKey(expr->pRight, key);
        if (token != NULL) { return token; }
    }

    return NULL;
}

TEST(LemonTest, testPerformance) {
    Parse *parseObj = sqlite3ParseNew();
    char *errMsg = 0;
    long time_start = getTime();
	long sum = 1000000;
    for (int i = 0; i < sum; i++) {
        resetParseObject(parseObj);
        int errNum = sqlite3RunParser(parseObj, "SELECT id, name FROM test WHERE name = 'test' AND id = 1000;", &errMsg);
        if (errMsg != NULL){
				printf("%s", errMsg);
				return;
		}
        Select *selectObj = parseObj->parsed.array[0].result.selectObj;
        strcmp(selectObj->pSrc->a[0].zName, "test");
        Token *pToken = findKey(selectObj->pWhere, "id");
        sqlite3SelectDelete(selectObj);
		parseObj->parsed.array[0].result.selectObj=NULL;
    }

    long time_end = getTime();
    double qps = sum/((time_end - time_start)/1000.0/1000.0);
    printf("time escaped:%ld  qps=%f \n", time_end - time_start, qps);
  	sqlite3ParseDelete(parseObj);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
	printf("go into main ....\n");
    return RUN_ALL_TESTS();
}
