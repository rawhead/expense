#include "db.h"

//TODO: find better name
#define TABLE_NAME "Bargeld"

#define COLUMN_ID       "id"
#define COLUMN_DATE     "date"
#define COLUMN_PURPOSE  "purpose"
#define COLUMN_EXPENSE  "expense"

#define QUERY_GET_SUM           "SELECT SUM(" COLUMN_EXPENSE ") FROM " \
                                TABLE_NAME
#define QUERY_GET_LIST          "SELECT " COLUMN_ID ", " \
                                        COLUMN_DATE ", " \
                                        COLUMN_PURPOSE ", " \
                                        COLUMN_EXPENSE \
                                " FROM " TABLE_NAME
#define QUERY_GET_LIST_SINCE    "SELECT " COLUMN_ID ", " \
                                          COLUMN_DATE ", " \
                                          COLUMN_PURPOSE ", " \
                                          COLUMN_EXPENSE \
                                " FROM " TABLE_NAME \
                                " WHERE " COLUMN_DATE " >= "
#define QUERY_GET_LIST_FROM_TO  " AND " COLUMN_DATE " <= "

//TODO create function to create the database

sqlite3 *openDatabase(const char *path);
struct DBList *_dbGetList(const char *query);
char *dbToDBDate(const char *string);

sqlite3 *openDatabase(const char *path)
{
  sqlite3 *db;
  
  if(sqlite3_open(path, &db) != SQLITE_OK)
    return 0;

  return db;
}

char *dbToDBDate(const char *string)
{
  char *date;
  int pos = 0;

  date = (char *)malloc(9);

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
  sqlite3 *db = openDatabase(DATABASE_FILE);
  
  if(!db)
    return sum;

  result = sqlite3_prepare_v2(db, QUERY_GET_SUM, -1, &statement, 0);

  if(result != SQLITE_OK)
    return sum;

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
  char query[strlen(QUERY_GET_LIST_SINCE) + 9];
  char *dateString;

  dateString = dbToDBDate(date);
  if(!dateString)
    return 0;

  strcpy(query, QUERY_GET_LIST_SINCE);
  strcat(query, dateString);

  return _dbGetList(query);
}

struct DBList *dbGetListFromTo(const char *dateFrom, const char *dateTo)
{
  char query[strlen(QUERY_GET_LIST_SINCE) + 8
             + strlen(QUERY_GET_LIST_FROM_TO) + 9];
  char *dateFromFormatted, *dateToFormatted;

  dateFromFormatted = dbToDBDate(dateFrom);
  if(!dateFromFormatted)
    return 0;

  dateToFormatted = dbToDBDate(dateTo);
  if(!dateToFormatted)
    return 0;

  strcpy(query, QUERY_GET_LIST_SINCE);
  strcat(query, dateFromFormatted);
  strcat(query, QUERY_GET_LIST_FROM_TO);
  strcat(query, dateToFormatted);

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
  sqlite3 *db = openDatabase(DATABASE_FILE);

  if(!db)
    return 0;

  result = sqlite3_prepare_v2(db, query, -1, &statement, 0);
  if(result != SQLITE_OK)
    return 0;

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

  return list;
}
