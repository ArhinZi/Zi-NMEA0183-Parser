#pragma once
#include "Nmea0183Types.h"

namespace zi {
	// convert nmea degrees in strange format to normal double degrees
	double nmeadeg2geodeg(nmea_degrees deg);
}