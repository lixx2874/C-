//	JpegFacade.c
//	Yutong Li



#include "JpegFacade.h"
#include "FlashPhotoApp.h"
#include "BaseGfxApp.h"
#include "PixelBuffer.h"
#include <iostream>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "jpeglib.h"
#include <zlib.h>
#include <unistd.h>
#include <stdarg.h>

JpegFacade::JpegFacade(std::string filename){
	m_filename = filename;
	//memset(image, 0, sizeof(image));

}

JpegFacade::~JpegFacade(){

}

int JpegFacade::getWidth() const{
	return new_width;
}

int JpegFacade::getHeight() const{
	return new_height;
}

int JpegFacade::getColor_type() const{
	return color_type;
}

// functions that load the Jpg file
void JpegFacade::loadStamp(int x, int y, PixelBuffer* pixel_buffer){
	PixelBuffer* pb=load();
	for(int yy=0;yy<pb->getHeight();yy++){
		for(int xx=0;xx<pb->getWidth();xx++){
				 if(((x+xx-pb->getWidth()/2)<pixel_buffer->getWidth())&&((yy+y-pb->getHeight()/2)>=0)&&((x+xx-pb->getWidth()/2)>=0)&&((y+yy-pb->getHeight()/2)<pixel_buffer->getHeight())){
					 pixel_buffer->setPixel(x+xx-pb->getWidth()/2,yy+y-pb->getHeight()/2,pb->getPixel(xx,yy));
				 }
	}
	}
			// float r,g,b,a;
			// struct jpeg_decompress_struct cinfo;
			// struct jpeg_error_mgr jerr;
			// FILE* infile;
			// JSAMPROW row_pointer[1];
			// unsigned long location = 0;
			// int i = 0;
			//
			// if ((infile = fopen(m_filename.c_str(),"rb"))==NULL){
			// 	printf("can't read");
			// }
			// cinfo.err = jpeg_std_error(&jerr);
			// jpeg_create_decompress(&cinfo);
			// jpeg_stdio_src(&cinfo,infile);
			// jpeg_read_header(&cinfo, TRUE);
			// jpeg_start_decompress(&cinfo);
			//
			// new_width = cinfo.output_width;;
			// new_height = cinfo.output_height;
			// new_pixel_buffer=new PixelBuffer(new_width,new_height,ColorData(1,1,1));
			// int comp = cinfo.output_components;
			// unsigned char* raw_image = (unsigned char*)malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );
			// printf("length is: %d \n",cinfo.output_width*cinfo.output_height*cinfo.num_components);
			// row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );
			// while( cinfo.output_scanline < cinfo.image_height )
			// {
			// 		jpeg_read_scanlines( &cinfo, row_pointer, 1 );
			// 		for( i=0; i<cinfo.image_width*cinfo.num_components;i++)
			// 		raw_image[location++] = row_pointer[0][i];
			// }
			// jpeg_finish_decompress(&cinfo);
			// fclose(infile);
			// int origin_x = x-new_width/2;
			// int origin_y = y - new_height/2;
			// if (comp == 3){
			// 	for ( unsigned int j = origin_y ; j < origin_y + new_height ; j++ )
			// 	{
			// 		for ( unsigned int i = origin_x ; i < origin_x + new_width ; i++ )
			// 		{
			// 			int index = (new_height - j - 1) * new_width * comp + i * comp;
			// 			r = (float)raw_image[index]/255;
			// 			g = (float)raw_image[index+1]/255;
			// 			b = (float)raw_image[index+2]/255;
			// 			new_pixel_buffer->setPixel(i,j,ColorData(r,g,b));
			//
			// 		}
			// 	}
			// }
			// else
			// {
			// 	for ( unsigned int j = origin_y ; j <  origin_y +new_height ; j++ )
			// 	{
			// 		for ( unsigned int i = origin_x ; i <origin_x + new_width ; i++ )
			// 		{
			// 			int index = (new_height - j - 1) * new_width * comp + i * comp;
			// 			r = (float)raw_image[index]/255;
			// 			b = (float)raw_image[index+1]/255;
			// 			g = (float)raw_image[index+2]/255;
			// 			a = (float)raw_image[index+3]/255;
			// 				//printf("Color: %d %d %d \n",r,b,g);
			// 			new_pixel_buffer->setPixel(i,j,ColorData(r,g,b,a));
			// 		}
			// 	}
			//  }
			//  delete row_pointer[0];
			//  delete raw_image;
}
PixelBuffer* JpegFacade::load(){
      float r,g,b,a;
      struct jpeg_decompress_struct cinfo;
      struct jpeg_error_mgr jerr;
      FILE* infile;
      JSAMPROW row_pointer[1];
      unsigned long location = 0;
      int i = 0;

      if ((infile = fopen(m_filename.c_str(),"rb"))==NULL){
        printf("can't read");
      }
      cinfo.err = jpeg_std_error(&jerr);
      jpeg_create_decompress(&cinfo);
      jpeg_stdio_src(&cinfo,infile);
      jpeg_read_header(&cinfo, TRUE);
      jpeg_start_decompress(&cinfo);

      new_width = cinfo.output_width;;
      new_height = cinfo.output_height;
      new_pixel_buffer=new PixelBuffer(new_width,new_height,ColorData(1,1,1));

      int comp = cinfo.output_components;
      unsigned char* raw_image = (unsigned char*)malloc( cinfo.output_width*cinfo.output_height*cinfo.num_components );
      //printf("length is: %d \n",cinfo.output_width*cinfo.output_height*cinfo.num_components);
      row_pointer[0] = (unsigned char *)malloc( cinfo.output_width*cinfo.num_components );
      while( cinfo.output_scanline < cinfo.image_height )
      {
          jpeg_read_scanlines( &cinfo, row_pointer, 1 );
          for( i=0; i<cinfo.image_width*cinfo.num_components;i++)
          raw_image[location++] = row_pointer[0][i];
      }
      jpeg_finish_decompress(&cinfo);
      fclose(infile);
      if (comp == 3){
        for ( int y = 0 ; y < new_height ; y++ )
        {
          for ( int x = 0 ; x < new_width ; x++ )
          {
            int index = (new_height - y - 1) * new_width * comp + x * comp;
            r = (float)raw_image[index]/255;
            g = (float)raw_image[index+1]/255;
            b = (float)raw_image[index+2]/255;
            new_pixel_buffer->setPixel(x,y,ColorData(r,g,b));

          }
        }
      }
      else
      {
        for ( int y = 0 ; y < new_height ; y++ )
        {
          for ( int x = 0 ; x < new_width ; x++ )
          {
            int index = y * new_width * comp + x * comp;
            r = (float)raw_image[index]/255;
            b = (float)raw_image[index+1]/255;
            g = (float)raw_image[index+2]/255;
            a = (float)raw_image[index+3]/255;
              //printf("Color: %d %d %d \n",r,b,g);
            new_pixel_buffer->setPixel(x,new_height - y -1,ColorData(r,g,b,a));
          }
        }
       }
			 delete row_pointer[0];
			 delete raw_image;
	return new_pixel_buffer;
}

void JpegFacade::save(int width,int height, PixelBuffer* pixelbuffer){
	int r,g,b,a;
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE *outfile = fopen(m_filename.c_str(), "wb");
	JSAMPROW buffer[1];
	//int row_stride;
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);
	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, 100, TRUE);
	jpeg_start_compress(&cinfo, TRUE);
	//row_stride = cinfo.image_width * 3;
	//(unsigned char*)raw_image = (unsigned char*)malloc( cinfo.image_width*cinfo.image_height*cinfo.num_components );
	//number of total elements per row
	buffer[0] = (unsigned char *)malloc( cinfo.image_width*cinfo.num_components );
	int row_stride = cinfo.image_width*cinfo.input_components;
	while (cinfo.next_scanline < cinfo.image_height) {
		//buffer[0] = &raw_image[ cinfo.next_scanline * cinfo.image_width * cinfo.input_components];
		for (int i  = 0 ; i < row_stride; i=i+3){
				ColorData color = pixelbuffer->getPixel((int)i/3,cinfo.image_height-cinfo.next_scanline-1);
				r = color.getRed()*255;
				g = color.getGreen()*255;
				b = color.getBlue()*255;

				buffer[0][i] = r;
				buffer[0][i+1] = g;
				buffer[0][i+2] = b;

			}
		(void) jpeg_write_scanlines(&cinfo, buffer, 1);
	}
	//Don't need to motify, save memory
	jpeg_finish_compress(&cinfo);
	fclose(outfile);
	delete buffer[0];
	jpeg_destroy_compress(&cinfo);
}







// }
