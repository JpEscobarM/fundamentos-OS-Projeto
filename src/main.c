#include "file.h"
#include "city.h"
#include "logger.h"
#include <time.h>

#define FILE_1 DATA_DIR "/mqtt_senzemo_cx_bg.json"
#define FILE_2 DATA_DIR "/senzemo_cx_bg.json"

int main() {


    clock_t start = clock();

    // Inicializa a thread de log
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

    print_full_report();


    registrar_log("Parando o sistema");
    fechar_logger();

    clock_t end = clock();

    double tempo = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\nTempo total de execução: %.3f segundos\n", tempo);

    return 0;
}