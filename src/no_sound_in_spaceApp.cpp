#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class no_sound_in_spaceApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void no_sound_in_spaceApp::setup()
{
}

void no_sound_in_spaceApp::mouseDown( MouseEvent event )
{
}

void no_sound_in_spaceApp::update()
{
}

void no_sound_in_spaceApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( no_sound_in_spaceApp, RendererGl )
