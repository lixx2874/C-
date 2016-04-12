#ifndef SPRAYCAN_H
#define SPRAYCAN_H
#include "PixelBuffer.h"
#include "ColorData.h"
#include "Tool.h"
class SprayCan: public Tool
{
public:
SprayCan(float r,float g,float b);
virtual ~SprayCan();
void setColor(float r,float g,float b);
void draw(PixelBuffer* background, int x,int y);

};


#endif
