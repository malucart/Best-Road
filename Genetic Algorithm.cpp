#include <math.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

int main () {
    // generating real random numbers
    srand((unsigned)time(0)); 
    int how_many_cities = 5;
    int distance[5][5] = { {0,2,9,3,6}, {2,0,4,3,8}, {9,4,0,7,3}, {3,3,7,0,3}, {6,8,3,3,0} };
    
    int population_size = 5;
    int number_of_generations = 20;
    int mutation_factor = 1;
    int elites = 1;
    
    int random_number;
    bool add;
    
    int road[population_size][how_many_cities];
    float fitness[population_size];
    float fitness_overall;
    float accumulated_fitness;
    
    float roulette;
    int father1[how_many_cities];
    int father2[how_many_cities];
    int son1[how_many_cities];
    int son2[how_many_cities];
    
    bool right_now1;
    bool right_now2;
    int new_road[population_size][how_many_cities];
    
    int position1;
    int position2;
    int aux;
    
    float biggest_fitness;
    int index_biggest_fitness;
    
    // Initialization
    for (int i = 0; i < population_size; i++) {
        for (int j = 0; j < how_many_cities; j++) {
            random_number = rand() % how_many_cities + 1;
            add = false;
            for (int k = 0; k < j; k++) {
                if (road[i][k] == random_number) {
                    add = true;
                    break;
                }
            }
            if (add == false) {
                road[i][j] = random_number;
            }
        }
    }
    
    // Fitness calculation
    for (int geracao = 0; geracao < number_of_generations; geracao++) {        
        fitness_overall = 0;
        for (int i = 0; i < population_size; i++) {
            fitness[i] = 0;
            for (int j = 0; j < (how_many_cities - 1); j++) {
                fitness[i] = fitness[i] + distance[road[i][j]-1][road[i][j+1]-1];
            }
            fitness[i] = 1/fitness[i];
            fitness_overall = fitness_overall + fitness[i];
        }
        
        // Selection
        for (int i = 0; i < population_size/2; i++) {
            roulette = ((float)rand()/(RAND_MAX+1)) * fitness_overall;
            accumulated_fitness = 0;
            for (int j = 0; j < population_size; j++) {
                accumulated_fitness = accumulated_fitness + fitness[j];
                if (accumulated_fitness > roulette) {
                    for (int k = 0; k < how_many_cities; k++) {
                        father1[k] = road[j][k];
                    }
                    break;
                }
            }
            
            roulette = ((float)rand()/(RAND_MAX+1)) * fitness_overall;
            accumulated_fitness = 0;
            for (int j = 0; j < population_size; j++) {
                accumulated_fitness = accumulated_fitness + fitness[j];
                if (accumulated_fitness > roulette) {
                    for (int k = 0; k < how_many_cities; k++) {
                        father2[k] = road[j][k];
                    }
                    break;
                }
            }
            
            // Reproduction
            for (int j = 0; j < how_many_cities; j++) {
                son1[j] = 0;
                son2[j] = 0;
            }
            for (int j = 0; j < how_many_cities; j++) {
                if (j < how_many_cities/2) {
                    son1[j] = father1[j];
                    son2[j] = father2[j];
                } else {
                    for (int k = 0; k < how_many_cities; k++) {  //Percorrer Pais
                        right_now1 = false;
                        right_now2 = false;
                        for (int l = 0; l < j; l++) {   //Percorrer Filhos
                            if (father2[k] == son1[l]) {
                                right_now1 = true;
                            }
                            if (father1[k] == son2[l]) {
                                right_now2 = true;
                            }
                        }
                        if (right_now1 == false) {
                            son1[j] = father2[k];
                        }
                        if (right_now2 == false) {
                            son2[j] = father1[k];
                        }
                    }
                }
            }
            for (int j = 0; j < how_many_cities; j++) {
                new_road[i][j] = son1[j];
                new_road[population_size-1-i][j] = son2[j];
            } // end of reproduction
        } // end of selection
        
        // Mutation
        for (int i = 0; i < mutation_factor; i++) {
            random_number = rand() % population_size;
            position1 = rand() % how_many_cities;
            do {
                position2 = rand() % how_many_cities;
            } while (position2 == position1);
            aux = new_road[random_number][position1];
            new_road[random_number][position1] = new_road[random_number][position2];
            new_road[random_number][position2] = aux;
        }
        
        // Elitism
        biggest_fitness = -1;
        index_biggest_fitness = -1;
        for (int i = 0; i < population_size; i++) {
            if (fitness[i] > biggest_fitness) {
                biggest_fitness = fitness[i];
                index_biggest_fitness = i;
            }
        }
        random_number = rand() % population_size;
        for (int i = 0; i < how_many_cities; i++) {
            new_road[random_number][i] = road[index_biggest_fitness][i];
        } // end of elitism
        
        for (int i = 0; i < population_size; i++) {
            for (int j = 0; j < how_many_cities; j++) {
                road[i][j] = new_road[i][j];
            }
        }
    }
    
    // Finalization
    for (int i = 0; i < population_size; i++) {
        fitness[i] = 0;
        for (int j = 0; j < (how_many_cities - 1); j++) {
            fitness[i] = fitness[i] + distance[road[i][j]-1][road[i][j+1]-1];
        }
        fitness[i] = 1/fitness[i];
    }
    biggest_fitness = -1;
    index_biggest_fitness = -1;
    for (int i = 0; i < population_size; i++) {
        if (fitness[i] > biggest_fitness) {
            biggest_fitness = fitness[i];
            index_biggest_fitness = i;
        }
    }
    
    cout << "The best road:" << endl;
    for (int i = 0; i < how_many_cities; i++) {
        if (i == how_many_cities-1) {
            cout << road[index_biggest_fitness][i] << endl;
        } else {
            cout << road[index_biggest_fitness][i] << " -> ";
        }
    }
    cout << "Cost of the road: " << 1/fitness[index_biggest_fitness] << endl;
}
