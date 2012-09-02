#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class homeWork1Phase2App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void homeWork1Phase2App::setup()
{
}

void homeWork1Phase2App::mouseDown( MouseEvent event )
{
}

void homeWork1Phase2App::update()
{
}

void homeWork1Phase2App::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_BASIC( homeWork1Phase2App, RendererGl )
