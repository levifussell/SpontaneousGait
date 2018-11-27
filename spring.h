#ifndef SPRING_H
#define SPRING_H

#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include "mass.h"
#include "three_d_utils.h"

class Spring
{
protected:
    struct SpringVariables
    {
        float natural;
        float natural_base;
        float current;
        float current_previous; //used to compute approx. velocity
        float current_velocity;
        float spring_const;
        float damp_const;

        void SetZeros()
        {
            this->natural_base = 0.0f;
            this->natural = 0.0f;
            this->current = 0.0f;
            this->current_previous = 0.0f;
            this->current_velocity = 0.0f;
            this->spring_const = 0.0f;
            this->damp_const = 0.0f;
        }

        void UpdateVelocity(float dt)
        {
            this->current_velocity = (this->current - this->current_previous) / dt;
        }

        float ComputeForceMagnitude()
        {
            return this->spring_const * (this->natural - this->current) - this->damp_const * this->current_velocity;
        }
    };

    // for prismatic actuation
    SpringVariables lengthVars;
    SpringVariables angleVars;
    float angle_radius;
    float angle_init; //init angle for actuating joint
    float b_mass_angle_offset;
    //float natural_length;
    //float current_length;
    //float current_length_previous; //used to compute approx. velocity
    //float current_velocity;
    //float prism_spring_const;
    //float damp_spring_const;

    bool debug_mode;

    // for rotorary actuation

    Mass* mass_a;
    Mass* mass_b;

public:
    Spring();
    Spring(float natural_length, float length_spring_const, float length_damp_const, 
                float natural_angle, float angle_spring_const, float angle_damp_const, 
                float angle_radius,
                Mass * mass_a, Mass * mass_b);

    static Spring* CreateSpringPrismatic(
                                float natural_length, float length_spring_const, 
                                float length_damp_const, 
                                Mass * mass_a, Mass * mass_b);
    static Spring* CreateSpringRotary(
                                float natural_length,
                                float natural_angle, float angle_spring_const, 
                                float angle_damp_const, float angle_radius,
                                Mass * mass_a, Mass * mass_b);

    void Init();
    //virtual float ComputePrismaticForceMagnitude();
    //float ComputePrismaticForceMagnitude();
    //float ComputeRotaryTorque();
    void Update(float dt, bool skip_internal_update);
    void Draw(sf::RenderWindow& window, const float PIXEL_TO_METER, sf::Vector3f POS_OFFSET);

    void SetLength(float value);
    float GetCurrentLength();
    void SetAngle(float value);
    void SetBMassAngleOffset(float value);
    void TurnOnDebug();
};

#endif
