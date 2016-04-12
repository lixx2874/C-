
#include "PixelBuffer.h"
#include "ColorData.h"
#include "Tool.h"
Tool::Tool(){

}

Tool::Tool(float r, float g, float b){
  /*  red=r;
    green=g;
    blue=b;

     m_mask = new float *[5];
    for(int i=0;i<5;i++){
        m_mask[i]=new float [5];
        for(int c=0;c<5;c++){
            m_mask[i][c]=1.0;
        }
    }*/
}
void Tool::setColor(float r, float g, float b){
/*    red=r;
    green=g;
    blue=b;*/
}

void Tool::draw(PixelBuffer *background, int x, int y){
    /*for(int i=0;i<5;i++){
        for(int c=0;c<5;c++){
            const ColorData& color=ColorData(red,green,blue);
            background->setPixel(x-2+c,y-2+i,color);

        }
    }*/
}

Tool::~Tool(){
    if(m_mask){
        delete m_mask;
    }
}
