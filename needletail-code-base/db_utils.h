#ifndef _DB_UTILS_H_
#define _DB_UTILS_H_

#include <string>
#include <cinttypes>
#include <cstring>
#include <vector>

#include "utils.h"

const uint64_t MAX_TYPE_SIZE = 8;

enum type_constant_t {
    INT_TYPE,
    FLOAT_TYPE,
    ULONG_TYPE,
    DOUBLE_TYPE,
    STRING_TYPE
};

class Type {
public:
    virtual uint64_t get_size() const { UNIMPLEMENTED(get_size); }
    virtual const char* get_name() const { UNIMPLEMENTED(get_name); }
    virtual void parse(uint8_t* buf, const std::string& data) const
    { UNIMPLEMENTED(parse); }
    virtual const std::string to_string(const uint8_t* buf) const
    { UNIMPLEMENTED(to_string); }
    virtual bool is_num_type() const { UNIMPLEMENTED(is_num_type); }
    virtual type_constant_t get_type_constant() const
    { UNIMPLEMENTED(get_type_constant); }
    virtual double to_double(const uint8_t* buf) const
    { UNIMPLEMENTED(to_double); }
    virtual unsigned long long to_ulong(const uint8_t* buf) const
    { UNIMPLEMENTED(to_long); }
};

class ULongType : public Type {
public:
    virtual uint64_t get_size() const { return sizeof(unsigned long long); }
    virtual const char* get_name() const { return "ulong"; }
    virtual void parse(uint8_t* buf, const std::string& data) const
    {
        unsigned long long i = std::stoull(data);
        memcpy(buf, &i, sizeof(unsigned long long));
    }
    virtual const std::string to_string(const uint8_t* buf) const
    {
        return std::to_string(*(unsigned long long*) buf);
    }
    virtual bool is_num_type() const { return true; }
    virtual type_constant_t get_type_constant() const { return ULONG_TYPE; }
    virtual double to_double(const uint8_t* buf) const
    {
        return (double) *(unsigned long long*) buf;
    }
    virtual unsigned long long to_ulong(const uint8_t* buf) const
    {
        return *(unsigned long long*) buf;
    }
};
    

class IntType : public Type {
public:
    uint64_t get_size() const { return sizeof(int); }
    const char* get_name() const { return "int"; }
    void parse(uint8_t* buf, const std::string& data) const
    {
        int i = string2int(data);
        memcpy(buf, &i, sizeof(int));
    }
    const std::string to_string(const uint8_t* buf) const
    {
        return std::to_string(*(int*) buf);
    }
    virtual double to_double(const uint8_t* buf) const
    {
        return (double) *(int*) buf;
    }
    bool is_num_type() const { return true; }
    type_constant_t get_type_constant() const { return INT_TYPE; }
};

class DoubleType : public Type {
public:
    virtual uint64_t get_size() const { return sizeof(double); }
    virtual const char* get_name() const { return "double"; }
    virtual void parse(uint8_t* buf, const std::string& data) const
    {
        double d = std::stod(data);
        memcpy(buf, &d, sizeof(float));
    }
    virtual const std::string to_string(const uint8_t* buf) const
    {
        return std::to_string(*(double*) buf);
    }
    virtual bool is_num_type() const { return true; }
    virtual type_constant_t get_type_constant() const { return DOUBLE_TYPE; }
    virtual double to_double(const uint8_t* buf) const
    {
        return (double) *(double*) buf;
    }
};

class FloatType : public Type {
public:
    uint64_t get_size() const { return sizeof(float); }
    const char* get_name() const { return "float"; }
    void parse(uint8_t* buf, const std::string& data) const
    {
        float f = string2float(data);
        memcpy(buf, &f, sizeof(float));
    }
    const std::string to_string(const uint8_t* buf) const
    {
        return std::to_string(*(float*) buf);
    }
    virtual double to_double(const uint8_t* buf) const
    {
        return (double) *(float*) buf;
    }
    bool is_num_type() const { return true; }
    type_constant_t get_type_constant() const { return FLOAT_TYPE; }
};

class StringType : public Type {
public:
    static const uint64_t MAX_STRING_LEN = 7;  // Not including NULL byte
    uint64_t get_size() const { return MAX_STRING_LEN + 1; }
    void parse(uint8_t* buf, const std::string& data) const
    {
        strncpy((char*) buf, data.c_str(), MAX_STRING_LEN);
        buf[MAX_STRING_LEN] = '\0';
    }
    const std::string to_string(const uint8_t* buf) const
    {
        return std::string((char*) buf);
    }
    bool is_num_type() const { return false; }
    type_constant_t get_type_constant() const { return STRING_TYPE; }
};

class UnknownType : public Type {
public:
    uint64_t get_size() const { panic("UnknownType has no size"); }
    const char* get_name() const { panic("UnknownType has no size"); }
    void parse(uint8_t* buf, const std::string& data) const
    { panic("UnknownType has no size"); }
    bool is_num_type() const { panic("UnknownType"); }
    type_constant_t get_type_constant() const
    { panic("UnknownType has no type constant"); }
};

class Attr {
public:
    enum bin_type_t {
        DISC_BIN,       // For discrete bins
        CONT_BIN        // For continuous bins
    };

    void set_name(const std::string& name) { this->name = name; }
    void set_type(Type* type) { this->type = type; }
    void set_offset(uint64_t offset) { this->offset = offset; }
    void set_cardinality(uint64_t cardinality) { this->cardinality = cardinality; }
    void set_min(double min) { this->min = min; }
    void set_max(double max) { this->max = max; }
    void set_bin_type(bin_type_t bin_type) { this->bin_type = bin_type; }

    const std::string& get_name() const { return name; }
    const Type* get_type() const { return type; }
    uint64_t get_offset() const { return offset; }
    uint64_t get_cardinality() const { return cardinality; }
    double get_min() const { return min; }
    double get_max() const { return max; }
    bin_type_t get_bin_type() const { return bin_type; }

protected:
    std::string name;
    Type* type;
    uint64_t offset;
    uint64_t cardinality;
    bin_type_t bin_type;
    /* FIXME - This assumes the attrs are number-based */
    double min;     // Only used for continuous bins to figure out bin width
    double max;     // Only used for continuous bins to figure out bin width

};

void read_types(std::vector<Attr>& attrs, const char* types_fname);

#endif /* _DB_UTILS_H_ */
