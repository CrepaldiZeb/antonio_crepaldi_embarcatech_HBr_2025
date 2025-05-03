# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

# --- Configurações Iniciais (mantidas do exemplo anterior) ---
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Integração com VS Code (mantida do exemplo anterior) ---
# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
# Use as versões detectadas pela sua extensão ou ajuste se necessário
set(sdkVersion 2.1.1) # Exemplo, use a sua versão
set(toolchainVersion 14_2_Rel1) # Exemplo, use a sua versão
set(picotoolVersion 2.1.1) # Exemplo, use a sua versão
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

# --- Configuração da Placa e SDK (mantidas) ---
set(PICO_BOARD pico_w CACHE STRING "Board type")
include(pico_sdk_import.cmake)

# --- Definição do Projeto (mantido) ---
project(meu_projeto_modular C CXX ASM) # Nome do projeto pode ser alterado

# --- Inicialização do SDK (mantido) ---
pico_sdk_init()

# --- Definição do Executável (ATUALIZADO) ---
# Lista todos os arquivos .c que compõem o executável
add_executable(${PROJECT_NAME}
    app/main.c
    drivers/led_embutido.c
    hal/hal_led.c
)

# --- Adicionar Diretório de Includes (NOVO) ---
# Informa ao compilador onde encontrar os arquivos .h
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --- Vinculação de Bibliotecas (mantido e essencial) ---
# Bibliotecas padrão do Pico
target_link_libraries(${PROJECT_NAME} pico_stdlib)

# Biblioteca específica para o hardware CYW43 (LED do Pico W)
if (PICO_CYW43_SUPPORTED)
    target_link_libraries(${PROJECT_NAME} pico_cyw43_arch_none)
endif()

# --- Geração de Arquivos de Saída (mantido) ---
pico_add_extra_outputs(${PROJECT_NAME})

# --- Opcional: Definir o delay do blink via CMake ---
# target_compile_definitions(${PROJECT_NAME} PUBLIC BLINK_DELAY_MS=1000) # Ex: 1 segundo