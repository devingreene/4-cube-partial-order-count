NONSOURCE := createPermBlock.o int2table.o IsPoset.o \
			 main.o symmetries4.o filter.o 

.PHONY : execs
execs : printAllIsoPoz4
	
createPermBlock.o : createPermBlock.c headers.h
	cc  $(CFLAGS) -c $<
printAllIsoPoz4 : int2table.o IsPoset.o main.o symmetries4.o filter.o
	cc  $^ -o $@
int2table.o : int2table.c headers.h
	cc  $(CFLAGS) -c $<
IsPoset.o : IsPoset.c headers.h
	cc  $(CFLAGS) -c $<
main.o : main.c headers.h
	cc  $(CFLAGS) -c $<
symmetries4.o : symmetries4.c headers.h
	cc  $(CFLAGS) -c $<
filter.o : filter.c headers.h
	cc $(CFLAGS) -c $<


clean : 
	rm -f $(NONSOURCE)
