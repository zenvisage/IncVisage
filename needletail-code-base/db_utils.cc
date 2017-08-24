#include <fstream>
#include <sstream>

#include "db_utils.h"
#include "utils.h"

using namespace std;

void read_types(vector<Attr>& attrs, const char* types_fname)
{
    ifstream types_f(types_fname);
    string line;
    stringstream ss;
    string token;
    unsigned int i;

    // Get attr names
    getline(types_f, line);
    ss.str(line);
    while (getline(ss, token, ',')) {
        attrs.push_back(Attr());
        attrs.back().set_name(token);
    }
    ss.clear();

    // Get attr types
    getline(types_f, line);
    ss.str(line);
    i = 0;
    while (getline(ss, token, ',')) {
        if (token == "int") {
            attrs[i].set_type(new IntType());
        } else if (token == "float") {
            attrs[i].set_type(new FloatType());
        } else {
            attrs[i].set_type(new UnknownType());
        }
        i++;
    }
    ss.clear();

    // Get cardinalities
    getline(types_f, line);
    ss.str(line);
    i = 0;
    while (getline(ss, token, ',')) {
        attrs[i].set_cardinality(string2uint64_t(token));
        i++;
    }
    ss.clear();

    // Get bin type
    getline(types_f, line);
    ss.str(line);
    i = 0;
    while (getline(ss, token, ',')) {
        if (token == "d") {
            attrs[i].set_bin_type(Attr::DISC_BIN);
        } else if (token == "c") {
            attrs[i].set_bin_type(Attr::CONT_BIN);
        } else {
            panic("Unknown bin type");
        }
        i++;
    }
    ss.clear();

    // Get min value
    getline(types_f, line);
    ss.str(line);
    i = 0;
    while (getline(ss, token, ',')) {
        attrs[i].set_min(string2float(token));
        i++;
    }
    ss.clear();

    // Get max value
    getline(types_f, line);
    ss.str(line);
    i = 0;
    while (getline(ss, token, ',')) {
        attrs[i].set_max(string2float(token));
        i++;
    }
    ss.clear();

    types_f.close();
}
