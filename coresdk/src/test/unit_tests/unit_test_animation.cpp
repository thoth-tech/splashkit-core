/**
 * Animation Unit Tests
 *
 * Tests for animation script and animation functions in SplashKit.
 * Covers loading, creation, updating, and lifecycle of animations.
 */

#include "catch.hpp"

#include "animations.h"
#include "resources.h"

#include "logging_handling.h"

using namespace splashkit_lib;

// Test constants
const std::string TEST_SCRIPT_NAME = "kermit";
const std::string TEST_SCRIPT_FILE = "kermit.txt";
const std::string TEST_ANIMATION_NAME = "walkfront";  // Case-insensitive match

// =============================================================================
// Animation Script Tests
// =============================================================================

TEST_CASE("animation scripts can be loaded and freed", "[animation_script]")
{
    SECTION("can detect non-existent animation script")
    {
        REQUIRE_FALSE(has_animation_script("non_existent_script"));
        animation_script script = animation_script_named("non_existent_script");
        REQUIRE(script == nullptr);
    }
    SECTION("can load animation script from file")
    {
        REQUIRE_FALSE(has_animation_script(TEST_SCRIPT_NAME));
        animation_script script = load_animation_script(TEST_SCRIPT_NAME, TEST_SCRIPT_FILE);
        REQUIRE(script != nullptr);
        REQUIRE(has_animation_script(TEST_SCRIPT_NAME));
        free_animation_script(script);
        REQUIRE_FALSE(has_animation_script(TEST_SCRIPT_NAME));
    }
    SECTION("can retrieve loaded animation script by name")
    {
        animation_script script = load_animation_script(TEST_SCRIPT_NAME, TEST_SCRIPT_FILE);
        REQUIRE(script != nullptr);
        animation_script retrieved = animation_script_named(TEST_SCRIPT_NAME);
        REQUIRE(retrieved == script);
        free_animation_script(script);
    }
    SECTION("can free animation script by name")
    {
        load_animation_script(TEST_SCRIPT_NAME, TEST_SCRIPT_FILE);
        REQUIRE(has_animation_script(TEST_SCRIPT_NAME));
        free_animation_script(TEST_SCRIPT_NAME);
        REQUIRE_FALSE(has_animation_script(TEST_SCRIPT_NAME));
    }
    SECTION("loading invalid file returns nullptr")
    {
        disable_logging(WARNING);
        animation_script script = load_animation_script("invalid", "nonexistent.txt");
        enable_logging(WARNING);
        REQUIRE(script == nullptr);
        REQUIRE_FALSE(has_animation_script("invalid"));
    }
}

TEST_CASE("animation script properties can be retrieved", "[animation_script]")
{
    animation_script script = load_animation_script(TEST_SCRIPT_NAME, TEST_SCRIPT_FILE);
    REQUIRE(script != nullptr);

    SECTION("can get animation script name")
    {
        REQUIRE(animation_script_name(script) == TEST_SCRIPT_NAME);
    }
    SECTION("can get animation count")
    {
        int count = animation_count(script);
        REQUIRE(count > 0);
    }
    SECTION("can check if animation exists in script")
    {
        REQUIRE(has_animation_named(script, "WalkFront"));
        REQUIRE(has_animation_named(script, "walkfront"));  // Case insensitive
        REQUIRE(has_animation_named(script, "Dance"));
        REQUIRE_FALSE(has_animation_named(script, "NonExistentAnimation"));
    }
    SECTION("can get animation index by name")
    {
        int idx = animation_index(script, "WalkFront");
        REQUIRE(idx >= 0);

        disable_logging(WARNING);
        int invalid_idx = animation_index(script, "NonExistent");
        enable_logging(WARNING);
        REQUIRE(invalid_idx == -1);
    }

    free_animation_script(script);
}

TEST_CASE("animation script invalid pointer handling", "[animation_script]")
{
    SECTION("animation_script_name returns empty for null")
    {
        REQUIRE(animation_script_name(nullptr) == "");
    }
    SECTION("animation_count returns 0 for null")
    {
        disable_logging(WARNING);
        REQUIRE(animation_count(nullptr) == 0);
        enable_logging(WARNING);
    }
    SECTION("has_animation_named returns false for null")
    {
        disable_logging(WARNING);
        REQUIRE_FALSE(has_animation_named(nullptr, "test"));
        enable_logging(WARNING);
    }
    SECTION("animation_index returns -1 for null")
    {
        disable_logging(WARNING);
        REQUIRE(animation_index(nullptr, "test") == -1);
        enable_logging(WARNING);
    }
}

TEST_CASE("all animation scripts can be freed", "[animation_script]")
{
    load_animation_script("script1", TEST_SCRIPT_FILE);
    load_animation_script("script2", TEST_SCRIPT_FILE);
    REQUIRE(has_animation_script("script1"));
    REQUIRE(has_animation_script("script2"));

    free_all_animation_scripts();

    REQUIRE_FALSE(has_animation_script("script1"));
    REQUIRE_FALSE(has_animation_script("script2"));
}

// =============================================================================
// Animation Tests
// =============================================================================

TEST_CASE("animations can be created and freed", "[animation]")
{
    animation_script script = load_animation_script(TEST_SCRIPT_NAME, TEST_SCRIPT_FILE);
    REQUIRE(script != nullptr);

    SECTION("can create animation from script by name")
    {
        animation anim = create_animation(script, TEST_ANIMATION_NAME);
        REQUIRE(anim != nullptr);
        free_animation(anim);
    }
    SECTION("can create animation from script by index")
    {
        int idx = animation_index(script, TEST_ANIMATION_NAME);
        animation anim = create_animation(script, idx, false);
        REQUIRE(anim != nullptr);
        free_animation(anim);
    }
    SECTION("can create animation with sound disabled")
    {
        animation anim = create_animation(script, TEST_ANIMATION_NAME, false);
        REQUIRE(anim != nullptr);
        free_animation(anim);
    }
    SECTION("can create animation from script name string")
    {
        animation anim = create_animation(TEST_SCRIPT_NAME, TEST_ANIMATION_NAME);
        REQUIRE(anim != nullptr);
        free_animation(anim);
    }

    free_animation_script(script);
}

TEST_CASE("animation properties can be retrieved", "[animation]")
{
    animation_script script = load_animation_script(TEST_SCRIPT_NAME, TEST_SCRIPT_FILE);
    REQUIRE(script != nullptr);
    animation anim = create_animation(script, TEST_ANIMATION_NAME, false);
    REQUIRE(anim != nullptr);

    SECTION("can get animation name")
    {
        std::string name = animation_name(anim);
        REQUIRE_FALSE(name.empty());
    }
    SECTION("can get current cell")
    {
        int cell = animation_current_cell(anim);
        REQUIRE(cell >= 0);
    }
    SECTION("animation has not ended initially")
    {
        REQUIRE_FALSE(animation_ended(anim));
    }
    SECTION("animation entered frame initially")
    {
        REQUIRE(animation_entered_frame(anim));
    }
    SECTION("can get frame time")
    {
        float frame_time = animation_frame_time(anim);
        REQUIRE(frame_time >= 0.0f);
    }
    SECTION("can get current vector")
    {
        vector_2d vec = animation_current_vector(anim);
        // Vector may be (0,0) for static frames
        REQUIRE((vec.x == vec.x));  // Just check it's a valid number (not NaN)
    }

    free_animation(anim);
    free_animation_script(script);
}

TEST_CASE("animation lifecycle works correctly", "[animation]")
{
    animation_script script = load_animation_script(TEST_SCRIPT_NAME, TEST_SCRIPT_FILE);
    REQUIRE(script != nullptr);

    SECTION("animation updates correctly")
    {
        animation anim = create_animation(script, TEST_ANIMATION_NAME, false);
        REQUIRE(anim != nullptr);

        int initial_cell = animation_current_cell(anim);

        // Update animation multiple times
        for (int i = 0; i < 100; i++)
        {
            update_animation(anim);
        }

        // Cell may have changed after updates
        int final_cell = animation_current_cell(anim);
        REQUIRE(final_cell >= 0);

        free_animation(anim);
    }
    SECTION("animation can be restarted")
    {
        animation anim = create_animation(script, TEST_ANIMATION_NAME, false);
        REQUIRE(anim != nullptr);

        // Update to progress the animation
        for (int i = 0; i < 50; i++)
        {
            update_animation(anim);
        }

        float time_before = animation_frame_time(anim);

        // Restart animation
        restart_animation(anim);

        float time_after = animation_frame_time(anim);
        REQUIRE(time_after == 0.0f);
        REQUIRE_FALSE(animation_ended(anim));

        free_animation(anim);
    }
    SECTION("animation with percentage update")
    {
        animation anim = create_animation(script, TEST_ANIMATION_NAME, false);
        REQUIRE(anim != nullptr);

        // Update with percentage
        update_animation(anim, 0.5f);

        float frame_time = animation_frame_time(anim);
        REQUIRE(frame_time > 0.0f);

        free_animation(anim);
    }

    free_animation_script(script);
}

TEST_CASE("animation can be assigned to different scripts/animations", "[animation]")
{
    animation_script script = load_animation_script(TEST_SCRIPT_NAME, TEST_SCRIPT_FILE);
    REQUIRE(script != nullptr);

    SECTION("can assign animation by name")
    {
        animation anim = create_animation(script, TEST_ANIMATION_NAME, false);
        REQUIRE(anim != nullptr);

        // Assign to different animation
        assign_animation(anim, "Dance");
        std::string name = animation_name(anim);
        REQUIRE(name == "dance");  // Names are stored lowercase

        free_animation(anim);
    }
    SECTION("can assign animation by index")
    {
        animation anim = create_animation(script, TEST_ANIMATION_NAME, false);
        REQUIRE(anim != nullptr);

        int dance_idx = animation_index(script, "Dance");
        assign_animation(anim, dance_idx);

        free_animation(anim);
    }
    SECTION("can assign animation with script and name")
    {
        animation anim = create_animation(script, TEST_ANIMATION_NAME, false);
        REQUIRE(anim != nullptr);

        assign_animation(anim, script, "WalkBack");
        std::string name = animation_name(anim);
        REQUIRE(name == "walkback");

        free_animation(anim);
    }
    SECTION("can assign animation with script name string")
    {
        animation anim = create_animation(script, TEST_ANIMATION_NAME, false);
        REQUIRE(anim != nullptr);

        // Use the string-based script name version
        std::string script_name = TEST_SCRIPT_NAME;
        std::string anim_name = "WalkLeft";
        assign_animation(anim, script_name, anim_name);
        std::string name = animation_name(anim);
        REQUIRE(name == "walkleft");

        free_animation(anim);
    }

    free_animation_script(script);
}

TEST_CASE("animation invalid pointer handling", "[animation]")
{
    SECTION("animation_ended returns true for null")
    {
        REQUIRE(animation_ended(nullptr));
    }
    SECTION("animation_current_cell returns 0 for null")
    {
        REQUIRE(animation_current_cell(nullptr) == 0);
    }
    SECTION("animation_name returns empty for null")
    {
        disable_logging(WARNING);
        REQUIRE(animation_name(nullptr) == "");
        enable_logging(WARNING);
    }
    SECTION("animation_entered_frame returns false for null")
    {
        disable_logging(WARNING);
        REQUIRE_FALSE(animation_entered_frame(nullptr));
        enable_logging(WARNING);
    }
    SECTION("animation_frame_time returns 0 for null")
    {
        disable_logging(WARNING);
        REQUIRE(animation_frame_time(nullptr) == 0.0f);
        enable_logging(WARNING);
    }
    SECTION("animation_current_vector returns zero vector for null")
    {
        vector_2d vec = animation_current_vector(nullptr);
        REQUIRE(vec.x == 0.0);
        REQUIRE(vec.y == 0.0);
    }
}
