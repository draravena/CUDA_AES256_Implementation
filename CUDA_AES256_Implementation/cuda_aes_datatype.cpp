#include "cuda_aes256.cuh"

#include <iostream>



namespace cuda_aes {
	namespace cuda_datatype {
		void convertToAESBlock(char* buf, uint64_t size, uint64_t& blockIndex, datatype::ThreadSafeVector<cudaAESBlock_t>& blockBuffer, std::deque<char>& byteBuffer) {
			if (size < 16) {
				for (uint64_t i = 0; i < size; i++) {
					byteBuffer.push_back(buf[i]);
				}
				return;
			}
			for (uint64_t i = 0; i < size / 16; i++) {
				cudaAESBlock_t block;
				block.locationInFile = blockIndex;
				block.size = maxAESBlockSize;
				blockIndex++;
				uint8_t row = 0, col = 0;
				for (int64_t j = 0; j < 16; j++) {
					block.bytes[row][col] = buf[i + j];
					row++;
					if (row == 4) {
						row = 0;
						col++;
					}
				}
				blockBuffer.push_back(block);
			}
			if (size % 16 != 0) {
				for (uint64_t i = 16 * (size / 16); i < size; i++) {
					byteBuffer.push_back(buf[i]);
				}
			}
		}
	};
};


