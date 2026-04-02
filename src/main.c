#include "file.h"

#define FILE_1 DATA_DIR "/mqtt_senzemo_cx_bg.json"
#define FILE_2 DATA_DIR "/senzemo_cx_bg.json"

int main() {


    char *buffer_mqtt_senzemo_cx_bg = read_with_buffer(FILE_1,"r",1);
    char *buffer_senzemo_cx_bg = read_with_buffer(FILE_2,"r",1);


return 0;
}