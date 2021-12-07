merge.out: merge.o
	g++ -o merge.out merge.o
merge.o: merge.cpp
	g++ -c merge.cpp

debug:
	g++ -g -o merge.out merge.cpp
clean:
	rm -f *.out *.o *.stackdump *~