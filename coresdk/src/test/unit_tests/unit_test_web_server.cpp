/*  Copyright (C) 2024 Hayley Hughes
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <vector>

#include "catch.hpp"

#include "web_server.h"

using namespace splashkit_lib;

TEST_CASE("split uri stubs", "[web_server]")
{
    vector<string> empty;

    SECTION("can parse uri without path")
    {
        CHECK(split_uri_stubs("") == empty);
        CHECK(split_uri_stubs("/") == (vector<string>){""});
    }

    SECTION("can parse uri with path")
    {
        CHECK(split_uri_stubs("/one") == (vector<string>){"one"});
        CHECK(split_uri_stubs("/one/two/") == (vector<string>){"one", "two"});
    }

    SECTION("can parse uri with arguments")
    {
        CHECK(split_uri_stubs("?foo=bar") == empty);
        CHECK(split_uri_stubs("/one/?foo=bar") == (vector<string>){"one"});
        CHECK(split_uri_stubs("/one?foo=bar") == (vector<string>){"one"});
    }
}

TEST_CASE("split uri stubs edge cases", "[web_server]")
{
    vector<string> empty;

    SECTION("handles trailing slashes consistently")
    {
        CHECK(split_uri_stubs("/one/two") == (vector<string>){"one", "two"});
        CHECK(split_uri_stubs("/one/two/") == (vector<string>){"one", "two"});
    }

    SECTION("handles multiple consecutive slashes")
    {
        CHECK(split_uri_stubs("//one") == (vector<string>){"", "one"});
        CHECK(split_uri_stubs("/one//two") == (vector<string>){"one", "", "two"});
    }

    SECTION("handles deeply nested paths")
    {
        CHECK(split_uri_stubs("/a/b/c/d/e") == (vector<string>){"a", "b", "c", "d", "e"});
    }

    SECTION("handles numeric path segments")
    {
        CHECK(split_uri_stubs("/1/2/3") == (vector<string>){"1", "2", "3"});
    }

    SECTION("handles mixed query strings and fragments")
    {
        CHECK(split_uri_stubs("/api/users?id=123") == (vector<string>){"api", "users"});
        CHECK(split_uri_stubs("/api/users#section") == (vector<string>){"api", "users#section"});
        CHECK(split_uri_stubs("/api/users?id=123&name=test") == (vector<string>){"api", "users"});
    }

    SECTION("handles percent-encoded characters")
    {
        CHECK(split_uri_stubs("/api/users/john%20doe") == (vector<string>){"api", "users", "john%20doe"});
    }

    SECTION("handles empty segments as stubs")
    {
        CHECK(split_uri_stubs("//") == (vector<string>){""}); 
        CHECK(split_uri_stubs("/a//b") == (vector<string>){"a", "", "b"});
    }
}

TEST_CASE("request uri parsing", "[web_server]")
{
    SECTION("split_uri_stubs extracts path correctly")
    {
        // Given URI: /api/v1/users/123
        auto stubs = split_uri_stubs("/api/v1/users/123");
        CHECK(stubs.size() == 4);
        CHECK(stubs[0] == "api");
        CHECK(stubs[1] == "v1");
        CHECK(stubs[2] == "users");
        CHECK(stubs[3] == "123");
    }

    SECTION("split_uri_stubs handles root path")
    {
        auto stubs = split_uri_stubs("/");
        CHECK(stubs.size() == 1);
        CHECK(stubs[0] == "");
    }

    SECTION("split_uri_stubs preserves segment order")
    {
        auto stubs = split_uri_stubs("/first/second/third/fourth");
        CHECK(stubs.size() == 4);
        CHECK(stubs[0] == "first");
        CHECK(stubs[1] == "second");
        CHECK(stubs[2] == "third");
        CHECK(stubs[3] == "fourth");
    }
}

// INTEGRATION TESTS (Require Running Server)
// ===============================================
// The following tests cannot be implemented as unit tests because they require
// actual http_request objects, which can only be obtained from a running web server.
// See test_web_server.cpp and test_web_service.cpp for integration test examples.
//
// Future test candidates (if http_request mocking is implemented):
// - request_uri() - Extract full URI from request
// - request_method() - Get HTTP method (GET, POST, PUT, DELETE)
// - request_query_parameter() - Parse query parameters
// - request_body() - Get request body content
// - request_has_query_parameter() - Check parameter existence
// - is_request_for() - Route matching by method and path
// - is_get_request_for() - GET route matching
// - is_post_request_for() - POST route matching
// - is_put_request_for() - PUT route matching
// - is_delete_request_for() - DELETE route matching
// - send_response() - Response building and sending
// - send_html_file_response() - HTML file serving
// - Web server lifecycle (start, process requests, stop)
