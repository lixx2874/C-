#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include "PixelBuffer.h"
#include "ColorData.h"
#include "Tool.h"

class Highlighter: public Tool{
public:
Highlighter(float r,float g,float b);
virtual ~Highlighter();
void setColor(float r,float g,float b);
void draw(PixelBuffer* background, int x,int y);

};



#endif
