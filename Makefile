#meshDNS by rémi arnaud

SOURCES = main.c config.c parsing.c request.c response.c
OFILE = main.o config.o parsing.o request.o response.o
NAME = mdns

all: build 

re: fclean build

build: 
	gcc $(SOURCES) -o $(NAME)

fclean: clean
	rm -f $(NAME)

clean:
	rm -f $(OFILE)

