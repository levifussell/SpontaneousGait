#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <math.h>
#include <time.h>
#include <stdio.h>

#include "mass.h"
#include "spring.h"

//COMPILE: g++ main.cpp mass.cpp -lsfml-audio -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -o run.sh

const int SCREEN_WIDTH = 1300;
const int SCREEN_HEIGHT = 500;
const float DT = 0.1f; //1.0f;
const float FLOOR_Y = SCREEN_HEIGHT - 50.0f;

//const int MASS_COUNT = 13;
const int MASS_COUNT = 4;
//const int SPRING_COUNT = 12;
const int SPRING_COUNT = 3;

Mass *masses[MASS_COUNT];
Spring *springs[SPRING_COUNT];

void Init(Mass *masses[MASS_COUNT], Spring *springs[SPRING_COUNT]);
void CleanUp(Mass *masses[MASS_COUNT], Spring *springs[SPRING_COUNT]);
void Update(Mass *masses[MASS_COUNT], Spring *springs[SPRING_COUNT]);
void Draw(sf::RenderWindow& window, Mass *masses[MASS_COUNT], Spring *springs[SPRING_COUNT]);

int main(int n, char** vars)
{
    Init(masses, springs); 
    //std::cout << "INIT\n";

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Spontaneous");

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                CleanUp(masses, springs);
                window.close();
            } 
        }
        
        window.clear();

        Update(masses, springs);
        //std::cout << "UPDATE\n";
        Draw(window, masses, springs);
        //std::cout << "DRAW\n";

        window.display();
    }

    return 0;
}

void Init(Mass *masses[MASS_COUNT], Spring *springs[SPRING_COUNT])
{
    ////dog:
    //float startX = 100.0f;
    //float startY = 300.0f;
    //    //body:
    //float mass_b = 0.5f;
    //float k_b = 1.0f;
    //float d_b = 1.0f;
    //masses[0] = new Mass(sf::Vector2f(startX, startY), mass_b, FLOOR_Y);
    //masses[1] = new Mass(sf::Vector2f(startX+100.0f, startY), mass_b, FLOOR_Y);
    //masses[2] = new Mass(sf::Vector2f(startX+120.0f, startY), mass_b, FLOOR_Y);
    //masses[3] = new Mass(sf::Vector2f(startX+140.0f, startY), mass_b, FLOOR_Y);
    //masses[4] = new Mass(sf::Vector2f(startX+240.0f, startY), mass_b, FLOOR_Y);
    //springs[0] = new Spring(100.0f, k_b, d_b, 
    //                    0.0f, 0.4f, 0.3f, 1.0f,
    //                    masses[0], masses[1]
    //                    );
    ////springs[0] = Spring::CreateSpringPrismatic(100.0f, k_b, d_b, masses[0], masses[1]);
    //springs[1] = Spring::CreateSpringPrismatic(20.0f, k_b, d_b, masses[1], masses[2]);
    //springs[2] = Spring::CreateSpringPrismatic(20.0f, k_b, d_b, masses[2], masses[3]);
    //springs[3] = Spring::CreateSpringPrismatic(100.0f, k_b, d_b, masses[3], masses[4]);

    //    //leg left-front
    //masses[5] = new Mass(sf::Vector2f(startX + 10.0f, startY + 50.0f), mass_b, FLOOR_Y);
    //masses[6] = new Mass(sf::Vector2f(startX + 20.0f, startY + 100.0f), mass_b, FLOOR_Y);
    //springs[4] = Spring::CreateSpringPrismatic(51.0f, k_b, d_b, masses[0], masses[5]);
    //springs[5] = Spring::CreateSpringPrismatic(51.0f, k_b, d_b, masses[5], masses[6]);

    //    //leg right-front
    //masses[7] = new Mass(sf::Vector2f(startX + 240.0f + 10.0f, startY + 50.0f), mass_b, FLOOR_Y);
    //masses[8] = new Mass(sf::Vector2f(startX + 240.0f + 20.0f, startY + 100.0f), mass_b, FLOOR_Y);
    //springs[6] = Spring::CreateSpringPrismatic(51.0f, k_b, d_b, masses[4], masses[7]);
    //springs[7] = Spring::CreateSpringPrismatic(51.0f, k_b, d_b, masses[7], masses[8]);

    //triangle pendulum:
    masses[0] = new Mass(sf::Vector2f(300.0f, 550.0f), 0.1f, FLOOR_Y);
    masses[1] = new Mass(sf::Vector2f(350.0f, 550.0f), 0.8f, FLOOR_Y);
    masses[2] = new Mass(sf::Vector2f(360.0f, 500.0f), 0.8f, FLOOR_Y);
    masses[3] = new Mass(sf::Vector2f(250.0f, 300.0f), 0.1f, FLOOR_Y);

    float k = 1.0f;
    float d = 1.0f;
    //springs[0] = new Spring(100.0f, k, d, masses[0], masses[1]);
    //springs[1] = new Spring(100.0f, k, d, masses[1], masses[2]);
    //springs[2] = new Spring(100.0f, k, d, masses[2], masses[0]);
    //springs[0] = Spring::CreateSpringPrismatic(100.0f, k, d, masses[0], masses[1]);
    //springs[1] = Spring::CreateSpringPrismatic(100.0f, k, d, masses[1], masses[2]);
    //springs[2] = Spring::CreateSpringPrismatic(100.0f, k, d, masses[2], masses[0]);
    springs[0] = new Spring(50.0f, k, d, 
                           3.14152f*0.5f, 2.9f, 0.1f, 1.0f,
                            masses[1], masses[0]);
    springs[1] = new Spring(50.0f, k, d, 
                           0.00001f, 0.1f, 0.1f, 1.0f,
                            masses[1], masses[2]);
    springs[2] = new Spring(50.0f, k, d, 
                           3.14152f*0.5f, 2.9f, 0.1f, 1.0f,
                            masses[2], masses[3]);
    //springs[2] = new Spring(100.0f, k, d, 
    //                       0.0f, 1.0f, 0.01f, 1.0f,
    //                        masses[2], masses[0]);
    //springs[3] = Spring::CreateSpringRotary(40.0f, 3.1415f*1.5f, 0.9f, 0.01f, 1.0f, masses[1], masses[3]);
}

void CleanUp(Mass *masses[MASS_COUNT], Spring *springs[SPRING_COUNT])
{
    for(int i = 0; i < MASS_COUNT; ++i)
    {
        delete masses[i];
    }

    for(int i = 0; i < MASS_COUNT; ++i)
    {
        delete springs[i];
    }
}

void Update(Mass *masses[MASS_COUNT], Spring *springs[SPRING_COUNT])
{
    for(int i = 0; i < MASS_COUNT; ++i)
    {
        masses[i]->Update(DT);
    }
    
    for(int i = 0; i < SPRING_COUNT; ++i)
    {
        springs[i]->Update(DT);
    }
}

void Draw(sf::RenderWindow& window, Mass *masses[MASS_COUNT], Spring *springs[SPRING_COUNT])
{
    // draw the masses
    for(int i = 0; i < MASS_COUNT; ++i)
    {
        masses[i]->Draw(window);
    }

    // draw the springs
    for(int i = 0; i < SPRING_COUNT; ++i)
    {
        springs[i]->Draw(window);
    }

    // draw the floor
    sf::Vertex floor[] = 
    {
        sf::Vertex(sf::Vector2f(0.0f, FLOOR_Y)),
        sf::Vertex(sf::Vector2f(SCREEN_WIDTH, FLOOR_Y))
    };
    window.draw(floor, 2, sf::Lines);
}
