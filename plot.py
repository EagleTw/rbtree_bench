#!/usr/bin/env python3

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


def main():
    print('---------- plot bench ----------')

    '''
    df         0       1       2        3          4    5
        testname time(ns) test_id op_type test_scale reps
    '''
    df = pd.read_table('bench.txt', sep=',', header=None)

    #TODO: Handle dataframe and plot figure

    return

if __name__ == '__main__':
    main()