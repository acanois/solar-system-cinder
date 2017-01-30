//
//  Planet.hpp
//  no_sound_in_space
//
//  Created by David Richter on 1/28/17.
//
//

#ifndef Planet_h
#define Planet_h

#include <stdio.h>
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class Planet 
{
public:
    Planet();
    Planet( float planetRadiusX, float planetRadiusZ, size_t planetDiameter );
    ~Planet();
    
    void init(float planetRadiusX, float planetRadiusZ, size_t planetDiameter );
    void update();
    void display();
    
    // Planet attributes
    gl::GlslProgRef mGlsl;
    gl::TextureRef  planetTexture;
    gl::BatchRef    planetBatch;
    
    vec3            planetRadius;
    vec3            planetPosition;
    size_t          planetDiameter;
    
    float  orbitspeed;
    float  theta;
    mat4   rotation;
    size_t diameter;
};

#endif /* Planet_h */




























