#pragma once

#include <string>
#include "stopwatch.hpp"
namespace d_common {

/**
 * Uses plog and fmt to write a message on destructor call. Message to be written is
 * provied in object's constructor. Uses fmt lib to format the message. Construct with the
 * message that the destructor shall print. Message must conta
 */
class DetailingOutputStopwatch : public Stopwatch {
 public:
  // TODO d : Fix fmt template wall.
  //    friend std::ostream& operator<<(std::ostream& os,
  //                                    const DetailingOutputStopwatch& dt) {
  //        os << dt.expired_ms() << "." << dt.expired_micros() % 1000 << " ms";
  //        return os;
  //    }

  [[nodiscard]] std::string as_string() const {
    return fmt::to_string(expired_ms()) + "." + fmt::to_string(expired_micros() % 1000) + " ms";
  }

  ~DetailingOutputStopwatch() override = default;
};

}  // namespace d_common