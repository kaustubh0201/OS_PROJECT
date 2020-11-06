 #include <gui/window.h>

using namespace osd::common;
using namespace osd::gui;

Window::Window(Widget* parent,
            common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
            common::uint8_t r, common::uint8_t g, common::uint8_t b)
: CompositeWidget(parent, x,y,w,h, r,g,b)
{
    Dragging = false; // we are initializing it with false
}

Window::~Window()
{
}

void Window::OnMouseDown(common::int32_t x, common::int32_t y, common::uint8_t button)
{
    Dragging = button == 1; // clicking left mouse button will make it one i.e. true
    CompositeWidget::OnMouseDown(x,y,button);
    // the widgets that the window contain will be notified about the dragging 
}

void Window::OnMouseUp(common::int32_t x, common::int32_t y, common::uint8_t button)
{
    Dragging = false; // we are also setting it to false when button is not held on the mouse
    CompositeWidget::OnMouseUp(x,y,button); 
    // the widgets that the window contain will be notified about the dragging 
}

void Window::OnMouseMove(common::int32_t oldx, common::int32_t oldy, common::int32_t newx, common::int32_t newy)
{
    // we will be changing the 
    if(Dragging)
    {
        this->x += newx-oldx;
        this->y += newy-oldy;
    }
    CompositeWidget::OnMouseMove(oldx,oldy,newx, newy);
    // the widgets that the window contain will be notified about the dragging 

}

    