#pragma once
#ifndef _THREAD_SAFE_VECTOR_HPP
#define _THREAD_SAFE_VECTOR_HPP

#ifndef LOCK_GUARD(mutex_)
#define LOCK_GUARD(mutex_) std::lock_guard<std::mutex> lock_mutex(mutex_)
#endif

#include <mutex>
#include <unordered_map>
#include <condition_variable>
#include <vector>
#include <cstdint>
#include <optional>

namespace datatype {
	template <typename T>
	class ThreadSafeVector {
	public:
		ThreadSafeVector(uint64_t maxSize, uint64_t elementsPerMutex = UINT64_MAX);
		ThreadSafeVector();
		void setMaxSize(uint64_t maxSize, uint64_t elementsPerMutex = UINT64_MAX);
		bool push_back(T&& item);
		bool push_back(T& item);
		bool pop_back();
		std::optional<T> access(uint64_t index);
		bool set(uint64_t index, T& val);
		bool set(uint64_t index, T&& val);
		int64_t move(ThreadSafeVector& destination, bool explicitAll = true);
		uint64_t size();
		uint64_t maxSize();
		uint64_t remaining();
	private:
		std::vector<T>& vec();
		void updateSize();
		void updateMutexMap();
		std::mutex& mtx();
		uint64_t maxSize_;
		uint64_t elementsPerMutex_;
		std::atomic<uint64_t> size_;
		std::vector<T> vector_;
		std::unordered_map<uint64_t, std::shared_ptr<std::mutex>> mutex_map_;
		std::mutex globalmtx_;
	};
};

#include "threadSafeVector.ipp"

#endif