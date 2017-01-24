#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Easing.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class no_sound_in_spaceApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
    
    CameraPersp mCam;
    // gl::BatchRef mSphere;
    gl::BatchRef mShape;
    
    float theta;
};

void no_sound_in_spaceApp::setup()
{
    auto lambert = gl::ShaderDef().lambert().color();
    gl::GlslProgRef shader = gl::getStockShader( lambert );
    // mCam.lookAt( vec3( 3, 3, 3 ), vec3( 0 ) );
    
    // auto sphere = geom::Sphere().subdivisions( 30 );
    // mSphere = gl::Batch::create( sphere, shader );
    auto icosahedron = geom::Icosahedron();
    mShape = gl::Batch::create( icosahedron, shader );
    
    theta = 0;
    
}

void no_sound_in_spaceApp::mouseDown( MouseEvent event )
{
}

void no_sound_in_spaceApp::update()
{
    theta += 0.01;
}

void no_sound_in_spaceApp::draw()
{   
    // .lookAt( vec3(&eyepoint), vec3( target ) )
    mCam.lookAt( vec3( 3, 0, 10 ), vec3( 0 ) );
    
    
    // This does some neat stuff
    // float angle = easeInOutQuint( theta ) * M_PI / 2.0f;
    
	gl::clear( Color( 0, 0, 0 ) ); 
    gl::enableDepthRead();
    gl::enableDepthWrite();
    
    gl::setMatrices( mCam );
    
    gl::color( 0.2, 0.7, 0.2 );
    
    gl::rotate( angleAxis( theta, vec3( 0, 1, 0 ) ) );
    // mSphere->draw();
    mShape->draw();
}

CINDER_APP( no_sound_in_spaceApp, RendererGl )
