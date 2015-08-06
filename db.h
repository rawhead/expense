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
