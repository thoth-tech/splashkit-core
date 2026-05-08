/**
 * Text and font Unit Tests
 */

#include <vector>

#include "catch.hpp"

#include "text.h"
#include "types.h"
#include "backend_types.h"
#include "utility_functions.h"

using namespace splashkit_lib;

TEST_CASE("can load system fonts", "[text]")
{
    font test;
    #ifndef __linux__
    test = load_font("Arial", "Arial");
    #else
    test = load_font("Arial", "DejaVuSans.ttf");
    #endif

    SECTION("can load arial")
    {
        REQUIRE(test != nullptr);
        REQUIRE(VALID_PTR(test, FONT_PTR));
        REQUIRE(has_font(test));
        REQUIRE(font_has_size(test, 64));
    }

    SECTION("uses font_named to retrieve loaded font")
    {
        font named = font_named("Arial");
        REQUIRE(named != nullptr);
        REQUIRE(VALID_PTR(named, FONT_PTR));
        REQUIRE(has_font(named));
    }

    SECTION("can set font style and read it back")
    {
        set_font_style(test, ITALIC_FONT);
        REQUIRE(get_font_style(test) == ITALIC_FONT);

        set_font_style("Arial", UNDERLINE_FONT);
        REQUIRE(get_font_style("Arial") == UNDERLINE_FONT);
    }

    SECTION("can load additional font sizes")
    {
        REQUIRE_FALSE(font_has_size(test, 32));
        font_load_size(test, 32);
        REQUIRE(font_has_size(test, 32));

        REQUIRE_FALSE(font_has_size("Arial", 18));
        font_load_size("Arial", 18);
        REQUIRE(font_has_size("Arial", 18));
    }

    free_all_fonts();
}

TEST_CASE("can manipulate font styles", "[text]")
{
    font test;
    #ifndef __linux__
    test = load_font("Arial", "Arial");
    #else
    test = load_font("Arial", "DejaVuSans.ttf");
    #endif

    SECTION("font starts with normal style")
    {
        REQUIRE(get_font_style(test) == NORMAL_FONT);
    }

    SECTION("can set font to bold style")
    {
        set_font_style(test, BOLD_FONT);
        REQUIRE(get_font_style(test) == BOLD_FONT);
    }

    SECTION("can set font to italic style")
    {
        set_font_style(test, ITALIC_FONT);
        REQUIRE(get_font_style(test) == ITALIC_FONT);
    }

    SECTION("can set font to underline style")
    {
        set_font_style(test, UNDERLINE_FONT);
        REQUIRE(get_font_style(test) == UNDERLINE_FONT);
    }

    SECTION("can cycle through multiple font styles")
    {
        set_font_style(test, BOLD_FONT);
        REQUIRE(get_font_style(test) == BOLD_FONT);

        set_font_style(test, ITALIC_FONT);
        REQUIRE(get_font_style(test) == ITALIC_FONT);

        set_font_style(test, UNDERLINE_FONT);
        REQUIRE(get_font_style(test) == UNDERLINE_FONT);

        set_font_style(test, NORMAL_FONT);
        REQUIRE(get_font_style(test) == NORMAL_FONT);
    }

    SECTION("can set font style by name")
    {
        set_font_style("Arial", BOLD_FONT);
        REQUIRE(get_font_style("Arial") == BOLD_FONT);

        set_font_style("Arial", ITALIC_FONT);
        REQUIRE(get_font_style("Arial") == ITALIC_FONT);
    }

    free_all_fonts();
}

TEST_CASE("can load multiple font sizes", "[text]")
{
    font test;
    #ifndef __linux__
    test = load_font("Arial", "Arial");
    #else
    test = load_font("Arial", "DejaVuSans.ttf");
    #endif

    SECTION("default size 64 is available")
    {
        REQUIRE(font_has_size(test, 64));
    }

    SECTION("can preload multiple sizes")
    {
        font_load_size(test, 12);
        REQUIRE(font_has_size(test, 12));

        font_load_size(test, 24);
        REQUIRE(font_has_size(test, 24));

        font_load_size(test, 48);
        REQUIRE(font_has_size(test, 48));
    }

    SECTION("can verify size availability")
    {
        REQUIRE_FALSE(font_has_size(test, 99));
        font_load_size(test, 99);
        REQUIRE(font_has_size(test, 99));
    }

    SECTION("can preload sizes by font name")
    {
        REQUIRE_FALSE(font_has_size("Arial", 36));
        font_load_size("Arial", 36);
        REQUIRE(font_has_size("Arial", 36));
    }

    free_all_fonts();
}

TEST_CASE("can check font availability", "[text]")
{
    #ifndef __linux__
    REQUIRE_FALSE(has_font("NonExistentFont"));
    #else
    REQUIRE_FALSE(has_font("NonExistentFont"));
    #endif

    font test;
    #ifndef __linux__
    test = load_font("TestFont", "Arial");
    #else
    test = load_font("TestFont", "DejaVuSans.ttf");
    #endif

    SECTION("loaded font can be checked by name")
    {
        REQUIRE(has_font("TestFont"));
    }

    SECTION("loaded font can be checked by reference")
    {
        REQUIRE(has_font(test));
    }

    free_all_fonts();
}
