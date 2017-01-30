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
    pDiameter = {
                    0.383,
                    0.949,
                    1,
                    0.532,
                    11.21,
                    9.45,
                    4.01,
                    3.88,
                    0.186,
                };
    
    // Load shaders
    mGlsl = gl::GlslProg::create( loadAsset( "shader.vert" ),
                                  loadAsset( "shader.frag" ) );
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
        gl::scale( vec3( 1 * pDiameter[planetId] ) );
        planetTexture->bind();
        planetBatch->draw();
    gl::popMatrices();
}















































