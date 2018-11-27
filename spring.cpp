#include "spring.h"

Spring::Spring()
{
    this->Init();
}

// Prismatic and Rotary Spring Joint
Spring::Spring(float natural_length, float length_spring_const, float length_damp_const, 
                float natural_angle, float angle_spring_const, float angle_damp_const, 
                float angle_radius,
                Mass * mass_a, Mass * mass_b)
{
    this->Init();

    this->lengthVars.natural = natural_length;
    this->lengthVars.current = natural_length;
    this->lengthVars.current_previous = natural_length;
    this->lengthVars.current_velocity = 0.0f;
    this->lengthVars.spring_const = length_spring_const;
    this->lengthVars.damp_const = length_damp_const;

    this->angleVars.natural = natural_angle;
    this->angleVars.natural_base = natural_angle;
    this->angleVars.current = natural_angle;
    this->angleVars.current_previous = natural_angle;
    this->angleVars.current_velocity = 0.0f;
    this->angleVars.spring_const = angle_spring_const;
    this->angleVars.damp_const = angle_damp_const;
    this->angle_radius = angle_radius;
    this->b_mass_angle_offset = 0.0f;

    this->mass_a = mass_a;
    this->mass_b = mass_b;
    //this->angle_init = this->mass_a->GetRotation();
    //sf::Vector2f dist = sf::Vector2f(
    //                        this->mass_a->GetPosCentreX() - this->mass_b->GetPosCentreX(), 
    //                        this->mass_a->GetPosCentreY() - this->mass_b->GetPosCentreY());
    //double two_pi = 2.0f * 3.1415f;
    //float comp_angle = atan2(-dist.y, -dist.x);
    //if(comp_angle < 0)
    //    this->angle_init = comp_angle + two_pi;
    //else
    //    this->angle_init = comp_angle;
}

Spring* Spring::CreateSpringPrismatic(
                            float natural_length, float length_spring_const, 
                            float length_damp_const, 
                            Mass * mass_a, Mass * mass_b)
{
    Spring* sp = new Spring(natural_length, length_spring_const, length_damp_const,
                                0.0f, 0.0f, 0.0f, 0.0f,
                                mass_a, mass_b);
    return sp;
}

Spring* Spring::CreateSpringRotary(
                            float natural_length,
                            float natural_angle, float angle_spring_const, 
                            float angle_damp_const, float angle_radius,
                            Mass * mass_a, Mass * mass_b)
{
    Spring* sp = new Spring(natural_length, 1.0f, 1.0f,
                            natural_angle, angle_spring_const, angle_damp_const,
                            angle_radius,
                            mass_a, mass_b);
    return sp;
}

void Spring::Init()
{
    this->lengthVars.SetZeros();
    this->angleVars.SetZeros();
    this->angle_init = 0.0f;
    this->angle_radius = 0.0f;
    //this->lengthVars.natural = 0.0f;
    //this->lengthVars.current = 0.0f;
    //this->lengthVars.current_previous = 0.0f;
    //this->lengthVars.current_velocity = 0.0f;
    //this->lengthVars.spring_const = 0.0f;
    //this->lengthVars.damp_const = 0.0f;

    //this->angleVars.natural = 0.0f;
    //this->angleVars.current = 0.0f;
    //this->angleVars.current_previous = 0.0f;
    //this->angleVars.current_velocity = 0.0f;
    //this->angleVars.spring_const = 0.0f;
    //this->angleVars.damp_const = 0.0f;

    this->mass_a = NULL;
    this->mass_b = NULL;

    this->debug_mode = false;
}

//float Spring::ComputeForceMagnitude() {}
//float SpringPrism::ComputeForceMagnitude()
//{
//    return this->prism_spring_const * (this->natural_length - this->current_length) - this->damp_spring_const * this->current_velocity;
//}

void Spring::Update(float dt, bool skip_internal_update)
{
    sf::Vector2f dist = sf::Vector2f(
                            this->mass_a->GetPosCentreX() - this->mass_b->GetPosCentreX(), 
                            this->mass_a->GetPosCentreY() - this->mass_b->GetPosCentreY());
    // update the length of the spring according to the masses
    this->lengthVars.current_previous = this->lengthVars.current;
    this->lengthVars.current = pow(dist.x*dist.x + dist.y*dist.y, 0.5f);


    // update the angle of the spring according to the masses
    this->angleVars.current_previous = this->angleVars.current;
    double two_pi = 2.0f * 3.1415f;
    float comp_angle = atan2(-dist.y, -dist.x) ;//+ this->mass_a->GetRotation();
    //float comp_angle = atan2(-dist.y, -dist.x) - this->mass_a->GetRotation();
    //float comp_angle = this->mass_a->GetRotation() - this->angle_init;
    this->angleVars.current = comp_angle;
    if(this->debug_mode && !skip_internal_update)
    {
        std::cout << "ANG BEFORE: " << this->angleVars.current << "\n";
        std::cout << "ANG NATURAL: " << this->angleVars.natural << "\n";
    }
    if(comp_angle < 0)
        this->angleVars.current = comp_angle + two_pi;
    else
        this->angleVars.current = comp_angle;

    // below is to deal with edge cases of angles and make sure we turn the 
    //   direction which is the shortest path to the target angle
    double ang_norm = abs(this->angleVars.natural - this->angleVars.current);
    double ang_offset1 = abs(this->angleVars.natural - (this->angleVars.current - two_pi));
    double ang_offset2 = abs(this->angleVars.natural - (this->angleVars.current + two_pi));
    if(ang_offset1 < ang_norm && ang_offset1 < ang_offset2)
        this->angleVars.current -= two_pi;
    else if(ang_offset2 < ang_norm && ang_offset2 < ang_offset1)
        this->angleVars.current += two_pi;

    // update natural
    this->angleVars.natural = this->angleVars.natural_base + this->mass_a->GetRotation();
    if(this->angleVars.natural > two_pi)
        this->angleVars.natural -= two_pi;
 
    // compute approximate velocity
    this->lengthVars.UpdateVelocity(dt);
    this->angleVars.UpdateVelocity(dt);
    
    // physics does not affect the spring, therefore the spring only has to apply
    //   forces to the masses.
    // --------------------- UPDATE LENGTH TERM ---------------------------
    //float force_magnitude = this->ComputeForceMagnitude();
    float force_magnitude = this->lengthVars.ComputeForceMagnitude();
    sf::Vector2f dist_norm = sf::Vector2f(
                            dist.x / this->lengthVars.current, 
                            dist.y / this->lengthVars.current);
    this->mass_a->AddForce(force_magnitude * dist_norm);
    this->mass_b->AddForce(-force_magnitude * dist_norm);

    // --------------------- UPDATE LENGTH TERM ---------------------------
    float torque = this->angleVars.ComputeForceMagnitude();
    // we assume that the FIRST mass is the pivot and the distance to the SECOND
    //   mass is where the torque is applied
    sf::Vector2f force_rot = sf::Vector2f(
                                -torque * -dist.y, 
                                torque * -dist.x); //F = R x (-T) 
                                //torque * -dist_norm.y*angle_radius, 
                                //-torque * -dist_norm.x*angle_radius);
    //if(this->angleVars.natural != 0.0f)
    //{
    //    std::cout << "--\n";
    //    std::cout << "comp: " << comp_angle << "\n";
    //    std::cout << "ang_p: " << this->angleVars.current_previous << "\n";
    //    std::cout << "ang: " << this->angleVars.current << "\n";
    //    std::cout << "nat: " << this->angleVars.natural << "\n";
    //    std::cout << "vel: " << this->angleVars.current_velocity << "\n";
    //    std::cout << "d_x: " << dist.x << "\n";
    //    std::cout << "d_y: " << dist.y << "\n";
    //    std::cout << "t_m: " << torque << "\n";
    //    std::cout << "t_x: " << force_rot.x << "\n";
    //    std::cout << "t_y: " << force_rot.y << "\n";
    //    std::cout << "--\n";
    //}
    //
    //sf::Vector2f force_rot = sf::Vector2f(
    //                            0.0f, //torque * dist.y/2.0f, 
    //                            -torque * dist.y/2.0f); //F = T x R
    
    this->mass_b->AddForce(force_rot);
    this->mass_a->AddForce(-force_rot);

    //this->mass_a->AddTorque(torque);
    //this->mass_b->AddTorque(torque);
    //this->mass_b->SetRotation(this->angleVars.current); //TODO: bad, should use torque physics
    if(!skip_internal_update)
        this->mass_b->SetRotation(this->angleVars.current + this->b_mass_angle_offset); //TODO: bad, should use torque physics

    if(this->debug_mode && !skip_internal_update)
    {
        std::cout << "ANG AFTER: " << this->angleVars.current << "\n";
        std::cout << "\tBMASS: " << this->mass_b->GetRotation() << "\n";
    }

    // compute torque on the other body
    //float torque_b = dist.y*force_rot.x - dist.x*force_rot.y;
    //this->mass_b->AddTorque(torque); //-torque_b);
}

void Spring::Draw(sf::RenderWindow& window, const float PIXEL_TO_METER, sf::Vector2f POS_OFFSET)
{
    sf::Vertex floor[] = 
    {
        sf::Vertex(sf::Vector2f(this->mass_a->GetPosX()*PIXEL_TO_METER, this->mass_a->GetPosY()*PIXEL_TO_METER) + POS_OFFSET),
        sf::Vertex(sf::Vector2f(this->mass_b->GetPosX()*PIXEL_TO_METER, this->mass_b->GetPosY()*PIXEL_TO_METER) + POS_OFFSET)
    };
    window.draw(floor, 2, sf::Lines);
}

//get/set
void Spring::SetLength(float value)
{
    this->lengthVars.natural = value;
}
float Spring::GetCurrentLength()
{
    return this->lengthVars.current;
}
void Spring::SetAngle(float value)
{
    this->angleVars.natural = value;
    this->angleVars.natural_base = value;
}
void Spring::SetBMassAngleOffset(float value)
{
    this->b_mass_angle_offset = value;
}
void Spring::TurnOnDebug()
{
    this->debug_mode = true;
}
