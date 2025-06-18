TARGET:=coffee-machine

OBJECTS:= \
	apps.o \
	build.o \
	info.o

CXXFLAGS:=-I$(HOME)/.tbox/include -DLOG_MODULE_ID='"coffee"' -std=c++11 -ggdb
LDFLAGS:=-L$(HOME)/.tbox/lib -rdynamic

include coffee/app.mk

LIBS +=\
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

.PHONY: all clean distclean build.cpp

all: $(TARGET)

$(TARGET): $(OBJECTS)
	g++ -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f $(OBJECTS)

distclean: clean
	rm -f $(TARGET)
