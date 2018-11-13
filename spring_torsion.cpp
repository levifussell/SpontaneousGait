#include "spring_torsion.h"

SpringPrism::SpringTorsion()
    : Spring()
{
}

SpringTorsion::SpringTorsion(float natural_length, float prism_spring_const, 
                float damp_spring_const, Mass * mass_A, Mass * mass_B)
    :Spring(natural_length, prism_spring_const,
                    damp_spring_const, mass_A, mass_B)
{
}

float SpringTorsion::ComputeForceMagnitude()
{
    //return this->prism_spring_const * (this->natural_length - this->current_length) - this->damp_spring_const * this->current_velocity;

    // torque is always normal to the screen therefore we only need the z-term
    torque = this->prism_spring_const * 
}
