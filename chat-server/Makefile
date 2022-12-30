LIBS=-lsqlite3
HEADERS=wsServer/*.h cJSON.h picohttpparser.h requests.h controller.h user_service.h hashtable/*.h messages_service.h
FILES=main.c wsServer/*.c cJSON.c picohttpparser.c requests.c controller.c user_service.c hashtable/*.c messages_service.c

main: $(FILES) $(HEADERS)
	gcc -g -fsanitize=address -Wall -o main $(FILES) $(LIBS)

.PHONY: clean

clean:
	rm -f main
