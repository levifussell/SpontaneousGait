#include "mass.h"

Mass::Mass()
{
    this->Init();
}

Mass::Mass(sf::Vector3f pos_init, float mass_kg, float y_floor_boundary)
{
    this->Init();

    this->pos = pos_init;
    this->mass_kg = mass_kg;
    this->y_floor_boundary = y_floor_boundary;

    this->mom_inertia = this->mass_kg; //TODO: copmute real moment of inertia

    this->color = sf::Color(255.0f, 0.0f, 0.0f, 255.0f);
}

Mass::Mass(sf::Vector3f pos_init, float mass_kg, float y_floor_boundary, sf::Color color)
{
    this->Init();

    this->pos = pos_init;
    this->mass_kg = mass_kg;
    this->y_floor_boundary = y_floor_boundary;

    this->mom_inertia = this->mass_kg; //TODO: copmute real moment of inertia
    
    this->color = color;
}

void Mass::Init()
{
    this->pos = sf::Vector3f(0.0f, 0.0f, 0.0f);
    this->vel = sf::Vector3f(0.0f, 0.0f, 0.0f);
    this->acc = sf::Vector3f(0.0f, 0.0f, 0.0f);
    this->force_total = sf::Vector3f(0.0f, 0.0f, 0.0f);
    this->mass_kg = 1.0f;

    this->rotation = 0.0f;
    this->vel_angle = 0.0f;
    this->torque_total = 0.0f;
    this->mom_inertia = this->mass_kg; //TODO: copmute real moment of inertia

    this->normal_force = sf::Vector3f(0.0f, 0.0f, 0.0f);

    // historical
    this->old_force_total = sf::Vector3f(0.0f, 0.0f, 0.0f);
    this->old_torque_total = 0.0f;

    this->y_floor_boundary = 0.0f;

    this->debug_forces = true; //display forces applied to the mass
    this->arr_idx = 0;
    
    this->print_data = false;
}

void Mass::AddForce(sf::Vector3f force)
{
    this->force_total += force;

}
void Mass::AddForcePositional(sf::Vector3f force, sf::Vector3f radius)
{
    this->AddForce(force);

    //float torque = radius.x * force.y - radius.y * force.x;
    //this->AddTorque(torque);
}
void Mass::AddForceVisualise(sf::Vector3f force)
{
    this->AddForce(force);

    this->forces_list[this->arr_idx] = force;
    this->arr_idx = (this->arr_idx + 1) % 10;
}

void Mass::AddTorque(float torque)
{
    this->torque_total += torque;
}

void Mass::UpdateGravity()
{
    // add gravity to the mass
    const float gravity = 9.81f;
    //const float gravity = 200.81f;
    sf::Vector3f gravForce = sf::Vector3f(0.0f, -gravity*this->mass_kg, 0.0f);
    this->AddForce(gravForce);
}
//sf::Vector2f Mass::GetGravity()
//{
//    const float gravity = 9.81f;
//    sf::Vector2f gravForce = sf::Vector2f(0.0f, gravity*this->mass_kg);
//    return gravForce;
//}

void Mass::UpdateFriction(float dt)
{
    // add frictional forces
    if(this->pos.y <= y_floor_boundary)
    {
        sf::Vector3f friction = sf::Vector3f(0.0f, 0.0f, 0.0f);
        float mu = 0.8f; //1.1f;
        float beta = 15.0f; //15.0f;
        //friction.y = 0.008f;
        //float normal_y = 0.008f;
       
        float normal_y = 0.0f;
        if(this->force_total.y <= 0.0)
        {
            normal_y = -this->force_total.y - (1.0/dt)*this->mass_kg*this->vel.y;
            if(this->print_data)
            {
                std::cout << "NORM: " << normal_y << "\n";
                std::cout << "FORCEY: " << this->force_total.y << "\n";
                std::cout << "VELY: " << this->vel.y << "\n";
            }
        }
        //friction.y = 1.0f*normal_y;
        //friction.y = -0.001f*normal_y;

        if(this->print_data)
        {
            std::cout << "VELX: " << this->vel.x << "\n";
            std::cout << "TAN: " << -tanh(beta*this->vel.x) << "\n";
        }
        friction.x = -1.0f * mu * normal_y * tanh(beta * this->vel.x);

        this->normal_force.x = friction.x;
        this->normal_force.y = normal_y; //friction.y;
        //if(this->vel.x < 0.0)
        //    friction.x = mu * normal_y; // * -tanh(beta * this->vel.x);
        //else
        //    friction.x = -mu * normal_y; // * -tanh(beta * this->vel.x);
        this->AddForceVisualise(friction);
    }
    else
    {
        this->normal_force = sf::Vector3f(0.0f, 0.0f, 0.0f);
    }
}
void Mass::UpdateTorque(Mass** masses, const int MASS_COUNT)
{
    // NOW DO MASS INTERACTIONS
    // now get all masses and assume they are connected so we apply torque forces to them all
    float new_mom_iner = 0.0f;
    for(int j = 0; j < MASS_COUNT; ++j)
    {
        if(masses[j] != this)
        {
            float distX = masses[j]->GetPosCentreX() - this->GetPosCentreX();
            float distY = masses[j]->GetPosCentreY() - this->GetPosCentreY();
            float length = sqrt(distX*distX + distY*distY);
            new_mom_iner += length*length*masses[j]->GetMass();
           
            float perpX = distY;
            float perpY = -distX;
            float f_perp = perpX*masses[j]->GetTotalForcesX() + perpY*masses[j]->GetTotalForcesY();

            //float torque = distX * masses[j]->GetTotalForcesY() - distY * masses[j]->GetTotalForcesX();
            float torque = distX * f_perp*perpX/length - distY * f_perp*perpY/length;
            std::cout << "TOR:" << torque << "\n";
            this->AddForce(sf::Vector3f(f_perp*perpX/length, f_perp*perpY/length, 0.0f)/1000.0f);
            //this->AddTorque(torque/10000.0f);
            //this->AddTorque(torque/10000.0f);
        }
    }
    this->mom_inertia = new_mom_iner;
}
void Mass::UpdateDynamics(float dt)
{
    // Leap-frog method
    //this->pos += this->vel * dt + 0.5f * dt * dt * this->acc;
    //sf::Vector2f acc_new = this->force_total / this->mass_kg;
    //this->vel += 0.5f*(this->acc + acc_new)*dt;
    //this->acc = acc_new;
    //sf::Vector2f v_half = this->vel + this->acc * dt * 0.5f;
    //this->pos += v_half * dt;
    //this->acc = this->force_total / this->mass_kg;
    //this->vel = v_half + this->acc * dt * 0.5f;

    //Runge-Kutta method
    //float k0 = this->vel;
    //float k1 = 
    //this->pos = this->pos + dt/6.0f*(k0 + 2.0f*k1 + 2.0f*k2 + k3);

    // Eluer's method
    //this->acc = this->force_total / this->mass_kg;
    //this->vel += this->acc * dt;
    //this->pos += this->vel * dt;

        //rotational (for now using Euler's Method)
    //this->rotation += this->vel_angle * dt + 0.5f * dt * dt * this->acc_angle;
    //float acc_angle_new = this->torque_total / this->mom_inertia;
    //this->vel_angle += 0.5f*(this->acc_angle + acc_angle_new)*dt;
    //this->acc_angle = acc_angle_new;

    this->acc_angle = this->torque_total / this->mom_inertia;
    this->vel_angle += dt * this->acc_angle;
    this->rotation += dt * this->vel_angle;

    //this->vel_angle *= 0.9f;

    // check rotation remains [0.0, 2*pi]
    float two_pi = 2.0f * 3.14152f;
    if(this->rotation < 0.0f)
        this->rotation += two_pi;
    else if(this->rotation > two_pi)
        this->rotation -= two_pi;

    //std::cout << "tor: " << this->torque_total << "\n";

    // check that the mass has not exceeded the floor
    //   NOTE: because the circle is a point mass, only the centre 'collides'
    //     so we don't mind if it is through the floor by its radius
    //std::cout << "POS: " << this->pos.y << "\n";
    //std::cout << "FLOOR: " << y_floor_boundary << "\n";
    if(this->pos.y <= y_floor_boundary)
    {
        this->pos.y = this->y_floor_boundary;
        this->vel.y = 0.0f;
    }
}

//Euler's Method
//sf::Vector2f Mass::Pos_t_plus_delta(sf::Vector2f pos_t, sf::Vector2f vel_t, float dt)
//{
//    sf::Vector2f pos_t_plus_delta = pos_t + vel_t * dt;
//    return pos_t_plus_delta;
//}
//sf::Vector2f Mass::Vel_t_plus_delta(sf::Vector2f vel_t, sf::Vector2f acc_t, float dt)
//{
//    sf::Vector2f vel_t_plus_delta = pos_t + acc_t * dt;
//    return pos_t_plus_delta;
//}

//Verlet Method for dynamics
//sf::Vector2f Mass::Pos_t_plus_delta(sf::Vector2f pos_t, sf::Vector2f vel_t, 
//                                    sf::Vector2f acc_t, float dt)
//{
//    sf::Vector2f pos_t_plus_delta = pos_t + vel_t * dt + 0.5f * acc_t * dt * dt;
//    return pos_t_plus_delta;
//}
//sf::Vector2f Mass::Vel_t_plus_delta(sf::Vector2f vel_t, sf::Vector2f acc_t, 
//                                    sf::Vector2f acc_t_plus_delta, float dt)
//{
//    sf::Vector2f vel_t_plus_delta = vel_t + (acc_t + acc_t_plus_delta)* 0.5f * dt;
//    return vel_t_plus_delta;
//}


void Mass::ResetForces()
{
    //// reset the force
    this->old_force_total.x = this->force_total.x;
    this->old_force_total.y = this->force_total.y;
    this->old_force_total.z = this->force_total.z;
    this->force_total.x = 0.0f;
    this->force_total.y = 0.0f;
    this->force_total.z = 0.0f;
    this->old_torque_total = torque_total;
    this->torque_total = 0.0f;
}

void Mass::Update(float dt, Mass** masses, const int MASS_COUNT)
{
    // reset state
    //this->force_total.x = 0.0f;
    //this->force_total.y = 0.0f;
    //this->torque_total = 0.0f;

    // add gravity to the mass
    const float gravity = 9.81f;
    //const float gravity = 200.81f;
    sf::Vector3f gravForce = sf::Vector3f(0.0f, gravity*this->mass_kg, 0.0f);
    this->AddForceVisualise(gravForce);

    // add frictional forces
    if(this->pos.y + this->ComputeRadius() >= y_floor_boundary)
    {
        sf::Vector3f friction = sf::Vector3f(0.0f, 0.0f, 0.0f);
        float mu = 0.8f; //1.1f;
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

        this->normal_force.x = friction.x;
        this->normal_force.y = friction.y;
        //if(this->vel.x < 0.0)
        //    friction.x = mu * normal_y; // * -tanh(beta * this->vel.x);
        //else
        //    friction.x = -mu * normal_y; // * -tanh(beta * this->vel.x);
        this->AddForceVisualise(friction);
    }
    else
    {
        this->normal_force = sf::Vector3f(0.0f, 0.0f, 0.0f);
    }
    //else
    //{
        //// add gravity to the mass
        //const float gravity = 9.81f;
        //sf::Vector2f gravForce = sf::Vector2f(0.0f, gravity*this->mass_kg);
        //this->AddForce(gravForce);
    //}


    // NOW DO MASS INTERACTIONS
    // now get all masses and assume they are connected so we apply torque forces to them all
    float new_mom_iner = 0.0f;
    for(int j = 0; j < MASS_COUNT; ++j)
    {
        if(masses[j] != this)
        {
            float distX = masses[j]->GetPosCentreX() - this->GetPosCentreX();
            float distY = masses[j]->GetPosCentreY() - this->GetPosCentreY();
            float length = sqrt(distX*distX + distY*distY);
            new_mom_iner += length*length*masses[j]->GetMass();
           
            float perpX = distY;
            float perpY = -distX;
            float f_perp = perpX*masses[j]->GetTotalForcesX() + perpY*masses[j]->GetTotalForcesY();

            //float torque = distX * masses[j]->GetTotalForcesY() - distY * masses[j]->GetTotalForcesX();
            float torque = distX * f_perp*perpX/length - distY * f_perp*perpY/length;
            std::cout << "TOR:" << torque << "\n";
            //this->AddTorque(torque/10000.0f);
            this->AddTorque(torque/1000.0f);
        }
    }
    this->mom_inertia = new_mom_iner;

    // update kinematics
        // positional
    //sf::Vector2f acc = this->force_total / this->mass_kg;
    //std::cout << "fY: " << acc.y << "\n";

    //Euler Method
    //this->vel += dt * acc;
    //this->pos += dt * this->vel;

    //Leap Frog Method
    //sf::Vector2f vel_half = this->vel + dt/2.0f * this->acc;
    //this->pos += vel_half*dt;
    //this->acc = this->force_total / this->mass_kg;
    //this->vel = vel_half + dt/2.0f * this->acc;
    this->pos += this->vel * dt + 0.5f * dt * dt * this->acc;
    sf::Vector3f acc_new = this->force_total / this->mass_kg;
    this->vel += 0.5f*(this->acc + acc_new)*dt;
    this->acc = acc_new;

        //rotational (for now using Euler's Method)
    this->acc_angle = this->torque_total / this->mom_inertia;
    this->vel_angle += dt * this->acc_angle;
    this->rotation += dt * this->vel_angle;

    // check rotation remains [0.0, 2*pi]
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

    //// reset the force
    this->old_force_total.x = this->force_total.x;
    this->old_force_total.y = this->force_total.y;
    this->old_force_total.z = this->force_total.z;
    this->force_total.x = 0.0f;
    this->force_total.y = 0.0f;
    this->force_total.z = 0.0f;
    this->old_torque_total = torque_total;
    this->torque_total = 0.0f;
}

void Mass::Draw(sf::RenderWindow& window, const float PIXEL_TO_METER, 
                sf::Vector3f POS_OFFSET)
{
    // point mass is just a point. We make this a circle (with no collisions),
    //   where the radius is proportional to the mass.
    //sf::CircleShape circ = sf::CircleShape(this->ComputeRadius());
    ////circ.setPosition(this->pos*PIXEL_TO_METER);
    //circ.setPosition(sf::Vector3f(
    //                this->pos.x*PIXEL_TO_METER - this->ComputeRadius(), 
    //                this->pos.y*PIXEL_TO_METER - this->ComputeRadius()) + POS_OFFSET);
    //circ.setFillColor(this->color);

    //window.draw(circ);
    ThreeDUtils::DrawCube(
                      sf::Vector3f(
                           this->pos.x*PIXEL_TO_METER,
                           this->pos.y*PIXEL_TO_METER,
                           this->pos.z) + POS_OFFSET,
    //ThreeDUtils::DrawCube(
    //                    sf::Vector3f(-10.0f, 0.0f, -90.0f),
                        this->ComputeRadius(),
                        this->color,
                        //sf::Vector3f(this->rotation, 0.0f, 0.0f));
                        sf::Vector3f(0.0f, 0.0f, this->rotation));

    // draw a line denoting rotation
    float rot_x = cos(this->rotation) * this->ComputeRadius();
    float rot_y = sin(this->rotation) * this->ComputeRadius();
    sf::Color c_rot = sf::Color(0.0f, 0.0f, 0.0f);
    //sf::Vertex floor[] = 
    //{
    //    sf::Vertex(sf::Vector3f(this->GetPosCentreX(), this->GetPosCentreY())*PIXEL_TO_METER + POS_OFFSET, c_rot),
    //    sf::Vertex(sf::Vector3f(this->GetPosCentreX()*PIXEL_TO_METER + rot_x, this->GetPosCentreY()*PIXEL_TO_METER + rot_y) + POS_OFFSET, c_rot)
    //};
    ThreeDUtils::DrawLine(
            sf::Vector3f(this->GetPosCentreX()*PIXEL_TO_METER, 
                            this->GetPosCentreY()*PIXEL_TO_METER, 
                            this->GetPosCentreZ()),
            sf::Vector3f(this->GetPosCentreX()*PIXEL_TO_METER + rot_x, 
                            this->GetPosCentreY()*PIXEL_TO_METER + rot_y,
                            this->GetPosCentreZ()),
            c_rot);

    //window.draw(floor, 2, sf::Lines);

    // draw debug forces
    if(this->debug_forces)
    {
        sf::Vector3f cent = sf::Vector3f(
                            this->GetPosCentreX()*PIXEL_TO_METER, 
                            this->GetPosCentreY()*PIXEL_TO_METER,
                            this->GetPosCentreZ());
        sf::Color f_col = sf::Color(255.0f, 125.0f, 0.0f, 255.0f);
        for(int f = 0; f < this->arr_idx; ++f)
        {
            //sf::Vertex force[] =
            //{
            //    sf::Vertex(cent, f_col),
            //    sf::Vertex(cent + 
            //            sf::Vector3f(this->forces_list[f].x, this->forces_list[f].y)*PIXEL_TO_METER, f_col)
            //};
            //window.draw(force, 2, sf::Lines);
            ThreeDUtils::DrawLine(
                    cent,
                    cent + sf::Vector3f(
                        this->forces_list[f].x*PIXEL_TO_METER,
                        this->forces_list[f].y*PIXEL_TO_METER,
                        this->forces_list[f].z
                        ),
                    f_col
                    );
        }
        this->arr_idx = 0;
        ThreeDUtils::DrawLine(
                cent,
                cent + sf::Vector3f(
                    this->normal_force.x*PIXEL_TO_METER,
                    this->normal_force.y*PIXEL_TO_METER,
                    this->normal_force.z),
                f_col
                );
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

void Mass::DrawData(sf::RenderWindow& window, sf::Vector2f position)
{
    // load the font
    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf");
    std::stringstream ss;
    ss << "--------------------------------------------------\n";

    ss << "pos: (" << this->pos.x << ", " << this->pos.y << ")\n";
    ss << "vel: (" << this->vel.x << ", " << this->vel.y << ")\n";
    ss << "acc: (" << this->acc.x << ", " << this->acc.y << ")\n";
    ss << "force: (" << this->old_force_total.x << ", " << this->old_force_total.y << ")\n";
    ss << "angle: " << this->rotation << "\n";
    ss << "a_vel: " << this->vel_angle << ")\n";
    ss << "a_acc: " << this->acc_angle << ")\n";
    ss << "tor: " << this->old_torque_total << ")\n";
    ss << "mom iner: " << this->mom_inertia << ")\n";

    ss << "--------------------------------------------------\n";
    sf::Text text;
    text.setFont(font);
    text.setString(ss.str());
    text.setCharacterSize(10);
    text.setColor(this->color);
    text.setPosition(position);

    window.draw(text);
}

float Mass::ComputeAccX()
{
    return this->force_total.x / this->mass_kg;
}
float Mass::ComputeAccY()
{
    return this->force_total.y / this->mass_kg;
}
float Mass::ComputeAccZ()
{
    return this->force_total.z / this->mass_kg;
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
float Mass::GetPosZ()
{
    return this->pos.z;
}
void Mass::SetStationary(sf::Vector3f position)
{
    this->pos = position;
    this->vel.x = 0.0f;
    this->vel.y = 0.0f;
    this->vel.z = 0.0f;
    this->acc.x = 0.0f;
    this->acc.y = 0.0f;
    this->acc.z = 0.0f;
}

float Mass::GetPosCentreX()
{
    return this->pos.x; //+ this->ComputeRadius();
}
float Mass::GetPosCentreY()
{
    return this->pos.y; // + this->ComputeRadius();
}
float Mass::GetPosCentreZ()
{
    return this->pos.z; // + this->ComputeRadius();
}
void Mass::SetPos(sf::Vector3f value)
{
    this->pos = value;
}
float Mass::GetVelX()
{
    return this->vel.x;
}
float Mass::GetVelY()
{
    return this->vel.y;
}
float Mass::GetVelZ()
{
    return this->vel.z;
}
void Mass::SetVel(sf::Vector3f value)
{
    this->vel = value;
}
float Mass::GetAccX()
{
    return this->acc.x;
}
float Mass::GetAccY()
{
    return this->acc.y;
}
float Mass::GetAccZ()
{
    return this->acc.z;
}
float Mass::GetRotation()
{
    return this->rotation;
}
void Mass::SetRotation(float value) //TODO: bad function should use physics
{
    this->rotation = value;
}
float Mass::GetNormalX()
{
    return this->normal_force.x;
}
float Mass::GetNormalY()
{
    return this->normal_force.y;
}
float Mass::GetNormalZ()
{
    return this->normal_force.z;
}
float Mass::GetTotalForcesX()
{
    return this->force_total.x;
}
float Mass::GetTotalForcesY()
{
    return this->force_total.y;
}
float Mass::GetTotalForcesZ()
{
    return this->force_total.z;
}
float Mass::GetMass()
{
    return this->mass_kg;
}
void Mass::SetMomOfInertia(float value)
{
    this->mom_inertia = value;
}

// private
float Mass::ComputeRadius()
{
    //return std::min(this->mass_kg * 80.5f, 15.0f);
    return std::min(this->mass_kg * 7.5f, 2.0f);
}
