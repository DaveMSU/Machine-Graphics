CC=g++
CFLAG = -c

all: clean compile
	./tmp
	rm tmp

compile: geometry.o my_lib.o v1.0.o
	$(CC) -o tmp v1.0.o my_lib.o geometry.o
	@echo "All seccessfully complited!"

geom_test: geom_test.o geometry.o
	$(CC) -o tmp geom_test.o geometry.o

geom_test.o:
	$(CC) $(CFLAG) geom_test.cpp
	
v1.0.o:
	$(CC) $(CFLAG) v1.0.cpp

my_lib.o:
	$(CC) $(CFLAG) my_lib.cpp

geometry.o:
	$(CC) $(CFLAG) geometry.cpp

clean:
	@rm -rf .??*
	@rm -rf *.o
