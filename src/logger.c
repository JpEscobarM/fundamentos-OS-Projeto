#include "../include/logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define MAX_MENSAGENS_LOG 1024
#define TAMANHO_MENSAGEM_LOG 256

static char fila_log[MAX_MENSAGENS_LOG][TAMANHO_MENSAGEM_LOG];
static int inicio_fila = 0;
static int fim_fila = 0;
static int total_fila = 0;
static int logger_rodando = 0;

static pthread_t thread_logger;
static pthread_mutex_t mutex_logger = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond_logger = PTHREAD_COND_INITIALIZER;

static void *tarefa_logger(void *arg) {
    FILE *arquivo_log = fopen("auditoria.log", "a");
    if (!arquivo_log) {
        perror("Erro ao abrir auditoria.log");
        pthread_exit(NULL);
    }

    while (1) {
        pthread_mutex_lock(&mutex_logger);

        // Espera enquanto a fila estiver vazia e a thread estiver rodando
        while (total_fila == 0 && logger_rodando) {
            pthread_cond_wait(&cond_logger, &mutex_logger);
        }

        // Se a fila estiver vazia e foi solicitado para parar, sai do loop
        if (total_fila == 0 && !logger_rodando) {
            pthread_mutex_unlock(&mutex_logger);
            break;
        }

        // Pega a mensagem da fila
        char mensagem[TAMANHO_MENSAGEM_LOG];
        strncpy(mensagem, fila_log[fim_fila], TAMANHO_MENSAGEM_LOG);
        fim_fila = (fim_fila + 1) % MAX_MENSAGENS_LOG;
        total_fila--;

        pthread_mutex_unlock(&mutex_logger);

        // Grava no arquivo
        fprintf(arquivo_log, "%s\n", mensagem);
        fflush(arquivo_log);
    }

    fclose(arquivo_log);
    return NULL;
}

void inicializar_logger() {
    logger_rodando = 1;
    if (pthread_create(&thread_logger, NULL, tarefa_logger, NULL) != 0) {
        perror("Erro ao criar a thread de log\n");
        exit(EXIT_FAILURE);
    }
}

void registrar_log(const char *formato, ...) {
    char mensagem_temp[TAMANHO_MENSAGEM_LOG];
    va_list args;
    va_start(args, formato);
    vsnprintf(mensagem_temp, TAMANHO_MENSAGEM_LOG, formato, args);
    va_end(args);

    pthread_mutex_lock(&mutex_logger);
    if (total_fila < MAX_MENSAGENS_LOG) {
        strncpy(fila_log[inicio_fila], mensagem_temp, TAMANHO_MENSAGEM_LOG);
        inicio_fila = (inicio_fila + 1) % MAX_MENSAGENS_LOG;
        total_fila++;
        pthread_cond_signal(&cond_logger);
    } else {
        // Fila cheia: opção de descartar ou bloquear (aqui descarta e imprime alerta)
        fprintf(stderr, "Aviso: Fila de logs cheia. Mensagem ignorada: %s\n", mensagem_temp);
    }
    pthread_mutex_unlock(&mutex_logger);
}

void fechar_logger() {
    pthread_mutex_lock(&mutex_logger);
    logger_rodando = 0;
    pthread_cond_signal(&cond_logger);
    pthread_mutex_unlock(&mutex_logger);

    pthread_join(thread_logger, NULL);
}
