#include <stdexcept>

namespace getcracked 
{
    template <typename T>
    struct custom_deleter
    {
        void operator()(T* pointer) const
        {
            delete pointer;
        }
    };

    template <typename T, typename custom_deleter = custom_deleter<T>>
    class unique_ptr
    {
    public:
        unique_ptr() : ptr_{nullptr} 
        { 
        }

        unique_ptr(T* pointer)
        {
            ptr_ = pointer;
        }

        unique_ptr(const unique_ptr&) { 
            throw std::runtime_error("unique_ptr not copyable");
        };

        unique_ptr& operator=(const unique_ptr&) { 
            throw std::runtime_error("unique_ptr not copyable");
        };

        unique_ptr(unique_ptr&& other) noexcept
            : ptr_(other.ptr_)
        {
            other.ptr_ = nullptr;
        }

        unique_ptr& operator=(unique_ptr&& other) noexcept
        {
            if (this != other) {
                custom_deleter{}(ptr_);
                ptr_ = other.ptr_;
                other.ptr_ = nullptr;
            }

            return *this;
        }

        ~unique_ptr()
        {
            custom_deleter{}(ptr_);
            ptr_ = nullptr;
        }

        T* release()
        {
            auto tmp = ptr_;
            ptr_ = nullptr;
            return tmp;
        }

        void reset(T* pointer)
        {
            custom_deleter{}(ptr_);
            ptr_ = pointer;
        }

        bool is_owning() const { 
            return ptr_ != nullptr;
        }


        T& operator*() const {
            return *ptr_;
        }

        T* operator->() const {
            return ptr_;
        }

        operator bool() const { 
            return ptr_ != nullptr;
        }

    private:
        T* ptr_;
    };
}