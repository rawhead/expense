#include <sqlite3.h>

//TODO: replace this string with dynamic option to choose database file
#define DATABASE_FILE "/home/fenk/expense/expense.db"

float dbGetSum();
char **dbGetList();
char **dbGetListSince(const char *date);
char **dbGetListFromTo(const char *dateFrom, const char *dateTo);
char dbAdd(const char *date, const char *purpose, float expense);
char dbDelete(int id);
