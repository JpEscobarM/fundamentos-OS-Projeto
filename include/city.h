//
// Created by jmartins on 07/04/2026.
//

#ifndef ANALISE_IOT_CITY_H
#define ANALISE_IOT_CITY_H
#include "types.h"
#include "stdio.h"
#include <float.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>


#define N_THREADS 6
#define CITY_CAXIAS 0
#define CITY_BENTO 1
#define PORCENTAGEM_LOG 25

extern int N_ELEMENTOS_FILE_1; //json_mqtt_senzemo_cx_bg
extern int N_ELEMENTOS_FILE_2;//json_senzemo_cx_bg



/**
 * variavel para guardar o Maior/Menor valor das medições de Caxias

 */
extern Cidade caxias;

/**
 * variavel para guardar o Maior/Menor valor das medições de Bento
 */
extern Cidade bento;

/**
 * Recebe uma data sem formatação vinda do JSON e formata no modelo dd-mm-yyyy hh:mm:ss
 * @param aUnformattedDate uma data no formato bruto (yyyy-mm-ddThh:mm:ss)
 * @param out a data de input formatada (dd-mm-yyyy hh:mm:ss)
 * @return
 */
void format_date(char *aUnformattedDate, char *out);

/**
 * Inicializa os parâmetros de processamento para cada thread responsável
 * por percorrer os JSONs contendo as medições das cidades.
 *
 * A função calcula o tamanho de cada JSON e divide os elementos entre
 * as threads disponíveis para processamento.
 *
 * Estratégia utilizada:
 * - A thread de índice 0 é responsável por processar completamente o
 *   JSON do arquivo 1 (json_file_1), que possui tamanho significativamente
 *   menor.
 * - As demais threads (1 até N_THREADS-1) dividem entre si os elementos
 *   do JSON do arquivo 2 (json_file_2), que possui maior volume de dados.
 *
 * Para cada thread são definidos:
 * - o índice inicial do trecho do JSON que ela deve processar (`inicio`)
 * - o índice final exclusivo (`fim`)
 * - o ponteiro para o JSON correspondente (`json`)
 *
 * Dessa forma cada thread percorre apenas sua fatia do vetor JSON,
 * evitando sobreposição de trabalho e permitindo processamento paralelo.
 *
 * @param json_file_1 ponteiro para o cJSON contendo os dados do primeiro arquivo
 * @param json_file_2 ponteiro para o cJSON contendo os dados do segundo arquivo
 */
void init_city_thread_params(cJSON *json_file_1, cJSON *json_file_2);

/**
 * Exibe os parametros e os seus respectivos arquivos do vetor de parametros
 * city_process_params
 */
void show_city_thread_params();

/**
 * encerra os mutex utilizados para as cidades Caxias e Bento
 */
void destroy_city_locks();

/**
 * Atualiza os valores de temperatura máxima e mínima da cidade informada.
 *
 * A função protege a região crítica com mutex, garantindo acesso exclusivo
 * à estrutura compartilhada da cidade durante a atualização.
 * Se o valor recebido for maior que a temperatura máxima atual, atualiza
 * o valor e registra a data correspondente. Se for menor que a mínima atual,
 * também atualiza o valor e sua respectiva data.
 *
 * @param city cidade a ser atualizada: CITY_CAXIAS ou CITY_BENTO
 * @param value valor de temperatura lido do JSON
 * @param valueDate data bruta associada à medição, no formato ISO
 */
void city_update_temperature(int city, double value,  char *valueDate);


/**
 * Atualiza os valores de umidade máxima e mínima da cidade informada.
 *
 * A função protege a região crítica com mutex, garantindo acesso exclusivo
 * à estrutura compartilhada da cidade durante a atualização.
 * Se o valor recebido for maior que a umidade máxima atual, atualiza
 * o valor e registra a data correspondente. Se for menor que a mínima atual,
 * também atualiza o valor e sua respectiva data.
 *
 * @param city cidade a ser atualizada: CITY_CAXIAS ou CITY_BENTO
 * @param value valor de umidade lido do JSON
 * @param valueDate data bruta associada à medição, no formato ISO
 */
void city_update_humidity(int city, double value,  char *valueDate);

/**
 * Atualiza os valores de pressão atmosférica máxima e mínima da cidade informada.
 *
 * A função protege a região crítica com mutex, garantindo acesso exclusivo
 * à estrutura compartilhada da cidade durante a atualização.
 * Se o valor recebido for maior que a pressão máxima atual, atualiza
 * o valor e registra a data correspondente. Se for menor que a mínima atual,
 * também atualiza o valor e sua respectiva data.
 *
 * @param city cidade a ser atualizada: CITY_CAXIAS ou CITY_BENTO
 * @param value valor de pressão atmosférica lido do JSON
 * @param valueDate data bruta associada à medição, no formato ISO
 */
void city_update_atmospheric_pressure(int city, double value,  char *valueDate);

/**
 * Função executada por cada thread responsável por processar uma parte
 * dos dados do JSON.
 *
 * Cada thread recebe como parâmetro uma estrutura `City_param_t`
 * contendo:
 *  - o ponteiro para o JSON que será percorrido
 *  - o intervalo de índices `[inicio, fim)` que a thread deve processar
 *  - o nome do campo que contém os dados a serem analisados
 *
 * Durante o processamento, a thread percorre os elementos do JSON dentro
 * do intervalo atribuído, identifica a cidade associada a cada medição
 * e atualiza uma estrutura auxiliar local (`Cidade aux[2]`) com os
 * valores máximos e mínimos encontrados para temperatura, umidade e
 * pressão atmosférica.
 *
 * Após processar todos os elementos do seu intervalo, a thread consolida
 * os resultados locais nas estruturas globais das cidades (`caxias` e
 * `bento`) utilizando funções de monitor que garantem sincronização
 * entre as threads através de mutex.
 *
 * @param args ponteiro para uma estrutura `City_param_t` contendo os
 *             parâmetros de processamento da thread
 * @return ponteiro nulo (não utilizado)
 */
void *process_data_items(void *args);

/**
 * Inicializa e executa o processamento paralelo das medições das cidades.
 *
 * Esta função é responsável por preparar todo o ambiente necessário para
 * o processamento dos dados vindos dos arquivos JSON. O fluxo executado é:
 *
 * 1. Inicializa as estruturas globais das cidades (`caxias` e `bento`),
 *    definindo valores iniciais para máximos e mínimos.
 *
 * 2. Divide o trabalho entre as threads através da função
 *    `init_city_thread_params`, que define para cada thread o intervalo
 *    de elementos do JSON que ela deverá processar.
 *
 * 3. Cria as threads responsáveis por percorrer os dados e calcular
 *    localmente os extremos (temperatura, umidade e pressão).
 *
 * 4. Aguarda a finalização de todas as threads utilizando `pthread_join`,
 *    garantindo que o processamento completo seja concluído antes da
 *    continuação do programa.
 *
 * O processamento interno de cada thread utiliza estruturas auxiliares
 * locais para reduzir contenção de mutex, consolidando os resultados
 * globais apenas ao final do processamento.
 *
 * @param json_file_1 ponteiro para o cJSON contendo os dados do primeiro arquivo
 * @param json_file_2 ponteiro para o cJSON contendo os dados do segundo arquivo
 */
void start_city_processig(cJSON *json_file_1, cJSON *json_file_2);

/**
 * Função apenas para debug, exibe os valores da cidade passada como parametro
 * @param cidade cidade que será exibida no printf
 */
void print_city(Cidade *cidade);

/**
 * Consolida os resultados locais calculados por uma thread nas variáveis
 * globais das cidades.
 *
 * Cada thread mantém durante o processamento uma estrutura auxiliar
 * `Cidade aux[2]`, onde:
 *   - aux[CITY_CAXIAS] armazena os extremos encontrados para Caxias
 *   - aux[CITY_BENTO]  armazena os extremos encontrados para Bento
 *
 * Ao final do processamento da thread, esta função envia os valores
 * máximos e mínimos encontrados localmente para as estruturas globais
 * (`caxias` e `bento`). A atualização das globais é feita através das
 * funções de monitor (`city_update_temperature`, `city_update_humidity`
 * e `city_update_atmospheric_pressure`), que garantem sincronização
 * entre as threads usando mutex.
 *
 * Dessa forma, reduzimos o número de acessos concorrentes às variáveis
 * globais e evitamos contenção excessiva de locks durante o
 * processamento das medições.
 *
 * @param aux vetor de estruturas `Cidade` contendo os resultados locais
 *            da thread para cada cidade (índices CITY_CAXIAS e CITY_BENTO)
 */
void consolidate_city_results(Cidade aux[2]);

/**
 * Função que inicializa uma variavel Cidade com valores padrão, zerando variáveis e declarando strings como '\0'
 * @param cidade Cidade que terá seus valores iniciados
 * @param nome nome da cidade
 */
void init_city(Cidade *cidade, const char *nome);

/**
 * Adiciona soma e contagem de temperatura à cidade global correspondente.
 * @param city identificador da cidade (CITY_CAXIAS ou CITY_BENTO)
 * @param sum soma local das temperaturas
 * @param count quantidade de medições de temperatura
 */
void city_add_temperature_stats(int city, double sum, int count);

/**
 * Adiciona soma e contagem de umidade à cidade global correspondente.
 * @param city identificador da cidade (CITY_CAXIAS ou CITY_BENTO)
 * @param sum soma local das umidades
 * @param count quantidade de medições de umidade
 */
void city_add_humidity_stats(int city, double sum, int count);

/**
 * Adiciona soma e contagem de pressão atmosférica à cidade global correspondente.
 * @param city identificador da cidade (CITY_CAXIAS ou CITY_BENTO)
 * @param sum soma local das pressões
 * @param count quantidade de medições de pressão
 */
void city_add_atmospheric_pressure_stats(int city, double sum, int count);

/**
 * Consolida os resultados locais de uma cidade nas estruturas globais.
 * @param city identificador da cidade
 * @param cidade estrutura local com resultados da thread
 */
void consolidate_one_city_results(int city, Cidade *cidade);

/**
 *  Imprime a tabela de resultados para a variável de temperatura de cada cidade.
 */
void print_temperature_table();

/**
 *  Imprime a tabela de resultados para a variável de pressão atmosférica de cada cidade.
 */
void print_pressure_table();

/**
 *  Imprime a tabela de resultados para a variável de pressão atmosférica de cada codade.
 */
void print_humidity_table();

/**
 * Imprime o relatório completo da análise dos dados dos sensores.
 * @param json_file_1
 * @param json_file_2
 */
void print_full_report(double tempo_execucao);

int isNecessary(char *variable);

void city_add_spreading_factor(int city, int sf);
void consolidate_one_city_spreading_factors(int city, Cidade *cidade);
void print_spreading_factor_table();
void update_file_period(int file_index, char *valueDate);
void print_performance_table(double tempo_execucao);

#endif //ANALISE_IOT_CITY_H
/**
 * Atualiza os dados de bateria de uma cidade com base no timestamp da medicao.
 *
 * Compara a data ISO recebida com as datas ja armazenadas na estrutura global:
 *  - Se for ANTERIOR a data inicial (ou a primeira medicao), atualiza bateriaInicial.
 *  - Se for POSTERIOR a data final, atualiza bateriaFinal.
 *
 * Regiao critica protegida por mutex.
 *
 * @param city      identificador da cidade (CITY_CAXIAS ou CITY_BENTO)
 * @param value     tensao de bateria em Volts
 * @param valueDate data ISO 8601 da medicao (ex: "2025-01-15T10:30:00.000Z")
 */
void city_update_battery(int city, double value, char *valueDate);

/**
 * Consolida os dados de bateria da estrutura auxiliar local da thread
 * nas variaveis globais de cada cidade, usando city_update_battery.
 *
 * @param city   identificador da cidade
 * @param cidade ponteiro para a estrutura auxiliar local da thread
 */
void consolidate_one_city_battery(int city, Cidade *cidade);

/**
 * Imprime a tabela de consumo de bateria no formato do template do trabalho:
 *
 * Cidade | Inicial (V) | Final (V) | Consumo (V)
 */
void print_battery_table();