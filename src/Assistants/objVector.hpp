/*
	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

template<typename Base>
class objVector {
    using VecPtr = std::vector<std::unique_ptr<Base>>;
    VecPtr vec;

public:
    std::size_t size() const { return vec.size(); }
    typename VecPtr::iterator begin() { return vec.begin(); }
    typename VecPtr::iterator end()   { return vec.end(); }

          Base& operator[](const std::size_t idx)       { return *vec[idx]; }
    const Base& operator[](const std::size_t idx) const { return *vec[idx]; }

    template<typename Derived, typename... Args>
        requires(std::constructible_from<Derived, Args...> && std::is_base_of_v<Base, Derived>)
    Derived& createEntry(Args&&... args) {
        return static_cast<Derived&>(*vec.emplace_back(
            std::make_unique<Derived>(std::forward<Args>(args)...)
        ));
    }

    template<typename Derived>
        requires(std::is_base_of_v<Base, Derived>)
    Derived& insertEntry(Derived&& obj) {
        return static_cast<Derived&>(*vec.emplace_back(
            std::make_unique<Derived>(std::move(obj))
        ));
    }

    void deleteEntry(const std::size_t);
    void deleteEntry(typename VecPtr::iterator);

    void pushEntry(const std::size_t, objVector<Base>&, const std::size_t);
    void pushEntry(const std::size_t, objVector<Base>&);

    void pullEntry(const std::size_t, objVector<Base>&, const std::size_t);
    void pullEntry(const std::size_t, objVector<Base>&);
};

/*------------------------------------------------------------------*/
/*  Member function definitions below                               */
/*------------------------------------------------------------------*/

template<typename Base>
void objVector<Base>::deleteEntry(const std::size_t idx) {
    assert(idx >= 0 && idx < vec.size() && "index out of range");
    vec.erase(vec.begin() + idx);
}
template<typename Base>
void objVector<Base>::deleteEntry(typename VecPtr::iterator it) {
    vec.erase(it);
}

template<typename Base>
void objVector<Base>::pushEntry(const std::size_t src_idx, objVector<Base>& dst, const std::size_t dst_idx) {
    assert(src_idx < vec.size() && "source index out of range");
    assert(dst_idx < dst.size() && "dest index out of range");

    const auto idx{ dst.begin() + dst_idx };
    dst.vec.emplace(idx, std::move(vec[src_idx]));
    vec.erase(vec.begin() + src_idx);
}
template<typename Base>
void objVector<Base>::pushEntry(const std::size_t src_idx, objVector<Base>& dst) {
    assert(src_idx < vec.size() && "source index out of range");

    dst.vec.emplace_back(std::move(vec[src_idx]));
    vec.erase(vec.begin() + src_idx);
}

template<typename Base>
void objVector<Base>::pullEntry(const std::size_t src_idx, objVector<Base>& src, const std::size_t dst_idx) {
    assert(src_idx < src.size() && "source index out of range");
    assert(dst_idx < vec.size() && "dest index out of range");

    const auto idx{ vec.begin() + dst_idx };
    vec.emplace(idx, std::move(src.vec[src_idx]));
    src.vec.erase(src.begin() + src_idx);
}
template<typename Base>
void objVector<Base>::pullEntry(const std::size_t src_idx, objVector<Base>& src) {
    assert(src_idx < src.size() && "source index out of range");

    vec.emplace_back(std::move(src.vec[src_idx]));
    src.vec.erase(src.begin() + src_idx);
}
