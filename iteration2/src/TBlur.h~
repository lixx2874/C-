#ifndef TBLUR_H
#define TBLUR_H

#include "Tool.h"
#include "PixelBuffer.h"
class TBlur : public Tool 
{
public:
	TBlur();
	void applyToBuffer(int toolX, int toolY, ColorData toolColor, PixelBuffer* buffer);
        PixelBuffer* applyBlur(int toolX, int toolY, ColorData toolColor, PixelBuffer* buffer);
	std::string getName();
};

#endif
