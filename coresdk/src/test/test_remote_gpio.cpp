#include <iostream>
#include <csignal>



//#include "graphics.h"
#include "terminal.h"
#include "window_manager.h"
#include "utils.h"
#include "input.h"
#include "interface.h"

#include "raspi_gpio.h"
#include "types.h"
using namespace splashkit_lib;

connection remote_pi;
// Currently remote functions ONLY supports 
// referencing pins by their Broadcom number,
// NOT their physical number.
const int red_LED_pin = 17;
const int yel_LED_pin = 27;
const int grn_LED_pin = 22;
const std::string ipaddr = "192.168.0.19";

void signalHandler(int signum)
{
    // Turn output off
    //remote_raspi_write(remote_pi, red_LED_pin, 0);
    //remote_raspi_write(remote_pi, yel_LED_pin, 0);
    //remote_raspi_write(remote_pi, grn_LED_pin, 0);
    
    // Set to input
    //remote_raspi_set_mode(remote_pi, red_LED_pin, 0);
    //remote_raspi_set_mode(remote_pi, yel_LED_pin, 0);
    //remote_raspi_set_mode(remote_pi, grn_LED_pin, 0);
    
    // Close connection
    remote_raspi_cleanup(remote_pi);
    exit(signum);
}

void create_gpio_control_window()
{
    // Create connection
    remote_pi = remote_raspi_init("remote_raspi", ipaddr, 8888);
    

    // Set to output
    remote_raspi_set_mode(remote_pi, red_LED_pin, 1);
    remote_raspi_set_mode(remote_pi, yel_LED_pin, 1);
    remote_raspi_set_mode(remote_pi, grn_LED_pin, 1);
    
    // Turn red light on
    remote_raspi_write(remote_pi, red_LED_pin, 1);
    
    open_window("Remote GPIO Test UI", 600, 600);

    // I had trouble obtaining the system font in WSL2; not sure why, I suspect
    // it is not looking in the right location -- hardcoding the path is a solution (albeit hacky)
    font text_font = load_font("hack",          "/home/liquidscroll/.local/share/fonts/hack-nerd-font-mono/HackNerdFontMono-Regular.ttf");
    set_interface_font(text_font);
    set_interface_font_size(12);


    while(!quit_requested())
    {
        process_events();
        clear_screen(COLOR_WHITE);
        
        paragraph("These buttons will control LEDS", {40, 10, 600, 20});

        if(button("Green", rectangle_from(40, 40, 350, 125)))
        {
            // Turn Green LED ON, others OFF
            remote_raspi_write(remote_pi, red_LED_pin, 0);
            remote_raspi_write(remote_pi, yel_LED_pin, 0);
            remote_raspi_write(remote_pi, grn_LED_pin, 1);
        }
        if(button("Yellow", rectangle_from(40, 180, 350, 125)))
        {
            // Turn Orange LED ON, others OFF
            remote_raspi_write(remote_pi, red_LED_pin, 0);
            remote_raspi_write(remote_pi, yel_LED_pin, 1);
            remote_raspi_write(remote_pi, grn_LED_pin, 0);
        }
        if(button("Red", rectangle_from(40, 320, 350, 125)))
        {
            // Turn Red LED ON, others OFF
            remote_raspi_write(remote_pi, red_LED_pin, 1);
            remote_raspi_write(remote_pi, yel_LED_pin, 0);
            remote_raspi_write(remote_pi, grn_LED_pin, 0);
        }
        draw_interface();
        refresh_screen(60);
        if(quit_requested()) break;
    }
    close_window(window_named("Remote GPIO Test UI"));
    
    
    // Turn output off
    //remote_raspi_write(remote_pi, red_LED_pin, 0);
    //remote_raspi_write(remote_pi, yel_LED_pin, 0);
    //remote_raspi_write(remote_pi, grn_LED_pin, 0);
    
    // Set to input
    //remote_raspi_set_mode(remote_pi, red_LED_pin, 0);
    //remote_raspi_set_mode(remote_pi, yel_LED_pin, 0);
    //remote_raspi_set_mode(remote_pi, grn_LED_pin, 0);
    
    // Close connection
    remote_raspi_cleanup(remote_pi);
}

void run_remote_gpio_tests()
{
    std::cout << "Start run_remote_gpio_tests" << std::endl;
    create_gpio_control_window();
    std::cout << "End run_remote_gpio_tests" << std::endl;
}

