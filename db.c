#include "db.h"

//TODO: find better name
#define TABLE_NAME "Bargeld"

#define COLUMN_ID       "id"
#define COLUMN_DATE     "date"
#define COLUMN_PURPOSE  "purpose"
#define COLUMN_EXPENSE  "expense"

#define QUERY_GET_SUM   "SELECT SUM(" COLUMN_EXPENSE ") FROM " TABLE_NAME
#define QUERY_GET_LIST  "SELECT " COLUMN_ID ", " COLUMN_DATE ", " \
  COLUMN_PURPOSE ", " COLUMN_EXPENSE " FROM " TABLE_NAME

sqlite3 *openDatabase(const char *path)
{
  sqlite3 *db;
  
  if(sqlite3_open(path, &db) != SQLITE_OK)
    return 0;

  return db;
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
  struct DBList *list = 0;
  struct DBList *listCurrent = 0;
  int length = 0;
  int result = 0;
  int rows = 0;
  sqlite3_stmt *statement;
  sqlite3 *db = openDatabase(DATABASE_FILE);

  if(!db)
    return 0;

  result = sqlite3_prepare_v2(db, QUERY_GET_LIST, -1, &statement, 0);
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
