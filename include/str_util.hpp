#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <cstring>

namespace d_common {

namespace constants::String {
static const wchar_t *WSTRING_TRUE = L"true";
static const wchar_t *WSTRING_FALSE = L"false";

constexpr static const char *const STRING_TRUE = "true";
constexpr static const char *const STRING_FALSE = "false";
constexpr static int MAX_STRING_LENGTH = 65536;
}  // namespace constants::String

#define D_STRUTIL_CHAR_LITERAL_SIZE(item) (sizeof item - 1)
#define D_SCHLSIZ(item) D_STRUTIL_CHAR_LITERAL_SIZE(item)

class str_util {
public:
    static void safe_strcpy(char *dest, size_t how_many_bytes, const char *src) {
#if defined(__STDC_LIB_EXT1__) || defined(WIN32)
        strcpy_s(dest, how_many_bytes, src);
#else
        strncpy(dest, src, how_many_bytes);
#endif
    }

    template <typename char_type>
    static bool is_empty_or_spaces(std::basic_string<char_type> string);

    /**
     * \brief User friendly wrapper around string compare. Similiar to starts_with
     * and ends_with function.
     *
     * \return true if to_find equals to string at specified location with specified
     * length.
     */
    static bool matches(const std::string &string, size_t offset,
                        const char *const to_find, const size_t to_find_size) {
        return !string.compare(offset, to_find_size, to_find);
    }

    static std::string boolToString(bool b);

    static std::string ltrim(const std::string &s);
    static std::string rtrim(const std::string &s);

    static std::string trim(const std::string &s);

    static void println(const std::string &text);

    static void println(const std::string &parameter, const std::string &description);

    static bool equalsCaseInsensitive(const std::string &string1,
                                      const std::string &string2);
    static bool equalsCaseInsensitive(const std::wstring &string1,
                                      const std::wstring &string2);

    static std::string toLowerCase(const std::string &s);
    static void lower_case_transform(std::string &s);
    static void upper_case_transform(std::string &s);

    static std::wstring toLowerCase(const std::wstring &s);
    static std::string toUpperCase(const std::string &s);

    static bool replace_one(std::string &str, const std::string &from,
                            const std::string &to, int from_pos = 0);
    static bool replace_one(std::wstring &str, const std::wstring &from,
                            const std::wstring &to, int from_pos = 0);

    /**
     * \brief Replace all occurences of from in string str. Transforms in place in attempt
     * to reduce number of memory allocations. Grows input str at most once.
     * \param str Input string to transform.
     * \param from token to search for
     * \param to replacement
     * \param to_size Number of characters in `to` to use for replacement. Can be
     * smaller than size of `to`. Note that if you provide this value, you should not
     * count terminating '\0' into the count. If you pass 0, the function will delete all
     * occurences of `from`.
     * \return reference to modified `str`.
     */
    static std::string &transform_replace_all(std::string &str, char from, const char *to,
                                              size_t to_size);

    // Calculates to_size as strlen(`to`) then delegates to other overload.
    // @see transform_replace_all(std::string, char, const char *, size_t)
    static std::string &transform_replace_all(std::string &str, char from,
                                              const char *to) {
        return transform_replace_all(str, from, to, strlen(to));
    }

    static std::string & transform_replace_all(std::string& str, const std::string& from,
                                               const std::string& to);

    /**
     * \brief Transform input, erase all occurences of \param character .
     * \return Reference to modified \param str .
     */
    static std::string &erase_all_of(std::string &str, char character);

    template <typename char_type>
    static std::vector<std::basic_string<char_type>> split(
        const std::basic_string<char_type> &str, char_type delimiter);

    template <typename char_type>
    static bool contains_case_insensitive(const std::basic_string<char_type> &string1,
                                          const std::basic_string<char_type> &string2);

    template <typename char_type>
    static bool contains(const std::basic_string<char_type> &string1,
                         const std::basic_string<char_type> &string2) {
        return string1.find(string2) != std::basic_string<char_type>::npos;
    }

    template <typename char_type>
    static bool contains(const std::basic_string<char_type> &string1,
                         const char_type *const string2) {
        return string1.find(string2) != std::basic_string<char_type>::npos;
    }

    //    static bool contains(const std::string &string1, const std::string &string2) {
    //    return string1.find(string2) != std::string::npos;
    //}

    static bool ends_with(const std::wstring &input, const std::wstring &ending);
    static bool starts_with(const std::wstring &input, const std::wstring &starting);

    static bool ends_with(const std::string &input, const std::string &ending);
    static bool starts_with(const std::string &input, const std::string &starting);

    /**
     * \brief "Optimized" version that does not call strlen.
     *
     * \tparam string_type Recomended: const std::string&, std::string_view, ...
     * \param[in] input input to search within.
     * \param[in] starting What to find.
     * \param[in] match_size Number of characters in `to_find` to try to match.
     * \return true if `input` starts with `starting`.
     */
    template <typename string_type = const std::string &>
    static bool starts_with(string_type input, string_type starting,
                            size_t const match_size) {
        return !input.compare(0, match_size, starting);
    }

    template <class char_type, class container_type>
    static std::basic_string<char_type> join(const container_type &elems,
                                             const char_type &delim);

    static std::vector<std::string> split(const std::string &str, char delimiter);

    /**
     * \brief Creates a vector of chars. Does not copy indicidual strings. Therfore,
     * original vector of strings must be alive and not changed for result of this
     * function to be valid. Use with caution.
     *
     * \param vector_of_strings Input to read from.
     * \return vector of char* pointers .. they point into input vector_of_strings.
     */
    static std::vector<char *> vector_of_strings_to_vector_of_chars(
        const std::vector<std::string> &vector_of_strings);

    template <typename charT>
    static bool ends_with(const std::basic_string<charT> &string, const charT *to_find,
                          size_t to_find_size);

    static std::string single_line(std::string_view input, int line_number);

    static std::vector<std::string_view> split_in_place(std::string_view strv,
                                                        std::string_view delims = " ");

    static std::pair<std::string_view, std::string_view> tokenize(std::string_view input,
                                                                  char delimiter = ' ');
    template <class iterator_type>
    static bool equals_one_of(std::string_view input, iterator_type begin,
                              iterator_type end);

    static std::string ensure_trailing(std::string &&item, char c);

private:
    template <typename charT, typename Tfunction>
    static std::basic_string<charT> &convert_case(std::basic_string<charT> &&s,
                                                  Tfunction f);

    template <typename charT, typename Tfunction>
    static void convert_case_in_place_impl(std::basic_string<charT> &s, Tfunction f);

    template <typename stringT>
    static bool replace_one_template(stringT &str, const stringT &from, const stringT &to,
                                     int from_pos = 0);

    template <typename charT>
    static bool startsWithTemplate(const std::basic_string<charT> &input,
                                   const std::basic_string<charT> &starting);

    template <typename charT>
    static bool endsWithTemplate(const std::basic_string<charT> &input,
                                 const std::basic_string<charT> &ending);
};
}  // namespace d_common

// template <typename char_type>
// std::vector<std::basic_string<char_type>> d_common::str_util::split(const
// std::basic_string<char_type> &str,
//                                                                    char_type delimiter)
//                                                                    {
//    std::vector<std::basic_string<char_type>> result;
//
//    std::basic_istringstream<char_type> ss(str);
//    std::basic_string<char_type> token;
//    while (getline(ss, token, delimiter)) {
//        result.push_back(token);
//    }
//
//    return result;
//}

template <typename char_type>
bool d_common::str_util::is_empty_or_spaces(std::basic_string<char_type> string) {
    return string.find_first_not_of(' ') == std::basic_string<char_type>::npos;
    ;
}

template <class char_type, class container_type>
std::basic_string<char_type> d_common::str_util::join(const container_type &elems,
                                                      const char_type &delim) {
    if (elems.empty()) {
        return {};
    }
    std::basic_stringstream<char_type> ss;
    auto e = elems.begin();
    ss << *e++;
    for (; e != elems.end(); ++e) {
        ss << delim << *e;
    }
    return ss.str();
}

template <typename char_type>
bool d_common::str_util::contains_case_insensitive(
    const std::basic_string<char_type> &string1,
    const std::basic_string<char_type> &string2) {
    return str_util::toLowerCase(string1).find(str_util::toLowerCase(string2)) !=
           std::basic_string<char_type>::npos;
}

template <class iterator_type>
bool d_common::str_util::equals_one_of(const std::string_view input, iterator_type begin,
                                       iterator_type end) {
    while (begin != end) {
        if (input == *(begin++))
            return true;
    }
    return false;
}