#ifndef BASIC_FUNCTIONS_H_
#define BASIC_FUNCTIONS_H_

#include <document.h>
#include <stdint.h>

namespace basic
{
    bool json_check_object(const rapidjson::Value& json, const char * field);
    bool json_check_array(const rapidjson::Value& json, const char * field);
    bool json_check_null(const rapidjson::Value& json, const char * field);
    bool json_get_value(const rapidjson::Value& json, const char * field, std::string & value);
    bool json_get_value(const rapidjson::Value& json, const char * field, int & value);
    bool json_get_value(const rapidjson::Value& json, const char * field, unsigned & value);
    bool json_get_value(const rapidjson::Value& json, const char * field, int64_t & value);
    bool json_get_value(const rapidjson::Value& json, const char * field, uint64_t & value);
    bool json_get_value(const rapidjson::Value& json, const char * field, bool & value);
    bool json_get_value(const rapidjson::Value& json, const char * field, double & value);
    bool json_get_value(const rapidjson::Value& json, const char * field, time_t & value);
}


#endif /* BASIC_FUNCTIONS_H_ */
