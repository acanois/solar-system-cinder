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

Planet::Planet( float planetRadiusX, float planetRadiusZ, size_t planetId )
{
    init(planetRadiusX, planetRadiusZ, planetId);
}

Planet::~Planet() 
{
    
}

void Planet::init( float planetRadiusX, float planetRadiusZ, size_t planetId )
{
    pId = planetId;
    
    pTexture = {
                    "mercury_map.jpg",
                    "venus_map.jpg",
                    "earth_map.jpg",
                    "mars_map.jpg",
                    "jupiter_map.jpg",
                    "saturn_map.jpg",
                    "uranus_map.jpg",
                    "neptune_map.jpg",
                    "moon_map.jpg"
                };
    
    // Multipliers for planet diameter. Earth = 1.
    // Info taken from a NASA website
    pDiameter = {
                    0.383, // mercury
                    0.949, // venus
                    1,     // earth
                    0.532, // mars
                    5.605, // jupiter
                    4.725, // saturn
                    2.005, // uranus
                    1.94,  // neptune
                    0.186, // pluto
                };
    
    // Load shaders
    mGlsl = gl::GlslProg::create( loadAsset( "shader_normal.vert" ),
                                  loadAsset( "shader_normal.frag" ) );
    planetTexture = gl::Texture::create( loadImage( loadAsset( pTexture[pId] ) ),
                                         gl::Texture::Format().mipmap() );
    
    auto sphere = geom::Sphere().subdivisions( 30 );
    planetBatch = gl::Batch::create( sphere, mGlsl );
    
    planetRadius   = vec3( planetRadiusX, planetRadiusX * glm::linearRand(-0.33, 0.33), 
                          planetRadiusZ );
    planetPosition = vec3( 0, 0, 0 );
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
        gl::scale( vec3( pDiameter[pId] ) );
        planetTexture->bind();
        planetBatch->draw();
    gl::popMatrices();
}















































