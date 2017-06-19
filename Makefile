NONSOURCE := createPermBlock.o int2table.o IsPoset.o \
			 main.o symmetries4.o filter.o 

.PHONY : execs
execs : printAllIsoPoz4

ifeq ($(CFLAGS),-ggdb)
OPTIMIZATION_OPT=
else 
OPTIMIZATION_OPT = -O3
endif
	
createPermBlock.o : createPermBlock.c headers.h
	cc  $(CFLAGS) $(OPTIMIZATION_OPT) -c $<
printAllIsoPoz4 : int2table.o IsPoset.o main.o symmetries4.o filter.o
	cc  $(CFLAGS) $(OPTIMIZATION_OPT) $^ -o $@
int2table.o : int2table.c headers.h
	cc  $(CFLAGS) $(OPTIMIZATION_OPT) -c $<
IsPoset.o : IsPoset.c headers.h
	cc  $(CFLAGS) $(OPTIMIZATION_OPT) -c $<
main.o : main.c headers.h
	cc  $(CFLAGS) $(OPTIMIZATION_OPT) -c $<
symmetries4.o : symmetries4.c headers.h
	cc  $(CFLAGS) $(OPTIMIZATION_OPT) -c $<
filter.o : filter.c headers.h
	cc $(CFLAGS) $(OPTIMIZATION_OPT) -c $<

clean : 
	rm -f $(NONSOURCE)
