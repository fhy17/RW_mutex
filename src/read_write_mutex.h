#ifndef __READ_WRITE_MUTEX__
#define __READ_WRITE_MUTEX__
#include <iostream>
#include <mutex>
#include <condition_variable>
namespace RWZone{
	class ReadWriteMutex{
	public:
		//ReadWriteMutex() = default;
		ReadWriteMutex(size_t read_count = 0, size_t write_count = 0, bool writing = false)
		: _read_count(read_count), _write_count(write_count), _writing(writing){ }
		~ReadWriteMutex() = default;

		ReadWriteMutex(const ReadWriteMutex&) = delete;
		ReadWriteMutex& operator=(const ReadWriteMutex&) = delete;

		ReadWriteMutex(const ReadWriteMutex&&) = delete;
		ReadWriteMutex& operator=(const ReadWriteMutex&&) = delete;

		void lock_read(){
			std::unique_lock<std::mutex> lck(_mtx);
			_read_cv.wait(lck, [this]()-> bool {
				return _write_count == 0;
			});
			++_read_count;
		}

		void unlock_read(){
			std::unique_lock<std::mutex> lck(_mtx);
			if(--_read_count == 0 && _write_count > 0){
				_write_cv.notify_one();
			}
		}

		void lock_write(){
			std::unique_lock<std::mutex> lck(_mtx);
			++_write_count;
			_write_cv.wait(lck, [this]()-> bool {
				return _read_count == 0 && !_writing;
			});
			_writing = true;
		}

		void unlock_write(){
			std::unique_lock<std::mutex> lck(_mtx);
			if(--_write_count == 0){
				_read_cv.notify_all();	
			}else{
				_read_cv.notify_one();
			}
			_writing = false;
		}
	private:
		volatile size_t _read_count;
		volatile size_t _write_count;
		volatile bool _writing;
		std::mutex _mtx;
		std::condition_variable _read_cv;
		std::condition_variable _write_cv;
	};

	template<typename RWLock>
	class UniqueReadLock{
	public:
		explicit UniqueReadLock(RWLock &rw_lck): _p_lck(&rw_lck){
			_p_lck->lock_read();
			std::cout<< "read lock" << std::endl;
		}
		~UniqueReadLock(){
			if(_p_lck){
				_p_lck->unlock_read();
			}
			std::cout<< "read unlock" << std::endl;
		}

		UniqueReadLock() = delete;
		UniqueReadLock(const UniqueReadLock&) = delete;
		UniqueReadLock& operator=(const UniqueReadLock&) = delete;
		
		UniqueReadLock(const UniqueReadLock&&) = delete;
		UniqueReadLock& operator=(const UniqueReadLock&&) = delete;
	private:
		RWLock *_p_lck = nullptr;
	};

	template<typename RWLock>
	class UniqueWriteLock{
	public:
		explicit UniqueWriteLock(RWLock &rw_lck): _p_lck(&rw_lck){
			_p_lck->lock_write();
			std::cout<< "write lock" << std::endl;
		}
		~UniqueWriteLock(){
			if(_p_lck){
				_p_lck->unlock_write();
			}
			std::cout<< "write unlock" << std::endl;
		}

		UniqueWriteLock() = delete;
		UniqueWriteLock(const UniqueWriteLock&) = delete;
		UniqueWriteLock& operator=(const UniqueWriteLock&) = delete;
		
		UniqueWriteLock(const UniqueWriteLock&&) = delete;
		UniqueWriteLock& operator=(const UniqueWriteLock&&) = delete;
	private:
		RWLock *_p_lck = nullptr;
	};

}
#endif // __READ_WRITE_MUTEX__
