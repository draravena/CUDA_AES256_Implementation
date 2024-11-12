#pragma once
#ifndef _THREAD_SAFE_DATATYPES_HPP
#define _THREAD_SAFE_DATATYPES_HPP

#ifndef LOCK_GUARD(mutex_)
#define LOCK_GUARD(mutex_) std::lock_guard<std::mutex> lock_mutex(mutex_)
#endif

#include <deque>
#include <optional>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <cstdint>

namespace datatype {
	template <typename T>
	class ThreadSafeDeque {
	public:
		ThreadSafeDeque();
		ThreadSafeDeque(uint64_t maxSize);
		void setMaxSize(uint64_t maxSize);
		bool push_back(T& item);
		bool push_back(T&& item);
		bool push_front(T& item);
		bool push_front(T&& item);
		std::optional<T&> front(bool erase = false);
		std::optional<T&> back(bool erase = false);
		bool pop_back();
		bool pop_front();
		// possibly not thread-safe?? fix next time
		std::mutex& mtx();
		std::deque<T>& deque();
		uint64_t size();
		uint64_t maxSize();
		uint64_t remaining();
	private:
		void updateSize();
		uint64_t maxSize_;
		std::atomic<uint64_t> size_;
		std::deque<T> deque_;
		std::mutex mtx_;
	};


	template <typename T>
	class ThreadSafeVector {
	public:
		ThreadSafeVector(uint64_t maxSize);
		ThreadSafeVector();
		void setMaxSize(uint64_t maxSize);
		bool push_back(T&& item);
		bool push_back(T& item);
		bool pop_back();
		std::optional<T> access(uint64_t index);
		int64_t move(ThreadSafeVector& destination, bool explicitAll = true);
		// possibly not thread-safe?? fix next time
		std::mutex& mtx();
		std::vector<T>& vec();
		uint64_t size();
		uint64_t maxSize();
		uint64_t remaining();
	private:
		void updateSize();
		uint64_t maxSize_;
		std::atomic<uint64_t> size_;
		std::vector<T> vector_;
		std::mutex mtx_;

	};
};

#include "threadSafeDeque.ipp"
#include "threadSafeVector.ipp"



#endif