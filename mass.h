#ifndef MASS_H
#define MASS_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <string>
#include <sstream>

#include "three_d_utils.h"

class Mass
{
private:
    // positional
    sf::Vector3f pos;
    sf::Vector3f vel;
    sf::Vector3f acc;
    sf::Vector3f force_total;
    float mass_kg;
    // rotational
    float rotation;
    float vel_angle;
    float acc_angle;
    float torque_total;
    float mom_inertia;

    // special normal-force term to keep track of for controller
    sf::Vector3f normal_force;

    // other
    sf::Color color;

    // historical data for logging
    sf::Vector3f old_force_total;
    float old_torque_total;

    bool print_data;

    bool debug_forces;
    sf::Vector3f forces_list[10];
    int arr_idx;

    float y_floor_boundary;

    float ComputeRadius();

public:
    Mass();
    Mass(sf::Vector3f pos_init, float mass_kg, float y_floor_boundary);
    Mass(sf::Vector3f pos_init, float mass_kg, float y_floor_boundary, sf::Color color);

    void Init();
    void AddForce(sf::Vector3f force);
    void AddForcePositional(sf::Vector3f force, sf::Vector3f radius);
    void AddForceVisualise(sf::Vector3f force);
    void AddTorque(float torque);
    void UpdateGravity();
    //sf::Vector2f GetGravity();
    void UpdateFriction(float dt);
    //sf::Vector2f GetFriction();
    void UpdateTorque(Mass** masses, const int MASS_COUNT);
    void UpdateDynamics(float dt);
    void ResetForces();
    void Update(float dt, Mass** masses, const int MASS_COUNT);
    void Draw(sf::RenderWindow& window, const float PIXEL_TO_METER, sf::Vector3f POS_OFFSET);

    void DrawData(sf::RenderWindow& window, sf::Vector2f position);

    float ComputeAccX();
    float ComputeAccY();
    float ComputeAccZ();

    float GetPosX();
    float GetPosY();
    float GetPosZ();
    float GetPosCentreX();
    float GetPosCentreY();
    float GetPosCentreZ();
    void SetPos(sf::Vector3f value);
    float GetVelX();
    float GetVelY(); 
    float GetVelZ(); 
    void SetVel(sf::Vector3f value);
    float GetAccX();
    float GetAccY();
    float GetAccZ();
    void SetStationary(sf::Vector3f position);
    float GetRotation();
    void SetRotation(float value);
    // below are used to give the normal forces to the controller
    float GetNormalX();
    float GetNormalY(); 
    float GetNormalZ(); 
    float GetTotalForcesX();
    float GetTotalForcesY();
    float GetTotalForcesZ();
    float GetMass();
    void SetMomOfInertia(float value);
};

#endif
