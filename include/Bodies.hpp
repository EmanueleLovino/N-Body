#pragma once

#include <cstddef>
#include <vector>

#include "Concepts.hpp"

namespace nbody {

namespace detail {

template <template <typename...> class Container, Scalar T>
struct SoAData {
    Container<T> qx_, qy_, qz_;

    Container<T> vx_, vy_, vz_;

    Container<T> ax_, ay_, az_;

    Container<T> m_;
    Container<T> r_;

    T& qx(std::size_t i) { return qx_[i]; }
    T& qy(std::size_t i) { return qy_[i]; }
    T& qz(std::size_t i) { return qz_[i]; }
    T& vx(std::size_t i) { return vx_[i]; }
    T& vy(std::size_t i) { return vy_[i]; }
    T& vz(std::size_t i) { return vz_[i]; }
    T& ax(std::size_t i) { return ax_[i]; }
    T& ay(std::size_t i) { return ay_[i]; }
    T& az(std::size_t i) { return az_[i]; }
    T& m(std::size_t i) { return m_[i]; }
    T& r(std::size_t i) { return r_[i]; }

    void reserve(std::size_t n) {
        qx_.reserve(n), qy_.reserve(n), qz_.reserve(n);
        vx_.reserve(n), vy_.reserve(n), vz_.reserve(n);
        ax_.reserve(n), ay_.reserve(n), az_.reserve(n);
        m_.reserve(n), r_.reserve(n);
    }

    void addParticle(T qx, T qy, T qz, T vx, T vy, T vz, T ax, T ay, T az, T m,
                     T r) {
        qx_.push_back(qx), qy_.push_back(qy), qz_.push_back(qz);
        vx_.push_back(vx), vy_.push_back(vy), vz_.push_back(vz);
        ax_.push_back(ax), ay_.push_back(ay), az_.push_back(az);
        m_.push_back(m), r_.push_back(r);
    }

    std::size_t size() const { return qx_.size(); }
};
template <template <typename...> class Container, Scalar T>
struct AoSData {
   private:
    struct ParticleData {
        T qx_, qy_, qz_;

        T vx_, vy_, vz_;

        T ax_, ay_, az_;

        T m_;
        T r_;
    };

   public:
    Container<ParticleData> data_;

    T& qx(std::size_t i) { return data_[i].qx_; }
    T& qy(std::size_t i) { return data_[i].qy_; }
    T& qz(std::size_t i) { return data_[i].qz_; }
    T& vx(std::size_t i) { return data_[i].vx_; }
    T& vy(std::size_t i) { return data_[i].vy_; }
    T& vz(std::size_t i) { return data_[i].vz_; }
    T& ax(std::size_t i) { return data_[i].ax_; }
    T& ay(std::size_t i) { return data_[i].ay_; }
    T& az(std::size_t i) { return data_[i].az_; }
    T& m(std::size_t i) { return data_[i].m_; }
    T& r(std::size_t i) { return data_[i].r_; }

    void reserve(std::size_t n) { data_.reserve(n); }

    void addParticle(T qx, T qy, T qz, T vx, T vy, T vz, T ax, T ay, T az, T m,
                     T r) {
        data_.push_back({qx, qy, qz, vx, vy, vz, ax, ay, az, m, r});
    }
    std::size_t size() const { return data_.size(); }
};

}  // namespace detail

/*
 * Type traits for compile time dispatching based on the memory layout
 */
struct AoSTag {};
struct SoATag {};

template <typename Tag, template <typename...> class Container, Scalar T>
struct StorageTraits {};

/*
 * Partial specializations for the different tags
 */

template <template <typename...> class Container, Scalar T>
struct StorageTraits<SoATag, Container, T> {
    using type = nbody::detail::SoAData<Container, T>;
};

template <template <typename...> class Container, Scalar T>
struct StorageTraits<AoSTag, Container, T> {
    using type = nbody::detail::AoSData<Container, T>;
};

template <template <typename...> class Container, Scalar T, typename layoutTag>
using Bodies = typename StorageTraits<layoutTag, Container, T>::type;

}  // namespace nbody
   // namespace nbody
