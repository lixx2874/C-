#include "PixelBuffer.h"
#include "ColorData.h"
#include "ColorfulPen.h"
#include <stdlib.h>  
#include  <cmath>
ColorfulPen::ColorfulPen(float r, float g, float b){
	red = r;
	green = g;
	blue = b;


    m_mask = new float *[9];
    for (int i = 0; i < 9; i++){
		// Construct 2D array
        m_mask[i]= new float [9];
        for (int j = 0; j<9; j++){
            m_mask[i][j] = 1;
        }
	}
}

// We can just set the initial color
void ColorfulPen::setColor(float r, float g, float b){
	red = r;
	green = g;
	blue = b;
}

void ColorfulPen::draw(PixelBuffer *background, int x, int y){
 
    red =((float) rand()) / (float) RAND_MAX;
    green =((float) rand()) / (float) RAND_MAX;
    blue =((float) rand()) / (float) RAND_MAX;
    for (int i=0; i<9; i++){
        for (int c = 0; c<9;  c++){
            const ColorData& color = ColorData(red*m_mask[i][c],green*m_mask[i][c],blue*m_mask[i][c]);
            background->setPixel(x-4+c,y-4+i,color);
		}
	}
}

ColorfulPen::~ColorfulPen(){
	if (m_mask){
        for(int i=0;i<9;i++){

           delete m_mask[i];
        }
        delete m_mask;
	}
}
