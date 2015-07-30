APP = expense

CC = gcc
CC_OPT = -Wall -g -std=gnu99

OBJ = main.o

$(APP): $(OBJ)
	$(CC) -o $(APP) $(OBJ) $(CC_OPT)

%.o: %.c
	$(CC) -c $<

run: $(APP)
	./$(APP)
