#include <fstream>
#include <pthread.h>
#include <vector>
#include <iterator>
#include <utility>
#include <cstdlib>
#include <sstream>

#include "config.h"
#include "swift_index.h"

using namespace std;

static inline uint8_t* alloc_attr_val(vector<uint8_t>& attr_buf, const Type* type)
{
    attr_buf.insert(attr_buf.end(), type->get_size(), 0);
    return &attr_buf[attr_buf.size() - type->get_size()];
}

SwiftIndex::SwiftIndex(SDB& sdb, Schema& schema, Binner& binner,
        SwiftMapFactory::map_type_t bmap_type, const char* fname)
    : sdb(sdb), schema(schema), binner(binner), map_factory(bmap_type, sdb.records_per_buf())
{

    if (fname) {
        init(fname);
    } else {
        init();
    }

}


SwiftIndex::~SwiftIndex(){

	//delete this;
	for (l1map::iterator it = index.begin(); it != index.end(); it++) {
	        for (l2map::iterator jt = it->second.begin(); jt != it->second.end();
	                jt++) {
	            map_factory.destroy_swiftmap(jt->second);
            map_factory.destroy_tmp();

	     }
	}
	 index.clear();
}


void SwiftIndex::init(const char* fname)
{
    ifstream in(fname);
    string line, name;

    while (in.peek() != EOF) {
        std::cout << "init create" << std::endl;
        // Get attr name
        getline(in, name);
        index.insert(make_pair(name, l2map()));
      //  cout << "name" << name;
        // Get attr val
        getline(in, line);
        Binner::key_t key;
        stringstream(line) >> key;
      //  cout << "key" << key;
        // FIXME - CHANGE TO ARB BITMAP
        SwiftMap* m = map_factory.create_map(schema.get_num_rows(), false, false);
        m->read(in);
        index[name].insert(make_pair(key, m));

    }

    in.close();
}
#if 0

        stringstream ss(line);

        getline(ss, name, ',');
        const Attr& attr = attrs.at(name);
        const Type* type = attr.get_type();
        index.insert(make_pair(name, l2map()));
        /* FIXME - A speed hack which can be used only if we know the
         * cardinality ahead of time. */
        while (getline(ss, token, ',')) {
            type->parse(val, token);
            Binner::key_t key = binner.get_key(attr, val);
            // FIXME - CHANGE TO ARB BITMAP
            index[name].insert(make_pair(key, new SingleBitmap(num_records, false)));
        }
    }

    for (l1map::iterator it = index.begin(); it != index.end(); it++) {
        l2map& attrvalmap = it->second;
        for (l2map::iterator jt = attrvalmap.begin(); jt != attrvalmap.end();
                jt++) {
#if USE_EWAH
            jt->second->read(f);
#else
            cout << it->first << " " << jt->first << endl;
            jt->second->read(f);
#endif
        }
    }

    f.close();
#endif


void SwiftIndex::save(const char* fname)
{
    ofstream out(fname, ios::out);

    for (l1map::iterator it = index.begin(); it != index.end(); it++) {
        const string& name = it->first;
        for (l2map::iterator jt = it->second.begin(); jt != it->second.end();
                jt++) {
            out << name << endl << jt->first << endl;
            jt->second->write(out);
        }
    }

#if 0
    for (l1map::iterator it = index.begin(); it != index.end(); it++) {
        for (l2map::iterator jt = it->second.begin(); jt != it->second.end();
                jt++) {
#if USE_EWAH
            jt->second->write(f);
#else
            cout << it->first << " " << jt->first << endl;
            jt->second->write(f);
#endif
        }
    }
#endif
    out.close();
}

struct threaded_read_arg {
    uint32_t thread_id;
    uint32_t num_threads;
    SDB* sdb;
    SwiftIndex::l1map* index;
    SwiftIndex::lockmap* index_lock;
};

static void* threaded_read(void* arg)
{
#if 0
    struct threaded_read_arg* a = (struct threaded_read_arg*) arg;
    uint32_t thread_id = a->thread_id;
    uint32_t num_threads = a->num_threads;
    SDB* sdb = a->sdb;
    l1swiftmap* index = a->index;

    FILE* f = fopen(sdb->get_fname(), "r");
    uint8_t buf[sdb->get_record_size()];

    uint64_t rows_per_thread = sdb->get_num_records() / num_threads;
    uint64_t row_begin = thread_id * rows_per_thread;
    fseek(f, row_begin * sdb->get_record_size(), SEEK_SET);

    for (uint64_t row = row_begin; row < (thread_id+1) * rows_per_thread &&
            row < sdb->get_num_records(); row++) {
        assert(fread(buf, sdb->get_record_size(), 1, f) == 1);

        uint64_t so_far = 0;
        for (uint64_t col = 0; col < sdb->get_num_attrs(); col++) {
            /* FIXME - we can only handle one column attributes that are
             * guaranteed to be ints now. */
            if (col != 0) {
                continue;
            }

            string attr = sdb->get_attrs()[col].get_name();
            /* FIXME - we can only handle one column attributes that are
             * guaranteed to be ints now. */
            int attr_val = *(int*) &buf[so_far];
            (*(*index)[attr][attr_val])[row] = true;
            so_far += sdb->get_attrs()[col].get_type()->get_size();
        }
    }

    fclose(f);

    return NULL;

    /* NOTE - The following uses the more general get_record interface of sdb
     * but is slower, likely due to the number of function calls that need to be
     * made, so we opt to use the hacky interface above instead.
     * NOTE - Actually, this turns out to be faster when use DIRECT_IO backend
     * for sdb, so let's just use this. */
    struct threaded_read_arg* a = (struct threaded_read_arg*) arg;
    uint32_t thread_id = a->thread_id;
    uint32_t num_threads = a->num_threads;
    SDB* sdb = a->sdb;
    SwiftIndex::l1map* index = a->index;
    SwiftIndex::lockmap* index_lock = a->index_lock;


    uint64_t rows_per_thread = sdb->get_num_records() / num_threads;
    uint64_t row_begin = thread_id * rows_per_thread;

    for (uint64_t row = row_begin; row < (thread_id+1) * rows_per_thread &&
            row < sdb->get_num_records(); row++) {
        const uint8_t* buf = sdb->get_record(row, thread_id);

        const vector<Attr>& ordered_attrs = sdb->get_ordered_attrs();
        uint64_t so_far = 0;
        for (uint64_t col = 0; col < ordered_attrs.size(); col++) {
            const Attr& attr = ordered_attrs[col];
            const string& name = attr.get_name();
            const Type* type = attr.get_type();
            if (attr.get_cardinality() == 0) {
                so_far += type->get_size();
                continue;
            }
            pthread_mutex_lock(&(*index_lock)[name]);
            /* TODO - Update with binner */
            if ((*index)[name].find(&buf[so_far]) == (*index)[name].end()) {
                uint8_t* attr_val_ptr = alloc_attr_val((*l2_buf)[name], type);
                memcpy(attr_val_ptr, &buf[so_far], type->get_size());
                add_new_bitmap((*index)[name], attr_val_ptr, sdb->get_num_records());
            }
            pthread_mutex_unlock(&(*index_lock)[name]);
#if USE_EWAH
            (*index)[attr.get_name()][&buf[so_far]]->set(row);
#else
            (*(*index)[attr.get_name()][&buf[so_far]])[row] = true;
#endif
            so_far += type->get_size();

#if 0
            int attr_val = *(int*) &buf[so_far];
            (*(*index)[attr.get_name()][attr_val])[row] = true;
            so_far += attr.get_type()->get_size();
#endif
        }
    }

    return NULL;
#endif
    return NULL;
}

void SwiftIndex::read()
{
    const vector<Attr>& attrs = schema.get_attrs_v();
    for (uint64_t row = 0; row < schema.get_num_rows(); row++) {
        const uint8_t* record = sdb.get_record(row);
        for (uint64_t col = 0; col < attrs.size(); col++) {
            const Attr& attr = attrs[col];
            if (attr.get_cardinality() == 0) {
                continue;
            }

            const string& name = attr.get_name();
            const uint8_t* val = &record[attr.get_offset()];
            Binner::key_t key = binner.get_key(attr, val);
            if (index[name].find(key) == index[name].end()) {
                SwiftMap* m = map_factory.create_map(schema.get_num_rows(),
                        false, false);
                index[name].insert(make_pair(key, m));
            }
            index[name][key]->set(row);

            int groupVal =  key;
            //cout << name << "," << groupVal <<endl;;
            if(perGroupValCount.find(name)==perGroupValCount.end()) // dim not found
            {
                perGroupValCount[name].insert(std::make_pair(groupVal, 1));
                
            }
            else
            {
                if(perGroupValCount[name].find(groupVal)==perGroupValCount[name].end()) // dim not found
                {
                     perGroupValCount[name].insert(std::make_pair(groupVal, 1));
                }
                else
                {
                    uint64_t count = perGroupValCount[name][groupVal];

                    count++;

                    perGroupValCount[name][groupVal] = count;
                }
            }
        }
    }

    for (l1map::iterator it = index.begin(); it != index.end(); it++) {
        for (l2map::iterator jt = it->second.begin(); jt != it->second.end();
                jt++) {
            jt->second->post_set();
        }
    }

    cout << "index set" << endl;

   sdb.clear();





#if 0
    FILE* f = direct_fopen(sdb.get_fname(), "r");
    uint8_t* buf;
    int64_t buf_size = DISK_READ_SIZE;
    uint64_t record_size = sdb.get_record_size();
    uint8_t tmp_buf[record_size];
    uint64_t extra_offset = 0;

    posix_memalign((void**) &buf, DISK_BLOCK_SIZE, buf_size);

    size_t num_read;

    uint64_t num_records = sdb.get_num_records();
    const vector<Attr>& ordered_attrs = sdb.get_ordered_attrs();

    uint64_t row = 0;
    while ((num_read = fread(buf, 1, buf_size, f)) > 0) {
        for (int64_t row_begin = -extra_offset; row_begin < buf_size && row <
                num_records; row_begin += record_size) {

            uint8_t* cur_buf;
            if (row_begin < 0) {
                memcpy(&tmp_buf[extra_offset], buf, record_size - extra_offset);
                extra_offset = 0;
                cur_buf = tmp_buf;
            } else if (row_begin + (int64_t) record_size > buf_size) {
                extra_offset = buf_size - row_begin;
                memcpy(tmp_buf, &buf[row_begin], extra_offset);
                continue;
            } else {
                cur_buf = &buf[row_begin];
            }


            uint64_t so_far = 0;
            for (uint64_t col = 0; col < ordered_attrs.size(); col++) {
                const Attr& attr = ordered_attrs[col];
                const string& name = attr.get_name();
                const Type* type = attr.get_type();
                if (attr.get_cardinality() == 0) {
                    so_far += type->get_size();
                    continue;
                }

                uint8_t* val = &cur_buf[so_far];
                Binner::key_t key = binner.get_key(attr, val);
                if (index[name].find(key) == index[name].end()) {
                    /* FIXME - Double check that creating a new dynamic_bitset
                     * sets all bits to zero. */
                    // FIXME - CHANGE TO ARB BITMAP
                    SwiftMap* m = map_factory.create_map(num_records, false,
                            false);
                    index[name].insert(make_pair(key, m));
                }
                index[name][key]->set(row);
                so_far += type->get_size();
            }

            row++;
        }
    }

    free(buf);
#endif
}

void SwiftIndex::init()
{
    cout << "in init" << endl;
    const vector<Attr>& attrs = schema.get_attrs_v();
    for (size_t i = 0; i < attrs.size(); i++) {
        if (attrs[i].get_cardinality() == 0) {
            continue;
        }
        cout << attrs[i].get_name() << endl;
        index.insert(make_pair(attrs[i].get_name(), l2map()));
    }

    read();
}

void SwiftIndex::clear(){
	 map_factory.destroy_tmp();  
     mapSwiftMap.clear();
}

#if 0
    for (
    const SDB::attrmap& attrs = sdb.get_attrs();
    for (SDB::attrmap::const_iterator it = attrs.begin(); it != attrs.end(); it++) {
        const string& name = it->first;
        const Attr& attr = it->second;
        if (attr.get_cardinality() == 0) {
            continue;
        }
        index.insert(make_pair(name, l2map()));
#endif
#if 0
        pthread_mutex_init(&index_lock[name], NULL);
#endif
#if 0
        for (uint64_t attr_val = 0; attr_val < it->second.get_cardinality();
                attr_val++) {
            /* FIXME - Do we actually need to declare new for attr_val_hasher
             * and attr_val_equal? */
            //index[name][attr_val] = new bitmap(sdb.get_num_records());
            //index[name][attr_val]->reset();
        }
#endif

#if 0
    struct threaded_read_arg args[NUM_THREADS];
    pthread_t threads[NUM_THREADS];
    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        args[i].thread_id = i;
        args[i].num_threads = NUM_THREADS;
        args[i].sdb = &sdb;
        args[i].index = &index;
        args[i].index_lock = &index_lock;
        args[i].l2_buf = &l2_buf;
    }

    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, threaded_read, (void*) &args[i]);
    }

    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
#endif


#if 0
/* FIXME - Must rememeber to actually implement this. */
void SwiftIndex::sample(vector<float>& records, uint64_t num_samples,
        constraintmap& constraints)
{
    bitmap result_set(sdb.get_num_records());
    result_set.set();

    for (constraintmap::iterator it = constraints.begin(); it !=
            constraints.end(); it++) {
        const string& attr = it->first;
        int attr_val = it->second;


        if (index.find(attr) == index.end() || index[attr].find(attr_val) ==
                index[attr].end()) {
            //return -1;
            return;
        }
        result_set &= *index[attr][attr_val];
    }

    if (result_set.none()) {
        //return -1;
        return;
    } else {
        vector<size_t> valid_indices;

        size_t idx = result_set.find_first();
        while (idx != bitmap::npos) {
            valid_indices.push_back(idx);
            idx = result_set.find_next(idx);
        }

        boost::uniform_int<uint64_t> indicdes(0, valid_indices.size() -1);
        boost::variate_generator<boost::mt19937&, boost::uniform_int<uint64_t> >
            rand_index(rng, indices);

        return valid_indices[rand_index()];

        /* FIXME - Following is hack for intel-sample experiment, we should use
         * code above. */
        size_t next_idx;
        uint8_t buf[sdb.get_record_size()];
        for (uint64_t i = 0; i < num_samples; i++) {
            if (sdb_f == NULL) {
                next_idx = result_set.find_first();
                sdb_f = fopen(sdb.get_fname(), "r");
            } else {
                next_idx = result_set.find_next(last_seen);
            }

            if (next_idx == bitmap::npos) {
                break;
            }


            fseek(sdb_f, (next_idx - last_seen - 1) * sdb.get_record_size(), SEEK_CUR);
            assert(fread(buf, sdb.get_record_size(), 1, sdb_f) == 1);
            float record_val = *(float *) &buf[sizeof(int)];
            records.push_back(record_val);

            last_seen = next_idx;
        }
    }
}
#endif

void SwiftIndex::run_query(const Query& query, ResultSet& results,
        SwiftMap::sample_type_t sample_type)
{
    const QueryNode& root = query.get_root();
    SwiftMap* m;
    if (root.is_singleton()) {
        /* NOTE - The reason we have this case singled out is for performance
         * reasons. */
        const Attr* a = root.get_attr();
        Binner::key_t key = binner.get_key(*a, root.get_val());
        m = index.at(a->get_name()).at(key);

    } else {
        /* FIXME - Is there no way we can conditionally staticly allocate an
         * object and then use it later? */
        // FIXME - CHANGE TO ARB BITMAP
        m = map_factory.create_map(schema.get_num_rows(), root.is_and(), true);
        combine_maps(root, m);
    }

   m->set_total_samples(query.get_num_samples());
   m->set_sampled_so_far(0);
	//cout << "full scan---" << sample_type << endl;
   if (sample_type == SwiftMap::SCAN){
	//cout << "full scan" << endl;
	   full_scan(m, query, results, sample_type);
   }else if (m->is_approx()) {
        approx_sample_mean(m, query, results, sample_type);
   } else {
        sample_mean(m, query, results, sample_type); 
   }
    //Mainly to handle Noindex.
    m->clear();
}

uint64_t SwiftIndex::run_query(const Query& query, ResultSet& results,
        SwiftMap::sample_type_t sample_type, uint64_t record_id,bool fistIter)
{
    int id = 0;
    const QueryNode& root = query.get_root();
    SwiftMap* m;
    if (root.is_singleton()) {
        /* NOTE - The reason we have this case singled out is for performance
         * reasons. */
       // cout << "in singleton" << endl;
        const Attr* a = root.get_attr();
        Binner::key_t key = binner.get_key(*a, root.get_val());
        m = index.at(a->get_name()).at(key);

    } else {
        /* FIXME - Is there no way we can conditionally staticly allocate an
         * object and then use it later? */
        // FIXME - CHANGE TO ARB BITMAP
        m = map_factory.create_map(schema.get_num_rows(), root.is_and(), true);
        combine_maps(root, m);
    }

   m->set_total_samples(query.get_num_samples());
   m->set_sampled_so_far(0);
    //cout << "full scan---" << sample_type << endl;
   if (sample_type == SwiftMap::SCAN){
    //cout << "full scan" << endl;
       full_scan(m, query, results, sample_type);
   }else if (m->is_approx()) {
        approx_sample_mean(m, query, results, sample_type);
   } else {
       // cout << "call sampling" << endl;
        id  = sample_mean(m, query, results, sample_type,record_id,fistIter); 
   }
    //Mainly to handle Noindex.
  // cout << "clear" << endl;
   if(this->isTrend==false)
       m->clear();
   // cout << "clear done" << endl;
    return id;
}

uint64_t SwiftIndex::run_query(const Query& query, ResultSet& results,
        SwiftMap::sample_type_t sample_type, uint64_t record_id,bool fistIter, int dimX)
{
    int id = 0;
    //cout << "dimX" << dimX << endl;
    string key = to_string(dimX);
    if(this->mapSwiftMap.find(key) == this->mapSwiftMap.end())
    {
        const QueryNode& root = query.get_root();
        SwiftMap* m;
        if (root.is_singleton()) {
            /* NOTE - The reason we have this case singled out is for performance
             * reasons. */
           // cout << "in singleton" << endl;
            const Attr* a = root.get_attr();
            Binner::key_t key = binner.get_key(*a, root.get_val());
            m = index.at(a->get_name()).at(key);

        } else {
            /* FIXME - Is there no way we can conditionally staticly allocate an
             * object and then use it later? */
            // FIXME - CHANGE TO ARB BITMAP
            m = map_factory.create_map(schema.get_num_rows(), root.is_and(), true);
            combine_maps(root, m);
        }

       this->mapSwiftMap[key] = m; 
       this->mapSwiftMap[key]->set_total_samples(query.get_num_samples());
       this->mapSwiftMap[key]->set_sampled_so_far(0);
        //cout << "full scan---" << sample_type << endl;
       if (sample_type == SwiftMap::SCAN){
        //cout << "full scan" << endl;
           full_scan(this->mapSwiftMap[key], query, results, sample_type);
       }else if (this->mapSwiftMap[key]->is_approx()) {
            approx_sample_mean(this->mapSwiftMap[key], query, results, sample_type);
       } else {
           // cout << "call sampling" << endl;
            id  = sample_mean(this->mapSwiftMap[key], query, results, sample_type,record_id,fistIter,1); 
       }
        //Mainly to handle Noindex.
      // cout << "clear" << endl;
       
       
       // cout << "clear done" << endl;

    }
    else
    {
        //SwiftMap* m;

        this->mapSwiftMap[key]->set_total_samples(query.get_num_samples());

        if (sample_type == SwiftMap::SCAN){
        //cout << "full scan" << endl;
           full_scan(this->mapSwiftMap[key], query, results, sample_type);
       }else if (this->mapSwiftMap[key]->is_approx()) {
            approx_sample_mean(this->mapSwiftMap[key], query, results, sample_type);
       } else {
           // cout << "call sampling" << endl;
            id  = sample_mean(this->mapSwiftMap[key], query, results, sample_type,record_id,fistIter,1); 
       }
        
    }

    return id;

    
}

uint64_t SwiftIndex::run_query(const Query& query, ResultSet& results,
        SwiftMap::sample_type_t sample_type, uint64_t record_id,bool fistIter, int dimX,int dimY)
{
    int id = 0;
    //cout << "dimX,dimY:" << dimX << "," << dimY << endl;
    string key = to_string(dimX)+","+to_string(dimY);
    if(this->mapSwiftMap.find(key) == this->mapSwiftMap.end())
    {
        const QueryNode& root = query.get_root();
        SwiftMap* m;
        if (root.is_singleton()) {
            /* NOTE - The reason we have this case singled out is for performance
             * reasons. */
           // cout << "in singleton" << endl;
            const Attr* a = root.get_attr();
            Binner::key_t key = binner.get_key(*a, root.get_val());
            m = index.at(a->get_name()).at(key);

        } else {
            /* FIXME - Is there no way we can conditionally staticly allocate an
             * object and then use it later? */
            // FIXME - CHANGE TO ARB BITMAP
            m = map_factory.create_map(schema.get_num_rows(), root.is_and(), true);
            combine_maps(root, m);
        }

       this->mapSwiftMap[key] = m; 
       this->mapSwiftMap[key]->set_total_samples(query.get_num_samples());
       this->mapSwiftMap[key]->set_sampled_so_far(0);
        //cout << "full scan---" << sample_type << endl;
       if (sample_type == SwiftMap::SCAN){
        //cout << "full scan" << endl;
           full_scan(this->mapSwiftMap[key], query, results, sample_type);
       }else if (this->mapSwiftMap[key]->is_approx()) {
            approx_sample_mean(this->mapSwiftMap[key], query, results, sample_type);
       } else {
           // cout << "call sampling" << endl;
            id  = sample_mean(this->mapSwiftMap[key], query, results, sample_type,record_id,fistIter,1); 
       }
        //Mainly to handle Noindex.
      // cout << "clear" << endl;
       
       
       // cout << "clear done" << endl;

    }
    else
    {
        //SwiftMap* m;

        this->mapSwiftMap[key]->set_total_samples(query.get_num_samples());

        if (sample_type == SwiftMap::SCAN){
        //cout << "full scan" << endl;
           full_scan(this->mapSwiftMap[key], query, results, sample_type);
       }else if (this->mapSwiftMap[key]->is_approx()) {
            approx_sample_mean(this->mapSwiftMap[key], query, results, sample_type);
       } else {
           // cout << "call sampling" << endl;
            id  = sample_mean(this->mapSwiftMap[key], query, results, sample_type,record_id,fistIter,1); 
       }
        
    }

    return id;

    
}


void SwiftIndex::combine_maps(const QueryNode& node, SwiftMap* base)
{
    /* TODO - Consider caching at this point - Have a map which goes from query
     * -> stored bitmap. */
    const vector<QueryNode>& children = node.get_children();
    for (size_t i = 0; i < children.size(); i++) {
        if (children[i].is_singleton()) {
            const Attr* a = children[i].get_attr();
            //cout<<"combine_maps:"<< a->get_name() <<endl;
            Binner::key_t key = binner.get_key(*a, children[i].get_val());
            SwiftMap* m = index.at(a->get_name()).at(key);

            if (node.is_and()) {
                base->and_combine(m);
            } else {
                base->or_combine(m);
            }
        } else {
            SwiftMap* new_base = map_factory.create_map(schema.get_num_rows(),
                    children[i].is_and(), true);
            combine_maps(children[i], new_base);

            if (node.is_and()) {
                base->and_combine(new_base);
            } else {
                base->or_combine(new_base);
            }
        }
    }
            /*
    if (node.is_singleton()) {
        const Attr* a = node.get_attr();
        Binner::key_t key = binner.get_key(*a, node.get_val());
        return index.at(a->get_name()).at(key);
    } else {
        const vector<QueryNode>& children = node.get_children();
        for (size_t i = 0; i < children.size(); i++) {
            if (children[i].is_singleton()) {
                SwiftMap* m = combine_maps(children[i], NULL);
                if (node.is_and()) {
                    *base &= *m;
                } else {
                    *base |= *m;
                }
            } else {
                // FIXME - CHANGE TO ARB BITMAP
                SingleBitmap new_base(sdb.get_num_records(), children[i].is_and());
                SwiftMap* m = combine_maps(children[i], &new_base);
                if (node.is_and()) {
                    *base &= *m;
                } else {
                    *base |= *m;
                }
            }
            SwiftMap* m;
            if (children[i].is_singleton()) {
                m = combine_maps(children[i], NULL);
            } else {
                // FIXME - CHANGE TO ARB BITMAP
                SingleBitmap new_base(sdb.get_num_records(), children[i].is_and());
                m = combine_maps(children[i], &new_base);
            }

            if (node.is_and()) {
                *base &= *m;
            } else {
                *base |= *m;
            }
        }
        return base;
    }
            */
}

void SwiftIndex::sample_mean(SwiftMap* m, const Query& query, ResultSet&
        results, SwiftMap::sample_type_t sample_type)
{
    /* TODO - Reenable this to make it robust. Otherwise, we assume every
     * bitmap will have at least one valid record. */
#if 0
    if (m->none()) {
        results.set_exit_code(ResultSet::NO_VALID_RECORDS);
        return;
    }
#endif

    vector<const uint8_t*> vals;
    vector<uint64_t> offsets;
    const vector<const Attr*>& targets = query.get_targets();

    for (uint64_t i = 0; i < query.get_num_samples(); i++) {
        /* FIXME - This uses next_valid _record_idx which samples from the next
         * valid index which may not be what we want all the time; we could
         * also want purely random and also maybe random to within a couple of
         * apges. */
        sdb.get_vals(m->next_sample(sample_type), targets, vals);
            
        //results_file << idx / sdb.records_per_buf() << "\t";
	for (size_t j = 0; j < vals.size(); j++) {
            	results.update(j, targets[j]->get_type()->to_double(vals[j]));
	}
	//result_file << endl;
        vals.clear();
    }

   
}

uint64_t SwiftIndex::sample_mean(SwiftMap* m, const Query& query, ResultSet&
        results, SwiftMap::sample_type_t sample_type,uint64_t record_id,bool firstIter)
{
    /* TODO - Reenable this to make it robust. Otherwise, we assume every
     * bitmap will have at least one valid record. */
#if 0
    if (m->none()) {
        results.set_exit_code(ResultSet::NO_VALID_RECORDS);
        return;
    }
#endif

    vector<const uint8_t*> vals;
    vector<uint64_t> offsets;
    const vector<const Attr*>& targets = query.get_targets();

    if (firstIter == false)
    {

      // cout << "dummy sample" << endl;
        
        m->set_sampled_so_far(0);
        m->next_sample(sample_type); // to reinitialize the and map for the new iteration
        m->set_sampled_so_far(record_id);

    }

    for (uint64_t i = 0; i < query.get_num_samples(); i++) {
        /* FIXME - This uses next_valid _record_idx which samples from the next
         * valid index which may not be what we want all the time; we could
         * also want purely random and also maybe random to within a couple of
         * apges. */
        
        //cout << "sample a sample" << endl;
        sdb.get_vals(m->next_sample(sample_type), targets, vals);

        if(m->isSamplesEmpty==true)
        {
           // cout << "index break" << endl;
            break;
        }
        isRecordEmpty = false;
        //results_file << idx / sdb.records_per_buf() << "\t";
        for (size_t j = 0; j < vals.size(); j++) {
                    results.update(j, targets[j]->get_type()->to_double(vals[j]));
        }
        //result_file << endl;
        vals.clear();
    }
    if(m->isSamplesEmpty==true)
    {
        //cout << "index break flag" << endl;
        isRecordEmpty = true;
        return 0;
    }
    record_id = m->get_sampled_so_far();

    return record_id;
}

uint64_t SwiftIndex::sample_mean(SwiftMap* m, const Query& query, ResultSet&
        results, SwiftMap::sample_type_t sample_type,uint64_t record_id,bool firstIter,int filler)
{
    /* TODO - Reenable this to make it robust. Otherwise, we assume every
     * bitmap will have at least one valid record. */
#if 0
    if (m->none()) {
        results.set_exit_code(ResultSet::NO_VALID_RECORDS);
        return;
    }
#endif

    vector<const uint8_t*> vals;
    vector<uint64_t> offsets;
    const vector<const Attr*>& targets = query.get_targets();

    for (uint64_t i = 0; i < query.get_num_samples(); i++) {
        /* FIXME - This uses next_valid _record_idx which samples from the next
         * valid index which may not be what we want all the time; we could
         * also want purely random and also maybe random to within a couple of
         * apges. */
        
        //cout << "sample a sample" << endl;
        sdb.get_vals(m->next_sample(sample_type), targets, vals);

        if(m->isSamplesEmpty==true)
        {
           // cout << "index break" << endl;
            break;
        }
        isRecordEmpty = false;
        //results_file << idx / sdb.records_per_buf() << "\t";
        for (size_t j = 0; j < vals.size(); j++) {
                    results.update(j, targets[j]->get_type()->to_double(vals[j]));
        }
        //result_file << endl;
        vals.clear();
    }
    if(m->isSamplesEmpty==true)
    {
        //cout << "index break flag" << endl;
        isRecordEmpty = true;
        return 0;
    }
    record_id = m->get_sampled_so_far();

    return record_id;
}

bool SwiftIndex::verify_record(const QueryNode& node, const uint8_t* record)
{
    if (node.is_singleton()) {
        const Attr* a = node.get_attr();
        return !memcmp(&record[a->get_offset()], node.get_val(),
                a->get_type()->get_size());
    }

    const vector<QueryNode>& children = node.get_children();
    for (size_t i = 0; i < children.size(); i++) {
        bool ok = verify_record(children[i], record);
        if (!ok && node.is_and()) {
            return false;
        } else if (ok && !node.is_and()) {
            return true;
        }
    }
    if (node.is_and()) {
        return true;
    } else {
        return false;
    }
}


#if 0
void SwiftIndex::approx_sample_mean(SwiftMap* m, const Query& query, ResultSet&
        results, SwiftMap::sample_type_t sample_type)
{
    const vector<const Attr*>& targets = query.get_targets();
    uint64_t total_time = 0;

    ofstream samples_file;
    std::ostringstream temp;
    temp <<  "Samples_" << query.get_dataset_name() << "_" << query.get_num_terms() << "_" << query.get_sample_rate() << "_convinient_" << query.get_conv_rate() << "_" << query.get_trial() << "_" << query.get_dataset() << ".txt" << std::ends;
    samples_file.open (temp.str());

    uint64_t num_samples =m->get_sampled_so_far();
    m->set_total_samples(query.get_num_samples());

    sdb.get_record(0);
    clear_io_timer();

    while (num_samples < query.get_num_samples()) {
        size_t idx = m->next_sample(sample_type);
        uint64_t prev_num_samples = num_samples;

        clear_io_timer();
        results.clear_vals();
        for (uint64_t i = 0; i < sdb.records_per_buf(); i++) {
            if (num_samples >= query.get_num_samples()) {

                //cout << idx / sdb.records_per_buf() << endl;
/**
                results.set_io_time(get_io_timer());
                samples_file << idx / sdb.records_per_buf() << " " << num_samples - prev_num_samples;
                std::vector<double> avgs = results.get_avgs();
                std::cout <<  avgs.size() << endl;
                for (size_t i = 0; i < avgs.size(); i++){
                        samples_file << " " << avgs[i];

                }
                samples_file << endl;
                //results.clear_vals();
**/
                break;
            }

	    clear_io_timer();
            const uint8_t* record = sdb.get_record(idx + i);
	    results.set_io_time(get_io_timer());


            if (verify_record(query.get_root(), record)) {
		samples_file << idx / sdb.records_per_buf() << "\t" ;
                for (size_t j = 0; j < targets.size(); j++) {
		    double val = targets[j]->get_type()->to_double(
                                &record[targets[j]->get_offset()]);
		    results.update(j, val);
                    //results.update(j, targets[j]->get_type()->to_double(
                    //            &record[targets[j]->get_offset()]));

                    samples_file << val << "\t";
	            //	samples_file << targets[j]->get_type()->to_double(
                    //            &record[targets[j]->get_offset()]) << endl;
                }
		samples_file << endl;

                num_samples++;
            }
        }
        m->set_sampled_so_far(num_samples);



        //results.set_io_time(get_io_timer());
        //cout << idx / sdb.records_per_buf() << endl;


/**
        if (num_samples < query.get_num_samples()){
                samples_file << idx / sdb.records_per_buf() << " " << num_samples - prev_num_samples << " ";
                std::vector<double> avgs = results.get_avgs();
                for (size_t i = 0; i < avgs.size(); i++){
                        samples_file << avgs[i] << " ";

                }
                samples_file << endl;
        }

**/
	//results.clear_vals();


    }
}

#endif



void SwiftIndex::approx_sample_mean(SwiftMap* m, const Query& query, ResultSet&
        results, SwiftMap::sample_type_t sample_type)
{
    const vector<const Attr*>& targets = query.get_targets();
/**
   ofstream samples_file;
   std::ostringstream temp;
   temp << query.get_dataset_name() << "_" << query.get_num_terms() << "_" << query.get_sample_rate() << ".txt"  << std::ends;
   samples_file.open (temp.str());
**/
/**
    std::ostringstream temp;
    temp <<  "Samples_" << query.get_dataset_name() << "_" << query.get_num_terms() << "_" << query.get_sample_rate() << "_convinient_" << query.get_conv_rate() << "_" << query.get_trial() << "_" << query.get_dataset() << ".txt" << std::ends;
    samples_file.open (temp.str());
**/

    uint64_t num_samples =m->get_sampled_so_far();
    m->set_total_samples(query.get_num_samples());
    std::unordered_set<uint64_t> fetched_block;

    //sdb.get_record(0);
    //clear_io_timer();
    //results.clear_vals();

    std::vector<double> conv_sum(targets.size() , 0.0);
    double num_conv_samples = 0.0;

    while (num_samples < query.get_num_samples()) {
        size_t idx = m->next_sample(sample_type);
	//samples_file << idx / sdb.records_per_buf() << endl;
        uint64_t prev_num_samples = num_samples;

//      clear_io_timer();
//	    results.clear_vals();
        for (uint64_t i = 0; i < sdb.records_per_buf(); i++) {

        
            if (num_samples >= query.get_num_samples()) {
                break;
            }
            //get the records
            const uint8_t* record = sdb.get_record(idx + i);

            if (verify_record(query.get_root(), record)) {
                for (size_t j = 0; j < targets.size(); j++) {
                    double val = targets[j]->get_type()->to_double(
                                &record[targets[j]->get_offset()]);
                    results.update(j, val);
                    conv_sum[j] += val;
		            //samples_file << targets[j]->get_type()->to_double(
                    //          &record[targets[j]->get_offset()]) << endl;
		        }

		        num_samples++;
			num_conv_samples += 1;
            }
        }

        m->set_sampled_so_far(num_samples);

        fetched_block.insert(idx / sdb.records_per_buf());
	//samples_file << idx / sdb.records_per_buf() << "\t" << num_samples << endl;
   
	
    }
    //samples_file.close();
    if (query.get_rand_num_samples() == 0){
        return;
    }

   // Random sampling
   //samples_file << "Random" << endl;

   //uint64_t num_blocks = UPDIV(schema.get_num_rows(), sdb.records_per_buf());

   std::vector<double> rand_sum(targets.size() , 0.0);
   std::vector<uint64_t> valid_blocks = m -> find_valid_blocks();

  // samples_file << "valid_blocks" << " " << valid_blocks.size() << endl;
   std::vector<uint64_t> rand_blocks;
   for (uint64_t i = 0; i < valid_blocks.size(); i++){
	 uint64_t idx = valid_blocks[i];
         if (fetched_block.find(idx) == fetched_block.end())
                rand_blocks.push_back(idx);
   }

   std::random_shuffle(rand_blocks.begin(), rand_blocks.end());
   m->set_sampled_so_far(0);
   num_samples = m-> get_sampled_so_far();

   uint64_t j = 0;
   double num_rand_blocks = 0.0;
   double num_rand_samples = 0.0;
   while(num_samples < query.get_rand_num_samples()){
        uint64_t prev_num_samples = num_samples;
        uint64_t block_index = rand_blocks[j];
        size_t idx = sdb.records_per_buf() * block_index;
        num_rand_blocks += 1;

	    for (uint64_t i = 0; i < sdb.records_per_buf(); i++){

		/**
                if (num_samples > query.get_rand_num_samples()){
                    break;
                } **/
                const uint8_t* record = sdb.get_record(idx + i);
                if (verify_record(query.get_root(), record)) {
                    for (size_t j = 0; j < targets.size(); j++) {
                            double val = targets[j]->get_type()->to_double(
                                &record[targets[j]->get_offset()]);
                            results.update(j, val);

                            rand_sum[j] += val;
                    }
                    num_samples++;
		    num_rand_samples += 1;
                }
        }
        j ++;
        m->set_sampled_so_far(num_samples);

   }

    double rand_weight = num_rand_blocks / (valid_blocks.size() - fetched_block.size());
/**
    cout << "rand_weight: " << rand_weight << endl;
    cout << "esti total valid records: " << m -> get_est_valid_records() << endl;
    cout << "num conv samples: " << query.get_num_samples() << endl;
    cout << "num rand samples: " << query.get_rand_num_samples() << endl;
**/

    for (size_t j = 0; j < targets.size(); j++) {
   	//cout << "rand_sum: " << rand_sum[j] << " rand samples: " << num_samples << endl;
	double reweighted_rand_sum = rand_sum[j] / rand_weight;
        double est_sum = reweighted_rand_sum + conv_sum[j];
        double unb_est_avg = est_sum / m -> get_est_valid_records();
        double ratio_est_avg = est_sum / (num_rand_samples / rand_weight + num_conv_samples);
        //cout << "reweighted_rand_sum: " << reweighted_rand_sum << " est_sum: " << est_sum << " unb_est_avg: " << unb_est_avg << " ratio_est_avg: " << ratio_est_avg << " num_rand_blocks: " << num_rand_blocks << endl;
	results.update_esti_results(j, est_sum, unb_est_avg, ratio_est_avg);

   }
   results.set_rand_blocks(num_rand_blocks);

//    dp_file.close();
}

void SwiftIndex::full_scan(SwiftMap* m, const Query& query, ResultSet&
        results, SwiftMap::sample_type_t sample_type)
{
    const vector<const Attr*>& targets = query.get_targets();

    uint64_t record_size = schema.get_record_size();
    size_t idx=0;
    size_t record_index=0;
    size_t cardinality=0;
    //std::ostringstream temp;
    //temp << "results_chunk_density_"<<query.get_num_terms()<<ends;
    //ofstream results_file;
    //results_file.open(temp.str());
    while(idx < schema.get_num_rows()){
          const uint8_t* record = sdb.get_record(idx);
          if (verify_record(query.get_root(), record)) {
    	        for (size_t j = 0; j < targets.size(); j++) {
        		results.update(j, targets[j]->get_type()->to_double(&record[targets[j]->get_offset()]));
                 }
	         	cardinality+=1;
          }
          record_index++;
          idx++;
          if(record_index>=sdb.records_per_buf()){
        	//cout << idx << "\t" << idx / sdb.records_per_buf()<<"\t" << static_cast<double>(cardinality)/sdb.records_per_buf() << endl;
           	if (cardinality > 0){
			//results_file << idx / sdb.records_per_buf() - 1 << endl;
		}
        	record_index=0;
        	cardinality=0;
          }

   }

   // results_file.close();


}



