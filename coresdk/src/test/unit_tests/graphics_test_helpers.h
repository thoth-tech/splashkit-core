/**
 * Graphics Test Helpers
 *
 * Utility functions for unit testing graphics/drawing functionality.
 * Provides bitmap comparison and verification capabilities.
 */

#ifndef GRAPHICS_TEST_HELPERS_H
#define GRAPHICS_TEST_HELPERS_H

#include "images.h"
#include "color.h"
#include "point_drawing.h"

#include <cmath>

namespace splashkit_lib
{
    /**
     * Calculates the difference between two colors as a value from 0.0 to 1.0.
     * 0.0 means identical, 1.0 means completely different.
     *
     * @param c1 First color to compare
     * @param c2 Second color to compare
     * @return Difference value between 0.0 and 1.0
     */
    inline double color_difference(color c1, color c2)
    {
        // Color component functions return 0-255, normalize to 0-1
        double r_diff = std::abs(red_of(c1) - red_of(c2)) / 255.0;
        double g_diff = std::abs(green_of(c1) - green_of(c2)) / 255.0;
        double b_diff = std::abs(blue_of(c1) - blue_of(c2)) / 255.0;
        double a_diff = std::abs(alpha_of(c1) - alpha_of(c2)) / 255.0;

        // Return average difference across all channels (each channel is 0-1)
        return (r_diff + g_diff + b_diff + a_diff) / 4.0;
    }

    /**
     * Checks if two colors match within a tolerance.
     *
     * @param c1 First color to compare
     * @param c2 Second color to compare
     * @param tolerance Maximum allowed difference (0.0 = exact match, 1.0 = any match)
     * @return True if colors are within tolerance
     */
    inline bool colors_match(color c1, color c2, double tolerance = 0.0)
    {
        return color_difference(c1, c2) <= tolerance;
    }

    /**
     * Compares two bitmaps pixel-by-pixel.
     * Returns true if all pixels match within the specified tolerance.
     *
     * @param a First bitmap to compare
     * @param b Second bitmap to compare
     * @param tolerance Color difference tolerance (0.0 = exact, 0.1 = 10% tolerance)
     * @return True if bitmaps match within tolerance
     */
    inline bool bitmaps_match(bitmap a, bitmap b, double tolerance = 0.0)
    {
        if (!bitmap_valid(a) || !bitmap_valid(b))
            return false;

        if (bitmap_width(a) != bitmap_width(b) ||
            bitmap_height(a) != bitmap_height(b))
            return false;

        for (int y = 0; y < bitmap_height(a); y++)
        {
            for (int x = 0; x < bitmap_width(a); x++)
            {
                color c1 = get_pixel(a, x, y);
                color c2 = get_pixel(b, x, y);
                if (!colors_match(c1, c2, tolerance))
                    return false;
            }
        }
        return true;
    }

    /**
     * Counts the number of pixels that differ between two bitmaps.
     * Useful for debugging when bitmaps don't match.
     *
     * @param a First bitmap to compare
     * @param b Second bitmap to compare
     * @param tolerance Color difference tolerance
     * @return Number of differing pixels, or -1 if bitmaps have different dimensions
     */
    inline int bitmap_diff_count(bitmap a, bitmap b, double tolerance = 0.0)
    {
        if (!bitmap_valid(a) || !bitmap_valid(b))
            return -1;

        if (bitmap_width(a) != bitmap_width(b) ||
            bitmap_height(a) != bitmap_height(b))
            return -1;

        int diff_count = 0;
        for (int y = 0; y < bitmap_height(a); y++)
        {
            for (int x = 0; x < bitmap_width(a); x++)
            {
                color c1 = get_pixel(a, x, y);
                color c2 = get_pixel(b, x, y);
                if (!colors_match(c1, c2, tolerance))
                    diff_count++;
            }
        }
        return diff_count;
    }

    /**
     * Verifies that a filled circle was drawn correctly by sampling pixels
     * inside and outside the expected circle boundary.
     *
     * @param bmp Bitmap to verify
     * @param center Center point of the expected circle
     * @param radius Radius of the expected circle
     * @param fill_color Expected color inside the circle
     * @param background_color Expected color outside the circle
     * @param sample_count Number of points to sample (higher = more accurate)
     * @return True if the circle appears to be drawn correctly
     */
    inline bool verify_filled_circle(bitmap bmp, point_2d center, double radius,
                                     color fill_color, color background_color,
                                     int sample_count = 36)
    {
        if (!bitmap_valid(bmp))
            return false;

        const double PI = 3.14159265358979323846;
        double angle_step = 360.0 / sample_count;

        // Check points inside the circle (at 80% of radius)
        for (int i = 0; i < sample_count; i++)
        {
            double angle = i * angle_step * (PI / 180.0);
            double r = radius * 0.8;
            int x = static_cast<int>(center.x + r * std::cos(angle));
            int y = static_cast<int>(center.y + r * std::sin(angle));

            if (x >= 0 && x < bitmap_width(bmp) && y >= 0 && y < bitmap_height(bmp))
            {
                color pixel = get_pixel(bmp, x, y);
                if (!colors_match(pixel, fill_color, 0.1))
                    return false;
            }
        }

        // Check points outside the circle (at 120% of radius)
        for (int i = 0; i < sample_count; i++)
        {
            double angle = i * angle_step * (PI / 180.0);
            double r = radius * 1.2;
            int x = static_cast<int>(center.x + r * std::cos(angle));
            int y = static_cast<int>(center.y + r * std::sin(angle));

            if (x >= 0 && x < bitmap_width(bmp) && y >= 0 && y < bitmap_height(bmp))
            {
                color pixel = get_pixel(bmp, x, y);
                if (!colors_match(pixel, background_color, 0.1))
                    return false;
            }
        }

        return true;
    }

    /**
     * Verifies that a filled rectangle was drawn correctly.
     *
     * @param bmp Bitmap to verify
     * @param rect Rectangle that should be filled
     * @param fill_color Expected color inside the rectangle
     * @param tolerance Color matching tolerance
     * @return True if the rectangle appears to be filled correctly
     */
    inline bool verify_filled_rectangle(bitmap bmp, rectangle rect,
                                        color fill_color, double tolerance = 0.1)
    {
        if (!bitmap_valid(bmp))
            return false;

        // Sample points inside the rectangle
        int sample_points[][2] = {
            {static_cast<int>(rect.x + rect.width * 0.25), static_cast<int>(rect.y + rect.height * 0.25)},
            {static_cast<int>(rect.x + rect.width * 0.75), static_cast<int>(rect.y + rect.height * 0.25)},
            {static_cast<int>(rect.x + rect.width * 0.25), static_cast<int>(rect.y + rect.height * 0.75)},
            {static_cast<int>(rect.x + rect.width * 0.75), static_cast<int>(rect.y + rect.height * 0.75)},
            {static_cast<int>(rect.x + rect.width * 0.5), static_cast<int>(rect.y + rect.height * 0.5)}
        };

        for (auto& point : sample_points)
        {
            int x = point[0];
            int y = point[1];
            if (x >= 0 && x < bitmap_width(bmp) && y >= 0 && y < bitmap_height(bmp))
            {
                color pixel = get_pixel(bmp, x, y);
                if (!colors_match(pixel, fill_color, tolerance))
                    return false;
            }
        }

        return true;
    }

} // namespace splashkit_lib

#endif // GRAPHICS_TEST_HELPERS_H
