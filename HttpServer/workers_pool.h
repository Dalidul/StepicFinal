#ifndef WORKERS_POOL_H
#define WORKERS_POOL_H

#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <atomic>


template<typename ValueT>
class ThredsafeQueue
{
public:
	bool pop(ValueT &result);
	void push(const ValueT &val);
	void clear();

private:
	std::mutex _mutex;
	std::queue<ValueT> _queue;
};

template<typename ValueT>
bool ThredsafeQueue<ValueT>::pop(ValueT &result)
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (!_queue.empty()) {
		result = _queue.front();
		_queue.pop();
		return true;
	}
	else return false;
}

template<typename ValueT>
void ThredsafeQueue<ValueT>::push(const ValueT &val)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_queue.push(val);
}

template<typename ValueT>
void ThredsafeQueue<ValueT>::clear()
{
	std::lock_guard<std::mutex> lock(_mutex);
	while(!_queue.empty()) _queue.pop();
}


// =============================================================================

template<typename T>
class WorkersPool
{
	typedef std::shared_ptr<std::thread> ThreadPtr;

	static unsigned DEFAULT_TIMEOUT;


public:
	template <typename HandlerT>
	WorkersPool(HandlerT dataHandler, unsigned timeout = DEFAULT_TIMEOUT)
	    : _workersCount(0)
	    , _stop(false)
	    , _timeout(timeout)
	{
		_workersCount = std::thread::hardware_concurrency()
		                ? std::thread::hardware_concurrency()
		                : 2;
		initWorkers(dataHandler);
	}

	~WorkersPool();

	template <typename HandlerT>
	void initWorkers(HandlerT dataHandler)
	{
		auto mainCycle = [this](HandlerT handler) -> void {
			while(!this->_stop) {
				T data;
				if (this->_queue.pop(data)) {
					handler(data);
				}
				else {
					std::this_thread::sleep_for(
					        std::chrono::milliseconds(this->_timeout));
				}
			}
		};

		for (unsigned i = 0; i < _workersCount; ++i) {
			_workers.push_back(ThreadPtr(new std::thread(mainCycle, dataHandler)));
		}
	}

	void addDataToQueue(T val);

private:
	unsigned _workersCount;
	std::vector<ThreadPtr> _workers;
	ThredsafeQueue<T> _queue;
	std::atomic_bool _stop;
	unsigned _timeout;  // milliseconds
};

// -----------------------------------------------------------------------------

template <typename T> unsigned WorkersPool<T>::DEFAULT_TIMEOUT = 100;

template <typename T>
WorkersPool<T>::~WorkersPool()
{
	_stop = true;
	for (auto &worker: _workers) {
		worker->join();
	}
}

template <typename T>
void WorkersPool<T>::addDataToQueue(T val)
{
	_queue.push(val);
}

#endif //WORKERS_POOL_H
