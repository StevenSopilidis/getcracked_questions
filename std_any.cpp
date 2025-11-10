#include <variant>
#include <type_traits>
#include <typeinfo>

namespace getcracked {

    template <typename T>
    struct is_small_trait {
        constexpr static inline bool value = false;
    };

    template <>
    struct is_small_trait<int> {
        constexpr static inline bool value = true;
    };

    template <typename T>
    constexpr bool is_small_v = is_small_trait<T>::value;

    class any
    {
    public:
        any(const any&) = delete;
        any& operator=(const any&) = delete;
        any(any&&) = delete;
        any& operator=(any&&) = delete;

        any() noexcept = default;

        // Implement a constructor that takes in an object by copy.
        template<typename T>
        any(const T& value) : type_(&typeid(T)){
            if constexpr(is_small_v<T>)  {
                storage_ = value;
            } else {
                storage_ = new T(value);
            }
        }

        ~any() {
            if (std::holds_alternative<void*>(storage_)) {
                operator delete(std::get<void*>(storage_));
            }
        }

        template <typename T>
        const T& any_cast() const {
            if (&typeid(T) != type_)
                throw std::bad_cast();

            if constexpr(is_small_v<T>)
                return std::get<int>(storage_);
            else {
                return *reinterpret_cast<T*>(std::get<void*>(storage_));
            }
        }

    private:
        const std::type_info* type_;
        std::variant<void*, int> storage_; // for soo
    };
}