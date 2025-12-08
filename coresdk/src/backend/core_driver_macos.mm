//
//  core_driver_macos.mm
//  splashkit
//
//  macOS-specific display name retrieval using NSScreen API.
//  This file is only compiled on macOS.
//

#ifdef __APPLE__

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>
#include <cstring>

extern "C" {

/**
 * Get the localized name of a display on macOS.
 * 
 * SDL_GetDisplayName returns the display index as a string on newer macOS versions,
 * so we use NSScreen.localizedName to get the actual display name.
 *
 * @param display_index The index of the display (0-based).
 * @return The display name as a C string, or empty string if not found.
 *         The returned string is stored in a static buffer and is valid until
 *         the next call to this function.
 */
const char* sk_macos_get_display_name(int display_index)
{
    static char name_buffer[256];
    name_buffer[0] = '\0';
    
    @autoreleasepool {
        NSArray<NSScreen *>* screens = [NSScreen screens];
        
        if (display_index < 0 || (NSUInteger)display_index >= screens.count) {
            return name_buffer;
        }
        
        NSScreen *screen = screens[display_index];
        
        // localizedName is available on macOS 10.15+
        if (@available(macOS 10.15, *)) {
            NSString *name = screen.localizedName;
            
            if (name && name.length > 0) {
                strncpy(name_buffer, name.UTF8String, sizeof(name_buffer) - 1);
                name_buffer[sizeof(name_buffer) - 1] = '\0';
            }
        }
    }
    
    return name_buffer;
}

} // extern "C"

#endif // __APPLE__
