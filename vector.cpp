#include <cstddef>

namespace getcracked {
    template <typename Element>
    class vector {
    public:
        vector() : storage_(nullptr), capacity_(1), current_size_(0) {
            storage_ = static_cast<Element*>(::operator new[](capacity_ * sizeof(Element)));
        }

        ~vector() {
            clear();
            ::operator delete[](storage_);
        }

        void push_back(const Element& element) {
            if (current_size_ + 1 == capacity_)
                grow();
            new (storage_ + current_size_) Element(element);
            current_size_++;
        }

        void pop_back() {
            if (current_size_ == 0) return;
            current_size_--;
            storage_[current_size_].~Element();
        }

        const Element& at(std::size_t index) const {
            if (index >= current_size_)
                throw std::out_of_range("Invalid index");
            return storage_[index];
        }

        std::size_t get_size() const { return current_size_; }
        std::size_t get_capacity() const { return capacity_; }

        void shrink_to_fit() {
            if (current_size_ == capacity_) return;
            Element* new_storage = static_cast<Element*>(::operator new[](current_size_ * sizeof(Element)));
            for (std::size_t i = 0; i < current_size_; ++i) {
                new (new_storage + i) Element(std::move(storage_[i]));
                storage_[i].~Element();
            }
            ::operator delete[](storage_);
            storage_ = new_storage;
            capacity_ = current_size_;
        }

        void clear() {
            for (std::size_t i = 0; i < current_size_; ++i)
                storage_[i].~Element();
            current_size_ = 0;
        }

    private:
        void grow() {
            capacity_ *= 3;
            Element* new_storage = static_cast<Element*>(::operator new[](capacity_ * sizeof(Element)));
            for (std::size_t i = 0; i < current_size_; ++i) {
                new (new_storage + i) Element(std::move(storage_[i]));
                storage_[i].~Element();
            }
            ::operator delete[](storage_);
            storage_ = new_storage;
        }

        Element* storage_;
        std::size_t current_size_;
        std::size_t capacity_;
    };
}