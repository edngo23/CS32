#include "CarMap.h"

#include <iostream>
using namespace std;


bool CarMap::addCar(std::string license) {
	return map.insert(license, 0);
}

double CarMap::miles(std::string license) const {
	double mileage = -1;
	map.get(license, mileage);
	return mileage;
}

bool CarMap::drive(std::string license, double distance) {
	if (distance < 0)
		return false;
	return map.update(license, this->miles(license) + distance);
}

int CarMap::fleetSize() const {
	return map.size();
}

void CarMap::print() const {
	string tempString;
	double tempDouble;
	for (int i = 0; i < fleetSize(); i++) {
		map.get(i, tempString, tempDouble);
		cout << tempString << " " << tempDouble << endl;
	}
}
