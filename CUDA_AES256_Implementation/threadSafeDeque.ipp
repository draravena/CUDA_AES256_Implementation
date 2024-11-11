#pragma once
#ifndef _THREAD_SAFE_DEQUE_HPP
#define _THREAD_SAFE_DEQUE_HPP
#include "cuda_aes256.cuh"

namespace cuda_aes {
	namespace datatype {
		
		template <typename T>
		ThreadSafeDeque<T>::ThreadSafeDeque(uint64_t maxSize) {
			maxSize_ = maxSize;
			deque_.clear();
			updateSize();
		}
		template <typename T>
		ThreadSafeDeque<T>::ThreadSafeDeque() {}
		template <typename T>
		void ThreadSafeDeque<T>::setMaxSize(uint64_t maxSize) {
			maxSize_ = maxSize;
			deque_.clear();
			updateSize();
		}	
		template <typename T>
		bool ThreadSafeDeque<T>::push_back(T& item) {
			LOCK_GUARD(mtx_);
			if (deque_.size() >= maxSize_) {
				return false;
			}
			deque_.push_back(item);
			updateSize();
			return true;
		}
		template <typename T>
		bool ThreadSafeDeque<T>::push_back(T&& item) {
			LOCK_GUARD(mtx_);
			if (deque_.size() >= maxSize_) {
				return false;
			}
			deque_.push_back(std::move(item));
			updateSize();
			return true;
		}
		template <typename T>
		bool ThreadSafeDeque<T>::push_front(T& item) {
			LOCK_GUARD(mtx_);
			if (deque_.size() >= maxSize_) {
				return false;
			}
			deque_.push_front(item);
			updateSize();
			return true;
		}
		template <typename T>
		bool ThreadSafeDeque<T>::push_front(T&& item) {
			LOCK_GUARD(mtx_);
			if (deque_.size() >= maxSize_) {
				return false;
			}
			deque_.push_front(std::move(item));
			updateSize();
			return true;
		}
		template <typename T>
		std::optional<T&> ThreadSafeDeque<T>::front(bool erase) {
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
		template <typename T>
		std::optional<T&> ThreadSafeDeque<T>::back(bool erase) {
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
		template <typename T>
		bool ThreadSafeDeque<T>::pop_back() {
			LOCK_GUARD(mtx_);
			if (deque_.empty()) {
				return false;
			}
			deque_.pop_back();
			updateSize();
			return true;
		}
		template <typename T>
		bool ThreadSafeDeque<T>::pop_front() {
			LOCK_GUARD(mtx_);
			if (deque_.empty()) {
				return false;
			}
			deque_.pop_front();
			updateSize();
			return true;
		}
		template <typename T>
		std::mutex& ThreadSafeDeque<T>::mtx() {
			return mtx_;
		}
		template <typename T>
		std::deque<T>& ThreadSafeDeque<T>::deque() {
			return deque_;
		}
		template <typename T>
		uint64_t ThreadSafeDeque<T>::size() {
			return size_;
		}
		template <typename T>
		uint64_t ThreadSafeDeque<T>::maxSize() {
			return maxSize_;
		}
		template <typename T>
		uint64_t ThreadSafeDeque<T>::remaining() {
			return maxSize_ - deque_.size();
		}
		template <typename T>
		void ThreadSafeDeque<T>::updateSize() {
			size_ = deque_.size();
		}
	};
};

#endif