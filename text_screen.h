#ifndef TEXT_SCREEN_H
#define TEXT_SCREEN_H

#include "renderable.h"
#include "text.h"

class text_screen: public text
{
    public:
        using text::text;
        void render(); //override
};

#endif // TEXT_SCREEN_H 
