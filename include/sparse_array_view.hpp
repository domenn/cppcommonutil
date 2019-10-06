#pragma once

#include <initializer_list>
#include <vector>
namespace d_common {
template <typename item_type, typename indexed_container_type>
class SparseArrayView {
    std::vector<size_t> indices_;
    indexed_container_type& container_ref_;
    //    template <typename iterator_type>
    //    SparseArrayView(iterator_type indices_begin, iterator_type indices_end){
    //        for(auto first = indices_begin; first != indices_end; ++first){
    //
    //        }
    //    }
public:
    class Iterator {
        friend class d_common::SparseArrayView<item_type, indexed_container_type>;

        size_t value_;
        d_common::SparseArrayView<item_type, indexed_container_type>& container_ref_;

        Iterator(
            size_t value,
            d_common::SparseArrayView<item_type, indexed_container_type>& container_ref)
            : value_(value), container_ref_(container_ref) {}

    public:
        Iterator& operator++() {
            ++value_;
            return *this;
        }

        Iterator operator++(int) { return {value_++, container_ref_}; }

//        item_type& operator->() {
//            return container_ref_.container_ref_[container_ref_.indices_[value_]];
//        }

        item_type& operator*() {
            return container_ref_.container_ref_[container_ref_.indices_[value_]];
        }

        bool operator==(const Iterator other) {
            return value_ == other.value_ && &container_ref_ == &other.container_ref_;
        }

        bool operator!=(const Iterator other) { return !(*this == other); }
    };

    SparseArrayView(indexed_container_type& items_ref, std::vector<size_t> indices)
        : indices_(std::move(indices)), container_ref_(items_ref) {}

    Iterator begin() { return {0, *this}; }
    Iterator end() { return {indices_.size(), *this}; }
};

}