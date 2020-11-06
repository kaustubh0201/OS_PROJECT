#ifndef __OSD__GUI__WINDOW_H
#define __OSD__GUI__WINDOW_H

#include <gui/widget.h>
#include <drivers/mouse.h>

namespace osd
{
    namespace gui
    {
        // this class is just a child of the composite widget
        class Window : public osd::gui::CompositeWidget
        { 
        protected:
            bool Dragging; // tells us if the window is currently dragging or not
            
        public:
            Window(osd::gui::Widget* parent,
                   osd::common::int32_t x, osd::common::int32_t y, osd::common::int32_t w, osd::common::int32_t h,
                   osd::common::uint8_t r, osd::common::uint8_t g, osd::common::uint8_t b);
            ~Window();

            void OnMouseDown(osd::common::int32_t x, osd::common::int32_t y, osd::common::uint8_t button);
            void OnMouseUp(osd::common::int32_t x, osd::common::int32_t y, osd::common::uint8_t button);
            void OnMouseMove(osd::common::int32_t oldx, osd::common::int32_t oldy, osd::common::int32_t newx, osd::common::int32_t newy);
            
        };
    }
}

#endif