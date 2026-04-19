//
// Created by jmartins on 07/04/2026.
//

#include "city.h"
int N_ELEMENTOS_FILE_1;
int N_ELEMENTOS_FILE_2;

Cidade caxias;
Cidade bento;


static pthread_mutex_t lock_caxias = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t lock_bento = PTHREAD_MUTEX_INITIALIZER;

static  pthread_t processing_threads[N_THREADS];

static  City_param_t city_process_param[N_THREADS];

static pthread_mutex_t lock_file_stats[2] = {
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER
};

static char file_period_start[2][64];
static char file_period_end[2][64];

void init_city(Cidade *cidade, const char *nome)
{
    strcpy(cidade->nomeCidade, nome);

    cidade->maiorTemperatura = -DBL_MAX;
    cidade->menorTemperatura = DBL_MAX;

    cidade->maiorUmidade = -DBL_MAX;
    cidade->menorUmidade = DBL_MAX;

    cidade->maiorPressaoAtmosferica = -DBL_MAX;
    cidade->menorPressaoAtmosferica = DBL_MAX;

    cidade->dataMaiorTemperatura[0] = '\0';
    cidade->dataMenorTemperatura[0] = '\0';

    cidade->dataMaiorUmidade[0] = '\0';
    cidade->dataMenorUmidade[0] = '\0';

    cidade->dataMaiorPressaoAtmosferica[0] = '\0';
    cidade->dataMenorPressaoAtmosferica[0] = '\0';

    cidade->somaTemperatura = 0;
    cidade->somaUmidade = 0;
    cidade->somaPressao = 0;

    cidade->contTemperatura = 0;
    cidade->contUmidade = 0;
    cidade->contPressao = 0;

    // g) Bateria — ainda sem medicao, flag marca que nao foi inicializado
    cidade->bateriaInicial    = 0.0;
    cidade->bateriaFinal      = 0.0;
    cidade->dataBateriaInicial[0] = '\0';
    cidade->dataBateriaFinal[0]   = '\0';
    cidade->bateriaInicialSet = 0;

    for (int i = 0; i < 16; i++)
    {
        cidade->spreadingFactors[i] = 0;
    }
    cidade->qtdSpreadingFactors = 0;
}

void init_city_thread_params(cJSON *json_file_1, cJSON *json_file_2)
{
    int size_json_1 = cJSON_GetArraySize(json_file_1);
    N_ELEMENTOS_FILE_1 = size_json_1;


    int size_json_2 = cJSON_GetArraySize(json_file_2);
    N_ELEMENTOS_FILE_2 = size_json_2;

    for (int i = 0; i < 2; i++)
    {
        file_period_start[i][0] = '\0';
        file_period_end[i][0] = '\0';
    }

    // thread 0 resposavel pelo file 1
    city_process_param[0].inicio = 0;
    city_process_param[0].fim = size_json_1;
    city_process_param[0].file_index = 0;
    city_process_param[0].json = json_file_1;
    strcpy(city_process_param[0].field,"payload");

    int bloco = size_json_2 / (N_THREADS - 1);

    for (int i = 1; i < N_THREADS; i++)
    {
        int idx = i - 1; //para começar do inidice 0 no jsonfile2

        city_process_param[i].inicio = idx * bloco;

        if (i == N_THREADS - 1)
        {
            city_process_param[i].fim = size_json_2;
        }
        else
        {
            city_process_param[i].fim = (idx + 1) * bloco;
        }

        city_process_param[i].file_index = 1;
        city_process_param[i].json = json_file_2;
        strcpy(city_process_param[i].field,"brute_data");
    }
}

void start_city_processig(cJSON *json_file_1, cJSON *json_file_2)
{
    init_city(&caxias, "Caxias do Sul");
    init_city(&bento, "Bento Gonçalves");

    init_city_thread_params(json_file_1,json_file_2);

    for (int i = 0 ; i  < N_THREADS; i++)
    {
        pthread_create(&processing_threads[i],NULL,process_data_items,(void*)&city_process_param[i]);
    }

    for (int i = 0; i < N_THREADS;i++)
    {
        pthread_join(processing_threads[i],NULL);
    }

}

void format_date(char *aUnformattedDate, char *out)
{
    int day,month,year, hour,minute,second;

    sscanf(aUnformattedDate,"%4d-%2d-%2dT%2d:%2d:%2d",&year,&month,&day,
        &hour,&minute,&second);

   sprintf(out,"%02d/%02d/%04d %02d:%02d:%02d",
       day,month,year,hour,minute,second);
}

void show_city_thread_params()
{
    for (int i = 0; i < N_THREADS; i++)
    {
        char *file = (i == 0) ? "FILE_1" : "FILE_2";

        printf("Thread %d (%s) -> inicio: %d fim: %d\n",
               i,
               file,
               city_process_param[i].inicio,
               city_process_param[i].fim);
    }
}

void destroy_city_locks()
{
    pthread_mutex_destroy(&lock_caxias);
    pthread_mutex_destroy(&lock_bento);
}

void city_update_temperature(int city, double value,  char *valueDate)
{
    switch (city)
    {
    case CITY_CAXIAS:
        pthread_mutex_lock(&lock_caxias);

        if (value > caxias.maiorTemperatura)
        {
            caxias.maiorTemperatura = value;
            format_date(valueDate, caxias.dataMaiorTemperatura);
        }

        if (value < caxias.menorTemperatura)
        {
            caxias.menorTemperatura = value;
            format_date(valueDate, caxias.dataMenorTemperatura);
        }

        pthread_mutex_unlock(&lock_caxias);
        break;

    case CITY_BENTO:
        pthread_mutex_lock(&lock_bento);

        if (value > bento.maiorTemperatura)
        {
            bento.maiorTemperatura = value;
            format_date(valueDate, bento.dataMaiorTemperatura);
        }

        if (value < bento.menorTemperatura)
        {
            bento.menorTemperatura = value;
            format_date(valueDate, bento.dataMenorTemperatura);
        }

        pthread_mutex_unlock(&lock_bento);
        break;

    default:
        printf("\nERRO city_update_temperature: Cidade invalida passada como parametro");
        break;
    }
}

void city_update_humidity(int city, double value,  char *valueDate)
{
    switch (city)
    {
    case CITY_CAXIAS:
        pthread_mutex_lock(&lock_caxias);

        if (value > caxias.maiorUmidade)
        {
            caxias.maiorUmidade = value;
            format_date(valueDate, caxias.dataMaiorUmidade);
        }

        if (value < caxias.menorUmidade)
        {
            caxias.menorUmidade = value;
            format_date(valueDate, caxias.dataMenorUmidade);
        }

        pthread_mutex_unlock(&lock_caxias);
        break;

    case CITY_BENTO:
        pthread_mutex_lock(&lock_bento);

        if (value > bento.maiorUmidade)
        {
            bento.maiorUmidade = value;
            format_date(valueDate, bento.dataMaiorUmidade);
        }

        if (value < bento.menorUmidade)
        {
            bento.menorUmidade = value;
            format_date(valueDate, bento.dataMenorUmidade);
        }

        pthread_mutex_unlock(&lock_bento);
        break;

    default:
        fprintf(stderr, "\nERRO city_update_humidity: Cidade invalida passada como parametro");
        break;
    }
}

void city_update_atmospheric_pressure(int city, double value,  char *valueDate)
{
    switch (city)
    {
        case CITY_CAXIAS:
            pthread_mutex_lock(&lock_caxias);

            if (value > caxias.maiorPressaoAtmosferica)
            {
                caxias.maiorPressaoAtmosferica = value;
                format_date(valueDate, caxias.dataMaiorPressaoAtmosferica);
            }

            if (value < caxias.menorPressaoAtmosferica)
            {
                caxias.menorPressaoAtmosferica = value;
                format_date(valueDate, caxias.dataMenorPressaoAtmosferica);
            }

            pthread_mutex_unlock(&lock_caxias);
            break;

        case CITY_BENTO:
            pthread_mutex_lock(&lock_bento);

            if (value > bento.maiorPressaoAtmosferica)
            {
                bento.maiorPressaoAtmosferica = value;
                format_date(valueDate, bento.dataMaiorPressaoAtmosferica);
            }

            if (value < bento.menorPressaoAtmosferica)
            {
                bento.menorPressaoAtmosferica = value;
                format_date(valueDate, bento.dataMenorPressaoAtmosferica);
            }

            pthread_mutex_unlock(&lock_bento);
            break;

        default:
            fprintf(stderr, "\nERRO city_update_atmospheric_pressure: Cidade invalida passada como parametro");
            break;
    }
}

void city_add_temperature_stats(int city, double sum, int count)
{
    switch (city)
    {
        case CITY_CAXIAS:
            pthread_mutex_lock(&lock_caxias);

            caxias.somaTemperatura += sum;
            caxias.contTemperatura += count;

            pthread_mutex_unlock(&lock_caxias);
            break;

        case CITY_BENTO:
            pthread_mutex_lock(&lock_bento);

            bento.somaTemperatura += sum;
            bento.contTemperatura += count;

            pthread_mutex_unlock(&lock_bento);
            break;

        default:
            fprintf(stderr, "\nERRO city_update_temperature_avg: Cidade invalida passada como parametro");
            break;
    }
}

void city_add_humidity_stats(int city, double sum, int count)
{
    switch (city)
    {
        case CITY_CAXIAS:
            pthread_mutex_lock(&lock_caxias);

            caxias.somaUmidade += sum;
            caxias.contUmidade += count;

            pthread_mutex_unlock(&lock_caxias);
            break;

        case CITY_BENTO:
            pthread_mutex_lock(&lock_bento);

            bento.somaUmidade += sum;
            bento.contUmidade += count;

            pthread_mutex_unlock(&lock_bento);
            break;

        default:
            fprintf(stderr, "\nERRO city_update_humidity_avg: Cidade invalida passada como parametro");
            break;
    }
}

void city_add_atmospheric_pressure_stats(int city, double sum, int count)
{
    switch (city)
    {
        case CITY_CAXIAS:
            pthread_mutex_lock(&lock_caxias);

            caxias.somaPressao += sum;
            caxias.contPressao += count;

            pthread_mutex_unlock(&lock_caxias);
            break;

        case CITY_BENTO:
            pthread_mutex_lock(&lock_bento);

            bento.somaPressao += sum;
            bento.contPressao += count;

            pthread_mutex_unlock(&lock_bento);
            break;

        default:
            fprintf(stderr, "\nERRO city_update_atmospheric_pressure_avg: Cidade invalida passada como parametro");
            break;
    }
}

void print_city(Cidade *cidade)
{
    if (cidade == NULL)
    {
        printf("\nCidade == NULL\n");
        return;
    }

    printf("\n====================================");
    printf("\nCidade: %s", cidade->nomeCidade);

    printf("\n--- Temperatura ---");
    printf("\nMaior: %.2f  Data: %s",
           cidade->maiorTemperatura,
           cidade->dataMaiorTemperatura);

    printf("\nMenor: %.2f  Data: %s",
           cidade->menorTemperatura,
           cidade->dataMenorTemperatura);

    printf("\n--- Umidade ---");
    printf("\nMaior: %.2f  Data: %s",
           cidade->maiorUmidade,
           cidade->dataMaiorUmidade);

    printf("\nMenor: %.2f  Data: %s",
           cidade->menorUmidade,
           cidade->dataMenorUmidade);

    printf("\n--- Pressão Atmosférica ---");
    printf("\nMaior: %.2f  Data: %s",
           cidade->maiorPressaoAtmosferica,
           cidade->dataMaiorPressaoAtmosferica);

    printf("\nMenor: %.2f  Data: %s",
           cidade->menorPressaoAtmosferica,
           cidade->dataMenorPressaoAtmosferica);

    printf("\n====================================\n");
}


void print_temperature_table()
{
    double media;

    printf("\n------------------------------------------------------------");
    printf("\nTEMPERATURA (°C)");
    printf("\n------------------------------------------------------------");

    printf("\n%-18s | %-6s | %-19s | %-6s | %-19s | %-6s",
           "Cidade", "Mínima", "Data/Hora", "Máxima", "Data/Hora", "Média");

    printf("\n-----------------------------------------------------------------------------------------------\n");

    media = caxias.contTemperatura ? caxias.somaTemperatura / caxias.contTemperatura : 0;

    printf("%-18s | %6.2f | %-19s | %6.2f | %-19s | %6.2f\n",
           caxias.nomeCidade,
           caxias.menorTemperatura,
           caxias.dataMenorTemperatura,
           caxias.maiorTemperatura,
           caxias.dataMaiorTemperatura,
           media);

    media = bento.contTemperatura ? bento.somaTemperatura / bento.contTemperatura : 0;

    printf("%-19s | %6.2f | %-19s | %6.2f | %-19s | %6.2f\n",
           bento.nomeCidade,
           bento.menorTemperatura,
           bento.dataMenorTemperatura,
           bento.maiorTemperatura,
           bento.dataMaiorTemperatura,
           media);
}

void print_humidity_table()
{
    double media;

    printf("\n------------------------------------------------------------");
    printf("\nUMIDADE (%%)");
    printf("\n------------------------------------------------------------");

    printf("\n%-18s | %-6s | %-19s | %-6s | %-19s | %-6s",
           "Cidade", "Mínima", "Data/Hora", "Máxima", "Data/Hora", "Média");

    printf("\n-----------------------------------------------------------------------------------------------\n");

    media = caxias.contUmidade ? caxias.somaUmidade / caxias.contUmidade : 0;

    printf("%-18s | %6.2f | %-19s | %6.2f | %-19s | %6.2f\n",
           caxias.nomeCidade,
           caxias.menorUmidade,
           caxias.dataMenorUmidade,
           caxias.maiorUmidade,
           caxias.dataMaiorUmidade,
           media);

    media = bento.contUmidade ? bento.somaUmidade / bento.contUmidade : 0;

    printf("%-19s | %6.2f | %-19s | %6.2f | %-19s | %6.2f\n",
           bento.nomeCidade,
           bento.menorUmidade,
           bento.dataMenorUmidade,
           bento.maiorUmidade,
           bento.dataMaiorUmidade,
           media);
}

void print_pressure_table()
{
    double media;

    printf("\n------------------------------------------------------------");
    printf("\nPRESSÃO ATMOSFÉRICA (hPa)");
    printf("\n------------------------------------------------------------");

    printf("\n%-18s | %-6s | %-19s | %-6s | %-19s | %-6s",
           "Cidade", "Mínima", "Data/Hora", "Máxima", "Data/Hora", "Média");

    printf("\n-----------------------------------------------------------------------------------------------\n");

    media = caxias.contPressao ? caxias.somaPressao / caxias.contPressao : 0;

    printf("%-18s | %6.2f | %-19s | %6.2f | %-19s | %6.2f\n",
           caxias.nomeCidade,
           caxias.menorPressaoAtmosferica,
           caxias.dataMenorPressaoAtmosferica,
           caxias.maiorPressaoAtmosferica,
           caxias.dataMaiorPressaoAtmosferica,
           media);

    media = bento.contPressao ? bento.somaPressao / bento.contPressao : 0;

    printf("%-19s | %6.2f | %-19s | %6.2f | %-19s | %6.2f\n",
           bento.nomeCidade,
           bento.menorPressaoAtmosferica,
           bento.dataMenorPressaoAtmosferica,
           bento.maiorPressaoAtmosferica,
           bento.dataMaiorPressaoAtmosferica,
           media);
}


void print_full_report(double tempo_execucao)
{
    char periodo_inicio[2][64] = {{0}};
    char periodo_fim[2][64] = {{0}};

    if (file_period_start[0][0] != '\0') format_date(file_period_start[0], periodo_inicio[0]);
    if (file_period_end[0][0] != '\0') format_date(file_period_end[0], periodo_fim[0]);
    if (file_period_start[1][0] != '\0') format_date(file_period_start[1], periodo_inicio[1]);
    if (file_period_end[1][0] != '\0') format_date(file_period_end[1], periodo_fim[1]);

    printf("\n============================================================");
    printf("\nANÁLISE DE DADOS DOS SENSORES - CityLivingLab");
    printf("\nProcessamento utilizando pthreads");
    printf("\n============================================================\n");

    printf("\nArquivo analisado: mqtt_senzemo_cx_bg.json");
    printf("\nTotal de registros processados: %d", N_ELEMENTOS_FILE_1);
    printf("\nPeriodo analisado: %s a %s\n",
           periodo_inicio[0][0] ? periodo_inicio[0] : "Sem dados",
           periodo_fim[0][0] ? periodo_fim[0] : "Sem dados");

    printf("\nArquivo analisado: senzemo_cx_bg.json");
    printf("\nTotal de registros processados: %d", N_ELEMENTOS_FILE_2);
    printf("\nPeriodo analisado: %s a %s\n",
           periodo_inicio[1][0] ? periodo_inicio[1] : "Sem dados",
           periodo_fim[1][0] ? periodo_fim[1] : "Sem dados");

    print_temperature_table();
    print_humidity_table();
    print_pressure_table();
    print_battery_table();
    print_spreading_factor_table();
    print_performance_table(tempo_execucao);
}

// ─── g) Bateria ──────────────────────────────────────────────────────────────

void city_update_battery(int city, double value, char *valueDate)
{
    Cidade *c = (city == CITY_CAXIAS) ? &caxias : &bento;
    pthread_mutex_t *lock = (city == CITY_CAXIAS) ? &lock_caxias : &lock_bento;

    pthread_mutex_lock(lock);

    if (!c->bateriaInicialSet)
    {
        // Primeira medição registrada na global
        c->bateriaInicial = value;
        c->bateriaFinal   = value;
        strcpy(c->dataBateriaInicial, valueDate);
        strcpy(c->dataBateriaFinal,   valueDate);
        c->bateriaInicialSet = 1;
    }
    else
    {
        // strcmp em ISO 8601 funciona como comparação cronológica
        if (strcmp(valueDate, c->dataBateriaInicial) < 0)
        {
            c->bateriaInicial = value;
            strcpy(c->dataBateriaInicial, valueDate);
        }
        if (strcmp(valueDate, c->dataBateriaFinal) > 0)
        {
            c->bateriaFinal = value;
            strcpy(c->dataBateriaFinal, valueDate);
        }
    }

    pthread_mutex_unlock(lock);
}

void consolidate_one_city_battery(int city, Cidade *cidade)
{
    // Envia o valor inicial (timestamp mais antigo) e o final (mais recente)
    // para city_update_battery, que decide qual é realmente o extremo global
    city_update_battery(city, cidade->bateriaInicial, cidade->dataBateriaInicial);
    city_update_battery(city, cidade->bateriaFinal,   cidade->dataBateriaFinal);
}

void print_battery_table()
{
    printf("\n------------------------------------------------------------");
    printf("\nBATERIA");
    printf("\n------------------------------------------------------------");

    printf("\n%-18s | %-11s | %-9s | %-11s",
           "Cidade", "Inicial (V)", "Final (V)", "Consumo (V)");

    printf("\n------------------------------------------------------------\n");

    double consumo_cx = caxias.bateriaInicial - caxias.bateriaFinal;
    printf("%-18s | %11.2f | %9.2f | %11.2f\n",
           caxias.nomeCidade,
           caxias.bateriaInicial,
           caxias.bateriaFinal,
           consumo_cx);

    double consumo_bg = bento.bateriaInicial - bento.bateriaFinal;
    printf("%-19s | %11.2f | %9.2f | %11.2f\n",
           bento.nomeCidade,
           bento.bateriaInicial,
           bento.bateriaFinal,
           consumo_bg);
}

// ─────────────────────────────────────────────────────────────────────────────

void update_file_period(int file_index, char *valueDate)
{
    if (file_index < 0 || file_index > 1 || valueDate == NULL || valueDate[0] == '\0')
    {
        return;
    }

    pthread_mutex_lock(&lock_file_stats[file_index]);

    if (file_period_start[file_index][0] == '\0' || strcmp(valueDate, file_period_start[file_index]) < 0)
    {
        strcpy(file_period_start[file_index], valueDate);
    }

    if (file_period_end[file_index][0] == '\0' || strcmp(valueDate, file_period_end[file_index]) > 0)
    {
        strcpy(file_period_end[file_index], valueDate);
    }

    pthread_mutex_unlock(&lock_file_stats[file_index]);
}

void city_add_spreading_factor(int city, int sf)
{
    Cidade *c = (city == CITY_CAXIAS) ? &caxias : &bento;
    pthread_mutex_t *lock = (city == CITY_CAXIAS) ? &lock_caxias : &lock_bento;

    pthread_mutex_lock(lock);

    // Mantem apenas valores distintos de SF por cidade.
    for (int i = 0; i < c->qtdSpreadingFactors; i++)
    {
        if (c->spreadingFactors[i] == sf)
        {
            pthread_mutex_unlock(lock);
            return;
        }
    }

    if (c->qtdSpreadingFactors < 16)
    {
        c->spreadingFactors[c->qtdSpreadingFactors] = sf;
        c->qtdSpreadingFactors++;
    }

    pthread_mutex_unlock(lock);
}

void consolidate_one_city_spreading_factors(int city, Cidade *cidade)
{
    for (int i = 0; i < cidade->qtdSpreadingFactors; i++)
    {
        city_add_spreading_factor(city, cidade->spreadingFactors[i]);
    }
}

static void print_city_sfs(Cidade *cidade)
{
    int sorted[16];

    printf("%-19s | ", cidade->nomeCidade);

    if (cidade->qtdSpreadingFactors == 0)
    {
        printf("Sem dados\n");
        return;
    }

    for (int i = 0; i < cidade->qtdSpreadingFactors; i++)
    {
        sorted[i] = cidade->spreadingFactors[i];
    }

    // Ordena para deixar a saida estavel mesmo com consolidacao paralela.
    for (int i = 0; i < cidade->qtdSpreadingFactors - 1; i++)
    {
        for (int j = i + 1; j < cidade->qtdSpreadingFactors; j++)
        {
            if (sorted[j] < sorted[i])
            {
                int tmp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = tmp;
            }
        }
    }

    for (int i = 0; i < cidade->qtdSpreadingFactors; i++)
    {
        printf("SF%d", sorted[i]);
        if (i < cidade->qtdSpreadingFactors - 1)
        {
            printf(", ");
        }
    }

    printf("\n");
}

void print_spreading_factor_table()
{
    printf("\n------------------------------------------------------------");
    printf("\nSPREADING FACTORS UTILIZADOS");
    printf("\n------------------------------------------------------------");
    printf("\n%-18s | %s", "Cidade", "SF utilizados");
    printf("\n------------------------------------------------------------\n");

    print_city_sfs(&caxias);
    print_city_sfs(&bento);
}

void print_performance_table(double tempo_execucao)
{
    // Secao final no formato pedido pelo template do trabalho.
    printf("\n------------------------------------------------------------");
    printf("\nDESEMPENHO");
    printf("\n------------------------------------------------------------");
    printf("\nTempo total de execucao: %.3f segundos", tempo_execucao);
    printf("\nThreads utilizadas: %d", N_THREADS + 1);
    printf("\n - Thread principal: leitura dos dados e parse dos JSONs");
    printf("\n - Threads 1-%d: calculo das estatisticas", N_THREADS);
    printf("\n - Thread de log: registro de logs");
    printf("\n\nArquivo de log gerado: processamento.log");
    printf("\n\n============================================================");
    printf("\nProcessamento finalizado com sucesso.");
    printf("\n============================================================\n");
}

void consolidate_one_city_results(int city, Cidade *cidade)
{
    city_update_temperature(city, cidade->maiorTemperatura, cidade->dataMaiorTemperatura);
    city_update_temperature(city, cidade->menorTemperatura, cidade->dataMenorTemperatura);

    city_update_humidity(city, cidade->maiorUmidade, cidade->dataMaiorUmidade);
    city_update_humidity(city, cidade->menorUmidade, cidade->dataMenorUmidade);

    city_update_atmospheric_pressure(city, cidade->maiorPressaoAtmosferica, cidade->dataMaiorPressaoAtmosferica);
    city_update_atmospheric_pressure(city, cidade->menorPressaoAtmosferica, cidade->dataMenorPressaoAtmosferica);

    city_add_temperature_stats(city, cidade->somaTemperatura, cidade->contTemperatura);
    city_add_humidity_stats(city, cidade->somaUmidade, cidade->contUmidade);
    city_add_atmospheric_pressure_stats(city, cidade->somaPressao, cidade->contPressao);

    // g) Consolida bateria apenas se a thread encontrou alguma medição
    if (cidade->bateriaInicialSet)
    {
        consolidate_one_city_battery(city, cidade);
    }

    consolidate_one_city_spreading_factors(city, cidade);
}

void consolidate_city_results(Cidade aux[2])
{
    consolidate_one_city_results(CITY_CAXIAS, &aux[CITY_CAXIAS]);
    consolidate_one_city_results(CITY_BENTO, &aux[CITY_BENTO]);
}

void *process_data_items(void *args)
{

    //PARAMETROS
    City_param_t *param = (City_param_t*) args;

    // variáveis locais da thread
    Cidade aux[2];
    init_city(&aux[0], "Caxias do Sul-AUX");
    init_city(&aux[1], "Bento Gonçalves-AUX");


    if (param == NULL || param->json == NULL)
    {
        fprintf(stderr, "\nERRO: param ou json == NULL\n");
        return NULL;
    }

    if (!cJSON_IsArray(param->json))
    {
        fprintf(stderr, "\nERRO: json não é array\n");
        return NULL;
    }

    for (int i = param->inicio; i < param->fim; i++)
    {
        cJSON *outer_item = cJSON_GetArrayItem(param->json, i);
        if (outer_item == NULL)
            continue;

        //field = "brute_data" ou "payloard", é o campo que contem as informações que precisamos
        cJSON *raw_field = cJSON_GetObjectItemCaseSensitive(outer_item, param->field);

        if (raw_field == NULL || !cJSON_IsString(raw_field) || raw_field->valuestring == NULL)
            continue;

        //faz um parse novamente pois a string interna de "brute_data" ou "payload" também é um JSON
        cJSON *inner_json = cJSON_Parse(raw_field->valuestring);
        if (inner_json == NULL)
            continue;

        //usado pra separar qual cidade está processando no momento e atualizar o vetor Cidades aux[]
        int cidadeAtual = -1;

        cJSON *device_name = cJSON_GetObjectItemCaseSensitive(inner_json, "device_name");
        if (device_name != NULL && cJSON_IsString(device_name) && device_name->valuestring != NULL)
        {
            if (strcmp(device_name->valuestring, "Caxias - Praça (S2)") == 0)
            {
                cidadeAtual = CITY_CAXIAS;
            }
            else if (strcmp(device_name->valuestring, "Bento - Praça (S3)") == 0)
            {
                cidadeAtual = CITY_BENTO;
            }
        }

        if (cidadeAtual == -1)
        {
            cJSON_Delete(inner_json);
            continue;
        }

        //pega o vetor data[] que contém as leituras de umidade,temperatura e pressao atmosférica
        cJSON *data = cJSON_GetObjectItemCaseSensitive(inner_json, "data");
        if (data != NULL && cJSON_IsArray(data))
        {
            int data_size = cJSON_GetArraySize(data);

            //laço que percorre o vetor de data, separando o conteúdo de variavel(temperatura,umidade,pressao),valor,time
            for (int j = 0; j < data_size; j++)
            {
                cJSON *measurement = cJSON_GetArrayItem(data, j);
                if (measurement == NULL)
                    continue;

                cJSON *variable = cJSON_GetObjectItemCaseSensitive(measurement, "variable");

                cJSON *value    = cJSON_GetObjectItemCaseSensitive(measurement, "value");
                cJSON *time     = cJSON_GetObjectItemCaseSensitive(measurement, "time");

                if (variable == NULL || !cJSON_IsString(variable) || variable->valuestring == NULL)
                    continue;

                if (!isNecessary(variable->valuestring))
                {
                    continue;
                }

                if (value == NULL || !cJSON_IsNumber(value))
                    continue;

                if (time == NULL || !cJSON_IsString(time) || time->valuestring == NULL)
                    continue;

                double valor = cJSON_GetNumberValue(value);
                update_file_period(param->file_index, time->valuestring);

                //atualiza a variavel de aux de acordo com o campo lido no momento, se a variável for algo que precisamos
                if (strcmp(variable->valuestring, "temperature") == 0)
                {
                    if (valor > aux[cidadeAtual].maiorTemperatura)
                    {
                        aux[cidadeAtual].maiorTemperatura = valor;
                        strcpy(aux[cidadeAtual].dataMaiorTemperatura, time->valuestring);
                    }

                    if (valor < aux[cidadeAtual].menorTemperatura)
                    {
                        aux[cidadeAtual].menorTemperatura = valor;
                        strcpy(aux[cidadeAtual].dataMenorTemperatura, time->valuestring);
                    }

                    aux[cidadeAtual].somaTemperatura += valor;
                    aux[cidadeAtual].contTemperatura++;
                }
                else if (strcmp(variable->valuestring, "humidity") == 0)
                {
                    if (valor > aux[cidadeAtual].maiorUmidade)
                    {
                        aux[cidadeAtual].maiorUmidade = valor;
                        strcpy(aux[cidadeAtual].dataMaiorUmidade, time->valuestring);
                    }

                    if (valor < aux[cidadeAtual].menorUmidade)
                    {
                        aux[cidadeAtual].menorUmidade = valor;
                        strcpy(aux[cidadeAtual].dataMenorUmidade, time->valuestring);
                    }
                    aux[cidadeAtual].somaUmidade += valor;
                    aux[cidadeAtual].contUmidade++;
                }
                else if (strcmp(variable->valuestring, "airpressure") == 0)
                {
                    if (valor > aux[cidadeAtual].maiorPressaoAtmosferica)
                    {
                        aux[cidadeAtual].maiorPressaoAtmosferica = valor;
                        strcpy(aux[cidadeAtual].dataMaiorPressaoAtmosferica, time->valuestring);
                    }

                    if (valor < aux[cidadeAtual].menorPressaoAtmosferica)
                    {
                        aux[cidadeAtual].menorPressaoAtmosferica = valor;
                        strcpy(aux[cidadeAtual].dataMenorPressaoAtmosferica, time->valuestring);
                    }
                    aux[cidadeAtual].somaPressao += valor;
                    aux[cidadeAtual].contPressao++;
                }
                // g) Bateria: guarda o primeiro e o último valor por timestamp ISO
                else if (strcmp(variable->valuestring, "batterylevel") == 0)
                {
                    char *t = time->valuestring;

                    // Primeiro registro de bateria desta thread para esta cidade
                    if (!aux[cidadeAtual].bateriaInicialSet)
                    {
                        aux[cidadeAtual].bateriaInicial  = valor;
                        aux[cidadeAtual].bateriaFinal    = valor;
                        strcpy(aux[cidadeAtual].dataBateriaInicial, t);
                        strcpy(aux[cidadeAtual].dataBateriaFinal,   t);
                        aux[cidadeAtual].bateriaInicialSet = 1;
                    }
                    else
                    {
                        // strcmp em datas ISO 8601 funciona lexicograficamente
                        if (strcmp(t, aux[cidadeAtual].dataBateriaInicial) < 0)
                        {
                            aux[cidadeAtual].bateriaInicial = valor;
                            strcpy(aux[cidadeAtual].dataBateriaInicial, t);
                        }
                        if (strcmp(t, aux[cidadeAtual].dataBateriaFinal) > 0)
                        {
                            aux[cidadeAtual].bateriaFinal = valor;
                            strcpy(aux[cidadeAtual].dataBateriaFinal, t);
                        }
                    }
                }
                else if (strcmp(variable->valuestring, "lora_spreading_factor") == 0)
                {
                    // Coleta localmente os SFs distintos e consolida no fim da thread.
                    int sf = (int)valor;
                    int exists = 0;

                    for (int k = 0; k < aux[cidadeAtual].qtdSpreadingFactors; k++)
                    {
                        if (aux[cidadeAtual].spreadingFactors[k] == sf)
                        {
                            exists = 1;
                            break;
                        }
                    }

                    if (!exists && aux[cidadeAtual].qtdSpreadingFactors < 16)
                    {
                        aux[cidadeAtual].spreadingFactors[aux[cidadeAtual].qtdSpreadingFactors] = sf;
                        aux[cidadeAtual].qtdSpreadingFactors++;
                    }
                }
            }
        }

        cJSON_Delete(inner_json);
    }

    // consolidação final da thread nas variáveis globais
    consolidate_city_results(aux);


   pthread_exit(NULL);
}

int isNecessary(char *variable)
{
    if (strcmp(variable, "temperature") == 0) return 1;
    if (strcmp(variable, "humidity") == 0) return 1;
    if (strcmp(variable, "airpressure") == 0) return 1;
    if (strcmp(variable, "batterylevel") == 0) return 1;
    if (strcmp(variable, "lora_spreading_factor") == 0) return 1;

    return 0;
}
