#include "cuda_aes256.cuh"

#include <iostream>

namespace cuda_aes {
	namespace file {
		CUDA_AES_FileReader::CUDA_AES_FileReader() {}
		CUDA_AES_FileReader::CUDA_AES_FileReader(const std::string& fileDirectory, uint64_t maxByteBufferSize, uint64_t maxBlockBufferSize) {
			fileDirectory_ = fileDirectory;
			maxByteBufferSize_ = maxByteBufferSize;
			maxBlockBufferSize_ = maxBlockBufferSize;
			if (maxByteBufferSize < 16) {
				maxByteBufferSize_ = 16;
			}
			if (maxBlockBufferSize == 0) {
				maxBlockBufferSize_ = 1;
			}
			block_buffer_.setMaxSize(maxBlockBufferSize);
			byteBuffer_.clear();
		}
		void CUDA_AES_FileReader::start() {
			//readerThread_ = std::thread([&] {
				file_ = std::fstream(fileDirectory_, std::ios::binary | std::ios::ate | std::ios::out | std::ios::in);	
				if (!file_.is_open()) {						// Check if file opened
					fileDidNotOpen_ = true;
					return;
				}
				fileDidNotOpen_ = false;
				fileSize_ = file_.tellg();
				file_.seekg(0);
				while (currentPositionInFile_ < fileSize_) {	
					// add pause if block buffer is full or byte buffer is full
					file_.seekg(currentPositionInFile_);
					uint64_t remaining = min(maxByteBufferSize_ - byteBuffer_.size(), fileSize_ - currentPositionInFile_);		// Remaining space in char buffer
					char* buffer = (char*)malloc(remaining);
					file_.read(buffer, remaining);
					currentPositionInFile_ += remaining;
					datatype::convertToAESBlock(buffer, remaining, currentBlockIndex_, block_buffer_, byteBuffer_);
				}
			//});
			//readerThread_.join();
		}	
		uint64_t CUDA_AES_FileReader::size() {
			return fileSize_;
		}
		bool CUDA_AES_FileReader::openFailed() {
			return fileDidNotOpen_;
		}
		bool CUDA_AES_FileReader::halted() {
			return halted_;
		}
		bool CUDA_AES_FileReader::terminated() {
			return terminated_;
		}
		uint64_t CUDA_AES_FileReader::failedConversions() {
			return failedConversions_;
		}
		// No definitions
		void CUDA_AES_FileReader::halt() {}
		void CUDA_AES_FileReader::terminate() {}
	};
};
