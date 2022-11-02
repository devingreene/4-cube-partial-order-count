TEST = TESTING_b534ff4f

.PHONY : execs test
execs : printAllIsoPoz4 stanleycount
test : bin_groupops bin_posets

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
bin_groupops : symmetries4.c
	cc -D$(TEST) $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) $^ -o $@ $(COVERAGE)
bin_posets: IsPoset.c int2table.c
	cc -D$(TEST) $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) $^ -o $@ $(COVERAGE)

clean:
	rm -f printAllIsoPoz4 stanleycount bin_groupops bin_posets
