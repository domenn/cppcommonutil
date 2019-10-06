#pragma once
#include <vector>
namespace d_common {
namespace vector {
/**
 * \brief Returns index of item. -1 if not exists.
 * \tparam T types in vector
 * \param items vector to search
 * \param to_search item we try to find
 * \return Index if found.
 */
template <typename T>
int64_t find_in_vector(const std::vector<T>& items, const T& to_search) {
    auto found = std::find(items.begin(), items.end(), to_search);
    if (found != items.end()) {
        return found - items.begin();
    }
    return -1;
}

/**
 * \brief Does vector items contain item to_search?
 * \tparam T types in vector
 * \param items vector to search
 * \param to_search item we try to find
 * \return True if it has item.
 */
template <typename T>
bool contains(const std::vector<T>& items, const T& to_search) {
    return find_in_vector(items, to_search) >= 0;
}

/**
 * \brief Does vector `items` contain anything in `items_to_search`?
 *
 * \tparam T types in vector
 * \tparam container Container of `T`, should have begin() and end() methods.
 * \param items vector to search.
 * \param items_to_search items we try to find.
 * \return True if it has item.
 */
template <typename T, typename container = std::initializer_list<std::string>>
bool contains_one_of(const std::vector<T>& items, const container& items_to_search) {
    for (const auto& itm : items_to_search) {
        if (find_in_vector(items, itm) >= 0) {
            return true;
        }
    }
    return false;
}

template <typename Tfirst, typename Tsecond>
auto vector_of_pairs_to_two_vectors(
    std::vector<std::pair<Tfirst, Tsecond>>&& input_item) {
    std::vector<Tfirst> keys;
    std::vector<Tsecond> values;

    for (auto it = std::make_move_iterator(input_item.begin()),
              end = std::make_move_iterator(input_item.end());
         it != end; ++it) {
        keys.push_back(std::move(it->first));
        values.push_back(std::move(it->second));
    }
    return std::make_pair(std::move(keys), std::move(values));
}

}  // namespace vector
}  // namespace d_common