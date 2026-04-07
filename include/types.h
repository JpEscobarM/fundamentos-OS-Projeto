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

} Cidade;

typedef struct
{
    int inicio;
    int fim;
    char field[24];
    cJSON *json; // precisa de um ponteiro para o json que irá percorrer

}City_param_t;



#endif //ANALISE_IOT_TYPES_H