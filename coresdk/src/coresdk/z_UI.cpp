#include "animations.h"
#include "images.h"
#include "graphics.h"
#include "window_manager.h"
#include "utils.h"
#include "audio.h"
#include "input.h"
#include "text.h"
#include "z_UI.h"

namespace splashkit_lib
{
// Button
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    Button::Button(const string& id, float x, float y, float width, float height){
        this->button_id = id;
        this->x_pos = x;
        this->y_pos = y;
        this->button_width = width;
        this->button_height = height;

        this->button_color = rgba_color(0,0,0,0);
        this->font_color = COLOR_BLACK;
    }

    void Button::label(const color& font_color, const string& label, const float font_size, const string& fnt){
        // Draw the button label
        int text_width_val = text_width(label, fnt, font_size); 
        int text_height_val = text_height(label, fnt, font_size); 
        
        this->text_x = x_pos + (button_width - text_width_val) / 2;
        this->text_y = y_pos + (button_height - text_height_val) / 2;
        this->font_size = font_size;
        this->font_color = font_color;
        this->btn_label = label;
        this->font_style = fnt;
    }

    void Button::fill_color(color btn_color){
        this->button_color = btn_color;
    }

    void Button::boarderline(const color& boarder_color, float thickness){
        this->boarder_color = boarder_color;
        this->boarder_thickness = thickness;
    }

    bool Button::create_button(bool is_pressed) {
        rectangle button_rect = rectangle_from(x_pos, y_pos, button_width, button_height);
        point_2d mouse = mouse_position();

        bool is_hovering = point_in_rectangle(mouse, button_rect);
        // bool is_pressed = mouse_down(LEFT_BUTTON);

        color default_color = button_color;
        color click_color = rgb_color(red_of(default_color) + 80, green_of(default_color)+ 80, blue_of(default_color) + 80);
        color hover_color = rgb_color(red_of(default_color) + 40, green_of(default_color)+ 40, blue_of(default_color) + 40);

        // color button_color = is_hovering ? (is_pressed_now ? button_color : click_color): hover_color;
        color button_color = is_hovering ? (is_pressed ? default_color :click_color) : hover_color;

        //thickness 
        // fill_rectangle(boarder_color, x_pos - boarder_thickness, y_pos - boarder_thickness, button_width + boarder_thickness * 2, button_height + boarder_thickness * 2);
        if(alpha_of(default_color) != 0) fill_rectangle(button_color, button_rect);
        // draw_rectangle(rgb_color(0,255,0),button_rect,option_line_width(100));

        for(int i = 0; i < boarder_thickness; i++){
            draw_rectangle(boarder_color,x_pos - i, y_pos - i, button_width + (i*2), button_height + (i*2));
        }
        draw_text(btn_label, font_color, font_style, font_size, text_x, text_y);

        bool clicked = false;

        if (is_hovering && !buttonStates && is_pressed) {
            buttonStates = true;
        } else if (buttonStates && !is_pressed) {
            if (is_hovering) {
                clicked = true;
            }
            buttonStates = false;
        }
        return clicked;
    }

// Drop down menu
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    Drop_Down_Menu::Drop_Down_Menu(const string& id, float x, float y, float width, float height){
        this->menu_id = id;
        this->x_pos = x;
        this->y_pos = y;
        this->menu_width = width;
        this->menu_height = height;

        this->menu_open = false;
        this->selected_item = -1;
        this->menu_color = rgba_color(0,0,0,0);
        this->font_color = COLOR_BLACK;
    }

    float text_center(string text, string fnt, float font_size, float x, float y, float width, float height){
        int text_width_val = text_width(text, fnt, font_size); 
        int text_height_val = text_height(text, fnt, font_size); 
        
        float text_x = x + (width - text_width_val) / 2;
        float text_y = y + (height - text_height_val) / 2;

        return text_x, text_y;
    }

    void Drop_Down_Menu::label(const color& font_color, const string& label, const float font_size, const string& fnt){
        int text_width_val = text_width(label, fnt, font_size); 
        int text_height_val = text_height(label, fnt, font_size); 
        
        this->text_x = x_pos + (menu_width - text_width_val) / 2;
        this->text_y = y_pos + (menu_height - text_height_val) / 2;

        // this->text_x, this->text_y = text_center(label, fnt, font_size, x_pos, y_pos, menu_width, menu_height);

        this->font_size = font_size;
        this->font_color = font_color;
        this->menu_label = label;
        this->font_style = fnt;
    }

    void Drop_Down_Menu::fill_color(color menu_color){
        this->menu_color = menu_color;
    }

    void Drop_Down_Menu::boarderline(const color& boarder_color, float thickness){
        this->boarder_color = boarder_color;
        this->boarder_thickness = thickness;
    }

    int Drop_Down_Menu::create_menu(vector<string> items, bool is_pressed){
        this->menu_items = items;

        //Drop down trigger
        if (is_pressed)
        {
            point_2d mouse = mouse_position();
            // Check if the mouse is within the area
            if (mouse.x >= x_pos && mouse.x <= x_pos + menu_width && mouse.y >= y_pos && mouse.y <= y_pos + menu_height)
            {
                menu_open = !menu_open;
            }
            else if (mouse.x >= x_pos && mouse.x <= x_pos + menu_width)
            {
                for (int i = 0; i < menu_items.size(); i++)
                {
                    if (mouse.y >= y_pos + menu_height*(i+1) && mouse.y <= y_pos + menu_height *(i+2))
                    {
                        selected_item = i;
                        menu_open = false;
                        break;
                    }
                }
            }
        }


        // Draw menu button
        if(alpha_of(menu_color) != 0) fill_rectangle(rgb_color(80,80,80), x_pos, y_pos, menu_width, menu_height);

        for(int i = 0; i < boarder_thickness; i++){
            draw_rectangle(boarder_color,x_pos - i, y_pos - i, menu_width + (i*2), menu_height + (i*2));
        }
        draw_text(menu_id, font_color, font_style, font_size, text_x, text_y);

        // Draw drop-down items
        if (menu_open) {
            for (int i = 0; i < menu_items.size(); i++) {
                float item_y_pos = y_pos + menu_height * (i + 1);

                //centering text
                int item_text_width = text_width(menu_items[i], font_style, font_size);
                int item_text_height = text_height(menu_items[i], font_style, font_size);
                float item_text_x = x_pos + (menu_width - item_text_width) / 2;
                float item_text_y = item_y_pos + (menu_height - item_text_height) / 2;

                fill_rectangle(menu_color, x_pos, item_y_pos, menu_width, menu_height);
                for(int j = 0; j < boarder_thickness; j++){
                    draw_rectangle(boarder_color, x_pos - j, item_y_pos - j, menu_width + (j*2), menu_height + (j*2));
                }
                draw_text(menu_items[i], font_color, font_style, font_size, item_text_x, item_text_y);
            }
        }
        return selected_item;
    }

        



    // std::map<std::string, bool> buttonStates;
    // bool create_button(const std::string& button_id, float x_pos, float y_pos, float button_width, float button_height, const string& button_label, const string& fnt)
    // {
    //     if (buttonStates.find(button_id) == buttonStates.end()) {
    //         buttonStates[button_id] = false;
    //     }

    //     rectangle button_rect = rectangle_from(x_pos, y_pos, button_width, button_height);
    //     point_2d mouse = mouse_position();

    //     bool is_hovering = point_in_rectangle(mouse, button_rect);
    //     bool is_pressed_now = mouse_down(LEFT_BUTTON);

    //     color button_color = is_hovering ? (is_pressed_now ? rgb_color(80, 80, 80) : rgb_color(180, 180, 180)) : rgb_color(120, 120, 120);
    //     fill_rectangle(button_color, button_rect);

    //     // Draw the button label
    //     int text_width_val = text_width(button_label, fnt, 14); 
    //     int text_height_val = text_height(button_label, fnt, 14); 
    //     float text_x = x_pos + (button_width - text_width_val) / 2;
    //     float text_y = y_pos + (button_height - text_height_val) / 2;

    //     draw_text(button_label, COLOR_WHITE, fnt, 14, text_x, text_y);

    //     // Check for button click
    //     bool clicked = false;
    //     bool& was_pressed = buttonStates[button_id]; // Reference to the specific button's state

    //     if (is_hovering && !was_pressed && is_pressed_now) {
    //         was_pressed = true; // The mouse is currently pressing the button
    //     } else if (was_pressed && !is_pressed_now) {
    //         if (is_hovering) {
    //             clicked = true; // The button was clicked (pressed and then released)
    //         }
    //         was_pressed = false; // Reset the press state
    //     }

    //     return clicked;
    // }
}
