#ifndef TEXT_H
#define TEXT_H

#include "renderable.h"
#include "point.h"

class text: public renderable
{
	private:
		point* m_position;
		const char* m_text;
	public:
		text(const char* text);
		void setPosition(point* p);
		~text() = default;
		void render() const;
};

#endif // TEXT_H
