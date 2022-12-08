HEADERS=cJSON.h picohttpparser.h requests.h
FILES=main.c cJSON.c picohttpparser.c requests.c

main: $(FILES) $(HEADERS)
	gcc -g -Wall -o main $(FILES)

.PHONY: clean

clean:
	rm -f main