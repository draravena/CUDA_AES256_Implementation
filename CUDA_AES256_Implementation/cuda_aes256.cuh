#pragma once

#ifndef _CUDA_AES256_CUH
#define _CUDA_AES256_CUH

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include "Windows.h"

#include <optional>
#include <cstdint>
#include <vector>
#include <deque>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <bitset>
#include <fstream>

#define LOCK_GUARD(mutex_) std::lock_guard<std::mutex> lock_mutex(mutex_)


namespace cuda_aes {
	namespace datatype {
		const uint8_t maxAESBlockSize = 16;
		typedef struct cudaAESBlock_t {
			char bytes[4][4] = { 0 };
			uint64_t locationInFile = 0;
			uint8_t currentRound = 0;
			uint8_t currentStep = 0;
			uint8_t size = 0;
			bool readyForProcessing = true;
		} cudaAESBlock_t;
		typedef struct cudaAES_Error_t {
			cudaError_t cudaStatus;
			bool fileDidNotOpen = true;
		} cudaAES_Error_t;
		template <typename T>
		class ThreadSafeVector;
		template <typename T>
		class ThreadSafeDeque;	
	};
	class CUDA_AES_Manager;
	namespace control {
	}
	namespace cuda_kernel {
		class CUDA_AES_Kernel_Processor;
	};
	namespace file {
		class CUDA_AES_FileReader;
		class CUDA_AES_FileWriter;
	};
	namespace system {
		uint64_t getFreeRAM();
		uint64_t getFreeVRAM();
		uint64_t getTotalRAM();
		uint64_t getTotalVRAM();
	}
	namespace sBox {
		const uint8_t affineTransformationConstant = 0x63;
		const uint16_t irreduciblePolynomial = 0x11B;
	};


};

#include "threadSafeVector.ipp"
#include "threadSafeDeque.ipp"

#endif