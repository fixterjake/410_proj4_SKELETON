#include <string>
#include "stdlib.h"

#include "../includes/waiter.h"
#include "../includes/file_IO.h"
#include "../includes/externs.h"

using namespace std;

Waiter::Waiter(int id,std::string filename):id(id),myIO(filename){
}

Waiter::~Waiter()
{
}

//gets next Order(s) from file_IO
int Waiter::getNext(ORDER &anOrder){
	int result = myIO.getNext(anOrder);
	// No more orders so set is finished
	if (result != SUCCESS) {
		b_WaiterIsFinished = true;
		return NO_ORDERS;
	}
	return SUCCESS;
}

void Waiter::beWaiter() {
	ORDER order;
	getNext(order);

	while (!b_WaiterIsFinished) {
		// Add next order to queue
		{
			std::lock_guard<std::mutex> lock(mutex_order_inQ);
			order_in_Q.push(order);
		}
		// Notify bakers
		cv_order_inQ.notify_all();
		// Get next if any exists
		getNext(order);
	}
}

