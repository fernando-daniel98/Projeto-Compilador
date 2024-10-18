cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(Compilador)
set(EXECUTABLE_NAME "main")

# Definindo o padrão do C
set(CMAKE_C_STANDARD 99)

# Definindo o diretório onde estão os arquivos fonte
set(SOURCE_DIR "Src")
include_directories(${SOURCE_DIR})

set(INCLUDE_DIR "Include")
include_directories(${INCLUDE_DIR})

# Adicionar os arquivos que serão compilados há medida que forem implementados
set(SOURCE_FILES
    ${SOURCE_DIR}/main.c
    ${SOURCE_DIR}/lexico.c
    )

# Adicionando o executável
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})