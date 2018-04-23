#ifndef PARALUTION_BACKEND_MANAGER_HPP_
#define PARALUTION_BACKEND_MANAGER_HPP_

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>

namespace paralution {

template <typename ValueType>
class AcceleratorVector;
template <typename ValueType>
class AcceleratorMatrix;
template <typename ValueType>
class HostMatrix;

/// Backend descriptor - keeps information about the
/// hardware - OpenMP (threads); HIP (blocksizes, handles, etc);
struct Paralution_Backend_Descriptor {

  // set by initbackend();
  bool init;

  // current backend
  int backend;
  bool accelerator;
  bool disable_accelerator;

  // OpenMP threads
  int OpenMP_threads;
  // OpenMP threads before PARALUTION init
  int OpenMP_def_threads;
  // OpenMP nested before PARALUTION init
  int OpenMP_def_nested;
  // Host affinity (true-yes/false-no)
  bool OpenMP_affinity;
  // Host threshold size
  int OpenMP_threshold;

  // HIP section
  // handles
  // rocblasHandle_t casted in void **
  void *HIP_rocblas_handle;
  // rocsparseHandle_t casted in void **
  void *HIP_rocsparse_handle;

  int HIP_dev;
  int HIP_warp;
  int HIP_block_size;
  int HIP_max_threads;
  int HIP_num_procs;
  int HIP_threads_per_proc;

  // MPI rank/id
  int rank;

  std::ofstream *log_file;

};
  
/// Global backend descriptor
extern struct Paralution_Backend_Descriptor _Backend_Descriptor;

/// Host name
extern const std::string _paralution_host_name [1];

/// Backend names
extern const std::string _paralution_backend_name [2];

/// Backend IDs
enum _paralution_backend_id {None=0,
                             HIP=1};



/// Initialization of the paralution platform
int init_paralution(const int rank = -1, const int dev_per_node = 1, const int platform = 0);
/// Shutdown the paralution platform
int stop_paralution(void);

/// Select a device
int set_device_paralution(int dev);

/// Set the number of threads in the platform
void set_omp_threads_paralution(const int nthreads);

/// Set host affinity (true-on/false-off)
void set_omp_affinity(bool affinity);

/// Set a specific HIP device
void set_hip_gpu_paralution(const int ngpu);

/// Set OpenMP threshold size
void set_omp_threshold(const int threshold);

/// Print information about the platform
void info_paralution(void);

/// Print information about the platform via specific backend descriptor
void info_paralution(const struct Paralution_Backend_Descriptor backend_descriptor);

/// Return true if any accelerator is available
bool _paralution_available_accelerator(void);

/// Disable/Enable the accelerator
void disable_accelerator_paralution(const bool onoff=true);

/// Return backend descriptor
struct Paralution_Backend_Descriptor *_get_backend_descriptor(void);

/// Set backend descriptor
void _set_backend_descriptor(const struct Paralution_Backend_Descriptor backend_descriptor);

/// Set the OMP threads based on the size threshold
void _set_omp_backend_threads(const struct Paralution_Backend_Descriptor backend_descriptor,
                              const int size);

/// Build (and return) a vector on the selected in the descriptor accelerator
template <typename ValueType>
AcceleratorVector<ValueType>* _paralution_init_base_backend_vector(const struct Paralution_Backend_Descriptor backend_descriptor);

/// Build (and return) a matrix on the host
template <typename ValueType>
HostMatrix<ValueType>* _paralution_init_base_host_matrix(const struct Paralution_Backend_Descriptor backend_descriptor,
                                                         const unsigned int matrix_format);

/// Build (and return) a matrix on the selected in the descriptor accelerator
template <typename ValueType>
AcceleratorMatrix<ValueType>* _paralution_init_base_backend_matrix(const struct Paralution_Backend_Descriptor backend_descriptor,
                                                                   const unsigned int matrix_format);

/// Sync the active async transfers
void _paralution_sync(void);

size_t _paralution_add_obj(class ParalutionObj* ptr);
bool _paralution_del_obj(class ParalutionObj* ptr,
                         size_t id);
void _paralution_delete_all_obj(void);
bool _paralution_check_if_any_obj(void);

}

#endif // PARALUTION_BACKEND_MANAGER_HPP_
