// #pragma once
// #include <concepts>
// #include <iterator>
// #include <type_traits>
// #include <utility>
// #include <vector>
// #include <unordered_map>
// #include <functional>

// namespace mystd {

//     namespace {
//         template <typename container, typename T>
//         concept container_topsort = requires (container cont) {
//             cont.begin();
//             cont.end();

//             requires std::forward_iterator<std::remove_reference_t<decltype(cont.begin())>>;
//             requires std::same_as<T, std::remove_reference_t<decltype(*(cont.begin()))>>;
//             requires std::forward_iterator<std::remove_reference_t<decltype(cont.end())>>;
//             requires std::same_as<T, std::remove_reference_t<decltype(*(cont.end()))>>;
//         };
//     }



//     template<typename it, typename T = std::remove_reference_t<decltype(*((std::declval<it>())->begin()))>, typename container_output = std::vector<T>>
//     requires std::forward_iterator<it> && container_topsort<container_output, T> && 
//     container_topsort<std::remove_reference_t<decltype(*(std::declval<it>()))>, T> && requires (container_output cont_output) {
//         cont_output.push_back();
//     }
//     container_output topsort(it begin, it end, T start_graph) {
//         std::unordered_map<T, it> vertical; 
//         std::unordered_map<T, bool> colors;
//         for (auto iterator = begin; iterator < end; ++iterator, ++start_graph) {
//             vertical.insert(start_graph, iterator);
//             colors.insert(start_graph, false);
//         }

//         container_output result;
//         std::function<void(T, T)> dfs = [&](T vertex, T parent) {
//             for (auto node : vertical[vertex]) {
//                 if (node == parent || vertex == node) continue;
//                 if (colors[node]) continue;
//                 colors[node] = true;
//                 dfs(node, vertex);
//                 result.push_back(node);
//             }
//         };
//     }

// }
