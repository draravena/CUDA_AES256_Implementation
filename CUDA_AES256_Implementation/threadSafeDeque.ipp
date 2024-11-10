#pragma once
#include "cuda_aes256.cuh"

namespace cuda_aes {
	namespace datatype {
		template <typename T>
		class ThreadSafeDeque {
			public:
				ThreadSafeDeque() {}
				ThreadSafeDeque(uint64_t maxSize) {
					maxSize_ = maxSize;
					deque_.reserve(maxSize);
					deque_.clear();
					updateSize();
				}			
				void setMaxSize(uint64_t maxSize) {
					maxSize_ = maxSize;
					deque_.reserve(maxSize);
					deque_.clear();
					updateSize();
				}
				bool push_back(T&& item) {
					LOCK_GUARD(mtx_);
					if (deque_.size() >= maxSize_) {
						return false;
					}
					deque_.push_back(std::move(item));
					updateSize();
					return true;
				}
				bool push_front(T&& item) {
					LOCK_GUARD(mtx_);
					if (deque_.size() >= maxSize_) {
						return false;
					}
					deque_.push_front(std::move(item));
					updateSize();
					return true;
				}

				std::optional<T&> front(bool erase = false) {
					LOCK_GUARD(mtx_);
					if (deque_.empty()) {
						return std::nullopt;
					}
					T& val = deque_.front();
					if (erase == true) {
						deque_.pop_front();
					}
					return val;
				}
				std::optional<T&> back(bool erase = false) {
					LOCK_GUARD(mtx_);
					if (deque_.empty()) {
						return std::nullopt;
					}
					T& val = deque_.back();
					if (erase == true) {
						deque_.pop_back();
					}
					return val;
				}
				bool pop_back() {
					LOCK_GUARD(mtx_);
					if (deque_.empty()) {
						return false;
					}
					deque_.pop_back();
					updateSize();
					return true;
				}
				bool pop_front() {
					LOCK_GUARD(mtx_);
					if (deque_.empty()) {
						return false;
					}
					deque_.pop_front();
					updateSize();
					return true;
				}
				// possibly not thread-safe?? fix next time
				std::mutex& mtx() {
					return mtx_;
				}
				std::deque<T>& deque() {
					return deque_;
				}
				uint64_t size() {
					return size_;
				}
				uint64_t maxSize() {
					return maxSize_;
				}
				uint64_t remaining() {
					return maxSize_ - deque_.size();
				}
			private:
				void updateSize() {
					size_ = deque_.size();
				}
				uint64_t maxSize_;
				std::atomic<uint64_t> size_;
				std::deque<T> deque_;
				std::mutex mtx_;
		};
	};
};