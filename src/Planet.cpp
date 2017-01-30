//
//  Planet.cpp
//  no_sound_in_space
//
//  Created by David Richter on 1/28/17.
//
//

#include <glm/gtc/random.hpp>

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
    planetTexture = gl::Texture::create( loadImage( loadAsset( "moon_map.jpg" ) ),
                                        gl::Texture::Format().mipmap() );
    
    auto sphere = geom::Sphere().subdivisions( 30 );
    planetBatch = gl::Batch::create( sphere, mGlsl );
    
    planetRadius   = vec3( planetRadiusX, planetRadiusX * glm::linearRand(-0.33, 0.33), 
                          planetRadiusZ );
    planetPosition = vec3( 0, 0, 0 );
    diameter       = planetDiameter;
    theta          = 0.0f;
    orbitspeed     = glm::linearRand( 0.005, 0.02 );
}

void Planet::update() 
{
    theta    += orbitspeed;
    rotation *= rotate( toRadians( 0.2f ), normalize( vec3( 0, 1, 0 ) ) );
    
    planetPosition.x = planetRadius.x * sin( theta );
    planetPosition.y = planetRadius.y * sin( theta );
    planetPosition.z = planetRadius.z * cos( theta );
}

void Planet::display()
{
    gl::pushMatrices();
        gl::translate( planetPosition );
        gl::multModelMatrix( rotation );
        gl::scale( vec3( 1 ) );
        planetTexture->bind();
        planetBatch->draw();
    gl::popMatrices();
}















































