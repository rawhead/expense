#include <stdio.h>
#include <string.h>

#define USAGE "\
%s - usage\n \
--------\n\n \
list\t[DATE FROM [DATE TO]]\tlist saved entrys\n \
add\t[DATE] PURPOSE EXPENSE\tadd a new entry\n \
delete\tID\t\t\tdelete entry by id\n \
help\t\t\t\tshow this text\n"

int main(int argc, char **args)
{
  printf(USAGE, args[0]);

  return 0;
}
