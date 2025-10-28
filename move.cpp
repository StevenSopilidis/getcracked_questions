namespace getcracked {
    template <typename T>
    constexpr decltype(auto) move(T&& t) noexcept {
        return static_cast<std::remove_reference_t<T>&&>(t);
    }
}
