#ifndef COMMON_HPP_
#define COMMON_HPP_

#include <type_traits>

namespace hlp {

#define TO_STR_(...) #__VA_ARGS__
#define TO_STR(...) TO_STR_(__VA_ARGS__)

template <typename EnumT, typename T = std::underlying_type_t<EnumT> >
constexpr inline EnumT ToEnum(T value) {
    return static_cast<EnumT>(value);
}

template <typename EnumT, typename T = std::underlying_type_t<EnumT> >
constexpr inline T FromEnum(EnumT enum_value) {
    return static_cast<T>(enum_value);
}

} // namespace hlp
 
#endif // COMMON_HPP_
