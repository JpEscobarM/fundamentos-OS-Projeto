//
// Created by jmartins on 31/03/2026.
//

#ifndef FILE_H
#define FILE_H
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "city.h"
#include <types.h>

/**
 * Abre um arquivo utilizando fopen e encerra o programa caso ocorra erro.
 * Opcionalmente registra uma mensagem de log indicando que o arquivo foi aberto.
 *
 * @param FILE_PATH Caminho do arquivo a ser aberto.
 * @param mode Modo de abertura do arquivo (ex: "r", "w", "a", "rb","wb").
 * @param log Se 1 então imprime uma mensagem informando que o arquivo foi aberto.
 *
 * @return Ponteiro para o arquivo aberto (FILE*).
 *
 * @note O programa é finalizado com EXIT_FAILURE caso o arquivo não possa ser aberto.
 */
FILE *open_file(char *FILE_PATH,char *mode, int log);

/**
 *Retorna o o tamanho do arquivo em bytes
 * @param file Ponteiro para o arquivo do tipo (FILE*)
 * @param log Se 1 então Exibe os logs durante a execução da função
 * @return quantidade de bytes lidos do tipo (long)
 */
long tamanho_total_arquivo(FILE *file,int log);

/**
 *Le um arquivo .json e retorna um buffer do tipo char* do arquivo completo.
 * @param FILE_PATH Caminho do arquivo a ser aberto.
 * @param mode Modo de abertura do arquivo (ex: "r", "w", "a", "rb","wb").
 * @param log Se 1 então Exibe os logs durante a execução da função
 * @return
 */
char *read_with_buffer(char *FILE_PATH,char *mode, int log);

/**
 * Converte um buffer contendo texto JSON em uma estrutura cJSON.
 *
 * A função utiliza cJSON_Parse para transformar o conteúdo do buffer
 * em um objeto cJSON que pode ser manipulado posteriormente no programa.
 * Caso o parse falhe, o programa é encerrado com EXIT_FAILURE.
 *
 * Opcionalmente, se o parâmetro log for diferente de 0, o JSON resultante
 * é impresso no terminal em formato formatado utilizando cJSON_Print.
 *
 * @param buffer Ponteiro para o buffer contendo o JSON em formato texto.
 * @param log Se diferente de 0, exibe logs de depuração durante a execução.
 *
 * @return Ponteiro para a estrutura cJSON criada a partir do buffer.
 *
 * @note A memória retornada deve ser liberada posteriormente utilizando
 *       cJSON_Delete() quando o objeto não for mais necessário.
 */
cJSON *cjson_transform(char *buffer, int log);

/**
 * Percorre um array JSON principal e processa os objetos presentes no campo "data"
 * contido dentro de um JSON interno armazenado em formato de string.
 *
 * O campo indicado em `field` deve conter um JSON serializado em string
 * (exemplo: "payload" ou "brute_data"). A função realiza o parse dessa string,
 * localiza o array "data" e percorre cada item desse array.
 *
 * Cada item do array "data" possui a estrutura:
 *
 * {
 *     "variable": "nome_da_variavel",
 *     "value": valor_da_variavel,
 *     "time": "timestamp_da_medicao"
 * }
 *
 * A função percorre esses objetos permitindo processar variáveis como:
 * temperature, humidity, airpressure, batterylevel, rssi, snr, etc.
 *
 * O objetivo é acessar diretamente os dados necessários para cálculo
 * de métricas (ex.: maior/menor temperatura, umidade, consumo de bateria),
 * evitando armazenar todos os payloads ou brute_data em memória.
 *
 * @param json  JSON principal contendo os registros (array de objetos).
 * @param field Nome do campo que contém o JSON interno em formato de string
 *              (exemplo: "payload" ou "brute_data").
 * @param log   Se diferente de 0, habilita mensagens de log para depuração.
 */
void *process_data_items(void *args);


#endif //ANALISE_IOT_FILE_H
