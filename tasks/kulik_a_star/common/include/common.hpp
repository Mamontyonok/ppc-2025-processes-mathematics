#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <tuple>
#include <vector>

#include "task/include/task.hpp"

namespace kulik_a_star {

using InType = std::tuple<int, int, std::vector<int>>; // src dest message
using OutType = std::vector<int>;
using TestType = std::tuple<int, int, std::vector<int>>;
using BaseTask = ppc::task::Task<InType, OutType>;

}  // namespace kulik_a_star
