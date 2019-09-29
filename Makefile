CXX=g++
CXXFLAGS='-std=c++11'
LDFLAGS=
LDLIBS=

SRCS=nyansa1.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: nyansa1 

nyansa1 : $(OBJS)
	$(CXX) -o nyansa1 $(OBJS) $(LDLIBS)
