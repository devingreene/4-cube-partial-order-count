NONSOURCE := int2table.o IsPoset.o main.o symmetries4.o filter.o 

.PHONY : execs
execs : printAllIsoPoz4

ifeq ($(CFLAGS),-ggdb)
OPTIMIZATION_OPT =
else 
OPTIMIZATION_OPT = -O2
endif

ifdef COVERAGE
OPTIMIZATION_OPT =
endif

WARNINGS = -Wall -Wextra -Werror
	
printAllIsoPoz4 : int2table.o IsPoset.o main.o symmetries4.o filter.o
	cc $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) $^ -o $@ $(COVERAGE)
int2table.o : int2table.c headers.h
	cc $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) -c $< $(COVERAGE)
IsPoset.o : IsPoset.c headers.h
	cc $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) -c $< $(COVERAGE)
main.o : main.c headers.h
	cc $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) -c $< $(COVERAGE)
symmetries4.o : symmetries4.c headers.h
	cc $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) -c $< $(COVERAGE)
filter.o : filter.c headers.h
	cc $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) -c $< $(COVERAGE)

clean : 
	rm -f $(NONSOURCE)
