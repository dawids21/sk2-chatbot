LIBS=-lsqlite3
HEADERS=
FILES=main.c

main: $(FILES) $(HEADERS)
	gcc -Wall -o main $(FILES) $(LIBS)

.PHONY: clean

clean:
	rm -f main example.db