#!/bin/sh

./map-linux/build/bench-map-linux-random | sed -e 's/^/old-map, /' > bench.txt
./map-linux/build/bench-map-linux-sequential | sed -e 's/^/old-map, /' >> bench.txt
./map-jemalloc/build/bench-map-jemalloc-random | sed -e 's/^/proposed-map, /' >> bench.txt
./map-jemalloc/build/bench-map-jemalloc-sequential | sed -e 's/^/proposed-map, /' >> bench.txt

./plot.py
