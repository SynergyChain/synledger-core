# src/CMakeLists.txt

# Добавляем файлы исходного кода для библиотеки consensus
add_library(consensus
    consensus/consensus.cpp
    consensus/posyg_engine.cpp
)

# Добавляем файлы исходного кода для библиотеки cryptography
add_library(cryptography
    cryptography/crypto.cpp
    cryptography/ecdsa.cpp
    cryptography/zk_proofs.cpp
)

# Добавляем файлы исходного кода для библиотеки economic
add_library(economic
    economic/synergy_model.cpp
)

# Добавляем файлы исходного кода для библиотеки governance
add_library(governance
    governance/governance.cpp
)

# Добавляем файлы исходного кода для библиотеки ledger
add_library(ledger
    ledger/block.cpp
    ledger/ledger.cpp
)

# Добавляем файлы исходного кода для библиотеки network
add_library(network
    network/node_discovery.cpp
    network/p2p_protocol.cpp
)

# Добавляем файлы исходного кода для библиотеки subnet
add_library(subnet
    subnet/subnet_manager.cpp
)

# Указываем компилятору, где искать заголовочные файлы
target_include_directories(consensus PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(cryptography PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(economic PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(governance PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(ledger PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(network PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_include_directories(subnet PUBLIC ${CMAKE_SOURCE_DIR}/include)
