# fundamentos-OS-Projeto

Trabalho de análise de dados de sensoriamento utilizando **pthreads**.

------------------------------------------------------------------------

## Requisitos

É necessário ter instalado no Linux:

-   CMake
-   Make
-   GCC

------------------------------------------------------------------------

## Build e execução do projeto

No diretório raiz do projeto, execute:

``` bash
mkdir build
cd build
```

Os comandos acima criam o diretório de build do projeto e entram nele.

Depois, execute:

``` bash
cmake ..
make
./analise
```

------------------------------------------------------------------------

## Estrutura esperada do projeto

``` text
fundamentos-OS-Projeto/
├── CMakeLists.txt
├── include/
├── input/
├── src/
├── build/
├── README.md
└── descricao_projeto.md
```

### include/

Todo arquivo de cabeçalho `.h` deve ser colocado nessa pasta.\
O CMake irá incluir esse diretório no processo de compilação.

### src/

Arquivos de código `.c`.\
Ao criar um novo arquivo `.c`, ele deve ser adicionado ao
`CMakeLists.txt`.

Exemplo:

``` cmake
add_executable(analise
    src/main.c
    src/arquivo_novo_exemplo.c
)
```

### build/

Diretório onde o build do projeto é gerado.\
Essa pasta está no `.gitignore`, portanto cada integrante deve ter o seu
próprio diretório de build.

### input/

Arquivos `.json` de entrada dos dados.

------------------------------------------------------------------------

## Observação importante

Ao atualizar o arquivo `CMakeLists.txt`, incluindo novos arquivos `.h`
ou `.c`, é necessário repetir o processo de build antes da execução:

``` bash
cmake ..
make
./analise
```
