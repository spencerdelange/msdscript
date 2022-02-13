INCS = msdscript.h catch.h Expr.h test.h

OBJS = msdscript.o test.o Expr.o

CXXFLAGS = --std=c++14 -O2

msdscript: $(OBJS)
	$(CXX) $(CXXFLAGS) -o msdscript $(OBJS)

.PHONY: test
test: msdscript
	./msdscript --test

msdscript.o: $(INCS)

test.o: $(INCS)

Expr.o: $(INCS)

# Remove output files and executables

clean:
	rm -f *~ *.o msdscript test_msdscript test_msdscript_project/test_msdscript.o

# test_msdscript makefile

test_msdscript: test_msdscript_project/test_msdscript.o test_msdscript_project/exec.o
	$(CXX) $(CXXFLAGS) -o test_msdscript test_msdscript_project/test_msdscript.o test_msdscript_project/exec.o

test_msdscript_project/test_msdscript.o: test_msdscript_project/test_msdscript.h

test_msdscript_project/exec.o: test_msdscript_project/exec.h

