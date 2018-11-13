#ifndef SPRING_PRISM_H
#define SPRING_PRISM_H

#include <SFML/Graphics.hpp>
#include "spring.h"
#include "mass.h"

class SpringPrism : public Spring
{
public:
    SpringPrism();
    SpringPrism(float natural_length, float prism_spring_const, 
            float damp_spring_const, Mass * mass_A, Mass * mass_B);

    virtual float ComputeForceMagnitude() override;
};

#endif
