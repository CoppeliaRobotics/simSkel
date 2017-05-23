BOOST_DIR ?= $(shell find /usr/local/Cellar/boost -mindepth 1 -maxdepth 1 -type d | sort | tail -n1)
BOOST_CFLAGS = -I$(BOOST_DIR)/include
BOOST_LDLIBS = -L$(BOOST_DIR)/lib -lboost_system

# for when $PWD is a symlink:
PARENT_DIR = $(shell sh -c 'cd $$PWD/..; pwd')

CXXFLAGS = -ggdb -O0 -I$(PARENT_DIR)/include -Igenerated -Iexternal -Wall -Wno-unused -Wno-overloaded-virtual -Wno-sign-compare -fPIC $(BOOST_CFLAGS)
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

all: libv_repExtPluginSkeletonNG.$(EXT)

v_repExtPluginSkeletonNG.o: generated/stubs.h

generated/stubs.o: generated/stubs.h generated/stubs.cpp

external/v_repPlusPlus/Plugin.o: external/v_repPlusPlus/Plugin.h external/v_repPlusPlus/Plugin.cpp

generated/stubs.h generated/stubs.cpp generated/reference.html: callbacks.xml
	python external/v_repStubsGen/generate.py --xml-file callbacks.xml --gen-all "$(PWD)/generated/"

libv_repExtPluginSkeletonNG.$(EXT): v_repExtPluginSkeletonNG.o external/v_repPlusPlus/Plugin.o generated/stubs.o $(PARENT_DIR)/common/v_repLib.o
	$(CXX) $^ $(LDLIBS) -shared -o $@

clean:
	rm -f libv_repExtPluginSkeletonNG.$(EXT)
	rm -f *.o
	rm -f external/v_repPlusPlus/Plugin.o
	rm -rf generated

install: all
	cp libv_repExtPluginSkeletonNG.$(EXT) $(INSTALL_DIR)
