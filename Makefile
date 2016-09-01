OBJS=png.o
BUILDPATH=./build
OBJPATHS=$(addprefix $(BUILDPATH)/, $(OBJS))
INCLUDE=-I../../
CPPFLAGS=$(INCLUDE) -Wall -O2 -std=c++14 -MMD
CXX=ccache clang++
TARGET=libcapacitypp_png.a

all: makefolder $(SUBOBJS) $(OBJPATHS)
	ar rs $(TARGET) $(OBJPATHS)

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

