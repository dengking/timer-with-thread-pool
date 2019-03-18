/*
 * time.h
 *
 *  Created on: 2019Äê3ÔÂ18ÈÕ
 *      Author: dengkai17334
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <chrono>
#include <functional>
#include <queue>
#include <string>
#include <vector>

//#include <thread>
//#include <mutex>
//#include <condition_variable>

namespace TimeWithThreadPool {

/// \brief the timed event
class Event {
public:
	typedef uint64_t EventIdT;
	typedef std::chrono::steady_clock ClockT;
	typedef std::chrono::time_point<ClockT> TimestampT;
	typedef std::chrono::milliseconds DurationT;
	typedef std::function<void()> TaskT;

	Event(EventIdT id = 0) :
			id_(id), is_running_(false) {
	}

	template<typename FunctionT>
	Event(EventIdT id, TimestampT next, DurationT when, FunctionT&& task)
	noexcept :
			id_(id), when_(next), period_(when), task_(
					std::forward<FunctionT>(task)), is_running_(false) {
	}
	// not CopyConstructible
	Event(Event const& e) = delete;
	// MoveConstructible
	Event(Event&& e) noexcept :
			id_(e.id_), when_(e.when_), period_(e.period_), task_(
					std::move(e.task_)), is_running_(e.is_running_) {
	}
	// not CopyAssignable
	Event& operator=(Event const& e) = delete;
	// MoveAssignable
	Event& operator=(Event&& e) {
		if (this != &e) {
			id_ = e.id_;
			when_ = e.when_;
			period_ = e.period_;
			task_ = std::move(e.task_);
			is_running_ = e.is_running_;
		}
		return *this;
	}

public:
	std::string id_;
	std::chrono::time_point<std::chrono::steady_clock> when_; //time_point that indicates when it needs to fire
	std::chrono::milliseconds period_; //
	std::function<void()> task_; //
	bool is_running_; //whether the task is being run
};

class Timer {
public:
	Timer();
	virtual ~Timer() {
	}
private:
	// need to keep track of threads so we can join them
	std::vector<std::thread> workers_;
	// the task queue
	std::queue<std::function<void()> > tasks_;
};

} /* namespace TimeWithThreadPool */

#endif /* TIMER_H_ */
