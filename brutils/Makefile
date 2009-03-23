CFLAGS = -O3 -Wall
OBJS_BRP = brp.o
OBJS_BRM = brm.o
HEADERS = brutils.h
LIBS =
TARGET_BRP = brp
TARGET_BRM = brm
BINDIR=/usr/local/bin

all:	$(TARGET_BRP) $(TARGET_BRM)

$(TARGET_BRP):	$(OBJS_BRP) $(HEADERS)
	$(CC) -o $(TARGET_BRP) $(OBJS_BRP) $(LIBS)

$(TARGET_BRM):	$(OBJS_BRM) $(HEADERS)
	$(CC) -o $(TARGET_BRM) $(OBJS_BRM) $(LIBS)

clean:
	rm -f $(OBJS_BRP) $(OBJS_BRM) $(TARGET_BRP) $(TARGET_BRM)

install: all
	install -c brp $(BINDIR)
	install -c brm $(BINDIR)

