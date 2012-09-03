#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Resources.h"
#include <cmath>
#include <vector>

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

		struct circle{
			int x;
			int y;
			int r;
		};

		struct square{
			int x;
			int y;
			int w;
		};

		deque<square> squares_;
		deque<circle> circles_;

	
	//Width and height of the screen
	static const int kAppWidth=800;
	static const int kAppHeight=600;
	static const int kTextureSize=1024; //Must be the next power of 2 bigger or equal to app dimensions

	void drawGradient(uint8_t* pixels, int shift);
	void drawSquare(uint8_t* pixels, square s, Color8u c);

};

void homeWork1Phase2App::prepareSettings(Settings* settings){
	(*settings).setWindowSize(kAppWidth,kAppHeight);
	(*settings).setResizable(false);
}

void homeWork1Phase2App::setup()
{
		surface_ = new Surface(kTextureSize,kTextureSize,false);
		numFrames = 0;
		square test;
		
		for(int i=0; i<20; i++){
			test.x = 20+i*10;
			test.y = 40+i*10;
			test.w = 20;
			squares_.push_back(test); 
		}
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

void homeWork1Phase2App::drawSquare(uint8_t* pixels, square s, Color8u c){
	if(s.w<=0) return;

	for(int y=s.y; y<=s.y+s.w; y++){
		for(int x=s.x; x<=s.x+s.w; x++){
			if(y < 0 || x < 0 || x >= kAppWidth || y >= kAppHeight) continue;

			if(y==s.y||y==s.y+s.w||x==s.x||x==s.x+s.w){
				pixels[3*(x + y*kTextureSize)] = c.r;
				pixels[3*(x + y*kTextureSize)+1] = c.g;
				pixels[3*(x + y*kTextureSize)+2] = c.b;
			}
		}
	}
}

void homeWork1Phase2App::update()
{
	uint8_t* dataArray = (*surface_).getData();

	drawGradient(dataArray, (numFrames*5)%kAppHeight);

	for(int i=0; i<squares_.size(); i++){
		drawSquare(dataArray, squares_[i], Color8u(0, 255, 0));
	}

	numFrames++;
}

void homeWork1Phase2App::draw()
{
	// clear out the window with black
	gl::draw(*surface_);
}



CINDER_APP_BASIC( homeWork1Phase2App, RendererGl )
