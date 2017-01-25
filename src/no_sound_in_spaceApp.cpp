#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
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
    
    gl::TextureCubeMapRef mCubeMap;
    gl::BatchRef    planetBatch, skyBoxBatch;
    gl::TextureRef  sunTex;
    gl::GlslProgRef mGlsl;
    mat4            rotation;
    CameraPersp     mCam;
};

const int SKY_BOX_SIZE = 500;

void no_sound_in_spaceApp::setup()
{
    // For planet
    mGlsl = gl::GlslProg::create( loadAsset( "shader.vert" ), 
                                 loadAsset( "shader.frag" ) );
    
    // For sky box
    mCubeMap = gl::TextureCubeMap::create( loadImage( loadAsset( "space_orangey.jpg" ) ),
                                          gl::TextureCubeMap::Format().mipmap() );
    
    auto envMapGlsl = gl::GlslProg::create( loadAsset( "env_map.vert" ), 
                                           loadAsset("env_map.frag" ) );
    auto skyBoxGlsl = gl::GlslProg::create( loadAsset( "sky_box.vert" ), 
                                           loadAsset( "sky_box.frag" ) );
    
    planetBatch = gl::Batch::create( geom::Sphere(), envMapGlsl );
    planetBatch->getGlslProg()->uniform( "uCubeMapTex", 0 );
    
    skyBoxBatch = gl::Batch::create( geom::Cube(), skyBoxGlsl );
    skyBoxBatch->getGlslProg()->uniform( "uCubeMapTex", 0 );
    
//    sunTex = gl::Texture::create( loadImage( loadAsset( "jupiter_map.jpg" ) ), 
//                                  gl::Texture::Format().mipmap() );
//    sunTex->bind();
    
    gl::enableDepthRead();
    gl::enableDepthWrite();
    
}

void no_sound_in_spaceApp::mouseDown( MouseEvent event )
{
}

void no_sound_in_spaceApp::update()
{
    // .lookAt( vec3(&eyepoint), vec3( target ) )
    mCam.lookAt( vec3( 3, 0, 10 ), vec3( 0 ) );
    
    rotation *= rotate( toRadians( 0.2f ), normalize( vec3( 0, 1, 0 ) ) );
}

void no_sound_in_spaceApp::draw()
{   
    // This does some neat stuff
    // float angle = easeInOutQuint( theta ) * M_PI / 2.0f;
    
    gl::clear( Color( 0, 0, 0 ) ); 
    gl::setMatrices( mCam );
    
    // I forgot this line because I am stupid
    mCubeMap->bind();
    // draw planet
    gl::pushMatrices();
        gl::multModelMatrix( rotation );
        planetBatch->draw();
    gl::popMatrices();
    
    // draw sky box
    gl::pushMatrices();
        gl::scale( SKY_BOX_SIZE, SKY_BOX_SIZE, SKY_BOX_SIZE );
        skyBoxBatch->draw();
    gl::popMatrices();
}

CINDER_APP( no_sound_in_spaceApp, RendererGl )










































