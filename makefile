#
# Makefile
#

.SUFFIXES: .yxx .lxx .d .cpp .c .g .o
.PHONY: clean all

CCC=g++
CC=gcc

CPPFLAGS	=  -std=c++11 -Wpointer-arith -O3 -pipe \
			-I. -I/usr/local/include/
CFLAGS 		= -static -static-libgcc -I. -Wpointer-arith -O3 -pipe
LFLAGS 		= -L/usr/lib -L/usr/local/lib
LIBS   		= -lsqlite3 -lboost_system -lboost_date_time -lcrypto
#LIBS   		= \
#			/usr/lib/x86_64-linux-gnu/libsqlite3.a \
#			/usr/lib/x86_64-linux-gnu/libboost_system.a \
#			/usr/lib/x86_64-linux-gnu/libboost_date_time.a \
#			/usr/lib/x86_64-linux-gnu/libpthread.a \
#			/usr/lib/x86_64-linux-gnu/libdl.a

CPP_Sources = \
	main.cpp \
	database.cpp

C_Sources = \
	simplelog.c

DEPPATH=.dep
OBJPATH=.obj

Objs  = $(addprefix $(OBJPATH)/, $(CPP_Sources:.cpp=.o))
Objs  += $(addprefix $(OBJPATH)/, $(C_Sources:.c=.o))
Deps  = $(addprefix $(DEPPATH)/, $(CPP_Sources:.cpp=.d))
Deps  += $(addprefix $(DEPPATH)/, $(C_Sources:.c=.d))

TARGET_EXE=genordxml

all: $(TARGET_EXE)

$(TARGET_EXE): $(Objs)
	$(CCC) $(CPPFLAGS) $(LFLAGS) -o $@ $(Objs) $(LIBS)

$(OBJPATH)/%.o: %.cpp
	if [ ! -d $(OBJPATH) ]; then mkdir -p $(OBJPATH); fi; \
	$(CCC) $(CPPFLAGS)  -c $< -o $@

$(OBJPATH)/%.o: %.c
	if [ ! -d $(OBJPATH) ]; then mkdir -p $(OBJPATH); fi; \
	$(CC) $(CFLAGS)  -c $< -o $@

$(DEPPATH)/%.d: %.cpp
	if [ ! -d $(DEPPATH) ]; then mkdir -p $(DEPPATH); fi; \
	set -e; rm -f $@; \
	$(CCC) -MM $(CPPFLAGS) $< >$@.$$$$ ;\
	sed 's,\($*\)\.o[ :]*,$(OBJPATH)/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

$(DEPPATH)/%.d: %.c
	if [ ! -d $(DEPPATH) ]; then mkdir -p $(DEPPATH); fi; \
	set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< >$@.$$$$ ;\
	sed 's,\($*\)\.o[ :]*,$(OBJPATH)/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

clean:
	rm -f $(Objs) $(TARGET_EXE)

-include $(Deps)
