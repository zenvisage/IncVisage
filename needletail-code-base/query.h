#ifndef _QUERY_H_
#define _QUERY_H_

#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "db_utils.h"
#include "schema.h"

class QueryNode {
public:
    enum node_type_t {
        OPERATOR,
        SINGLETON
    };

    enum oper_t {
        AND,
        OR
    };

    QueryNode() : children() { }

    bool is_singleton() const { return node_type == SINGLETON; }
    bool is_and() const { return oper == AND; }
    const Attr* get_attr() const { return attr; }
    const uint8_t* get_val() const { return val; }
    const std::vector<QueryNode>& get_children() const { return children; }

    void parse_yaml(const YAML::Node& yaml_node, Schema& schema,bool is_node_);

    void parse_yaml(const YAML::Node& yaml_node, Schema& schema);

protected:
    node_type_t node_type;
    const Attr* attr;
    uint8_t val[MAX_TYPE_SIZE];
    oper_t oper;
    std::vector<QueryNode> children;
};

class Query {
public:
    Query(const char* fname, Schema& schema);
    Query(YAML::Node& query, Schema& schema);
  //  Query();
    const QueryNode& get_root() const { return root; }
    const std::vector<const Attr*>& get_targets() const { return targets; }

    uint64_t get_num_samples() const { return num_samples; }
    void set_num_samples(uint64_t num_samples) { this->num_samples=num_samples;  }

    void set_rand_num_samples(uint64_t num_samples) {this -> num_rand_samples = num_samples; }
    uint64_t get_rand_num_samples() const {return num_rand_samples;}

    size_t get_num_terms() const { return num_terms; }
    void set_num_terms(size_t num_terms) { this->num_terms=num_terms;  }

    void set_sample_rate(double rate) {this -> sample_rate = rate; }
    double get_sample_rate() const { return sample_rate; }
    
    int get_trial() const{return trial;}
    void set_trial(int t){this-> trial = t;}   

    int get_dataset() const{return dataset_num;}
    void set_dataset(int num) {this-> dataset_num = num;}

    void set_dataset_name(std::string name) {this -> dataset_name = name;}
    std::string get_dataset_name() const{return this -> dataset_name;}

    void set_conv_rate(double conv_rate){this->conv_rate = conv_rate;}
    double get_conv_rate() const{return conv_rate;}

protected:
    
    uint64_t num_samples;
    uint64_t num_rand_samples;
    size_t num_terms;
    int trial;
    int dataset_num;
    double sample_rate;
    double conv_rate; //percentage of conv sampling
    std::string dataset_name;

    std::vector<const Attr*> targets;
    QueryNode root;
};

class ResultSet {
public:
    enum exit_code_t {
        OK,
        NO_VALID_RECORDS
    };

    void set_exit_code(exit_code_t exit_code) { this->exit_code = exit_code; }

    void update(size_t idx, double val)
    {
        num_samples[idx]++;
        avgs[idx] += (val - avgs[idx]) / num_samples[idx];
    }

    void update_esti_results(size_t idx, double sum, double unb, double ratio){
	est_sum[idx] = sum;
	unb_avg[idx] = unb;
	ratio_avg[idx] = ratio;
   
    }
    
    void clear_vals(){
        std::fill(num_samples.begin(), num_samples.end(), 0);
        std::fill(avgs.begin(), avgs.end(), 0);
	std::fill(est_sum.begin(), est_sum.end(), 0.0);
	std::fill(unb_avg.begin(), unb_avg.end(), 0.0);
	std::fill(ratio_avg.begin(), ratio_avg.end(), 0.0);
    }

    void set_io_time(uint64_t io_time) {this->total_io_time += io_time;}
    uint64_t get_io_time() const{return total_io_time;}
   
    void set_rand_blocks(double num_blocks) {this -> num_rand_blocks = num_blocks;}
    double get_rand_blocks() const{ return this -> num_rand_blocks;} 

    void set_samples_file(std::ofstream& file){this->samples_file = &file;} 
    std::ofstream*& get_sample_file() {return samples_file;}
 
    const std::vector<uint64_t>& get_num_samples() const { return num_samples; }
    const std::vector<double>& get_avgs() const { return avgs; }

    const std::vector<double>& get_est_sum() const { return est_sum; }
    const std::vector<double>& get_unb_avgs() const { return unb_avg; }
    const std::vector<double>& get_ratio_avgs() const { return ratio_avg; }


    ResultSet(size_t num_avgs)
        : exit_code(OK), num_samples(), avgs()
    {
        for (size_t i = 0; i < num_avgs; i++) {
            num_samples.push_back(0);
            avgs.push_back(0);
	    est_sum.push_back(0.0);
            unb_avg.push_back(0.0);
	    ratio_avg.push_back(0.0);
        }
    }

protected:
    exit_code_t exit_code;
    std::vector<uint64_t> num_samples;
    std::vector<double> avgs;
    uint64_t total_io_time = 0;
    double num_rand_blocks = 0.0;
    std::ofstream *samples_file;

    std::vector<double> est_sum;
    std::vector<double> unb_avg;
    std::vector<double> ratio_avg;

};

#endif /* _QUERY_H_ */

