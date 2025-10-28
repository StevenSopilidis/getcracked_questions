#include <cstddef>
#include <stdexcept>
#include <vector>
#include <atomic>

namespace getcracked 
{
    template <typename T>
    class SPSCQ
    {
    public:
        SPSCQ(size_t capacity) { 
            auto is_power_of_one =  capacity >= 2 && (capacity & (capacity - 1)) == 0;
            if (!is_power_of_one)
                throw std::runtime_error("Capacity must be power of 2");

            capacity_ = capacity;
            mask_ = capacity - 1;
            storage_.resize(capacity_);
            head_.store(0);
            tail_.store(0);
        }

        SPSCQ(const SPSCQ&) {
            throw std::runtime_error("Not copyable");
        };
        SPSCQ& operator=(const SPSCQ&) {
            throw std::runtime_error("Not copy assignable");
        }

        SPSCQ(SPSCQ&&) {
            throw std::runtime_error("Not movable");
        };
        SPSCQ& operator=(SPSCQ&&) {
            throw std::runtime_error("Not move assignable");
        }

        
        bool push(const T& item)
        {
            auto head = head_.load();
            auto next_head = (head + 1) & mask_;

            if (next_head == tail_.load())
                return false;

            storage_[head] = item;
            head_.store(next_head);
            return true;
        }

        bool pop(T& item)
        {
            auto tail = tail_.load();
            if (tail == head_.load())
                return false;

            item = storage_[tail];
            tail_.store((tail + 1) & mask_);
            return true;
        }

        [[nodiscard]] bool full() const
        {
            auto next_head = (head_.load() + 1) & mask_;
            return next_head == tail_.load();
        }

        [[nodiscard]] size_t size() const
        {
            auto head = head_.load();
            auto tail = tail_.load();
            return (head - tail) & mask_;
        }

        [[nodiscard]] bool empty() const
        {
            return head_.load() == tail_.load();
        }

    private:
        size_t capacity_;
        size_t mask_;
        std::vector<T> storage_;
        std::atomic<size_t> head_;
        std::atomic<size_t> tail_;
    };
}