
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "cuda_aes256.cuh"
#include <iostream>

using namespace cuda_aes;
using namespace cuda_aes::file;

cuda_aes::file::CUDA_AES_FileReader reader;

int main() {
	std::string txt = "test.txt";
	
	return 0;
}
