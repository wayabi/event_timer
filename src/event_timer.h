#ifndef __U_EVENT_TIMER__
#define __U_EVENT_TIMER__

#include <utility>
#include <functional>
#include <list>
#include <memory>
#include <mutex>

#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

class event_timer {
public:
	event_timer(boost::asio::io_service& ios);
	void add_timer_event(long long t, std::function<void()> f);
	void do_event(std::function<void()> f, std::shared_ptr<boost::asio::deadline_timer> t);

private:
	boost::asio::io_service& ios_;
	std::list<std::shared_ptr<boost::asio::deadline_timer>> timer_;
	std::mutex mtx_;
};

#endif
