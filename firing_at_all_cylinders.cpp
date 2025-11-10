#include <atomic>
#include <type_traits>
#include <cstddef>
#include <any>

namespace getcracked
{
    template <typename Element>
    class InstantWriteMultipleRead
    {
    static_assert(std::is_copy_constructible<Element>::value, "Type must be copy constructible");
    static_assert(std::is_default_constructible<Element>::value, "Type must be default constructible");

    public:
        InstantWriteMultipleRead() : seq_(0), data_() {}

        void Write(const Element& value)
        {
            auto s = seq_.load();

            seq_.store(s + 1); // odd --> write in progress

            data_ = value;

            seq_.store(s + 2); // even --> write was complete
        }
        
        bool Read(Element& out) const
        {
            for (;;) {
                auto before = seq_.load();

                if (before & 1)
                    // odd write not finished
                    continue;

                out = data_;

                auto after = seq_.load();

                // check if seq was written again
                if (after == before && !(after & 1))
                    return after != 0; // not empty
            }
        }

    private:
        alignas(64) mutable std::atomic<std::size_t> seq_;
        alignas(64) Element data_;
    };
}