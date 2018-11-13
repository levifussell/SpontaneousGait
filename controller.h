#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>

#include "mass.h"
#include "spring.h"

class Controller
{
private:
    static const int MASS_COUNT = 13;
    static const int SPRING_COUNT = 12;
    Mass *masses[MASS_COUNT];
    Spring *springs[SPRING_COUNT];

public:
    Controller();
    Controller(int FLOOR_Y);
    ~Controller();

    void InitModel(int FLOOR_Y);
    void Update(float dt);
    void Draw(sf::RenderWindow& window);
};

#endif
