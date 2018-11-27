#include "platform.h"

Platform::Platform(sf::Vector3f origin, float boundary_size_x, int num_panels)
{
    this->origin = origin;
    this->boundary_size_x = boundary_size_x;
    this->num_panels = num_panels;
    this->panel_rate = 2.0f*this->boundary_size_x/(float)num_panels;
    this->panels = new sf::Vector3f[this->num_panels];
    for(int i = 0; i < this->num_panels; ++i)
    {
        this->panels[i] = sf::Vector3f(
                    this->origin.x - this->boundary_size_x + this->panel_rate*i,
                    this->origin.y,
                    this->origin.z);
    }
}

Platform::~Platform()
{
    delete[] this->panels;
}

void Platform::Update(float dt)
{
    for(int i = 0; i < this->num_panels; ++i)
    {
        // if a panel position goes beyond the edge,
        //  move it to the other side
        if(this->panels[i].x < this->origin.x - this->boundary_size_x)
            this->panels[i].x += 2.0f*this->boundary_size_x;
    }
}

void Platform::Draw(sf::RenderWindow& window, float PIXEL_TO_METER)
{
    float panel_size_x = this->panel_rate * 0.4f;
    float panel_size_y = 10.0f;
    for(int i = 0; i < this->num_panels; ++i)
    {
        ThreeDUtils::DrawPlane(
                sf::Vector3f(
                    this->panels[i].x*PIXEL_TO_METER, 
                    this->panels[i].y*PIXEL_TO_METER, 
                    this->panels[i].z), 
                    panel_size_x*PIXEL_TO_METER, panel_size_y,
                    sf::Vector3f(0.2f, 0.2f, 0.2f));
    }
}

//get/set
void Platform::SetOrigin(sf::Vector3f origin)
{
    this->origin = origin;
}
