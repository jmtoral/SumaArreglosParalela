#include <iostream>
#include <vector>
#include <cstdlib> // Para rand()
#include <ctime>   // Para time()
#include <omp.h>   // Librería OpenMP obligatoria
#include <iomanip> // Para la tabla bonita
#include <string>

// --- CONFIGURACIÓN ---
#define CHUNK 100  // Tamaño del pedazo para cada hilo 
#define MOSTRAR 20 // <--- CAMBIAR: N muestras a mostrar

// --- COLORES ---
#define RESET   "\033[0m"
#define GREEN   "\033[32m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"

// Formatea enteros con separadores de miles
std::string format_with_commas(long long value) {
    std::string s = std::to_string(value);
    int insertPosition = static_cast<int>(s.length()) - 3;
    while (insertPosition > 0) {
        s.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return s;
}

int main() {
    // Configuración inicial
    system(" ");
    std::srand(static_cast<unsigned int>(std::time(0)));

    int n;
    int opcion;
    int chunk = CHUNK;

    std::vector<float> A, B, R;
    std::vector<int> thread_ids;

    // --- ENCABEZADO ---
    std::cout << GREEN << "=========================================================\n" << RESET;
    std::cout << R"(
  ___  ___ ______   ______   _____   _____  ___  ___ 
  |  \/  | | ___|   |  _  \ |  _  | |  _  | |  \/  | 
  | .  . | | |_     | | | | | | | | | .  . | 
  | |\/| | |  _|    | | | | | | | | | | | |\/| | 
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
            A[i] = static_cast<float>(rand() % 1000) / 10.0f;
            B[i] = static_cast<float>(rand() % 1000) / 10.0f;
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

    // Preparar almacenamiento para tiempos por hilo
    int max_threads = omp_get_max_threads();
    std::vector<double> thread_times(max_threads, 0.0);
    std::vector<int> thread_counts(max_threads, 0);

    // número real de hilos usados dentro de la región paralela
    int actual_threads = 0;

    // Tiempo total de la sección paralela
    double t_total_start = omp_get_wtime();

#pragma omp parallel shared(A, B, R, thread_ids, chunk, thread_times, thread_counts, actual_threads) private(i)
    {
        int tid = omp_get_thread_num();

        // capturar número real de hilos (una sola vez)
#pragma omp single
        actual_threads = omp_get_num_threads();

        double t_thread_start = omp_get_wtime();

#pragma omp for schedule(static, chunk)
        for (i = 0; i < n; i++) {
            R[i] = A[i] + B[i];
            thread_ids[i] = tid;
            thread_counts[tid]++; // cada hilo actualiza su propio contador
        }

        double t_thread_end = omp_get_wtime();
        thread_times[tid] = t_thread_end - t_thread_start;
    } // fin paralelo

    double t_total_end = omp_get_wtime();
    double total_time = t_total_end - t_total_start;

    // --- CONFIGURACION DE EJECUCION ---
    std::cout << "\n" << MAGENTA << "[CONFIGURACION DE EJECUCION]" << RESET << "\n";
    std::cout << std::left;
    std::cout << "Cantidad de elementos: " << format_with_commas(static_cast<long long>(n)) << "\n";
    std::cout << "Paralelizacion:       OpenMP Schedule(Static, " << chunk << ")\n";
    std::cout << "Estado:               Procesamiento completado con exito\n";

    // --- IMPRESIÓN ---
    std::cout << "\n" << MAGENTA << "--- MUESTREO ALEATORIO (" << MOSTRAR << " RESULTADOS) ---" << RESET << "\n";

    std::cout << std::left
        << std::setw(8) << "Index"
        << std::setw(10) << "Hilo"
        << std::setw(10) << "Arreglo A"
        << std::setw(5) << "+"
        << std::setw(10) << "Arreglo B"
        << std::setw(5) << "="
        << std::setw(10) << "Resultado" << "\n";
    std::cout << "--------------------------------------------------------------\n";

    int cantidad_a_mostrar = (n <= MOSTRAR) ? n : MOSTRAR;

    for (int k = 0; k < cantidad_a_mostrar; k++) {
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

    // Imprimir tiempos
    std::cout << "\n" << GREEN << "--- TIEMPOS ---" << RESET << "\n";
    std::cout << std::fixed << std::setprecision(6);

    // Mostrar cuántos hilos se usaron realmente en la región paralela
    std::cout << "Numero de hilos (region paralela): " << actual_threads << "\n";
    std::cout << "Tiempo total (seccion paralela): " << total_time << " s\n";

    std::cout << "\nTiempos por hilo (id : tiempo [s] , elementos procesados):\n";
    for (int t = 0; t < max_threads; t++) {
        if (thread_counts[t] > 0) {
            std::cout << "  Hilo " << t << " : " << thread_times[t]
                << " s , " << thread_counts[t] << " elementos\n";
        }
    }

    std::cout << "\n" << GREEN << ">> Just remember ALL CAPS when you spell the man name." << RESET << "\n";

    std::cout << "\n" << CYAN << ">> Todo bien, bye." << RESET << "\n";

    std::cout << "\nPresiona Enter para salir...";
    std::cin.ignore(); std::cin.get();

    return 0;
}