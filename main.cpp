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
const int SCREEN_HEIGHT = 500;
const float DT = 0.001f; //1.0f;
const float FLOOR_Y = SCREEN_HEIGHT - 50.0f;

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
    dog->Draw(window);

    // draw the floor
    sf::Vertex floor[] = 
    {
        sf::Vertex(sf::Vector2f(0.0f, FLOOR_Y)),
        sf::Vertex(sf::Vector2f(SCREEN_WIDTH, FLOOR_Y))
    };
    window.draw(floor, 2, sf::Lines);
}
