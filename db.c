//expense - manages expenses
//Copyright (C) 2015  Julian Fenk julian@nastymatter.net

//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License along
//with this program; if not, write to the Free Software Foundation, Inc.,
//51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include "db.h"

#define NAME ".expense.db"
#define TABLE_NAME "Expenses"

#define COLUMN_ID       "id"
#define COLUMN_DATE     "date"
#define COLUMN_PURPOSE  "purpose"
#define COLUMN_EXPENSE  "expense"

#define COLUMN_ID_TYPE      "INTEGER PRIMARY KEY AUTOINCREMENT"
#define COLUMN_DATE_TYPE    "INTEGER NOT NULL"
#define COLUMN_PURPOSE_TYPE "TEXT NOT NULL"
#define COLUMN_EXPENSE_TYPE "REAL NOT NULL"

#define QUERY_CREATE_TABLE      "CREATE TABLE IF NOT EXISTS " \
                                TABLE_NAME "( " \
                                COLUMN_ID " " COLUMN_ID_TYPE ", " \
                                COLUMN_DATE " " COLUMN_DATE_TYPE ", " \
                                COLUMN_PURPOSE " " COLUMN_PURPOSE_TYPE ", " \
                                COLUMN_EXPENSE " " COLUMN_EXPENSE_TYPE ")"
#define QUERY_GET_SUM           "SELECT SUM(" COLUMN_EXPENSE ") FROM " \
                                TABLE_NAME
#define QUERY_GET_LIST          "SELECT " COLUMN_ID ", " \
                                        COLUMN_DATE ", " \
                                        COLUMN_PURPOSE ", " \
                                        COLUMN_EXPENSE \
                                " FROM " TABLE_NAME \
                                QUERY_ORDER_BY_DATE
#define QUERY_GET_LIST_SINCE    "SELECT " COLUMN_ID ", " \
                                          COLUMN_DATE ", " \
                                          COLUMN_PURPOSE ", " \
                                          COLUMN_EXPENSE \
                                " FROM " TABLE_NAME \
                                " WHERE " COLUMN_DATE " >= "
#define QUERY_GET_LIST_FROM_TO  " AND " COLUMN_DATE " <= "
#define QUERY_ORDER_BY_DATE     " ORDER BY " COLUMN_DATE
#define QUERY_ADD_1             "INSERT INTO " TABLE_NAME "(" \
                                COLUMN_DATE ", " \
                                COLUMN_PURPOSE ", " \
                                COLUMN_EXPENSE \
                                ") VALUES ("
#define QUERY_ADD_2             ", \""
#define QUERY_ADD_3             "\", "
#define QUERY_ADD_4             ")"
#define QUERY_DELETE            "DELETE FROM " TABLE_NAME " WHERE id="

#define DATE_LENGTH         8
#define MAX_EXPENSE_LENGTH  10
#define MAX_PURPOSE_LENGTH  128

sqlite3 *openDatabase();
struct DBList *_dbGetList(const char *query);
char *dbToDBDate(const char *string);

sqlite3 *openDatabase()
{
  sqlite3 *db;
  int result = 0;
  sqlite3_stmt *statement;
  struct passwd *pw;

  pw = getpwuid(getuid());
  char file[strlen(pw->pw_dir) + 1 + strlen(NAME)];
  sprintf(file, "%s/%s", pw->pw_dir, NAME);

  result = sqlite3_open_v2(file, &db, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, 0);
  if(result != SQLITE_OK)
    return 0;

  result = sqlite3_prepare_v2(db, QUERY_CREATE_TABLE, -1, &statement, 0);

  if(result != SQLITE_OK)
  {
    sqlite3_close(db);
    return 0;
  }

  result = sqlite3_step(statement);
  if(result != SQLITE_DONE && result != SQLITE_ROW)
  {
    sqlite3_close(db);
    return 0;
  }

  sqlite3_finalize(statement);

  return db;
}

char *dbToDBDate(const char *string)
{
  char *date;
  int pos = 0;

  date = (char *)malloc(DATE_LENGTH + 1);

  for(int i = 0; i < (int)strlen(string); i++)
  {
    if(string[i] > 47 && string[i] < 58)
    {
      date[pos] = string[i];
      pos++;
    }
  }

  date[pos] = '\0';

  return date;
}

char *dbGetSum()
{
  char *sum = 0;
  int length = 0;
  int result = 0;
  sqlite3_stmt *statement;
  sqlite3 *db = openDatabase();
  
  if(!db)
    return sum;

  result = sqlite3_prepare_v2(db, QUERY_GET_SUM, -1, &statement, 0);

  if(result != SQLITE_OK)
  {
    sqlite3_close(db);
    return sum;
  }

  result = sqlite3_step(statement);

  if(result == SQLITE_ROW)
  {
    length = sqlite3_column_bytes(statement, 0);
    sum = (char *)malloc(length);
    strcpy(sum, (char *)sqlite3_column_text(statement, 0));
  }

  sqlite3_finalize(statement);
  sqlite3_close(db);

  return sum;
}

struct DBList *dbGetList()
{
  return _dbGetList(QUERY_GET_LIST);
}

struct DBList *dbGetListSince(const char *date)
{
  char query[strlen(QUERY_GET_LIST_SINCE)
             + DATE_LENGTH
             + strlen(QUERY_ORDER_BY_DATE)];
  char *dateString;

  dateString = dbToDBDate(date);
  if(!dateString)
    return 0;

  sprintf(query, "%s%s%s",
          QUERY_GET_LIST_SINCE,
          dateString,
          QUERY_ORDER_BY_DATE);

  return _dbGetList(query);
}

struct DBList *dbGetListFromTo(const char *dateFrom, const char *dateTo)
{
  char query[strlen(QUERY_GET_LIST_SINCE)
             + DATE_LENGTH
             + strlen(QUERY_GET_LIST_FROM_TO)
             + DATE_LENGTH
             + strlen(QUERY_ORDER_BY_DATE)];
  char *dateFromFormatted, *dateToFormatted;

  dateFromFormatted = dbToDBDate(dateFrom);
  if(!dateFromFormatted)
    return 0;

  dateToFormatted = dbToDBDate(dateTo);
  if(!dateToFormatted)
    return 0;

  sprintf(query, "%s%s%s%s%s",
          QUERY_GET_LIST_SINCE,
          dateFromFormatted,
          QUERY_GET_LIST_FROM_TO,
          dateToFormatted,
          QUERY_ORDER_BY_DATE);

  return _dbGetList(query);
}

void dbClearList(struct DBList *list)
{
  struct DBList *temp;
  while(list)
  {
    free(list->id);
    free(list->date);
    free(list->purpose);
    free(list->expense);
    temp = list;
    list = list->next;
    free(temp);
  }
}

struct DBList *_dbGetList(const char *query)
{
  struct DBList *list = 0;
  struct DBList *listCurrent = 0;
  int length = 0;
  int result = 0;
  int rows = 0;
  sqlite3_stmt *statement;
  sqlite3 *db = openDatabase();

  if(!db)
    return 0;

  result = sqlite3_prepare_v2(db, query, -1, &statement, 0);
  if(result != SQLITE_OK)
  {
    sqlite3_close(db);
    return 0;
  }

  result = sqlite3_step(statement);
  while(result == SQLITE_ROW)
  {
    if(rows)
    {
      listCurrent->next = (struct DBList *)malloc(sizeof(struct DBList));
      listCurrent = listCurrent->next;
    }
    else
    {
      list = (struct DBList *)malloc(sizeof(struct DBList));
      listCurrent = list;
    }
    listCurrent->next = 0;
    
    // ID
    length = sqlite3_column_bytes(statement, 0);
    listCurrent->id = (char *)malloc(length);
    strcpy(listCurrent->id, (char *)sqlite3_column_text(statement, 0));

    // DATE
    length = sqlite3_column_bytes(statement, 1);
    listCurrent->date = (char *)malloc(length);
    strcpy(listCurrent->date, (char *)sqlite3_column_text(statement, 1));

    // PURPOSE
    length = sqlite3_column_bytes(statement, 2);
    listCurrent->purpose = (char *)malloc(length);
    strcpy(listCurrent->purpose, (char *)sqlite3_column_text(statement, 2));

    // EXPENSE
    length = sqlite3_column_bytes(statement, 3);
    listCurrent->expense = (char *)malloc(length);
    strcpy(listCurrent->expense, (char *)sqlite3_column_text(statement, 3));

    rows++;
    result = sqlite3_step(statement);
  }

  sqlite3_finalize(statement);
  sqlite3_close(db);

  return list;
}

char dbAdd(const char *date, const char *purpose, const char *expense)
{
  char query[strlen(QUERY_ADD_1)
             + DATE_LENGTH
             + strlen(QUERY_ADD_2)
             + MAX_PURPOSE_LENGTH
             + strlen(QUERY_ADD_3)
             + MAX_EXPENSE_LENGTH
             + strlen(QUERY_ADD_4)];
  char *dateFormatted;
  int result = 0;
  sqlite3_stmt *statement;
  sqlite3 *db;

  dateFormatted = dbToDBDate(date);
  if(!dateFormatted)
    return 0;

  sprintf(query, "%s%s%s%s%s%s%s",
          QUERY_ADD_1,
          dateFormatted,
          QUERY_ADD_2,
          purpose,
          QUERY_ADD_3,
          expense,
          QUERY_ADD_4);

  db = openDatabase();
  if(!db)
    return 0;

  result = sqlite3_prepare_v2(db, query, -1, &statement, 0);
  if(result != SQLITE_OK)
  {
    sqlite3_close(db);
    return 0;
  }

  result = sqlite3_step(statement);
  if(result != SQLITE_DONE && result != SQLITE_ROW)
  {
    sqlite3_close(db);
    return 0;
  }

  sqlite3_finalize(statement);
  sqlite3_close(db);

  return 1;
}

char dbDelete(const char *id)
{
  char query[strlen(QUERY_DELETE)
             + strlen(id)];
  int result = 0;
  sqlite3_stmt *statement;
  sqlite3 *db;

  sprintf(query, "%s%s",
          QUERY_DELETE,
          id);

  db = openDatabase();
  if(!db)
    return 0;

  result = sqlite3_prepare_v2(db, query, -1, &statement, 0);
  if(result != SQLITE_OK)
  {
    sqlite3_close(db);
    return 0;
  }

  result = sqlite3_step(statement);
  if(result != SQLITE_DONE && result != SQLITE_ROW)
  {
    sqlite3_close(db);
    return 0;
  }

  sqlite3_finalize(statement);
  sqlite3_close(db);

  return 1;
}
