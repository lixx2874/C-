//
//  ImageFacade.h
//  
//
//  Created by Xinyuan Liu
// 
//
//  This tool simulates the facade of loading and saving the image.
// may have pngfacade and jpg facade as subclasses

#ifndef IMAGEFACADE_H
#define IMAGEFACADE_H

#include "FlashPhotoApp.h"
#include "ColorData.h"
#include "PixelBuffer.h"
#include <string>
#include <png.h>
#include <zlib.h>


class ImageFacade {

friend class FlashPhotoApp;

public:
	ImageFacade();
	virtual ~ImageFacade();
	//virtual void load(const char * filename);
	PixelBuffer* load();
	PixelBuffer* getPixelBuffer() ;


protected:
	std::string m_filename;
	int new_width;
	int new_height;
	PixelBuffer* new_pixel_buffer;
	
};

#endif
