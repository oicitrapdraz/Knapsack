#ifndef OPERATORS_H

#include "chromosome.h"

void initialize_population(chrom **population);

void calculate_next_pop(chrom **population);

void delete_population(chrom **population);

void show_population(chrom *population);

void show_chromosome(chrom *c);

void calculate_chrom_fitness(chrom *population);

#endif
