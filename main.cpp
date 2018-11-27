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
#include "platform.h"

//COMPILE: g++ main.cpp mass.cpp spring.cpp -lsfml-audio -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -o run.sh

const int SCREEN_WIDTH = 1300;
const int SCREEN_HEIGHT = 800;
//const float DT = 0.0001f; //1.0f;
//const float DT = 0.0004f;
const float DT = 0.0002f;
//const float DT = 0.0001f;

//const float PIXEL_TO_METER = 1200.0f;
const float PIXEL_TO_METER = 100.0f;
sf::Vector3f CAMERA_POS = sf::Vector3f(0.0f, -900.0f, 0.0f);
const float FLOOR_Y = -0.2f; //(SCREEN_HEIGHT - 50.0f)/PIXEL_TO_METER;
float TIME = 0.0f;

void Init(Controller* dog, Platform* plat);
void CleanUp(Controller* dog, Platform* plat);
void Update(Controller* dog, Platform* plat);
void Draw(sf::RenderWindow& window, Controller* dog, Platform* plat);

int main(int n, char** vars)
{
    Controller *dog = new Controller(FLOOR_Y);
    Platform *plat = new Platform(sf::Vector3f(0.0f, FLOOR_Y, -50.0f), 1.0f, 25);
    //Controller dog = Controller(FLOOR_Y);
    //Init(dog);
    std::cout << "INIT\n";

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Spontaneous");

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
                CleanUp(dog, plat);
                //delete dog;
                window.close();
            } 
        }
        
        //window.clear();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        float x_target = dog->ComputeCentreOfMassX()*PIXEL_TO_METER;
        gluLookAt(x_target, 0.0f, 0.0f, x_target, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f);
        
        // update platform origin
        plat->SetOrigin(sf::Vector3f(x_target/PIXEL_TO_METER, FLOOR_Y, -50.0f));

        // move dog leg
        //dog->MoveLeftFront(sin(time)*10.0f + 40.0f, sin(time)*0.3f + 3.14152/2.0f);
        //dog->MoveLeftBack(sin(time)*10.0f + 40.0f, -sin(time)*0.3f + 3.14152/2.0f);
        //dog->MoveRightFront(sin(time)*10.0f + 40.0f, -sin(time)*0.3f + 3.14152/2.0f);
        //dog->MoveRightBack(sin(time)*10.0f + 40.0f, sin(time)*0.3f + 3.14152/2.0f);

        // move camera to COM
        //CAMERA_POS.x = -dog->ComputeCentreOfMassX()*PIXEL_TO_METER + SCREEN_WIDTH/2.0f;
        CAMERA_POS.x = 0.0f;
        CAMERA_POS.y = 0.0f;
        CAMERA_POS.z = 0.0f;

        //std::cout << "TIME: " << time << "\n";
        if(TIME > 0.0f)
        {
            //dog->SetLegAngularVelocity(8.0f);
            //float ang = dog->GetLegAngularVelocity();
            //if(ang == 8.0f)
            //    dog->SetLegAngularVelocity(18.0f);
            //else if(ang == 18.0f)
            //    dog->SetLegAngularVelocity(36.0f);
            //TIME = 0.0f;
            dog->SetLegAngularVelocity(dog->GetLegAngularVelocity() + 4.0f*DT);
            std::cout << "LEG ANG: " << dog->GetLegAngularVelocity() << "\n";
        }


        //std::cout << "prep\n";
        //dog->Update(DT);
        //dog.Update(DT);
        Update(dog, plat);
        //std::cout << "UPDATE\n";
        //dog->Draw(window);
        //dog.Draw(window);
        Draw(window, dog, plat);
        //std::cout << "DRAW\n";

        window.display();
        
        TIME += DT;
    }

    if(dog != NULL)
        delete dog;

    return 0;
}

void Init(Controller* dog)
{
    dog = new Controller(FLOOR_Y);
}

void CleanUp(Controller* dog, Platform* plat)
{
    delete dog;
    delete plat;
}

void Update(Controller* dog, Platform* plat)
{
    dog->Update(DT);
    plat->Update(DT);
}

void Draw(sf::RenderWindow& window, Controller* dog, Platform* plat)
{
    // draw the floor
    //ThreeDUtils::DrawPlane(sf::Vector3f(0.0f, FLOOR_Y*PIXEL_TO_METER, -50.0f), 100.0f, 100.0f,
    //                        sf::Color(0.1f, 0.1f, 0.1f));

    plat->Draw(window, PIXEL_TO_METER);
    dog->Draw(window, PIXEL_TO_METER, CAMERA_POS);
    //dog->DrawData(window, sf::Vector2f(0.0f, 0.0f), TIME);

    //   where the radius is proportional to the mass.
    //sf::CircleShape circ = sf::CircleShape(5.0f);
    //std::cout << "CEN: " << dog->ComputeCentreOfMassX() << ", " << dog->ComputeCentreOfMassY() << "\n";
    //circ.setPosition(sf::Vector2f(dog->ComputeCentreOfMassX(), dog->ComputeCentreOfMassY())*PIXEL_TO_METER + CAMERA_POS);

    //window.draw(circ);

    // draw the floor
    //sf::Vertex floor[] = 
    //{
    //    sf::Vertex(sf::Vector2f(0.0f, FLOOR_Y)*PIXEL_TO_METER + sf::Vector2f(0.0f, CAMERA_POS.y)),
    //    sf::Vertex(sf::Vector2f(2.0f, FLOOR_Y)*PIXEL_TO_METER + sf::Vector2f(0.0f, CAMERA_POS.y))
    //};
    //window.draw(floor, 2, sf::Lines);
}
