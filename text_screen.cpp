#include "text_screen.h"

void text_screen::render(){
    glPushMatrix();
    glLoadIdentity();
    text::render();
    glPopMatrix();
}
