#include "TBlur.h"
#include "MLinear.h"
#include "ColorData.h"
#include "PixelBuffer.h"
#include <iostream>

using std::cout;
using std::endl;
TBlur::TBlur() 
{
    m_mask = new MLinear(9, 1);
	
}


std::string TBlur::getName() 
{
	return "Blur";
}
PixelBuffer* TBlur::applyBlur(int toolX, int toolY, ColorData toolColor, PixelBuffer* buffer){
    PixelBuffer *n=new PixelBuffer(buffer->getWidth(),buffer->getHeight(),buffer->getBackgroundColor());
    PixelBuffer::copyPixelBuffer(buffer,n);
    for(int y=0;y<19;y++){
        for(int x=0;x<19;x++){
            float total=0;
              ColorData data=ColorData(0,0,0);
                  int r=(int)(m_mask->getValue(x,y)*10);
            if(r>0){


                Mask *nm=new MLinear(r,1);
                for(int yy=-r;yy<r+1;yy++){
                    for(int xx=-r;xx<r+1;xx++){
                        data=data+buffer->getPixel(toolX-9+x+xx,toolY-9+yy+y)*nm->getValue(xx+r,yy+r);
                        total=total+nm->getValue(xx+r,yy+r);
                    }
                }
                data=data*(1.0f/total);

                n->setPixel(toolX-9+x,toolY-9+y,data);
            }

        }
    }
    return n;

}

void TBlur::applyToBuffer(int toolX, int toolY, ColorData toolColor, PixelBuffer* buffer){
    PixelBuffer *n=new PixelBuffer(buffer->getWidth(),buffer->getHeight(),buffer->getBackgroundColor());
    PixelBuffer::copyPixelBuffer(buffer,n);
    for(int y=0;y<19;y++){
        for(int x=0;x<19;x++){
            float total=0;
              ColorData data=ColorData(0,0,0);
                  int r=(int)(m_mask->getValue(x,y)*10);
            if(r>0){


                Mask *nm=new MLinear(r,1);
                for(int yy=-r;yy<r+1;yy++){
                    for(int xx=-r;xx<r+1;xx++){
                        data=data+buffer->getPixel(toolX-9+x+xx,toolY-9+yy+y)*nm->getValue(xx+r,yy+r);
                        total=total+nm->getValue(xx+r,yy+r);
                    }
                }
                data=data*(1.0f/total);

                n->setPixel(toolX-9+x,toolY-9+y,data);
            }

        }
    }
    ColorData a=ColorData(1,1,1);
    buffer=new PixelBuffer(buffer->getWidth(),buffer->getHeight(),a);
}
