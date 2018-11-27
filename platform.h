#ifndef PLATFORM_H
#define PLATFORM_H

#include "SFML/Graphics.hpp"
#include <iostream>

#include "three_d_utils.h"

class Platform
{
private:
    sf::Vector3f origin; // origin.x is same position as spawn_pos
    float boundary_size_x;
    int num_panels;
    sf::Vector3f* panels;
    float panel_rate;
public:
    Platform(sf::Vector3f origin, float boundary_size_x, int num_panels);
    ~Platform();

    void Update(float dt);
    void Draw(sf::RenderWindow& window, float PIXEL_TO_METER);

    void SetOrigin(sf::Vector3f origin);
};

#endif
