#pragma once
#include <string>

namespace zi {
	enum NMEA_TYPE
	{
		NONE,
		GPGGA, 
		GPGSA, 
		GPGSV, 
		GPRMC, 
		GPCHN, 
		GPGLL, 
		GPVTG, 
		GPZDA
	};

	enum LATITUDE_DIRECTION :bool {
		NORTH,
		SOUTH
	};

	enum LONGITUDE_DIRECTION :bool {
		EAST,
		WEST
	};

	enum NMEA_GLL_STATUS:char {
		VALID = 'A',
		INVALID = 'V'
	};

	struct nmea_time {
		unsigned int hours;
		unsigned int minutes;
		unsigned int seconds;
	};

	struct nmea_latitude {
		float value;
		LATITUDE_DIRECTION direction;
	};

	struct nmea_longitude {
		float value;
		LONGITUDE_DIRECTION direction;
	};

	struct nmea_gll {
		struct nmea_time time;
		NMEA_GLL_STATUS status;
		struct nmea_latitude latitude;
		struct nmea_longitude longitude;
	};

	struct nmea_sentence {
		std::string type;
		std::string data;
		std::string checksum;
	};
}