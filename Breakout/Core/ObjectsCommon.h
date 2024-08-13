#pragma once

#include <memory>

class GameObject;

template <typename T, std::enable_if_t<std::is_base_of_v<GameObject, T>, bool> = true>
using ObjectRef = std::shared_ptr<T>;
template <typename T, std::enable_if_t<std::is_base_of_v<GameObject, T>, bool> = true>
using ObjectHandle = std::weak_ptr<T>;