#include <sqlite3.h>

float dbGetSum();
char **dbGetList();
char **dbGetListSince(const char *date);
char **dbGetListFromTo(const char *dateFrom, const char *dateTo);
char dbAdd(const char *date, const char *purpose, float expense);
char dbDelete(int id);
