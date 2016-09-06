/*---------------------------------------------------------
-   Copyright (C): 2016
-   File name    : create_table.c
-   Author       : - Zhaoxinan -
-   Date         : 2016年05月25日 星期三 16时42分21秒
-   Description  : 
-                  
---------------------------------------------------------*/
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <sqlite3.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>

#define SIZE 400000


int main()
{
//    int i = 0;
    int ret = 0;
    sqlite3 *db = NULL;
    
    FILE *fp;
    int flag = 0;
    
    char *head = NULL;
    char *mind = NULL;
    char *tail = NULL;

    char *sql = NULL;
    char *errmsg = NULL;
    char buffer[SIZE];
    
    if (SQLITE_OK != sqlite3_open("word.db", &db))
    {
	fprintf(stderr,"Cannot open database:%s\n", sqlite3_errmsg(db));
	sqlite3_close(db);
	exit(1);
    }
    printf("------------\n");
    sql = (char *)malloc(sizeof(char) * 500);
    if (sql == NULL)
    {
	printf("create_sqlite malloc error!\n");
	exit(1);
    }
    printf("************\n");
    ret = sqlite3_exec(db, "create table if not exists entochword(englishword varchar(30) primary key, chineseword varchar(80));",NULL,NULL,&errmsg);
    if (ret != SQLITE_OK)
    {
	fprintf(stderr, "Create table err:");
    }
    //打开文件
    fp = fopen("entochword.csv", "r");

    fread(buffer, SIZE, 1, fp);
    head = buffer;
    mind = buffer;
    tail = buffer;
    printf("~~~~~~~~~~~~~~~\n");
   // printf("%s",tail);
    while(*tail != '^')
    {
	if (*tail == '\n')
	{
            *tail = '\0';
	    while (*mind != ',')
	    {
		mind++;
	    }
	    *mind = '\0';
	    
	    mind++;
            //memset(sql, '\0', SIZE);
            sprintf(sql, "insert into entochword values('%s','%s');",head, mind);
            printf("%s\n",sql);
	    sqlite3_exec(db, sql, NULL, NULL, &errmsg);
	    //usleep(500000);
	    //printf("head = %s\n", head);
	    //printf("mind = %s\n", mind);
	    tail++;
	    head = tail;
	    mind = tail;
	    if (*tail == '^')
	    {
		break;
	    }
	    flag = 1;
	}
        if (flag == 1)
	{
	    flag = 0;
	    continue;
	}
	tail++;
    }
    printf("success\n");
    sleep(3);
    sqlite3_close(db);
    fclose(fp);
    return 0;
}












