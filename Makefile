SOURCE=containers_iterator
EXEC=test

all:
	g++ -std=c++17 -O2 -Wall -Wextra -pedantic -o $(EXEC) $(SOURCE).cpp

run:
	./$(EXEC)
