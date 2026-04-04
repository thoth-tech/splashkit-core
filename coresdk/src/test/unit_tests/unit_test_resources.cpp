#include "catch.hpp"

#include "resources.h"

using namespace splashkit_lib;
using std::string;

namespace
{
    struct resources_path_guard
    {
        string original;

        resources_path_guard()
            : original(path_to_resources())
        {
        }

        ~resources_path_guard()
        {
            set_resources_path(original);
        }
    };

#ifdef WINDOWS
    const string PATH_SEP = "\\";
#else
    const string PATH_SEP = "/";
#endif

    string dir_path(const string &base, const string &folder)
    {
        return base + PATH_SEP + folder + PATH_SEP;
    }

    string file_path(const string &base, const string &filename)
    {
        return base + PATH_SEP + filename;
    }
}

TEST_CASE("configured resources path is returned", "[resources][path_to_resources]")
{
    resources_path_guard guard;
    const string test_path = "resources-base";

    set_resources_path(test_path);

    REQUIRE(path_to_resources() == test_path);
}

TEST_CASE("resource kind paths map to expected folders", "[resources][path_to_resources_kind]")
{
    resources_path_guard guard;
    const string base_path = "resources-base";

    set_resources_path(base_path);

    REQUIRE(path_to_resources(ANIMATION_RESOURCE) == dir_path(base_path, "animations"));
    REQUIRE(path_to_resources(BUNDLE_RESOURCE) == dir_path(base_path, "bundles"));
    REQUIRE(path_to_resources(FONT_RESOURCE) == dir_path(base_path, "fonts"));
    REQUIRE(path_to_resources(IMAGE_RESOURCE) == dir_path(base_path, "images"));
    REQUIRE(path_to_resources(JSON_RESOURCE) == dir_path(base_path, "json"));
    REQUIRE(path_to_resources(MUSIC_RESOURCE) == dir_path(base_path, "sounds"));
    REQUIRE(path_to_resources(SERVER_RESOURCE) == dir_path(base_path, "server"));
    REQUIRE(path_to_resources(SOUND_RESOURCE) == dir_path(base_path, "sounds"));
    REQUIRE(path_to_resources(TIMER_RESOURCE) == base_path);
    REQUIRE(path_to_resources(OTHER_RESOURCE) == base_path);
}

TEST_CASE("resources path can be updated", "[resources][set_resources_path]")
{
    resources_path_guard guard;
    const string first_path = "resources-path-one";
    const string second_path = "resources-path-two";

    set_resources_path(first_path);
    REQUIRE(path_to_resources() == first_path);

    set_resources_path(second_path);
    REQUIRE(path_to_resources() == second_path);
    REQUIRE(path_to_resources(IMAGE_RESOURCE) == dir_path(second_path, "images"));
}

TEST_CASE("path_to_resource combines folder and filename", "[resources][path_to_resource]")
{
    resources_path_guard guard;
    const string base_path = "resources-base";

    set_resources_path(base_path);

    REQUIRE(path_to_resource("ufo.png", IMAGE_RESOURCE) == dir_path(base_path, "images") + "ufo.png");
    REQUIRE(path_to_resource("beep.wav", SOUND_RESOURCE) == dir_path(base_path, "sounds") + "beep.wav");
    REQUIRE(path_to_resource("config.json", JSON_RESOURCE) == dir_path(base_path, "json") + "config.json");
    REQUIRE(path_to_resource("root.txt", OTHER_RESOURCE) == file_path(base_path, "root.txt"));
}
