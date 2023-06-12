#!/usr/bin/env python3

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

def trimmed_mean(arr, percent):
    n = len(arr)
    k = int(round(n * (float(percent) / 100) / 2))
    return np.mean(arr[k + 1 : n - k])

def subplots(i, j):
    # subplots with a consistent return type (always a numpy array)
    fig, axs = plt.subplots(i, j)
    if not isinstance(axs, np.ndarray):
        axs = np.asarray([axs])
    return fig, axs

def main():
    print('---------- plot bench ----------')

    '''
    df         0       1          2       3          4    5
        testname time(ns) test_type op_type test_scale reps
    '''
    df = pd.read_table(
        'bench.txt',
        sep=',',
        header=None,
        names=['name', 'time', 'test_type', 'op_type', 'scale', 'reps']
        )

    map_names = df['name'].unique()
    op_types = df['op_type'].unique()
    scales = df['scale'].unique()
    test_types = df['test_type'].unique()

    fig, axs = subplots(1, len(op_types))
    fig.suptitle("Compare propsed and old map average operation time")

    for n_ix, name in enumerate(map_names):
        for test_ix, test_type in enumerate(test_types):
            for o_ix, op_type in enumerate(op_types):
                tavg = [0.0] * len(scales)
                for s_ix, scale in enumerate(scales):
                    data = df.loc[
                        (df['name'] == name)
                        & (df['op_type'] == op_type)
                        & (df['scale'] == scale)
                        & (df['test_type'] == test_type)
                       ]
                    davg = data['time'].mean()
                    tavg[s_ix] = davg
                axs[o_ix].plot(scales, tavg, "o-", label=name + " " + str(test_type) + " operation")
                print(name, test_type, scale, op_type, tavg)
                axs[o_ix].set_xscale("log")
                #axs[o_ix].set_ylim(0, 1800)
                axs[o_ix].set_title(op_type)

    axs[0].set_ylabel("ns/op")
    axs[0].legend()

    plt.show()

    return


if __name__ == '__main__':
    main()
