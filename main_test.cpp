#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"
#include "GL/glu.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <math.h>
#include <time.h>
#include <stdio.h>

#include "mass.h"
#include "spring.h"
#include "controller.h"
#include "three_d_utils.h"

// THIS FILE IS FOR TESTING THE PHYSICS USING SIMPLE EXAMPLES

//COMPILE: g++ main_test.cpp mass.cpp spring.cpp -lsfml-audio -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -o run_test.sh

const int SCREEN_WIDTH = 1300;
const int SCREEN_HEIGHT = 800;
const float DT = 0.00005f; //1.0f;
//const float FLOOR_Y = SCREEN_HEIGHT - 50.0f;

const float PIXEL_TO_METER = 1200.0f;
sf::Vector3f CAMERA_POS = sf::Vector3f(0.0f, -900.0f, 0.0f);
const float FLOOR_Y = -0.2f; //(SCREEN_HEIGHT - 50.0f)/PIXEL_TO_METER;
float TIME = 0.0f;

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

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), "Spontaneous");

    glClearDepth(1.0f);
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0f, (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 1.0f, 900.0f);

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
        
        //window.clear();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

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
    float startX = 0.0f; //400.0f;
    float startY = 0.0f; //400.0f;
    float startZ = -50.0f; //400.0f;
    float mass = 0.2f;
    float c_dark = 200.0f;
    masses[0] = new Mass(sf::Vector3f(startX, startY, startZ), mass, FLOOR_Y, sf::Color(c_dark, 0.0f, 0.0f, 255.0f));
    //masses[1] = new Mass(sf::Vector3f(startX, startY - 50.0f, startZ), mass, FLOOR_Y, sf::Color(0.0f, c_dark, 0.0f, 255.0f));
    //masses[2] = new Mass(sf::Vector3f(startX + 50.0f, startY - 50.0f, startZ), mass, FLOOR_Y, sf::Color(0.0f, 0.0f, c_dark, 255.0f));
    masses[1] = new Mass(sf::Vector3f(startX, startY + 0.3f, startZ), mass, FLOOR_Y, sf::Color(0.0f, c_dark, 0.0f, 255.0f));
    masses[2] = new Mass(sf::Vector3f(startX + 0.3f, startY + 0.3f, startZ), mass, FLOOR_Y, sf::Color(0.0f, 0.0f, c_dark, 255.0f));
    //masses[3] = new Mass(sf::Vector2f(startX + 100.0f, startY - 50.0f), mass, FLOOR_Y, sf::Color(0.0f, 0.0f, c_dark, 255.0f));
    //masses[4] = new Mass(sf::Vector2f(startX + 100.0f, startY), mass, FLOOR_Y, sf::Color(0.0f, 0.0f, c_dark, 255.0f));

    float s_length = 0.2f;
    float k_link = 1.25*pow(10.0f,4);
    float d_link = 1.0*pow(10.0f,2);
    //float s_k = 1800.0f;
    //float s_d = 200.0f;
    float s_angle = 0.0f;
    float k_spine = 4.0*pow(10.0f,5);
    float d_spine = 1.6*pow(10.0f,2);
    //float s_a_k = 80.0f;
    //float s_a_d = 3.6f;
    float s_r = 1.0f;
    springs[0] = new Spring(s_length, k_link, d_link,
                            0.0f, k_spine, d_spine, s_r,
                            masses[0], masses[1]);
    //springs[0]->SetBMassAngleOffset(-3.14152f/1.0f);
    springs[0]->TurnOnDebug();
    springs[1] = new Spring(s_length, k_link, d_link,
                            -3.1415f/2.0f, k_spine, d_spine, s_r,
                            masses[1], masses[2]);
    //springs[1]->SetBMassAngleOffset(-3.14152f/1.0f);
    //springs[1]->TurnOnDebug();
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
    std::cout << "-------------------------------------------------------------------\n";
    //for(int i = 0; i < MASS_COUNT; ++i)
    //    masses[i]->Update(DT, masses, MASS_COUNT);
    for(int i = 0; i < MASS_COUNT; ++i)
        masses[i]->UpdateGravity();

    //for(int i = 0; i < MASS_COUNT; ++i)
        //masses[i]->UpdateTorque(masses, MASS_COUNT);
        
    for(int i = 0; i < SPRING_COUNT; ++i)
        springs[i]->Update(DT, false);
    for(int i = 0; i < MASS_COUNT; ++i)
        masses[i]->UpdateFriction(DT);
    for(int i = 0; i < MASS_COUNT; ++i)
        masses[i]->UpdateDynamics(DT);

    for(int i = 0; i < MASS_COUNT; ++i)
    {
        sf::Vector3f acc = sf::Vector3f(masses[i]->ComputeAccX(), 
                                        masses[i]->ComputeAccY(), 
                                        masses[i]->ComputeAccZ());
        masses[i]->SetVel(sf::Vector3f(masses[i]->GetVelX() + acc.x*DT, 
                                       masses[i]->GetVelY() + acc.y*DT, 
                                       masses[i]->GetVelZ() + acc.z*DT));
        masses[i]->SetPos(sf::Vector3f(masses[i]->GetPosX() + masses[i]->GetVelX()*DT, 
                                       masses[i]->GetPosY() + masses[i]->GetVelY()*DT, 
                                       masses[i]->GetPosZ() + masses[i]->GetVelZ()*DT));
    }

    for(int i = 0; i < MASS_COUNT; ++i)
        masses[i]->ResetForces();
}

void Draw(sf::RenderWindow& window, Mass* masses[MASS_COUNT], Spring* springs[SPRING_COUNT])
{
    float m_scale = 100.0f;

    // draw the floor
    ThreeDUtils::DrawPlane(sf::Vector3f(0.0f, FLOOR_Y*m_scale, -50.0f), 100.0f, 100.0f,
                            sf::Color(0.1f, 0.1f, 0.1f));

    sf::Vector3f cam = sf::Vector3f(0.0f, 0.0f, 0.0f);
    for(int i = 0; i < SPRING_COUNT; ++i)
        springs[i]->Draw(window, m_scale, cam);
    for(int i = 0; i < MASS_COUNT; ++i)
        masses[i]->Draw(window, m_scale, cam);

    //sf::Vertex floor[] = 
    //{
    //    sf::Vertex(sf::Vector2f(0.0f, FLOOR_Y)*PIXEL_TO_METER + sf::Vector2f(0.0f, CAMERA_POS.y)),
    //    sf::Vertex(sf::Vector2f(2.0f, FLOOR_Y)*PIXEL_TO_METER + sf::Vector2f(0.0f, CAMERA_POS.y))
    //};
    //window.draw(floor, 2, sf::Lines);

    // print out stats
    //for(int i = 0; i < MASS_COUNT; ++i)
    //    masses[i]->DrawData(window, sf::Vector2f(250.0f*i, 0.0f));
}
