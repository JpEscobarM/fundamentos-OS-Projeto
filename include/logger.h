#ifndef LOGGER_H
#define LOGGER_H

#include <pthread.h>

// Inicializa a thread de log e as estruturas de sincronização
void inicializar_logger();

// Envia uma mensagem para a fila de logs
void registrar_log(const char *formato, ...);

// Encerra a thread de log, garantindo que todas as mensagens sejam gravadas
void fechar_logger();


void get_timestamp(char *buffer, size_t size);

#endif // LOGGER_H
