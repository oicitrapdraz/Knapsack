target = knapsack
compiler = gcc
flags = -O3 -Wall -std=c99

$(target): main.o operators.o
	$(compiler) $(flags) $^ -o $@

main.o: main.c operators.h
	$(compiler) $(flags) -c main.c

operators.o: operators.c operators.h
	$(compiler) $(flags) -c operators.c

clean:
	rm -f $(target) *.o
