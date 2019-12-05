#include <iostream>
#include <thread>
#include "read_write_mutex.h"

const size_t max_count = 10;

void readData(RWZone::ReadWriteMutex &mtx, int &data, size_t &count){
	//std::cout << __FUNCTION__ << " "<< __LINE__  <<std::endl;
	RWZone::UniqueReadLock<RWZone::ReadWriteMutex> lck(mtx);
	// To do
	if(count < max_count){
		std::cout<< "read thread " << std::this_thread::get_id()
		<< " the " << count << "th " << data <<  std::endl;
		++count;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void writeData(RWZone::ReadWriteMutex &mtx, int &data, size_t &count){
	RWZone::UniqueWriteLock<RWZone::ReadWriteMutex> lck(mtx);
	// To do
	if(count >= max_count){
		data++;
		std::cout<< "write thread " << std::this_thread::get_id()
		<< " the " << count << "th " << data << std::endl;
		count = 0;
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void readThread(RWZone::ReadWriteMutex &mtx, int &data, size_t &count){
	std::cout << __FUNCTION__ << " "<< __LINE__  <<std::endl;
	while(true){
		readData(mtx, data, count);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void writeThread(RWZone::ReadWriteMutex &mtx, int &data, size_t &count){
	std::cout << __FUNCTION__ << " "<< __LINE__  <<std::endl;
	while(true){
		writeData(mtx, data, count);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}
void funcReadWriteMutex(){
	std::cout << __FUNCTION__ << " "<< __LINE__  <<std::endl;
	int data = 0;
	size_t count = max_count;
	RWZone::ReadWriteMutex mtx;
	std::thread write_thread(writeThread, std::ref(mtx), std::ref(data), std::ref(count));
	std::thread read_thread1(readThread, std::ref(mtx), std::ref(data), std::ref(count));
	std::thread read_thread2(readThread, std::ref(mtx), std::ref(data), std::ref(count));
	std::thread read_thread3(readThread, std::ref(mtx), std::ref(data), std::ref(count));
	std::thread read_thread4(readThread, std::ref(mtx), std::ref(data), std::ref(count));

	write_thread.join();
	read_thread1.join();
	read_thread2.join();
	read_thread3.join();
	read_thread4.join();
}
