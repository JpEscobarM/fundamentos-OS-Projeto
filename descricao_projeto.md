# Análise de Dados dos Sensores -- CityLivingLab

O objetivo deste trabalho é, em duplas ou trios, desenvolver um pequeno
software em **C utilizando pthreads** capaz de ler arquivos no formato
JSON contendo medições de:

-   temperatura
-   umidade
-   nível de bateria
-   pressão atmosférica

Essas medições foram coletadas por sensores instalados nas cidades de
**Caxias do Sul** e **Bento Gonçalves** ao longo do último ano.

Ambos os sensores fazem parte de um projeto conduzido pelo grupo de
pesquisa **CityLivingLab**.

🔗 https://www.citylivinglab.com/

------------------------------------------------------------------------

# Requisitos

O programa deverá atender aos seguintes requisitos:

### a)

Utilizar **múltiplas threads (pthreads)** para separar pelo menos as
seguintes etapas do processamento:

-   leitura dos dados
-   cálculo das estatísticas
-   registro dos logs

### b)

Ler os arquivos JSON disponíveis em:

https://drive.google.com/file/d/1BkaovTMQS00TYr_LemH9GqtW7rjGJ5vi/view?usp=sharing

### c)

Para cada uma das cidades, identificar e apresentar:

-   maior temperatura registrada
-   menor temperatura registrada
-   data e horário dessas medições

### d)

Para cada uma das cidades, identificar e apresentar:

-   maior umidade registrada
-   menor umidade registrada
-   data e horário dessas medições

### e)

Para cada uma das cidades, identificar e apresentar:

-   maior pressão atmosférica registrada
-   menor pressão atmosférica registrada
-   data e horário dessas medições

### f)

Calcular e apresentar a **média de:**

-   temperatura
-   umidade
-   pressão atmosférica

para cada cidade.

### g)

Calcular e apresentar o **consumo de bateria** em cada cidade através da
diferença entre:

-   tensão inicial da bateria
-   tensão final da bateria

### h)

Identificar e apresentar os **Spreading Factors (SF)** utilizados nas
transmissões.

### i)

Registrar **logs em arquivo** durante a execução do programa para
permitir auditoria e análise do comportamento do código.

### j)

Calcular e apresentar o **tempo total de execução do programa**.

> Quanto mais rápida a execução, maior será a avaliação desse requisito.

### k)

Os resultados deverão ser apresentados conforme o **template
fornecido**.

------------------------------------------------------------------------

# Instruções de Entrega

## Código

O código-fonte deverá estar disponível em um **repositório GitLab**,
contendo:

-   instruções de compilação
-   instruções de execução
-   demais orientações necessárias

## Vídeo

Deverá ser apresentado um **vídeo de até 5 minutos** demonstrando:

-   funcionamento do programa
-   atendimento de todos os requisitos

⚠️ Não é permitido acelerar o vídeo.

------------------------------------------------------------------------

# Critérios de Avaliação

  Critério                         Pontos
  -------------------------------- --------
  Publicação do código no GitLab   0,5
  Qualidade do vídeo               0,5
  Atendimento dos requisitos       6,0

------------------------------------------------------------------------

# Template de Saída do Programa

    ============================================================
    ANÁLISE DE DADOS DOS SENSORES - CityLivingLab
    Processamento utilizando pthreads
    ============================================================

    Arquivo analisado: sensores_caxias.json
    Total de registros processados: 52.184
    Período analisado: 01/01/2025 a 31/12/2025

    Arquivo analisado: sensores_bento.json
    Total de registros processados: 52.184
    Período analisado: 01/01/2025 a 31/12/2025

    ------------------------------------------------------------
    TEMPERATURA (°C)
    ------------------------------------------------------------
    Cidade            | Mínima | Data/Hora             | Máxima | Data/Hora             | Média
    -----------------------------------------------------------------------------------------------
    Caxias do Sul     | 2.10   | 05/07/2025 06:14:02   | 38.45  | 12/02/2025 15:32:18   | 18.73
    Bento Gonçalves   | 1.85   | 06/07/2025 06:10:44   | 36.90  | 14/02/2025 16:02:11   | 17.95


    ------------------------------------------------------------
    UMIDADE (%)
    ------------------------------------------------------------
    Cidade            | Mínima | Data/Hora             | Máxima | Data/Hora             | Média
    -----------------------------------------------------------------------------------------------
    Caxias do Sul     | 21.30  | 09/01/2025 14:20:10   | 98.40  | 21/06/2025 05:48:03   | 71.22
    Bento Gonçalves   | 24.90  | 10/01/2025 13:58:02   | 97.80  | 22/06/2025 06:02:17   | 69.81


    ------------------------------------------------------------
    PRESSÃO ATMOSFÉRICA (hPa)
    ------------------------------------------------------------
    Cidade            | Mínima | Data/Hora             | Máxima | Data/Hora             | Média
    -----------------------------------------------------------------------------------------------
    Caxias do Sul     | 987.20 | 18/08/2025 03:15:44   | 1024.50| 11/09/2025 11:21:02   | 1007.84
    Bento Gonçalves   | 989.10 | 18/08/2025 03:10:15   | 1023.90| 11/09/2025 11:20:03   | 1008.12


    ------------------------------------------------------------
    BATERIA
    ------------------------------------------------------------
    Cidade            | Inicial (V) | Final (V) | Consumo (V)
    ------------------------------------------------------------
    Caxias do Sul     | 3.71        | 3.42      | 0.29
    Bento Gonçalves   | 3.69        | 3.51      | 0.18


    ------------------------------------------------------------
    SPREADING FACTORS UTILIZADOS
    ------------------------------------------------------------
    Cidade            | SF utilizados
    ------------------------------------------------------------
    Caxias do Sul     | SF7, SF8, SF9
    Bento Gonçalves   | SF7, SF8


    ------------------------------------------------------------
    DESEMPENHO
    ------------------------------------------------------------
    Tempo total de execução: 0.84 segundos
    Threads utilizadas: 3
     - Thread 1: leitura dos dados
     - Thread 2: cálculo das estatísticas
     - Thread 3: registro de logs

    Arquivo de log gerado: processamento.log

    ============================================================
    Processamento finalizado com sucesso.
    ============================================================
