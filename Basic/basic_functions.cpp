#include "basic_functions.h"
#include <dbgout.h>
#include <sstream>
#include <time.h>

namespace basic
{

    template<class T>
    bool str_convert(std::string str, T & value)
    {
        bool result = false;
        if(str != "")
        {
            std::istringstream num(str);
            num >> value;
            result = (!num.fail() && num.eof());
        }
        return result;
    }

    bool json_check_object(const rapidjson::Value& json, const char * field)
    {
        bool result = false;

        if(json.HasMember(field))
        {
            if(json[field].IsObject())
            {
                result = true;
            }
            else
            {
                dbg::dout  << dbg::err << "Member '" << field << "' is not an object" << dbg::endl;
            }
        }
        else
        {
            dbg::dout  << dbg::err << "Member '" << field << "' is not exists." << dbg::endl;
        }

        return result;
    }

    bool json_check_array(const rapidjson::Value& json, const char * field)
    {
        bool result = false;

        if(json.HasMember(field))
        {
            if(json[field].IsArray())
            {
                result = true;
            }
            else
            {
                dbg::dout  << dbg::err << "Member '" << field << "' is not an array" << dbg::endl;
            }
        }
        else
        {
            dbg::dout  << dbg::err << "Member '" << field << "' is not exists." << dbg::endl;
        }

        return result;
    }

    bool json_get_value(const rapidjson::Value& json, const char * field, std::string & value)
    {
        bool result = false;

        if(json.HasMember(field))
        {
            if(json[field].IsString())
            {
                value = json[field].GetString();
                result = true;
            }
            else
            {
                dbg::dout  << dbg::err << "Member '" << field << "' is not a string." << dbg::endl;
            }
        }
        else
        {
            dbg::dout  << dbg::err << "Member '" << field << "' is not exists." << dbg::endl;
        }
        return result;
    }

    bool json_get_value(const rapidjson::Value& json, const char * field, int & value)
    {
        bool result = false;

        if(json.HasMember(field))
        {
            if(json[field].IsInt())
            {
                value = json[field].GetInt();
                result = true;
            }
            else if(json[field].IsString())
            {
                std::string tmp(json[field].GetString());
                if(str_convert(tmp, value))
                {
                    result = true;
                }
                else
                {
                    dbg::dout  << dbg::err << "Member '" << field << "' is string which can't be converted to integer." << dbg::endl;
                }
            }
            else
            {
                dbg::dout  << dbg::err << "Member '" << field << "' is not an integer." << dbg::endl;
            }
        }
        else
        {
            dbg::dout  << dbg::err << "Member '" << field << "' is not exists." << dbg::endl;
        }
        return result;
    }

    bool json_get_value(const rapidjson::Value& json, const char * field, unsigned int & value)
    {
        bool result = false;

        if(json.HasMember(field))
        {
            if(json[field].IsUint())
            {
                value = json[field].GetUint();
                result = true;
            }
            else if(json[field].IsString())
            {
                std::string tmp(json[field].GetString());
                if(str_convert(tmp, value))
                {
                    result = true;
                }
                else
                {
                    dbg::dout  << dbg::err << "Member '" << field << "' is string which can't be converted to unsigned integer." << dbg::endl;
                }
            }
            else
            {
                dbg::dout  << dbg::err << "Member '" << field << "' is not an unsigned integer." << dbg::endl;
            }
        }
        else
        {
            dbg::dout  << dbg::err << "Member '" << field << "' is not exists." << dbg::endl;
        }
        return result;
    }

    bool json_get_value(const rapidjson::Value& json, const char * field, int64_t & value)
    {
        bool result = false;

        if(json.HasMember(field))
        {
            if(json[field].IsInt64())
            {
                value = json[field].GetInt64();
                result = true;
            }
            else if(json[field].IsString())
            {
                std::string tmp(json[field].GetString());
                if(str_convert(tmp, value))
                {
                    result = true;
                }
                else
                {
                    dbg::dout  << dbg::err << "Member '" << field << "' is string which can't be converted to integer 64." << dbg::endl;
                }
            }
            else
            {
                dbg::dout  << dbg::err << "Member '" << field << "' is not an integer 64." << dbg::endl;
            }
        }
        else
        {
            dbg::dout  << dbg::err << "Member '" << field << "' is not exists." << dbg::endl;
        }
        return result;
    }

    bool json_get_value(const rapidjson::Value& json, const char * field, uint64_t & value)
    {
        bool result = false;

        if(json.HasMember(field))
        {
            if(json[field].IsUint64())
            {
                value = json[field].GetUint64();
                result = true;
            }
            else if(json[field].IsString())
            {
                std::string tmp(json[field].GetString());
                if(str_convert(tmp, value))
                {
                    result = true;
                }
                else
                {
                    dbg::dout  << dbg::err << "Member '" << field << "' is string which can't be converted to unsigned integer 64." << dbg::endl;
                }
            }
            else
            {
                dbg::dout  << dbg::err << "Member '" << field << "' is not an unsigned integer 64." << dbg::endl;
            }
        }
        else
        {
            dbg::dout  << dbg::err << "Member '" << field << "' is not exists." << dbg::endl;
        }
        return result;
    }

    bool json_get_value(const rapidjson::Value& json, const char * field, bool & value)
    {
        bool result = false;

        if(json.HasMember(field))
        {
            if(json[field].IsBool())
            {
                value = json[field].GetBool();
                result = true;
            }
            else if(json[field].IsString())
            {
                std::string tmp(json[field].GetString());
                if(str_convert(tmp, value))
                {
                    result = true;
                }
                else
                {
                    dbg::dout  << dbg::err << "Member '" << field << "' is string which can't be converted to boolean." << dbg::endl;
                }
            }
            else
            {
                dbg::dout  << dbg::err << "Member '" << field << "' is not a boolean." << dbg::endl;
            }
        }
        else
        {
            dbg::dout  << dbg::err << "Member '" << field << "' is not exists." << dbg::endl;
        }
        return result;
    }

    bool json_get_value(const rapidjson::Value& json, const char * field, double & value)
    {
        bool result = false;

        if(json.HasMember(field))
        {
            if(json[field].IsDouble())
            {
                value = json[field].GetDouble();
                result = true;
            }
            else if(json[field].IsString())
            {
                std::string tmp(json[field].GetString());
                if(str_convert(tmp, value))
                {
                    result = true;
                }
                else
                {
                    dbg::dout  << dbg::err << "Member '" << field << "' is string which can't be converted to double." << dbg::endl;
                }
            }
            else
            {
                dbg::dout  << dbg::err << "Member '" << field << "' is not a double." << dbg::endl;
            }
        }
        else
        {
            dbg::dout  << dbg::err << "Member '" << field << "' is not exists." << dbg::endl;
        }
        return result;
    }

    bool json_get_value(const rapidjson::Value& json, const char * field, time_t & value)
    {
        bool result = false;

        if(json.HasMember(field))
        {
            if(json[field].IsUint64())
            {
                value = json[field].GetUint64();
                result = true;
            }
            else if(json[field].IsString())
            {
                std::string tmp(json[field].GetString());
                struct tm timeinfo;
                if(str_convert(tmp, value))
                {
                    result = true;
                }
                else if(strptime(tmp.c_str(), "%Y-%m-%dT%T%Z", &timeinfo))
                {
                    value = mktime(&timeinfo);
                    result = true;
                }
                else
                {
                    dbg::dout  << dbg::err << "Member '" << field << "' is string which can't be converted to time_t." << dbg::endl;
                }
            }
            else
            {
                dbg::dout  << dbg::err << "Member '" << field << "' is not time_t" << dbg::endl;
            }
        }
        else
        {
            dbg::dout  << dbg::err << "Member '" << field << "' is not exists." << dbg::endl;
        }
        return result;
    }

}
