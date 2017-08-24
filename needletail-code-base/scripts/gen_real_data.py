#!/usr/bin/python
# -*- coding: utf-8 -*-

import argparse
import csv
import ctypes
import os.path
import struct
import sys
import uuid
import shutil
from scipy.stats import randint, uniform, zipf, truncnorm
import data_utils
import pandas as pd

MIN_DISTINCT = 10
MAX_DISTINCT = 10
BUF_ROW_SIZE = 1000

DISK_PAGE_SIZE = 1024 * 256

# DATA_DIR = os.path.join(os.path.dirname(__file__), '../data')

DATA_DIR = os.path.join(os.path.dirname(__file__),
                        '/home/srahman7/incvisage-cpp/needletail-code-base/data')

SDB_FNAME = 'sdb'
SCHEMA_FNAME = 'schema'

DISTTYPE = 'uniform'
DENSITY = 0.1
DBCOUNT = 1
airports = {}
index = 0
dataset = ''
flight = False

dict_carrier = {"AA":1,"UA":2,"US":3} #{"AA":1,"AS":2,"CO":3,"OH(1)":4,"UA":5,"US":6}
dict_city = {"Atlanta- GA":1,"Chicago- IL":2,"Houston- TX":3}
dict_state = {"NY":1} #{"CA":1,"GA":2,"IL":3,"NY":4,"TX":5}
dict_airport = {"ATL":1,"DEN":2,"DFW":3,"LGA":4,"ORD":5}

dict_carrier_tmp = {}
dict_city_tmp = {}
dict_state_tmp = {}
dict_airport_tmp = {}


def read_airport_names():
    global airports
    with open('flight_names.csv') as f1:
        for line in f1:
            line = line.rstrip('\n')
            names = line.split('\t')
            airports[names[0]] = names[1]


def gen_dirpath(args):

    # dirname = '%s,rows%d,dcols%d,ccols%d' % (
    #        uuid.uuid4(), args.rows, args.dcols, args.ccols)

    dirname = dataset
    dirpath = os.path.join(args.outdir, dirname)
    print dirpath

    return dirpath

def gen_test_data(
    dirpath,
    schema,
    args,
    db_index,
    ):
    global index
    global airports
    if flight:
        read_airport_names()
    sdb_path = os.path.join(dirpath, args.sdb_fname + '_'
                            + str(db_index))
    sdb = data_utils.SDB(sdb_path, schema)
    sdb.open('w')
    records_per_page = sdb.records_per_page()
    num_pages = (args.rows + records_per_page - 1) / records_per_page

    num_rows = 0
    row_data = []
    print records_per_page

#    for row in df.itertuples(index = False):

    flag = True
    blockcount = 0
    count = 0
    dim = 1
    meas = 100
    for i in range(16384):

        row_data.append([int(dim),float(meas)])
        num_rows = num_rows + 1
        if num_rows == records_per_page:
            blockcount += 1
            print count
            sdb.write(row_data)
            num_rows = 0
            row_data = []
        count = count + 1
    

    sdb.close()

def gen_wounder_data(
    dirpath,
    schema,
    args,
    db_index,
    ):
    global index
    global airports
    if flight:
        read_airport_names()
    sdb_path = os.path.join(dirpath, args.sdb_fname + '_'
                            + str(db_index))
    sdb = data_utils.SDB(sdb_path, schema)
    sdb.open('w')
    records_per_page = sdb.records_per_page()
    num_pages = (args.rows + records_per_page - 1) / records_per_page

    num_rows = 0
    row_data = []
    print records_per_page

#    for row in df.itertuples(index = False):

    flag = True
    blockcount = 0
    count = 0
    with open('/home/srahman7/needletail-code-base/data/wundergroundWeed.csv'
              ) as f:
        for line in f:
            if flag:
                flag = False
                continue
            line = line.rstrip('\n')
            row = line.split(',')

            c = 0
            new_row = []
            for i in row:
                if c>0 and c<7:
                    new_row.append(int(i))
                elif c>6 and c< 12:
                    new_row.append(float(i))
                c = c + 1

            # print row;

            row_data.append(new_row)
            num_rows = num_rows + 1
            if num_rows == records_per_page:
                blockcount += 1
                print count
                sdb.write(row_data)
                num_rows = 0
                row_data = []
            count = count + 1
        if row_data != []:
            sdb.write(row_data)
            blockcount + 1
            print blockcount

    sdb.close()

def gen_flight_data(
    dirpath,
    schema,
    args,
    db_index,
    ):
    global index
    global airports
    if flight:
        read_airport_names()
    sdb_path = os.path.join(dirpath, args.sdb_fname + '_'
                            + str(db_index))
    sdb = data_utils.SDB(sdb_path, schema)
    sdb.open('w')
    records_per_page = sdb.records_per_page()
    num_pages = (args.rows + records_per_page - 1) / records_per_page

    num_rows = 0
    row_data = []
    print records_per_page

#    for row in df.itertuples(index = False):

    flag = True
    blockcount = 0
    count = 0
    carrier = 7
    o_state = 12
    o_city = 8
    o_airport = 12
    d_state = 12
    d_city = 8
    d_airport = 12 
    lineCount = 0
    with open('/media/srahman7/OS/Users/srahman7/workspace/datarepo/flightWeed.csv'
              ) as f:
        for line in f:
            if flag:
                flag = False
                continue
            line = line.rstrip('\n')
            row = line.split(',')
            
            c = 0
            new_row = []
            willAdd = True
            for i in row:
                if c>=0 and c<6:
                    new_row.append(int(i))
                elif c>12:
                    new_row.append(float(i))
                elif c==6:
                    if i in dict_carrier:
                        new_row.append(dict_carrier[i])
                    else:
                        willAdd=False
                        #new_row.append(carrier)
                        #dict_carrier[i] = carrier
                        #carrier = carrier+1
                elif c==7:
                    if i in dict_airport:
                        new_row.append(dict_airport[i])
                    else:
                        willAdd=False
                        #new_row.append(o_airport)
                        #dict_airport[i] = o_airport
                        #o_airport = o_airport+1
                elif c==8:
                    if i in dict_city:
                        new_row.append(dict_city[i])
                    else:
                        willAdd=False
                        #new_row.append(o_city)
                        #dict_city[i] = o_city
                        #o_city = o_city+1
                elif c==9:
                    if i in dict_state:
                        new_row.append(dict_state[i])
                    else:
                        willAdd=False
                        #new_row.append(o_state)
                        #dict_state[i] = o_state
                        #o_state = o_state+1
                elif c==10:
                    if i in dict_airport:
                        new_row.append(dict_airport[i])
                    else:
                        willAdd=False
                        #new_row.append(o_airport)
                        #dict_airport[i] = o_airport
                        #o_airport = o_airport+1
                elif c==11:
                    if i in dict_city:
                        new_row.append(dict_city[i])
                    else:
                        willAdd=False
                        #new_row.append(o_city)
                        #dict_city[i] = o_city
                        #o_city = o_city+1
                else:
                    if i in dict_state:
                        new_row.append(dict_state[i])
                    else:
                        willAdd=False
                        #new_row.append(o_state)
                        #dict_state[i] = o_state
                        #o_state = o_state+1
                c = c + 1

            # print row;
            if willAdd==True:
                row_data.append(new_row)
                num_rows = num_rows + 1
                count = count + 1
            if num_rows == records_per_page:
                blockcount += 1
                print count
                sdb.write(row_data)
                num_rows = 0
                row_data = []
            
        if row_data != []:
            sdb.write(row_data)
            blockcount + 1
            print blockcount
    print count
    sdb.close()

def gen_flight_data_shrink(
    dirpath,
    schema,
    args,
    db_index,
    ):
    global index
    global airports
    if flight:
        read_airport_names()
    sdb_path = os.path.join(dirpath, args.sdb_fname + '_'
                            + str(db_index))
    sdb = data_utils.SDB(sdb_path, schema)
    sdb.open('w')
    records_per_page = sdb.records_per_page()
    num_pages = (args.rows + records_per_page - 1) / records_per_page

    num_rows = 0
    row_data = []
    print records_per_page

#    for row in df.itertuples(index = False):

    flag = True
    blockcount = 0
    count = 0
    carrier = 7
    o_state = 12
    o_city = 8
    o_airport = 12
    d_state = 12
    d_city = 8
    d_airport = 12 
    lineCount = 0
    with open('/media/srahman7/OS/Users/srahman7/workspace/datarepo/flightWeed.csv'
              ) as f:
        for line in f:

            if flag:
                flag = False
                continue
            lineCount = lineCount+1

            if lineCount==10000001:
                break
            
            line = line.rstrip('\n')
            row = line.split(',')
            #print line
            c = 0
            new_row = []
            willAdd = False
            for i in row:
                if c>=0 and c<6:
                    new_row.append(int(i))
                elif c>12:
                    new_row.append(float(i))
                elif c==6:
                    if i in dict_carrier:
                        new_row.append(dict_carrier[i])
                        willAdd=True
                       # print "carrier-willAdd"
                    else:
                        if i in dict_carrier_tmp:
                            new_row.append(dict_carrier_tmp[i])
                        else:
                            new_row.append(carrier)
                            dict_carrier_tmp[i] = carrier
                            carrier = carrier+1
                elif c==7:
                    if i=="DFW" or i=="LGA" or i=="ORD":
                        new_row.append(dict_airport[i])
                        willAdd=True
                    elif i in dict_airport:
                        new_row.append(dict_airport[i])
                        # print "airports-willAdd"
                    else:
                        if i in dict_airport_tmp:
                            new_row.append(dict_airport_tmp[i])
                        else:
                            new_row.append(o_airport)
                            dict_airport_tmp[i] = o_airport
                            o_airport = o_airport+1
                elif c==8:
                    if i in dict_city:
                        new_row.append(dict_city[i])
                        willAdd=True
                       # print "city-willAdd"
                    else:
                        if i in dict_city_tmp:
                            new_row.append(dict_city_tmp[i])
                        else:
                            new_row.append(o_city)
                            dict_city_tmp[i] = o_city
                            o_city = o_city+1
                elif c==9:
                    if i in dict_state:
                        new_row.append(dict_state[i])
                        #willAdd=True #no originstate in QN
                       # print "state-willAdd"
                    else:
                        if i in dict_state_tmp:
                            new_row.append(dict_state_tmp[i])
                        else:
                            new_row.append(o_state)
                            dict_state_tmp[i] = o_state
                            o_state = o_state+1
                elif c==10:
                    if i=="ATL" or i=="DEN":
                        new_row.append(dict_airport[i])
                        willAdd=True
                    elif i in dict_airport:
                        new_row.append(dict_airport[i])
                      #  print "airports-willAdd2"
                    else:
                        if i in dict_airport_tmp:
                            new_row.append(dict_airport_tmp[i])
                        else:
                            new_row.append(o_airport)
                            dict_airport_tmp[i] = o_airport
                            o_airport = o_airport+1
                elif c==11:
                    if i in dict_city:
                        new_row.append(dict_city[i])
                        #willAdd=True #since there is not dest city in qn
                      #  print "city-willAdd2"
                    else:
                        if i in dict_city_tmp:
                            new_row.append(dict_city_tmp[i])
                        else:
                            new_row.append(o_city)
                            dict_city_tmp[i] = o_city
                            o_city = o_city+1
                else:
                    if i=="NY":
                        new_row.append(dict_state[i])
                        willAdd=True
                    elif i in dict_state:
                        new_row.append(dict_state[i])
                       # print "state-willAdd2"
                    else:
                        if i in dict_state_tmp:
                            new_row.append(dict_state_tmp[i])
                        else:
                            new_row.append(o_state)
                            dict_state_tmp[i] = o_state
                            o_state = o_state+1
                c = c + 1

            # print row;
            if willAdd==True:
                row_data.append(new_row)
                num_rows = num_rows + 1
                count = count + 1
                #print count
            if num_rows == records_per_page:
                blockcount += 1
                print count,lineCount
                sdb.write(row_data)
                num_rows = 0
                row_data = []
            
        if row_data != []:
            sdb.write(row_data)
            blockcount + 1
            print blockcount
    print count
    sdb.close()

def gen_sensor_data(
    dirpath,
    schema,
    args,
    db_index,
    ):
    global index
    global airports
    if flight:
        read_airport_names()
    sdb_path = os.path.join(dirpath, args.sdb_fname + '_'
                            + str(db_index))
    sdb = data_utils.SDB(sdb_path, schema)
    sdb.open('w')
    records_per_page = sdb.records_per_page()
    num_pages = (args.rows + records_per_page - 1) / records_per_page

    num_rows = 0
    row_data = []
    print records_per_page

#    for row in df.itertuples(index = False):

    flag = True
    blockcount = 0
    count = 0
    with open('/media/srahman7/OS/Users/srahman7/workspace/datarepo/sensordata.csv') as f:
        for line in f:
            if flag:
                flag = False
                continue
            line = line.rstrip('\n')
            row = line.split(',')

            c = 0
            new_row = []
            for i in row:
                if c==0:
                    day_ = int(i)-59+1
                    new_row.append(day_)
                elif c==1:
                    new_row.append(int(i))
                else:
                    new_row.append(float(i))
                c = c + 1

            # print row;

            row_data.append(new_row)
            num_rows = num_rows + 1
            if num_rows == records_per_page:
                blockcount += 1
                print count
                sdb.write(row_data)
                num_rows = 0
                row_data = []
            count = count + 1
        if row_data != []:
            sdb.write(row_data)
            blockcount + 1
            print blockcount

    sdb.close()


class TruncnormWrapper(object):

    def __init__(
        self,
        _min,
        _max,
        mean,
        std,
        ):
        self._mean = mean
        self._std = std
        a = (_min - mean) * 1. / std
        b = (_max - mean) * 1. / std
        self._rv = truncnorm(a, b)

    def rvs(self, size):
        return self._mean + self._std * self._rv.rvs(size=size)


def create_schema(dirpath, args, db_index):
    cols = []
    for i in range(args.dcols):
        distrib_a = 1.5 + 3.5 * uniform().rvs()
        rv = zipf(distrib_a)
        cols.append(data_utils.Column(
            'd' + str(i),
            'ulong',
            'discrete',
            args.disttype,
            rv,
            True,
            {'distrib_a': distrib_a},
            ))

        print 'dcol' + str(i) + ' done'

    for i in range(args.ccols):
        distrib_min = 0
        distrib_max = 1
        distrib_mean = .2 + .6 * uniform().rvs()
        distrib_std = 0.1
        rv = TruncnormWrapper(distrib_min, distrib_max, distrib_mean,
                              distrib_std)
        cols.append(data_utils.Column(
            'c' + str(i),
            'double',
            'continuous',
            'trucnorm',
            rv,
            False,
            {
                'distrib_min': 0,
                'distrib_max': 1,
                'distrib_mean': distrib_mean,
                'distrib_std': distrib_std,
                },
            ))
        print 'ccol ' + str(i) + ' done'

    schema_path = os.path.join(dirpath, args.schema_fname + '_'
                               + str(db_index))
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
    parser.add_argument('--dbcount', type=int, default=DBCOUNT)
    parser.add_argument('--min_distinct', type=int,
                        default=MIN_DISTINCT)
    parser.add_argument('--max_distinct', type=int,
                        default=MAX_DISTINCT)
    parser.add_argument('--sdb_fname', default=SDB_FNAME)
    parser.add_argument('--disttype', default=DISTTYPE)
    parser.add_argument('--density', type=float, default=DENSITY)
    parser.add_argument('--schema_fname', default=SCHEMA_FNAME)
    parser.add_argument('--real_data', default=None)

    args = parser.parse_args()
    args.max_distinct = max(args.min_distinct, args.max_distinct)
    return args


def main():
    args = get_args()
    dirpath = (gen_dirpath(args) if args.dirpath
               is None else args.dirpath)
    print 'Generated: %s' % dirpath

    for db_index in range(2, 3):
        schema = create_schema(dirpath, args, db_index)
        print 'schema done'
        #gen_data(dirpath, schema, args, db_index)
        #gen_flight_data_shrink(dirpath, schema, args, db_index)
        gen_sensor_data(dirpath, schema, args, db_index)
        schema.save()


if __name__ == '__main__':
    main()
