#include "PixelBuffer.h"
#include "ColorData.h"
#include "Highlighter.h"
#include  <cmath>
Highlighter::Highlighter(float r, float g, float b){
    red = r;
    green = g;
    blue = b;


    m_mask = new float *[15];
    for (int i = 0; i < 15; i++){
		// Construct 2D array
        m_mask[i]= new float [5];
        for (int j =0; j< 5; j++){
            m_mask[i][j]=0.4;
        }
	}
}

void Highlighter::setColor(float r, float g, float b){

    red = r ;
    green = g ;
    blue = b ;
}

void Highlighter::draw(PixelBuffer *background, int x, int y){
 
    for (int i=0; i<15; i++){
        for (int c = 0; c < 5; c++){
            ColorData get = background->getPixel(x-2+c, y-7+i);
  //          float s=0.2126*red + 0.7152*green + 0.0722*blue;
//            float i=s*0.4;
            const ColorData& color = ColorData(red * m_mask[i][c]*get.getLuminance()+ (1-m_mask[i][c]*get.getLuminance())* get.getRed(),green * m_mask[i][c]*get.getLuminance() + (1-m_mask[i][c]*get.getLuminance()) * get.getGreen(), blue *m_mask[i][c]*get.getLuminance() + (1-m_mask[i][c]*get.getLuminance()) * get.getBlue());
            background->setPixel(x-2+c,y-7+i,color);
		}
	}
}

Highlighter::~Highlighter(){
	if (m_mask){
        for(int i=0;i<15;i++){

           delete m_mask[i];
        }
        delete m_mask;
	}
}

