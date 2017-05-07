# GA_NN_experiment
Experimenting with genetic algorithms and neural networks

Compilation : gcc -g -c nn.c -I./ && gcc -c creature.c -I./ && gcc -o ga ga.c -I./ nn.o creature.o -lncurses -lm

Quick and dirty, from scratch implementation of genetic algorithm on creatures with simple fully connected neural network

Depedencies
--------

- ncurses
