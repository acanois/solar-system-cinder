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
    
    CameraPersp     mCam;
    gl::BatchRef    sun;
    gl::TextureRef  sunTex;
    gl::GlslProgRef mGlsl;
    mat4            rotation;
};

void no_sound_in_spaceApp::setup()
{
    mGlsl = gl::GlslProg::create( loadAsset( "shader.vert" ), 
                                 loadAsset( "shader.frag" ) );
    
    sunTex = gl::Texture::create( loadImage( loadAsset( "jupiter_map.jpg" ) ), 
                                  gl::Texture::Format().mipmap() );
    sunTex->bind();
    
    auto sphere = geom::Sphere();
    sun = gl::Batch::create( sphere, mGlsl );
    
}

void no_sound_in_spaceApp::mouseDown( MouseEvent event )
{
}

void no_sound_in_spaceApp::update()
{
    rotation *= rotate( toRadians( 0.2f ), normalize( vec3( 0, 1, 0 ) ) );
}

void no_sound_in_spaceApp::draw()
{   
    // .lookAt( vec3(&eyepoint), vec3( target ) )
    mCam.lookAt( vec3( 3, 0, 10 ), vec3( 0 ) );
    
    
    // This does some neat stuff
    // float angle = easeInOutQuint( theta ) * M_PI / 2.0f;
    
	gl::clear( Color( 0, 0, 0 ) ); 
    
    gl::ScopedModelMatrix modelScope;
    gl::enableDepthRead();
    gl::enableDepthWrite();
    
    gl::setMatrices( mCam );
    
    gl::multModelMatrix( rotation );
    
    sun->draw();
}

CINDER_APP( no_sound_in_spaceApp, RendererGl )
