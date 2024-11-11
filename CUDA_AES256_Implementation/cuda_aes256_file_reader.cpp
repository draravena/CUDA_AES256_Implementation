#include "cuda_aes256.cuh"

namespace cuda_aes {
	namespace file {
		CUDA_AES_FileReader::CUDA_AES_FileReader() {}
		CUDA_AES_FileReader::CUDA_AES_FileReader(const std::string& fileDirectory, uint64_t maxBufferSize) {
			fileDirectory_ = &fileDirectory;
			maxBufferSize_ = maxBufferSize;
			block_buffer_.setMaxSize(maxBufferSize);
		}
		void CUDA_AES_FileReader::start() {
			readerThread_ = std::thread([&] {
				file_ = std::fstream(*fileDirectory_, std::ios::binary | std::ios::ate | std::ios::out | std::ios::in);	
				if (!file_.is_open()) {						// Check if file opened
					fileDidNotOpen_ = true;
					return;
				}
				fileDidNotOpen_ = false;
				fileSize_ = file_.tellg();
				while (currentPositionInFile_ < fileSize_) {	
					std::unique_lock<std::mutex> lock(mtx_);			// Lock resources with mutex
					cv_.wait(lock, [&] {
						return true;
						});
					file_.seekg(currentPositionInFile_);					// Set pointer to current position in file
					uint64_t remaining = byteBuffer_.remaining();		// Remaining space in char buffer
					char* buf = new char[remaining];				
					file_.read(buf, remaining);
					currentPositionInFile_ += remaining;
					bool leftover = false;							
					//convertToAESBlock(buf, remaining, block_buffer_);										
				}
			});
		}	
		// No definitions
		void CUDA_AES_FileReader::halt() {}
		void CUDA_AES_FileReader::terminate() {}
	};
};
