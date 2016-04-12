//
//  PngFacade.h
//
//
//  Created by Xinyuan Liu
//
//
//  This tool simulates the facade of loading and saving the .png image.
//

#ifndef PNGFACADE_H
#define PNGFACADE_H

#include "ImageFacade.h"
#include "PixelBuffer.h"
#include <iostream>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <png.h>
#include <zlib.h>
#include <cmath>
#include <string>

class PngFacade : public ImageFacade
{
friend class Tool;
public:
	PngFacade(std::string filename);
	virtual ~PngFacade();
	void loadStamp(int x,int y,PixelBuffer* pixel_buffer);
	PixelBuffer* load();
	void save(int width,int height, PixelBuffer* pixelbuffer);
	//void load(png_image image,png_bytep buffer,const char *  filename,PixelBuffer* pixel_buffer);
	png_uint_32 getWidth() const;
	png_uint_32 getHeight() const;
	int getColor_type() const;
	int getBit_depth() const;
protected:
	std::string m_filename;
	png_structp png_ptr;
	png_infop info_ptr;
	png_uint_32 new_width;
	png_uint_32 new_height;
	int color_type;
	int bit_depth;
	//png_bytep *row_pointers;
	PixelBuffer* new_pixel_buffer;
// 	typedef struct {
//     		uint8_t red;
//     		uint8_t green;
//     		uint8_t blue;
// 	} pixel_t;
//
// /* A picture. */
//
// 	typedef struct  {
//     		pixel_t *pixels;
//     		size_t width;
//    		size_t height;
// 	} bitmap_t;
};

#endif
