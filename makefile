CC	=	g++
CFLAGS	= 
INCLUDES = -I./src/include
LFLAGS	=	-g	-Wall
LIBS	=	
SRCS	=	$(wildcard src/*.cpp) main.cpp
OBJS	=	$(SRCS:.cpp=.o)
MAIN	=	vicar

.PHONY:	depend	clean

all:	$(MAIN)
		@echo	Program have been compiled succesfully

$(MAIN):	$(OBJS) 
		$(CC)	$(CFLAGS)	$(INCLUDES)	-o	$(MAIN)	$(OBJS)	$(LFLAGS)	$(LIBS)

%.o: %.cpp
		$(CC)	$(CFLAGS)	$(INCLUDES)	-c	$<	-o	$@

clean:
		rm -rf $(OBJS) $(MAIN)

depend: $(SRCS)
		makedepend	$(INCLUDES)	$^