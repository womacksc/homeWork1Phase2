/**
* @file homeWork1Phase.cpp
* CSE 274 - Fall 2012
*
* @author Harrison Bourne
* @date 2012-09-04
*
* @note This project satisfies goals A.1 (rectangle), A.2 (circle), A.4 (gradient),
* A.6 (tint) B.1 (blur), E.5 (animation) and E.6 (mouse interaction)
*/

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Resources.h"
#include <cmath>
#include <vector>
#include <time.h>

using namespace ci;
using namespace ci::app;
using namespace std;

class homeWork1Phase2App : public AppBasic {
  public:
	void setup();
	void update();
	void draw();
    void prepareSettings(Settings* settings);
	/**
	 *Inside this function is the code wich creates the squares and circles
	 *
	 *This satisfies the "mouse interaction" requirement, goal E.6
	 */
	void mouseDrag( MouseEvent event );
	/**
	 *Inside this function is the code which resizes the shapes and which
	 *changes the tint of the window.
	 *
	 *This satisfies the "tint" requirement, goal A.6
	 */
	void mouseWheel( MouseEvent event );
	/**
	 *Inside this function is the code which turns bluring  on and off.
	 */
	void keyDown( KeyEvent event );

	private:
		//Surface object pixel array
		Surface* surface_;
		//Number of frames that have been shown (for animation)
		int numFrames_;
		//Size of shapes to draw.
		int drawSize_;
		//Keep track of mouse location
		int mouseX_, mouseY_;
		//Keep track of if blur is on or off.
		bool blurFlag_;
		Color8u squareColor_;
		Color8u circleColor_;
		//amount of green tint to use
		uint8_t tintValue_;

		//Structure to hold circle information
		struct circle{
			int x;
			int y;
			int r;
		};

		//Structure to hold square information
		struct square{
			int x;
			int y;
			int w;
		};

		//Keep track of squares drawn
		deque<square> squares_;
		//Keep track of circles drawn
		deque<circle> circles_;

	
	//Width and height of the screen
	static const int kAppWidth=800;
	static const int kAppHeight=600;
	static const int kTextureSize=1024; //Must be the next power of 2 bigger or equal to app dimensions

	/**
	 *Draw a red and blue gradient with sifted by a given ammout.
	 *
	 *This satisfies the "gradient" requirment, goal A.4 and "animation", goal E.5
	 */
	void drawGradient(uint8_t* pixels, int shift);
	/**
	 *Draw a square with center and size given by s in the color given by c.
	 *
	 *This satisfies the "rectangle" requirement, goal A.1
	 */
	void drawSquare(uint8_t* pixels, square s, Color8u c);
	/**
	 *Draw a cricle with center and size given by cir in the color given by c.
	 *
	 *This satisfies the "circle" requirment, goal A.2
	 */
	void drawCircle(uint8_t* pixels, circle cir, Color8u c);
	/**
	 *Blurs the entire window
	 *
	 *This satisfies the "blur" requirement, goal B.1
	 */
	void blur(uint8_t* pixels);

};

void homeWork1Phase2App::prepareSettings(Settings* settings){
	(*settings).setWindowSize(kAppWidth,kAppHeight);
	(*settings).setResizable(false);
}

void homeWork1Phase2App::setup()
{		surface_ = new Surface(kTextureSize,kTextureSize,false);
		numFrames_ = 0;
		drawSize_ = 50;
		srand ( time(NULL) );
		squareColor_ = Color8u(255*rand(),255*rand(),255*rand()) ;
		circleColor_ = Color8u(255*rand(),255*rand(),255*rand()) ;
		tintValue_ = 0;
		blurFlag_ = false;
		mouseX_ = -100; 
		mouseY_ = -100;
}		  

void homeWork1Phase2App::mouseDrag( MouseEvent event )
{	square s;
	circle cir;
	int const knumShapes = 50; 
	int const minMove = 10;

	if(event.isLeftDown()){
			s.x = event.getX();
			s.y = event.getY();
		s.w = drawSize_;
		if(abs(event.getX()-mouseX_)>minMove||abs(event.getY()-mouseY_)>minMove){
			squares_.push_back(s); 
			mouseY_ = s.y;
			mouseX_ = s.x;
		}
		if(squares_.size()>knumShapes)
			squares_.pop_front();
}

	if(event.isRightDown()){
		cir.x = event.getX();
		cir.y = event.getY();
		cir.r = drawSize_;
		if(abs(event.getX()-mouseX_)>minMove||abs(event.getY()-mouseY_)>minMove){
			circles_.push_back(cir);
			mouseY_ = cir.y;
			mouseX_ = cir.x;
		}
		if(circles_.size()>knumShapes)
			circles_.pop_front();
	}
}

void homeWork1Phase2App::mouseWheel( MouseEvent event ){
	float wheel = event.getWheelIncrement();
	int temp;
	
	if(event.isShiftDown()){
		temp = ((int)tintValue_)+((int)wheel)*5;
		if((temp<255&&wheel>0)||(temp>0&&wheel<0))
			tintValue_ = (uint8_t)temp;
	squareColor_.g = tintValue_;
	circleColor_.g = tintValue_;
	}
	else
		drawSize_ +=((int)wheel)*10;
}

void homeWork1Phase2App::keyDown( KeyEvent event ) {
    if( event.getChar() == 'b' )
		blurFlag_ = !blurFlag_;
}

void homeWork1Phase2App::drawGradient(uint8_t* pixels, int shift){
	Colorf red = Colorf(1,0,0);
	Colorf blue = Colorf(0,0,1);

	Color8u lineColor;
	lineColor.g = tintValue_;
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

void homeWork1Phase2App::drawCircle(uint8_t* pixels, circle cir, Color8u c){
	int function;
	if(cir.r<=0) return;

	for(int y=cir.y-cir.r; y<=cir.y+cir.r; y++){
		for(int x=cir.x-cir.r; x<=cir.x+cir.r; x++){
			if(y < 0 || x < 0 || x >= kAppWidth || y >= kAppHeight) continue;
			function = (int)sqrt((double)((x-cir.x)*(x-cir.x) + (y-cir.y)*(y-cir.y)));
			if(function<= cir.r){
				if(function>cir.r/2-1 && function<cir.r/2+1){
					pixels[3*(x + y*kTextureSize)] = c.r;
					pixels[3*(x + y*kTextureSize)+1] = c.g;
					pixels[3*(x + y*kTextureSize)+2] = c.b;
				}
			}
		}
	}
}

void homeWork1Phase2App::blur(uint8_t* pixels){
	//Following 2 lines stolen from Dr. Brinkman
	static uint8_t workCopy[3*kTextureSize*kTextureSize];
	memcpy(workCopy,pixels,3*kTextureSize*kTextureSize);

	int offset[9];

	for(int y=1;y<kAppHeight-1;y++){
		for(int x=1;x<kAppWidth-1;x++){
			offset[0] = 3*((x-1) + (y-1)*kTextureSize);
			offset[1] = 3*(x + (y-1)*kTextureSize);
			offset[2] = 3*((x+1) + (y-1)*kTextureSize);
			offset[3] = 3*((x-1) + y*kTextureSize);
			offset[4] = 3*(x + y*kTextureSize);
			offset[5] = 3*((x+1) + y*kTextureSize);
			offset[6] = 3*((x-1) + (y+1)*kTextureSize);
			offset[7] = 3*(x + (y+1)*kTextureSize);
			offset[8] = 3*((x+1) + (y+1)*kTextureSize);

			pixels[offset[4]] = (workCopy[offset[0]]+workCopy[offset[1]]+workCopy[offset[2]]+
				workCopy[offset[3]]+workCopy[offset[4]]+workCopy[offset[5]]+
				workCopy[offset[6]]+workCopy[offset[7]]+workCopy[offset[8]])/9;

			pixels[offset[4]+1] = (workCopy[offset[0]+1]+workCopy[offset[1]+1]+workCopy[offset[2]+1]+
				workCopy[offset[3]+1]+workCopy[offset[4]+1]+workCopy[offset[5]+1]+
				workCopy[offset[6]+1]+workCopy[offset[7]+1]+workCopy[offset[8]+1])/9;

			pixels[offset[4]+2] = (workCopy[offset[0]+2]+workCopy[offset[1]+2]+workCopy[offset[2]+2]+
				workCopy[offset[3]+2]+workCopy[offset[4]+2]+workCopy[offset[5]+2]+
				workCopy[offset[6]+2]+workCopy[offset[7]+2]+workCopy[offset[8]+2])/9;
		}
	}
}

void homeWork1Phase2App::update()
{	
	uint8_t* dataArray = (*surface_).getData();

	drawGradient(dataArray, (numFrames_*5)%kAppHeight);

	for(uint8_t i=0; i<squares_.size(); i++){
		drawSquare(dataArray, squares_[i], squareColor_);
	}

	for(uint8_t i=0; i<circles_.size(); i++){
		drawCircle(dataArray, circles_[i], circleColor_);
	}

	if(blurFlag_)
		blur(dataArray);

	numFrames_++;
}

void homeWork1Phase2App::draw()
{
	gl::draw(*surface_);
}



CINDER_APP_BASIC( homeWork1Phase2App, RendererGl )
