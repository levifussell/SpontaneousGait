#include "mass.h"

Mass::Mass()
{
    this->Init();
}

Mass::Mass(sf::Vector2f pos_init, float mass_kg, float y_floor_boundary)
{
    this->Init();

    this->pos = pos_init;
    this->mass_kg = mass_kg;
    this->y_floor_boundary = y_floor_boundary;

    this->mom_inertia = this->mass_kg; //TODO: copmute real moment of inertia
}

void Mass::Init()
{
    this->pos = sf::Vector2f(0.0f, 0.0f);
    this->vel = sf::Vector2f(0.0f, 0.0f);
    this->force_total = sf::Vector2f(0.0f, 0.0f);
    this->mass_kg = 1.0f;

    this->rotation = 0.0f;
    this->vel_angle = 0.0f;
    this->torque_total = 0.0f;
    this->mom_inertia = this->mass_kg; //TODO: copmute real moment of inertia

    this->y_floor_boundary = 0.0f;

    this->debug_forces = true; //display forces applied to the mass
    this->arr_idx = 0;
    
    this->print_data = false;
}

void Mass::AddForce(sf::Vector2f force)
{
    this->force_total += force;
}
void Mass::AddForceVisualise(sf::Vector2f force)
{
    this->AddForce(force);

    this->forces_list[this->arr_idx] = force;
    this->arr_idx = (this->arr_idx + 1) % 10;
}

void Mass::AddTorque(float torque)
{
    this->torque_total += torque;
}

void Mass::Update(float dt)
{

    // add gravity to the mass
    const float gravity = 9.81f;
    sf::Vector2f gravForce = sf::Vector2f(0.0f, gravity*this->mass_kg);
    this->AddForce(gravForce);

    // add frictional forces
    if(this->pos.y + this->ComputeRadius() >= y_floor_boundary)
    {
        sf::Vector2f friction = sf::Vector2f(0.0f, 0.0f);
        float mu = 1.1f;
        float beta = 1005.0f; //15.0f;
        //friction.y = 0.008f;
        //float normal_y = 0.008f;
       
        float normal_y = 0.0f;
        if(this->force_total.y >= 0.0)
        {
            normal_y = -this->force_total.y + (1.0/dt)*this->mass_kg*this->vel.y;
            if(this->print_data)
            {
                std::cout << "NORM: " << normal_y << "\n";
                std::cout << "FORCEY: " << this->force_total.y << "\n";
                std::cout << "VELY: " << this->vel.y << "\n";
            }
        }
        friction.y = -1.0f*normal_y;

        if(this->print_data)
        {
            std::cout << "VELX: " << this->vel.x << "\n";
            std::cout << "TAN: " << -tanh(beta*this->vel.x) << "\n";
        }
        friction.x = mu * normal_y * -tanh(beta * this->vel.x);
        //if(this->vel.x < 0.0)
        //    friction.x = mu * normal_y; // * -tanh(beta * this->vel.x);
        //else
        //    friction.x = -mu * normal_y; // * -tanh(beta * this->vel.x);
        this->AddForce(friction);
    }
    //else
    //{
        //// add gravity to the mass
        //const float gravity = 9.81f;
        //sf::Vector2f gravForce = sf::Vector2f(0.0f, gravity*this->mass_kg);
        //this->AddForce(gravForce);
    //}

    // update kinematics
        // positional
    sf::Vector2f acc = this->force_total / this->mass_kg;
    //std::cout << "fY: " << acc.y << "\n";
    this->vel += dt * acc;
    this->pos += dt * this->vel;
        //rotational
    double acc_angle = this->torque_total / this->mom_inertia;
    this->vel_angle += dt * acc_angle;
    this->rotation += dt * this->vel_angle;
    float two_pi = 2.0f * 3.14152f;
    if(this->rotation < 0.0f)
        this->rotation += two_pi;
    else if(this->rotation > two_pi)
        this->rotation -= two_pi;
    //std::cout << "tor: " << this->torque_total << "\n";

    // check that the mass has not exceeded the floor
    //   NOTE: because the circle is a point mass, only the centre 'collides'
    //     so we don't mind if it is through the floor by its radius
    if(this->pos.y + this->ComputeRadius() >= y_floor_boundary)
    {
        this->pos.y = this->y_floor_boundary - this->ComputeRadius();
        //this->vel.y = 0.0f;
    }

    // reset the force
    this->force_total.x = 0.0f;
    this->force_total.y = 0.0f;
    this->torque_total = 0.0f;
}

void Mass::Draw(sf::RenderWindow& window)
{
    // point mass is just a point. We make this a circle (with no collisions),
    //   where the radius is proportional to the mass.
    sf::Color color = sf::Color(255.0f, 0.0f, 0.0f, 255.0f);
    sf::CircleShape circ = sf::CircleShape(this->ComputeRadius());
    circ.setPosition(this->pos);
    circ.setFillColor(color);

    window.draw(circ);

    // draw a line denoting rotation
    float rot_x = cos(this->rotation) * this->ComputeRadius();
    float rot_y = sin(this->rotation) * this->ComputeRadius();
    sf::Vertex floor[] = 
    {
        sf::Vertex(sf::Vector2f(this->GetPosCentreX(), this->GetPosCentreY())),
        sf::Vertex(sf::Vector2f(this->GetPosCentreX() + rot_x, this->GetPosCentreY() + rot_y))
    };
    window.draw(floor, 2, sf::Lines);

    // draw debug forces
    if(this->debug_forces)
    {
        for(int f = 0; f < this->arr_idx; ++f)
        {
            sf::Color f_col = sf::Color(255.0f, 125.0f, 0.0f, 255.0f);
            sf::Vector2f cent = sf::Vector2f(this->GetPosCentreX(), this->GetPosCentreY());
            sf::Vertex force[] =
            {
                sf::Vertex(cent, f_col),
                sf::Vertex(cent + this->forces_list[f]*10.0f, f_col)
            };
            window.draw(force, 2, sf::Lines);
        }
        this->arr_idx = 0;
        //sf::Color f_col = sf::Color(255.0f, 125.0f, 0.0f, 255.0f);
        //sf::Vector2f cent = sf::Vector2f(this->GetPosCentreX(), this->GetPosCentreY());
        //sf::Vertex force[] =
        //{
        //    sf::Vertex(cent, f_col),
        //    sf::Vertex(cent + this->force_total*10.0f, f_col)
        //};
        //window.draw(force, 2, sf::Lines);
    }
}

//get/set
float Mass::GetPosX()
{
    return this->pos.x;
}
float Mass::GetPosY()
{
    return this->pos.y;
}
float Mass::GetPosCentreX()
{
    return this->pos.x + this->ComputeRadius();
}
float Mass::GetPosCentreY()
{
    return this->pos.y + this->ComputeRadius();
}
float Mass::GetRotation()
{
    return this->rotation;
}
void Mass::SetRotation(float value) //TODO: bad function should use physics
{
    this->rotation = value;
}

// private
float Mass::ComputeRadius()
{
    return std::min(this->mass_kg * 60.0f, 10.0f);
}
