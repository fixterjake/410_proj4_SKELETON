#include <mutex>
#include <cmath>
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

// Method to bake donuts regardless of how many boxes are needed
void bake(ORDER &order, int numDonuts, int numBoxes) {
	std::vector<DONUT> donuts;
	std::vector<Box> boxes;
	// Bake all donuts at once
	for (int i = 0; i < numDonuts; i++) {
		donuts.push_back(DONUT());
	}
	// Box all donuts
	for (int i = 0; i < numBoxes; i++) {
		Box box;
		for (int j = 0; j < 12; j++) {
			if (numDonuts == 0) {
				break;
			}
			box.addDonut(donuts.front());
			donuts.erase(donuts.begin());
		}
		boxes.push_back(box);
	}
	order.boxes = boxes;
}

void Baker::bake_and_box(ORDER &anOrder) {
	// Check how many boxes are needed
	if (anOrder.number_donuts % 12 != 0) {
		int numBoxes = std::round(anOrder.number_donuts / 12) + 1;
		int numDonuts = anOrder.number_donuts;
		bake(anOrder, numDonuts, numBoxes);
	}
	// Else just bake donuts with easy number of boxes
	else {
		int numBoxes = anOrder.number_donuts / 12;
		int numDonuts = anOrder.number_donuts;
		bake(anOrder, numDonuts, numBoxes);
	}
}

void Baker::beBaker() {
	if (order_in_Q.size() > 0 || b_WaiterIsFinished == true) {
		// Iterate through orders and bake them
		for (int i = 0; i < (int)order_in_Q.size(); i++) {
			bake_and_box(order_in_Q.front());
			order_in_Q.pop();
		}
	}
}
