#include "kulik_a_star/mpi/include/ops_mpi.hpp"

#include <mpi.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

#include "kulik_a_star/common/include/common.hpp"

namespace kulik_a_star {

KulikAStarMPI::KulikAStarMPI(const InType &in) {
  SetTypeOfTask(GetStaticTypeOfTask());
  auto source_rank = std::get<0>(in);
  int proc_rank = 0;
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  if (proc_rank == source_rank) {
    GetInput() = in;
  } else {
    GetInput() = InType{};
  }
}

bool KulikAStarMPI::ValidationImpl() {
  int proc_num = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
  int proc_rank = 0;
  auto source_rank = std::get<0>(GetInput());
  auto destination_rank = std::get<1>(GetInput()); 
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  if (proc_rank == source_rank) {
    bool fs = (source_rank >= 0 && source_rank < proc_num);
    bool fd = (destination_rank >= 0 && destination_rank < proc_num);
    bool fn = (proc_num >= 3);
    return (fs && fd && fn);
  }
  return true;
}

bool KulikAStarMPI::PreProcessingImpl() {
  return true;
}

bool KulikAStarMPI::RunImpl() {
  int proc_num = 0;
  int proc_rank = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &proc_num);
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
  const auto &input = GetInput();
  auto source_rank = std::get<0>(input);
  auto destination_rank = std::get<1>(input);
  size_t size = std::get<2>(input).size();
  int has_data = (size > 0) ? 1 : 0;  
  std::vector<int> all_has_data(proc_num);
  MPI_Allgather(&has_data, 1, MPI_INT, 
                all_has_data.data(), 1, MPI_INT, MPI_COMM_WORLD);
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
  if (source_rank == destination_rank) {
    if (proc_rank == source_rank) {
      GetOutput() = std::get<2>(input);
    }
    MPI_Bcast(GetOutput().data(), static_cast<int>(size), MPI_INT, destination_rank, MPI_COMM_WORLD);
  }
  else {
    std::vector<int> buff;
    MPI_Status status;
    
    if (proc_rank == 0) {
      buff.resize(size);
      if (source_rank != 0) {
        MPI_Recv(buff.data(), static_cast<int>(size), MPI_INT, source_rank, 0, MPI_COMM_WORLD, &status);
        if (destination_rank != 0) {
          MPI_Send(buff.data(), static_cast<int>(size), MPI_INT, destination_rank, 0, MPI_COMM_WORLD);
        }
        else {
          GetOutput() = buff;
        }
      }
      else {
        MPI_Send(std::get<2>(input).data(), static_cast<int>(size), MPI_INT, destination_rank, 0, MPI_COMM_WORLD);
      }
    }
    else if (proc_rank == destination_rank) {
      buff.resize(size);
      if (destination_rank != 0) {
        MPI_Recv(buff.data(), static_cast<int>(size), MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
      }
      GetOutput() = buff;
    }
    else if (proc_rank == source_rank && source_rank != 0) {
      MPI_Send(std::get<2>(input).data(), static_cast<int>(size), MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Bcast(GetOutput().data(), size, MPI_INT, destination_rank, MPI_COMM_WORLD);
  }

  return true;
}

bool KulikAStarMPI::PostProcessingImpl() {
  return true;
}

}  // namespace kulik_a_star
