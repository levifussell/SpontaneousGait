#ifndef MASS_H
#define MASS_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <string>
#include <sstream>

class Mass
{
private:
    // positional
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Vector2f acc;
    sf::Vector2f force_total;
    float mass_kg;
    // rotational
    float rotation;
    float vel_angle;
    float acc_angle;
    float torque_total;
    float mom_inertia;

    // special normal-force term to keep track of for controller
    sf::Vector2f normal_force;

    // other
    sf::Color color;

    // historical data for logging
    sf::Vector2f old_force_total;
    float old_torque_total;

    bool print_data;

    bool debug_forces;
    sf::Vector2f forces_list[10];
    int arr_idx;

    float y_floor_boundary;

    float ComputeRadius();

public:
    Mass();
    Mass(sf::Vector2f pos_init, float mass_kg, float y_floor_boundary);
    Mass(sf::Vector2f pos_init, float mass_kg, float y_floor_boundary, sf::Color color);

    void Init();
    void AddForce(sf::Vector2f force);
    void AddForcePositional(sf::Vector2f force, sf::Vector2f radius);
    void AddForceVisualise(sf::Vector2f force);
    void AddTorque(float torque);
    void UpdateGravity();
    void UpdateFriction(float dt);
    void UpdateTorque(Mass** masses, const int MASS_COUNT);
    void UpdateDynamics(float dt);
    void ResetForces();
    void Update(float dt, Mass** masses, const int MASS_COUNT);
    void Draw(sf::RenderWindow& window, const float PIXEL_TO_METER);

    void DrawData(sf::RenderWindow& window, sf::Vector2f position);

    float GetPosX();
    float GetPosY();
    float GetPosCentreX();
    float GetPosCentreY();
    void SetStationary(sf::Vector2f position);
    float GetRotation();
    void SetRotation(float value);
    // below are used to give the normal forces to the controller
    float GetNormalX();
    float GetNormalY(); 
    float GetTotalForcesX();
    float GetTotalForcesY();
    float GetMass();
    void SetMomOfInertia(float value);
};

#endif
