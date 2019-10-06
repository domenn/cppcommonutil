#pragma once
#include <cstdint>
#include <ostream>

namespace d_common {
template <typename T, typename char_t_>
class BasicByteArrayOstream {
public:
    const char_t_* ptr_item_{};
    size_t size_{};
    // public:
    //    ByteArrayAsNumbers(const ByteArrayAsNumbers& other) = delete;
    //    ByteArrayAsNumbers(ByteArrayAsNumbers&& other) = default;
    //    ByteArrayAsNumbers& operator=(const ByteArrayAsNumbers& other) = delete;
    //    ByteArrayAsNumbers& operator=(ByteArrayAsNumbers&& other) = default;
    //    ~ByteArrayAsNumbers() noexcept = default;

    friend std::ostream& operator<<(std::ostream& os,
                                    const BasicByteArrayOstream<T, char_t_>& inst) {
        for (size_t i = 0; i < inst.size_; ++i) {
            os << "[" << static_cast<T>(inst.ptr_item_[i]) << "]";
        }
        return os;
    }
};

template <typename T>
using UByteArrayOstream = BasicByteArrayOstream<T, uint8_t>;
template <typename T>
using ByteArrayOstream = BasicByteArrayOstream<T, char>;
}  // namespace d_common