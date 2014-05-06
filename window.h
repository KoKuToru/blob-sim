#ifndef WINDOW_H
#define WINDOW_H

#include <string>

class window
{
    private:
        int m_width;
        int m_height;
        static window* s_instance;

    public:
        window(const std::string &title, int w=640, int h=480);
        virtual ~window() = default;
        virtual void onRender() = 0;
        virtual void onResize(int w, int h) = 0;
        virtual void onScroll(int dir) = 0;
        virtual void onMouse(int btn, int state, int x, int y) = 0;
        virtual void onMouseMotion(int x, int y) = 0;
        void loop();

        int width() const;
        int height() const;
};

#endif // WINDOW_H
