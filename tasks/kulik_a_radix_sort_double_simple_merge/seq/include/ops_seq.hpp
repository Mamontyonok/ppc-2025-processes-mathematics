#pragma once

#include "kulik_a_radix_sort_double_simple_merge/common/include/common.hpp"
#include "task/include/task.hpp"

namespace kulik_a_radix_sort_double_simple_merge {

class KulikARadixSortDoubleSimpleMergeSEQ : public BaseTask {
 public:
  static constexpr ppc::task::TypeOfTask GetStaticTypeOfTask() {
    return ppc::task::TypeOfTask::kSEQ;
  }
  explicit KulikARadixSortDoubleSimpleMergeSEQ(const InType &in);

 private:
  bool ValidationImpl() override;
  bool PreProcessingImpl() override;
  bool RunImpl() override;
  bool PostProcessingImpl() override;
};

}  // namespace kulik_a_radix_sort_double_simple_merge
