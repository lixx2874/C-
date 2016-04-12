#ifndef PEN_H
#define PEN_H
#include "PixelBuffer.h"
#include "ColorData.h"
#include "Tool.h"
class Pen: public Tool
{
public:
Pen(float r,float g,float b);
virtual ~Pen();
void setColor(float r,float g,float b);
void draw(PixelBuffer* background, int x,int y);



};


#endif
