# Это комментарий, который говорит, что переменная CC указывает компилятор, используемый для сборки
CC=g++-7
#Это еще один комментарий. Он поясняет, что в переменной CFLAGS лежат флаги, которые передаются компилятору
CFLAGS=-c -Wall -std=c++14# -g
LIBS   = -lpthread

BOOST_ROOT=/usr/local
BOOST_INC=${BOOST_ROOT}/include/boost/

all: clsfr

clsfr: main.o html.o theme.o
	$(CC) main.o html.o theme.o -I$(BOOST_INC) $(LIBS) -o clsfr

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

html.o: html.cpp
	$(CC) $(CFLAGS) html.cpp

theme.o: theme.cpp
	$(CC) $(CFLAGS) theme.cpp

clean:
	rm -rf *.o nirr
