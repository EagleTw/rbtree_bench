#!/bin/sh

./rb/build/bench-rb | sed -e 's/^/rb, /' > bench.txt
./map/build/bench-map | sed -e 's/^/map, /' >> bench.txt

./plot.py
