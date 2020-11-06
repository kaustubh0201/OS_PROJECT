#ifndef __OSD__GUI__WIDGET_H
#define __OSD__GUI__WIDGET_H

#include <common/types.h>
#include <common/graphicscontext.h>
#include <drivers/keyboard.h>

namespace osd 
{
    namespace gui
    {   
        // we are combining the keyboard and the widget
        class Widget : public osd::drivers::KeyboardEventHandler
        {
        protected:
            Widget* parent;
            osd::common::int32_t x; // x - axis
            osd::common::int32_t y; // y - axis
            osd::common::int32_t w; // width
            osd::common::int32_t h; // height

            osd::common::uint8_t r; // percentage of red
            osd::common::uint8_t g; // percentage of green
            osd::common::uint8_t b; // percentage of blue
            bool Focussable; // on which widget to focus on

        // you can make different type of things using widget 
        // most of the gui is made with the help of this only
        public:
            // taking the initial values of the variables declared above
            Widget(Widget* parent, osd::common::int32_t x, osd::common::int32_t y, osd::common::int32_t w, osd::common::int32_t h, osd::common::uint8_t r, osd::common::uint8_t g, osd::common::uint8_t b);
            ~Widget();

            virtual void GetFocus(Widget* widget); // for more than 1 widget
            virtual void ModelToScreen(osd::common::int32_t &x, osd::common::int32_t &y); // to show on screen
            virtual bool ContainsCoordinate(common::int32_t x, common::int32_t y);

            virtual void Draw(osd::common::GraphicsContext* gc);
            virtual void OnMouseDown(osd::common::int32_t x, osd::common::int32_t y, osd::common::uint8_t button);
            virtual void OnMouseUp(osd::common::int32_t x, osd::common::int32_t y, osd::common::uint8_t button);
            virtual void OnMouseMove(osd::common::int32_t oldx, osd::common::int32_t oldy, osd::common::int32_t newx, osd::common::int32_t newy);
        
        };
        
        // we are combining the widget and the mouse
        class CompositeWidget : public Widget
        {
        private:
            Widget* children[100]; // also same as above but this is for if you want a widget inside a widget 
            int numChildren;
            Widget* focussedChild;

        public:
            CompositeWidget(Widget* parent, osd::common::int32_t x, osd::common::int32_t y, osd::common::int32_t w, osd::common::int32_t h, osd::common::uint8_t r, osd::common::uint8_t g, osd::common::uint8_t b);
        
            ~CompositeWidget();

            virtual void GetFocus(Widget* widget);
            virtual bool AddChild(Widget* child);

            virtual void Draw(osd::common::GraphicsContext* gc);
            virtual void OnMouseDown(osd::common::int32_t x, osd::common::int32_t y, osd::common::uint8_t button);
            virtual void OnMouseUp(osd::common::int32_t x, osd::common::int32_t y, osd::common::uint8_t button);
            virtual void OnMouseMove(osd::common::int32_t oldx, osd::common::int32_t oldy, osd::common::int32_t newx, osd::common::int32_t newy);

            virtual void OnKeyDown(char str);
            virtual void OnKeyUp(char str);
        };
    }
}

#endif