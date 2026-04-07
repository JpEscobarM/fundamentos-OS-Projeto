//
// Created by jmartins on 31/03/2026.
//
#include  "file.h"
#include <city.h>

FILE *open_file(char *FILE_PATH,char *mode, int log)
{
    FILE *file = fopen(FILE_PATH,mode);

    if (file == NULL)
    {
        printf("\nERRO: Não foi possivel abrir arquivo em: %s",FILE_PATH);
        exit(EXIT_FAILURE);
    }

    if (log)
    {
        printf("\nArquivo %s aberto no modo %s",FILE_PATH,mode);
    }

return file;
}

long tamanho_total_arquivo(FILE *file,int log)
{
    if (file == NULL)
    {
        printf("\nERRO: Não foi possivel checar  o tamanho do arquivo");
        exit(EXIT_FAILURE);
    }

    fseek(file,0,SEEK_END);
    long size = ftell(file);

    if (size <= 0 )
    {
        printf("\nERRO: tamanho do arquivo = 0");
        fclose(file);
        exit(EXIT_FAILURE);
    }


    if (log)
    {
        printf("\nTotal de bytes do arquivo %ld",size);
    }

    rewind(file);

    return size;
}


char *read_with_buffer(char *FILE_PATH,char *mode, int log)
{

    FILE *file = open_file(FILE_PATH,mode,log);

    //pega o tmanaho total do arquivo e volta para o inicio
    long tamanhoArquivo = tamanho_total_arquivo(file,log);

    //aloca buffer e coloca JSON na memoria

    char *buffer = malloc((tamanhoArquivo + 1) * sizeof(char));


    size_t bytes_read = fread(buffer,1,tamanhoArquivo,file);
    buffer[bytes_read]='\0';



    fclose(file);


    if (buffer == NULL)
    {
        printf("\nERRO: Buffer vazio");
        exit(EXIT_FAILURE);
    }

    if (log)
    {
    printf("\nBuffer de texto do arquivo JSON alocado com sucesso");
    }


return buffer;
}


/**
 *
 * @param buffer
 * @param log
 * @return
 */
cJSON *cjson_transform(char *buffer, int log)
{
   cJSON *json = cJSON_Parse(buffer);

    if (json == NULL)
    {
        printf("\nERRO: Erro no parse do JSON");
        exit(EXIT_FAILURE);

    }

    if (log)
    {

       char *log = cJSON_Print(json);
        printf("\n%s",log);
        free(log);
        printf("\nParse para JSON realizado com sucesso");
    }

return json;
}

