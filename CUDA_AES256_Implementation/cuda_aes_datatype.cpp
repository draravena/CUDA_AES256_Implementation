#include "cuda_aes256.cuh"

namespace cuda_aes {
	namespace datatype {
		void convertToAESBlock(const char* buf, uint64_t size, uint64_t& blockIndex, ThreadSafeVector<cudaAESBlock_t>& blockBuffer, ThreadSafeDeque<char>& byteBuffer) {
			if (size < 16) {
				return;
			}
			for (uint64_t i = 0; i < size; i += 16) {
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
				blockBuffer.push_back(std::move(block));
			}
			if (size % 16 != 0) {
				for (uint64_t i = 16 * (size / 16); i < size % 16; i++) {
					byteBuffer.push_back(std::move((char)buf[i]));
				}
			}
		}
	};
};


