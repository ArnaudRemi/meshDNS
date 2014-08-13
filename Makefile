#meshDNS by rémi arnaud

SOURCES = main.c config.c parsing.c
NAME = mdns

all: build 

re: fclean build

build: 
	gcc $(SOURCES) -o $(NAME)

fclean: clean
	rm $(NAME)

clean:
	rm *.o

