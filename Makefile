TEST = TESTING_b534ff4f

.PHONY : execs test
execs : printAllIsoPoz4
test : bin_groupops bin_posets

ifeq ($(CFLAGS),-ggdb)
OPTIMIZATION_OPT =
else 
OPTIMIZATION_OPT = -O2
endif

ifdef COVERAGE
OPTIMIZATION_OPT =
endif

ifdef STANLEY
FILTER = filter_stanley
else
FILTER = filter
endif

WARNINGS = -Wall -Wextra

printAllIsoPoz4 : int2table.c IsPoset.c main.c symmetries4.c $(FILTER).c
	cc $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) $^ -o $@ $(COVERAGE)
bin_groupops : symmetries4.c
	cc -D$(TEST) $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) $^ -o $@ $(COVERAGE)
bin_posets: IsPoset.c int2table.c
	cc -D$(TEST) $(WARNINGS) $(CFLAGS) $(OPTIMIZATION_OPT) $^ -o $@ $(COVERAGE)

clean:
	rm -f printAllIsoPoz4 bin_groupops bin_posets
