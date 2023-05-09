#!/bin/sh

./rb/build/rb_bench | sed -e 's/^/rb, /' > bench.txt
./map/build/map_bench | sed -e 's/^/map, /' >> bench.txt

./plot.py
