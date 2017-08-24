#include <cstdlib>

#include "db_utils.h"
#include "binner.h"

using namespace std;

Binner::key_t Binner::get_key(const Attr& attr, const uint8_t* val)
{
    /* TODO - Check to see how long boost::hash takes. If it takes too long,
     * consider moving to a simpler hashing scheme. */
    const Type* type = attr.get_type();
    if (attr.get_bin_type() == Attr::DISC_BIN) {
        /* TODO - Revert to this code when we want this to be more robust.
         * Otherwise, for now, assume we will only get int types for DISC_BIN
         * type. */
#if 0
        if (type->is_num_type()) {
            return double_hasher(type->to_double(val));
        } else if (type->get_type_constant() == STRING_TYPE) {
            return string_hasher(type->to_string(val));
        } else {
            panic("Cannot yet bin non-numeric, non-string discrete attribute.");
        }
#endif
        if (type->get_type_constant() == ULONG_TYPE) {
            return (size_t) type->to_ulong(val);
        } else {
            panic("Cannot yet bin non-numeric, non-int discrete attribute.");
        }

    } else if (attr.get_bin_type() == Attr::CONT_BIN) {
        if (type->is_num_type()) {
            /* FIXME - This only handles cases where bin size is constant and
             * min/max values make sense. */
            double bin_width = (attr.get_max() - attr.get_min()) / attr.get_cardinality();
            double key_dbl = ((type->to_double(val) - attr.get_min()) / bin_width);
            size_t key;
            if (key_dbl < 0) {
                key = 0;
            } else if (key_dbl > (attr.get_cardinality() - 1)) {
                key = attr.get_cardinality() - 1;
            } else {
                key = (size_t) key_dbl;
            }
            return key;
        } else {
            panic("Cannot yet bin non-numeric continuous attribute.");
        }
    } else {
        panic("Do not support other types of attributes");
    }
}
