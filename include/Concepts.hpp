#include <type_traits>

template <typename T>
concept static_array_t = std::is_trivially_copyable_v<T>;
