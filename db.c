#include "db.h"

//TODO: find better name
#define TABLE_NAME "Bargeld"

#define COLUMN_ID "id"
#define COLUMN_DATE "date"
#define COLUMN_PURPOSE "purpose"
#define COLUMN_EXPENSE "expense"

sqlite3 *openDatabase(const char *path)
{
  sqlite3 *db;
  
  if(sqlite3_open(path, &db) != SQLITE_OK)
    return 0;

  return db;
}

float dbGetSum()
{
  float sum = 0.001;
  int result = 0;
  sqlite3_stmt *statement;
  sqlite3 *db = openDatabase(DATABASE_FILE);
  
  if(!db)
    return sum;

  result = sqlite3_prepare_v2(db, "SELECT SQLITE_VERSION()", -1, $
}
