#ifndef _BINNER_H_
#define _BINNER_H_

#include <string>

#include <boost/functional/hash.hpp>

#include "db_utils.h"

/* TODO - Binner should ideally allocate space for the keys */
class Binner {
public:
    typedef size_t key_t;

    key_t get_key(const Attr& attr, const uint8_t* val);

protected:
    boost::hash<double> double_hasher;
    boost::hash<std::string> string_hasher;
};

#endif /* _BINNER_H_ */
