/**
 * Jacob Boyles & Avery Kitches
 * Jacob - 50%
 * Avery - 50%
 */
#include <mutex>
#include <iostream>
#include <vector>

#include "../includes/baker.h"
#include "../includes/externs.h"
using namespace std;

Baker::Baker(int id):id(id)
{
}

Baker::~Baker()
{
}

void Baker::bake_and_box(ORDER &anOrder) {
	// Get number of donuts & boxes
	int numDonuts = anOrder.number_donuts;
	int numBoxes = numDonuts / 12;

	// Fill each box with donuts
	std::vector<Box> theBoxes;
	for (int boxes = 0; boxes < numBoxes; boxes++) {
		Box box;
		for (int donuts = 0; donuts < numDonuts; donuts++) {
			DONUT donut;
			box.addDonut(donut);
		}
		theBoxes.push_back(box);
	}

	// Check if an extra box is needed
	if (numDonuts % 12 > 0) {
		Box box;
		for (int k = 0; k < numDonuts % 12; k++) {
			DONUT d;
			box.addDonut(d);
		}
		theBoxes.push_back(box);
	}
	anOrder.boxes = theBoxes;
}

void Baker::beBaker() {
	while (true) {
		std::unique_lock<std::mutex> inLock(mutex_order_inQ);
		while (order_in_Q.empty() && !b_WaiterIsFinished) {
			cv_order_inQ.wait(inLock);
		}
		// If the queue is not empty, bake and box the orders
		if (!order_in_Q.empty()) {
			ORDER order;
			order = order_in_Q.front();
			order_in_Q.pop();

			// Unlock as no longer accessing queue
			inLock.unlock();

			bake_and_box(order);

			std::lock_guard<std::mutex> outLock(mutex_order_outQ);
			order_out_Vector.push_back(order);
		}

		// No more orders so exit
		if (order_in_Q.empty() && b_WaiterIsFinished) {
			break;
		}
	}
}
