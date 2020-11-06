 
#ifndef __OSD__GUI__DESKTOP_H
#define __OSD__GUI__DESKTOP_H

#include <gui/widget.h>
#include <drivers/mouse.h>

namespace osd
{
    namespace gui
    {
        
        class Desktop : public osd::gui::CompositeWidget, public osd::drivers::MouseEventHandler
        {
        protected:
            osd::common::uint32_t MouseX; // this is the mouse position on x-axis
            osd::common::uint32_t MouseY; // this is the mouse position on y-axis
            
        public:
            Desktop(osd::common::int32_t w, osd::common::int32_t h,
                osd::common::uint8_t r, osd::common::uint8_t g, osd::common::uint8_t b);
            ~Desktop();
            
            void Draw(osd::common::GraphicsContext* gc); // it reference to the Video graphics array
            
            void OnMouseDown(osd::common::uint8_t button); // this is for mouse button clicked
            void OnMouseUp(osd::common::uint8_t button); // for mouse button released
            void OnMouseMove(int x, int y); // we have to change x - axis and y - axis when mouse is moved
        };
        
    }
}


#endif