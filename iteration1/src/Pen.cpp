
#include "PixelBuffer.h"
#include "ColorData.h"
#include "Pen.h"
#include  <cmath>
Pen::Pen(float r, float g, float b){
    red=r;
    green=g;
    blue=b;//initialize the color

     m_mask = new float *[3]; //initialize the mask
    for(int i=0;i<3;i++){
        m_mask[i]=new float [3];
    }
    m_mask[0][1]=1.0;//set the shape cross
m_mask[1][0]=1.0;
m_mask[1][1]=1.0;
m_mask[1][2]=1.0;
m_mask[2][1]=1.0;
}

void Pen::setColor(float r, float g, float b){//change the color
    red=r;
    green=g;
    blue=b;
}

void Pen::draw(PixelBuffer *background, int x, int y){

    for(int i=0;i<3;i++){//draw on panel
        for(int c=0;c<3;c++){
            if(m_mask[i][c]!=0){
            const ColorData& color=ColorData(red,green,blue);
            background->setPixel(x-1+c,y-1+i,color);
             }
        }
    }

}
Pen::~Pen(){
    if(m_mask){
        for(int i=0;i<3;i++){

           delete m_mask[i];
        }
        delete m_mask;
    }
}
