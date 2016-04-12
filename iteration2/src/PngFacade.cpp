//PngFacade.cpp
//Created by Xinyuan Liu

#include "PngFacade.h"
#include "FlashPhotoApp.h"
#include "BaseGfxApp.h"
#include "PixelBuffer.h"
#include <iostream>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <png.h>
#include <zlib.h>
#include <unistd.h>
#include <stdarg.h>

PngFacade::PngFacade(std::string filename){
	//fp = fopen(filename.c_str(), "rb");
	m_filename=filename;

}

PngFacade::~PngFacade(){
	/*if (buffer == NULL)
        png_free_image(&image);

    else
        free(buffer);
	*/
}

png_uint_32 PngFacade::getWidth() const{
	return new_width;
}

png_uint_32 PngFacade::getHeight() const{
	return new_height;
}

int PngFacade::getColor_type() const{
	return color_type;
}

int PngFacade::getBit_depth() const{
	return bit_depth;
}
void PngFacade::loadStamp(int x,int y,PixelBuffer* pixel_buffer){
	PixelBuffer* pb=load();
	for(int yy=0;yy<pb->getHeight();yy++){
		for(int xx=0;xx<pb->getWidth();xx++){
		     if(((x+xx-pb->getWidth()/2)<pixel_buffer->getWidth())&&((yy+y-pb->getHeight()/2)>=0)&&((x+xx-pb->getWidth()/2)>=0)&&((y+yy-pb->getHeight()/2)<pixel_buffer->getHeight())){
					 pixel_buffer->setPixel(x+xx-pb->getWidth()/2,yy+y-pb->getHeight()/2,pb->getPixel(xx,yy));
				 }
	}
	}
	// FILE *fp= fopen(m_filename.c_str(), "rb");
	// if (!fp)  printf("File could not be opened for reading");
	// //if (!fp)  printf("File could not be opened for reading");
	// 	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
	// if(!png_ptr) abort();
	// 	info_ptr = png_create_info_struct(png_ptr);
	// 	if(!info_ptr) abort();
	// 	new_width=png_get_image_width(png_ptr,info_ptr);
	// 	new_height=png_get_image_height(png_ptr,info_ptr);
	// if (setjmp(png_jmpbuf(png_ptr))) abort();
	// color_type=png_get_color_type(png_ptr,info_ptr);
	// 			//std::cout<<"color_type"<<color_type;
	// 	bit_depth  = png_get_bit_depth(png_ptr,info_ptr);
	// 	png_init_io(png_ptr, fp);
	// 	png_set_sig_bytes(png_ptr,0);
	// 	png_read_info(png_ptr, info_ptr);
	// 	png_get_IHDR(png_ptr, info_ptr, &new_width, &new_height, &bit_depth, &color_type,NULL,NULL,NULL);
	// //png_bytep *row_pointers =new png_bytep[new_height];
	// //png_read_update_info(png_ptr, info_ptr);
	// //png_bytep *row_pointers= (png_bytep*)malloc(sizeof(png_bytep) * new_height);
	// png_read_update_info(png_ptr, info_ptr);
	// png_bytep *row_pointers=new png_bytep[new_height];
	// for(unsigned int row=0; row<new_height; row++) {
	// 	//row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr,info_ptr));
	// 	//row_pointers[row] =(png_byte*)png_malloc(png_ptr, png_get_rowbytes(png_ptr,info_ptr));
	// 		row_pointers[row] =new png_byte[png_get_rowbytes(png_ptr,info_ptr)];
	// 			}
	// //if (row_pointers == NULL) abort();
	// //if(bit_depth == 16)
	//
	// 					// png_set_strip_alpha(png_ptr);
	// 					// png_set_packing(png_ptr);
	// 					// png_set_packswap(png_ptr);
	// 					//
	// 					// if(color_type == PNG_COLOR_TYPE_PALETTE)
	// 					// png_set_palette_to_rgb(png_ptr);
	// 					//
	// 					// // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	// 					// if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
	// 					// png_set_expand_gray_1_2_4_to_8(png_ptr);
	// 					//
	// 					// if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	// 					// png_set_tRNS_to_alpha(png_ptr);
	// 					//
	// 					// if (bit_depth < 16)
	// 					// png_set_expand_16(png_ptr);
	// 					// if (bit_depth == 16)	png_set_scale_16(png_ptr);
	// 					// else	png_set_strip_16(png_ptr);
	// 					// if (color_type & PNG_COLOR_MASK_ALPHA)
	// 				// 	png_set_strip_alpha(png_ptr);
	//
	// 					// These color_type don't have an alpha channel then fill it with 0xff.
	// 					// if(color_type == PNG_COLOR_TYPE_RGB ||
	// 					// color_type == PNG_COLOR_TYPE_GRAY ||
	// 					// color_type == PNG_COLOR_TYPE_PALETTE)
	// 					// png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
	// 					//
	// 					// if(color_type == PNG_COLOR_TYPE_GRAY ||
	// 					//   color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	// 					// png_set_gray_to_rgb(png_ptr);
	//
	//
	//
	// 	png_read_image(png_ptr, row_pointers);
	// 	/* Read rest of file, and get additional chunks in info_ptr - REQUIRED */
	// 	  //png_read_end(png_ptr, info_ptr);
	// 	/* Clean up after the read, and free any memory allocated - REQUIRED */
	//
	//
	// 		int origin_x=x-new_width/2;
	// 		int origin_y=y-new_height/2;
	// 		//new_pixel_buffer=new PixelBuffer(new_width,new_height,ColorData(0.0,0.0,0.0,0.0));
	// 		if(color_type==PNG_COLOR_TYPE_RGB){
	// 	            for (unsigned int j=origin_y; j<origin_y+new_height; j++) {
	// 	              //png_byte* row = row_pointers[origin_y+new_height-j-1];
	// 								png_byte* row = row_pointers[origin_y+new_height-j-1];
	// 	              for (unsigned int i=origin_x; i<origin_x+new_width; i++) {
	// 	                      png_byte* ptr = &(row[i*3]);
	// 	                      /*printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
	// 	                             x, y, ptr[0], ptr[1], ptr[2]);*/
	// 	                      /* set pixel on the pixelbuffer*/
	//
	// 	                      pixel_buffer->setPixel(i,j,ColorData ((float)ptr[0]/255,(float)ptr[1]/255,(float)ptr[2]/255));
	// 	                }
	// 	            }
	// 	         }
	//
	// 	  else if(color_type==PNG_COLOR_TYPE_RGBA){
	// 	            for (unsigned int j=origin_y; j<origin_y+new_height; j++) {
	// 									png_byte* row = row_pointers[origin_y+new_height-j-1];
	// 	              for (unsigned int i=origin_x; i<origin_x+new_width; i++) {
	// 	                      png_byte* ptr = &(row[i*4]);
	// 	                      /*printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
	// 	                             x, y, ptr[0], ptr[1], ptr[2], ptr[3]);*/
	// 	                      /* set pixel on the pixelbuffer*/
	//
	// 	                      pixel_buffer->setPixel(i,j,ColorData ((float)ptr[0]/255,(float)ptr[1]/255,(float)ptr[2]/255,(float)ptr[3]/255));
	//
	// 	                }
	// 	            }
	//
	// 	            }
	// 	        else  std::cout<<"[process_file] input file must be PNG_COLOR_TYPE_RGBA or PNG_COLOR_TYPE_RGB"
	// 	                       "(lacks the alpha channel)";
	// 		delete [] row_pointers;
	// 		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	// 		fclose(fp);




}

PixelBuffer* PngFacade::load(){//using library code
	FILE *fp= fopen(m_filename.c_str(), "rb");
	if (!fp)  printf("File could not be opened for reading");
	//if (!fp)  printf("File could not be opened for reading");
  	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,NULL,NULL,NULL);
	if(!png_ptr) abort();
  	info_ptr = png_create_info_struct(png_ptr);
  	if(!info_ptr) abort();
  	new_width=png_get_image_width(png_ptr,info_ptr);
  	new_height=png_get_image_height(png_ptr,info_ptr);
	if (setjmp(png_jmpbuf(png_ptr))) abort();
	color_type=png_get_color_type(png_ptr,info_ptr);
        //std::cout<<"color_type"<<color_type;
  	bit_depth  = png_get_bit_depth(png_ptr,info_ptr);
	png_init_io(png_ptr, fp);
  	png_set_sig_bytes(png_ptr,0);
  	png_read_info(png_ptr, info_ptr);
  	png_get_IHDR(png_ptr, info_ptr, &new_width, &new_height, &bit_depth, &color_type,NULL,NULL,NULL);
	//png_bytep *row_pointers =new png_bytep[new_height];
	//png_read_update_info(png_ptr, info_ptr);
	//png_bytep *row_pointers= (png_bytep*)malloc(sizeof(png_bytep) * new_height);
	png_read_update_info(png_ptr, info_ptr);
	png_bytep *row_pointers=new png_bytep[new_height];
	for(unsigned int row=0; row<new_height; row++) {
    //row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr,info_ptr));
		//row_pointers[row] =(png_byte*)png_malloc(png_ptr, png_get_rowbytes(png_ptr,info_ptr));
			row_pointers[row] =new png_byte[png_get_rowbytes(png_ptr,info_ptr)];
        }
	//if (row_pointers == NULL) abort();
	//if(bit_depth == 16)

						// png_set_strip_alpha(png_ptr);
            // png_set_packing(png_ptr);
            // png_set_packswap(png_ptr);
						//
            // if(color_type == PNG_COLOR_TYPE_PALETTE)
            // png_set_palette_to_rgb(png_ptr);
						//
            // // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
            // if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
            // png_set_expand_gray_1_2_4_to_8(png_ptr);
						//
            // if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
            // png_set_tRNS_to_alpha(png_ptr);
						//
						// if (bit_depth < 16)
      			// png_set_expand_16(png_ptr);
						// if (bit_depth == 16)	png_set_scale_16(png_ptr);
						// else	png_set_strip_16(png_ptr);
						// if (color_type & PNG_COLOR_MASK_ALPHA)
       		// 	png_set_strip_alpha(png_ptr);

            // These color_type don't have an alpha channel then fill it with 0xff.
            // if(color_type == PNG_COLOR_TYPE_RGB ||
            // color_type == PNG_COLOR_TYPE_GRAY ||
            // color_type == PNG_COLOR_TYPE_PALETTE)
            // png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
						//
            // if(color_type == PNG_COLOR_TYPE_GRAY ||
            //   color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
            // png_set_gray_to_rgb(png_ptr);



	png_read_image(png_ptr, row_pointers);
/* Read rest of file, and get additional chunks in info_ptr - REQUIRED */
  //png_read_end(png_ptr, info_ptr);
/* Clean up after the read, and free any memory allocated - REQUIRED */


        //png_read_image(png_ptr, row_ptrs);

	new_pixel_buffer=new PixelBuffer(new_width,new_height,ColorData(0.0,0.0,0.0,0.0));
	if(color_type==PNG_COLOR_TYPE_RGB){
            for (unsigned int y=0; y<new_height; y++) {
              png_byte* row = row_pointers[new_height-y-1];
              for (unsigned int x=0; x<new_width; x++) {
                      png_byte* ptr = &(row[x*3]);
                      /*printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
                             x, y, ptr[0], ptr[1], ptr[2]);*/
                      /* set pixel on the pixelbuffer*/

                      new_pixel_buffer->setPixel(x,y,ColorData ((float)ptr[0]/255,(float)ptr[1]/255,(float)ptr[2]/255));
                }
            }
         }

        else if(color_type==PNG_COLOR_TYPE_RGBA){
            for (unsigned int y=0; y<new_height; y++) {
		png_byte* row = row_pointers[new_height-y-1];
              for (unsigned int x=0; x<new_width; x++) {
                      png_byte* ptr = &(row[x*4]);
                      /*printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
                             x, y, ptr[0], ptr[1], ptr[2], ptr[3]);*/
                      /* set pixel on the pixelbuffer*/

                      new_pixel_buffer->setPixel(x,y,ColorData ((float)ptr[0]/255,(float)ptr[1]/255,(float)ptr[2]/255,(float)ptr[3]/255));

                }
            }

            }
        else  std::cout<<"[process_file] input file must be PNG_COLOR_TYPE_RGBA or PNG_COLOR_TYPE_RGB"
                       "(lacks the alpha channel)";
		delete [] row_pointers;
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);


	return new_pixel_buffer;




}




void PngFacade::save(int width,int height, PixelBuffer* pixelbuffer){
	new_width=width;
	new_height=height;
	FILE *fp = fopen(m_filename.c_str(), "wb");
	if(!fp) abort();
 	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  	if (png_ptr == NULL)
   	{
      		fclose(fp);
      		abort();
   	}
	if (info_ptr == NULL)
	{
      		fclose(fp);
      		png_destroy_write_struct(&png_ptr,  NULL);
      		abort();
   	}


  	png_infop info_ptr = png_create_info_struct(png_ptr);
	//png_colorp palette = (png_colorp)png_malloc(png_ptr, PNG_MAX_PALETTE_LENGTH* (sizeof (png_color)));
	//png_set_PLTE(png_ptr, info_ptr, palette, PNG_MAX_PALETTE_LENGTH);
  	if (!info_ptr) abort();

  	if (setjmp(png_jmpbuf(png_ptr))){
      /* If we get here, we had a problem writing the file */
      		fclose(fp);
      		png_destroy_write_struct(&png_ptr, &info_ptr);
      		abort();
   	};
	color_type=png_get_color_type(png_ptr,info_ptr);
  	//bit_depth  = png_get_bit_depth(png_ptr,info_ptr);
	png_set_IHDR(png_ptr,info_ptr,new_width,new_height,8,PNG_COLOR_TYPE_RGBA,PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	//png_bytep *row_pointers=(png_bytep*) malloc(sizeof(png_bytep) * height);
/*	for (int h = 0; h < new_height; h++) {
      		row_pointers[h] = (png_bytep) malloc (new_width * sizeof (png_byte));
   	}
*/

	//int bytes_per_pixel=3;
	png_bytep row_pointers[new_height];
	for (unsigned int y=0; y<new_height; y++){
                row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
	}
//	if (color_type==PNG_COLOR_TYPE_RGB){
		// for (unsigned int y=0; y<new_height; y++) {
		// 		png_byte* row = row_pointers[new_height-y-1];
		// 	for (unsigned int x=0; x<new_width; x++) {
		// 					png_byte* ptr = &(row[x*3]);
		// 					/*printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
		// 								 x, y, ptr[0], ptr[1], ptr[2], ptr[3]);*/
		// 					/* set pixel on the pixelbuffer*/
		// 					ColorData color=pixelbuffer->getPixel(x,y);
		// 					ptr[0]=color.getRed()*255.0;
		// 					ptr[1]=color.getGreen()*255.0;
		// 					ptr[2]=color.getBlue()*255.0;
		// 					//new_pixel_buffer->setPixel(x,y,ColorData ((float)ptr[0]/255,(float)ptr[1]/255,(float)ptr[2]/255,(float)ptr[3]/255));
		//
		// 		}
		// }


//	}
//	else if (color_type==PNG_COLOR_TYPE_RGBA){
	for (unsigned int y=0; y<new_height; y++) {
			png_byte* row = row_pointers[new_height-y-1];
		for (unsigned int x=0; x<new_width; x++) {
						png_byte* ptr = &(row[x*4]);
						/*printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
									 x, y, ptr[0], ptr[1], ptr[2], ptr[3]);*/
						/* set pixel on the pixelbuffer*/
						ColorData color=pixelbuffer->getPixel(x,y);
						ptr[0]=color.getRed()*255.0;
						ptr[1]=color.getGreen()*255.0;
						ptr[2]=color.getBlue()*255.0;
						ptr[3]=color.getAlpha()*255.0;
						//new_pixel_buffer->setPixel(x,y,ColorData ((float)ptr[0]/255,(float)ptr[1]/255,(float)ptr[2]/255,(float)ptr[3]/255));

			}
	}
//}
	// else std::cout<<"[process_file] input file is PNG_COLOR_TYPE_RGB but must be PNG_COLOR_TYPE_RGBA or PNG_COLOR_TYPE_RGB"
	// 							 "(lacks the alpha channel)";
/*	png_byte image[new_height*new_width*bytes_per_pixel];
	for (int k = 0; k < new_height; k++){
     		row_pointers[k] = image + k*new_width*bytes_per_pixel;
	}
*/
	png_set_rows (png_ptr, info_ptr, row_pointers);
  png_init_io(png_ptr, fp);
	if (setjmp(png_jmpbuf(png_ptr))) abort();
	png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
	//new_width=png_get_image_width(png_ptr,info);
	//new_height=png_get_image_height(png_ptr,info);

  	png_write_info(png_ptr, info_ptr);
	if (setjmp(png_jmpbuf(png_ptr))) abort();


	//png_bytep *row_pointers=(png_bytep) malloc (new_height * sizeof (png_bytep));
/*	for (int h = 0; h < new_height; h++) {
      		row_pointers[h] = (png_bytep) malloc (new_width * sizeof (png_byte));
   	}
*/

	png_write_image(png_ptr, row_pointers);
	if (setjmp(png_jmpbuf(png_ptr))) abort();
  	png_write_end(png_ptr, NULL);

/* 	for(int y = 0; y < new_height; y++) {
    		 delete[] row_pointers[y];
  	}
*/
	//delete [] row_pointers;
	//png_free (png_ptr, row_pointers);
	//free(row_pointers);
	/* Clean up after the write, and free any memory allocated */
   	png_destroy_write_struct(&png_ptr, &info_ptr);


  	fclose(fp);



}