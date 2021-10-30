#pragma once
#include "Nmea0183Types.h"

namespace zi {
	// convert nmea degrees in strange format to normal digital degrees
	double nmeadeg2geodeg(double ndeg, zi);
}