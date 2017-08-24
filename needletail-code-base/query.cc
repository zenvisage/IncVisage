#include <string>

#include <yaml-cpp/yaml.h>

#include "query.h"

using namespace std;

void QueryNode::parse_yaml(const YAML::Node& yaml_node, Schema& schema)
{
    if (yaml_node["__op__"]) {

        const std::string& oper_s = yaml_node["__op__"].as<string>();
        if (oper_s == "and") {
            this->oper = AND;
        } else if (oper_s == "or") {
            this->oper = OR;
        } else {
            panic("Unknown operation");
        }
        node_type = OPERATOR;

        YAML::Node nodes = yaml_node["__nodes__"];

       // cout<<"map"<<endl;
        for (YAML::const_iterator it = nodes.begin(); it != nodes.end(); it++) {
            cout<<"in map"<<endl;
            const std::string& key = it->first.as<std::string>();       // <- key
            cout<<key<< ","<< it->second.as<string>()<<endl;

            if(key=="d")
                continue;
            children.push_back(QueryNode());
            children.back().parse_yaml(*it, schema);
        }
        
        
    } else {
        
            const std::string& key = yaml_node.begin()->first.as<std::string>();
            if (schema.get_attrs_m().find(key) == schema.get_attrs_m().end()) {
                panic("Cannot find attr: %s\n", key.c_str());
            }
            attr = &schema.get_attrs_m().find(key)->second;
            const Type* type = attr->get_type();
            uint8_t val[MAX_TYPE_SIZE];
            /* FIXME - Make this more general */
            /* FIXME - Check to see if this parsing thing works. */
            type->parse(val, yaml_node.begin()->second.as<string>());
            node_type = SINGLETON;
            memcpy(this->val, val, MAX_TYPE_SIZE);
       
        

        
    }
}

void QueryNode::parse_yaml(const YAML::Node& yaml_node, Schema& schema,bool is_node_)
{
    if (yaml_node["__op__"]) {

        
        const std::string& oper_s = yaml_node["__op__"].as<string>();
        if (oper_s == "and") {
            this->oper = AND;
        } else if (oper_s == "or") {
            this->oper = OR;
        } else {
            panic("Unknown operation");
        }
        node_type = OPERATOR;

        YAML::Node nodes = yaml_node["__nodes__"];

       // cout<<"map"<<endl;
        for (YAML::const_iterator it = nodes.begin(); it != nodes.end(); it++) {
            const std::string& key = it->first.as<std::string>();       // <- key
           // cout<<key<< ","<< it->second.as<string>()<<endl;

            if(key=="d")
                continue;
           // children.push_back(QueryNode());
          //  cout << "hereeee" << endl;
          //  children.back().parse_yaml(*it, schema,true);
            QueryNode qNode;
            if (schema.get_attrs_m().find(key) == schema.get_attrs_m().end()) {
                panic("Cannot find attr: %s\n", key.c_str());
            }

            qNode.attr = &schema.get_attrs_m().find(key)->second;
            const Type* type = qNode.attr->get_type();
            uint8_t val[MAX_TYPE_SIZE];
            /* FIXME - Make this more general */
            /* FIXME - Check to see if this parsing thing works. */
            type->parse(val, it->second.as<string>());
            qNode.node_type = SINGLETON;
            memcpy(qNode.val, val, MAX_TYPE_SIZE);
            children.push_back(qNode);
            
        }
        
        
    } else {
        if(is_node_==false)
        {

            for(YAML::const_iterator it=yaml_node.begin();it != yaml_node.end();++it) {
                const std::string& key = it->first.as<std::string>();       // <- key
                //cout<<key<< ","<< it->second.as<string>()<<endl;

                if(key=="d")
                    continue;

                if (schema.get_attrs_m().find(key) == schema.get_attrs_m().end()) {
                    panic("Cannot find attr: %s\n", key.c_str());
                }
                attr = &schema.get_attrs_m().find(key)->second;
                const Type* type = attr->get_type();
                uint8_t val[MAX_TYPE_SIZE];
                /* FIXME - Make this more general */
                /* FIXME - Check to see if this parsing thing works. */
                type->parse(val, it->second.as<string>());
                node_type = SINGLETON;
                memcpy(this->val, val, MAX_TYPE_SIZE);
                            
            }
        }
        else
        {
            cout << "bait" << endl;
            const std::string& key = yaml_node.begin()->first.as<std::string>();
            cout << "taken" << endl;
            if (schema.get_attrs_m().find(key) == schema.get_attrs_m().end()) {
                panic("Cannot find attr: %s\n", key.c_str());
            }
            attr = &schema.get_attrs_m().find(key)->second;
            const Type* type = attr->get_type();
            uint8_t val[MAX_TYPE_SIZE];
            /* FIXME - Make this more general */
            /* FIXME - Check to see if this parsing thing works. */
            type->parse(val, yaml_node.begin()->second.as<string>());
            node_type = SINGLETON;
            memcpy(this->val, val, MAX_TYPE_SIZE);
        }
        

        
    }
}

//Query::Query()
//{}

Query::Query(const char* fname, Schema& schema)
    : targets(), root()
{
    YAML::Node query = YAML::LoadFile(fname);
    num_samples = query["num_samples"].as<uint64_t>();
    
    YAML::Node yaml_targets = query["targets"];
    for (YAML::const_iterator it = yaml_targets.begin(); it !=
            yaml_targets.end(); it++) {
        const std::string& key = it->as<string>();
        if (schema.get_attrs_m().find(key) == schema.get_attrs_m().end()) {
            panic("Cannot find target attr: %s\n", key.c_str());
        }
        targets.push_back(&schema.get_attrs_m().find(key)->second);
    }
    YAML::Node q = query["q"];
    root.parse_yaml(query["q"], schema);
}

Query::Query(YAML::Node& query, Schema& schema)
    : targets(), root()
{
   
    num_samples = query["num_samples"].as<uint64_t>();
    
    YAML::Node yaml_targets = query["targets"];
    for (YAML::const_iterator it = yaml_targets.begin(); it !=
            yaml_targets.end(); it++) {
        const std::string& key = it->as<string>();


        if (schema.get_attrs_m().find(key) == schema.get_attrs_m().end()) {
            panic("Cannot find target attr: %s\n", key.c_str());
        }
        targets.push_back(&schema.get_attrs_m().find(key)->second);
    }
    YAML::Node q = query["q"];
    root.parse_yaml(query["q"], schema,false);
}

