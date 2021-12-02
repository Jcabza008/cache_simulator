CC = g++
INCLUDEFLAG = -I include
LIBSFLAG = -L lib
CFLAGS = -g -Wall -std=c++14 $(LIBSFLAG) $(INCLUDEFLAG)

CORELIBS = lib/scanner.o lib/cache.o lib/rep_policy.o

GTESTFLAGS = `pkg-config gtest gmock --cflags --libs`

CPPCOREGUIDELINES = 'cppcoreguidelines-avoid-goto,	 						\
				cppcoreguidelines-avoid-non-const-global-variables,	 		\
				cppcoreguidelines-init-variables,	                    	\
				cppcoreguidelines-interfaces-global-init,	            	\
				cppcoreguidelines-narrowing-conversions,                	\
				cppcoreguidelines-no-malloc,                            	\
				cppcoreguidelines-owning-memory,                        	\
				cppcoreguidelines-prefer-member-initializer,				\
				cppcoreguidelines-pro-bounds-constant-array-index,	 		\
				cppcoreguidelines-pro-type-const-cast,	  					\
				cppcoreguidelines-pro-type-cstyle-cast,						\
				cppcoreguidelines-pro-type-member-init,						\
				cppcoreguidelines-pro-type-static-cast-downcast,			\
				cppcoreguidelines-pro-type-vararg,	  						\
				cppcoreguidelines-slicing,	  								\
				cppcoreguidelines-c-copy-assignment-signature,				\
				cppcoreguidelines-explicit-virtual-functions,				\
				cppcoreguidelines-pro-type-union-access,  					\
				cppcoreguidelines-avoid-magic-numbers,						\
				cppcoreguidelines-pro-bounds-array-to-pointer-decay, 		\
				cppcoreguidelines-pro-type-reinterpret-cast,  				\
				cppcoreguidelines-macro-usage,								\
				cppcoreguidelines-non-private-member-variables-in-classes,  \
				cppcoreguidelines-pro-bounds-pointer-arithmetic'

# TODO Fix issues with this guidelines
DISABLEDCPPCOREGUIDELINES = 'cppcoreguidelines-avoid-c-arrays \
				cppcoreguidelines-special-member-functions'

run_simulator: simulator
	./bin/simulator

run_clang_tidy:
	clang-tidy -checks=$(CPPCOREGUIDELINES) --warnings-as-errors=* -header-filter=.* src/* -- $(CFLAGS)

run_tests: tests
	./bin/tests

all: pathfinder tests

clean:
	rm -rf bin lib

bin/:
	mkdir -p bin

lib/:
	mkdir -p lib

simulator: bin/ lib/main.o $(CORELIBS)
	$(CC) $(CFLAGS) -o bin/simulator lib/main.o $(CORELIBS) -lboost_program_options

tests: bin/ lib/tests.o $(CORELIBS)
	$(CC) $(CFLAGS) -o bin/tests lib/tests.o $(CORELIBS) $(GTESTFLAGS) -lpthread

# Main Libs
lib/main.o: lib/ $(LIBS)
	$(CC) $(CFLAGS) -c src/main.cpp -o lib/main.o $(SFMLFLAGS)

lib/tests.o: lib/ $(LIBS)
	$(CC) $(CFLAGS) -c tests/tests.cpp -lpthread -o lib/tests.o

# Libs
lib/scanner.o: lib/
	$(CC) $(CFLAGS) -c src/scanner.cpp -o lib/scanner.o

lib/cache.o: lib/
	$(CC) $(CFLAGS) -c src/cache.cpp -o lib/cache.o

lib/rep_policy.o: lib/
	$(CC) $(CFLAGS) -c src/rep_policy.cpp -o lib/rep_policy.o
