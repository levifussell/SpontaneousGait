#ifndef MASS_H
#define MASS_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

class Mass
{
private:
    // positional
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Vector2f force_total;
    float mass_kg;
    // rotational
    float rotation;
    float vel_angle;
    float torque_total;
    float mom_inertia;

    bool print_data;

    bool debug_forces;
    sf::Vector2f forces_list[10];
    int arr_idx;

    float y_floor_boundary;

    float ComputeRadius();

public:
    Mass();
    Mass(sf::Vector2f pos_init, float mass_kg, float y_floor_boundary);

    void Init();
    void AddForce(sf::Vector2f force);
    void AddForceVisualise(sf::Vector2f force);
    void AddTorque(float torque);
    void Update(float dt);
    void Draw(sf::RenderWindow& window);

    float GetPosX();
    float GetPosY();
    float GetPosCentreX();
    float GetPosCentreY();
    float GetRotation();
    void SetRotation(float value);
};

#endif
