#!/usr/bin/env python2
"""
Every query will be a nested dict/list combination. The top level will have two
attributes - 'num_samples' and 'q'. 'q' will represent a dictionary of things
to filter for. Every node in 'q' is represnted by a dict. There are 2 kinds of
dicts possible for 'q'. If the '__op__' key exists, the dict is an combinator
node. The type of combination is given by the value of '__op__' and the nodes
to be combined are given by the '__nodes__' key. If the '__op__' key does not
exist, then it is a singleton dict whose key corresponds to an attribute in the
sdb and attribute value represents the attribute value we want to filter for.
"""

import os
import json
import sys
import struct

import numpy as np
from scipy.stats import zipf

import yaml

import data_utils
import gen_data

q0 = {
        'num_samples': 100,
        'targets': ['c0'],
        'q': {
            '__op__': 'and',
            '__nodes__': [
                {'d0': 2},
                {'d1': 2},
                {'d2': 2},
                {'d3': 1},
                {'d4': 1},
                {'d5': 1},
                ]
            }
        }

def match_constraint(schema, node, row):
    if '__nodes__' in node:
        and_op = node['__op__'] == 'and'
        for child in node['__nodes__']:
            if not match_constraint(schema, child, row) and and_op:
                return False
            elif match_constraint(schema, child, row) and not and_op:
                return True
        if and_op:
            return True
        else:
            return False
    else:
        key = node.keys()[0]
        return row[schema.attr_idx(key)] == node[key]

def calc_theo_pmf(schema, node):
    if '__nodes__' in node:
        and_op = node['__op__'] == 'and'
        if and_op:
            pmf = 1.0
        else:
            pmf = 0.0
        for child in node['__nodes__']:
            if and_op:
                pmf *= calc_theo_pmf(schema, child)
            else:
                pmf *= calc_theo_pmf(schema, child)
        return pmf
    else:
        key = node.keys()[0]
        a = schema.col_attr(schema.attr_idx(key), 'distrib_a')
        return zipf.pmf(node[key], a)

def main():
    if len(sys.argv) != 3:
        print 'Usage: %s <dirpath> <query-file>' % sys.argv[0]
        sys.exit(2)

    dirpath = sys.argv[1]
    qpath = sys.argv[2]
    schema = data_utils.Schema(os.path.join(dirpath, gen_data.SCHEMA_FNAME))
    sdb = data_utils.SDB(os.path.join(dirpath, gen_data.SDB_FNAME), schema)

    qname = os.path.basename(qpath)
    q = yaml.load(open(qpath))
    f = open(os.path.join(dirpath, qname), 'w')
    f.write(yaml.dump(q))
    f.close()

    count = 0
    # FIXME - Super hacky; fix
    sdb.open('r')
    hits_in_page = 0
    row_counter = 0
    for row in sdb.records():
        if row_counter % sdb.records_per_page() == 0:
            if hits_in_page > 0:
                print (row_counter / sdb.records_per_page())-1, hits_in_page
            hits_in_page = 0

        if match_constraint(schema, q['q'], row):
            count += 1
            hits_in_page += 1

        row_counter += 1

    if hits_in_page > 0:
        print (row_counter / sdb.records_per_page())-1, hits_in_page

    sdb.close()

    pmf = calc_theo_pmf(schema, q['q'])

    print('%s - theoretical selectivity: %f real selectivity: %f' % (
        qname, pmf, float(count) / schema.num_rows()))

if __name__ == '__main__':
    main()
