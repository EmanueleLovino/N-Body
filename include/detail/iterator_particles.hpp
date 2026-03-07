#pragma once
#include <cstddef>
#include <iterator>

#include "detail/particle_view.hpp"

namespace nbody::detail {

template <typename Storage>
struct Iterator_particles {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = ParticleView<typename Storage::value_type>;
    using pointer = void;
    using reference = value_type;
    using size_type = std::size_t;

    explicit Iterator_particles(Storage* storage, size_type index)
        : storage_(storage), i(index) {}

    // dereference
    auto operator*() { return storage_->view(i); }
    auto operator[](difference_type n) const { return storage_->view(i + n); }

    // increment / decrement
    Iterator_particles& operator++() noexcept {
        ++i;
        return *this;
    }
    Iterator_particles operator++(int) {
        auto tmp = *this;
        ++i;
        return tmp;
    }
    Iterator_particles& operator--() noexcept {
        --i;
        return *this;
    }
    Iterator_particles operator--(int) {
        auto tmp = *this;
        --i;
        return tmp;
    }

    // arithmetic
    Iterator_particles& operator+=(difference_type n) {
        i += n;
        return *this;
    }
    Iterator_particles& operator-=(difference_type n) {
        i -= n;
        return *this;
    }
    Iterator_particles operator+(difference_type n) const {
        return Iterator_particles{storage_, i + n};
    }
    Iterator_particles operator-(difference_type n) const {
        return Iterator_particles{storage_, i - n};
    }
    difference_type operator-(const Iterator_particles& other) const {
        return i - other.i;
    }

    // comparison
    bool operator==(const Iterator_particles& other) const noexcept {
        return i == other.i;
    }
    bool operator!=(const Iterator_particles& other) const noexcept {
        return i != other.i;
    }
    bool operator<(const Iterator_particles& other) const noexcept {
        return i < other.i;
    }
    bool operator>(const Iterator_particles& other) const noexcept {
        return i > other.i;
    }
    bool operator<=(const Iterator_particles& other) const noexcept {
        return i <= other.i;
    }
    bool operator>=(const Iterator_particles& other) const noexcept {
        return i >= other.i;
    }

   private:
    Storage* storage_{nullptr};
    size_type i{};
};

// non-member operator+ for n + it
template <typename Storage>
Iterator_particles<Storage> operator+(
    typename Iterator_particles<Storage>::difference_type n,
    const Iterator_particles<Storage>& it) {
    return it + n;
}

}  // namespace nbody::detail
