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
	enable_ = false;
}

bool event_timer::add_timer_event(long long t, std::function<void()> f, bool loop)
{
	if(!enable_){
		return false;
	}
	auto timer = make_shared<deadline_timer>(ios_, posix_time::milliseconds(t));
	{
		std::lock_guard<std::mutex> lock(mtx_);
		timer_.push_back(timer);
	}

	timer->async_wait(boost::bind(&event_timer::do_event, this, timer, t, f, loop, boost::placeholders::_1));
	return true;
}

void event_timer::do_event(std::shared_ptr<deadline_timer> dt, long long t, std::function<void()> f, bool loop, const boost::system::error_code& error)
{
	if(error){
		std::lock_guard<std::mutex> lock(mtx_);
		timer_.remove(dt);
		return;
	}

	f();
	{
		std::lock_guard<std::mutex> lock(mtx_);
		timer_.remove(dt);
	}
	if(enable_ && loop){
		add_timer_event(t, f, loop);
	}
}

void event_timer::start()
{
	std::lock_guard<std::mutex> lock(mtx_);
	enable_ = true;
}

void event_timer::stop()
{
	std::lock_guard<std::mutex> lock(mtx_);
	for(auto ite = timer_.begin();ite != timer_.end();++ite){
		(*ite)->cancel();
	}
	timer_.clear();
	enable_ = false;
}
