#include "text.h"

text::text(const std::string& text){
	this->m_text = text;
}

void text::setPosition(const point& p){
	this->m_position = p;
}

void text::render() const{
	const char * p = this->m_text.c_str();
	glDisable( GL_DEPTH_TEST ) ; // also disable the depth test so renders on top
	glPushMatrix();
	glTranslatef(this->m_position.x(), this->m_position.y(), this->m_position.layer());
	glScalef(0.2,0.2,1); //font size
	glLineWidth(2);
	glColor4f(colorR(), colorG(), colorB(), 1.0f);
	do glutStrokeCharacter( GLUT_STROKE_ROMAN, *p ); while( *(++p) ) ; //GLUT_STROKE_MONO_ROMAN fix width
	glPopMatrix();
	glEnable( GL_DEPTH_TEST ) ; // Turn depth testing back on
}
