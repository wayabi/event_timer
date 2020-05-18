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
	void start();
	void stop();
	bool add_timer_event(long long t, std::function<void()> f, bool loop);

private:
	void do_event(std::shared_ptr<boost::asio::deadline_timer> dt, long long t, std::function<void()> f, bool loop, const boost::system::error_code& error);

private:
	boost::asio::io_service& ios_;
	std::list<std::shared_ptr<boost::asio::deadline_timer>> timer_;
	std::mutex mtx_;
	bool enable_;
};

#endif
