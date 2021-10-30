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


	enum DIRECTION :char {
		NORTH = 'N',
		SOUTH = 'S',
		EAST = 'E',
		WEST = 'W'
	};

	enum NMEA_GLL_STATUS:char {
		VALID = 'A',
		INVALID = 'V'
	};

	enum GPS_QUALITY:char {
		UNDEF = '0',
		DEFAULT = '1',
		DIFFERENCIAL = '2',
		PRECISION = '3'
	};

	struct nmea_time {
		unsigned int hours =0;
		unsigned int minutes =0;
		unsigned int seconds =0;
	};

	struct nmea_degrees {
		float value =0;
		DIRECTION direction = DIRECTION::NORTH;
	};


	struct nmea_metering_value {
		float value = 0;
		char unit = 'M';
	};

	struct nmea_gll {
		struct nmea_time time;
		NMEA_GLL_STATUS status = NMEA_GLL_STATUS::INVALID;
		struct nmea_degrees latitude;
		struct nmea_degrees longitude;
	};

	struct nmea_gga {
		struct nmea_time time;
		struct nmea_degrees latitude;
		struct nmea_degrees longitude;
		GPS_QUALITY signal_quality = GPS_QUALITY::UNDEF;
		unsigned int sattelites_in_use = 0;
		float hdop = 0;
		struct nmea_metering_value antenna_height;
		struct nmea_metering_value geoidal_difference;
		float differencial_data_time = 0;
		std::string id_diff_station = 0;
	};

	struct nmea_sentence {
		std::string type ="";
		std::string data ="";
		std::string checksum ="";
	};
}