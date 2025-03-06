# Spécifie la version minimale de CMake requise
cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(MonProjet CXX)

# Définir les options de compilation
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -fsanitize=address")

# Trouver Boost, y compris les composants nécessaires
find_package(Boost 1.78 REQUIRED COMPONENTS filesystem system)

# Si Boost a été trouvé, inclure les répertoires d'en-têtes
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "Boost not found!")
endif()

# Ajouter le répertoire contenant la bibliothèque statique libbcrypt.a
include_directories(
    ${CMAKE_SOURCE_DIR}/src 
    ${CMAKE_SOURCE_DIR}/Crow/include
    ${CMAKE_SOURCE_DIR}/libs/bcrypt
)

link_directories(${CMAKE_SOURCE_DIR}/libs/bcrypt)

# Ajouter les répertoires d'inclusion de MySQL Connector/C++
include_directories(/usr/include/mysql)

# Ajouter les répertoires de bibliothèque MySQL Connector/C++
link_directories(/usr/lib)

# Ajouter le répertoire d'inclusion de spdlog
include_directories(/usr/local/include/spdlog)

# Ajouter le répertoire où la bibliothèque spdlog a été installée
link_directories(/usr/local/lib)

# --- Ajout de ta bibliothèque adastra ---

# Ajouter les répertoires d'inclusion de adastra
include_directories(/home/softadastra/adastra/include)

# Ajouter le répertoire de bibliothèques de adastra
link_directories(/home/softadastra/adastra/libs)

# Trouver OpenSSL sur ton système
find_package(OpenSSL REQUIRED)

# Ajouter le répertoire d'inclusion de OpenSSL
include_directories(${OPENSSL_INCLUDE_DIR})

# Assure-toi que tous les fichiers source sont inclus
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

# Ajouter l'exécutable et spécifier les fichiers sources
add_executable(prog ${SOURCE_FILES})

# Lier la bibliothèque MySQL Connector/C++ au projet
# Lier toutes les bibliothèques nécessaires
target_link_libraries(prog PRIVATE
    pthread
    bcrypt
    ssl
    crypto
    mysqlcppconn
    ${Boost_LIBRARIES}
    spdlog
    OpenSSL::SSL
    OpenSSL::Crypto
    /home/softadastra/adastra/libs/libadastra.so  
)

# Lier Boost à ton projet en utilisant les fichiers spécifiques au lieu des alias
target_link_libraries(prog PRIVATE ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})

# Lier spdlog à ton projet
target_link_libraries(prog PRIVATE spdlog)

# Lier OpenSSL à ton projet
target_link_libraries(prog PRIVATE OpenSSL::SSL OpenSSL::Crypto)

# Lier GTest si nécessaire
target_link_libraries(prog PRIVATE
    /usr/local/lib/libgtest.a
    /usr/local/lib/libgtest_main.a
)
