#include <stdio.h>

#include "global.h"
#include "operators.h"

// Parametros del AE

int pop_size = 8;
int cross_percentage = 60;
int mut_percentage = 5;

// Parametros del problema

int len_chrom = 7;

int values[7] = {40, 60, 10, 10, 3, 20, 60};
int weights[7] = {40, 50, 30, 10, 10, 40, 30};

int weight_limit = 100;

int main() {
  chrom *population = NULL;
  int iterations;

  printf("Ingrese numero de iteraciones: ");
  scanf("%d", &iterations);

  initialize_population(&population);

  printf("Generacion 0:\n");

  show_population(population);

  for (int i = 0; i < iterations; i++)
    calculate_next_pop(&population);

  printf("\nGeneracion %d:\n", iterations);
  show_population(population);

  delete_population(&population);

  return 1;
}
