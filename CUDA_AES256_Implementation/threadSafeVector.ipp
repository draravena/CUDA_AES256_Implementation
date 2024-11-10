#pragma once
#include "cuda_aes256.cuh"

namespace cuda_aes {
	namespace datatype {
		template <typename T>
		class ThreadSafeVector {
		public:
			ThreadSafeVector(uint64_t maxSize) {
				maxSize_ = maxSize;
				vector_.reserve(maxSize);
				vector_.clear();
				updateSize();
			}
			ThreadSafeVector() {}
			void setMaxSize(uint64_t maxSize) {
				maxSize_ = maxSize;
				vector_.reserve(maxSize);
				vector_.clear();
				updateSize();
			}
			bool push_back(T&& item) {
				LOCK_GUARD(mtx_);
				if (vector_.size() >= maxSize_) {
					return false;
				}
				vector_.push_back(std::move(item));
				updateSize();
				return true;
			}
			bool pop_back() {
				LOCK_GUARD(mtx_);
				if (vector_.empty()) {
					return false;
				}
				vector_.pop_back();
				updateSize();
				return true;
			}
			std::optional<T> access(uint64_t index) {
				LOCK_GUARD(mtx_);
				if (index >= vector_.size()) {
					return std::nullopt;
				}
				else {
					return vector_[index];
				}
			}
			int64_t move(ThreadSafeVector& destination, bool explicitAll = true) {
				std::lock_guard<std::mutex> lock1(destination.mtx());
				std::lock_guard<std::mutex> lock2(mtx_);
				uint64_t dest_remaining = destination.remaining();
				if (dest_remaining < vector_.size() && explicitAll == true) {
					return -1;
				}
				else if (dest_remaining < vector_.size() && explicitAll == false) {
					destination.vec().insert(destination.vec().end(), vector_.begin(), vector_.begin() + dest_remaining);
					vector_.erase(vector_.begin(), vector_.begin() + dest_remaining);
					destination.updateSize();
					updateSize();
					return 0;
				}
				destination.vec().insert(destination.vec().end(), vector_.begin(), vector_.end());
				vector_.clear();
				destination.updateSize();
				updateSize();
				return 1;
			}
			// possibly not thread-safe?? fix next time
			std::mutex& mtx() {
				return mtx_;
			}
			std::vector<T>& vec() {
				return vector_;
			}
			uint64_t size() {
				return size_;
			}
			uint64_t maxSize() {
				return maxSize_;
			}
			uint64_t remaining() {
				return maxSize_ - vector_.size();
			}
		private:
			void updateSize() {
				size_ = vector_.size();
			}
			uint64_t maxSize_;
			std::atomic<uint64_t> size_;
			std::vector<T> vector_;
			std::mutex mtx_;

		};
	};
};