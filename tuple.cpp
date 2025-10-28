#include <type_traits>
#include <utility>

namespace getcracked 
{
    // Implement tuple. This is a forward declaration.
    template <typename... Types>
    class tuple;

    // Base case
    template <>
    class tuple<> {
    public:
        tuple() = default;
    };

    // Recursive case
    template <typename Head, typename... Tail>
    class tuple<Head, Tail...> {
    public:
        Head head;
        tuple<Tail...> tail;

        tuple() : head(), tail() {}

        template <typename H, typename... T,
                  typename = std::enable_if_t<sizeof...(T) == sizeof...(Tail)>>
        tuple(H&& h, T&&... t)
            : head(std::forward<H>(h)), tail(std::forward<T>(t)...) {}
    };

    // get (const overload)
    template <unsigned N, typename... Types>
    decltype(auto) get(const tuple<Types...>& t) {
        static_assert(N < sizeof...(Types), "Index out of range");
        if constexpr (N == 0) {
            return (t.head); // return by reference
        } else {
            return get<N - 1>(t.tail);
        }
    }

    // get (non-const overload)
    template <unsigned N, typename... Types>
    decltype(auto) get(tuple<Types...>& t) {
        static_assert(N < sizeof...(Types), "Index out of range");
        if constexpr (N == 0) {
            return (t.head);
        } else {
            return get<N - 1>(t.tail);
        }
    }
}