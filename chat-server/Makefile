LIBS=-lsqlite3
HEADERS=wsServer/*.h cJSON.h picohttpparser.h requests.h
FILES=main.c wsServer/*.c cJSON.c picohttpparser.c requests.c

main: $(FILES) $(HEADERS)
	gcc -g -Wall -o main $(FILES) $(LIBS)

.PHONY: clean

clean:
	rm -f main