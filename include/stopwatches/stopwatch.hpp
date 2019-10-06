#pragma once

#include <chrono>
namespace d_common {
class Stopwatch {

public:
    Stopwatch() { reset(); }
    virtual ~Stopwatch() = default;
    size_t expired_ms() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::steady_clock::now() - start_)
            .count();
    }
    size_t expired_micros() const {
        return std::chrono::duration_cast<std::chrono::microseconds>(
                   std::chrono::steady_clock::now() - start_)
            .count();
    }
    void reset() { start_ = std::chrono::steady_clock::now(); };

private:
    std::chrono::time_point<std::chrono::steady_clock> start_;
};
}  // namespace d_common