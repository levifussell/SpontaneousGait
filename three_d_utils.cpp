#include "three_d_utils.h"

namespace ThreeDUtils
{
    float RadToDeg(float rad)
    {
        return rad*180.0f/3.14152f;
    }

    void DrawCube(sf::Vector3f position,
                            float size,
                            sf::Color colour,
                            sf::Vector3f rotation)
    {
        glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        glRotatef(RadToDeg(rotation.x), 1.0f, 0.0f, 0.0f);
        glRotatef(RadToDeg(rotation.y), 0.0f, 1.0f, 0.0f);
        glRotatef(RadToDeg(rotation.z), 0.0f, 0.0f, 1.0f);
        
        glBegin(GL_QUADS);
            glColor3f(colour.r, colour.g, colour.b);
            //glColor3f(1.0f, 1.0f, 1.0f);

            glVertex3f(-size, -size, -size);
            glVertex3f(-size, +size, -size);
            glVertex3f(+size, +size, -size);
            glVertex3f(+size, -size, -size);

            glVertex3f(-size, -size, +size);
            glVertex3f(-size, +size, +size);
            glVertex3f(+size, +size, +size);
            glVertex3f(+size, -size, +size);

            glVertex3f(-size, -size, -size);
            glVertex3f(-size, +size, -size);
            glVertex3f(-size, +size, +size);
            glVertex3f(-size, -size, +size);

            glVertex3f(+size, -size, -size);
            glVertex3f(+size, +size, -size);
            glVertex3f(+size, +size, +size);
            glVertex3f(+size, -size, +size);

            glVertex3f(-size, -size, +size);
            glVertex3f(-size, -size, -size);
            glVertex3f(+size, -size, -size);
            glVertex3f(+size, -size, +size);

            glVertex3f(-size, +size, +size);
            glVertex3f(-size, +size, -size);
            glVertex3f(+size, +size, -size);
            glVertex3f(+size, +size, +size);
        glEnd();

        glPopMatrix();
    }

    void DrawPlane(sf::Vector3f position,
                    float sizeX, float sizeY,
                    sf::Vector3f colour)
    {
        glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        glBegin(GL_QUADS);
            glColor3f(colour.x, colour.y, colour.z);
            glVertex3f(-sizeX, 0.0f, -sizeY);
            glVertex3f(+sizeX, 0.0f, -sizeY);
            glVertex3f(+sizeX, 0.0f, +sizeY);
            glVertex3f(-sizeX, 0.0f, +sizeY);
        glEnd();

        glPopMatrix();
    }

    void DrawLine(sf::Vector3f start_position,
                    sf::Vector3f end_position,
                    sf::Color colour)
    {
        glBegin(GL_LINES);
            glColor3f(colour.r, colour.g, colour.b);
            glVertex3f(start_position.x, start_position.y, start_position.z);
            glVertex3f(end_position.x, end_position.y, end_position.z);
        glEnd();
    }
}
