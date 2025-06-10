#ifndef UTILITY_H
#define UTILITY_H

#include <memory>
#include <utility>
#include <mutex>

// Represents the Result of a function that may or may not be valid
template <typename T>
using Result = std::pair<bool, T>;

// Smart pointer and lock abbreviations
template <typename T>
using u_ptr = std::unique_ptr<T>;

template <typename T>
using s_ptr = std::shared_ptr<T>;

using u_lock = std::unique_lock<std::mutex>;

#endif
