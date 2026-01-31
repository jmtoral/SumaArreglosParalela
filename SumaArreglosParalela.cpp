#include <iostream>
#include <vector>
#include <cstdlib> // Para rand() y srand()
#include <ctime>   // Para time()
#include <omp.h>   // Librería OpenMP
#include <iomanip> // Para dar formato bonito (setw)

// --- COLORES (Funcionan en Windows 10/11 y Linux) ---
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"

int main() {
    // Truco para activar colores en algunas consolas de Windows
    system(" ");

    // --- CORRECCIÓN AQUÍ ---
    // Usamos static_cast<unsigned int> para evitar el error de conversión
    std::srand(static_cast<unsigned int>(std::time(0)));

    int n;
    int opcion;
    std::vector<int> A, B, R;
    std::vector<int> thread_ids;

    std::cout << GREEN << "=========================================================\n" << RESET;
    // Banner ASCII
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

    std::cout << "1. Modo AUTOMATICO (Generar N elementos aleatorios)\n";
    std::cout << "2. Modo MANUAL (Tu pones los datos uno por uno)\n";
    std::cout << YELLOW << ">> Elige una opcion: " << RESET;
    std::cin >> opcion;

    if (opcion == 1) {
        std::cout << "Ingrese la cantidad de elementos a generar (ej. 1000, 50000): ";
        std::cin >> n;

        A.resize(n); B.resize(n); R.resize(n); thread_ids.resize(n);

        std::cout << CYAN << "\n[INFO] Generando " << n << " numeros aleatorios...\n" << RESET;
        for (int i = 0; i < n; i++) {
            A[i] = std::rand() % 100;
            B[i] = std::rand() % 100;
        }
    }
    else {
        std::cout << "\nCuantos elementos desea ingresar?: ";
        std::cin >> n;
        A.resize(n); B.resize(n); R.resize(n); thread_ids.resize(n);

        std::cout << "--- Datos Arreglo A ---\n";
        for (int i = 0; i < n; i++) {
            std::cout << "A[" << i << "]: "; std::cin >> A[i];
        }
        std::cout << "--- Datos Arreglo B ---\n";
        for (int i = 0; i < n; i++) {
            std::cout << "B[" << i << "]: "; std::cin >> B[i];
        }
    }

    // --- PROCESAMIENTO PARALELO ---
    std::cout << BLUE << "\n[PROCESANDO] Soltando los hilos..." << RESET << "\n";

#pragma omp parallel for
    for (int i = 0; i < n; i++) {
        R[i] = A[i] + B[i];
        thread_ids[i] = omp_get_thread_num();
    }

    // --- IMPRESIÓN CON ESTILO Y MUESTREO ALEATORIO ---
    std::cout << "\n" << MAGENTA << "--- RESULTADOS ---" << RESET << "\n";

    std::cout << std::left
        << std::setw(8) << "Index"
        << std::setw(10) << "Hilo(CPU)"
        << std::setw(8) << "Val A"
        << std::setw(5) << "+"
        << std::setw(8) << "Val B"
        << std::setw(5) << "="
        << std::setw(8) << "TOTAL" << "\n";
    std::cout << "--------------------------------------------------------\n";

    int cantidad_a_mostrar = (n <= 20) ? n : 20;

    for (int k = 0; k < cantidad_a_mostrar; k++) {
        int i;
        if (n <= 20) {
            i = k;
        }
        else {
            i = std::rand() % n; // Aleatorio
        }

        std::string colorHilo = (thread_ids[i] % 2 == 0) ? CYAN : YELLOW;

        std::cout << std::left
            << std::setw(8) << i
            << colorHilo << "Core " << thread_ids[i] << RESET
            << std::setw(3) << ""
            << std::setw(8) << A[i]
            << std::setw(5) << "+"
            << std::setw(8) << B[i]
            << std::setw(5) << "="
            << GREEN << std::setw(8) << R[i] << RESET << "\n";
    }

    if (n > 20) {
        std::cout << "...\n" << RED << "[Mostrando 20 muestras aleatorias de " << n << " elementos]" << RESET << "\n";
    }

    std::cout << "\n" << GREEN << ">> Just remember ALL CAPS when you spell the man name." << RESET << "\n";

    std::cout << "\nPresiona Enter para salir...";
    std::cin.ignore(); // Limpia el buffer
    std::cin.get();    // Espera un Enter

    return 0;
}