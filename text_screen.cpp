#include "text_screen.h"
#include "window.h"

void text_screen::render(){
    glPushMatrix();
    glLoadIdentity();
    glScalef(2.0/window::s_instance->m_width, 2.0/window::s_instance->m_height, 1);
    glTranslatef(-window::s_instance->m_width/2.0, -window::s_instance->m_height/2.0, 0);
    text::render();
    glPopMatrix();
}
