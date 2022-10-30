OBJECTS := int2table.o IsPoset.o main.o symmetries4.o filter.o filter_stanley.o
TEST := TESTING_b534ff4f

.PHONY : execs test
execs : printAllIsoPoz4
test : unittest

ifeq ($(CFLAGS),-ggdb)
OPTIMIZATION_OPT =
else 
OPTIMIZATION_OPT = -O2
endif

ifdef COVERAGE
OPTIMIZATION_OPT =
endif

ifdef STANLEY
FILTER=filter_stanley
else
FILTER=filter
endif

WARNINGS = -Wall -Wextra

printAllIsoPoz4 : int2table.o IsPoset.o main.o symmetries4.o $(FILTER).o
	cc $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) $^ -o $@ $(COVERAGE)
unittest : symmetries4.c
	cc -D$(TEST) $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) $^ -o $@ $(COVERAGE)
int2table.o : int2table.c headers.h
	cc $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) -c $< $(COVERAGE)
IsPoset.o : IsPoset.c headers.h
	cc $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) -c $< $(COVERAGE)
main.o : main.c headers.h
	cc $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) -c $< $(COVERAGE)
symmetries4.o : symmetries4.c headers.h
	cc $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) -c $< $(COVERAGE)
$(FILTER).o : $(FILTER).c headers.h
	cc $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) -c $< $(COVERAGE)

clean :
	rm -f $(OBJECTS)
