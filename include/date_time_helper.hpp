#pragma once
#include <cstdint>
#include <chrono>

namespace d_common::time_cvt {

template <typename inT, typename returnT = inT>
inline returnT ms_to_s(inT ms) {
    return static_cast<returnT>(static_cast<double>(ms) * 0.001);
}
}  // namespace d_common::time_cvt

namespace d_common::current_time {

inline uint64_t milliseconds() {
    return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(
                                     std::chrono::system_clock::now().time_since_epoch())
                                     .count());
}

}  // namespace d_common::current_time
