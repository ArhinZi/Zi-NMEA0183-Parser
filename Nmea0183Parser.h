#pragma once
#include <string>
#include "Nmea0183Types.h"
using namespace std;

namespace zi {
	class Nmea0183Parser
	{
	public:
		struct nmea_gll gll;

		Nmea0183Parser(int max_data_size, int max_buffer_size);
		bool parse(string data);
	private:
		unsigned int max_data_size;
		unsigned int max_buffer_size;
		string buffer = "";
		const char hex_ASCII[17] = "0123456789ABCDEF";
		const int min_sentence_lenght = 10;
		string intToHexChar(int var);
		int checksum(const string &sentence_data);
		bool check_checksum(const nmea_sentence& sentence);
		NMEA_TYPE check_sentence_type(const string &sentence_type);
		bool parse_gga(const string& sentence_data);
		bool parse_gll(const string &sentence_data);
		bool parse_buffer();
	};
}
