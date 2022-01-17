INCS = msdscript.h Add.h catch.h expr.h Mult.h Num.h test.h Var.h

OBJS = main.o msdscript.o Add.o Mult.o Num.o test.o Var.o

CXXFLAGS = --std=c++14 -O2

msdscript: $(OBJS)
	$(CXX) $(CXXFLAGS) -o msdscript $(OBJS)

main.o: $(INCS)

msdscript.o: $(INCS)

Add.o: $(INCS)

Mult.o: $(INCS)

Num.o: $(INCS)

test.o: $(INCS)

Var.o: $(INCS)