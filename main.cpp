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

//COMPILE: g++ main.cpp mass.cpp spring.cpp -lsfml-audio -lsfml-network -lsfml-graphics -lsfml-window -lsfml-system -o run.sh

const int SCREEN_WIDTH = 1300;
const int SCREEN_HEIGHT = 800;
//const float DT = 0.0001f; //1.0f;
const float DT = 0.0004f;
//const float DT = 0.0001f;

const float PIXEL_TO_METER = 1200.0f;
sf::Vector2f CAMERA_POS = sf::Vector2f(0.0f, -900.0f);
const float FLOOR_Y = 1.3f; //(SCREEN_HEIGHT - 50.0f)/PIXEL_TO_METER;
float TIME = 0.0f;

void Init(Controller* dog);
void CleanUp(Controller* dog);
void Update(Controller* dog);
void Draw(sf::RenderWindow& window, Controller* dog);

int main(int n, char** vars)
{
    Controller *dog = new Controller(FLOOR_Y);
    //Controller dog = Controller(FLOOR_Y);
    Init(dog);
    std::cout << "INIT\n";

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Spontaneous");

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                CleanUp(dog);
                //delete dog;
                window.close();
            } 
        }
        
        window.clear();

        // move dog leg
        //dog->MoveLeftFront(sin(time)*10.0f + 40.0f, sin(time)*0.3f + 3.14152/2.0f);
        //dog->MoveLeftBack(sin(time)*10.0f + 40.0f, -sin(time)*0.3f + 3.14152/2.0f);
        //dog->MoveRightFront(sin(time)*10.0f + 40.0f, -sin(time)*0.3f + 3.14152/2.0f);
        //dog->MoveRightBack(sin(time)*10.0f + 40.0f, sin(time)*0.3f + 3.14152/2.0f);

        // move camera to COM
        CAMERA_POS.x = -dog->ComputeCentreOfMassX()*PIXEL_TO_METER + SCREEN_WIDTH/2.0f;

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
        }

        //std::cout << "prep\n";
        //dog->Update(DT);
        //dog.Update(DT);
        Update(dog);
        //std::cout << "UPDATE\n";
        //dog->Draw(window);
        //dog.Draw(window);
        Draw(window, dog);
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

void CleanUp(Controller* dog)
{
    delete dog;
}

void Update(Controller* dog)
{
    dog->Update(DT);
}

void Draw(sf::RenderWindow& window, Controller* dog)
{
    dog->Draw(window, PIXEL_TO_METER, CAMERA_POS);
    dog->DrawData(window, sf::Vector2f(0.0f, 0.0f), TIME);

    //   where the radius is proportional to the mass.
    //sf::CircleShape circ = sf::CircleShape(5.0f);
    //std::cout << "CEN: " << dog->ComputeCentreOfMassX() << ", " << dog->ComputeCentreOfMassY() << "\n";
    //circ.setPosition(sf::Vector2f(dog->ComputeCentreOfMassX(), dog->ComputeCentreOfMassY())*PIXEL_TO_METER + CAMERA_POS);

    //window.draw(circ);

    // draw the floor
    sf::Vertex floor[] = 
    {
        sf::Vertex(sf::Vector2f(0.0f, FLOOR_Y)*PIXEL_TO_METER + sf::Vector2f(0.0f, CAMERA_POS.y)),
        sf::Vertex(sf::Vector2f(2.0f, FLOOR_Y)*PIXEL_TO_METER + sf::Vector2f(0.0f, CAMERA_POS.y))
    };
    window.draw(floor, 2, sf::Lines);
}
