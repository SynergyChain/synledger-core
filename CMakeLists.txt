cmake_minimum_required(VERSION 3.10)
project(SynLedger)

# Устанавливаем стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Включаем пути к директориям с заголовками
include_directories(${CMAKE_SOURCE_DIR}/include)

# Добавляем поддиректории исходного кода и тестов
add_subdirectory(src)
add_subdirectory(tests)

# Найдем и подключим OpenSSL и потоки
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

# Создаем главный исполняемый файл (основное приложение)
add_executable(synledger
    src/main.cpp
    src/network/p2p_protocol.cpp
    src/network/node_discovery.cpp
    src/ledger/block.cpp
    src/ledger/ledger.cpp
    src/consensus/posyg_engine.cpp
    src/consensus/consensus.cpp
    src/cryptography/crypto.cpp
    src/cryptography/ecdsa.cpp
    src/cryptography/zk_proofs.cpp
    src/governance/governance.cpp
    src/subnet/subnet_manager.cpp
)

# Линкуем библиотеки с исполняемым файлом
target_link_libraries(synledger consensus cryptography OpenSSL::SSL OpenSSL::Crypto Threads::Threads)
