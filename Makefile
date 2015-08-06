APP=expense

CC=gcc
CC_OPT=-Wall -g -std=gnu99 -lsqlite3

OBJ=main.o db.o

MAN=$(APP).1
COPYING=COPYING

prefix=/usr/local

$(APP): $(OBJ)
	$(CC) -o $(APP) $(OBJ) $(CC_OPT)

%.o: %.c
	$(CC) $(CC_OPT) -c $<

run: $(APP)
	./$(APP)

install: $(APP)
	install -d $(prefix)/bin
	install -m 0755 $(APP) $(prefix)/bin
	install -d $(prefix)/share/man/man1
	gzip -c $(MAN) > $(prefix)/share/man/man1/$(MAN).gz
	install -d $(prefix)/share/licenses/$(APP)
	install -m 0755 $(COPYING) $(prefix)/share/licenses/$(APP)

uninstall:
	rm $(prefix)/bin/$(APP)
	rm $(prefix)/share/man/man1/$(MAN).gz
