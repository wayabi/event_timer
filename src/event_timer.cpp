#include "event_timer.h"

#include <iostream>
#include <string>

#include <boost/bind.hpp>

using namespace std;
using namespace boost;
using namespace boost::asio;

event_timer::event_timer(boost::asio::io_service& ios) :
	ios_(ios)
{
}

void event_timer::add_timer_event(long long t, std::function<void()> f)
{
	auto timer = make_shared<deadline_timer>(ios_, posix_time::milliseconds(t));
	{
		std::lock_guard<std::mutex> lock(mtx_);
		timer_.push_back(timer);
	}

	timer->async_wait(boost::bind(&event_timer::do_event, this, f, timer));
}

void event_timer::do_event(std::function<void()> f, std::shared_ptr<deadline_timer> t)
{
	f();
	{
		std::lock_guard<std::mutex> lock(mtx_);
		timer_.remove(t);
	}
}
