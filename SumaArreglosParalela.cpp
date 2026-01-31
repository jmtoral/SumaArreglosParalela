#include <iostream>
#include <vector>
#include <cstdlib> // Para rand()
#include <ctime>   // Para time()
#include <omp.h>   // Librería OpenMP obligatoria
#include <iomanip> // Para la tabla bonita

// --- CONFIGURACIÓN ---
#define CHUNK 100  // Tamaño del pedazo para cada hilo 
#define MOSTRAR 50 // <--- CAMBIO AQUÍ: Ahora mostramos N muestras

// --- COLORES ---
#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"

int main() {
    // Configuración inicial
    system(" ");
    std::srand(static_cast<unsigned int>(std::time(0)));

    int n;
    int opcion;
    int chunk = CHUNK;

    std::vector<float> A, B, R;
    std::vector<int> thread_ids;

    // --- ENCABEZADO MF DOOM ---
    std::cout << GREEN << "=========================================================\n" << RESET;
    std::cout << R"(
  ___  ___ ______   ______   _____   _____  ___  ___ 
  |  \/  | | ___|   |  _  \ |  _  | |  _  | |  \/  | 
  | .  . | | |_     | | | | | | | | | | | | | .  . | 
  | |\/| | |  _|    | | | | | | | | | | | | | |\/| | 
  | |  | | | |      | |/ /  \ \_/ / \ \_/ / | |  | | 
  \_|  |_/ \_|      |___/    \___/   \___/  \_|  |_/ 
)" << "\n";
    std::cout << GREEN << "      O P E R A T I O N   :   D O O M S D A Y\n";
    std::cout << "=========================================================\n" << RESET;

    std::cout << "1. Modo AUTOMATICO (Generar N decimales aleatorios)\n";
    std::cout << "2. Modo MANUAL (Tu pones los datos)\n";
    std::cout << YELLOW << ">> Elige una opcion: " << RESET;
    std::cin >> opcion;

    if (opcion == 1) {
        std::cout << "Cantidad de elementos a generar: ";
        std::cin >> n;
        A.resize(n); B.resize(n); R.resize(n); thread_ids.resize(n);

        std::cout << CYAN << "\n[INFO] Generando floats aleatorios...\n" << RESET;
        for (int i = 0; i < n; i++) {
            // Genera decimales aleatorios
            A[i] = (float)(rand() % 1000) / 10.0f;
            B[i] = (float)(rand() % 1000) / 10.0f;
        }
    }
    else {
        std::cout << "Cuantos elementos?: ";
        std::cin >> n;
        A.resize(n); B.resize(n); R.resize(n); thread_ids.resize(n);

        std::cout << "--- Datos Arreglo A ---\n";
        for (int i = 0; i < n; i++) { std::cout << "A[" << i << "]: "; std::cin >> A[i]; }
        std::cout << "--- Datos Arreglo B ---\n";
        for (int i = 0; i < n; i++) { std::cout << "B[" << i << "]: "; std::cin >> B[i]; }
    }

    // --- PARALELISMO ---
    std::cout << BLUE << "\n[PROCESANDO] Ejecutando OpenMP Schedule(Static, " << chunk << ")...\n" << RESET;

    int i;

#pragma omp parallel for \
    shared(A, B, R, thread_ids, chunk) private(i) \
    schedule(static, chunk)
    for (i = 0; i < n; i++) {
        R[i] = A[i] + B[i];
        thread_ids[i] = omp_get_thread_num();
    }

    // --- IMPRESIÓN ---
    std::cout << "\n" << MAGENTA << "--- MUESTREO ALEATORIO (20 RESULTADOS) ---" << RESET << "\n";

    std::cout << std::left
        << std::setw(8) << "Index"
        << std::setw(10) << "Hilo"
        << std::setw(10) << "Arreglo A"
        << std::setw(5) << "+"
        << std::setw(10) << "Arreglo B"
        << std::setw(5) << "="
        << std::setw(10) << "Resultado" << "\n";
    std::cout << "--------------------------------------------------------------\n";

    // Lógica para mostrar siempre N (o menos si n es pequeño)
    int cantidad_a_mostrar = (n <= MOSTRAR) ? n : MOSTRAR;

    for (int k = 0; k < cantidad_a_mostrar; k++) {
        // Si n es grande, elegimos un índice al azar en cada vuelta
        // Si n es pequeño, vamos en orden (k)
        int idx = (n <= MOSTRAR) ? k : (rand() % n);

        std::string colorHilo = (thread_ids[idx] % 2 == 0) ? CYAN : YELLOW;

        std::cout << std::left
            << std::setw(8) << idx
            << colorHilo << "Core " << thread_ids[idx] << RESET
            << std::setw(3) << ""
            << std::fixed << std::setprecision(2) << std::setw(10) << A[idx]
            << std::setw(5) << "+"
            << std::setw(10) << B[idx]
            << std::setw(5) << "="
            << GREEN << std::setw(10) << R[idx] << RESET << "\n";
    }

    std::cout << "\n" << GREEN << ">> Just remember ALL CAPS when you spell the man name." << RESET << "\n";

    std::cout << "\nPresiona Enter para salir...";
    std::cin.ignore(); std::cin.get();

    return 0;
}