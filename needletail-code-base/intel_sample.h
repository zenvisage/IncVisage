#ifndef _INTEL_SAMPLE_H_
#define _INTEL_SAMPLE_H_

#include <cinttypes>
#include <string>
#include <vector>

#include <boost/unordered_map.hpp>

#include "sdb.h"
#include "swift_index.h"

const uint64_t ROUND_SIZE = 1000;

struct alg_args {
    uint64_t num_groups;
    boost::unordered_map<uint64_t,uint64_t> total_sizes;
    double span;
    double delta;
    double minres;          // Only meaningful for *r algorithms
};

struct alg_results {
    uint64_t total_num_samples;
    boost::unordered_map<uint64_t,uint64_t> num_samples;
    boost::unordered_map<uint64_t,double> means;
};

void get_alg_args(struct alg_args& args, const char* meta_fname, double delta);

void deploy_alg(SDB& sdb, SwiftIndex& swi, const char* alg_name, struct alg_args& args);

void scan(SDB& sdb, struct alg_args& args, struct alg_results& results);

void ifocus(SwiftIndex& swi, struct alg_args& args, struct alg_results& results);
void ifocusr(SwiftIndex& swi, struct alg_args& args, struct alg_results& results);
void irefine(SwiftIndex& swi, struct alg_args& args, struct alg_results& results);
void irefiner(SwiftIndex& swi, struct alg_args& args, struct alg_results& results);
void iroundrobin(SwiftIndex& swi, struct alg_args& args, struct alg_results& results);
void iroundrobinr(SwiftIndex& swi, struct alg_args& args, struct alg_results& results);

typedef boost::unordered_map<uint64_t,bool> conflictmap;

class ConfidenceInterval {
public:
    ConfidenceInterval(uint64_t group, double mean, double epsilon, double delta);

    struct PointerCompare {
        bool operator() (const ConfidenceInterval* l, const ConfidenceInterval*
                r)
        {
            return l->low < r->low;
        }
    };

    void update(double sample_mean, uint64_t num_new_samples, double epsilon);

    void resolve();

    bool conflict(ConfidenceInterval* rhs);

    uint64_t get_num_samples() const { return num_samples; }
    double get_mean() const { return mean; }
    double get_low() const { return low; }
    double get_high() const { return high; }
    bool get_resolved() const { return resolved; }
    uint64_t get_group() const { return group; }
    double get_epsilon() const { return epsilon; }

protected:
    uint64_t group;
    double mean;
    double epsilon;
    double delta;
    double low;
    double high;
    uint64_t num_samples;
    bool resolved;
};


class IntelSampleState {
public:
    IntelSampleState(uint64_t num_groups, uint64_t span, double delta);
    ~IntelSampleState();

    void update(uint64_t group, double sample_mean, uint64_t num_new_samples,
            double epsilon);

    void resolve(uint64_t group);

    /* Returns number of conflicts. */
    uint64_t get_conflicts(conflictmap& conflicts);

    uint64_t get_num_samples(uint64_t group)
    {
        return intervals[group]->get_num_samples();
    }

    double get_epsilon(uint64_t group)
    {
        return intervals[group]->get_epsilon();
    }

    void get_results(struct alg_results& results);

    void print_debug_state();

protected:
    uint64_t num_groups;
    uint64_t span;
    double delta;
    boost::unordered_map<uint64_t,ConfidenceInterval*> intervals;
    std::vector<ConfidenceInterval*> sorted_intervals;
};

#endif /* _INTEL_SAMPLE_H_ */
