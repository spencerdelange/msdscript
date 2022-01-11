INCS = cmdline.h

OBJS = main.o cmdline.o

CXXFLAGS = --std=c++14 -O2

cmdline: $(OBJS)
	$(CXX) $(CXXFLAGS) -o cmdline $(OBJS)

main.o: $(INCS)

cmdline.o: $(INCS)