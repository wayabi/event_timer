#include "event_timer.h"

#include <iostream>

#include <boost/thread.hpp>
#include <boost/asio.hpp>

using namespace std;
using namespace boost;
using namespace boost::asio;

void print1(){
	cout << "1" << endl;
}

int count2 = 0;
void print2(){
	cout << count2++ << endl;
}

int main(int argc, char** argv){
	io_service ios_;
	io_service::work work_(ios_);

	boost::thread_group tg_;
	tg_.create_thread(boost::bind(&io_service::run, &ios_));

	event_timer et_(ios_);
	et_.start();
	et_.add_timer_event(1000, print1, true);
	for(int i=0;i<1000;++i){
		et_.add_timer_event(500, print2, false);
		usleep(1*1000);
	}

	cout << "stop" << endl;
	et_.stop();

	usleep(2*1000*1000);

	cout << "restart" << endl;
	et_.start();
	usleep(2*1000*1000);
	et_.add_timer_event(1000, print1, true);
	usleep(5*1000*1000);

	return 0;
}
