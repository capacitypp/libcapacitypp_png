#include <iostream>
#include <png.h>

#include "Png.hpp"

using namespace std;

Png::Png(unsigned width, unsigned height)
{
	this->width = width;
	this->height = height;
	bitDepth = 8;
	colorType = PNG_COLOR_TYPE_RGB;
	filterMethod = PNG_FILTER_TYPE_BASE;
	compressionType = PNG_COMPRESSION_TYPE_BASE;
	interlaceType = PNG_INTERLACE_NONE;
	channels = 3;
	rowbytes = width * channels;
	rowPointers = new unsigned char*[height];
	for (unsigned i = 0; i < height; i++) {
		rowPointers[i] = new unsigned char[width * 4];
		for (unsigned j = 0; j < width; j++)
		for (unsigned k = 0; k < 4; k++)
			rowPointers[i][j * 4 + k] = 0xff;
	}
}

Png::Png(const string& fpath)
{
	read(fpath);
}

Png::~Png()
{
	for (unsigned i = 0; i < height; i++)
		delete[] rowPointers[i];
	delete[] rowPointers;
}

void Png::read(const string& fpath)
{
	FILE* fp = fopen(fpath.c_str(), "rb");
	if (!fp)
		throw CannotOpenFileException();

	png_byte sig[8];
	png_size_t num_to_check = sizeof(sig) / sizeof(sig[0]);
	fread(sig, 1, num_to_check, fp);
	if (!png_check_sig(sig, num_to_check))
		throw NotPngException();

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
		throw CannotCreateReadStructException();
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		throw CannotCreateInfoStructException();
	}
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		throw CannotCreateInfoStructException();
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(fp);
		throw CannotSetjmpException();
	}

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, num_to_check);

	png_read_info(png_ptr, info_ptr);

	png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
	this->width = (unsigned)width;
	png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
	this->height = (unsigned)height;
	png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	this->bitDepth = (unsigned short)bit_depth;
	switch (bit_depth) {
	case 8:
		break;
	default:
		cout << "bit_depth : " << (int)bit_depth << endl;
		throw UnsupportedBitDepthException();
	}
	png_byte color_type = png_get_color_type(png_ptr, info_ptr);
	this->colorType = (unsigned short)color_type;
	switch (color_type) {
	case PNG_COLOR_TYPE_RGB:
		break;
	default:
		cout << "color_type : " << (int)color_type << endl;
		throw UnsupportedColorTypeException();
	}
	png_byte filter_method = png_get_filter_type(png_ptr, info_ptr);
	this->filterMethod = (unsigned short)filter_method;
	switch (filter_method) {
	case PNG_FILTER_TYPE_BASE:
		break;
	default:
		cout << "filter_method : " << (int)filter_method << endl;
		throw UnsupportedFilterMethodException();
	}
	png_byte compression_type = png_get_compression_type(png_ptr, info_ptr);
	this->compressionType = (unsigned short)compression_type;
	switch (compression_type) {
	case PNG_COMPRESSION_TYPE_BASE:
		break;
	default:
		cout << "compression_type : " << (int)compression_type << endl;
		throw UnsupportedCompressionTypeException();
	}
	png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);
	this->interlaceType = (unsigned short)interlace_type;
	switch (interlace_type) {
	case PNG_INTERLACE_NONE:
		break;
	default:
		cout << "interlace_type : " << (int)interlace_type << endl;
		throw UnsupportedInterlaceTypeException();
	}
	png_byte channels = png_get_channels(png_ptr, info_ptr);


	this->channels = (unsigned short)channels;
	switch (channels) {
	case 3:
		break;
	default:
		cout << "channels : " << (int)channels << endl;
		throw UnsupportedChannelsException();
	}
	png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	this->rowbytes = (unsigned)rowbytes;

	png_bytepp row_pointers = new png_bytep[height];
	for (png_uint_32 i = 0; i < height; i++)
		row_pointers[i] = new png_byte[rowbytes];

	png_read_image(png_ptr, row_pointers);

	this->rowPointers = new unsigned char*[height];
	for (png_uint_32 i = 0; i < height; i++) {
		this->rowPointers[i] = new unsigned char[width * 4];
		for (unsigned j = 0; j < this->width; j++) { 
			for (unsigned k = 0; k < 3; k++)
				this->rowPointers[i][j * 4 + k] = (unsigned)row_pointers[i][j * 3 + k];
			this->rowPointers[i][j * 4 + 3] = 0xff;
		}
	}

	for (png_uint_32 i = 0; i < height; i++)
		delete[] row_pointers[i];
	delete[] row_pointers;

	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);

	fclose(fp);
}

void Png::write(const string& fpath) const
{
	FILE* fp = fopen(fpath.c_str(), "wb");
	if (!fp)
		throw CannotOpenFileException();

	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
		throw CannotCreateWriteStructException();
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		throw CannotCreateInfoStructException();
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		throw CannotSetjmpException();
	}

	png_init_io(png_ptr, fp);

	switch (bitDepth) {
	case 8:
		break;
	default:
		cout << "bitDepth : " << bitDepth << endl;
		throw UnsupportedBitDepthException();
	}

	switch (colorType) {
	case PNG_COLOR_TYPE_RGB:
		break;
	default:
		cout << "colorType : " << colorType << endl;
		throw UnsupportedColorTypeException();
	}

	switch (interlaceType) {
	case PNG_INTERLACE_NONE:
		break;
	default:
		cout << "interlaceType : " << interlaceType << endl;
		throw UnsupportedInterlaceTypeException();
	}

	switch (compressionType) {
	case PNG_COMPRESSION_TYPE_BASE:
		break;
	default:
		cout << "compressionType : " << compressionType << endl;
		throw UnsupportedCompressionTypeException();
	}

	switch (filterMethod) {
	case PNG_FILTER_TYPE_BASE:
		break;
	default:
		cout << "filterMethod : " << filterMethod << endl;
		throw UnsupportedFilterMethodException();
	}

	png_set_IHDR(png_ptr, info_ptr, (png_uint_32)width, (png_uint_32)height, (int)bitDepth, (int)colorType, (int)interlaceType, (int)compressionType, (int)filterMethod);

	png_write_info(png_ptr, info_ptr);

	png_bytepp row_pointers = new png_bytep[height];
	for (unsigned i = 0; i < height; i++) {
		row_pointers[i] = new png_byte[width * 3];
		for (unsigned j = 0; j < width; j++)
		for (unsigned k = 0; k < 3; k++)
			row_pointers[i][j * 3 + k] = rowPointers[i][j * 4 + k];
	}
	png_write_image(png_ptr, row_pointers);

	for (unsigned i = 0; i < height; i++)
		delete[] row_pointers[i];
	delete[] row_pointers;

	fclose(fp);
	png_destroy_write_struct(&png_ptr, &info_ptr);
}

unsigned Png::getWidth(void) const
{
	return width;
}

unsigned Png::getHeight(void) const
{
	return height;
}

unsigned short Png::getBitDepth(void) const
{
	return bitDepth;
}

unsigned short Png::getColorType(void) const
{
	return colorType;
}

unsigned short Png::getFilterMethod(void) const
{
	return filterMethod;
}

unsigned short Png::getCompressionType(void) const
{
	return compressionType;
}

unsigned short Png::getInterlaceType(void) const
{
	return interlaceType;
}

unsigned short Png::getChannels(void) const
{
	return channels;
}

unsigned Png::getRowbytes(void) const
{
	return rowbytes;
}

unsigned char** Png::getRowPointers(void)
{
	return rowPointers;
}

unsigned char* Png::operator[](int idx)
{
	return rowPointers[idx];
}

