OBJS=Png.o
BUILDPATH=./build
OBJPATHS=$(addprefix $(BUILDPATH)/, $(OBJS))
INCLUDE=-I/usr/local/libpng/libpng-1.6.24/include
CPPFLAGS=$(INCLUDE) -Wall -O2 -std=c++14 -MMD
CXX=ccache clang++
TARGET=libcapacitypp_png.a

all: makefolder $(OBJPATHS)
	ar rs $(TARGET) $(OBJPATHS)

$(BUILDPATH)/%.o : %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) -o $@ -c $<

makefolder:
	@mkdir -p $(BUILDPATH)

clean:
	$(RM) $(TARGET)
	$(RM) -r -f $(BUILDPATH)

-include $(BUILDPATH)/*.d

