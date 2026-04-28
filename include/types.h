//
// Created by jmartins on 02/04/2026.
//

#ifndef ANALISE_IOT_TYPES_H
#define ANALISE_IOT_TYPES_H
#include "cJSON.h"
/**
 * Estrutura criada para guardar as informações das medições das Cidades, de acordo com os requisitos C),D) e E).
 */
typedef struct
{
    char nomeCidade[64];

    //c)
    double maiorTemperatura;
    char dataMaiorTemperatura[64];
    double menorTemperatura;
    char dataMenorTemperatura[64];


    //d)
    double maiorUmidade;
    char dataMaiorUmidade[64];
    double menorUmidade;
    char dataMenorUmidade[64];

    //e)
    double maiorPressaoAtmosferica;
    char dataMaiorPressaoAtmosferica[64];
    double menorPressaoAtmosferica;
    char dataMenorPressaoAtmosferica[64];

    double somaTemperatura;
    double somaUmidade;
    double somaPressao;

    int contTemperatura;
    int contUmidade;
    int contPressao;

    //g) Bateria
    double bateriaInicial;          // primeiro valor de tensão registrado (timestamp mais antigo)
    char dataBateriaInicial[64];    // data/hora da primeira medição de bateria
    double bateriaFinal;            // último valor de tensão registrado (timestamp mais recente)
    char dataBateriaFinal[64];      // data/hora da última medição de bateria
    int bateriaInicialSet;          // flag: 1 se já foi atribuído o valor inicial

    int spreadingFactors[16];
    int qtdSpreadingFactors;

} Cidade;

typedef struct
{
    int id;
    int inicio;
    int fim;
    int file_index;
    char field[24];
    char nome_arquivo[256];
    cJSON *json; // precisa de um ponteiro para o json que irá percorrer



}City_param_t;



#endif //ANALISE_IOT_TYPES_H