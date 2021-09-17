/*
 * ParserNMEA0183.cpp
 *
 *  Created on: 9 сент. 2021 г.
 *      Author: ARHIN
 */

#include "ParserNMEA0183.h"
#include <stdio.h>
using namespace std;

string ParserNMEA0183::buffer = "";
bool ParserNMEA0183::data_is_corrupted = true;

int ParserNMEA0183::hours;
int ParserNMEA0183::minutes;
int ParserNMEA0183::secs;

float ParserNMEA0183::latitude;
float ParserNMEA0183::longitude;
LATITUDE_DIRECTION ParserNMEA0183::latitude_direction;
LONGITUDE_DIRECTION ParserNMEA0183::longitude_direction;

bool ParserNMEA0183::parse(string &message)
{
    ParserNMEA0183::buffer.append(message);
    int start = 0;
    int end = 0;
    for (int i = 0; i < ParserNMEA0183::buffer.size(); ++i)
    {
        if (ParserNMEA0183::buffer[i] == '$')
        {
            if (i < ParserNMEA0183::buffer.size() - 8)
            {
                start = i + 1;
            }
            else
            {
                ParserNMEA0183::buffer.erase(0, start - 1);
            }
        }
        else if (ParserNMEA0183::buffer[i] == '*')
        {
            if (i != 0 && i > start + 8
                    && i < ParserNMEA0183::buffer.size() - 2)
            {
                end = i - 1;
                NMEA_TYPE type = checkMsgType(start);
                string cs = intToHexChar(checksum(start, end));
                if (type == NONE
                        || ParserNMEA0183::buffer.compare(end + 2, 2, cs))
                {
                    return false;
                }
                else if (type == GPGLL)
                {
                    string data = ParserNMEA0183::buffer.substr(start + 6, end - start - 5);
                    parseGPGLL(data);
                }

                ParserNMEA0183::buffer.erase(0, end + 3);

            }
        }
    }
    if (end == 0 && start == 0)
    {
        ParserNMEA0183::buffer.clear();
        return false;
    }
    return false;
}

NMEA_TYPE ParserNMEA0183::checkMsgType(
        unsigned int start)
{

    if (ParserNMEA0183::buffer.compare(start, start + 4, "GPGGA") == 0)
    {
        return GPGGA;
    }
    else if (ParserNMEA0183::buffer.compare(start, start + 4, "GPGSA") == 0)
    {
        return GPGSA;
    }
    else if (ParserNMEA0183::buffer.compare(start, start + 4, "GPGSV") == 0)
    {
        return GPGSV;
    }
    else if (ParserNMEA0183::buffer.compare(start, start + 4, "GPRMC") == 0)
    {
        return GPRMC;
    }
    else if (ParserNMEA0183::buffer.compare(start, start + 4, "GPCHN") == 0)
    {
        return GPCHN;
    }
    else if (ParserNMEA0183::buffer.compare(start, start + 4, "GPGLL") == 0)
    {
        return GPGLL;
    }
    else if (ParserNMEA0183::buffer.compare(start, start + 4, "GPVTG") == 0)
    {
        return GPVTG;
    }
    else if (ParserNMEA0183::buffer.compare(start, start + 4, "GPZDA") == 0)
    {
        return GPZDA;
    }
    else
    {
        return NONE;
    }
}

int ParserNMEA0183::checksum(unsigned int start,
                                             unsigned int end)
{
    int c = 0;
    for (int i = start; i <= end; i++)
    {
        c ^= ParserNMEA0183::buffer[i];
    }

    return c;
}

string ParserNMEA0183::intToHexChar(int var)
{
    string res = "";
    for (int i = 0; i < 2; i++)
    {
        res = hex_ASCII[var % 16] + res;
        var /= 16;
    }
    return res;
}

void ParserNMEA0183::parseGPGLL(string message)
{
    int index = 0;
    string buff = "";
    for (int i = 0; i <= message.size(); i++) {
        char c = message[i];
        if (c != ',' && i != message.size()) {
            buff += c;
        }
        else {

            switch (index)
            {
                //Latitude
            case 0:
                if (!buff.empty() && isdigit(buff[0])) {
                    ParserNMEA0183::latitude = atof(buff.c_str());
                }
                break;
                //Latitude direction
            case 1:
                if (buff == "N") latitude_direction = LATITUDE_DIRECTION::NORTH;
                else if (buff == "S") latitude_direction = LATITUDE_DIRECTION::SOUTH;
                break;
                //Longitude
            case 2:
                if (!buff.empty() && isdigit(buff[0])) {
                    longitude = atof(buff.c_str());
                }
                break;
                //Longitude direction
            case 3:
                if (buff == "E") longitude_direction = LONGITUDE_DIRECTION::EAST;
                else if (buff == "W") longitude_direction = LONGITUDE_DIRECTION::WEST;
                break;
                //UTC
            case 4:
                if (!buff.empty() && isdigit(buff[0])) {
                    hours = atoi(buff.c_str());
                }
                break;
                //Data validity
            case 5:
                if (buff == "A") {
                    data_is_corrupted = false;
                }
                else {
                    data_is_corrupted = true;
                }
                break;
            default:
                break;
            }

            index++;
            buff.clear();
        }
    }
    return;
}
