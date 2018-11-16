#include "controller.h"

Controller::Controller() {}

Controller::Controller(float FLOOR_Y)
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

void Controller::InitModel(float FLOOR_Y)
{
    ////dog:
    //float startX = 600.0f;
    //float startY = 600.0f;
    float startX = 1.0f;
    float startY = 1.0f;
        //body:
    
    float body_length = 0.19f;
    float body_rate = body_length / 4.0f;
    float mass_shoulder = 0.6f;
    float mass_hip = 0.4f;
    float mass_spine = 0.1f;
    float mass_knee = 0.075f;
    float mass_foot = 0.015f;
    this->leg_offset = sf::Vector2f(0.0f, 0.08f);
    float leg_rate = this->leg_offset.y/2.0f;
    this->leg_amp = sf::Vector2f(0.02f, 0.016f);
    this->alpha_ground_scalar = 1.0f;
    float k_pri = 3.75*pow(10.0f,3);
    float d_pri = 1.0*pow(10.0f,2);
    float k_link = 1.25*pow(10.0f,4);
    float d_link = 1.0*pow(10.0f,2);
    float k_rot = 8.0*pow(10.0f,1);
    float d_rot = 1.6*pow(10.0f,-2);
    float k_spine = 8.0*pow(10.0f,1);
    float d_spine = 1.6*pow(10.0f,-2);

    //float mass_shoulder = 0.8f;
    //float mass_hip = 0.8f;
    //float mass_spine = 0.3f;
    //float mass_knee = 0.075f;
    //float mass_foot = 0.035f;
    //float k_b = 890.0f; //30.0f; //180.0f;
    //float d_b = 3.0f; //2.5f; //20.0f;
    //float t_k_b = 3480.0f; //30.0f; //180.0f;
    //float t_d_b = 20.0f; //2.5f; //20.0f;
    //float k_b = 3750.0f;
    //float d_b = 100.0f;
    this->masses[0] = new Mass(sf::Vector2f(startX, startY), mass_shoulder, FLOOR_Y);
    this->masses[1] = new Mass(sf::Vector2f(startX+body_rate, startY), mass_spine, FLOOR_Y);
    this->masses[2] = new Mass(sf::Vector2f(startX+body_rate*2.0f, startY), mass_spine, FLOOR_Y);
    this->masses[3] = new Mass(sf::Vector2f(startX+body_rate*3.0f, startY), mass_spine, FLOOR_Y);
    this->masses[4] = new Mass(sf::Vector2f(startX+body_rate*4.0f, startY), mass_shoulder, FLOOR_Y);
    //float k_rigid = 880.0f; //12500.0f;
    //float d_rigid = 20.0f;
    //float t_k_rigid = 100.0f;
    //float t_d_rigid = 100.0f;

    //float t_k = 0.0f; //15.9f;
    //float t_d = 0.0f; //1.1f;
    //float t_k_spine = 8.0f;
    //float t_d_spine = 0.016f;
    //float t_k_spine = 3880.0f;
    //float t_d_spine = 120.6f;
    //float body_length = 220.0f;
    this->springs[0] = new Spring(body_rate, k_link, d_link, 
                        0.0f, k_spine, d_spine, 1.0f,
                        this->masses[0], this->masses[1]);
    this->springs[1] = new Spring(body_rate, k_link, d_link, 
                        0.0f, k_spine, d_spine, 1.0f, 
                        this->masses[1], this->masses[2]);
    this->springs[2] = new Spring(body_rate, k_link, d_link, 
                        0.0f, k_spine, d_spine, 1.0f,
                        this->masses[2], this->masses[3]);
    this->springs[3] = new Spring(body_rate, k_link, d_link, 
                        0.0f, k_spine, d_spine, 1.0f,
                        this->masses[3], this->masses[4]);

    //float knee_offset_x = 10.0f;
    //float knee_offset_y = 50.0f;
    //float foot_offset_x = 10.0f;
    //float foot_offset_y = 70.0f;
    //float femur_size = 40.0f;
    //float shin_size = 20.0f;
    sf::Color c_red = sf::Color(255.0f, 0.0f, 0.0f, 255.0f);
    sf::Color c_green = sf::Color(0.0f, 255.0f, 0.0f, 255.0f);
        //leg left-front
    this->masses[5] = new Mass(sf::Vector2f(startX, startY + leg_rate), 
                                mass_knee, FLOOR_Y, c_red);
    this->masses[6] = new Mass(sf::Vector2f(startX, startY + leg_rate*2.0f), 
                                mass_foot, FLOOR_Y, c_red);
    this->springs[4] = new Spring(leg_rate, k_link, d_link, 
                            3.1415f/2.3f, k_rot, d_rot, 1.0f,
                            this->masses[0], this->masses[5]);
    this->springs[5] = new Spring(leg_rate, k_pri, d_pri, 
                            0.0f, k_rot, d_rot, 1.0f,
                            this->masses[5], this->masses[6]);

        //leg right-front
    this->masses[7] = new Mass(sf::Vector2f(startX + body_length, startY + leg_rate), 
                                mass_knee, FLOOR_Y, c_red);
    this->masses[8] = new Mass(sf::Vector2f(startX + body_length, startY + leg_rate*2.0f), 
                                mass_foot, FLOOR_Y, c_red);
    this->springs[6] = new Spring(leg_rate, k_link, d_link, 
                            3.1415f/2.3f, k_rot, d_rot, 1.0f,
                            this->masses[4], this->masses[7]);
    this->springs[7] = new Spring(leg_rate, k_pri, d_pri, 
                            0.0f, k_rot, d_rot, 1.0f,
                            this->masses[7], this->masses[8]);

        //leg left-back
    this->masses[9] = new Mass(sf::Vector2f(startX, startY + leg_rate), 
                                mass_knee, FLOOR_Y, c_green);
    this->masses[10] = new Mass(sf::Vector2f(startX, startY + leg_rate*2.0f), 
                                mass_foot, FLOOR_Y, c_green);
    this->springs[8] = new Spring(leg_rate, k_link, d_link, 
                            3.1415f/1.7f, k_rot, d_rot, 1.0f,
                            this->masses[0], this->masses[9]);
    this->springs[9] = new Spring(leg_rate, k_pri, d_pri, 
                            0.0f, k_rot, d_rot, 1.0f,
                            this->masses[9], this->masses[10]);

        //leg right-back
    this->masses[11] = new Mass(sf::Vector2f(startX + body_length, startY + leg_rate), 
                                mass_knee, FLOOR_Y, c_green);
    this->masses[12] = new Mass(sf::Vector2f(startX + body_length, startY + leg_rate*2.0f), 
                                mass_foot, FLOOR_Y, c_green);
    this->springs[10] = new Spring(leg_rate, k_link, d_link, 
                            3.1415f/1.7f, k_rot, d_rot, 1.0f,
                            this->masses[4], this->masses[11]);
    this->springs[11] = new Spring(leg_rate, k_pri, d_pri, 
                            0.0f, k_rot, d_rot, 1.0f,
                            this->masses[11], this->masses[12]);

    // set other leg parameters
    this->leg_phases[0] = 3.1415f/1.7f;
    this->leg_phases[1] = 3.1415f/2.3f;
    this->leg_phases[2] = 3.1415f/1.7f;
    this->leg_phases[3] = 3.1415f/2.3f;

    //this->leg_offset = sf::Vector2f(0.0f, 60.0f);
    //this->leg_amp = sf::Vector2f(0.02f, 0.016f);
    //this->leg_amp = sf::Vector2f(20.0f, 20.0f);
    //this->alpha_ground_scalar = 1.0f;

    this->leg_angular_velocity = 0.0f;
    this->support_scalar = 0.5f;
    this->propulsion_scalar = 0.0f; //0.5f;
    this->settle_angle = 3.14152f/2.0f;

    // create lookup table for legs names to joints
    this->leg_names[0] = "left_front";
    this->leg_joint_indices[0][0] = 4;
    this->leg_joint_indices[0][1] = 5;
    this->leg_joint_indices[0][2] = 6;
    this->leg_names[1] = "left_back";
    this->leg_joint_indices[1][0] = 8;
    this->leg_joint_indices[1][1] = 9;
    this->leg_joint_indices[1][2] = 10;
    this->leg_names[2] = "right_front";
    this->leg_joint_indices[2][0] = 6;
    this->leg_joint_indices[2][1] = 7;
    this->leg_joint_indices[2][2] = 8;
    this->leg_names[3] = "right_back";
    this->leg_joint_indices[3][0] = 10;
    this->leg_joint_indices[3][1] = 11;
    this->leg_joint_indices[3][2] = 12;

    // load the font
    this->font;
    this->font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf");
}

void Controller::SetSpringLength(int spring_idx, float target_length)
{
    this->springs[spring_idx]->SetLength(target_length);
}
void Controller::SetSpringAngle(int spring_idx, float target_angle)
{
    this->springs[spring_idx]->SetAngle(target_angle);
}
void Controller::SetLegTarget(int femur_idx, int knee_idx, float phase)
{
    // first compute target position from the phase
    sf::Vector2f target = sf::Vector2f(0.0f, 0.0f);
    target.x = this->leg_offset.x + this->leg_amp.x * cos(phase);
    if(sin(phase) > 0)
        target.y = this->leg_offset.y + this->leg_amp.y * sin(phase);
    else
        target.y = this->leg_offset.y + this->alpha_ground_scalar * this->leg_amp.y * sin(phase);

    // now compute target angles and lengths from the target position
    float length = sqrt(target.x*target.x + target.y*target.y);
    float target_angle = asin(target.x/length);
    float target_length = length - this->springs[femur_idx]->GetCurrentLength();
    this->SetSpringAngle(femur_idx, target_angle + this->settle_angle);
    this->SetSpringLength(knee_idx, target_length);
}
void Controller::UpdateLegPhase(std::string leg_name, float dt)
{
    // get leg index
    int idx = this->GetLegIndexFromName(leg_name);
    int mass_idx = this->leg_joint_indices[idx][2];

    // compute components of Tegotae
    float support = this->support_scalar * this->masses[mass_idx]->GetNormalY() * cos(this->leg_phases[idx]);
    float propulsion = this->propulsion_scalar * this->masses[mass_idx]->GetNormalX() * cos(this->leg_phases[idx]);
    float phase_update = this->leg_angular_velocity - support - propulsion;
    std::cout << "support: " << support << "\n";
    std::cout << "propulsion: " << propulsion << "\n";

    // update phase using Euler's Method
    this->leg_phases[idx] += dt * phase_update;
}

void Controller::SetLegTarget(std::string leg_name, float phase)
{
    // get leg index
    int idx = this->GetLegIndexFromName(leg_name);

    // set leg target
    this->SetLegTarget(this->leg_joint_indices[idx][0],
                        this->leg_joint_indices[idx][1],
                        phase);
}

void Controller::MoveLeftFront(float target_length, float target_angle)
{
    this->SetSpringLength(5, target_length);
    this->SetSpringAngle(4, target_angle);
}
void Controller::MoveLeftBack(float target_length, float target_angle)
{
    this->SetSpringLength(9, target_length);
    this->SetSpringAngle(8, target_angle);
}
void Controller::MoveRightFront(float target_length, float target_angle)
{
    this->SetSpringLength(7, target_length);
    this->SetSpringAngle(6, target_angle);
}
void Controller::MoveRightBack(float target_length, float target_angle)
{
    this->SetSpringLength(11, target_length);
    this->SetSpringAngle(10, target_angle);
}

void Controller::Update(float dt)
{
    // update leg movement
    //for(int i = 0; i < 4; ++i)
    //{
    //    this->UpdateLegPhase(this->leg_names[i], dt);
    //    this->SetLegTarget(this->leg_names[i], this->leg_phases[i]);
    //}

    

    for(int i = 0; i < MASS_COUNT; ++i)
        this->masses[i]->UpdateGravity();
    for(int i = 0; i < SPRING_COUNT; ++i)
        this->springs[i]->Update(dt);
    //for(int i = 0; i < MASS_COUNT; ++i)
    //    this->masses[i]->UpdateFriction(dt);
    for(int i = 0; i < MASS_COUNT; ++i)
        this->masses[i]->UpdateDynamics(dt);
    for(int i = 0; i < MASS_COUNT; ++i)
        this->masses[i]->ResetForces();

    //for(int i = 0; i < MASS_COUNT; ++i)
    //    this->masses[i]->AddForce(sf::Vector2f(2.0f, 0.0f));

    //for(int i = 0; i < MASS_COUNT; ++i)
    //    this->masses[i]->UpdateTorque(this->masses, MASS_COUNT);


}

void Controller::Draw(sf::RenderWindow& window, const float PIXEL_TO_METER)
{
    for(int i = 0; i < MASS_COUNT; ++i)
    {
        this->masses[i]->Draw(window, PIXEL_TO_METER);
    }

    for(int i = 0; i < SPRING_COUNT; ++i)
    {
        this->springs[i]->Draw(window, PIXEL_TO_METER);
    }
}

void Controller::DrawData(sf::RenderWindow& window, sf::Vector2f position)
{
    for(int i = 0; i < 4; ++i)
    {
        Mass* foot_mass = this->masses[this->leg_joint_indices[i][2]];
        Spring* shin_spring = this->springs[this->leg_joint_indices[i][1]];
        std::stringstream ss;
        ss << this->leg_names[i] << "\n";
        ss << "--------------------------------------------------\n";
         
        ss << "norm: (" << foot_mass->GetNormalX() << ", " << foot_mass->GetNormalY() << ")\n";
        //ss << "norm Y: (" << foot_mass->GetNormalY() << ")\n";
        ss << "phase: (" << this->leg_phases[i] << ")\n";
        ss << "shin length: (" << shin_spring->GetCurrentLength() << ")\n";

        ss << "--------------------------------------------------\n";
        sf::Text text;
        text.setFont(this->font);
        text.setString(ss.str());
        text.setCharacterSize(10);
        text.setColor(sf::Color::White);
        text.setPosition(position + sf::Vector2f(170.0f*i, 0.0f));

        window.draw(text);
    }
}

// utilities
int Controller::GetLegIndexFromName(std::string leg_name)
{
    int idx = -1;
    for(int i = 0; i < 4; ++i)
    {
        if(leg_name.compare(this->leg_names[i]) == 0)
        {
            idx = i;
            break;
        }
    }

    return idx;
}

// get/set
void Controller::SetLegAngularVelocity(float value)
{
    this->leg_angular_velocity = value;
}
