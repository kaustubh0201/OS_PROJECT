#include <gui/widget.h>

using namespace osd::common;
using namespace osd::gui;

Widget::Widget(Widget* parent, osd::common::int32_t x, osd::common::int32_t y, osd::common::int32_t w, osd::common::int32_t h, osd::common::uint8_t r, osd::common::uint8_t g, osd::common::uint8_t b)
: KeyboardEventHandler()
{
    // we will be making windows which are having child windows in there
    this -> parent = parent;
    this -> x = x;
    this -> y = y;
    this -> w = w;
    this -> h = h;
    this -> r = r;
    this -> g = g;
    this -> b = b;
}

Widget :: ~Widget()
{

}

void Widget ::GetFocus(Widget* widget)
{
    if(parent != 0)
        parent -> GetFocus(widget);
}

void Widget ::ModelToScreen(osd::common::int32_t &x, osd::common::int32_t &y)
{
    if(parent != 0)
        parent -> ModelToScreen(x, y);
        x += this -> x;
        y += this -> y;
}

void Widget ::Draw(GraphicsContext* gc)
{
    int X = 0;
    int Y = 0;
    ModelToScreen(X, Y);
    gc -> FillRectangle(X, Y, w, h, r, g, b);
}

void Widget ::OnMouseDown(osd::common::int32_t x, osd::common::int32_t y, osd::common::uint8_t button)
{
    if(Focussable)
        GetFocus(this);
}

void Widget ::OnMouseUp(osd::common::int32_t x, osd::common::int32_t y, osd::common::uint8_t button)
{

}

void Widget ::OnMouseMove(osd::common::int32_t oldx, osd::common::int32_t oldy, osd::common::int32_t newx, osd::common::int32_t newy)
{

}


bool Widget::ContainsCoordinate(common::int32_t x, common::int32_t y)
{
    return this->x <= x && x < this->x + this->w
        && this->y <= y && y < this->y + this->h;
}

CompositeWidget::CompositeWidget(Widget* parent, osd::common::int32_t x, osd::common::int32_t y, osd::common::int32_t w, osd::common::int32_t h, osd::common::uint8_t r, osd::common::uint8_t g, osd::common::uint8_t b)
: Widget(parent, x,y,w,h, r,g,b)
{
    focussedChild = 0;
    numChildren = 0;
}
        
CompositeWidget::~CompositeWidget()
{

}

void CompositeWidget::GetFocus(Widget* widget)
{
    this -> focussedChild = widget;
    if(parent != 0)
        parent -> GetFocus(this);
}

bool CompositeWidget::AddChild(Widget* child)
{
    if(numChildren >= 100)
        return false;
    children[numChildren++] = child;
    return true;
}

void CompositeWidget::Draw(osd::common::GraphicsContext* gc)
{
    Widget::Draw(gc);
    for(int i = numChildren - 1; i >= 0; i--)
        children[i] -> Draw(gc);
}

void CompositeWidget::OnMouseDown(osd::common::int32_t x, osd::common::int32_t y, osd::common::uint8_t button)
{
    for(int i = 0; i < numChildren; i++)
    {
        if(children[i] -> ContainsCoordinate(x - this -> x, y - this -> y))
        {
            children[i] -> OnMouseDown(x - this -> x, y - this -> y, button);
            break;
        }
    }
}
void CompositeWidget::OnMouseUp(osd::common::int32_t x, osd::common::int32_t y, osd::common::uint8_t button)
{
    for(int i = 0; i < numChildren; i++)
    {
        if(children[i] -> ContainsCoordinate(x - this -> x, y - this -> y))
        {
            children[i] -> OnMouseDown(x - this -> x, y - this -> y, button);
            break;
        }
    }
}
void CompositeWidget::OnMouseMove(osd::common::int32_t oldx, osd::common::int32_t oldy, osd::common::int32_t newx, osd::common::int32_t newy)
{
    int firstchild = -1;
    for(int i = 0; i < numChildren; i++)
    {
        if(children[i] -> ContainsCoordinate(oldx - this->x, oldy - this->y))
        {
            children[i] -> OnMouseMove(oldx - this->x, oldy - this->y, newx - this->x, newy - this->y);
            break;
        }
    }

    for(int i = 0; i < numChildren; i++)
    {
        if(children[i] -> ContainsCoordinate(oldx - this->x, oldy - this->y))
        {
            if(firstchild != i)
                children[i] -> OnMouseMove(oldx - this->x, oldy - this->y, newx - this->x, newy - this->y);
            break;
        }
    }
}

void CompositeWidget::OnKeyDown(char str)
{
    if(focussedChild != 0)
        focussedChild -> OnKeyDown(str);
}

void CompositeWidget::OnKeyUp(char str)
{
    if(focussedChild != 0)
        focussedChild -> OnKeyUp(str);
}