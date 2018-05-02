
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

int main () {
	srand((unsigned)time(0)); //Para gerar números aleatórios reais.
    int quantia_de_cidades = 5;
	int distancia[5][5] = { {0,2,9,3,6}, {2,0,4,3,8}, {9,4,0,7,3}, {3,3,7,0,3}, {6,8,3,3,0} };
	
	int tamanho_da_populacao = 5;
	int numero_de_geracoes = 20;
	int fator_de_mutacao = 1;
	int elites = 1;
	
	int valor_aleatorio;
	bool adicionada;
	
	int caminhos[tamanho_da_populacao][quantia_de_cidades];
	float aptidao[tamanho_da_populacao];
	float aptidao_total;
	float aptidao_acumulada;
	
	float roleta;
	int pai1[quantia_de_cidades];
	int pai2[quantia_de_cidades];
	int filho1[quantia_de_cidades];
	int filho2[quantia_de_cidades];
	
	bool presente1;
	bool presente2;
	int novos_caminhos[tamanho_da_populacao][quantia_de_cidades];
	
	int posicao1;
	int posicao2;
	int auxiliar;
	
	float maior_aptidao;
	int indice_maior_aptidao;
	
	//Inicialização
	for (int i = 0; i < tamanho_da_populacao; i++) {
		for (int j = 0; j < quantia_de_cidades; j++) {
			valor_aleatorio = rand() % quantia_de_cidades + 1;
			adicionada = false;
			for (int k = 0; k < j; k++) {
				if (caminhos[i][k] == valor_aleatorio) {
					adicionada = true;
					break;
				}
			}
			if (adicionada == false) {
				caminhos[i][j] = valor_aleatorio;
			}
		}
	}
	//FIM Inicialização
	
	for (int geracao = 0; geracao < numero_de_geracoes; geracao++) {		
		//Cálculo de Aptidão
		aptidao_total = 0;
		for (int i = 0; i < tamanho_da_populacao; i++) {
			aptidao[i] = 0;
			for (int j = 0; j < (quantia_de_cidades - 1); j++) {
				aptidao[i] = aptidao[i] + distancia[caminhos[i][j]-1][caminhos[i][j+1]-1];
			}
			aptidao[i] = 1/aptidao[i];
			aptidao_total = aptidao_total + aptidao[i];
		}
		//FIM Cálculo de Aptidão
	    
		//Seleção
		for (int i = 0; i < tamanho_da_populacao/2; i++) {
		    roleta = ((float)rand()/(RAND_MAX+1)) * aptidao_total;
		    aptidao_acumulada = 0;
		    for (int j = 0; j < tamanho_da_populacao; j++) {
		        aptidao_acumulada = aptidao_acumulada + aptidao[j];
		        if (aptidao_acumulada > roleta) {
		            for (int k = 0; k < quantia_de_cidades; k++) {
		                pai1[k] = caminhos[j][k];
		            }
		            break;
		        }
		    }
		    
		    roleta = ((float)rand()/(RAND_MAX+1)) * aptidao_total;
		    aptidao_acumulada = 0;
		    for (int j = 0; j < tamanho_da_populacao; j++) {
		        aptidao_acumulada = aptidao_acumulada + aptidao[j];
		        if (aptidao_acumulada > roleta) {
		            for (int k = 0; k < quantia_de_cidades; k++) {
		                pai2[k] = caminhos[j][k];
		            }
		            break;
		        }
		    }
		    
		    //Reprodução
		    for (int j = 0; j < quantia_de_cidades; j++) {
		        filho1[j] = 0;
		        filho2[j] = 0;
		    }
		    for (int j = 0; j < quantia_de_cidades; j++) {
		        if (j < quantia_de_cidades/2) {
		            filho1[j] = pai1[j];
		            filho2[j] = pai2[j];
		        } else {
		            for (int k = 0; k < quantia_de_cidades; k++) {  //Percorrer Pais
		                presente1 = false;
		                presente2 = false;
		                for (int l = 0; l < j; l++) {   //Percorrer Filhos
            				if (pai2[k] == filho1[l]) {
            					presente1 = true;
            				}
            				if (pai1[k] == filho2[l]) {
            					presente2 = true;
            				}
		                }
		                if (presente1 == false) {
            				filho1[j] = pai2[k];
            			}
		                if (presente2 == false) {
            				filho2[j] = pai1[k];
            			}
		            }
		        }
		    }
		    for (int j = 0; j < quantia_de_cidades; j++) {
		        novos_caminhos[i][j] = filho1[j];
		        novos_caminhos[tamanho_da_populacao-1-i][j] = filho2[j];
		    }
		    //FIM Reprodução
		}
		//FIM Seleção
		
		//Mutação
		for (int i = 0; i < fator_de_mutacao; i++) {
		    valor_aleatorio = rand() % tamanho_da_populacao;
		    posicao1 = rand() % quantia_de_cidades;
		    do {
		        posicao2 = rand() % quantia_de_cidades;
		    } while (posicao2 == posicao1);
		    auxiliar = novos_caminhos[valor_aleatorio][posicao1];
		    novos_caminhos[valor_aleatorio][posicao1] = novos_caminhos[valor_aleatorio][posicao2];
		    novos_caminhos[valor_aleatorio][posicao2] = auxiliar;
		}
		//FIM Mutação
		
		//Elitismo
		maior_aptidao = -1;
		indice_maior_aptidao = -1;
		for (int i = 0; i < tamanho_da_populacao; i++) {
		    if (aptidao[i] > maior_aptidao) {
		        maior_aptidao = aptidao[i];
		        indice_maior_aptidao = i;
		    }
		}
		valor_aleatorio = rand() % tamanho_da_populacao;
		for (int i = 0; i < quantia_de_cidades; i++) {
		    novos_caminhos[valor_aleatorio][i] = caminhos[indice_maior_aptidao][i];
		}
		//FIM Elitismo
		
		for (int i = 0; i < tamanho_da_populacao; i++) {
		    for (int j = 0; j < quantia_de_cidades; j++) {
		        caminhos[i][j] = novos_caminhos[i][j];
		    }
		}
	}
	
	//Finalização
	for (int i = 0; i < tamanho_da_populacao; i++) {
		aptidao[i] = 0;
		for (int j = 0; j < (quantia_de_cidades - 1); j++) {
			aptidao[i] = aptidao[i] + distancia[caminhos[i][j]-1][caminhos[i][j+1]-1];
		}
		aptidao[i] = 1/aptidao[i];
	}
    maior_aptidao = -1;
    indice_maior_aptidao = -1;
    for (int i = 0; i < tamanho_da_populacao; i++) {
        if (aptidao[i] > maior_aptidao) {
            maior_aptidao = aptidao[i];
            indice_maior_aptidao = i;
        }
    }
    
    cout << "Melhor caminho encontrado:" << endl;
    for (int i = 0; i < quantia_de_cidades; i++) {
        if (i == quantia_de_cidades-1) {
            cout << caminhos[indice_maior_aptidao][i] << endl;
        } else {
            cout << caminhos[indice_maior_aptidao][i] << " -> ";
        }
    }
    cout << "Custo do caminho: " << 1/aptidao[indice_maior_aptidao] << endl;
    //FIM Finalização (?)

}
