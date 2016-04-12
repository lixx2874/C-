#include "FlashPhotoApp.h"
#include "ColorData.h"
#include "PixelBuffer.h"
#include "ToolFactory.h"
#include "Tool.h"
#include "ImageFacade.h"
#include "PngFacade.h"
#include "JpegFacade.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <png.h>
#include <zlib.h>
#include <cmath>
#include "TBlur.h"

#include <iostream>

using std::cout;
using std::endl;

FlashPhotoApp::FlashPhotoApp(int argc, char* argv[], int width, int height, ColorData backgroundColor) : BaseGfxApp(argc, argv, width, height, 50, 50, GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH, true, width+51, 50)
{
    // Set the name of the window
    setCaption("FlashPhoto");

    // Initialize Interface
    initializeBuffers(backgroundColor, width, height);
    // Create array of tools and populate
    m_tools = new Tool* [ToolFactory::getNumTools()];
    undo=new PixelBuffer*[20];
    redo=new PixelBuffer*[20];
    m_undo=-1;
    m_redo=-1;
    for (int i = 0; i < ToolFactory::getNumTools(); i++) {
        m_tools[i] = ToolFactory::createTool(i);
    }

    initGlui();
    initGraphics();
}

void FlashPhotoApp::display()
{
    drawPixels(0, 0, m_width, m_height, m_displayBuffer->getData());
}

FlashPhotoApp::~FlashPhotoApp()
{
    if (m_displayBuffer) {
        delete m_displayBuffer;
    }
    // Delete each of the tools before deleting the list of tool pointers.
    if (m_tools) {
        Tool ** toolsEnd =  m_tools + ToolFactory::getNumTools();
        for (Tool ** tool_i = m_tools; tool_i < toolsEnd; tool_i++) {
            Tool* tool = *tool_i;
            if (tool) {
                delete tool;
            }
        }

        delete [] m_tools;
    }
}


void FlashPhotoApp::mouseDragged(int x, int y)
{
       int max_steps = 30;

    // We implimented a smoothing feature by interpolating between
    // mouse events. This is at the expense of processing, though,
    // because we just "stamp" the tool many times between the two
    // even locations. you can reduce max_steps until it runs
    // smoothly on your machine.

    // Get the differences between the events
    // in each direction
    int delta_x = x-m_mouseLastX;
    int delta_y = y-m_mouseLastY;

    // Calculate the min number of steps necesary to fill
    // completely between the two event locations.
    float pixelsBetween = fmax(abs(delta_x), abs(delta_y));
    int step_count = pixelsBetween;
    int step_size = 1;

    // Optimize by maxing out at the max_steps,
    // and fill evenly between
    if (pixelsBetween > max_steps)
    {
        step_size = pixelsBetween/max_steps;
        step_count = max_steps;
    }

    // Iterate between the event locations
    for (int i = 0; i < pixelsBetween; i+=step_size)
    {
        int x = m_mouseLastX+(i*delta_x/pixelsBetween);
        int y = m_mouseLastY+(i*delta_y/pixelsBetween);
        if (m_curTool<=4){
        m_tools[m_curTool]->applyToBuffer(x, m_height-y, ColorData(m_curColorRed, m_curColorGreen, m_curColorBlue), m_displayBuffer);
      }
        else if (m_curTool==5){
          if (m_fileName.substr( m_fileName.length() - 4 , m_fileName.length()) == ".png"){
           PngFacade *png=new PngFacade(m_fileName);
           m_tools[m_curTool]->apply_stamp(x,m_height-y,png,m_displayBuffer);
         }
         else{
           JpegFacade *jpeg=new JpegFacade(m_fileName);
           m_tools[m_curTool]->apply_stamp(x,m_height-y,jpeg,m_displayBuffer);
         }
        }else{
           m_displayBuffer=m_tools[m_curTool]->applyBlur(x, m_height-y, ColorData(m_curColorRed, m_curColorGreen, m_curColorBlue), m_displayBuffer);
        }
    }

    // let the previous point catch up with the current.
    m_mouseLastX = x;
    m_mouseLastY = y;
}

void FlashPhotoApp::mouseMoved(int x, int y)
{

}

void FlashPhotoApp::leftMouseDown(int x, int y)
{
	pushUndo();
    //std::cout << "mousePressed " << x << " " << y << std::endl;
    if (m_curTool<=4){
    m_tools[m_curTool]->applyToBuffer(x, m_height-y, ColorData(m_curColorRed, m_curColorGreen, m_curColorBlue), m_displayBuffer);
  }
    else if (m_curTool==5){
      if (m_fileName.substr( m_fileName.length() - 4 , m_fileName.length()) == ".png"){
       PngFacade *png=new PngFacade(m_fileName);
       m_tools[m_curTool]->apply_stamp(x,m_height-y,png,m_displayBuffer);
     }
     else{
       JpegFacade *jpeg=new JpegFacade(m_fileName);
       m_tools[m_curTool]->apply_stamp(x,m_height-y,jpeg,m_displayBuffer);
     }
    }else{
       m_displayBuffer=m_tools[m_curTool]->applyBlur(x, m_height-y, ColorData(m_curColorRed, m_curColorGreen, m_curColorBlue), m_displayBuffer);
    }
    m_mouseLastX = x;
    m_mouseLastY = y;
}

void FlashPhotoApp::leftMouseUp(int x, int y)
{
    //std::cout << "mouseReleased " << x << " " << y << std::endl;
}

void FlashPhotoApp::initializeBuffers(ColorData backgroundColor, int width, int height) {
    m_displayBuffer = new PixelBuffer(width, height, backgroundColor);
}
void FlashPhotoApp::pushUndo(){
	  m_undo++;
	if(m_undo==20){
		pushStack(undo);
		m_undo=19;
	}
	undo[m_undo]=new PixelBuffer(m_displayBuffer->getWidth(),m_displayBuffer->getHeight(),m_displayBuffer->getBackgroundColor());
    PixelBuffer::copyPixelBuffer(m_displayBuffer,undo[m_undo]);
}
void FlashPhotoApp::initGlui()
{
    // Select first tool (this activates the first radio button in glui)
    m_curTool = 0;

    GLUI_Panel *toolPanel = new GLUI_Panel(m_glui, "Tool Type");
    {
        GLUI_RadioGroup *radio = new GLUI_RadioGroup(toolPanel, &m_curTool, UI_TOOLTYPE, s_gluicallback);
        // Create interface buttons for different tools:
        new GLUI_RadioButton(radio, "Pen");
        new GLUI_RadioButton(radio, "Eraser");
        new GLUI_RadioButton(radio, "Spray Can");
        new GLUI_RadioButton(radio, "Caligraphy Pen");
        new GLUI_RadioButton(radio, "Highlighter");
        new GLUI_RadioButton(radio, "Stamp");
        new GLUI_RadioButton(radio, "Blur");
    }


    GLUI_Panel *colorPanel = new GLUI_Panel(m_glui, "Tool Color");
    {
        m_curColorRed = 0;
        m_gluiControlHooks.spinnerRed  = new GLUI_Spinner(colorPanel, "Red:", &m_curColorRed, UI_COLOR_R, s_gluicallback);
        m_gluiControlHooks.spinnerRed->set_float_limits(0, 1.0);

        m_curColorGreen = 0;
        m_gluiControlHooks.spinnerGreen  = new GLUI_Spinner(colorPanel, "Green:", &m_curColorGreen, UI_COLOR_G, s_gluicallback);
        m_gluiControlHooks.spinnerGreen->set_float_limits(0, 1.0);

        m_curColorBlue = 0;
        m_gluiControlHooks.spinnerBlue  = new GLUI_Spinner(colorPanel, "Blue:", &m_curColorBlue, UI_COLOR_B, s_gluicallback);
        m_gluiControlHooks.spinnerBlue->set_float_limits(0, 1.0);

        new GLUI_Button(colorPanel, "Red", UI_PRESET_RED, s_gluicallback);
        new GLUI_Button(colorPanel, "Orange", UI_PRESET_ORANGE, s_gluicallback);
        new GLUI_Button(colorPanel, "Yellow", UI_PRESET_YELLOW, s_gluicallback);
        new GLUI_Button(colorPanel, "Green", UI_PRESET_GREEN, s_gluicallback);
        new GLUI_Button(colorPanel, "Blue", UI_PRESET_BLUE, s_gluicallback);
        new GLUI_Button(colorPanel, "Purple", UI_PRESET_PURPLE, s_gluicallback);
        new GLUI_Button(colorPanel, "White", UI_PRESET_WHITE, s_gluicallback);
        new GLUI_Button(colorPanel, "Black", UI_PRESET_BLACK, s_gluicallback);
    }

    // UNDO,REDO,QUIT
    {
        m_gluiControlHooks.undoButton = new GLUI_Button(m_glui, "Undo", UI_UNDO, s_gluicallback);
        undoEnabled(true);
        m_gluiControlHooks.redoButton  = new GLUI_Button(m_glui, "Redo", UI_REDO, s_gluicallback);
        redoEnabled(true);

        new GLUI_Separator(m_glui);
        new GLUI_Button(m_glui, "Quit", UI_QUIT, (GLUI_Update_CB)exit);
    }

    new GLUI_Column(m_glui, true);
    GLUI_Panel *filterPanel = new GLUI_Panel(m_glui, "Filters");
    {
        GLUI_Panel *blurPanel = new GLUI_Panel(filterPanel, "Blur");
        {
            GLUI_Spinner * filterBlurAmount = new GLUI_Spinner(blurPanel, "Amount:", &m_filterParameters.blur_amount);
            filterBlurAmount->set_int_limits(0, 20);

            filterBlurAmount->set_int_val(5);

            new GLUI_Button(blurPanel, "Apply", UI_APPLY_BLUR, s_gluicallback);
        }

        GLUI_Panel *motionBlurPanel = new GLUI_Panel(filterPanel, "MotionBlur");
        {
            GLUI_Spinner * filterMotionBlurAmount = new GLUI_Spinner(motionBlurPanel, "Amount:", &m_filterParameters.motionBlur_amount);
            filterMotionBlurAmount->set_int_limits(0, 100);

            filterMotionBlurAmount->set_int_val(5);

            m_filterParameters.motionBlur_direction = 0;
            GLUI_RadioGroup *dirBlur = new GLUI_RadioGroup(motionBlurPanel, &m_filterParameters.motionBlur_direction);
            new GLUI_RadioButton(dirBlur, "North/South");
            new GLUI_RadioButton(dirBlur, "East/West");
            new GLUI_RadioButton(dirBlur, "NorthEast/SouthWest");
            new GLUI_RadioButton(dirBlur, "NorthWest/SouthEast");

            new GLUI_Button(motionBlurPanel, "Apply", UI_APPLY_MOTION_BLUR, s_gluicallback);
        }
        GLUI_Panel *sharpenPanel = new GLUI_Panel(filterPanel, "Sharpen");
        {
            GLUI_Spinner * filterSharpAmount = new GLUI_Spinner(sharpenPanel, "Amount:", &m_filterParameters.sharpen_amount);
            filterSharpAmount->set_int_limits(0, 100);

            filterSharpAmount->set_int_val(5);

            new GLUI_Button(sharpenPanel, "Apply", UI_APPLY_SHARP, s_gluicallback);
        }
        GLUI_Panel *edgeDetPanel = new GLUI_Panel(filterPanel, "Edge Detect");

        {
            new GLUI_Button(edgeDetPanel, "Apply", UI_APPLY_EDGE, s_gluicallback);
        }
        GLUI_Panel *thresPanel = new GLUI_Panel(filterPanel, "Threshold");
        {
            GLUI_Spinner * filterThresholdAmount = new GLUI_Spinner(thresPanel, "Level:", &m_filterParameters.threshold_amount);
            filterThresholdAmount->set_float_limits(0, 1);
            filterThresholdAmount->set_float_val(0.5);

            new GLUI_Button(thresPanel, "Apply", UI_APPLY_THRESHOLD, s_gluicallback);
        }

		new GLUI_Column(filterPanel, true);

        GLUI_Panel *saturPanel = new GLUI_Panel(filterPanel, "Saturation");
        {
            GLUI_Spinner * filterSaturationAmount = new GLUI_Spinner(saturPanel, "Amount:", &m_filterParameters.saturation_amount);
            filterSaturationAmount->set_float_limits(-10, 10);
            filterSaturationAmount->set_float_val(1);

            new GLUI_Button(saturPanel, "Apply", UI_APPLY_SATURATE, s_gluicallback);
        }

        GLUI_Panel *channelPanel = new GLUI_Panel(filterPanel, "Channels");
        {
            GLUI_Spinner * filterChannelRed = new GLUI_Spinner(channelPanel, "Red:", &m_filterParameters.channel_colorRed);
            GLUI_Spinner * filterChannelGreen = new GLUI_Spinner(channelPanel, "Green:", &m_filterParameters.channel_colorGreen);
            GLUI_Spinner * filterChannelBlue = new GLUI_Spinner(channelPanel, "Blue:", &m_filterParameters.channel_colorBlue);

            filterChannelRed->set_float_limits(0, 10);
            filterChannelRed->set_float_val(1);
            filterChannelGreen->set_float_limits(0, 10);
            filterChannelGreen->set_float_val(1);
            filterChannelBlue->set_float_limits(0, 10);
            filterChannelBlue->set_float_val(1);

            new GLUI_Button(channelPanel, "Apply", UI_APPLY_CHANNEL, s_gluicallback);
        }

        GLUI_Panel *quantPanel = new GLUI_Panel(filterPanel, "Quantize");
        {
            GLUI_Spinner * filterQuantizeBins = new GLUI_Spinner(quantPanel, "Bins:", &m_filterParameters.quantize_bins);
            filterQuantizeBins->set_int_limits(2, 256);
            filterQuantizeBins->set_int_val(8);
            filterQuantizeBins->set_speed(0.1);

            new GLUI_Button(quantPanel, "Apply", UI_APPLY_QUANTIZE, s_gluicallback);
        }

        GLUI_Panel *specialFilterPanel = new GLUI_Panel(filterPanel, "Special Filter"); // YOUR SPECIAL FILTER PANEL
        {
            new GLUI_Button(specialFilterPanel, "Apply", UI_APPLY_SPECIAL_FILTER, s_gluicallback);
        }
    }

    new GLUI_Column(m_glui, true);

    GLUI_Panel *imagePanel = new GLUI_Panel(m_glui, "Image I/O");
    {
        m_gluiControlHooks.fileBrowser = new GLUI_FileBrowser(imagePanel, "Choose Image", false, UI_FILE_BROWSER, s_gluicallback);

        m_gluiControlHooks.fileBrowser->set_h(400);

        m_gluiControlHooks.fileNameBox = new     GLUI_EditText( imagePanel , "Image:", m_fileName, UI_FILE_NAME, s_gluicallback );
        m_gluiControlHooks.fileNameBox->set_w(200);

        new GLUI_Separator(imagePanel);

        m_gluiControlHooks.currentFileLabel = new GLUI_StaticText(imagePanel, "Will load image: none");
        m_gluiControlHooks.loadCanvasButton = new GLUI_Button(imagePanel, "Load Canvas", UI_LOAD_CANVAS_BUTTON, s_gluicallback);
        m_gluiControlHooks.loadStampButton = new GLUI_Button(imagePanel, "Load Stamp", UI_LOAD_STAMP_BUTTON, s_gluicallback);

        new GLUI_Separator(imagePanel);

        m_gluiControlHooks.saveFileLabel = new GLUI_StaticText(imagePanel, "Will save image: none");

        m_gluiControlHooks.saveCanvasButton = new GLUI_Button(imagePanel, "Save Canvas", UI_SAVE_CANVAS_BUTTON, s_gluicallback);

        loadCanvasEnabled(true);
        loadStampEnabled(true);
        saveCanvasEnabled(true);
    }
    return;
}

void FlashPhotoApp::gluiControl(int controlID)
{

    switch (controlID) {
        case UI_PRESET_RED:
            m_curColorRed = 1;
            m_curColorGreen = 0;
            m_curColorBlue = 0;
            updateColors();
            break;
        case UI_PRESET_ORANGE:
            m_curColorRed = 1;
            m_curColorGreen = 0.5;
            m_curColorBlue = 0;
            updateColors();
            break;
        case UI_PRESET_YELLOW:
            m_curColorRed = 1;
            m_curColorGreen = 1;
            m_curColorBlue = 0;
            updateColors();
            break;
        case UI_PRESET_GREEN:
            m_curColorRed = 0;
            m_curColorGreen = 1;
            m_curColorBlue = 0;
            updateColors();
            break;
        case UI_PRESET_BLUE:
            m_curColorRed = 0;
            m_curColorGreen = 0;
            m_curColorBlue = 1;
            updateColors();
            break;
        case UI_PRESET_PURPLE:
            m_curColorRed = 0.5;
            m_curColorGreen = 0;
            m_curColorBlue = 1;
            updateColors();
            break;
        case UI_PRESET_WHITE:
            m_curColorRed = 1;
            m_curColorGreen = 1;
            m_curColorBlue = 1;
            updateColors();
            break;
        case UI_PRESET_BLACK:
            m_curColorRed = 0;
            m_curColorGreen = 0;
            m_curColorBlue = 0;
            updateColors();
            break;
        case UI_APPLY_BLUR:
      pushUndo();
            applyFilterBlur();
            break;
        case UI_APPLY_SHARP:
       pushUndo();
            applyFilterSharpen();
            break;
        case UI_APPLY_MOTION_BLUR:
        pushUndo();
            applyFilterMotionBlur();
            break;
        case UI_APPLY_EDGE:
      pushUndo();
            applyFilterEdgeDetect();
            break;
        case UI_APPLY_THRESHOLD:
       pushUndo();
            applyFilterThreshold();
            break;
        case UI_APPLY_DITHER:
       pushUndo();
            applyFilterThreshold();
            break;
        case UI_APPLY_SATURATE:
     pushUndo();
            applyFilterSaturate();
            break;
        case UI_APPLY_CHANNEL:
     pushUndo();
            applyFilterChannel();
            break;
        case UI_APPLY_QUANTIZE:
      pushUndo();
            applyFilterQuantize();
            break;
        case UI_APPLY_SPECIAL_FILTER:
       pushUndo();
            applyFilterSpecial();
            break;
        case UI_FILE_BROWSER:
            setImageFile(m_gluiControlHooks.fileBrowser->get_file());
            break;
        case UI_LOAD_CANVAS_BUTTON:
            loadImageToCanvas();
            break;
        case UI_LOAD_STAMP_BUTTON:
            loadImageToStamp();
            break;
        case UI_SAVE_CANVAS_BUTTON:
            saveCanvasToFile();
            // Reload the current directory:
            m_gluiControlHooks.fileBrowser->fbreaddir(".");
            break;
        case UI_FILE_NAME:
            setImageFile(m_fileName);
            break;
        case UI_UNDO:
            undoOperation();
            break;
        case UI_REDO:
            redoOperation();
            break;
        case UI_QUIT:
            // In the event of quit button,
            // destruct this PaintShop.
            delete this;
            exit(0);
        default:
            break;
    }

    // Forces canvas to update changes made in this function
    m_glui->post_update_main_gfx();
}

// **********************
// *** GLUI CALLBACKS ***

// Edit these functions to provide instructions
// for how FlashPhotoApp should respond to the
// button presses.

void FlashPhotoApp::loadImageToCanvas()
{
    //cout << "Load Canvas has been clicked for file " << m_fileName << endl;

    if(m_fileName.substr( m_fileName.length() - 4 , m_fileName.length()) == ".png"){
        PngFacade *png=new PngFacade(m_fileName);
        PixelBuffer* new_pixel_buffer=png->load();
        initializeBuffers(ColorData(0.0,0.0,0.0,0.0), new_pixel_buffer->getWidth(), new_pixel_buffer->getHeight());
      //  m_displayBuffer=new_pixel_buffer;
        PixelBuffer::copyPixelBuffer(png->load(), m_displayBuffer);
        setWindowDimensions(m_displayBuffer->getWidth(), m_displayBuffer->getHeight());
    }
    else if(m_fileName.substr( m_fileName.length() - 4 , m_fileName.length()) == ".jpg"){
	JpegFacade* jpeg=new JpegFacade(m_fileName);
        PixelBuffer* new_pixel_buffer=jpeg->load();
        initializeBuffers(ColorData(0.0,0.0,0.0,0.0), new_pixel_buffer->getWidth(), new_pixel_buffer->getHeight());
        PixelBuffer::copyPixelBuffer(new_pixel_buffer, m_displayBuffer);
        setWindowDimensions(m_displayBuffer->getWidth(), m_displayBuffer->getHeight());
    }
    else cout<< "The image should be in either .png format or .jpg format" <<endl;

}

void FlashPhotoApp::loadImageToStamp()
{
    //cout << "Load Stamp has been clicked for file " << m_fileName << endl;
    //PngFacade *pnga=new PngFacade(m_fileName);

}

void FlashPhotoApp::saveCanvasToFile()
{
    //cout << "Save Canvas been clicked for file " << m_fileName << endl;
    if(m_fileName.substr( m_fileName.length() - 4 , m_fileName.length()) == ".png"){
    PngFacade *pngs=new PngFacade(m_fileName);
    pngs->save(m_displayBuffer->getWidth(), m_displayBuffer->getHeight(),m_displayBuffer);
  }
    else if(m_fileName.substr( m_fileName.length() - 4 , m_fileName.length()) == ".jpg"){
    JpegFacade *jpeg=new JpegFacade(m_fileName);
    jpeg->save(m_displayBuffer->getWidth(), m_displayBuffer->getHeight(),m_displayBuffer);

  }
    else cout<< "The image should be in either .png format or .jpg format" <<endl;
}

void FlashPhotoApp::applyFilterThreshold()
{
	
    //cout << "Apply has been clicked for Threshold has been clicked with amount =" << m_filterParameters.threshold_amount << endl;
    for (int y=0;y<m_displayBuffer->getHeight();y++){
      for(int x=0;x<m_displayBuffer->getWidth();x++){
        ColorData color=m_displayBuffer->getPixel(x,y);
        if(color.getRed()<m_filterParameters.threshold_amount) color.setRed(0.0); else color.setRed(1.0);
        if(color.getGreen()<m_filterParameters.threshold_amount) color.setGreen(0.0); else color.setGreen(1.0);
        if(color.getBlue()<m_filterParameters.threshold_amount) color.setBlue(0.0); else color.setBlue(1.0);
        if(color.getAlpha()) {if(color.getAlpha()<m_filterParameters.threshold_amount) color.setAlpha(0.0); else color.setAlpha(1.0);}
        m_displayBuffer->setPixel(x,y,color);
      }
    }
}

void FlashPhotoApp::applyFilterChannel()
{
    // cout << "Apply has been clicked for Channels with red = " << m_filterParameters.channel_colorRed
    // << ", green = " << m_filterParameters.channel_colorGreen
    // << ", blue = " << m_filterParameters.channel_colorBlue << endl;
    for (int y=0;y<m_displayBuffer->getHeight();y++){
      for(int x=0;x<m_displayBuffer->getWidth();x++){
        ColorData color=m_displayBuffer->getPixel(x,y);
        color.setRed(m_filterParameters.channel_colorRed*color.getRed());
        color.setGreen(m_filterParameters.channel_colorGreen*color.getGreen());
        color.setBlue(m_filterParameters.channel_colorBlue*color.getBlue());
        m_displayBuffer->setPixel(x,y,color);
      }
    }
}

void FlashPhotoApp::applyFilterSaturate()
{
    //cout << "Apply has been clicked for Saturate with amount = " << m_filterParameters.saturation_amount << endl;
    //x*original+(1-x)gray
    for (int y=0;y<m_displayBuffer->getHeight();y++){
      for(int x=0;x<m_displayBuffer->getWidth();x++){
        ColorData color=m_displayBuffer->getPixel(x,y);
        ColorData grayscale(color.getLuminance(),color.getLuminance(),color.getLuminance());
        color.setRed(m_filterParameters.saturation_amount*color.getRed()+(1-m_filterParameters.saturation_amount)*grayscale.getRed());
        color.setGreen(m_filterParameters.saturation_amount*color.getGreen()+(1-m_filterParameters.saturation_amount)*grayscale.getGreen());
        color.setBlue(m_filterParameters.saturation_amount*color.getBlue()+(1-m_filterParameters.saturation_amount)*grayscale.getBlue());
        m_displayBuffer->setPixel(x,y,color);
      }
    }
}

void FlashPhotoApp::applyFilterBlur()
{
	 float kernel[3][3];
	 kernel[0][0]=0; kernel[0][1]=0.2;kernel[0][2]=0.0;
    kernel[1][0]=0.2; kernel[1][1]=0.2; kernel[1][2]=0.2;
    kernel[2][0]=0.0;  kernel[2][1]=0.2; kernel[2][2]=0.0;
    for(int i=0;i<m_filterParameters.blur_amount;i++){
    PixelBuffer *n=new PixelBuffer(m_displayBuffer->getWidth(),m_displayBuffer->getHeight(),m_displayBuffer->getBackgroundColor());
    int width=m_displayBuffer->getWidth();
    int height=m_displayBuffer->getHeight();
    for(int y=0;y<height;y++){
      for (int x=0;x<width;x++){
		  
          ColorData data=ColorData(0,0,0);
          
        int count=0;
          for(int yy=-1;yy<2;yy++){
			  for(int xx=-1;xx<2;xx++){
				
					   if(((x+xx)<width&&(x+xx>=0))&&((yy+y)<height&&(y+yy>=0))){
					  data=data+m_displayBuffer->getPixel(x+xx,yy+y)*kernel[yy+1][xx+1];
					  if(kernel[yy+1][xx+1]){
				count=count+1;
				  }
				  }else{
					
				  }
				
			
		  }
		  }
		
		 data=data*(5.0f/count);
          n->setPixel(x,y,data);
       
      }
    }
    
    m_displayBuffer=n;
}
    cout << "Apply has been clicked for Blur with amount = " << m_filterParameters.blur_amount << endl;
}

void FlashPhotoApp::applyFilterSharpen()
{
	float kernel[3][3];
     kernel[0][0]=-((14+m_filterParameters.sharpen_amount)/10-1)/8;  kernel[0][1]=-((14+m_filterParameters.sharpen_amount)/10-1)/8; kernel[0][2]=-((14+m_filterParameters.sharpen_amount)/10-1)/8;
    kernel[1][0]=-((14+m_filterParameters.sharpen_amount)/10-1)/8; kernel[1][1]=(14+m_filterParameters.sharpen_amount)/10; kernel[1][2]=-((14+m_filterParameters.sharpen_amount)/10-1)/8;
     kernel[2][0]=-((14+m_filterParameters.sharpen_amount)/10-1)/8; kernel[2][1]=-((14+m_filterParameters.sharpen_amount)/10-1)/8; kernel[2][2]=-((14+m_filterParameters.sharpen_amount)/10-1)/8;

    PixelBuffer *n=new PixelBuffer(m_displayBuffer->getWidth(),m_displayBuffer->getHeight(),m_displayBuffer->getBackgroundColor());
    PixelBuffer::copyPixelBuffer(m_displayBuffer,n);
    int width=m_displayBuffer->getWidth();
    int height=m_displayBuffer->getHeight();
    for(int y=1;y<height-1;y++){
      for (int x=1;x<width-1;x++){
		  
          ColorData data=ColorData(0,0,0);
          for(int yy=-1;yy<2;yy++){
			  for(int xx=-1;xx<2;xx++){
				  if(((x+xx)<width&&(x+xx>=0))&&((yy+y)<height&&(y+yy>=0))){
					  data=data+m_displayBuffer->getPixel(x+xx,yy+y)*kernel[yy+1][xx+1];
					  
				
				  	  
				  }else{
					  ColorData u=ColorData(0.5,0.5,0.5);
					   data=data+u*kernel[yy+1][xx+1];
				  }
			  }
			}  
		  
          n->setPixel(x,y,data);
      
      }
    }
    m_displayBuffer=n;

    cout << "Apply has been clicked for Sharpen with amount = " << m_filterParameters.sharpen_amount << kernel[1][1]<<kernel[1][0]<<endl;
}

void FlashPhotoApp::applyFilterMotionBlur()
{
   int amount=(int)m_filterParameters.motionBlur_amount;
    float kernel[amount][amount];
    for(int x=0;x<amount;x++){
        for(int y=0;y<amount;y++){
            kernel[x][y]=0;
        }

    }
  if(m_filterParameters.motionBlur_direction==0){
      for(int x=0;x<amount;x++){
          kernel[x][amount/2]=1.0f/amount;


      }
  }else if(m_filterParameters.motionBlur_direction==1){
      for(int x=0;x<amount;x++){
          kernel[amount/2][x]=1.0f/amount;


      }
  }else if(m_filterParameters.motionBlur_direction==2){
      for(int x=0;x<amount;x++){
          kernel[x][amount-x-1]=1.0f/amount;


      }
  }else{
      for(int x=0;x<amount;x++){
          kernel[x][x]=1.0f/amount;


      }
  }


   PixelBuffer *n=new PixelBuffer(m_displayBuffer->getWidth(),m_displayBuffer->getHeight(),m_displayBuffer->getBackgroundColor());
   int width=m_displayBuffer->getWidth();
   int height=m_displayBuffer->getHeight();
   for(int y=0;y<height;y++){
     for (int x=0;x<width;x++){

         ColorData data=ColorData(0,0,0);

       int count=0;
         for(int yy=0;yy<amount;yy++){
             for(int xx=0;xx<amount;xx++){

                      if(((x+xx-amount/2)<width&&(x+xx-amount/2>=0))&&((yy+y-amount/2)<height&&(y+yy-amount/2>=0))){
                     data=data+m_displayBuffer->getPixel(x+xx-amount/2,yy+y-amount/2)*kernel[yy][xx];
                     if(kernel[yy][xx]){
               count=count+1;
                 }
                 }else{

                 }


         }
         }

        data=data*((float)amount/count);
         n->setPixel(x,y,data);

     }
   }

  /*  int amount=9;
    float kernel[amount][amount];
    for(int x=0;x<amount;x++){
        for(int y=0;y<amount;y++){
            kernel[x][y]=0;
        }

    }
  if(m_filterParameters.motionBlur_direction==0){
      for(int x=0;x<amount;x++){
          kernel[x][amount/2]=1.0f/amount;


      }
  }else if(m_filterParameters.motionBlur_direction==1){
      for(int x=0;x<amount;x++){
          kernel[amount/2][x]=1.0f/amount;


      }
  }else if(m_filterParameters.motionBlur_direction==2){
      for(int x=0;x<amount;x++){
          kernel[x][amount-x-1]=1.0f/amount;


      }
  }else{
      for(int x=0;x<amount;x++){
          kernel[x][x]=1.0f/amount;


      }
  }

for(int i=0;i<m_filterParameters.motionBlur_amount;i++){
   PixelBuffer *n=new PixelBuffer(m_displayBuffer->getWidth(),m_displayBuffer->getHeight(),m_displayBuffer->getBackgroundColor());
   int width=m_displayBuffer->getWidth();
   int height=m_displayBuffer->getHeight();
   for(int y=0;y<height;y++){
     for (int x=0;x<width;x++){

         ColorData data=ColorData(0,0,0);

       int count=0;
         for(int yy=0;yy<amount;yy++){
             for(int xx=0;xx<amount;xx++){

                      if(((x+xx-amount/2)<width&&(x+xx-amount/2>=0))&&((yy+y-amount/2)<height&&(y+yy-amount/2>=0))){
                     data=data+m_displayBuffer->getPixel(x+xx-amount/2,yy+y-amount/2)*kernel[yy][xx];
                     if(kernel[yy][xx]){
               count=count+1;
                 }
                 }else{

                 }


         }
         }

        data=data*((float)amount/count);
         n->setPixel(x,y,data);

     }
   }*/
   m_displayBuffer=n;

    cout << "Apply has been clicked for Sharpen with amount = " <<amount
    << " and direction " <<kernel[0][0] << endl;
}

void FlashPhotoApp::applyFilterEdgeDetect() {
	float kernel[3][3];
	 kernel[0][0]=-1; kernel[0][1]=-1;kernel[0][2]=-1;
    kernel[1][0]=-1 ;kernel[1][1]=8; kernel[1][2]=-1;
    kernel[2][0]=-1;  kernel[2][1]=-1; kernel[2][2]=-1;
   
    PixelBuffer *n=new PixelBuffer(m_displayBuffer->getWidth(),m_displayBuffer->getHeight(),m_displayBuffer->getBackgroundColor());
    int width=m_displayBuffer->getWidth();
    int height=m_displayBuffer->getHeight();
    for(int y=0;y<height;y++){
      for (int x=0;x<width;x++){
          ColorData data=ColorData(0,0,0);
          for(int yy=-1;yy<2;yy++){
			  for(int xx=-1;xx<2;xx++){
				  if(((x+xx)<width&&(x+xx>=0))&&((yy+y)<height&&(y+yy>=0))){
					  data=data+m_displayBuffer->getPixel(x+xx,yy+y)*kernel[yy+1][xx+1];
					  
				  }else{
					  ColorData u=ColorData(1,1,1);
					   data=data+u*kernel[yy+1][xx+1];
				  }
				  
			  }
			  
		  }
          n->setPixel(x,y,data);
        }
      }
    
    m_displayBuffer=n;

    cout << "Apply has been clicked for Edge Detect" << endl;
}

void FlashPhotoApp::applyFilterQuantize() {
    //cout << "Apply has been clicked for Quantize with bins = " << m_filterParameters.quantize_bins << endl;
    for (int y=0;y<m_displayBuffer->getHeight();y++){
      for(int x=0;x<m_displayBuffer->getWidth();x++){
        ColorData color=m_displayBuffer->getPixel(x,y);
        float new_red=0.0;
        float new_green=0.0;
        float new_blue=0.0;
        float min_distance=1.0f/(m_filterParameters.quantize_bins-1)/2.0f;
        for(float x=0.0;x<1+1.0/(m_filterParameters.quantize_bins-1);x+=1.0/(m_filterParameters.quantize_bins-1)){
          if (std::abs(color.getRed()-x)<=min_distance) {new_red=x;}
          if (std::abs(color.getGreen()-x)<=min_distance) {new_green=x;}
          if (std::abs(color.getBlue()-x)<=min_distance) {new_blue=x;}
        }
        color.setRed(new_red);
        color.setGreen(new_green);
        color.setBlue(new_blue);
        m_displayBuffer->setPixel(x,y,color);
      }
    }
}

void FlashPhotoApp::applyFilterSpecial() {
    //cout << "Apply has been clicked for Special" << endl;
    //darken
     float kernel[3][3];
	 kernel[0][0]=-1; kernel[0][1]=-1;kernel[0][2]=0.0;
    kernel[1][0]=-1; kernel[1][1]=0; kernel[1][2]=1;
    kernel[2][0]=0.0;  kernel[2][1]=1; kernel[2][2]=1;

    PixelBuffer *n=new PixelBuffer(m_displayBuffer->getWidth(),m_displayBuffer->getHeight(),m_displayBuffer->getBackgroundColor());
    int width=m_displayBuffer->getWidth();
    int height=m_displayBuffer->getHeight();
    for(int y=1;y<height-1;y++){
      for (int x=1;x<width-1;x++){
		  
          ColorData data=ColorData(0,0,0);
          
        int count=0;
          for(int yy=-1;yy<2;yy++){
			  for(int xx=-1;xx<2;xx++){
				
					  
					  data=data+m_displayBuffer->getPixel(x+xx,yy+y)*kernel[yy+1][xx+1];
					
				
			
		  }
		  }
		ColorData bias=ColorData(0.5,0.5,0.5);
		 data=data+bias;
          n->setPixel(x,y,data);
       
      }
    }
    
    m_displayBuffer=n;

}
void FlashPhotoApp::pushStack(PixelBuffer** ff){
	for(int i=1;i<20;i++){
		ff[i-1]=new PixelBuffer(ff[i]->getWidth(),ff[i]->getHeight(),ff[i]->getBackgroundColor());
		PixelBuffer::copyPixelBuffer(ff[i],ff[i-1]);
	}
	
}
void FlashPhotoApp::undoOperation()
{
	
	if(m_undo>=0){
		
		m_redo++;
		if(m_redo==20){
			pushStack(redo);
			m_redo=19;
			
		}
		redo[m_redo]=new PixelBuffer(m_displayBuffer->getWidth(),m_displayBuffer->getHeight(),m_displayBuffer->getBackgroundColor());
    PixelBuffer::copyPixelBuffer(m_displayBuffer,redo[m_redo]);
    m_displayBuffer=new PixelBuffer(undo[m_undo]->getWidth(),undo[m_undo]->getHeight(),undo[m_undo]->getBackgroundColor());
    PixelBuffer::copyPixelBuffer(undo[m_undo],m_displayBuffer);
		delete undo[m_undo];
		m_undo--;
	}
    cout << "Undoing..." << endl;
}

void FlashPhotoApp::redoOperation()
{
	if(m_redo>=0){
		 cout << "Redoing..." << endl;
		
		m_undo++;
		if(m_undo==20){
			pushStack(undo);
			m_undo=19;
			
		}
		undo[m_undo]=new PixelBuffer(m_displayBuffer->getWidth(),m_displayBuffer->getHeight(),m_displayBuffer->getBackgroundColor());
    PixelBuffer::copyPixelBuffer(m_displayBuffer,undo[m_undo]);
    m_displayBuffer=new PixelBuffer(redo[m_redo]->getWidth(),redo[m_redo]->getHeight(),redo[m_redo]->getBackgroundColor());
    PixelBuffer::copyPixelBuffer(redo[m_redo],m_displayBuffer);
		delete redo[m_redo];
		m_redo--;
	}
   
}
// ** END OF CALLBACKS **
// **********************


// **********************
// Provided code for managing the
// GLUI interface.

void FlashPhotoApp::buttonEnabled(GLUI_Button * button, bool enabled) {
    if(enabled) button->enable();
    else button->disable();
    button->redraw();
}

void FlashPhotoApp::redoEnabled(bool enabled)
{
    buttonEnabled(m_gluiControlHooks.redoButton, enabled);
}

void FlashPhotoApp::undoEnabled(bool enabled)
{
    buttonEnabled(m_gluiControlHooks.undoButton, enabled);
}

void FlashPhotoApp::saveCanvasEnabled(bool enabled)
{
    buttonEnabled(m_gluiControlHooks.saveCanvasButton, enabled);
}

void FlashPhotoApp::loadStampEnabled(bool enabled)
{
    buttonEnabled(m_gluiControlHooks.loadStampButton, enabled);
}

void FlashPhotoApp::loadCanvasEnabled(bool enabled)
{
    buttonEnabled(m_gluiControlHooks.loadCanvasButton, enabled);
}

void FlashPhotoApp::updateColors() {
    m_gluiControlHooks.spinnerBlue->set_float_val(m_curColorBlue);
    m_gluiControlHooks.spinnerGreen->set_float_val(m_curColorGreen);
    m_gluiControlHooks.spinnerRed->set_float_val(m_curColorRed);
}

bool FlashPhotoApp::hasSuffix(const std::string & str, const std::string & suffix){
    return str.find(suffix,str.length()-suffix.length()) != std::string::npos;
}


bool FlashPhotoApp::isValidImageFileName(const std::string & name) {

    if (hasSuffix(name, ".png")
        || hasSuffix(name, ".jpg")
        || hasSuffix(name, ".jpeg")
        )
        return true;
    else
        return false;
}

bool FlashPhotoApp::isValidImageFile(const std::string & name) {

    FILE *f;
    bool isValid = false;
    if (isValidImageFileName(name)) {
        if ((f = fopen( name.c_str(), "r"))) {
            isValid = true;
            fclose(f);
        }
    }
    return isValid;
}

void FlashPhotoApp::setImageFile(const std::string & fileName)
{
    // If a directory was selected
    // instead of a file, use the
    // latest file typed or selected.
    std::string imageFile = fileName;
    if (!isValidImageFileName(imageFile)) {
        imageFile = m_fileName;
    }


    // TOGGLE SAVE FEATURE
    // If no file is selected or typed,
    // don't allow file to be saved. If
    // there is a file name, then allow
    // file to be saved to that name.

    if (!isValidImageFileName(imageFile)) {
        m_gluiControlHooks.saveFileLabel->set_text("Will save image: none");
        saveCanvasEnabled(false);
    } else {
        m_gluiControlHooks.saveFileLabel->set_text((std::string("Will save image: ") + imageFile).c_str());
        saveCanvasEnabled(true);
    }

    // TOGGLE LOAD FEATURE

    // If the file specified cannot be opened,
    // then disable stamp and canvas loading.
    if (isValidImageFile(imageFile)) {
        loadStampEnabled(true);
        loadCanvasEnabled(true);

        m_gluiControlHooks.currentFileLabel->set_text((std::string("Will load: ") + imageFile).c_str());
        m_gluiControlHooks.fileNameBox->set_text(imageFile);
    } else {
        loadStampEnabled(false);
        loadCanvasEnabled(false);
        m_gluiControlHooks.currentFileLabel->set_text("Will load: none");
    }
}

void FlashPhotoApp::initGraphics() {
	// Initialize OpenGL for 2D graphics as used in the BrushWork app
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluOrtho2D(0, m_width, 0, m_height);
	glViewport(0, 0, m_width, m_height);
}
