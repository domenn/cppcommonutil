#pragma once
#include <random>

#ifdef max
#undef max
#endif  // max
namespace d_common {

class RandomWrapper {
    static constexpr char ALLOWED_CHARS[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";
    static constexpr float NORMALIZER = 1.0f / std::mt19937::max();
    static RandomWrapper instance;
    static unsigned int seed();
    mutable std::mt19937 mt_;
    RandomWrapper();

public:
    RandomWrapper(const RandomWrapper& other) = delete;
    RandomWrapper(RandomWrapper&& other) noexcept = delete;

    static char get_char();
    static std::string get_string(size_t length);
    /**
     * Returns number in range  [`min`, `max`], but returning bounds is very rare.
     * @param min Inclusive min float.
     * @param max Inclusive max float.
     * @return generated random value.
     */
    static float get_float(float min, float max) {
        return instance.mt_() * NORMALIZER * (max - min) + min;
    }

    template <typename __integer_type, typename __min_max_type, typename __engine>
    static __integer_type random_integer(__engine& engine, __min_max_type min, __min_max_type max) {
        return static_cast<__integer_type>(engine() % (max - min) + min);
    }

    /**
     * Returns number in range  [`min`, `max`).
     * @param min Inclusive min integer.
     * @param max Exclusive max integer.
     * @return generated random value.
     */
    template <typename __integer_type>
    static __integer_type random_integer(uint32_t min, uint32_t max) {
        return random_integer<__integer_type, uint32_t, std::mt19937>(instance.mt_, min, max);
    }

    template <typename __integer_type, typename __engine>
    static __integer_type random_integer_nomod(__engine& engine) {
        return static_cast<__integer_type>(engine());
    }

    template <typename __integer_type>
    static __integer_type random_integer_nomod() {
        return random_integer_nomod<__integer_type>(instance.mt_);
    }

    static std::vector<uint8_t> random_bytes(size_t how_many);
};
}  // namespace d_common