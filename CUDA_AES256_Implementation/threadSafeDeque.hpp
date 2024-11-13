#pragma once
#ifndef _THREAD_SAFE_DEQUE_HPP
#define _THREAD_SAFE_DEQUE_HPP

#ifndef LOCK_GUARD(mutex_)
#define LOCK_GUARD(mutex_) std::lock_guard<std::mutex> lock_mutex(mutex_)
#endif

#include <cstdint>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <optional>

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
};

#include "threadSafeDeque.ipp"

#endif