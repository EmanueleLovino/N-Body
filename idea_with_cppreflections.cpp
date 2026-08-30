// #include <meta>
// #include <cassert>
// #include <vector>
// #include <utility>
//
// template <typename... Ts>
// using vector = std::vector<Ts...>;
//
// template <typename T>
// class soa_vector {
//     struct storage;                        // incomplete nested type
//
//     consteval {                            // <-- define_aggregate lives HERE
//         constexpr auto ctx = std::meta::access_context::unchecked();
//         std::vector<std::meta::info> members;
//         for (auto m : std::meta::nonstatic_data_members_of(^^T, ctx)) {
//             auto new_type = std::meta::substitute(^^vector, {std::meta::type_of(m)});
//             members.push_back(std::meta::data_member_spec(
//                 new_type, {.name = std::meta::identifier_of(m)}));
//         }
//         std::meta::define_aggregate(^^storage, members);
//     }
//
//     storage data;
//
//     static constexpr auto ctx = std::meta::access_context::unchecked();
//     static constexpr auto C = std::meta::nonstatic_data_members_of(^^T, ctx).size();
//     static constexpr auto src_members = std::define_static_array(
//         std::meta::nonstatic_data_members_of(^^T, ctx));
//     static constexpr auto dest_members = std::define_static_array(
//         std::meta::nonstatic_data_members_of(^^storage, ctx));
//
// public:
//     constexpr void push_back(const T& value) {
//         [&]<std::size_t... Is>(std::index_sequence<Is...>) {
//             ((data.[: dest_members[Is] :]).push_back(value.[: src_members[Is] :]), ...);
//         }(std::make_index_sequence<C>{});
//     }
//
//     [[nodiscard]] constexpr auto operator[](std::size_t index) const -> T {
//         return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
//             return T{ (data.[: dest_members[Is] :])[index]... };
//         }(std::make_index_sequence<C>{});
//     }
//
//     [[nodiscard]] constexpr std::size_t size() const {
//         if constexpr (C == 0) return 0;
//         else return (data.[: dest_members[0] :]).size();
//     }
// };
//
// int main() {
//     struct vec3_t { float x, y, z; };
//
//     soa_vector<vec3_t> v{};
//     v.push_back({.x = 1, .y = 2, .z = 3});
//     assert(1 == v[0].x and 2 == v[0].y and 3 == v[0].z);
// }
