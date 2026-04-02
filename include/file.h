//
// Created by jmartins on 31/03/2026.
//

#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include <pthread.h>
#include <semaphore.h>

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
 *
 * @param file Ponteiro para o arquivo do tipo (FILE*)
 * @param log Se 1 então Exibe os logs durante a execução da função
 * @return quantidade de bytes lidos do tipo (long)
 */
long tamanho_total_arquivo(FILE *file,int log);

/**
 *
 * @param FILE_PATH Caminho do arquivo a ser aberto.
 * @param mode Modo de abertura do arquivo (ex: "r", "w", "a", "rb","wb").
 * @param log Se 1 então Exibe os logs durante a execução da função
 * @return
 */
char *read_with_buffer(char *FILE_PATH,char *mode, int log);

#endif //ANALISE_IOT_FILE_H
