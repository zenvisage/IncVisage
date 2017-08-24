#include <string>

#include <yaml-cpp/yaml.h>

#include "schema.h"

using namespace std;

Schema::Schema(const char* fname, uint64_t num_rows)
    : attrs_v(), attrs_m(), record_size(0), num_records(num_rows)
{
    
    YAML::Node schema = YAML::LoadFile(fname);
    YAML::Node cols = schema["cols"];
    
    for (YAML::const_iterator it = cols.begin(); it != cols.end(); it++) {
        attrs_v.push_back(Attr());
        Attr& a = attrs_v.back();
        a.set_name((*it)["name"].as<string>());
        const string& type = (*it)["type"].as<string>();
        if (type == "int") {
            a.set_type(new IntType());
        } else if (type == "float") {
            a.set_type(new FloatType());
        } else if (type == "ulong") {
            a.set_type(new ULongType());
        } else if (type == "double") {
            a.set_type(new DoubleType());
        } else {
            panic("Unknown type");
        }
        a.set_offset(record_size);
        record_size += a.get_type()->get_size();
        a.set_cardinality((*it)["cardinality"].as<uint64_t>());
        a.set_min((*it)["min"].as<double>());
        a.set_max((*it)["max"].as<double>());
        const string& bin_type = (*it)["bin_type"].as<string>();
        if (bin_type == "discrete") {
            a.set_bin_type(Attr::DISC_BIN);
        } else if (bin_type == "continuous") {
            a.set_bin_type(Attr::CONT_BIN);
        } else {
            panic("Unknown bin type");
        }
        attrs_m[a.get_name()] = a;
    }

    if (num_records == 0) {
        num_records = schema["rows"].as<uint64_t>();
        records_per_page = schema["records_per_page"].as<uint64_t>();

        num_pages = ceil((num_records*1.0)/records_per_page);

    }
}
