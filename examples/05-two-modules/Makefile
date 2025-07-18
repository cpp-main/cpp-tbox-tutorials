TARGET:=demo

OBJECTS:= \
	apps.o \
	build.o \
	info.o

CXXFLAGS:=-I$(HOME)/.tbox/include -DLOG_MODULE_ID='"demo"' -std=c++11
LDFLAGS:=-L$(HOME)/.tbox/lib -rdynamic
LIBS:=\
	-ltbox_main \
	-ltbox_coroutine \
	-ltbox_trace \
	-ltbox_terminal \
	-ltbox_network \
	-ltbox_eventx \
	-ltbox_event \
	-ltbox_log \
	-ltbox_util \
	-ltbox_base \
	-lpthread -ldl

include my/app.mk
include your/app.mk

.PHONY: all clean distclean build.cpp

all: $(TARGET)

$(TARGET): $(OBJECTS)
	g++ -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f $(OBJECTS)

distclean: clean
	rm -f $(TARGET)
