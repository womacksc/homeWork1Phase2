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
	
	//Width and height of the screen
	static const int kAppWidth=800;
	static const int kAppHeight=600;
	static const int kTextureSize=1024; //Must be the next power of 2 bigger or equal to app dimensions

	void drawGradient(uint8_t* pixels);

};

void homeWork1Phase2App::prepareSettings(Settings* settings){
	(*settings).setWindowSize(kAppWidth,kAppHeight);
	(*settings).setResizable(false);
}

void homeWork1Phase2App::setup()
{
		surface_ = new Surface(kTextureSize,kTextureSize,false);
}

void homeWork1Phase2App::mouseDown( MouseEvent event )
{	


}

void homeWork1Phase2App::drawGradient(uint8_t* pixels){
	Colorf current = Colorf(0,0,1);
	Colorf target = Colorf(0,1,0);
	Colorf lineColor = current;
	Color8u red = Color8u(255,0,0);
	float increment = 1.0f/((float)kAppHeight);

	for(int y=0; y<kAppHeight; y++){
		lineColor.r = (int)floor(current.r*y*increment*255+0.5)+floor(target.r*(kAppHeight-y)*increment*255+0.5);
		lineColor.g = (int)floor(current.g*y*increment*255+0.5)+floor(target.g*(kAppHeight-y)*increment*255+0.5);
		lineColor.b = (int)floor(current.b*y*increment*255+0.5)+floor(target.b*(kAppHeight-y)*increment*255+0.5);
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

	drawGradient(dataArray);
}

void homeWork1Phase2App::draw()
{
	// clear out the window with black
	gl::draw(*surface_);
}



CINDER_APP_BASIC( homeWork1Phase2App, RendererGl )
