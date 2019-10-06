#include "random_wrapper.hpp"

d_common::RandomWrapper d_common::RandomWrapper::instance{};

d_common::RandomWrapper::RandomWrapper() : mt_(seed()) {}

unsigned int d_common::RandomWrapper::seed() {
    std::random_device dev;
    auto value = dev();
    return value;
}

char d_common::RandomWrapper::get_char() {
    const auto gendy = random_integer<uint32_t>(0, sizeof ALLOWED_CHARS - 1);
    return ALLOWED_CHARS[static_cast<uint32_t>(gendy)];
}

std::string d_common::RandomWrapper::get_string(const size_t length) {
    std::string returns(length, '\0');
    for (size_t i = 0; i < length; ++i) {
        returns[i] = get_char();
    }
    return returns;
}

std::vector<uint8_t> d_common::RandomWrapper::random_bytes(size_t how_many) {
    std::vector<uint8_t> bytes;
    bytes.reserve(how_many);
    for (size_t i = 0; i < how_many; ++i) {
        bytes.emplace_back(random_integer_nomod<uint8_t>());
    }
    return bytes;
}
