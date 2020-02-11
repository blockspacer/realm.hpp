#pragma once

#include <cstddef>

// todo: implement a fallback id generation
#if defined(__GNUC__)
#define __VALID_PRETTY_FUNC__ __PRETTY_FUNCTION__
#elif defined(__clang__)
#define __VALID_PRETTY_FUNC__ __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define __VALID_PRETTY_FUNC__ __FUNCSIG__
#else
#error "Compiler not supported"
#endif

namespace realm {
namespace detail {

using hash_t = uint64_t;

constexpr hash_t fnv_basis = 14695981039346656037ull;
constexpr hash_t fnv_prime = 1099511628211ull;

/**
 * @brief https://notes.underscorediscovery.com/constexpr-fnv1a/
 * @param str
 * @param value
 * @return
 */
inline constexpr hash_t
hash_fnv1a(const char* const str, const hash_t value = fnv_basis) noexcept
{
    return (str[0] == '\0') ? value
                            : hash_fnv1a(&str[1], (value ^ hash_t(str[0])) * fnv_prime);
}

template<typename T>
struct type_meta
{
private:
    // TODO: only use the name of the class when generating hash
    static constexpr auto get_hash() noexcept
    {
        return detail::hash_fnv1a(__VALID_PRETTY_FUNC__);
    }

public:
    using hash_type = detail::hash_t;
    static constexpr hash_type hash{ get_hash() };
};

} // namespace detail
} // namespace realm
