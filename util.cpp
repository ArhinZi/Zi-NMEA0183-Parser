#include "util.h"
#include <cmath>

double zi::nmeadeg2geodeg(nmea_degrees ndeg) {
	double deg = (int)(ndeg.value / 100);
	deg += std::fmod(ndeg.value, 100)/60.0;
	if (ndeg.direction == DIRECTION::WEST || 
		ndeg.direction == DIRECTION::SOUTH) {
		return (-1) * deg;
	}
	else {
		return deg;
	}
}