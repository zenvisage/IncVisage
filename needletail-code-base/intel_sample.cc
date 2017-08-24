#if 0
#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>

#include "intel_sample.h"
#include "utils.h"

using namespace std;

ConfidenceInterval::ConfidenceInterval(uint64_t group, double mean, double epsilon, double delta)
    : group(group), mean(mean), epsilon(epsilon), delta(delta),
    low(mean - epsilon), high(mean + epsilon), num_samples(0), resolved(false)
{ }

void ConfidenceInterval::update(double sample_mean, uint64_t num_new_samples, double epsilon)
{
    mean = ((mean * num_samples) + (sample_mean * num_new_samples)) /
        (num_samples + num_new_samples);
    low = mean - epsilon;
    high = mean + epsilon;
    num_samples += num_new_samples;

    this->epsilon = epsilon;
}

void ConfidenceInterval::resolve()
{
    epsilon = 0;
    resolved = true;
}

bool ConfidenceInterval::conflict(ConfidenceInterval* rhs)
{
    if (resolved || rhs->resolved) {
        return false;
    }

    double max_epsilon = max(epsilon, rhs->epsilon);
    if ((mean + max_epsilon) > (rhs->mean - epsilon)) {
        return true;
    } else {
        return false;
    }
}

IntelSampleState::IntelSampleState(uint64_t num_groups, uint64_t span, double delta)
    : num_groups(num_groups), span(span), delta(delta), intervals(),
    sorted_intervals()
{
    for (uint64_t g = 0; g < num_groups; g++) {
        double start_delta = delta / (2 * num_groups);
        intervals[g] = new ConfidenceInterval(g, span/2, span/2, start_delta);
        sorted_intervals.push_back(intervals[g]);
    }
}

IntelSampleState::~IntelSampleState()
{
    for (uint64_t g = 0; g < num_groups; g++) {
        delete intervals[g];
    }
}

void IntelSampleState::update(uint64_t group, double sample_mean, uint64_t
        num_new_samples, double epsilon)
{
    intervals[group]->update(sample_mean, num_new_samples, epsilon);
}

void IntelSampleState::resolve(uint64_t group)
{
    intervals[group]->resolve();
}

uint64_t IntelSampleState::get_conflicts(conflictmap& conflicts)
{
    sort(sorted_intervals.begin(), sorted_intervals.end(),
            ConfidenceInterval::PointerCompare());

    /* Debbuging print statements
    printf("sorted_intervals = [");
    for (size_t i = 0; i < sorted_intervals.size(); i++) {
        printf("(%lu: %f %f) ", sorted_intervals[i]->get_group(),
                sorted_intervals[i]->get_low(),
                sorted_intervals[i]->get_high());
    }
    printf("]\n");
    */

    conflicts.clear();
    for (uint64_t g = 0; g < num_groups; g++) {
        conflicts[g] = false;
    }

    for (unsigned int i = 0; i < sorted_intervals.size() - 1; i++) {
        unsigned int j = i + 1;
        if (sorted_intervals[i]->conflict(sorted_intervals[j])) {
            conflicts[sorted_intervals[i]->get_group()] = true;
            conflicts[sorted_intervals[j]->get_group()] = true;
        }
    }

    uint64_t num_conflicts = 0;
    for (conflictmap::iterator it = conflicts.begin();
            it != conflicts.end();
            it++) {
        if (it->second) {
            num_conflicts++;
        }
    }

    return num_conflicts;
}

void IntelSampleState::get_results(struct alg_results& results)
{
    results.total_num_samples = 0;
    for (uint64_t g = 0; g < num_groups; g++) {
        results.num_samples[g] = intervals[g]->get_num_samples();
        results.total_num_samples += results.num_samples[g];
        results.means[g] = intervals[g]->get_mean();
    }
}

void IntelSampleState::print_debug_state()
{
    printf("resolved = [");
    for (uint64_t g = 0; g < num_groups; g++) {
        printf("%s ", intervals[g]->get_resolved() ? "true" : "false");
    }
    printf("]\n");

    printf("num_samples = [");
    for (uint64_t g = 0; g < num_groups; g++) {
        printf("%lu ", intervals[g]->get_num_samples());
    }
    printf("]\n");

    printf("intervals = [");
    for (uint64_t g = 0; g < num_groups; g++) {
        printf("(%f %f) ", intervals[g]->get_low(), intervals[g]->get_high());
    }
    printf("]\n");
}

void scan(SDB& sdb, struct alg_args& args, struct alg_results& results)
{
    for (uint64_t g = 0; g < args.num_groups; g++) {
        results.means[g] = 0;
        results.num_samples[g] = 0;
    }

    for (uint64_t row = 0; row < sdb.get_num_records(); row++) {
        uint64_t g = (uint64_t) *(int*) sdb.get_val(row, "group");
        double record_val = (double) *(float*) sdb.get_val(row, "val");
        results.means[g] += (record_val - results.means[g]) /
            (results.num_samples[g] + 1);
        results.num_samples[g]++;
    }

    results.total_num_samples = 0;
    for (uint64_t g = 0; g < args.num_groups; g++) {
        results.total_num_samples += results.num_samples[g];
    }
}

void ifocus(SwiftIndex& swi, struct alg_args& args, struct alg_results& results)
{
    IntelSampleState state(args.num_groups, args.span, args.delta);

    conflictmap conflicts;
    SwiftIndex::constraintmap constraints;
    while (state.get_conflicts(conflicts)) {
        for (uint64_t g = 0; g < args.num_groups; g++) {
            if (!conflicts[g]) {
                continue;
            }

            uint64_t num_new_samples = args.total_sizes[g] - state.get_num_samples(g);
            num_new_samples = min(num_new_samples, ROUND_SIZE);
            double m = (double) state.get_num_samples(g) + num_new_samples;

            double epsilon = args.span * sqrt(
                    (1 - (m - 1) / args.total_sizes[g]) *
                    (2 * log(log(m)) +
                     log(M_PI * M_PI * args.num_groups / (3 * args.delta))) /
                    (2 * m));

            constraints["group"] = (uint8_t*) &g;
            double sample_mean = swi.ordered_sample_mean("val", constraints, num_new_samples);

            state.update(g, sample_mean, num_new_samples, epsilon);

            if ((uint64_t) m >= args.total_sizes[g]) {
                state.resolve(g);
            }
        }

        conflictmap prev_conflicts = conflicts;
        state.get_conflicts(conflicts);

        for (uint64_t g = 0; g < args.num_groups; g++) {
            if (prev_conflicts[g] && !conflicts[g]) {
                state.resolve(g);
            }
        }

        //state.print_debug_state();
    }

    state.get_results(results);
}

void ifocusr(SwiftIndex& swi, struct alg_args& args, struct alg_results& results)
{
    IntelSampleState state(args.num_groups, args.span, args.delta);

    conflictmap conflicts;
    SwiftIndex::constraintmap constraints;
    while (state.get_conflicts(conflicts)) {
        for (uint64_t g = 0; g < args.num_groups; g++) {
            if (!conflicts[g]) {
                continue;
            }

            uint64_t num_new_samples = args.total_sizes[g] - state.get_num_samples(g);
            num_new_samples = min(num_new_samples, ROUND_SIZE);
            double m = (double) state.get_num_samples(g) + num_new_samples;

            double epsilon = args.span * sqrt(
                    (1 - (m - 1) / args.total_sizes[g]) *
                    (2 * log(log(m)) +
                     log(M_PI * M_PI * args.num_groups / (3 * args.delta))) /
                    (2 * m));

            constraints["group"] = (uint8_t*) &g;
            double sample_mean = swi.ordered_sample_mean("val", constraints, num_new_samples);

            state.update(g, sample_mean, num_new_samples, epsilon);

            if ((uint64_t) m >= args.total_sizes[g]) {
                state.resolve(g);
            }

            if (epsilon < args.minres * args.span / 4) {
                state.resolve(g);
            }
        }

        conflictmap prev_conflicts = conflicts;
        state.get_conflicts(conflicts);

        for (uint64_t g = 0; g < args.num_groups; g++) {
            if (prev_conflicts[g] && !conflicts[g]) {
                state.resolve(g);
            }
        }

        //state.print_debug_state();
    }

    state.get_results(results);
}

void irefine(SwiftIndex& swi, struct alg_args& args, struct alg_results& results)
{
    IntelSampleState state(args.num_groups, args.span, args.delta);

    conflictmap conflicts;
    SwiftIndex::constraintmap constraints;
    uint64_t i = 0;
    uint64_t num_conflicts;
    while ((num_conflicts = state.get_conflicts(conflicts))) {
        for (uint64_t g = 0; g < args.num_groups; g++) {
            if (!conflicts[g]) {
                continue;
            }

            double epsilon = state.get_epsilon(g) / 2;
            double delta = args.delta / ((double) pow(2, i + 1) * num_conflicts);
            double m = ceil(pow(args.span, 2) / (2 * pow(epsilon, 2)) * log(2 / delta));
            uint64_t num_new_samples = min(args.total_sizes[g], (uint64_t) m) -
                state.get_num_samples(g);

            constraints["group"] = (uint8_t*) &g;
            double sample_mean = swi.ordered_sample_mean("val", constraints, num_new_samples);

            state.update(g, sample_mean, num_new_samples, epsilon);

            if ((uint64_t) m >= args.total_sizes[g]) {
                state.resolve(g);
            }
        }

        conflictmap prev_conflicts = conflicts;
        state.get_conflicts(conflicts);

        for (uint64_t g = 0; g < args.num_groups; g++) {
            if (prev_conflicts[g] && !conflicts[g]) {
                state.resolve(g);
            }
        }

        //state.print_debug_state();

        i++;
    }

    state.get_results(results);
}

void irefiner(SwiftIndex& swi, struct alg_args& args, struct alg_results& results)
{
    IntelSampleState state(args.num_groups, args.span, args.delta);

    conflictmap conflicts;
    SwiftIndex::constraintmap constraints;
    uint64_t i = 0;
    uint64_t num_conflicts;
    while ((num_conflicts = state.get_conflicts(conflicts))) {
        for (uint64_t g = 0; g < args.num_groups; g++) {
            if (!conflicts[g]) {
                continue;
            }

            double epsilon = state.get_epsilon(g) / 2;
            double delta = args.delta / ((double) pow(2, i + 1) * num_conflicts);
            double m = ceil(pow(args.span, 2) / (2 * pow(epsilon, 2)) * log(2 / delta));
            uint64_t num_new_samples = min(args.total_sizes[g], (uint64_t) m) -
                state.get_num_samples(g);

            constraints["group"] = (uint8_t*) &g;
            double sample_mean = swi.ordered_sample_mean("val", constraints, num_new_samples);

            state.update(g, sample_mean, num_new_samples, epsilon);

            if ((uint64_t) m >= args.total_sizes[g]) {
                state.resolve(g);
            }

            if (epsilon < args.minres * args.span / 4) {
                state.resolve(g);
            }
        }

        conflictmap prev_conflicts = conflicts;
        state.get_conflicts(conflicts);

        for (uint64_t g = 0; g < args.num_groups; g++) {
            if (prev_conflicts[g] && !conflicts[g]) {
                state.resolve(g);
            }
        }

        //state.print_debug_state();

        i++;
    }

    state.get_results(results);
}

void iroundrobin(SwiftIndex& swi, struct alg_args& args, struct alg_results& results)
{
    IntelSampleState state(args.num_groups, args.span, args.delta);

    conflictmap conflicts;
    SwiftIndex::constraintmap constraints;
    while (state.get_conflicts(conflicts)) {
        for (uint64_t g = 0; g < args.num_groups; g++) {

            uint64_t num_new_samples = args.total_sizes[g] - state.get_num_samples(g);
            num_new_samples = min(num_new_samples, ROUND_SIZE);
            double m = (double) state.get_num_samples(g) + num_new_samples;

            double epsilon = args.span * sqrt(
                    (1 - (m - 1) / args.total_sizes[g]) *
                    (2 * log(log(m)) +
                     log(M_PI * M_PI * args.num_groups / (3 * args.delta))) /
                    (2 * m));

            constraints["group"] = (uint8_t*) &g;
            double sample_mean = swi.ordered_sample_mean("val", constraints, num_new_samples);

            state.update(g, sample_mean, num_new_samples, epsilon);

            if ((uint64_t) m >= args.total_sizes[g]) {
                state.resolve(g);
            }
        }

        conflictmap prev_conflicts = conflicts;
        state.get_conflicts(conflicts);

        for (uint64_t g = 0; g < args.num_groups; g++) {
            if (prev_conflicts[g] && !conflicts[g]) {
                state.resolve(g);
            }
        }

        //state.print_debug_state();
    }

    state.get_results(results);
}

void iroundrobinr(SwiftIndex& swi, struct alg_args& args, struct alg_results& results)
{
    IntelSampleState state(args.num_groups, args.span, args.delta);

    conflictmap conflicts;
    SwiftIndex::constraintmap constraints;
    while (state.get_conflicts(conflicts)) {
        for (uint64_t g = 0; g < args.num_groups; g++) {

            uint64_t num_new_samples = args.total_sizes[g] - state.get_num_samples(g);
            num_new_samples = min(num_new_samples, ROUND_SIZE);
            double m = (double) state.get_num_samples(g) + num_new_samples;

            double epsilon = args.span * sqrt(
                    (1 - (m - 1) / args.total_sizes[g]) *
                    (2 * log(log(m)) +
                     log(M_PI * M_PI * args.num_groups / (3 * args.delta))) /
                    (2 * m));

            constraints["group"] = (uint8_t*) &g;
            double sample_mean = swi.ordered_sample_mean("val", constraints, num_new_samples);

            state.update(g, sample_mean, num_new_samples, epsilon);

            if ((uint64_t) m >= args.total_sizes[g]) {
                state.resolve(g);
            }

            if (epsilon < args.minres * args.span / 4) {
                state.resolve(g);
            }
        }

        conflictmap prev_conflicts = conflicts;
        state.get_conflicts(conflicts);

        for (uint64_t g = 0; g < args.num_groups; g++) {
            if (prev_conflicts[g] && !conflicts[g]) {
                state.resolve(g);
            }
        }

        //state.print_debug_state();
    }

    state.get_results(results);
}

void deploy_alg(SDB& sdb, SwiftIndex& swi, const char* alg_name, struct alg_args& args)
{
    string s(alg_name);
    struct alg_results results;

    INIT_TIMER(intel_sample);
    clear_io_timer();

    if (s == "ifocus") {
        START_TIMER(intel_sample);
        ifocus(swi, args, results);
        END_TIMER(intel_sample);
    } else if (s == "ifocusr") {
        START_TIMER(intel_sample);
        ifocusr(swi, args, results);
        END_TIMER(intel_sample);
    } else if (s == "irefine") {
        START_TIMER(intel_sample);
        irefine(swi, args, results);
        END_TIMER(intel_sample);
    } else if (s == "irefiner") {
        START_TIMER(intel_sample);
        irefiner(swi, args, results);
        END_TIMER(intel_sample);
    } else if (s == "iroundrobin") {
        START_TIMER(intel_sample);
        iroundrobin(swi, args, results);
        END_TIMER(intel_sample);
    } else if (s == "iroundrobinr") {
        START_TIMER(intel_sample);
        iroundrobinr(swi, args, results);
        END_TIMER(intel_sample);
    } else if (s == "scan") {
        START_TIMER(intel_sample);
        scan(sdb, args, results);
        END_TIMER(intel_sample);
    }

    printf("time_taken_s = %lu\n", (uint64_t) round((double) GET_TIMER(intel_sample) / 1000000));
    printf("time_taken_us = %lu\n", GET_TIMER(intel_sample));
    printf("io_time_taken_s = %lu\n", (uint64_t) round((double) get_io_timer() / 1e6));
    printf("io_time_taken_us = %lu\n", get_io_timer());
    printf("total_num_samples = %lu\n", results.total_num_samples);
    printf("num_samples = [");
    for (uint64_t g = 0; g < args.num_groups; g++) {
        printf("%lu, ", results.num_samples[g]);
    }
    printf("]\n");
    printf("means = [");
    for (uint64_t g = 0; g < args.num_groups; g++) {
        printf("%f, ", results.means[g]);
    }
    printf("]\n");
}

void get_alg_args(struct alg_args& args, const char* meta_fname, double delta)
{
    /* Constant things */
    args.delta = delta;
    args.span = 100;
    args.minres = 0.01;

    ifstream meta_f(meta_fname);
    string line;
    stringstream ss;
    string token;

    while (getline(meta_f, line)) {
        ss.str(line);
        getline(ss, token, ' ');
        if (token == "num_groups") {
            getline(ss, token, ' ');        // For '='
            getline(ss, token, ' ');        // For actual num_groups
            args.num_groups = string2uint64_t(token);
        } else if (token == "data_sizes") {
            getline(ss, token, ' ');        // For '='
            uint64_t g = 0;
            while (getline(ss, token, ' ')) {
                if (token.front() == '[') {
                    token.erase(0, 1);
                } else if (token.back() == ',') {
                    token.erase(token.length() - 1, 1);
                } else if (token.back() == ']') {
                    token.erase(token.length() - 1, 1);
                }
                args.total_sizes[g] = string2uint64_t(token);
                g++;
            }
            assert(args.num_groups == args.total_sizes.size());
        }
        ss.clear();
    }
}
#endif
