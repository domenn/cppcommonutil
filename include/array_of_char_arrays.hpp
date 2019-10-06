#pragma once
#include <cstring>
#include <vector>
#include "str_util.hpp"
#include <array>

class ArrayOfCharArrays {
    std::vector<char*> vec_;

public:
    /**
     * \brief Useful for tests .. creates "cmd arguments" as c/c++ makes them.
     * \param inputs list of arguments.
     * \param dummy_first_arg_already_added If true, constructor expects first parameter
     * already in list `inputs`. If this is false, it will add one parameter to start.
     * This parameter is program name which we ignore anyway, but must be there for boost
     * parser to work correctly.
     */
    ArrayOfCharArrays(std::initializer_list<std::string>&& inputs,
                      bool dummy_first_arg_already_added) {
        vec_.reserve(inputs.size());
        if (!dummy_first_arg_already_added) {
            const std::array<std::string, 1> first_thing{"n"};
            add_more_data(first_thing.begin(), first_thing.end());
        }
        add_more_data(inputs.begin(), inputs.end());
    }

    template <class iterator_type>
    void add_more_data(iterator_type it, iterator_type end) {
        while (it != end) {
            const auto& input = *it++;
            auto dynamic_all = new char[input.size() + 1];
            d_common::str_util::safe_strcpy(dynamic_all, input.size() + 1, input.data());
            vec_.emplace_back(dynamic_all);
        }
    }

    ~ArrayOfCharArrays() {
        for (size_t i = 0; i < vec_.size(); ++i) {
            delete[] vec_[i];
        }
    }
    char** array_of_arrays() { return vec_.data(); }

    template <typename integer_type>
    integer_type cnt() const {
        return static_cast<integer_type>(vec_.size());
    }
};