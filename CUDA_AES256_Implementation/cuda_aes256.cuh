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

namespace cuda_aes {
	namespace datatype {
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
		template <typename T>
		class ThreadSafeDeque {
		public:
			ThreadSafeDeque();
			ThreadSafeDeque(uint64_t maxSize);
			void setMaxSize(uint64_t maxSize);	
			bool push_back(T& item);
			bool push_back(T&& item);
			bool push_front(T& item);
			bool push_front(T&& item);
			std::optional<T&> front(bool erase = false);
			std::optional<T&> back(bool erase = false);
			bool pop_back();
			bool pop_front();
			// possibly not thread-safe?? fix next time
			std::mutex& mtx();
			std::deque<T>& deque();
			uint64_t size();
			uint64_t maxSize();
			uint64_t remaining();
		private:
			void updateSize();
			uint64_t maxSize_;
			std::atomic<uint64_t> size_;
			std::deque<T> deque_;
			std::mutex mtx_;
		};

		template <typename T>
		class ThreadSafeVector {
		public:
			ThreadSafeVector(uint64_t maxSize);
			ThreadSafeVector();
			void setMaxSize(uint64_t maxSize);
			bool push_back(T&& item);
			bool push_back(T& item);
			bool pop_back();
			std::optional<T> access(uint64_t index);
			int64_t move(ThreadSafeVector& destination, bool explicitAll = true);
			// possibly not thread-safe?? fix next time
			std::mutex& mtx();
			std::vector<T>& vec();
			uint64_t size();
			uint64_t maxSize();
			uint64_t remaining();
		private:
			void updateSize();
			uint64_t maxSize_;
			std::atomic<uint64_t> size_;
			std::vector<T> vector_;
			std::mutex mtx_;

		};
		
		
	};
}
#include "threadSafeDeque.ipp"
#include "threadSafeVector.ipp"
namespace cuda_aes {
	namespace datatype {
		void convertToAESBlock(char* buf, uint64_t size, uint64_t&, datatype::ThreadSafeVector<datatype::cudaAESBlock_t>& blockBuffer, std::deque<char>& byteBuffer);
	}
}


namespace cuda_aes {
	
	class CUDA_AES_Manager;
	namespace control {
	}
	namespace cuda_kernel {
		class CUDA_AES_Kernel_Processor;
	};
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
				// Buffers
				std::deque<char> byteBuffer_;
				datatype::ThreadSafeVector<datatype::cudaAESBlock_t> block_buffer_;
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
		};
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