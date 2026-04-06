#include "file.h"
#include "logger.h"

#define FILE_1 DATA_DIR "/mqtt_senzemo_cx_bg.json"
#define FILE_2 DATA_DIR "/senzemo_cx_bg.json"

//variavel para guardar o Maior/Menor valor das medições de Caxias
Cidade caxias;

//variavel para guardar o Maior/Menor valor das medições de Bento
Cidade bento;

int main() {
    // Inicializa a thread de log
    inicializar_logger();

    registrar_log("Sistema de analise iniciado");

    //sobe os dois arquivos na memória
    registrar_log("Lendo arquivo %s", FILE_1);
    char *buffer_mqtt_senzemo_cx_bg = read_with_buffer(FILE_1,"r",1);

    registrar_log("Lendo arquivo %s", FILE_2);
    char *buffer_senzemo_cx_bg = read_with_buffer(FILE_2,"r",1);

    registrar_log("Iniciando parse dos JSONs");
    cJSON *json_mqtt_senzemo_cx_bg = cjson_transform(buffer_mqtt_senzemo_cx_bg,0);
    cJSON *json_senzemo_cx_bg = cjson_transform(buffer_senzemo_cx_bg,0);

    registrar_log("Parando o sistema");
    fechar_logger();

    return 0;
}