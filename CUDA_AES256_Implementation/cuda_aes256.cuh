#pragma once

#ifndef _CUDA_AES256_CUH
#define _CUDA_AES256_CUH
#define LOCK_GUARD(mutex_) std::lock_guard<std::mutex> lock_mutex(mutex_)
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

#include "threadSafeDeque.hpp"
#include "threadSafeVector.hpp"
#include "threadSafeSet.hpp"

namespace cuda_aes {
	namespace cuda_datatype {
		/*
		*	CONSTANTS
		*/
		const uint8_t maxAESBlockSize = 16;
		/*
		*	STRUCTS
		*/
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
		/*
		*	CLASSES
		*/
		
	};
}
#include "threadSafeDeque.ipp"
#include "threadSafeVector.ipp"
namespace cuda_aes {
	namespace cuda_datatype {
		void convertToAESBlock(char* buf, uint64_t size, uint64_t&, datatype::ThreadSafeVector<cuda_datatype::cudaAESBlock_t>& blockBuffer, std::deque<char>& byteBuffer);
	}
}


namespace cuda_aes {
	extern bool RUNTIME_ERRORS_HALTS_PROGRAM;
	class CUDA_AES_Processor;
	namespace control {}
	namespace file {
		class CUDA_AES_FileReader {
			public:
				CUDA_AES_FileReader();
				CUDA_AES_FileReader(const std::string& fileDirectory, uint64_t maxByteBufferSize, uint64_t maxBlockBufferSize);
				void start();
				void halt();
				void terminate();
				uint64_t size();
				bool openFailed();
				bool terminated();
				bool halted();
				uint64_t failedConversions();
			private:
				// Parent Manager
				std::shared_ptr<CUDA_AES_Processor> parent_;
				// Thread Task
				void task();
				// Buffers
				std::deque<char> byteBuffer_;
				datatype::ThreadSafeVector<cuda_datatype::cudaAESBlock_t> block_buffer_;
				uint64_t maxByteBufferSize_ = 0;
				uint64_t maxBlockBufferSize_ = 0;
				// Threads
				std::mutex mtx_;
				std::condition_variable cv_;
				std::thread readerThread_;
				// File
				std::fstream file_;
				uint64_t currentPositionInFile_ = 0;
				uint64_t currentBlockIndex_ = 0;
				uint64_t fileSize_ = 0;
				std::string fileDirectory_;		
				// Flags
				bool halted_ = false;
				bool terminated_ = false;
				bool fileDidNotOpen_ = false;
				uint64_t failedConversions_ = 0;
		};
		class CUDA_AES_FileWriter {
			public:
				CUDA_AES_FileWriter();
			private:
				// Parent Manager
				std::shared_ptr<CUDA_AES_Processor> parent_;
				// Thread
				std::thread writerThread;
		};
	};
	class CUDA_AES_Processor {
	public:
		CUDA_AES_Processor();
		// Attempt to cleanly break out/stop threads
		static void stopAll();
		// Strict, stops all operations immediately
		static void terminateAll();
	private:
		bool terminateFlag_ = false;
		file::CUDA_AES_FileReader reader_;
		file::CUDA_AES_FileWriter writer_;
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


#endif