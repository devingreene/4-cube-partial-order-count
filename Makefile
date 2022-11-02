TEST = TESTING_b534ff4f

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

WARNINGS = -Wall -Wextra

printAllIsoPoz4 : int2table.c IsPoset.c main.c symmetries4.c filter.c
	cc $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) $^ -o $@ $(COVERAGE)
stanleycount : int2table.c IsPoset.c main.c symmetries4.c filter_stanley.c
	cc $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) $^ -o $@ $(COVERAGE)
unittest : symmetries4.c
	cc -D$(TEST) $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) $^ -o $@ $(COVERAGE)

clean:
	rm -f printAllIsoPoz4 stanleycount unittest
