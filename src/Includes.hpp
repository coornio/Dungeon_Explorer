/*
    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <filesystem>
#include <format>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

using namespace std::string_literals;
using namespace std::string_view_literals;

using u64 = std::uint64_t;
using s64 = std::int64_t;
using u32 = std::uint32_t;
using s32 = std::int32_t;
using u16 = std::uint16_t;
using s16 = std::int16_t;
using u8  = std::uint8_t;
using s8  = std::int8_t;

template <typename R, typename T>
#ifdef _MSC_VER
[[msvc::forceinline]]
#else
[[gnu::always_inline]]
#endif
constexpr R as(T&& t) {
    return static_cast<R>(std::forward<T>(t));
}
template <typename R, typename T>
#ifdef _MSC_VER
[[msvc::forceinline]]
#else
[[gnu::always_inline]]
#endif
constexpr R to(T&& t) {
    return reinterpret_cast<R>(std::forward<T>(t));
}

template <typename T, std::size_t X, std::size_t Y = X>
using arr2D = std::array<std::array<T, X>, Y>;

#include "Assistants/FrameLimiter.hpp"
#include "Assistants/BasicInput.hpp"
#include "Assistants/Well512.hpp"
#include "Assistants/objVector.hpp"
#include "Assistants/AwfulPrinter.hpp"

#include "RenderSettings.hpp"
