# Version minimum de cmake
cmake_minimum_required(VERSION 3.21)

project(programme C)

set(CMAKE_C_STANDARD 11)


# On utilise le module PkgConfig pour détecter la librairie GTK+ sur le système
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)

# On dit à CMake d'utiliser GTK+, on indique au compilateur où trouver les fichiers headers
# Et au linker où trouver les librairies
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})




file(GLOB_RECURSE RESOURCES *.png)









# Instructions pour le compilateur
ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})

##Pour linux, va servir à correctement liers les signaux de notre application au code
#if (UNIX)
#    set(CMAKE_EXE_LINKER_FLAGS "-Wl,-export-dynamic")
#endif (UNIX)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/images
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


# Ajoute un éxecutable à partir du main.c
add_executable(programme
        acc_aleatoire.h
        anim_rules.h
        animals.h
        choisir_parmis_les_regles.h
        help_rp.h
        interface_globale.h
        lesregles.h
        main.c
        main_headers.h
        myfunctions.h
        myheaders.h
        ur_own_rules.h
        )



# Lie à l'éxecutable la librairie GTK+
TARGET_LINK_LIBRARIES(programme ${GTK3_LIBRARIES})