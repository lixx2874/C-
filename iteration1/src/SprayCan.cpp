
#include "PixelBuffer.h"
#include "ColorData.h"
#include "SprayCan.h"
#include  <cmath>
SprayCan::SprayCan(float r, float g, float b){
    red=r;
    green=g;
    blue=b;//initialize the color

     m_mask = new float *[41];//initialize the mask
    for(int i=0;i<41;i++){
        m_mask[i]=new float [41];
    for(int c=0;c<41;c++){//set the shape circle
          float d=1-0.05*(int)(sqrt((20-i)*(20-i)+(20-c)*(20-c)));
          if(d<0){
              d=0;
          }
          m_mask[i][c]=d;
	}
    }

}

void SprayCan::setColor(float r, float g, float b){//change the color
    red=r;
    green=g;
    blue=b;
}

void SprayCan::draw(PixelBuffer *background, int x, int y){
  
    for(int i=0;i<41;i++){//draw on panel
        for(int c=0;c<41;c++){
            if(m_mask[i][c]>0){
      ColorData color=background->getPixel(x-20+c,y-20+i);
      float influ=m_mask[i][c]*0.2;
      const ColorData& colo=ColorData(influ*red+(1-influ)*color.getRed(),influ*green+(1-influ)*color.getGreen(),influ*blue+(1-influ)*color.getBlue());
            background->setPixel(x-20+c,y-20+i,colo);
}
        }
    }
}
SprayCan::~SprayCan(){
    if(m_mask){
        for(int i=0;i<41;i++){

           delete m_mask[i];
        }
        delete m_mask;
    }
}
