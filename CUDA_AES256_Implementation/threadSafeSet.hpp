#pragma once
#ifndef _THREAD_SAFE_SET_HPP
#define _THREAD_SAFE_SET_HPP

#ifndef LOCK_GUARD(mutex_)
#define LOCK_GUARD(mutex_) std::lock_guard<std::mutex> lock_mutex(mutex_)
#endif

#include <set>
#include <mutex>
#include <condition_variable>
#include <cstdint>
#include <optional>

namespace datatype {
	template <typename T>
	class ThreadSafeSet {
		public:
			ThreadSafeSet(uint64_t maxSize = UINT64_MAX);
			ThreadSafeSet(const ThreadSafeSet& others_set);
			ThreadSafeSet(std::set<T>::iterator begin, std::set<T>::iterator end);
			bool insert(const T& value);
			bool erase(const T& value);
			bool erase(std::set<T>::iterator position);
			bool erase(std::set<T>::iterator begin, std::set<T>::iterator end);
			void clear();
			uint64_t size();
			uint64_t maxSize();
			bool empty();

			std::set<T>::iterator find(const T& value);
			bool contains(const T& value);

			std::set<T>::iterator begin();
			std::set<T>::iterator end();
			std::set<T>::iterator cbegin();
			std::set<T>::iterator cend();
			std::set<T>::iterator rbegin();
			std::set<T>::iterator rend();
		private:
			std::set<T> set_;
			std::mutex mtx_;
			std::condition_variable cv_;
			uint64_t maxSize_;
	};
};

#include "threadSafeDeque.ipp"

#endif
