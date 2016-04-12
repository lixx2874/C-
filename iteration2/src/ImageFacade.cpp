//ImageFacade.cpp
//Created by Xinyuan Liu

#include "PngFacade.h"
#include "JpegFacade.h"
#include "FlashPhotoApp.h"
#include "BaseGfxApp.h"
#include "PixelBuffer.h"
#include <iostream>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <png.h>
#include <zlib.h>
//#include "png.h"

ImageFacade::ImageFacade(){
	
}

ImageFacade::~ImageFacade(){
	/*if (buffer == NULL)
        png_free_image(&image);

    else
        free(buffer);
	*/
}

PixelBuffer* ImageFacade::load(){
	return new_pixel_buffer;
}


PixelBuffer* ImageFacade::getPixelBuffer() {
	return new_pixel_buffer;
}




