#include "PixelBuffer.h"
#include "ColorData.h"
#include "Tool.h"
#include "CalligraphyPen.h"
#include  <cmath>
CalligraphyPen::CalligraphyPen(float r, float g, float b){
  red = r;
  green = g;
  blue = b;

  m_mask = new float *[15];
  for (int i = 0; i < 15; i++){
    m_mask[i] = new float [5];
    for (int j = 0; j < 5; j++){
      m_mask[i][j]=1;
  }
}
}

void CalligraphyPen::setColor(float r, float g, float b){
    red = r;
    green = g;
    blue = b;
}
void CalligraphyPen::draw(PixelBuffer *background, int x, int y){

    for (int i = 0;  i < 15; i++){
      for (int j = 0; j < 5; j++){
        const ColorData& color = ColorData(red,green,blue);
        background->setPixel(x-2+j,y-7+i,color);
        }
    }
}

CalligraphyPen::~CalligraphyPen(){
  if (m_mask){
      for(int i=0;i<15;i++){

         delete m_mask[i];
      }
      delete m_mask;
  }
}
