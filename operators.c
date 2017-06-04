#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "global.h"
#include "operators.h"

void  initialize_population(chrom **population) {
  chrom *tmp_pop = (chrom *)malloc(pop_size * sizeof(chrom));

  for (int i = 0; i < pop_size; i++) {
    for (int j = 0; j < len_chrom; j++)
      tmp_pop[i].bit[j] = rand() % 2;
    calculate_chrom_fitness(&tmp_pop[i]);
  }

  *population = tmp_pop;
}

void calculate_next_pop(chrom **population) {
  // Primero creamos a la poblacion intermedia resultante despues de la seleccion con la ruleta

  int *acum_fit = (int *)malloc(pop_size * sizeof(int));
  int index, selector, best_sol, acumulator = 0;

  index = 0;
  best_sol = (*population)[0].fit;

  // Aqui aprovechamos de calcular la sumatoria de los fitnes para la seleccion posterior
  for (int i = 0; i < pop_size; i++) {
    acumulator += (*population)[i].fit;
    acum_fit[i] = acumulator;

    // Tambien aprovecharemos de ver cual es el indice de la mejor solucion
    if ((*population)[i].fit > best_sol) {
      best_sol = (*population)[i].fit;
      index = i;
    }
  }

  // Aqui intercambiamos de posicion del mejor cromosoma con el primero

  chrom aux_chrom;

  memcpy(&aux_chrom, &(*population)[0], sizeof(chrom));
  memcpy(&(*population)[0], &(*population)[index], sizeof(chrom));
  memcpy(&(*population)[index], &aux_chrom, sizeof(chrom));

  chrom *tmp_pop = (chrom *)malloc(pop_size * sizeof(chrom));

  // En la poblacion intermedia pondremos automaticamente la mejor solucion de la poblacion original (que estaba en la primera posicion)

  memcpy(&tmp_pop[0], &(*population)[0], sizeof(chrom));

  // Aqui empezamos a seleccionar y poner los cromosoma en la poblacion intermedia dependiendo del fitness y de los numeros random generados para "simular" la ruleta

  for (int i = 1; i < pop_size; i++) {
    selector = rand() % acumulator + 1;
    index = 0;

    while (acum_fit[index] < selector)
      index++;

    memcpy(&tmp_pop[i], &(*population)[index], sizeof(chrom));
  }

  free(acum_fit);
  delete_population(&(*population));

  // En este punto tenemos en tmp_pop la poblacion intermedia, ahora falta hacer el cruzamiento (recordar que se hara desde el segundo cromosoma en adelante)

  chrom *father_1 = NULL;
  chrom *father_2 = NULL;

  int tmp, partition;

  for (int i = 1; i < pop_size; i++) {
    if (cross_percentage > rand() % 100) {
      if (father_1 == NULL)
        father_1 = &tmp_pop[i];
      else {
        father_2 = &tmp_pop[i];

        partition = rand() % 6;

        for (int i = 0; i <= partition; i++) {
          tmp = father_1->bit[i];
          father_1->bit[i] = father_2->bit[i];
          father_2->bit[i] = tmp;
        }

        calculate_chrom_fitness(father_1);
        calculate_chrom_fitness(father_2);

        father_1 = father_2 = NULL;
      }
    }
  }

  // Aqui haremos la mutacion (recordar que se hara desde el segundo cromosoma en adelante)

  bool changed;

  for (int i = 1; i < pop_size; i++) {
    changed = false;

    for (int j = 0; j < len_chrom; j++)
      if (mut_percentage > rand() % 100) {
        tmp_pop[i].bit[j] = rand() % 2;
        changed = true;
      }

    if (changed)
      calculate_chrom_fitness(&tmp_pop[i]);
  }

  (*population) = tmp_pop;
}

void delete_population(chrom **population) {
  if (*population) {
    free(*population);
    *population = NULL;
  }
}

void calculate_chrom_fitness(chrom *chromosome) {
  int sum_weight = 0, sum_fitness = 0, penalization = 0;

  for (int j = 0; j < len_chrom; j++) {
    sum_weight += chromosome->bit[j] * weights[j];
    sum_fitness += chromosome->bit[j] * values[j];
  }

  // Si rompenos la restriccion introducimos una penalizacion
  if (sum_weight > weight_limit) {
    penalization = (weight_limit - sum_weight);
    sum_fitness += 2 * penalization;
    chromosome->factible = false;
  } else
    chromosome->factible = true;

  chromosome->fit = sum_fitness;
}

void show_population(chrom *population) {
  int min, max, acumulator = 0;
  float average;

  min = max = population[0].fit;

  for (int i = 0; i < pop_size; i++) {
    acumulator += population[i].fit;

    show_chromosome(&population[i]);

    if (population[i].fit > max)
      max = population[i].fit;

    if (population[i].fit < min)
      min = population[i].fit;
  }

  average = acumulator / (float) pop_size;

  printf("\nEl Min es: %d, el Max es: %d y el promedio es: %f\n", min, max, average);
}

void show_chromosome(chrom *c) {
  for (int i = 0; i < len_chrom; i++)
    printf("[%d]", c->bit[i]);
  printf(" = %3d | ", c->fit);

  if (c->factible)
    printf("Factible\n");
  else
    printf("No Factible\n");
}
