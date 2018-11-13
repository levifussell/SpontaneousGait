#include "spring_prism.h"

SpringPrism::SpringPrism()
    : Spring()
{
}

SpringPrism::SpringPrism(float natural_length, float prism_spring_const, 
                float damp_spring_const, Mass * mass_A, Mass * mass_B)
    :Spring(natural_length, prism_spring_const,
                    damp_spring_const, mass_A, mass_B)
{
}

//float SpringPrism::ComputeForceMagnitude()
//{
//    return this->prism_spring_const * (this->natural_length - this->current_length) - this->damp_spring_const * this->current_velocity;
//}
