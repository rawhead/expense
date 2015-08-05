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

char test_date(const char *string);
char test_number(const char *string);

int main(int argc, char **args)
{
  if(argc > 5)
  {
    perror("Too much arguments. Aborting.\n");
    printf(USAGE, args[0]);
    return EINVAL;
  }

  if(argc < 2)
  {
    char *sum = dbGetSum();
    if(!sum)
    {
      perror("Something bad happened. Aborting");
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
      if(!test_date(args[2]) || !test_date(args[2]))
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
      if(!test_date(args[2]))
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
    printf("ID\tDATE\t\tPURPOSE\tEXPENSE\n");
    while(list)
    {
      printf("%s\t%s\t%s\t%s\n", list->id, list->date, list->purpose,
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
    if(argc > 4)
    {
      if(!test_date(args[2]))
      {
        fprintf(stderr, "The format of the given date \"%s\" is \
not correct. Please use following format: yyyy-mm-dd.\
Aborting.\n", args[2]);
        return EINVAL;
      }
      if(!dbAdd(args[2], args[3], args[4]))
      {
        perror("Could not add entry. Aborting");
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
      perror("Could not add entry. Aborting");
      return -1;
    }
    return 0;
  }

  if(!strcmp(args[1], OPT_DELETE))
  {
    if(argc != 3)
    {
      perror("Wrong number of arguments for option \"delete\". Aborting\n");
      printf(USAGE, args[0]);
      return EINVAL;
    }
    if(!test_number(args[3]))
    {
      perror("The value \"%s\" is not a number. Aborting\n");
      return EINVAL;
    }
  }

  fprintf(stderr, "Unknown argument :\"%s\". Aborting", args[1]);
  return EINVAL;
}

char test_date(const char *string)
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

char test_number(const char *string)
{
  for(int i = 0; i < (int)strlen(string); i++)
  {
    if(string[i] < 48 ||
       string[i] > 57)
      return 0;
  }
  return 1;
}
