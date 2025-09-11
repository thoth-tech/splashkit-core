/**
 * Bundle unit tests
 */

#include "catch.hpp"

#include "bundles.h"
#include "resources.h"
#include "audio.h"
#include "images.h"
#include "timers.h"
#include "text.h"

using namespace splashkit_lib;

// Log pointers of freed resources
void free_notification(void *resource)
{
    UNSCOPED_INFO("Freeing: " << resource);
}

TEST_CASE("Load and free resource bundle", "[load_resource_bundle][free_resource_bundle]")
{
    register_free_notifier(&free_notification);

    /**
     * Load test resource bundle containing:
     * ANIM,WalkingScript,kermit.txt
     * BITMAP,FrogBmp,frog.png, 73, 105, 4, 4, 16
     * FONT,hara,hara.ttf
     * SOUND,error,error.wav
     * MUSIC,background,energize_my_mind.mod
     * TIMER,my timer
     * BUNDLE,blah,blah.txt
     */
    load_resource_bundle("test", "test.txt");

    // Confirm bundle and its resources are loaded
    REQUIRE(has_resource_bundle("test"));
    REQUIRE(has_animation_script("WalkingScript"));
    REQUIRE(has_bitmap("FrogBmp"));
    REQUIRE(has_font("hara"));
    REQUIRE(has_sound_effect("error"));
    REQUIRE(has_music("background"));
    REQUIRE(has_timer("my timer"));
    REQUIRE(has_resource_bundle("blah"));
    REQUIRE(has_bitmap("ufo"));
    REQUIRE(has_resource_bundle("test"));

    // Free bundle and confirm resources are freed
    free_resource_bundle("test");

    REQUIRE_FALSE(has_resource_bundle("test"));
    REQUIRE_FALSE(has_animation_script("WalkingScript"));
    REQUIRE_FALSE(has_bitmap("FrogBmp"));
    REQUIRE_FALSE(has_font("hara"));
    REQUIRE_FALSE(has_sound_effect("error"));
    REQUIRE_FALSE(has_music("background"));
    REQUIRE_FALSE(has_timer("my timer"));
    REQUIRE_FALSE(has_resource_bundle("blah"));
    REQUIRE_FALSE(has_bitmap("ufo"));
}