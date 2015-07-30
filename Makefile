APP = expense

CC = gcc
CC_OPT = -Wall -g -std=gnu99

OBJ = main.o db.o

$(APP): $(OBJ)
	$(CC) -o $(APP) $(OBJ) $(CC_OPT)

%.o: %.c
	$(CC) $(CC_OPT) -c $<

run: $(APP)
	./$(APP)
