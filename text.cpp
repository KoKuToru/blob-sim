#include "text.h"

text::text(const std::string& message, float size):
    m_text(message), m_size(size) {
}

text::text(const point &origin_, const std::string &message, float size):
    m_origin(origin_), m_text(message), m_size(size) {

}

text& text::origin(const point& origin_) {
    m_origin = origin_;
    return *this;
}

const point& text::origin() const {
    return m_origin;
}

text& text::size(float size_) {
    m_size = size_;
    return *this;
}

float text::size() const {
    return m_size;
}

text& text::message(std::string& text) {
    m_text = text;
    return *this;
}

const std::string& text::message() const {
    return m_text;
}

void text::render() const{
    const char * p = this->m_text.c_str();
    glDisable( GL_DEPTH_TEST ) ; // also disable the depth test so renders on top
    glPushMatrix();
    glTranslatef(this->m_origin.x(), this->m_origin.y(), this->m_origin.layer());
    glScalef(m_size, m_size, 1); //font size
    //glRotatef(10.0, 0, 0, 1);
    glColor4f(colorR(), colorG(), colorB(), colorA());
    do glutStrokeCharacter( GLUT_STROKE_ROMAN, *p ); while( *(++p) ) ; //GLUT_STROKE_MONO_ROMAN fix width
    glPopMatrix();
    glEnable( GL_DEPTH_TEST ) ; // Turn depth testing back on
}
