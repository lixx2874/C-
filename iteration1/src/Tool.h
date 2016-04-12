#ifndef TOOL_H
#define TOOL_H
#include "PixelBuffer.h"
#include "ColorData.h"

class Tool{
public:
Tool();
Tool(float r,float g,float b);
virtual ~Tool();
virtual void setColor(float r,float g,float b);
virtual void draw(PixelBuffer* background, int x,int y);

protected:
float **m_mask;
float red;
float green;
float blue;

};


#endif
