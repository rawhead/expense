#include <stdio.h>
#include <string.h>
#include <errno.h>

#define USAGE "\
%s - usage\n \
--------\n\n \
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

int main(int argc, char **args)
{
  if(argc > 6)
  {
    perror("Too much arguments. Aborting.\n");
    printf(USAGE, args[0]);
    return EINVAL;
  }

  if(argc < 2)
  {
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
    if(argc > 5)
    {
      perror("Too much Arguments for option \"list\". Aborting\n");
      printf(USAGE, args[0]);
      return EINVAL;
    }
    if(argc > 4)
    {

    }
    if(argc > 3)
    {

    }
  }

  if(!strcmp(args[1], OPT_ADD))
  {
    if(argc < 5)
    {
      perror("The optione \"add\" needs more arguments. Aborting.\n");
      printf(USAGE, args[0]);
      return EINVAL;
    }
    if(argc > 5)
    {
      args++;
    }
  }

  if(!strcmp(args[1], OPT_DELETE))
  {
    if(argc != 4)
    {
      perror("Wrong number of arguments for option \"delete\". Aborting\n");
      printf(USAGE, args[0]);
      return EINVAL;
    }
  }

  fprintf(stderr, "Unknown argument :\"%s\". Aborting", args[1]);
  return EINVAL;
}
