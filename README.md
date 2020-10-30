# o-research

An Operations Research repo that contains implementations of the *Travelling Salesman Problem* (TSP) and *Minimum Latency Problem* (MLP) solvers. Current implemented solving strategies are the [GILS-RVND](https://www.sciencedirect.com/science/article/abs/pii/S037722171200269X) metaheuristic and the Branch and Bound algorithm.


## Compilation

To compile this code, you just need to tu run the following command:
```shell
$ make
```

## Execution

In order to solve an instance, run the command below:
```shell
$ ./solver path/to/instance.tsp --[mlp/tsp/bb] [-b]
```

### Execution Parameters

- --mlp: Will solve the instance as a MLP trough the use of the GILS-RVND metaheuristic.

- --tsp: Will solve the instance as a TSP trough the use of the GILS-RVND metaheuristic.

- --bb: Will solve the instance as a TSP trough the use of the Branch and Bound algorithm.

- -b: Will execute the code in *benchmark* mode, returning some useful metrics about execution time and results.
