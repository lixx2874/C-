#include "PixelBuffer.h"
#include "Tool.h"
#include "Erasure.h"
#include <cmath>
#include  <cmath>
Erasure::Erasure(float r, float g, float b){
  red=r;
  green = g;
  blue = b;

  m_mask = new float *[21];
  for (int i = 0; i < 21; i++){
    m_mask[i] = new float [21];
    for (int j = 0; j <21; j++){
        float diameter = (int) sqrt(((10-i)*(10-i)+(10-j)*(10-j)));
        if (diameter<=10){
            m_mask [i][j] = 1;
        }
    }
  }
}

void Erasure::setColor(float r, float g, float b){
   
}
void Erasure::draw(PixelBuffer *background, int x, int y){
 
    for(int i = 0;i < 21; i++){
        for(int j = 0;j < 21; j++){
	  if(m_mask[i][j]==1){
            const ColorData& color = ColorData(red,green,blue);
            background->setPixel(x-10+j,y-10+i,color);
	  }
        }
    }
}
Erasure::~Erasure(){
  if (m_mask){
      for(int i=0;i<21;i++){

         delete m_mask[i];
      }
      delete m_mask;
  }
}
