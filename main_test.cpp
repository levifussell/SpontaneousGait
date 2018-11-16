#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <math.h>
#include <time.h>
#include <stdio.h>

#include "mass.h"
#include "spring.h"
#include "controller.h"

// THIS FILE IS FOR TESTING THE PHYSICS USING SIMPLE EXAMPLES

//COMPILE: g++ main_test.cpp mass.cpp spring.cpp -lsfml-audio -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -o run_test.sh

const int SCREEN_WIDTH = 1300;
const int SCREEN_HEIGHT = 800;
const float DT = 0.005f; //1.0f;
const float FLOOR_Y = SCREEN_HEIGHT - 50.0f;

const int MASS_COUNT = 3;
const int SPRING_COUNT = 2;

Mass* masses[MASS_COUNT];
Spring* springs[SPRING_COUNT];

void Init(Mass* masses[MASS_COUNT], Spring* springs[SPRING_COUNT]);
void CleanUp(Mass* masses[MASS_COUNT], Spring* springs[SPRING_COUNT]);
void Update(Mass* masses[MASS_COUNT], Spring* springs[SPRING_COUNT]);
void Draw(sf::RenderWindow& window, Mass* masses[MASS_COUNT], Spring* springs[SPRING_COUNT]);

int main(int n, char** vars)
{
    Init(masses, springs);
    std::cout << "INIT\n";

    float time = 0.0f;

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

        //std::cout << "prep\n";
        //dog->Update(DT);
        //dog.Update(DT);
        Update(masses, springs);
        //std::cout << "UPDATE\n";
        //dog->Draw(window);
        //dog.Draw(window);
        Draw(window, masses, springs);
        //std::cout << "DRAW\n";

        window.display();
        
        time += DT;
    }

    return 0;
}

void Init(Mass* masses[MASS_COUNT], Spring* springs[SPRING_COUNT])
{
    float startX = 400.0f;
    float startY = 400.0f;
    float mass = 0.4f;
    float c_dark = 200.0f;
    masses[0] = new Mass(sf::Vector2f(startX, startY), mass, FLOOR_Y, sf::Color(c_dark, 0.0f, 0.0f, 255.0f));
    masses[1] = new Mass(sf::Vector2f(startX, startY - 50.0f), mass, FLOOR_Y, sf::Color(0.0f, c_dark, 0.0f, 255.0f));
    masses[2] = new Mass(sf::Vector2f(startX + 50.0f, startY - 50.0f), mass, FLOOR_Y, sf::Color(0.0f, 0.0f, c_dark, 255.0f));
    //masses[3] = new Mass(sf::Vector2f(startX + 100.0f, startY - 50.0f), mass, FLOOR_Y, sf::Color(0.0f, 0.0f, c_dark, 255.0f));
    //masses[4] = new Mass(sf::Vector2f(startX + 100.0f, startY), mass, FLOOR_Y, sf::Color(0.0f, 0.0f, c_dark, 255.0f));

    float s_length = 50.0f;
    float s_k = 180.0f;
    float s_d = 20.0f;
    float s_angle = 0.0f;
    float s_a_k = 80.0f;
    float s_a_d = 3.6f;
    float s_r = 1.0f;
    springs[0] = new Spring(s_length, s_k, s_d,
                            3.14152f/2.0f, s_a_k, s_a_d, s_r,
                            masses[1], masses[0]);
    springs[1] = new Spring(s_length, s_k, s_d,
                            s_angle, s_a_k, s_a_d, s_r,
                            masses[1], masses[2]);
    //springs[2] = new Spring(s_length, s_k, s_d,
    //                        s_angle, s_a_k, s_a_d, s_r,
    //                        masses[2], masses[3]);
    //springs[3] = new Spring(s_length, s_k, s_d,
    //                        3.14152f/2.0f, s_a_k, s_a_d, s_r,
    //                        masses[3], masses[4]);
}

void CleanUp(Mass* masses[MASS_COUNT], Spring* springs[SPRING_COUNT])
{
    for(int i = 0; i < MASS_COUNT; ++i)
        delete masses[i];
    for(int i = 0; i < SPRING_COUNT; ++i)
        delete springs[i];
}

void Update(Mass* masses[MASS_COUNT], Spring* springs[SPRING_COUNT])
{
    //for(int i = 0; i < MASS_COUNT; ++i)
    //    masses[i]->Update(DT, masses, MASS_COUNT);
    for(int i = 0; i < MASS_COUNT; ++i)
        masses[i]->UpdateGravity();
    for(int i = 0; i < MASS_COUNT; ++i)
        masses[i]->UpdateTorque(masses, MASS_COUNT);
    for(int i = 0; i < SPRING_COUNT; ++i)
        springs[i]->Update(DT);
    for(int i = 0; i < MASS_COUNT; ++i)
        masses[i]->UpdateFriction(DT);
    for(int i = 0; i < MASS_COUNT; ++i)
        masses[i]->UpdateDynamics(DT);
    for(int i = 0; i < MASS_COUNT; ++i)
        masses[i]->ResetForces();
}

void Draw(sf::RenderWindow& window, Mass* masses[MASS_COUNT], Spring* springs[SPRING_COUNT])
{
    for(int i = 0; i < MASS_COUNT; ++i)
        masses[i]->Draw(window);
    for(int i = 0; i < SPRING_COUNT; ++i)
        springs[i]->Draw(window);

    // draw the floor
    sf::Vertex floor[] = 
    {
        sf::Vertex(sf::Vector2f(0.0f, FLOOR_Y)),
        sf::Vertex(sf::Vector2f(SCREEN_WIDTH, FLOOR_Y))
    };
    window.draw(floor, 2, sf::Lines);

    // print out stats
    for(int i = 0; i < MASS_COUNT; ++i)
        masses[i]->DrawData(window, sf::Vector2f(250.0f*i, 0.0f));
}
