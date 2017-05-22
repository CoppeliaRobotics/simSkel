BOOST_DIR ?= $(shell find /usr/local/Cellar/boost -mindepth 1 -maxdepth 1 -type d | sort | tail -n1)
BOOST_CFLAGS = -I$(BOOST_DIR)/include
BOOST_LDLIBS = -L$(BOOST_DIR)/lib -lboost_system

# for when $PWD is a symlink:
PARENT_DIR = $(shell sh -c 'cd $$PWD/..; pwd')

CXXFLAGS = -ggdb -O0 -I$(PARENT_DIR)/include -Iexternal -Wall -Wno-unused -Wno-overloaded-virtual -Wno-sign-compare -fPIC $(BOOST_CFLAGS)
LDLIBS = -ggdb -O0 -lpthread -ldl $(BOOST_LDLIBS)

.PHONY: clean all install doc

OS = $(shell uname -s)
ifeq ($(OS), Linux)
	CFLAGS += -D__linux -DLIN_VREP
	EXT = so
	INSTALL_DIR ?= $(PARENT_DIR)/..
else
	CFLAGS += -D__APPLE__ -DMAC_VREP
	EXT = dylib
	INSTALL_DIR ?= $(PARENT_DIR)/../vrep.app/Contents/MacOS/
endif

debug: all

release: all

all: libv_repExtPluginSkeletonNG.$(EXT) doc

doc: reference.html

reference.html: external/v_repStubsGen/xsl/reference.xsl callbacks.xml
	xsltproc --path "$(PWD)" -o $@ $^

v_repExtPluginSkeletonNG.o: stubs.h

stubs.o: stubs.h stubs.cpp

external/v_repPlusPlus/Plugin.o: external/v_repPlusPlus/Plugin.h external/v_repPlusPlus/Plugin.cpp

stubs.h: callbacks.xml
	python external/v_repStubsGen/main.py -H $@ $<

stubs.cpp: callbacks.xml
	python external/v_repStubsGen/main.py -C $@ $<

libv_repExtPluginSkeletonNG.$(EXT): v_repExtPluginSkeletonNG.o external/v_repPlusPlus/Plugin.o stubs.o $(PARENT_DIR)/common/v_repLib.o
	$(CXX) $^ $(LDLIBS) -shared -o $@

clean:
	rm -f libv_repExtPluginSkeletonNG.$(EXT)
	rm -f *.o
	rm -f external/v_repPlusPlus/Plugin.o
	rm -f stubs.cpp stubs.h
	rm -f reference.html

install: all
	cp libv_repExtPluginSkeletonNG.$(EXT) $(INSTALL_DIR)
