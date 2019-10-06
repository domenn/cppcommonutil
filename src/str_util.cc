
#include <str_util.hpp>
#include <iomanip>
#include <algorithm>
#include <iostream>
#include <cstring>

using namespace std;

std::string d_common::str_util::boolToString(const bool b) {
  return b ? std::string("true") : std::string("false");
}

std::string d_common::str_util::ltrim(const std::string &s) {
  size_t first = s.find_first_not_of(' ');
  if (first == std::string::npos) {
    return s;
  }
  return s.substr(first, (s.length() - first + 1));
}

std::string d_common::str_util::rtrim(const std::string &s) {
  size_t last = s.find_last_not_of(' ');
  return s.substr(0, last + 1);
}

std::string d_common::str_util::trim(const std::string &s) { return ltrim(rtrim(s)); }

#ifdef _WIN32
#pragma warning(push)
// Disable: Warning C4244 '=': conversion from 'int' to 'wchar_t', possible loss of data
// * To get rid of this I would have to make a lambda that does explicit cast and
// delegates. tolower and toupper accept int and that's how it is.
#pragma warning(disable : 4244)
#endif
bool d_common::str_util::equalsCaseInsensitive(const std::wstring &string1,
                                               const std::wstring &string2) {
  return convert_case(std::wstring(string1), ::tolower) ==
      convert_case(std::wstring(string2), ::tolower);
}

bool d_common::str_util::equalsCaseInsensitive(const std::string &string1,
                                               const std::string &string2) {
  return convert_case(std::string(string1), ::tolower) ==
      convert_case(std::string(string2), ::tolower);
}

template<typename charT>
charT lower_wrapper(charT letter) {
  return static_cast<charT>(::tolower(letter));
}

template<typename charT>
charT upper_wrapper(charT letter) {
  return static_cast<charT>(::toupper(letter));
}

std::wstring d_common::str_util::toLowerCase(const std::wstring &s) {
  return convert_case(std::wstring(s), lower_wrapper<wchar_t>);
}

std::string d_common::str_util::toLowerCase(const std::string &s) {
  return convert_case(std::string(s), lower_wrapper<char>);
}

template<typename charT, typename Tfunction>
void d_common::str_util::convert_case_in_place_impl(std::basic_string<charT> &s,
                                                    Tfunction f) {
  std::transform(s.begin(), s.end(), s.begin(), f);
}

void d_common::str_util::lower_case_transform(std::string &s) {
  convert_case_in_place_impl(s, lower_wrapper<char>);
}

void d_common::str_util::upper_case_transform(std::string &s) {
  convert_case_in_place_impl(s, upper_wrapper<char>);
}

std::string d_common::str_util::toUpperCase(const std::string &s) {
  return convert_case(std::string(s), upper_wrapper<char>);
}

template<typename charT, typename Tfunction>
std::basic_string<charT> &d_common::str_util::convert_case(std::basic_string<charT> &&s,
                                                           Tfunction f) {
  str_util::convert_case_in_place_impl(s, f);
  return s;
}
#ifdef _WIN32
#pragma warning(pop)
#endif

void d_common::str_util::println(const std::string &text) {
  // C++ standard states that endl flushes the buffer
  cout << text << endl;
}

void d_common::str_util::println(const std::string &parameter,
                                 const std::string &description) {
  // C++ standard states that endl flushes the buffer
  cout << setw(10) << parameter << " - " << description << endl;
}

template<typename stringT>
bool d_common::str_util::replace_one_template(stringT &str, const stringT &from,
                                              const stringT &to, int from_pos) {
  size_t start_pos = str.find(from, from_pos);
  if (start_pos == stringT::npos)
    return false;
  str.replace(start_pos, from.length(), to);
  return true;
}

bool d_common::str_util::replace_one(std::wstring &str, const std::wstring &from,
                                     const std::wstring &to, int from_pos) {
  return replace_one_template(str, from, to, from_pos);
}

std::string &d_common::str_util::transform_replace_all(std::string &str, char from,
                                                       const char *to, size_t to_size) {
  const size_t occurrences = std::count(str.begin(), str.end(), from);
  if (!occurrences) {
    return str;
  }
  int64_t index_writing = str.size() + occurrences * (to_size - 1);
  int64_t index_reading = str.size() - 1;
  if (index_writing > static_cast<int64_t>(str.size())) {
    str.resize(index_writing);
  } else if (index_writing < static_cast<int64_t>(str.size())) {
    // To is empty string (that's the only way for us to be able to come here ..)
    // We only have to erase all apearances of from.
    return erase_all_of(str, from);
  }
  // Work from right to left to reduce number of char moves and allow in-place
  // transformation without seperate read / write buffers.
  int64_t current_token_size = 0;

  for (; index_reading >= 0; --index_reading, ++current_token_size, --index_writing) {
    if (str[index_reading] == from) {
      memcpy(str.data() + index_writing, str.data() + index_reading + 1,
             current_token_size);
      index_writing -= to_size;
      memcpy(str.data() + index_writing, to, to_size);
      current_token_size = -1;
      index_writing += 1;
    }
  }
  return str;
}

std::string &d_common::str_util::erase_all_of(std::string &str, char character) {
  // Removes characters but leaves gibberish on right, copying them left.
  std::string::iterator end_pos = std::remove(str.begin(), str.end(), character);
  // Remove gibberish on right that was introduced by copying to left.
  str.erase(end_pos, str.end());
  return str;
}

bool d_common::str_util::replace_one(std::string &str, const std::string &from,
                                     const std::string &to, int from_pos) {
  return replace_one_template(str, from, to, from_pos);
}

template<typename charT>
bool d_common::str_util::startsWithTemplate(const std::basic_string<charT> &input,
                                            const std::basic_string<charT> &starting) {
  return input.find(starting) == 0;
}

template<typename charT>
bool d_common::str_util::ends_with(const std::basic_string<charT> &string,
                                   const charT *const to_find,
                                   const size_t to_find_size) {
  if (string.length() >= to_find_size) {
    return string.compare(string.length() - to_find_size, to_find_size, to_find) == 0;
  }
  return false;
}

template<typename charT>
bool d_common::str_util::endsWithTemplate(const std::basic_string<charT> &input,
                                          const std::basic_string<charT> &ending) {
  return ends_with(input, ending.c_str(), ending.size());
}

bool d_common::str_util::ends_with(const std::wstring &input,
                                   const std::wstring &ending) {
  return endsWithTemplate(input, ending);
}

bool d_common::str_util::starts_with(const std::wstring &input,
                                     const std::wstring &starting) {
  return startsWithTemplate(input, starting);
}

bool d_common::str_util::ends_with(const std::string &input, const std::string &ending) {
  return endsWithTemplate(input, ending);
}

bool d_common::str_util::starts_with(const std::string &input,
                                     const std::string &starting) {
  return startsWithTemplate(input, starting);
}

std::vector<std::string> d_common::str_util::split(const std::string &str,
                                                   char const delimiter) {
  std::vector<std::string> result;

  std::istringstream ss(str);
  std::string token;
  while (getline(ss, token, delimiter)) {
    result.push_back(token);
  }

  return result;
}

std::vector<char *> d_common::str_util::vector_of_strings_to_vector_of_chars(
    const std::vector<std::string> &vector_of_strings) {
  std::vector<char *> results;
  results.reserve(vector_of_strings.size());
  for (auto &item : vector_of_strings) {
    results.push_back(const_cast<char *>(item.c_str()));
  }
  return results;
}

std::string d_common::str_util::single_line(std::string_view input, int line_number) {
  std::string returns(input);
  erase_all_of(returns, '\r');
  return split(returns, '\n')[line_number];
}

std::vector<std::string_view> d_common::str_util::split_in_place(
    std::string_view strv, std::string_view delims) {
  std::vector<std::string_view> output;
  size_t first = 0;
  while (first < strv.size()) {
    const auto second = strv.find_first_of(delims, first);
    if (first != second)
      output.emplace_back(strv.substr(first, second - first));
    if (second == std::string_view::npos)
      break;
    first = second + 1;
  }
  return output;
}

std::pair<std::string_view, std::string_view> d_common::str_util::tokenize(
    std::string_view input, char delimiter) {
  auto idx = input.find_first_of(delimiter);
  if (idx == std::string_view::npos) {
    return std::make_pair(input, std::string_view{});
  }
  auto empty_sw = std::string_view{};
  return std::make_pair<std::string_view, std::string_view>(input.substr(0, idx),
                                                            input.data() + idx + 1);
}
std::string d_common::str_util::ensure_trailing(std::string &&item, char c) {
  if (item.back() != c) {
    item.push_back(c);
  }
  return item;
}
std::string &d_common::str_util::transform_replace_all(std::string &str, const std::string &from, const std::string &to) {
  size_t pos = 0;
  while((pos = str.find(from, pos)) != std::string::npos) {
    str.replace(pos, from.length(), to);
    pos += to.length();
  }
  return str;
}

template bool d_common::str_util::ends_with(const std::basic_string<char> &, const char *,
                                            size_t);
template bool d_common::str_util::ends_with(const std::basic_string<wchar_t> &,
                                            const wchar_t *, size_t);


// // // // ADVANCED STR FORMAT WAY OF DOING THINGS ... PROTOTYPES
//
//
//  auto stringies = {"first"s, "second"s, "third"s};
//  // std::vector<std::string> stringies {"only"s};
//  // std::string empty_s[0];
//
//
//
////  SPDLOG_INFO(std::accumulate(
////      std::begin(stringies), std::end(stringies), std::string{},
////      [](std::string &s, const std::string &piece) -> decltype(auto) {
////        return s.empty() ? piece : s.append(", ").append(piece);
////      }));
////
////  SPDLOG_INFO(std::accumulate(std::begin(stringies), std::end(stringies),
////                              "No seps: "s));
////
////  SPDLOG_INFO(std::accumulate(
////      std::begin(stringies) + 1, std::end(stringies),
////      "With seps: " + *std::begin(stringies),
////      [](const auto &left, const auto &right) { return left + ", " + right; }));
////
////  std::ostringstream output;
////  std::copy(std::begin(stringies), std::end(stringies),
////      // std::back_inserter(output)
////            std::ostream_iterator<std::string>(output, " ;; ")
////  );
////  SPDLOG_INFO(output.str());
////
////  std::cout << d_common::str_util::toLowerCase(
////      "StrUtilLowerCASING_THIS-andOtherStuff!")
////            << std::endl;
//  return 0;