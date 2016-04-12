#ifndef JPGFACADE_H
#define JPGFACADE_H

#include "ImageFacade.h"
#include "jpeglib.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "jpeglib.h"
#include <zlib.h>
#include <cmath>
#include <string>


class JpegFacade : public ImageFacade
{
public:
	JpegFacade(std::string filename);
	virtual ~JpegFacade();
	PixelBuffer* load();
	void save(int width,int height, PixelBuffer* pixelbuffer);
	void load(int x, int y, PixelBuffer* pixelbuffer);
	void loadStamp(int x,int y,PixelBuffer* pixel_buffer);
	int getWidth() const;
	int getHeight() const;
	int getColor_type() const;
	//int load(jpg_image image, jpg_bytep buffer, const char * filename, PixelBuffer* pixel_buffer);

protected:
	std::string m_filename;
	int new_width;
	int new_height;
	int color_type;
	PixelBuffer* new_pixel_buffer;
	//jpg_image image;
	//jpg_bytep buffer;
};

#endif
