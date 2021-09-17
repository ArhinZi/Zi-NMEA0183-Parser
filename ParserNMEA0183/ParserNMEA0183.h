/*
 * ParserNMEA0183.h
 *
 *  Created on: 9 сент. 2021 г.
 *      Author: ARHIN
 */

#ifndef PARSERNMEA0183_PARSERNMEA0183_H_
#define PARSERNMEA0183_PARSERNMEA0183_H_

#include <string>
using namespace std;

enum NMEA_TYPE
{
    GPGGA, GPGSA, GPGSV, GPRMC, GPCHN, GPGLL, GPVTG, GPZDA, NONE
};
const char hex_ASCII[17] = "0123456789ABCDEF";

enum LATITUDE_DIRECTION:bool {
    NORTH,
    SOUTH
};

enum LONGITUDE_DIRECTION :bool {
    EAST,
    WEST
};

class ParserNMEA0183
{
private:
    ParserNMEA0183()
    {
    }
    ~ParserNMEA0183()
    {
    }

    static string buffer;
    static int checksum(unsigned int start, unsigned int end);
    static NMEA_TYPE checkMsgType(unsigned int start);
    static string intToHexChar(int var);

    static void parseGPGGA(string message);
    static void parseGPGSA(string message);
    static void parseGPGSV(string message);
    static void parseGPRMC(string message);

    static void parseGPCHN(string message);
    static void parseGPGLL(string message);
    static void parseGPVTG(string message);
    static void parseGPZDA(string message);

    static void parseUTC(string message);

public:
    static bool data_is_corrupted;
    static int hours;
    static int minutes;
    static int secs;

    static float latitude;
    static float longitude;
    static LATITUDE_DIRECTION latitude_direction;
    static LONGITUDE_DIRECTION longitude_direction;

    static bool parse(string &message);

};


#endif /* PARSERNMEA0183_PARSERNMEA0183_H_ */
