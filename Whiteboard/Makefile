#------------------------------------------
#Makefile for Project Whiteboard
#Allan Pendergrast
#------------------------------------------

SRCS = chat.cpp $(wildcard RakNet/*.cpp)
OBJS = $(SRCS:.cpp=.o)
EXE	 = projwb
CC   = g++ -Wall

#------------------------------------------
 
.PHONY: all depend clean

all: $(EXE)

depend:
	g++ -E -M $(SRCS) > Makefile.depend
 
clean:
	rm -f $(EXE) chat.o RakNet/*.o

#------------------------------------------

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $@
 
.cpp.o:
	$(CC) $< -c -o $@

#------------------------------------------

-include Makefile.depend
