#ifndef DB_H
#define DB_H

#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

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
char          dbAdd(const char *date, const char *purpose, const char *expense);
char          dbDelete(const char *id);

#endif //DB_H
