CCCFLAGS = -O3 -Wall
OBJS = brp.o
LIBS =
TARGET = brp
BINDIR=/usr/local/bin

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

install: all
	install -c brp $(BINDIR)

