#include "animations.h"
#include "images.h"
#include "graphics.h"
#include "window_manager.h"
#include "utils.h"
#include "audio.h"
#include "input.h"
#include "text.h"
#include "z_UI.h"
#include "microUI.h"
#include "timers.h"

#include <vector>
#include <iostream>
using namespace std;
using namespace splashkit_lib;

// void openHelloWorldWindow(string val)
// {
//     window message_window = open_window(val, 400, 300);
    
//     timer message_timer = create_timer("message_timer");
//     start_timer(message_timer); 

//     while (timer_ticks(message_timer) < 5000) // 5000 milliseconds = 5 seconds
//     {
//         process_events();
//         clear_screen(COLOR_WHITE);
//         draw_text(val, COLOR_BLACK, 100, 150);
//         refresh_screen(60);
//     }

//     close_window(message_window);
// }

void run_ui_test()
{
    // Open a new window
    open_window("Button Example", 800, 600);

    // Define button properties
    float button_x = 350; // X position
    float button_y = 275; // Y position
    float button_width = 100;
    float button_height = 50;
    string button_label = "Click Me";

    // open_window("Drop Down Menu", 800, 600);

    vector<string> menu_items = {"Home", "Content", "Contect us", "About us"};
    
    bool menu_open = false;
    string selected_item = "";

    load_font("hara", "hara.ttf");
    // bool wasPressedButton1;
    // bool wasPressedButton2;

    
    Button btn1("button1", 10, 10, 100, 50);
        btn1.label(COLOR_WHITE, "Button 1", 14, "hara");
        btn1.fill_color(rgb_color(80,80,80));
        btn1.boarderline(rgb_color(0,0,0), 1);

    Button btn2("button2", 150, 10, 100, 50);
        btn2.label(COLOR_WHITE, "Button 2", 14, "hara");
        btn2.fill_color(rgb_color(80,80,80));
        btn2.boarderline(rgb_color(0,255,0), 5);

    Drop_Down_Menu menu1("menu1", 10, 100, 100, 50);
        menu1.label(COLOR_WHITE, "Menu 1", 14, "hara");
        menu1.fill_color(rgb_color(80,80,80));
        menu1.boarderline(rgb_color(150,150,150), 2);

    string Which_button = "";


    while (not quit_requested())
    {
        // Process events
        process_events();
        clear_screen(COLOR_WHITE);

        if (btn1.create_button(mouse_clicked(LEFT_BUTTON))){
            Which_button = "Button 1 Pressed";
        }

        if (btn2.create_button(mouse_clicked(LEFT_BUTTON))){
            Which_button = "Button 2 Pressed";
        }

        draw_text(Which_button, COLOR_BLACK, "hara", 14, 350, 30);

        int menu1_choice = menu1.create_menu(menu_items, mouse_clicked(LEFT_BUTTON));

        // if(menu1.create_menu(menu_items, mouse_clicked(LEFT_BUTTON)) != -1){
        //     cout << "Selected: " + to_string(menu1.create_menu(menu_items, mouse_clicked(LEFT_BUTTON))) << endl;
        // }

        if(menu1_choice != -1){
            // cout << "Selected: " + menu_items[menu1_choice] << endl;
            draw_text("Selected: " + menu_items[menu1_choice], COLOR_BLACK, "hara", 14, 20, 380);
        }

        refresh_screen(60);
    }

    // Close the window
    close_window("Button Example");
}