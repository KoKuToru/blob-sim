#ifndef TEXT_H
#define TEXT_H

#include "renderable.h"
#include "point.h"
#include <string>

class text: public renderable
{
    private:
        point       m_origin;
        std::string m_text;
        float       m_size;
    public:
        text(const std::string& message=std::string(), float size=1);
        text(const point &origin_, const std::string &message=std::string(), float size=1);
        text& origin(const point& origin_);
        const point& origin() const;
        text& size(float size_);
        float size() const;
        text& message(std::string& text);
        const std::string& message() const;
        ~text() = default;
        void render() const;
};

#endif // TEXT_H
