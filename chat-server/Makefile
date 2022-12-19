LIBS=-lsqlite3
HEADERS=wsServer/*.h cJSON.h picohttpparser.h requests.h controller.h endpoints.h
FILES=main.c wsServer/*.c cJSON.c picohttpparser.c requests.c controller.c endpoints.c

main: $(FILES) $(HEADERS)
	gcc -g -fsanitize=address -Wall -o main $(FILES) $(LIBS)

.PHONY: clean

clean:
	rm -f main