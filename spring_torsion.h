#ifndef SPRING_TORSION_H
#define SPRING_TORSION_H

#include <SFML/Graphics.hpp>
#include "spring.h"
#include "mass.h"

class SpringTorsion : public Spring
{
public:
    SpringTorsion();
    SpringTorsion(float natural_length, float prism_spring_const, 
            float damp_spring_const, Mass * mass_A, Mass * mass_B);

    virtual float ComputeForceMagnitude() override;
};

#endif
