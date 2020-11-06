 #include <gui/desktop.h>

using namespace osd;
using namespace osd::common;
using namespace osd::gui;


Desktop::Desktop(osd::common::int32_t w, osd::common::int32_t h, osd::common::uint8_t r, osd::common::uint8_t g, common::uint8_t b)
:   CompositeWidget(0,0,0, w,h,r,g,b),
    MouseEventHandler()
{
    // w is width and h is the height
    MouseX = w/2;
    MouseY = h/2;
}

Desktop :: ~Desktop()
{

}

void Desktop::Draw(osd::common::GraphicsContext* gc)
{
    // this draw is for drawing the desktop
    CompositeWidget::Draw(gc);
    
    // here we are drawing the mouse cursor that will
    // be shown on the screen
    for(int i = 0; i < 4; i++)
    {
        gc -> PutPixel(MouseX-i, MouseY, 0xFF, 0xFF, 0xFF);
        gc -> PutPixel(MouseX+i, MouseY, 0xFF, 0xFF, 0xFF);
        gc -> PutPixel(MouseX, MouseY-i, 0xFF, 0xFF, 0xFF);
        gc -> PutPixel(MouseX, MouseY+i, 0xFF, 0xFF, 0xFF);
    }
}
            
void Desktop::OnMouseDown(osd::common::uint8_t button)
{
    CompositeWidget::OnMouseDown(MouseX, MouseY, button); 
    // we are accessing the composite widget's event handler
}

void Desktop::OnMouseUp(osd::common::uint8_t button)
{
    CompositeWidget::OnMouseUp(MouseX, MouseY, button);
    // we are accessing the composite widget's event handler
}

void Desktop::OnMouseMove(int x, int y)
{
    x /= 4;
    y /= 4;
    
    int32_t newMouseX = MouseX + x;
    if(newMouseX < 0) newMouseX = 0; // constraints so cursor don't get outside
    if(newMouseX >= w) newMouseX = w - 1;
    
    int32_t newMouseY = MouseY + y;
    if(newMouseY < 0) newMouseY = 0; // constraints so that cursor don't get outside
    if(newMouseY >= h) newMouseY = h - 1;
    
    CompositeWidget::OnMouseMove(MouseX, MouseY, newMouseX, newMouseY);
    
    MouseX = newMouseX;
    MouseY = newMouseY;
}