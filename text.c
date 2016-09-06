/*---------------------------------------------------------
-   Copyright (C): 2016
-   File name    : text.c
-   Author       : - Zhaoxinan -
-   Date         : 2016年05月26日 星期四 16时49分27秒
-   Description  : 
-                  
---------------------------------------------------------*/
#include <gtk-2.0/gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sqlite3.h>

char buf[80];       //临时数组用来拼接中文翻译
GtkWidget *word;    //文本框柄
sqlite3 *db = NULL; //数据库句柄

void increase(GtkWidget *widget, gpointer label)
{
    int row = 0;
    int column = 0;
    int ret = 0;

    char *errmsg = NULL;
    char **resultSet = NULL;
    char sql[500];
    const gchar *uname;
    uname = (gchar*)malloc(sizeof(gchar) * 80);
    uname = gtk_entry_get_text(GTK_ENTRY(word));
    //printf("uname = %s\n", uname);
    //凭借sql语句，用文本框获取的英文单词去数据库查找中文含义
    sprintf(sql, "select chineseword from entochword where englishword = '%s';", uname);
    //printf("sql = %s\n", sql);
    //使用get_table方法查询数据库
    if (SQLITE_OK != sqlite3_get_table(db, sql, &resultSet, &row, &column, &errmsg))
    {
        printf("select error!\n");
	exit(1);
    }
    //row > 0说明查到了记录
    if (row > 0)
    {
	strcpy(buf, "中文含义: ");
        strcat(buf, resultSet[1 * column + 0]);
        //printf("buf = %s\n", buf);
	//将中文含义显示在标签处
	gtk_label_set_text(label, buf);
    }
    else
    {
        strcpy(buf, "您查询的单词不再词库中！");
        gtk_label_set_text(label, buf);
    }
}

int main(int argc, char *argv[])
{
    GtkWidget *label;
    GtkWidget *window;
    GtkWidget *frame;
    GtkWidget *plus;
    GtkWidget *minus;

    gtk_init(&argc, &argv);

    //打开数据库
    if (SQLITE_OK != sqlite3_open("word.db", &db))
    {
        fprintf(stderr, "Cannot open database:%s\n", sqlite3_errmsg(db));
	sqlite3_close(db);
	exit(1);
    }

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);
    gtk_window_set_title(GTK_WINDOW(window),"英译汉电子词典-ZXN");

    frame = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), frame);
    
    //设置录入文本框
    word = gtk_entry_new();
    gtk_widget_set_size_request(word, 150,30);
    gtk_fixed_put(GTK_FIXED(frame), word, 25, 25);
    
    //设置按钮
    plus = gtk_button_new_with_label("查询");
    gtk_widget_set_size_request(plus, 60, 30);
    gtk_fixed_put(GTK_FIXED(frame), plus, 215, 25);
    
    //设置标签
    label = gtk_label_new(" ");
    gtk_fixed_put(GTK_FIXED(frame), label, 26, 60);
    
    gtk_widget_show_all(window);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(plus, "clicked", G_CALLBACK(increase), label);

    gtk_main();
    //sqlite3_close(db);
}

