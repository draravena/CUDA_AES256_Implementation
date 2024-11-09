#ifndef _CUDA_AES256_CUH
#define _CUDA_AES256_CUH

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <cstdint>
#include <vector>
#include <deque>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <bitset>

#define LOCK_GUARD(mutex_) std::lock_guard<std::mutex> lock_mutex(mutex_)


namespace cuda_aes {
	class CUDA_AES_Manager;
	namespace control {
	}
	namespace cuda_kernel {
		class CUDA_AES_Kernel_Processor;
	};
	namespace file {
		class CUDA_AES_File_Reader;
		class CUDA_AES_File_Writer;
	};
	namespace datatypes {
		class ThreadSafeVector;
		class ThreadSafeDeque;
		struct cudaAESBlock;
		struct cudaAES_Error_t;
	};
	namespace debug {
		uint64_t getFreeRAM();
		uint64_t getFreeVRAM();
	}
	namespace sBox {
		const uint8_t affineTransformationConstant = 0x63;
		const uint16_t irreduciblePolynomial = 0x11B;
	};


};



#endif