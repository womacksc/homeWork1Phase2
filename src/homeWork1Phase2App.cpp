#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Resources.h"
#include <cmath>

using namespace ci;
using namespace ci::app;
using namespace std;

class homeWork1Phase2App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    void prepareSettings(Settings* settings);

	private:
		Surface* surface_;
		int numFrames;
	
	//Width and height of the screen
	static const int kAppWidth=800;
	static const int kAppHeight=600;
	static const int kTextureSize=1024; //Must be the next power of 2 bigger or equal to app dimensions

	void drawGradient(uint8_t* pixels, int shift);

};

void homeWork1Phase2App::prepareSettings(Settings* settings){
	(*settings).setWindowSize(kAppWidth,kAppHeight);
	(*settings).setResizable(false);
}

void homeWork1Phase2App::setup()
{
		surface_ = new Surface(kTextureSize,kTextureSize,false);
		numFrames = 0;
}

void homeWork1Phase2App::mouseDown( MouseEvent event )
{	


}

void homeWork1Phase2App::drawGradient(uint8_t* pixels, int shift){
	Colorf red = Colorf(1,0,0);
	Colorf blue = Colorf(0,0,1);

	Colorf lineColor;
	int bla = kAppHeight/2;
	float increment = 255.0f/((float)bla);

	for(int y=0; y<kAppHeight; y++){
		lineColor.r = (y+shift+bla)%kAppHeight<bla ? (int)floor(red.r*((y+shift+bla)%kAppHeight)*increment*+0.5):
			(int)floor(red.r*((kAppHeight-((y+shift+bla)%kAppHeight))*increment*+0.5));

		lineColor.b = (y+shift)%kAppHeight<bla ? (int)floor(blue.b*((y+shift)%kAppHeight)*increment*+0.5):
			(int)floor(blue.b*((kAppHeight-((y+shift)%kAppHeight))*increment*+0.5));

		for(int x=0; x<kAppWidth; x++){
			pixels[3*(x + y*kTextureSize)] = lineColor.r;
			pixels[3*(x + y*kTextureSize)+1] = lineColor.g;
			pixels[3*(x + y*kTextureSize)+2] = lineColor.b;
		}
	}
}

void homeWork1Phase2App::update()
{
	uint8_t* dataArray = (*surface_).getData();

	drawGradient(dataArray, (numFrames*5)%kAppHeight);

	numFrames++;
}

void homeWork1Phase2App::draw()
{
	// clear out the window with black
	gl::draw(*surface_);
}



CINDER_APP_BASIC( homeWork1Phase2App, RendererGl )
