#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>
#include <string>

#include "mass.h"
#include "spring.h"

class Controller
{
private:
    //static const int MASS_COUNT = 13;
    //static const int MASS_COUNT = 14;
    static const int MASS_COUNT = 15;
    //static const int SPRING_COUNT = 14;
    static const int SPRING_COUNT = 14;
    Mass *masses[MASS_COUNT];
    Spring *springs[SPRING_COUNT];

    float support_scalar;
    float propulsion_scalar;
    float settle_angle; // angle where the legs are settled

    float leg_angular_velocity;
    float leg_phases[4];
    sf::Vector2f leg_offset;
    sf::Vector2f leg_amp;
    float alpha_ground_scalar;
    std::string leg_names[4];
    int leg_joint_indices[4][3]; //indices: [thigh_spring, knee_spring, foot_mass]
    
    sf::Font font;

    void SetLegTarget(int femur_idx, int knee_idx, float phase);
    void UpdateLegPhase(std::string leg_name, float dt);

    float ComputeCentreOfMass(bool isX);

    int GetLegIndexFromName(std::string leg_name);
    
public:
    Controller();
    Controller(float FLOOR_Y);
    ~Controller();

    void InitModel(float FLOOR_Y);

    void SetSpringLength(int spring_idx, float target_length);
    void SetSpringAngle(int spring_idx, float target_angle);
    void SetLegTarget(std::string leg_name, float phase);

    void MoveLeftFront(float target_length, float target_angle);
    void MoveLeftBack(float target_length, float target_angle);
    void MoveRightFront(float target_length, float target_angle);
    void MoveRightBack(float target_length, float target_angle);

    void Update(float dt);
    void Draw(sf::RenderWindow& window, const float PIXEL_TO_METER, sf::Vector2f POS_OFFSET);
    void DrawData(sf::RenderWindow& window, sf::Vector2f position, float time);

    float ComputeCentreOfMassX();
    float ComputeCentreOfMassY();

    float GetLegAngularVelocity();
    void SetLegAngularVelocity(float value);
};

#endif
