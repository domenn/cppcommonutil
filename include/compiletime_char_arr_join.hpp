#pragma once

//#include <iostream>
//#include <string>
#include <utility>
#include <array>
#include <type_traits>
//#include <iostream>
// https://stackoverflow.com/questions/28708497/constexpr-to-concatenate-two-or-more-char-strings 
// Not rly workin ...
//namespace d_common::cpp_char {
//template <unsigned...>
//struct seq {
//    using type = seq;
//};
//template <unsigned N, unsigned... Is>
//struct gen_seq_x : gen_seq_x<N - 1, N - 1, Is...> {};
//template <unsigned... Is>
//struct gen_seq_x<0, Is...> : seq<Is...> {};
//template <unsigned N>
//using gen_seq = typename gen_seq_x<N>::type;
//
//template <size_t S>
//using size = std::integral_constant<size_t, S>;
//
//template <class T, size_t N>
//constexpr size<N> length(T const (&)[N]) {
//    return {};
//}
//template <class T, size_t N>
//constexpr size<N> length(std::array<T, N> const&) {
//    return {};
//}
//
//template <class T>
//using length_t = decltype(length(std::declval<T>()));
//
//constexpr size_t string_size() { return 0; }
//template <class... Ts>
//constexpr size_t string_size(size_t i, Ts... ts) {
//    return (i ? i - 1 : 0) + string_size(ts...);
//}
//template <class... Ts>
//using string_length = size<string_size(length_t<Ts>{}...)>;
//
//template <class... Ts>
//using combined_string = std::array<char, string_length<Ts...>{} + 1>;
//
//template <class Lhs, class Rhs, unsigned... I1, unsigned... I2>
//constexpr const combined_string<Lhs, Rhs> concat_impl(Lhs const& lhs, Rhs const& rhs,
//                                                      seq<I1...>, seq<I2...>) {
//    return {{lhs[I1]..., rhs[I2]..., '\0'}};
//}
//
//template <class Lhs, class Rhs>
//constexpr const combined_string<Lhs, Rhs> concat(Lhs const& lhs, Rhs const& rhs) {
//    return concat_impl(lhs, rhs, gen_seq<string_length<Lhs>{}>{},
//                       gen_seq<string_length<Rhs>{}>{});
//}
//
//template <class T0, class T1, class... Ts>
//constexpr const combined_string<T0, T1, Ts...> concat(T0 const& t0, T1 const& t1,
//                                                      Ts const&... ts) {
//    return concat(t0, concat(t1, ts...));
//}
//
//template <class T>
//constexpr const combined_string<T> concat(T const& t) {
//    return concat(t, "");
//}
//constexpr const combined_string<> concat() { return concat(""); }
//
//}  // namespace d_common::cpp_char
   // namespace d_common::cpp_char {
//
// template <unsigned...>
// struct seq {
//    using type = seq;
//};
// template <unsigned N, unsigned... Is>
// struct gen_seq_x : gen_seq_x<N - 1, N - 1, Is...> {};
// template <unsigned... Is>
// struct gen_seq_x<0, Is...> : seq<Is...> {};
// template <unsigned N>
// using gen_seq = typename gen_seq_x<N>::type;
//
// template <size_t S>
// using size = std::integral_constant<size_t, S>;
//
// template <class T, size_t N>
// constexpr size<N> length(T const (&)[N]) {
//    return {};
//}
// template <class T, size_t N>
// constexpr size<N> length(std::array<T, N> const&) {
//    return {};
//}
//
// template <class T>
// using length_t = decltype(length(std::declval<T>()));
//
// constexpr size_t string_size() { return 0; }
// template <class... Ts>
// constexpr size_t string_size(size_t i, Ts... ts) {
//    return (i ? i - 1 : 0) + string_size(ts...);
//}
// template <class... Ts>
// using string_length = size<string_size(length_t<Ts>{}...)>;
//
// template <class... Ts>
// using combined_string = std::array<char, string_length<Ts...>{} + 1>;
//
// template <class Lhs, class Rhs, unsigned... I1, unsigned... I2>
// constexpr const combined_string<Lhs, Rhs> concat_impl(Lhs const& lhs, Rhs const& rhs,
//                                                      seq<I1...>, seq<I2...>) {
//    return {{lhs[I1]..., rhs[I2]..., '\0'}};
//}
//
// template <class Lhs, class Rhs>
// constexpr const combined_string<Lhs, Rhs> concat(Lhs const& lhs, Rhs const& rhs) {
//    return concat_impl(lhs, rhs, gen_seq<string_length<Lhs>{}>{},
//                       gen_seq<string_length<Rhs>{}>{});
//}
//
// template <class T0, class T1, class... Ts>
// constexpr const combined_string<T0, T1, Ts...> concat(T0 const& t0, T1 const& t1,
//                                                      Ts const&... ts) {
//    return concat(t0, concat(t1, ts...));
//}
//
// template <class T>
// constexpr const combined_string<T> concat(T const& t) {
//    return concat(t, "");
//}
// constexpr const combined_string<> concat() { return concat(""); }
//
// template <typename A, typename B>
// constexpr auto test1(const A& a, const B& b) {
//    return concat(a, b);
//}
//}  // namespace d_common::cpp_char