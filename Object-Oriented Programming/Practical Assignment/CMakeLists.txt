cmake_minimum_required(VERSION 3.26)
project(trabalhopratico)

set(CMAKE_CXX_STANDARD 17)

add_executable(trabalhopratico main.cpp
        main.cpp
        Simulador.cpp
        UI.cpp
        Buffer.cpp
        Caravana.cpp
        Cidade.cpp
        Caravana_Comercio.h
        Caravana_Comercio.cpp
        Caravana_Militar.cpp
        Caravana_Secreta.cpp
        Caravana_Barbara.h
        Caravana_Barbara.cpp
        ArcaTesouro.cpp
        CaixaPandora.cpp
        Itens.cpp
        Jaula.cpp
        Mina.cpp
        Surpresa.cpp)
