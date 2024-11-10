#include "cuda_aes256.cuh"

namespace cuda_aes {
	namespace file {
		class CUDA_AES_FileReader {
		public:
			CUDA_AES_FileReader() {}
			CUDA_AES_FileReader(const std::string& fileDirectory, uint64_t maxBufferSize) {
				fileDirectory_ = &fileDirectory;
				maxBufferSize_ = maxBufferSize;
				block_buffer_.setMaxSize(maxBufferSize);
			}
			void start() {
				readerThread_ = std::thread([&] {
					file_ = std::fstream(*fileDirectory_, std::ios::binary | std::ios::ate | std::ios::out | std::ios::in);	
					if (!file_.is_open()) {						// Check if file opened
						fileDidNotOpen = true;
						return;
					}
					fileDidNotOpen = false;
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
						convertToAESBlock(buf, remaining, block_buffer_);										
					}
					});
			}
			void halt();
			void terminate();
			void convertToAESBlock(const char* buf, uint64_t size, datatype::ThreadSafeVector<datatype::cudaAESBlock_t>& blockBuffer) {
				if (size < 16) {
					return;
				}
				for (int64_t i = 0; i < size; i += 16) {
					datatype::cudaAESBlock_t block;
					block.locationInFile = currentBlocksIndex_;
					block.size = datatype::maxAESBlockSize;
					currentBlocksIndex_++;
					uint8_t row = 0, col = 0;
					for (int64_t j = 0; j < 16; j++) {
						block.bytes[row][col] = buf[i + j];
						row++;
						if (row == 4) {
							row = 0;
							col++;
						}
					}
					blockBuffer.push_back(std::move(block));
				}
				if (size % 16 != 0) {
					for (uint64_t i = 16 * (size / 16); i < size % 16; i++) {
						byteBuffer_.push_back(std::move((char)buf[i]));
					}
				}
			}
			bool fileDidNotOpen = true;
		private:
			// Buffers
			datatype::ThreadSafeDeque<char> byteBuffer_;
			datatype::ThreadSafeVector<datatype::cudaAESBlock_t> block_buffer_;
			uint64_t maxBufferSize_ = 0;
			// Threads
			std::mutex mtx_;
			std::condition_variable cv_;
			std::thread readerThread_;
			// File
			std::fstream file_;
			uint64_t currentPositionInFile_ = 0;
			uint64_t currentBlocksIndex_ = 0;
			uint64_t fileSize_ = 0;
			const std::string* fileDirectory_;
			// Flags
			bool halted = false;
			bool terminated = false;
			
			
			
			
		};
	};
};