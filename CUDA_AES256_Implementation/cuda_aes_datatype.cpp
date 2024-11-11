#include "cuda_aes256.cuh"

#include <iostream>

namespace cuda_aes {
	namespace datatype {
		void convertToAESBlock(char* buf, uint64_t size, uint64_t& blockIndex, ThreadSafeVector<cudaAESBlock_t>& blockBuffer, std::deque<char>& byteBuffer) {
			std::cout << size << std::endl;   //
			if (size < 16) {
				for (uint64_t i = 0; i < size; i++) {
					byteBuffer.push_back(buf[i]);
				}
				return;
			}
			for (uint64_t i = 0; i < size / 16; i++) {
				datatype::cudaAESBlock_t block;
				block.locationInFile = blockIndex;
				block.size = datatype::maxAESBlockSize;
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
				std::cout << blockBuffer.size() << std::endl; //
			}
			if (size % 16 != 0) {
				for (uint64_t i = 16 * (size / 16); i < size; i++) {
					byteBuffer.push_back(buf[i]);
				}
			}
			std::cout << byteBuffer.size() << std::endl; //
		}
	};
};


