#include <array>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIo.h"
#include "cinder/params/Params.h"
#include "cinder/CameraUi.h"

// For planet class
#include "Planet.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class no_sound_in_spaceApp : public App {
  public:
    static void prepareSettings( Settings *settings );
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
    void resize() override;
    
    // for default camera values
    void setDefaultCameraValues();
    
    static const size_t    FBO_WIDTH = 1280, FBO_HEIGHT = 720;
    static const size_t    NUM_PLANETS = 9;
    
    gl::FboRef             mObjectFbo;
    CameraPersp            mCam;      // Operating camera
    CameraPersp            mCamInit;  // Camera initializer
    CameraUi               mCamUi;
    
    array<Planet, NUM_PLANETS> planet;
    
    float theta;
    
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
    gl::BatchRef          sunBatch, skyBoxBatch;
    gl::TextureRef        sunTex;
    gl::TextureRef        diffuseTex, normalTex;
    gl::GlslProgRef       mGlsl;
    mat4                  rotation;
    
    vec3                  lightPosition;
};

// Const for sky box, this probably needs to go somewhere else
const size_t SKY_BOX_SIZE = 500;
const size_t SUN_DIAMETER = 19; // 1/10th - fullsize = 109, set camEye Z to 250

void no_sound_in_spaceApp::prepareSettings( Settings *settings ) 
{
    settings->setResizable( false );
}

void no_sound_in_spaceApp::setup()
{
    setWindowSize( 1280, 720 );
    
    gl::enableDepthRead();
    gl::enableDepthWrite();
    gl::enableAlphaBlending();
    
    setDefaultCameraValues();
    
    theta = 0;
    
    for ( size_t i = 0; i < NUM_PLANETS; i++ ) {
        planet[i] = Planet( ( SUN_DIAMETER + 5 ) + i * 10, ( SUN_DIAMETER + 5 ) + i * 10, i );
    }
    
    // FBO
    // gl::Fbo::Format format;
    // mObjectFbo = gl::Fbo::create( FBO_WIDTH, FBO_HEIGHT, format.depthTexture() );
    
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
    sunBatch    = gl::Batch::create( sphere, mGlsl );
    
    // Textures
        
    sunTex = gl::Texture::create( loadImage( loadAsset( "sun_map_orange.jpg" ) ), 
                                  gl::Texture::Format().mipmap() );
//    
    // UI Window
    mParams = params::InterfaceGl::create( getWindow(), "CameraPersp", 
                                          toPixels( ivec2( 200, 300 ) ) );
    
    mParams->addSeparator();
    mParams->addSeparator();
    mParams->addParam( "Eye Point X", &mEyePoint.x ).step( 0.01f );
    mParams->addParam( "Eye Point Y", &mEyePoint.y ).step( 0.01f );
    mParams->addParam( "Eye Point Z", &mEyePoint.z ).step( 0.01f );
    mParams->addSeparator();
    mParams->addParam( "Look At X", &mLookAt.x ).step( 0.01f );
    mParams->addParam( "Look At Y", &mLookAt.y ).step( 0.01f );
    mParams->addParam( "Look At Z", &mLookAt.z ).step( 0.01f );
    mParams->addSeparator();
    mParams->addParam( "FOV", &mFov ).min( 1.0f ).max( 179.0f );
    mParams->addParam( "Near Plane", &mNearPlane ).step( 0.02f ).min( 0.1f );
    mParams->addParam( "Far Plane", &mFarPlane ).step( 0.02f ).min( 0.1f );
    mParams->addParam( "Lens Shift X", &mLensShift.x ).step( 0.01f );
    mParams->addParam( "Lens Shift Y", &mLensShift.y ).step( 0.01f );
    mParams->addSeparator();
    mParams->addButton( "Reset Defaults", 
                       bind ( &no_sound_in_spaceApp::setDefaultCameraValues, this ) );
    
    mCam.setEyePoint( vec3( 0.0f, 0.0f, 10.0f) );
    mCam.lookAt( vec3( 0 ) );
    mCamInit = mCam;
    mCamUi   = CameraUi( &mCam );
}

void no_sound_in_spaceApp::resize()
{
    mCam.setPerspective( 60, getWindowAspectRatio(), 1, 1000 );
}

void no_sound_in_spaceApp::mouseDown( MouseEvent event )
{
    mCamUi.mouseDown( event );
}

void no_sound_in_spaceApp::update()
{
    theta += 0.01;
    
    mCam.setEyePoint( mEyePoint );
    mCam.lookAt( mLookAt );
    mCam.setLensShift( mLensShift );
    mCam.setOrientation( quat(1, 0, 0, 0) );
    // mCam.setPerspective( mFov, mObjectFbo->getAspectRatio(), mNearPlane, mFarPlane );
    
    rotation *= rotate( toRadians( 0.2f ), normalize( vec3( 0, 1, 0 ) ) );
    
    for ( size_t i = 0; i < NUM_PLANETS; i++ ) {
        planet[i].update();
    }
}

void no_sound_in_spaceApp::setDefaultCameraValues()
{
    mEyePoint			= vec3( 0.0f, 0.0f, ( SUN_DIAMETER * 6.0f ) );
    mLookAt				= vec3( 0.0f );
    mFov				= 25.0f;
    mAspectRatio		= getWindowAspectRatio();
    mNearPlane			= 3.5f;
    mFarPlane			= 15.0f;
    mLensShift			= vec2 ( 0 );
    mCam			    = mCamInit;
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
    for ( size_t i = 0; i < NUM_PLANETS; i++ ) {
        planet[i].display();
    }
    
    gl::pushMatrices();
        gl::multModelMatrix( rotation );
    // This is 1/10th scale to the actual sun. But setting it to the full 109 is kinda fun.
        gl::scale( vec3( SUN_DIAMETER ) );
        sunTex->bind();
        sunBatch->draw();
    gl::popMatrices();
    
    // draw sky box
    gl::pushMatrices();
        gl::scale( SKY_BOX_SIZE, SKY_BOX_SIZE, SKY_BOX_SIZE );
        skyBoxBatch->draw();
    gl::popMatrices();
    
    // draw camera parameter window
    mParams->draw();
}

// RendererGl::Options().msaa( 16 ) = Multi-sample anti-aliasing( 16 samples )
CINDER_APP( no_sound_in_spaceApp, RendererGl( RendererGl::Options().msaa( 16 ) ) )










































