#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/params/Params.h"
#include "cinder/CameraUi.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class no_sound_in_spaceApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
    void resize() override;
    
    // for default camera values
    void setDefaultCameraValues();
    
    CameraPersp     mCam;      // Operating camera
    CameraPersp     mCamInit;  // Camera initializer
    CameraUi        mCamUi;
    
    // params for operating camera
    params::InterfaceGlRef mParams;
    vec3                   mEyePoint;
    vec3                   mLookAt;
    float                  mFov;
    float                  mAspectRatio;
    float                  mNearPlane;
    float                  mFarPlane;
    vec2                   mLensShift;
    
    // Shapes and textures - CLEAN UP
    gl::TextureCubeMapRef mCubeMap;
    gl::BatchRef    planetBatch, skyBoxBatch;
    gl::TextureRef  sunTex;
    gl::GlslProgRef mGlsl;
    mat4            rotation;
};

// Const for sky box, this probably needs to go somewhere else
const int SKY_BOX_SIZE = 500;

void no_sound_in_spaceApp::setup()
{
    setWindowSize( 1280, 720 );
    
    gl::enableDepthRead();
    gl::enableDepthWrite();
    gl::enableAlphaBlending();
    
    // Reflective Texture
    mGlsl = gl::GlslProg::create( loadAsset( "shader.vert" ), 
                                 loadAsset( "shader.frag" ) );
     
    
    // For sky box
    mCubeMap = gl::TextureCubeMap::create( loadImage( loadAsset( "space_2.png" ) ),
                                          gl::TextureCubeMap::Format().mipmap() );
    
#if defined( CINDER_GL_ES )
    auto envMapGlsl = gl::GlslProg::create( loadAsset( "env_map_es2.vert" ), loadAsset( "env_map_es2.frag" ) );
    auto skyBoxGlsl = gl::GlslProg::create( loadAsset( "sky_box_es2.vert" ), loadAsset( "sky_box_es2.frag" ) );
#else
    auto envMapGlsl = gl::GlslProg::create( loadAsset( "env_map.vert" ), loadAsset( "env_map.frag" ) );
    auto skyBoxGlsl = gl::GlslProg::create( loadAsset( "sky_box.vert" ), loadAsset( "sky_box.frag" ) );
#endif
    
//    planetBatch = gl::Batch::create( geom::Sphere(), envMapGlsl );
//    planetBatch->getGlslProg()->uniform( "uCubeMapTex", 0 );
    
    skyBoxBatch = gl::Batch::create( geom::Cube(), skyBoxGlsl );
    skyBoxBatch->getGlslProg()->uniform( "uCubeMapTex", 0 );
    
    auto sphere = geom::Sphere().subdivisions( 30 );
    planetBatch = gl::Batch::create( sphere, mGlsl );
    
    sunTex = gl::Texture::create( loadImage( loadAsset( "jupiter_map.jpg" ) ), 
                                  gl::Texture::Format().mipmap() );
    sunTex->bind();
    
    
}

void no_sound_in_spaceApp::resize()
{
    mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
}

void no_sound_in_spaceApp::mouseDown( MouseEvent event )
{
}

void no_sound_in_spaceApp::update()
{
    // .lookAt( vec3(&eyepoint), vec3( target ) )
    mCam.lookAt( vec3( 3, 0, 10 ), vec3( 0 ) );
    
//    mCam.lookAt( vec3( 8 * sin( theta / 1 + 10 ), 7 * 
//                      cos( theta / 2 ), 8 * 
//                      cos( theta / 4 + 11 ) ), vec3( 0 ) );
    
    
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
        gl::scale( vec3( 2 ) );
        planetBatch->draw();
    gl::popMatrices();
    
    // draw sky box
    gl::pushMatrices();
        gl::scale( SKY_BOX_SIZE, SKY_BOX_SIZE, SKY_BOX_SIZE );
        skyBoxBatch->draw();
    gl::popMatrices();
}

CINDER_APP( no_sound_in_spaceApp, RendererGl )










































