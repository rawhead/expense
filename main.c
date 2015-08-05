#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "db.h"

#define USAGE "\
%s - usage\n \
--------\n\n \
---\t\t\t\tshow current estate\n \
list\t[DATE FROM [DATE TO]]\tlist saved entrys\n \
add\t[DATE] PURPOSE EXPENSE\tadd a new entry\n \
delete\tID\t\t\tdelete entry by id\n \
help\t\t\t\tshow this text\n"

#define OPT_LIST    "list"
#define OPT_ADD     "add"
#define OPT_DELETE  "delete"
#define OPT_HELP_1  "help"
#define OPT_HELP_2  "-h"
#define OPT_HELP_3  "--help"

enum DateType
{
  DMY,
  YMD
};

char testDate(const char *string);
char testNumber(const char *string);
char *formatDate(const char *string, enum DateType dateType);

int main(int argc, char **args)
{
  if(argc < 2)
  {
    char *sum = dbGetSum();
    if(!sum)
    {
      perror("Something bad happened. Aborting\n");
      return -1;
    }
    printf("%s\n", sum);
    free(sum);
    return 0;
  }

  if(!strcmp(args[1], OPT_HELP_1) ||
     !strcmp(args[1], OPT_HELP_2) ||
     !strcmp(args[1], OPT_HELP_3))
  {
    printf(USAGE, args[0]);
    return 0;
  }

  if(!strcmp(args[1], OPT_LIST))
  {
    struct DBList *list;
    struct DBList *listBegin;
    if(argc > 4)
    {
      perror("Too much Arguments for option \"list\". Aborting\n");
      printf(USAGE, args[0]);
      return EINVAL;
    }
    if(argc > 3)
    {
      if(!testDate(args[2]) || !testDate(args[2]))
      {
        fprintf(stderr, "The format of the given dates \"%s\", \"%s\" is \
not correct. Please use following format: yyyy-mm-dd.\
Aborting.\n", args[2], args[3]);
        return EINVAL;
      }
      list = dbGetListFromTo(args[2], args[3]);
      goto print_list;
    }
    if(argc > 2)
    {
      if(!testDate(args[2]))
      {
        fprintf(stderr, "The format of the given date \"%s\" is \
not correct. Please use following format: yyyy-mm-dd.\
Aborting.\n", args[2]);
        return EINVAL;
      }
      list = dbGetListSince(args[2]);
      goto print_list;
    }
    list = dbGetList();
print_list:
    listBegin = list;
    if(!list)
    {
      printf("No data available.\n");
      return 0;
    }
    printf("%-6s%-13s%-18s%s\n", "ID", "DATE", "PURPOSE", "EXPENSE");
    printf("----------------------------------------------\n");
    while(list)
    {
      printf("%-6s%-13s%-18s%s\n",
             list->id,
             formatDate(list->date, YMD),
             list->purpose,
             list->expense);
      list = list->next;
    }
    dbClearList(listBegin);
    return 0;
  }

  if(!strcmp(args[1], OPT_ADD))
  {
    if(argc < 4)
    {
      perror("The optione \"add\" needs more arguments. Aborting.\n");
      printf(USAGE, args[0]);
      return EINVAL;
    }
    if(argc < 4)
    {
      perror("To much arguments for option \"add\". Aborting.\n");
      printf(USAGE, args[0]);
      return EINVAL;
    }
    if(argc == 5)
    {
      if(!testDate(args[2]))
      {
        fprintf(stderr, "The format of the given date \"%s\" is \
not correct. Please use following format: yyyy-mm-dd.\
Aborting.\n", args[2]);
        return EINVAL;
      }
      if(!dbAdd(args[2], args[3], args[4]))
      {
        perror("Could not add entry. Aborting\n");
        return -1;
      }
      return 0;
    }
    time_t t = time(0);
    struct tm tm = *localtime(&t);
    char date[11];
    strftime(date, 11, "%Y-%m-%d", &tm);

    if(!dbAdd(date, args[2], args[3]))
    {
      perror("Could not add entry. Aborting\n");
      return -1;
    }
    return 0;
  }

  if(!strcmp(args[1], OPT_DELETE))
  {
    if(argc < 3)
    {
      perror("Not enough arguments for option \"delete\". Aborting\n");
      printf(USAGE, args[0]);
      return EINVAL;
    }
    
    char *ids[argc - 2];
    for(int i = 2; i < argc; i++)
    {
      if(!testNumber(args[i]))
      {
        fprintf(stderr, "The value \"%s\" is not a number. Aborting\n", args[i]);
        return EINVAL;
      }
      ids[i - 2] = (char *)malloc(strlen(args[i]));
      strcpy(ids[i - 2], args[i]);
    }
    for(int i = 0; i < argc - 2; i++)
    {
      if(!dbDelete(ids[i]))
      {
        fprintf(stderr, "Could not delete entry %s. Aborting.\n", ids[i]);
        return -1;
      }
    }
    return 0;
  }

  fprintf(stderr, "Unknown argument :\"%s\". Aborting\n", args[1]);
  return EINVAL;
}

char testDate(const char *string)
{
  int i = 0;

  if(strlen(string) != 10)
    return 0;

  for(i = 0; i < 4; i++)
  {
    if(string[i] < 48 ||
       string[i] > 57)
      return 0;
  }

  if(string[i] != '-')
    return 0;

  for(i = 5; i < 7; i++)
  {
    if(string[i] < 48 ||
       string[i] > 57)
      return 0;
  }

  if(string[i] != '-')
    return 0;

  for(i = 8; i < 10; i++)
  {
    if(string[i] < 48 ||
       string[i] > 57)
      return 0;
  }
  return 1;
}

char testNumber(const char *string)
{
  for(int i = 0; i < (int)strlen(string); i++)
  {
    if(string[i] < 48 ||
       string[i] > 57)
      return 0;
  }
  return 1;
}

char *formatDate(const char *string, enum DateType dateType)
{
  char *date;
  char year[5];
  char month[3];
  char day[3];
  int i = 0;

  date = (char *)malloc(11);

  if(testNumber(string))
  {
    if(strlen(string) != 8)
      return 0;

    for(i = 0; i < 4; i++)
      year[i] = string[i];
    year[i] = '\0';
    for(i = 0; i < 2; i++)
      month[i] = string[i + 4];
    month[i] = '\0';
    for(i = 0; i < 2; i++)
      day[i] = string[i + 6];
    day[i] = '\0';
  }

  switch(dateType)
  {
    case YMD:
      sprintf(date, "%s-%s-%s", year, month, day);
      break;

    case DMY:
      sprintf(date, "%s.%s.%s", day, month, year);
      break;

    default:
      return 0;
  }

  return date;
}
