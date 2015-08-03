#ifndef DB_H
#define DB_H

#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>

//TODO: replace this string with dynamic option to choose database file
#define DATABASE_FILE "/home/fenk/expense/expense.db"

char *dbGetSum();
char **dbGetList();
char **dbGetListSince(const char *date);
char **dbGetListFromTo(const char *dateFrom, const char *dateTo);
char dbAdd(const char *date, const char *purpose, double expense);
char dbDelete(int id);

#endif //DB_H
