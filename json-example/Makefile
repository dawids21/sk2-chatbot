HEADERS=cJSON.h
FILES=main.c cJSON.c

main_file: $(FILES) $(HEADERS)
	gcc -Wall -o main $(FILES)

.PHONY: clean

clean:
	rm -f main