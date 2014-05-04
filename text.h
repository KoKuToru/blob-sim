#ifndef TEXT_H
#define TEXT_H

#include "renderable.h"
#include "point.h"
#include <string>

class text: public renderable
{
	private:
		point m_position;
		std::string m_text;
	public:
		text(const std::string& text);
		void setPosition(const point& p);
		~text() = default;
		void render() const;
};

#endif // TEXT_H
