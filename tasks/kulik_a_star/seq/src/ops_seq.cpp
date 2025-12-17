#include "kulik_a_star/seq/include/ops_seq.hpp"

#include <chrono>
#include <cmath>
#include <thread>
#include <vector>

#include "kulik_a_star/common/include/common.hpp"

namespace kulik_a_star {

KulikAStarSEQ::KulikAStarSEQ(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  // auto source_rank = std::get<0>(in);
  // int proc_rank = 0;
  // MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  // if (proc_rank == source_rank) {
  GetInput() = in;
  //} else {
  // GetInput() = InType{};
  //}
}

bool KulikAStarSEQ::ValidationImpl() {
  // int proc_num = 0;
  // MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
  // int proc_rank = 0;
  // auto source_rank = std::get<0>(GetInput());
  // auto destination_rank = std::get<1>(GetInput());
  // MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  // if (proc_rank == source_rank) {
  //   bool fs = (source_rank >= 0);
  //   bool fd = (destination_rank >= 0);
  //   if (proc_num >= 3) {
  //     fs = fs && (source_rank < proc_num);
  //     fd = fd && (destination_rank < proc_num);
  //   }
  //   return (fs && fd);
  // }
  return true;
}

bool KulikAStarSEQ::PreProcessingImpl() {
  return true;
}

bool KulikAStarSEQ::RunImpl() {
  /*int proc_num = 0;
  int proc_rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  const auto &input = GetInput();
  auto source_rank = std::get<0>(GetInput());
  if (proc_num < 3) {
    GetOutput().resize(1u);
    GetOutput()[0] = INT_MAX;
  } else {
    auto destination_rank = std::get<1>(GetInput());
    size_t size = std::get<2>(input).size();
    int has_data = (size > 0) ? 1 : 0;
    std::vector<int> all_has_data(proc_num);
    MPI_Allgather(&has_data, 1, MPI_INT, all_has_data.data(), 1, MPI_INT, MPI_COMM_WORLD);
    int actual_source = -1;
    for (int i = 0; i < proc_num; i++) {
      if (all_has_data[i] == 1) {
        actual_source = i;
        break;
      }
    }
    MPI_Bcast(&source_rank, 1, MPI_INT, actual_source, MPI_COMM_WORLD);
    MPI_Bcast(&destination_rank, 1, MPI_INT, actual_source, MPI_COMM_WORLD);
    MPI_Bcast(&size, 1, MPI_UINT64_T, actual_source, MPI_COMM_WORLD);
    GetOutput().resize(size);
    std::vector<int> index(proc_num, 1);
    if (!index.empty()) {
      index[0] = proc_num - 1;
    }
    std::vector<int> edges(2 * (proc_num - 1), 0);
    for (int i = 0; i < proc_num - 1; i++) {
      edges[i] = i + 1;
    }
    MPI_Comm star;
    MPI_Graph_create(MPI_COMM_WORLD, proc_num, index.data(), edges.data(), 0, &star);
    if (source_rank == destination_rank) {
      if (proc_rank == source_rank) {
        GetOutput() = std::get<2>(input);
      }
      MPI_Bcast(GetOutput().data(), static_cast<int>(size), MPI_INT, destination_rank, star);
    } else {
      std::vector<int> buff;
      MPI_Status status;
      if (proc_rank == destination_rank) {
        buff.resize(size);
        MPI_Recv(buff.data(), static_cast<int>(size), MPI_INT, source_rank, MPI_ANY_TAG, star, &status);
      } else if (proc_rank == source_rank) {
        MPI_Send(std::get<2>(input).data(), static_cast<int>(size), MPI_INT, destination_rank, 0, star);
      }
      MPI_Bcast(GetOutput().data(), static_cast<int>(size), MPI_INT, destination_rank, star);
    }
    MPI_Comm_free(&star);
  }*/
  const auto &input = GetInput();
  GetOutput() = std::get<2>(input);
  std::this_thread::sleep_for(std::chrono::milliseconds(1));
  return true;
}

bool KulikAStarSEQ::PostProcessingImpl() {
  return true;
}

}  // namespace kulik_a_star
