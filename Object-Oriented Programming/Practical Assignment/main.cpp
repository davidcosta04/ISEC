#include <iostream>
#include <fstream>
#include <string>
#include "Simulador.h"
#include "UI.h"

using namespace std;


int main() {

    Simulador s;

    UI ui(s);

    ui.comando();
    return 0;

}


