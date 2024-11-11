
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "cuda_aes256.cuh"
#include <iostream>

using namespace std;
using namespace cuda_aes;
using namespace cuda_aes::file;

int main() {
	std::string txt = "test.txt";
	CUDA_AES_FileReader reader(txt, 10, 10);
	reader.start();
	return 0;
}
