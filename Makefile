INCS = msdscript.h catch.h Expr.h test.h

OBJS = main.o msdscript.o test.o Expr.o

CXXFLAGS = --std=c++14 -O2

msdscript: $(OBJS)
	$(CXX) $(CXXFLAGS) -o msdscript $(OBJS)

main.o: $(INCS)

msdscript.o: $(INCS)

test.o: $(INCS)

Expr.o: $(INCS)