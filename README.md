# Tarea 1: Solución Paralela - Suma de Arreglos

Este repositorio contiene la implementación de un algoritmo en **C++** diseñado para realizar la suma de dos arreglos ($A + B = R$) de manera paralela, utilizando la librería **OpenMP**.

## 🎯 Objetivo
Demostrar el uso de memoria compartida y paralelismo de datos mediante la distribución de iteraciones de un ciclo `for` entre múltiples hilos de procesamiento (threads).

## 🛠️ Características
* **Procesamiento Paralelo:** Uso de la directiva `#pragma omp parallel for` para dividir la carga de trabajo.
* **Visualización de Hilos:** El programa identifica y muestra qué núcleo (Core ID) procesó cada índice del arreglo.
* **Modos de Ejecución:**
    * **Automático:** Generación de hasta 1,000,000+ de elementos aleatorios para pruebas de estrés.
    * **Manual:** Entrada de datos personalizada para validación aritmética simple.
* **Interfaz CLI:** Salida formateada con soporte para caracteres ASCII y códigos de color ANSI.

## 📋 Requisitos
* Compilador de C++ compatible con OpenMP (Visual Studio MSVC, GCC, Clang).
* En Visual Studio: Activar "OpenMP Support" en `Properties > C/C++ > Language`.

## 🚀 Ejecución
El código principal se encuentra en el archivo `.cpp`. Al ejecutarlo, seleccione el modo de operación deseado en el menú de consola.

---
**Autor:** José Manuel Toral Cruz
**Curso:** Computo en la Nube / Maestría en IA Aplicada - Tec de Monterrey