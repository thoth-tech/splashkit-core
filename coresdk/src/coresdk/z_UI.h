/**
 * @header  input
 * @author  Andrew Cain
 * @brief   Input handles user interaction and events such as keypresses.
 *
 * @attribute group  input
 * @attribute static input
 */

#ifndef z_UI_h
#define z_UI_h

#include "animations.h"
#include "images.h"
#include "graphics.h"
#include "window_manager.h"
#include "utils.h"
#include "audio.h"
#include "input.h"
#include "text.h"


namespace splashkit_lib
{

    // bool create_button(const std::string& button_id, float x_pos, float y_pos, float button_width, float button_height, const string& button_label, const string& fnt);
    struct Button {
        bool buttonStates; // Stores the state of each button
        std::string button_id;
        float x_pos, y_pos, button_width, button_height;
        color button_color;
        string btn_label;

        float text_x, text_y;
        float font_size;
        color font_color;
        string font_style;

        float boarder_thickness;
        color boarder_color;

        Button(const std::string& id, float x, float y, float width, float height);
        void fill_color(color btn_color);
        void label(const color& font_color, const string& label, const float font_size, const string& fnt);
        void boarderline(const color& boarder_color, float thickness);
        bool create_button(bool is_pressed);
    };

    struct Drop_Down_Menu {
        bool menu_open = false;
        int selected_item;
        vector<string> menu_items;
        string menu_id;

        float x_pos, y_pos, menu_width, menu_height;
        float item_width, item_height;
        color menu_color;
        string menu_label;

        float text_x, text_y;
        float font_size;
        color font_color;
        string font_style;

        float boarder_thickness;
        color boarder_color;

        // Drop_Down_Menu(const std::string& id, float x, float y, float width, float height);
        // void menu_set_color(color menu_color);
        // void menu_label(const string& menu_label,const float font_size, const string& fnt);
        // void menu_boarderline(const color& boarder_color, float thickness);
        Drop_Down_Menu(const string& id, float x, float y, float width, float height);
        void label(const color& font_color, const string& label,const float font_size, const string& fnt);
        void fill_color(color menu_color);
        void boarderline(const color& boarder_color, float thickness);
        int create_menu(vector<string> menu_items, bool is_pressed);
    };
}
#endif /* input_hpp */
