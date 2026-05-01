#include "file.h"
#include "city.h"
#include "logger.h"
#include <time.h>

#define FILE_1 DATA_DIR "/mqtt_senzemo_cx_bg.json"
#define FILE_2 DATA_DIR "/senzemo_cx_bg.json"

int main() {


    struct timespec inicio;
    struct timespec fim;

    if (clock_gettime(CLOCK_MONOTONIC, &inicio) != 0)
    {
        perror("clock_gettime inicio");
        return 1;
    }

    inicializar_logger();

    registrar_log("Sistema de analise iniciado");

    //sobe os dois arquivos na memória
    registrar_log("Lendo arquivo %s", FILE_1);
    char *buffer_mqtt_senzemo_cx_bg = read_with_buffer(FILE_1,"r",0);
    registrar_log("Lendo arquivo %s", FILE_2);
    char *buffer_senzemo_cx_bg = read_with_buffer(FILE_2,"r",0);

    registrar_log("Iniciando parse dos JSONs");
    cJSON *json_mqtt_senzemo_cx_bg = cjson_transform(buffer_mqtt_senzemo_cx_bg,0);
    cJSON *json_senzemo_cx_bg = cjson_transform(buffer_senzemo_cx_bg,0);


    start_city_processig(json_mqtt_senzemo_cx_bg , json_senzemo_cx_bg);

  if (clock_gettime(CLOCK_MONOTONIC, &fim) != 0)
    {
        perror("clock_gettime fim");
        return 1;
    }

    double tempo_execucao =
        (double)(fim.tv_sec - inicio.tv_sec) +
        (double)(fim.tv_nsec - inicio.tv_nsec) / 1e9;

    print_full_report(tempo_execucao);


    registrar_log("Parando o sistema");
    fechar_logger();

    return 0;
}