//
//  Planet.cpp
//  no_sound_in_space
//
//  Created by David Richter on 1/28/17.
//
//

#include "Planet.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

Planet::Planet() 
{
    
}

Planet::Planet( float planetRadiusX, float planetRadiusZ, size_t planetDiameter )
{
    init(planetRadiusX, planetRadiusZ, planetDiameter);
}

Planet::~Planet() 
{
    
}

void Planet::init( float planetRadiusX, float planetRadiusZ, size_t planetDiameter )
{
    // Load shaders
    mGlsl = gl::GlslProg::create( loadAsset( "shader.vert" ),
                                  loadAsset( "shader.frag" ) );
    planetTexture = gl::Texture::create( loadImage( loadAsset( "jupiter_map.jpg" ) ),
                                        gl::Texture::Format().mipmap() );
    planetTexture->bind();
    
    auto sphere = geom::Sphere().subdivisions( 30 );
    planetBatch = gl::Batch::create( sphere, mGlsl );
    
    planetRadius   = vec3( planetRadiusX, 0, planetRadiusZ );
    planetPosition = vec3( 0, 0, 0 );
    diameter       = planetDiameter;
    theta          = 0.0f;
    orbitspeed     = 0.01;
}

void Planet::update() 
{
    theta += orbitspeed;
    rotation *= rotate( toRadians( 0.2f ), normalize( vec3( 0, 1, 0 ) ) );
}

void Planet::display()
{
    gl::pushMatrices();
    gl::translate( planetPosition );
        gl::multModelMatrix( rotation );
        gl::scale( vec3( 2 ) );
        planetBatch->draw();
    gl::popMatrices();
}















































