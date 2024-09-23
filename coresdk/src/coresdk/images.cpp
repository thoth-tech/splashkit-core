//
//  images.cpp
//  splashkit
//
//  Created by Andrew Cain on 24/07/2016.
//  Copyright © 2016 Andrew Cain. All rights reserved.
//

#include "images.h"

#include "graphics_driver.h"
#include "backend_types.h"
#include "utility_functions.h"
#include "resources.h"

#include <map>
#include <cstdlib>
#include <cmath>

using std::map;
using std::to_string;

namespace splashkit_lib
{
    static map<string, bitmap> _bitmaps;

    void setup_collision_mask(bitmap bmp)
    {
        if ( INVALID_PTR(bmp, BITMAP_PTR) )
        {
            LOG(WARNING) << "Attempt to setup collision map with invalid bitmp";
            return;
        }
        
        int *pixels;
        int sz;
        int r, c;

        sz = bmp->image.surface.width * bmp->image.surface.height;
        pixels = (int *) malloc(sizeof(int) * sz);

        sk_to_pixels(&bmp->image.surface, pixels, sz);

        if ( bmp->pixel_mask == nullptr )
            bmp->pixel_mask = (bool *) malloc( sizeof(bool) * sz );

        // WriteLn(bmp^.name);
        for (r = 0; r < bmp->image.surface.height; r++)
        {
            for(c = 0; c < bmp->image.surface.width; c++)
            {
                bmp->pixel_mask[c + r * bmp->image.surface.width] = ((pixels[c + r * bmp->image.surface.width] & 0x000000FF) > 0x0000007F );
                // if b^.nonTransparentPixels[c, r] then Write(1) else Write(0);
            }
            // WriteLn();
        }
        // WriteLn();

        free(pixels);
    }
    
    bool bitmap_valid(bitmap bmp)
    {
        return VALID_PTR(bmp, BITMAP_PTR);
    }

    bool has_bitmap(string name)
    {
        return _bitmaps.count(name) > 0;
    }

    bitmap bitmap_named(string name)
    {
        if (has_bitmap(name))
            return _bitmaps[name];
        else
        {
            string filename = path_to_resource(name, IMAGE_RESOURCE);

            if ( file_exists(filename) or file_exists(name))
                return load_bitmap(name, name);
            return nullptr;
        }
    }


    bitmap load_bitmap(string name, string filename)
    {
        if (has_bitmap(name)) return bitmap_named(name);

        sk_drawing_surface surface;
        bitmap result = nullptr;

        string file_path = filename;

        if ( ! file_exists(file_path) )
        {
            file_path = path_to_resource(filename, IMAGE_RESOURCE);

            if ( ! file_exists(file_path) )
            {
                LOG(WARNING) << cat({ "Unable to locate file for ", name, " (", file_path, ")"});
                return nullptr;
            }
        }

        surface = sk_load_bitmap(file_path.c_str());
        if ( not surface._data )
        {
            LOG(WARNING) <<  cat({ "Error loading image for ", name, " (", file_path, ")"}) ;
            return nullptr;
        }

        result = new _bitmap_data;
        result->image.surface = surface;

        result->id         = BITMAP_PTR;
        result->cell_w     = surface.width;
        result->cell_h     = surface.height;
        result->cell_cols  = 1;
        result->cell_rows  = 1;
        result->cell_count = 1;
        result->pixel_mask = nullptr;

        result->name       = name;
        result->filename   = file_path;

        setup_collision_mask(result);

        _bitmaps[name] = result;

        return result;
    }

    bitmap load_bitmap_base64(string name, const char * image)
    {
        if (has_bitmap(name)) return bitmap_named(name);

        sk_drawing_surface surface;
        bitmap result = nullptr;

        surface = sk_load_bitmap_base64(image);
        if ( not surface._data )
        {
            LOG(WARNING) <<  cat({ "Error loading image for ", name}) ;
            return nullptr;
        }

        result = new _bitmap_data;
        result->image.surface = surface;

        result->id         = BITMAP_PTR;
        result->cell_w     = surface.width;
        result->cell_h     = surface.height;
        result->cell_cols  = 1;
        result->cell_rows  = 1;
        result->cell_count = 1;
        result->pixel_mask = nullptr;

        result->name       = name;

        setup_collision_mask(result);

        _bitmaps[name] = result;

        return result;
    }

    bitmap create_bitmap(string name, int width, int height)
    {
        bitmap result = new(_bitmap_data);

        result->id = BITMAP_PTR;
        result->image.surface = sk_create_bitmap(width, height);

        result->cell_w     = width;
        result->cell_h     = height;
        result->cell_cols  = 1;
        result->cell_rows  = 1;
        result->cell_count = 1;
        result->pixel_mask = nullptr;

        result->filename   = "";

        int idx = 0;
        string key = name;
        while (has_bitmap(key))
        {
            key = name + to_string(idx);
            idx++;
        }

        result->name       = key;

        _bitmaps[key] = result;

        return result;
    }

    void free_bitmap(bitmap bmp)
    {
        if ( VALID_PTR(bmp, BITMAP_PTR) )
        {
            notify_of_free(bmp);

            _bitmaps.erase(bmp->name);
            sk_close_drawing_surface(&bmp->image.surface);
            bmp->id = NONE_PTR;  // ensure future use of this pointer will fail...
            if ( bmp->pixel_mask != nullptr )
                free(bmp->pixel_mask);
            delete(bmp);
        }
        else
        {
            LOG(WARNING) << "Delete bitmap called without valid bitmap";
        }
    }

    void free_all_bitmaps()
    {
        FREE_ALL_FROM_MAP(_bitmaps, BITMAP_PTR, free_bitmap);
    }

    string bitmap_filename(bitmap bmp)
    {
        if ( INVALID_PTR(bmp, BITMAP_PTR)) return "";

        return bmp->filename;
    }

    string bitmap_name(bitmap bmp)
    {
        if ( INVALID_PTR(bmp, BITMAP_PTR)) return "";

        return bmp->name;
    }

    void clear_bitmap(bitmap bmp, color clr)
    {
        if ( INVALID_PTR(bmp, BITMAP_PTR))
        {
            LOG(WARNING) << "Attempting to clear invalid bitmap";
            return;
        }

        sk_clear_drawing_surface(&bmp->image.surface, clr);
    }

    void clear_bitmap(string name, color clr)
    {
        clear_bitmap(bitmap_named(name), clr);
    }


    void draw_bitmap(bitmap bmp, double x, double y)
    {
        draw_bitmap(bmp, x, y, option_defaults());
    }

    void draw_bitmap(bitmap bmp, double x, double y, drawing_options opts)
    {
        if ( INVALID_PTR(bmp, BITMAP_PTR))
        {
            LOG(WARNING) << "Error trying to draw bitmap: passed in bmp is an invalid bitmap pointer.";
            return;
        }

        double src_data[4];
        double dst_data[7];
        sk_renderer_flip flip;
        sk_drawing_surface * dest;
        
        

        if ( VALID_PTR(opts.anim, ANIMATION_PTR) || opts.draw_cell >= 0 )
        {
            int cell;
            if ( opts.draw_cell >= 0 )
                cell = opts.draw_cell;
            else
                cell = animation_current_cell(opts.anim);
            
            rectangle part = bitmap_rectangle_of_cell(bmp, cell);
            src_data[0] = part.x;
            src_data[1] = part.y;
            src_data[2] = part.width;
            src_data[3] = part.height;
        }
        else if (opts.is_part)
        {
            src_data[0] = opts.part.x;
            src_data[1] = opts.part.y;
            src_data[2] = opts.part.width;
            src_data[3] = opts.part.height;
        }
        else
        {
            src_data[0] = 0;
            src_data[1] = 0;
            src_data[2] = bmp->image.surface.width;
            src_data[3] = bmp->image.surface.height;
        }

        //
        if ((opts.flip_x) and (opts.flip_y))
            flip = sk_FLIP_BOTH;
        else if (opts.flip_x)
            flip = sk_FLIP_VERTICAL;
        else if (opts.flip_y)
            flip = sk_FLIP_HORIZONTAL;
        else
            flip = sk_FLIP_NONE;

        // make up dst data
        dst_data[0] = x; // X
        dst_data[1] = y; // Y
        dst_data[2] = opts.angle; // Angle
        dst_data[3] = opts.anchor_offset_x; // Centre X
        dst_data[4] = opts.anchor_offset_y; // Centre Y
        dst_data[5] = opts.scale_x; // Scale X
        dst_data[6] = opts.scale_y; // Scale Y

        xy_from_opts(opts, dst_data[0], dst_data[1]); // Camera?

        dest = to_surface_ptr(opts.dest);
        sk_draw_bitmap(&bmp->image.surface, dest, src_data, 4, dst_data, 7, flip);
    }

    void draw_bitmap_on_window(window destination, bitmap bmp, double x, double y)
    {
        draw_bitmap(bmp, x, y, option_draw_to(destination));
    }

    void draw_bitmap_on_window(window destination, bitmap bmp, double x, double y, drawing_options opts)
    {
        draw_bitmap(bmp, x, y, option_draw_to(destination, opts));
    }

    void draw_bitmap_on_bitmap(bitmap destination, bitmap bmp, double x, double y)
    {
        draw_bitmap(bmp, x, y, option_draw_to(destination));
    }

    void draw_bitmap_on_bitmap(bitmap destination, bitmap bmp, double x, double y, drawing_options opts)
    {
        draw_bitmap(bmp, x, y, option_draw_to(destination, opts));
    }

    void draw_bitmap(string name, double x, double y)
    {
        draw_bitmap(bitmap_named(name), x, y, option_defaults());
    }

    void draw_bitmap(string name, double x, double y, drawing_options opts)
    {
        draw_bitmap(bitmap_named(name), x, y, opts);
    }

    void draw_splash_screen(double x, double y)
    {   
        //Debug image for splash screen
                const char image[] = {"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"};
        
        bitmap bmp = load_bitmap_base64("SplashScreen", image);
        draw_bitmap("SplashScreen", x, y);
    }

    rectangle bitmap_cell_rectangle(bitmap src, const point_2d &pt)
    {
        if ( INVALID_PTR(src, BITMAP_PTR))
        {
            LOG(WARNING) << "Attempting to get bitmap cell rectangle from invalid image";
            return rectangle_from(0, 0, 0, 0);
        }

        return rectangle_from(pt, src->cell_w, src->cell_h);
    }

    rectangle bitmap_cell_rectangle(bitmap src)
    {
        return bitmap_cell_rectangle(src, point_at(0, 0));
    }

    vector_2d bitmap_cell_offset(bitmap src, int cell)
    {
        if ( (not VALID_PTR(src, BITMAP_PTR)) or (cell >= src->cell_count) or (cell < 0) )
            return vector_to(0,0);
        return vector_to(
                         (cell % src->cell_cols) * src->cell_w,
                         (cell - (cell % src->cell_cols)) / src->cell_cols * src->cell_h);
    }

    rectangle bitmap_rectangle_of_cell(bitmap src, int cell)
    {
        rectangle result;

        if ( (not VALID_PTR(src, BITMAP_PTR)) or (cell >= src->cell_count))
            result = rectangle_from(0,0,0,0);
        else if (cell < 0)
            result = rectangle_from(0,0,src->image.surface.width, src->image.surface.width);
        else
        {
            result.x = (cell % src->cell_cols) * src->cell_w;
            result.y = (cell - (cell % src->cell_cols)) / src->cell_cols * src->cell_h;
            result.width = src->cell_w;
            result.height = src->cell_h;
        }

        return result;
    }

    rectangle bitmap_bounding_rectangle(bitmap bmp)
    {
        return bitmap_bounding_rectangle(bmp, 0, 0);
    }

    rectangle bitmap_bounding_rectangle(bitmap bmp, double x, double y)
    {
        if ( INVALID_PTR(bmp, BITMAP_PTR) )
            return rectangle_from(0, 0, 0, 0);
        else
            return rectangle_from(x, y, bitmap_width(bmp), bitmap_height(bmp));
    }


    circle bitmap_cell_circle(bitmap bmp, const point_2d pt, double scale)
    {
        if ( INVALID_PTR(bmp, BITMAP_PTR) )
        {
            LOG(WARNING) << "Attempting to get cell circle from invalid bitmap";
            return circle_at(0, 0, 0);
        }

        return circle_at(pt, MAX(bmp->cell_w, bmp->cell_h) / 2.0f * scale);
    }

    circle bitmap_cell_circle(bitmap bmp, const point_2d pt)
    {
        return bitmap_cell_circle(bmp, pt, 1);
    }

    circle bitmap_cell_circle(bitmap bmp, double x, double y)
    {
        return bitmap_cell_circle(bmp, point_at(x, y), 1);
    }

    circle bitmap_bounding_circle(bitmap bmp, const point_2d &pt)
    {
        if ( INVALID_PTR(bmp, BITMAP_PTR))
        {
            return circle_at(0,0,0);
        }

        return circle_at(pt, MAX(bmp->image.surface.width, bmp->image.surface.height));
    }

    int bitmap_cell_columns(bitmap bmp)
    {
        if ( INVALID_PTR(bmp, BITMAP_PTR))
        {
            return 0;
        }

        return bmp->cell_cols;
    }

    int bitmap_cell_rows(bitmap bmp)
    {
        if ( INVALID_PTR(bmp, BITMAP_PTR))
        {
            return 0;
        }

        return bmp->cell_rows;
    }

    void bitmap_set_cell_details(bitmap bmp, int width, int height, int columns, int rows, int count)
    {
        if ( not VALID_PTR(bmp, BITMAP_PTR))
        {
            LOG(WARNING) << "Trying to set cell details of invalid bitmap.";
            return;
        }

        bmp->cell_w     = width;
        bmp->cell_h     = height;
        bmp->cell_cols  = columns;
        bmp->cell_rows  = rows;
        bmp->cell_count = count;
    }

    int bitmap_width(bitmap bmp)
    {
        if ( INVALID_PTR(bmp, BITMAP_PTR))
        {
            LOG(WARNING) << "Attempting to get width of invalid bitmap";
            return 0;
        }

        return bmp->image.surface.width;
    }

    int bitmap_width(string name)
    {
        return bitmap_width(bitmap_named(name));
    }

    int bitmap_height(bitmap bmp)
    {
        if ( INVALID_PTR(bmp, BITMAP_PTR))
        {
            LOG(WARNING) << "Attempting to get height of invalid bitmap";
            return 0;
        }

        return bmp->image.surface.height;
    }

    int bitmap_height(string name)
    {
        return bitmap_height(bitmap_named(name));
    }

    point_2d bitmap_center(bitmap bmp)
    {
        return point_at(bitmap_width(bmp) / 2.0f, bitmap_height(bmp) / 2.0f);
    }

    int bitmap_cell_width(bitmap bmp)
    {
        if ( INVALID_PTR(bmp, BITMAP_PTR))
        {
            LOG(WARNING) << "Attempting to read details of invalid bitmap";
            return 0;
        }

        return bmp->cell_w;
    }

    int bitmap_cell_height(bitmap bmp)
    {
        if ( INVALID_PTR(bmp, BITMAP_PTR))
        {
            LOG(WARNING) << "Attempting to read details of invalid bitmap";
            return 0;
        }

        return bmp->cell_h;
    }

    point_2d bitmap_cell_center(bitmap bmp)
    {
        return point_at(bitmap_cell_width(bmp) / 2.0f, bitmap_cell_height(bmp) / 2.0f);
    }


    int bitmap_cell_count(bitmap bmp)
    {
        if ( INVALID_PTR(bmp, BITMAP_PTR))
        {
            return 0;
        }

        return bmp->cell_count;
    }

    bool pixel_drawn_at_point(bitmap bmp, double x, double y)
    {
        int px = ceil(x);
        int py = ceil(y);

        if ( INVALID_PTR(bmp, BITMAP_PTR) or px < 0 or px >= bitmap_width(bmp) or py < 0 or py >= bitmap_height(bmp) or bmp->pixel_mask == nullptr ) return false;

        return bmp->pixel_mask[px + py * bmp->image.surface.width];
    }

    bool pixel_drawn_at_point(bitmap bmp, int cell, double x, double y)
    {
        vector_2d offset = bitmap_cell_offset(bmp, cell);
        return pixel_drawn_at_point(bmp, x + offset.x, y + offset.y);
    }

    bool pixel_drawn_at_point(bitmap bmp, const point_2d &pt)
    {
        return pixel_drawn_at_point(bmp, pt.x, pt.y);
    }

    bool pixel_drawn_at_point(bitmap bmp, int cell, const point_2d &pt)
    {
        return pixel_drawn_at_point(bmp, cell, pt.x, pt.y);
    }
}
