#include "event_timer.h"

#include <iostream>

#include <boost/thread.hpp>
#include <boost/asio.hpp>

using namespace std;
using namespace boost;
using namespace boost::asio;

void print(){
	cout << "1" << endl;
}

int main(int argc, char** argv){
	io_service ios_;
	io_service::work work_(ios_);

	boost::thread_group tg_;
	tg_.create_thread(boost::bind(&io_service::run, &ios_));

	event_timer et_(ios_);
	while(true){
		usleep(200*1000);
		et_.add_timer_event(1000, print);
	}

	return 0;
}
