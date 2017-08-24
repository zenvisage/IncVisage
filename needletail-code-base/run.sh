#!/bin/bash

# Flight Dataset
#nohup ./needletail -r seq -q /home/srahman7/needletail-code-base/scripts/q -b ewah -m flight -n 10000 -d 0.1  -x 1.0 -t  clustered /home/srahman7/needletail-code-base/data/sdb_1 /home/srahman7/needletail-code-base/data/schema_1 &

# FLIGHT -- DISK SCAN
#./needletail -r seq -q /home/srahman7/needletail-code-base/scripts/q -b noindex -m flight -n 74324253 -d 0.1  -x 1.0 -t  clustered /home/srahman7/needletail-code-base/data/sdb_1 /home/srahman7/needletail-code-base/data/schema_1

# FLIGHT -- BITMAP SCAN
#nohup ./needletail -r scan -q /home/srahman7/needletail-code-base/scripts/q -b ewah -m flight -n 74324253 -d 0.1  -x 1.0 -t  clustered /home/srahman7/needletail-code-base/data/sdb_1 /home/srahman7/needletail-code-base/data/schema_1 &




# T11 Dataset
#ionice -c2 -n0 ./needletail -r seq -q /home/srahman7/needletail-code-base/scripts/q -b single -m T11 -n 10000 -d 0.1  -x 1.0 -t  clustered /home/srahman7/needletail-code-base/data/sdb_2 /home/srahman7/needletail-code-base/data/schema_2

# T11 -- DISK SCAN
#./needletail -r seq -q /home/srahman7/needletail-code-base/scripts/q -b noindex -m T11 -n 170523443 -d 0.1  -x 1.0 -t  clustered /home/srahman7/needletail-code-base/data/sdb_2 /home/srahman7/needletail-code-base/data/schema_2

# T11 -- BITMAP SCAN
#./needletail -r scan -q /home/srahman7/needletail-code-base/scripts/q -b single -m T11 -n 170523443 -d 0.1  -x 1.0 -t  clustered /home/srahman7/needletail-code-base/data/sdb_2 /home/srahman7/needletail-code-base/data/schema_2




# T12 Dataset
#ionice -c2 -n0 ./needletail -r seq -q /home/srahman7/needletail-code-base/scripts/q -b single -m T12 -n 10000 -d 0.1  -x 1.0 -t  clustered /home/srahman7/needletail-code-base/data/sdb_3 /home/srahman7/needletail-code-base/data/schema_3

# T12 -- DISK SCAN
#./needletail -r seq -q /home/srahman7/needletail-code-base/scripts/q -b noindex -m T12 -n 166755863 -d 0.1  -x 1.0 -t  clustered /home/srahman7/needletail-code-base/data/sdb_3 /home/srahman7/needletail-code-base/data/schema_3


# T12 -- BITMAP SCAN
#./needletail -r scan -q /home/srahman7/needletail-code-base/scripts/q -b single -m T12 -n 166755863 -d 0.1  -x 1.0 -t  clustered /home/srahman7/needletail-code-base/data/sdb_3 /home/srahman7/needletail-code-base/data/schema_3




# T13 Dataset
#ionice -c2 -n0 ./needletail -r seq -q /home/srahman7/needletail-code-base/scripts/q -b single -m T13 -n 10000 -d 0.1  -x 1.0 -t  clustered /home/srahman7/needletail-code-base/data/sdb_4 /home/srahman7/needletail-code-base/data/schema_4

# T13 -- DISK SCAN
#./needletail -r seq -q /home/srahman7/needletail-code-base/scripts/q -b noindex -m T13 -n 161911174 -d 0.1  -x 1.0 -t  clustered /home/srahman7/needletail-code-base/data/sdb_4 /home/srahman7/needletail-code-base/data/schema_4


# T13 -- BITMAP SCAN
#./needletail -r scan -q /home/srahman7/needletail-code-base/scripts/q -b single -m T13 -n 161911174 -d 0.1  -x 1.0 -t  clustered /home/srahman7/needletail-code-base/data/sdb_4 /home/srahman7/needletail-code-base/data/schema_4



# Wunder Dataset
#ionice -c2 -n0 ./needletail -r seq -q /home/srahman7/needletail-code-base/scripts/q -b single -m Wunder -n 10000 -d 0.1  -x 1.0 -t  clustered /home/srahman7/needletail-code-base/data/sdb_4 /home/srahman7/needletail-code-base/data/schema_4

# Wunder -- DISK SCAN
#./needletail -r seq -q /home/srahman7/needletail-code-base/scripts/q -b noindex -m Wunder -n 415715600 -d 0.1  -x 1.0 -t  clustered /home/srahman7/needletail-code-base/data/sdb_4 /home/srahman7/needletail-code-base/data/schema_4


# Wunder -- BITMAP SCAN
#./needletail -r scan -q /home/srahman7/needletail-code-base/scripts/q -b single -m Wunder -n 415715600 -d 0.1  -x 1.0 -t  clustered /home/srahman7/needletail-code-base/data/sdb_4 /home/srahman7/needletail-code-base/data/schema_4
