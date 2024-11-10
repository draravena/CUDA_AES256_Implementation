#include "cuda_aes256.cuh"

namespace cuda_aes {
	namespace system {
		uint64_t getFreeVRAM() {
			uint64_t free_t;
			cudaMemGetInfo(&free_t, NULL);
			return free_t;
		}
		uint64_t getTotalVRAM() {
			uint64_t total_t;
			cudaMemGetInfo(&total_t, NULL);
			return total_t;
		}
		uint64_t getFreeRAM() {
			MEMORYSTATUSEX status;
			status.dwLength = sizeof(status);
			GlobalMemoryStatusEx(&status);
			return status.ullAvailPhys;
		}
		uint64_t getTotalRAM() {
			MEMORYSTATUSEX status;
			status.dwLength = sizeof(status);
			GlobalMemoryStatusEx(&status);
			return status.ullTotalPhys;
		}

	};
};