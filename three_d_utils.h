#ifndef THREE_D_UTILS_H
#define THREE_D_UTILS_H

#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"

namespace ThreeDUtils
{
    float RadToDeg(float rad);
    void DrawCube(sf::Vector3f position,
                    float size,
                    sf::Color colour,
                    sf::Vector3f rotation);
    void DrawPlane(sf::Vector3f position,
                    float sizeX, float sizeY,
                    sf::Vector3f colour);
    void DrawLine(sf::Vector3f start_position,
                    sf::Vector3f end_position,
                    sf::Color colour);
}

#endif
