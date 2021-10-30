#include "Nmea0183Parser.h"

using namespace zi;

Nmea0183Parser::Nmea0183Parser(int max_data_size, int max_buffer_size)
{
	this->max_data_size = max_data_size;
    this->max_buffer_size = max_buffer_size;
}

bool Nmea0183Parser::parse(string data)
{
    if (data.size() > max_data_size) {
        return false;
    }
    if (data.size() + buffer.size() > max_buffer_size) {
        buffer.clear();
    }
	buffer.append(data);
	return parse_buffer();
}

string Nmea0183Parser::intToHexChar(int var)
{
    string res = "";
    for (int i = 0; i < 2; i++)
    {
        res = hex_ASCII[var % 16] + res;
        var /= 16;
    }
    return res;
}

int Nmea0183Parser::checksum(const string &sentence_data)
{
    int res = 0;
    for (char c: sentence_data)
    {
        res ^= c;
    }
    return res;
}

bool Nmea0183Parser::parse_buffer()
{
    unsigned int start = 0;
    unsigned int end = 0;

    for (int i = 0; i < buffer.size(); ++i) {
        if (buffer[i] == '$')
        {
            if (i < buffer.size() - min_sentence_lenght)
            {
                start = i + 1;
            }
            else
            {
                buffer.erase(0, start - 1);
                return false;
            }
        }
        else if (buffer[i] == '*')
        {
            if (i > start + min_sentence_lenght && i < buffer.size() - 2) {
                end = i - 1;
                string raw_sentence = buffer.substr(start, end+3);
                struct nmea_sentence sentence;
                sentence.type = raw_sentence.substr(0, 5);
                sentence.checksum = raw_sentence.substr(raw_sentence.size()-2, 2);
                sentence.data = raw_sentence.substr(6, raw_sentence.size() - 9);

                if (!check_checksum(sentence)) {
                    buffer.erase(0, end+2);
                    return false;
                }
                NMEA_TYPE type = check_sentence_type(sentence.type);

                if (type == NMEA_TYPE::GPGLL)
                    parse_gll(sentence.data);

                buffer.erase(0, end + 2);

            }
            else {
                return false;
            }
        }
    }

    if (end == 0 && start == 0)
    {
        buffer.clear();
        return false;
    }
    return true;
}

bool Nmea0183Parser::check_checksum(const nmea_sentence& sentence)
{
    return sentence.checksum == intToHexChar(checksum(sentence.type + ',' + sentence.data));
}

NMEA_TYPE Nmea0183Parser::check_sentence_type(const string& sentence_type)
{
    //Checking first five chars in sentence
    if (sentence_type == "GPGGA")
        return NMEA_TYPE::GPGGA;
    if (sentence_type == "GPGSA")
        return NMEA_TYPE::GPGSA;
    if (sentence_type == "GPGSV")
        return NMEA_TYPE::GPGSV;
    if (sentence_type == "GPRMC")
        return NMEA_TYPE::GPRMC;
    if (sentence_type == "GPCHN")
        return NMEA_TYPE::GPCHN;
    if (sentence_type == "GPGLL")
        return NMEA_TYPE::GPGLL;
    if (sentence_type == "GPVTG")
        return NMEA_TYPE::GPVTG;
    if (sentence_type == "GPZDA")
        return NMEA_TYPE::GPZDA;

    return NMEA_TYPE::NONE;
}

bool Nmea0183Parser::parse_gll(const string &sentence_data)
{
    // 5532.8492,N,03729.0987,E,004241.469,A
    // latitude, lat_direction, longitude, lon_direction, utc, date_validity

    int index = 0;
    string buff = "";
    for (int i = 0; i <= sentence_data.size(); i++) {
        char c = sentence_data[i];
        if (c != ',' && i != sentence_data.size()) {
            if(c != ' ')
                buff += c;
        }
        else if (!buff.empty())
        {
            switch (index)
            {
                //Latitude
            case 0:
                if (!buff.empty() && isdigit(buff[0])) {
                    gll.latitude.value = atof(buff.c_str());
                }
                break;
                //Latitude direction
            case 1:
                if (buff == "N") gll.latitude.direction = DIRECTION::NORTH;
                else if (buff == "S") gll.latitude.direction = DIRECTION::SOUTH;
                break;
                //Longitude
            case 2:
                if (!buff.empty() && isdigit(buff[0])) {
                    gll.longitude.value = atof(buff.c_str());
                }
                break;
                //Longitude direction
            case 3:
                if (buff == "E") gll.longitude.direction = DIRECTION::EAST;
                else if (buff == "W") gll.longitude.direction = DIRECTION::WEST;
                break;
                //UTC
            case 4:
                if (!buff.empty() && isdigit(buff[0])) {
                    gll.time.hours = atoi(buff.substr(0, 2).c_str());
                    gll.time.minutes = atoi(buff.substr(2, 2).c_str());
                    gll.time.seconds = atoi(buff.substr(4, 2).c_str());
                }
                break;
                //Data validity
            case 5:
                if (*(buff.c_str()) == (char)NMEA_GLL_STATUS::VALID) {
                    gll.status = NMEA_GLL_STATUS::VALID;
                }
                else {
                    gll.status = NMEA_GLL_STATUS::INVALID;
                }
                break;
            default:
                break;
            }
            index++;
            buff.clear();
        }
        else {
            index++;
            buff.clear();
        }
    }
    return true;
}

bool zi::Nmea0183Parser::parse_gga(const string& sentence_data)
{
    // 004241.47, 5532.8492, N, 03729.0987, E, 1, 04, 2.0, -0015,M,,,,

    return false;
}
