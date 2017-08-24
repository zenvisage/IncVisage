#!/usr/bin/env python2

import argparse
import csv
import ctypes
import os.path
import struct
import sys
import uuid

from scipy.stats import randint, uniform, zipf, truncnorm

import data_utils

MIN_DISTINCT = 10
MAX_DISTINCT = 10

BUF_ROW_SIZE = 1000

DISK_PAGE_SIZE = 256 * 1024

DATA_DIR = os.path.join(os.path.dirname(__file__), '../data')

SDB_FNAME = 'sdb9'
SCHEMA_FNAME = 'schema9'

def gen_dirpath(args):
    dirname = '%s,rows%d,dcols%d,ccols%d' % (
            uuid.uuid4(), args.rows, args.dcols, args.ccols)
    dirpath = os.path.join(args.outdir, dirname)
    os.makedirs(dirpath)
    return dirpath

def gen_data(dirpath, schema, args):
    sdb_path = os.path.join(dirpath, args.sdb_fname)
    sdb = data_utils.SDB(sdb_path, schema)
    sdb.open('w')
    records_per_page = sdb.records_per_page()
    num_pages = (args.rows + records_per_page - 1) / records_per_page
    for page in range(num_pages):
        if page % 10 == 0:
            print 'Progress: %d%%' % ((records_per_page * page) * 100 / args.rows)
        col_data = []
        cols = schema.cols()
        for col in cols:
            col_data.append(col.rvs(records_per_page))

        row_data = []
        for row in range(records_per_page):
            row_data.append([array[row] for array in col_data])

        sdb.write(row_data)

    sdb.close()

class TruncnormWrapper(object):
    def __init__(self, _min, _max, mean, std):
        self._mean = mean
        self._std = std
        a = (_min - mean) * 1. / std
        b = (_max - mean) * 1. / std
        self._rv = truncnorm(a, b)

    def rvs(self, size):
        return self._mean + self._std * self._rv.rvs(size=size)


def create_schema(dirpath, args):
    cols = []
    for i in range(args.dcols):
        distrib_a = 1.5 + 3.5 * uniform().rvs()
        rv = zipf(distrib_a)
        cols.append(data_utils.Column('d' + str(i), 'ulong', 'discrete',
            'zipf', rv, True, {'distrib_a': distrib_a}))

    for i in range(args.ccols):
        distrib_min = 0
        distrib_max = 1
        distrib_mean = .2 + .6 * uniform().rvs()
        distrib_std = .1
        rv = TruncnormWrapper(distrib_min, distrib_max, distrib_mean, distrib_std)
        cols.append(data_utils.Column('c' + str(i), 'double', 'continuous',
            'trucnorm', rv, False, {'distrib_min': 0, 'distrib_max': 1,
                'distrib_mean': distrib_mean, 'distrib_std': distrib_std}))

    schema_path = os.path.join(dirpath, args.schema_fname)
    schema = data_utils.Schema(schema_path, cols, args)
    return schema

# NOTE - Assuems the distributikons for the columns are uniform
def get_args():
    parser = argparse.ArgumentParser()

    # Path to directory to create directory in
    parser.add_argument('rows', type=int)
    parser.add_argument('dcols', type=int)
    parser.add_argument('ccols', type=int)

    parser.add_argument('-o', dest='outdir', default=DATA_DIR)
    parser.add_argument('-p', '--page_size', type=int,
            default=DISK_PAGE_SIZE)
    parser.add_argument('-d', dest='dirpath', default=None)
    parser.add_argument('--min_distinct', type=int, default=MIN_DISTINCT)
    parser.add_argument('--max_distinct', type=int, default=MAX_DISTINCT)
    parser.add_argument('--sdb_fname', default=SDB_FNAME)
    parser.add_argument('--schema_fname', default=SCHEMA_FNAME)

    args = parser.parse_args()
    args.max_distinct = max(args.min_distinct, args.max_distinct)
    return args

def main():
    args = get_args()
    dirpath = gen_dirpath(args) if args.dirpath is None else args.dirpath
    print 'Generated: %s' % dirpath
    schema = create_schema(dirpath, args)
    gen_data(dirpath, schema, args)
    schema.save()


if __name__ == '__main__':
    main()
