//
//  json_driver.h
//  splashkit
//
//  Created by James Armstrong & Jake Renzella on 03/09/2016.
//  Copyright © 2016 Andrew Cain. All rights reserved.
//

#ifndef SPLASHKIT_JSON_DRIVER_H
#define SPLASHKIT_JSON_DRIVER_H

#include "json.h"
#include "backend_types.h"
#include "utility_functions.h"
#include "json.hpp"

#include <string>
#include <vector>
#include <functional>
#include <stdexcept>
#include <map>

using backend_json = nlohmann::json;
using std::string;
using std::vector;
using std::map;
using std::make_pair;

namespace splashkit_lib
{
    struct sk_json
    {
        pointer_identifier id;
        backend_json data;
    };

    void sk_delete_json(json j);

    string json_type_to_string(backend_json::value_t type);

    inline bool is_type_number(backend_json::value_t type)
    {
        return type == backend_json::value_t::number_float ||
               type == backend_json::value_t::number_integer ||
               type == backend_json::value_t::number_unsigned;
    }

    template <typename T>
    void sk_json_add_value(json j, const string &key, T value)
    {
        if (INVALID_PTR(j, JSON_PTR))
        {
            LOG(WARNING) << "Passed an invalid json object to json_add_value";
            return;
        }

        j->data[key] = value;
    }

    template <typename T>
    T sk_json_read_value(json j, const string &key, backend_json::value_t expected_type)
    {
        if (INVALID_PTR(j, JSON_PTR))
        {
            throw std::runtime_error("Invalid JSON pointer passed to json_read_value");
        }

        if (j->data.find(key) == j->data.end())
        {
            throw std::runtime_error("Key '" + key + "' not found in JSON");
        }

        backend_json value = j->data[key];
        backend_json::value_t actual_type = value.type();

        if ((actual_type != expected_type) &&
            !(is_type_number(actual_type) && is_type_number(expected_type)))
        {
            throw std::runtime_error("JSON key '" + key + "' has type '" +
                                     json_type_to_string(actual_type) +
                                     "', but expected '" +
                                     json_type_to_string(expected_type) + "'");
        }

        return value;
    }

    template <typename T>
    void sk_json_read_array(json j, const string &key, vector<T> &out)
    {
        if (INVALID_PTR(j, JSON_PTR))
        {
            LOG(WARNING) << "Passed an invalid json object to json_read_array";
            return;
        }

        if (!j->data[key].is_array())
        {
            LOG(ERROR) << "JSON key value is not an array. Has type "
                       << json_type_to_string(j->data[key].type());
            return;
        }

        out.clear();

        for (const auto &e : j->data[key])
        {
            out.push_back(e);
        }
    }

    inline map<string, string> sk_json_to_map(json j)
    {
        map<string, string> result;

        for (auto it = j->data.begin(); it != j->data.end(); ++it)
        {
            if (!it.value().is_string())
            {
                LOG(ERROR) << "Key '" << it.key() << "' is not a string in JSON map";
                continue;
            }

            result.insert(make_pair(it.key(), it.value()));
        }

        return result;
    }
}

#endif // SPLASHKIT_JSON_DRIVER_H
