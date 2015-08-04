#ifndef DB_H
#define DB_H

#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>

//TODO: replace this string with dynamic option to choose database file
#define DATABASE_FILE "/home/fenk/expense/expense.db"

struct DBList
{
  char          *id;
  char          *date;
  char          *purpose;
  char          *expense;
  struct DBList *next;
};

char          *dbGetSum();
struct DBList *dbGetList();
struct DBList *dbGetListSince(const char *date);
struct DBList *dbGetListFromTo(const char *dateFrom, const char *dateTo);
void          dbClearList(struct DBList *list);
char          dbAdd(const char *date, const char *purpose, double expense);
char          dbDelete(int id);

#endif //DB_H
