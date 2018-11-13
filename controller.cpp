#include "controller.h"

Controller::Controller() {}

Controller::Controller(int FLOOR_Y)
{
    this->InitModel(FLOOR_Y);
}

Controller::~Controller()
{
    for(int i = 0; i < MASS_COUNT; ++i)
    {
        delete this->masses[i];
    }

    for(int i = 0; i < MASS_COUNT; ++i)
    {
        delete this->springs[i];
    }
}

void Controller::InitModel(int FLOOR_Y)
{
    ////dog:
    float startX = 100.0f;
    float startY = 200.0f;
        //body:
    float mass_shoulder = 0.6f;
    float mass_hip = 0.4f;
    float mass_spine = 0.1f;
    float mass_knee = 0.075f;
    float mass_foot = 0.035f;
    float k_b = 180.0f;
    float d_b = 20.0f;
    //float k_b = 3750.0f;
    //float d_b = 100.0f;
    this->masses[0] = new Mass(sf::Vector2f(startX, startY), mass_shoulder, FLOOR_Y);
    this->masses[1] = new Mass(sf::Vector2f(startX+50.0f, startY), mass_spine, FLOOR_Y);
    this->masses[2] = new Mass(sf::Vector2f(startX+100.0f, startY), mass_spine, FLOOR_Y);
    this->masses[3] = new Mass(sf::Vector2f(startX+150.0f, startY), mass_spine, FLOOR_Y);
    this->masses[4] = new Mass(sf::Vector2f(startX+200.0f, startY), mass_shoulder, FLOOR_Y);
    float k_rigid = 180.0f; //12500.0f;
    float d_rigid = 20.0f;
    //float t_k_rigid = 100.0f;
    //float t_d_rigid = 100.0f;

    //float t_k = 0.0f; //15.9f;
    //float t_d = 0.0f; //1.1f;
    //float t_k_spine = 8.0f;
    //float t_d_spine = 0.016f;
    float t_k_spine = 50.0f;
    float t_d_spine = 3.6f;
    this->springs[0] = new Spring(80.0f, k_rigid, d_rigid, 
                        0.0f, t_k_spine, t_d_spine, 1.0f,
                        this->masses[0], this->masses[1]);
    this->springs[1] = new Spring(30.0f, k_rigid, d_rigid, 
                        0.0f, t_k_spine, t_d_spine, 1.0f, 
                        this->masses[1], this->masses[2]);
    this->springs[2] = new Spring(30.0f, k_rigid, d_rigid, 
                        0.0f, t_k_spine, t_d_spine, 1.0f,
                        this->masses[2], this->masses[3]);
    this->springs[3] = new Spring(80.0f, k_rigid, d_rigid, 
                        0.0f, t_k_spine, t_d_spine, 1.0f,
                        this->masses[3], this->masses[4]);

        //leg left-front
    this->masses[5] = new Mass(sf::Vector2f(startX + 10.0f, startY + 50.0f), mass_knee, FLOOR_Y);
    this->masses[6] = new Mass(sf::Vector2f(startX + 10.0f, startY + 100.0f), mass_foot, FLOOR_Y);
    this->springs[4] = new Spring(51.0f, k_rigid, d_rigid, 
                            3.1415f/2.3f, t_k_spine, t_d_spine, 1.0f,
                            this->masses[0], this->masses[5]);
    this->springs[5] = new Spring(51.0f, k_b, d_b, 
                            0.0f, t_k_spine, t_d_spine, 1.0f,
                            this->masses[5], this->masses[6]);

        //leg right-front
    this->masses[7] = new Mass(sf::Vector2f(startX + 240.0f + 10.0f, startY + 50.0f), mass_knee, FLOOR_Y);
    this->masses[8] = new Mass(sf::Vector2f(startX + 240.0f + 10.0f, startY + 100.0f), mass_foot, FLOOR_Y);
    this->springs[6] = new Spring(51.0f, k_rigid, d_rigid, 
                            3.1415f/2.3f, t_k_spine, t_d_spine, 1.0f,
                            this->masses[4], this->masses[7]);
    this->springs[7] = new Spring(51.0f, k_b, d_b, 
                            0.0f, t_k_spine, t_d_spine, 1.0f,
                            this->masses[7], this->masses[8]);

        //leg left-back
    this->masses[9] = new Mass(sf::Vector2f(startX + 10.0f, startY + 50.0f), mass_knee, FLOOR_Y);
    this->masses[10] = new Mass(sf::Vector2f(startX + 10.0f, startY + 100.0f), mass_foot, FLOOR_Y);
    this->springs[8] = new Spring(51.0f, k_rigid, d_rigid, 
                            3.1415f/1.7f, t_k_spine, t_d_spine, 1.0f,
                            this->masses[0], this->masses[9]);
    this->springs[9] = new Spring(51.0f, k_b, d_b, 
                            0.0f, t_k_spine, t_d_spine, 1.0f,
                            this->masses[9], this->masses[10]);

        //leg right-back
    this->masses[11] = new Mass(sf::Vector2f(startX + 240.0f + 10.0f, startY + 50.0f), mass_knee, FLOOR_Y);
    this->masses[12] = new Mass(sf::Vector2f(startX + 240.0f + 10.0f, startY + 100.0f), mass_foot, FLOOR_Y);
    this->springs[10] = new Spring(51.0f, k_rigid, d_rigid, 
                            3.1415f/1.7f, t_k_spine, t_d_spine, 1.0f,
                            this->masses[4], this->masses[11]);
    this->springs[11] = new Spring(51.0f, k_b, d_b, 
                            0.0f, t_k_spine, t_d_spine, 1.0f,
                            this->masses[11], this->masses[12]);
}

void Controller::Update(float dt)
{
    for(int i = 0; i < MASS_COUNT; ++i)
    {
        this->masses[i]->Update(dt);
    }

    for(int i = 0; i < SPRING_COUNT; ++i)
    {
        this->springs[i]->Update(dt);
    }
}

void Controller::Draw(sf::RenderWindow& window)
{
    for(int i = 0; i < MASS_COUNT; ++i)
    {
        this->masses[i]->Draw(window);
    }

    for(int i = 0; i < SPRING_COUNT; ++i)
    {
        this->springs[i]->Draw(window);
    }
}
