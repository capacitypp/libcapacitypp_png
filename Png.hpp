#ifndef ___Class_Png
#define ___Class_Png

#include <string>

class Png {
public:
	class CannotOpenFileException{};
	class NotPngException{};
	class CannotCreateReadStructException{};
	class CannotCreateInfoStructException{};
	class CannotSetjmpException{};
	class UnsupportedBitDepthException{};
	class UnsupportedColorTypeException{};
	class UnsupportedFilterMethodException{};
	class UnsupportedCompressionTypeException{};
	class UnsupportedInterlaceTypeException{};
	class UnsupportedChannelsException{};

private:
	unsigned width;
	unsigned height;
	unsigned short bitDepth;
	unsigned short colorType;
	unsigned short filterMethod;
	unsigned short compressionType;
	unsigned short interlaceType;
	unsigned short channels;
	unsigned rowbytes;
	unsigned char** rowPointers;

public:
	Png(const std::string& fpath);

	void read(const std::string& fpath);

	unsigned getWidth(void) const;
	unsigned getHeight(void) const;
	unsigned short getBitDepth(void) const;
	unsigned short getColorType(void) const;
	unsigned short getFilterMethod(void) const;
	unsigned short getCompressionType(void) const;
	unsigned short getInterlaceType(void) const;
	unsigned short getChannels(void) const;
	unsigned getRowbytes(void) const;
	unsigned char** getRowPointers(void) const;
};

#endif

