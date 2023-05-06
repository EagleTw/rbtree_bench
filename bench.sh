#!/bin/sh

./rb/build/rb_prof | sed -e 's/^/rb, /' > bench.txt
#./map/build/
