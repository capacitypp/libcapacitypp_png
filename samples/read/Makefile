OBJS=read.o
BUILDPATH=./build
OBJPATHS=$(addprefix $(BUILDPATH)/, $(OBJS))
INCLUDE=-I../../
CPPFLAGS=$(INCLUDE) -Wall -O2 -std=c++14 -MMD
LDFLAGS=-L../../
LIBS=-lcapacitypp_png /usr/local/libpng/libpng-1.6.24/lib/libpng.a /usr/local/zlib/zlib-1.2.8/lib/libz.a
CXX=ccache clang++
TARGET=read

all: makefolder $(SUBOBJS) $(OBJPATHS)
	$(CXX) -o $(TARGET) $(OBJPATHS) $(LDFLAGS) $(LIBS)

$(SUBDIRS)/%.a:
	$(MAKE) -C $(dir $@)

$(BUILDPATH)/%.o : %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -o $@ -c $<

makefolder:
	@mkdir -p $(BUILDPATH)

clean:
	$(RM) $(TARGET)
	$(RM) -r -f $(BUILDPATH)

-include $(BUILDPATH)/*.d

