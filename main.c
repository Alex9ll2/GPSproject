#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "headers/map.h"
#include "headers/set.h"
#include "headers/utils.h"
#include "headers/graph.h"


#define BLUE "\033[0;34m"
#define RESET "\033[0m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BOLD "\033[1m"
#define BOLDYELLOW "\033[1;33m"
#define BOLDGREEN "\033[1;32m"
#define BOLDBLUE "\033[1;34m"
#define GOLD "\033[1;33m"
#define PURPLE "\033[1;35m"
#define CYAN "\033[1;36m"
#define RED "\033[1;31m"


#define INITIAL_CAPACITY 100

//gcc main.c graph.c map.c set.c pq.c utils.c -o app
//graph


// Abimael


// Esta funcion muestra el menu principal en la consola
// Es muy sencilla: solo imprime las opciones disponibles
void mostrarMenu() {
    printf("\n");
    printf("╔══════════════════════════════════════╗\n");
    printf("║%s       SISTEMA GPS - MENU PRINCIPAL   %s║\n", CYAN, RESET);
    printf("╠══════════════════════════════════════╣\n");
    printf("║%s  1. Ver el grafo actual              %s║\n", BOLD, RESET);
    printf("║%s  2. Agregar una arista               %s║\n", BOLD, RESET);
    printf("║%s  3. Calcular ruta mas corta          %s║\n", BOLD, RESET);
    printf("║%s  4. Ejecutar ejemplo predefinido     %s║\n", BOLD, RESET);
    printf("║%s  5. Salir del programa               %s║\n", RED, RESET);
    printf("╚══════════════════════════════════════╝\n");
    printf("Seleccione una opcion: ");
}

// Esta funcion lee una cadena de texto del usuario
// Recibe el mensaje a mostrar y donde guardar la respuesta
void leerNodo(char* mensaje, char* destino, int tamano) {
    printf("%s", mensaje);
    scanf("%s", destino);
}

// Esta funcion lee un numero entero del usuario
// Recibe el mensaje a mostrar y retorna el numero leido
int leerPeso(char* mensaje) {
    int peso;
    printf("%s", mensaje);
    scanf("%d", &peso);
    return peso;
}

// Esta funcion permite agregar una arista al grafo
// Pide al usuario: nodo origen, nodo destino y peso
void opcionAgregarArista(graph* g) {
    // Variables para guardar los datos que ingresa el usuario
    static char nodos[100][10];  // Guardamos los nodos para que no se pierdan
    static int contadorNodos = 0;
    
    char tempOrigen[10];
    char tempDestino[10];
    int peso;
    
    printf("\n--- AGREGAR NUEVA ARISTA ---\n");
    
    // Pedimos el nodo de origen
    leerNodo("Ingrese el nodo de origen: ", tempOrigen, 10);
    
    // Pedimos el nodo de destino
    leerNodo("Ingrese el nodo de destino: ", tempDestino, 10);
    
    // Pedimos el peso (distancia o costo)
    peso = leerPeso("Ingrese el peso de la arista: ");
    
    // Copiamos los nodos a memoria persistente
    strcpy(nodos[contadorNodos], tempOrigen);
    char* origen = nodos[contadorNodos];
    contadorNodos++;
    
    strcpy(nodos[contadorNodos], tempDestino);
    char* destino = nodos[contadorNodos];
    contadorNodos++;
    
    // Agregamos la arista al grafo
    addEdge(g, origen, destino, peso);
    
    printf("\n%s✓ Arista agregada exitosamente: %s ←→ %s (peso: %d)\n%s", GREEN, origen, destino, peso, RESET);
}

// Esta funcion muestra el grafo de forma bonita y facil de entender
// Recibe el grafo y el mapa de adyacencia interno
void mostrarGrafoBonito(graph* g, map* adyacencia) {
    printf("\n");
    printf("┌─────────────────────────────────────────────────┐\n");
    printf("│              VISUALIZACION DEL GRAFO            │\n");
    printf("├─────────────────────────────────────────────────┤\n");
    
    // Usamos la funcion original para imprimir
    // pero primero mostramos un encabezado bonito
    graph_print(g);
    
    printf("└─────────────────────────────────────────────────┘\n");
}

// Esta funcion muestra los resultados de Dijkstra de forma amigable
// Recibe el mapa con las distancias y el nombre del origen
void mostrarDijkstraBonito(map* resultado, char* origen) {
    printf("\n");
    printf("\n");
    printf("%s         DISTANCIAS DESDE NODO: %-10s       %s\n", PURPLE, origen, RESET);
    printf("\n");
    printf("│   Destino    │   Distancia                     \n");
    printf("├──────────────┼─────────────\n");
    
    // Recorremos el mapa y mostramos cada distancia
    map_iterator* it = map_iter_create(resultado);
    
    while (map_iter_has_next(it)) {
        void* nodo = map_iter_next(it);
        int* distancia = (int*) map_get(resultado, nodo);
        
        // Si la distancia es muy grande (infinito), mostramos "∞"
        if (*distancia == INT_MAX || *distancia < 0) {
            printf("│      %-8s│   No alcanzable (∞)            \n", (char*)nodo);
        } else {
            printf("│      %-8s│   %d                             \n", (char*)nodo, *distancia);
        }
    }
    
    map_iter_destroy(it);
    
    printf("\n");
}

// Esta funcion calcula la ruta mas corta desde un nodo origen
// Usa el algoritmo de Dijkstra que ya esta implementado
void opcionCalcularRuta(graph* g) {
    static char nodosRuta[100][10];
    static int contadorRuta = 0;
    
    char tempOrigen[10];
    
    printf("\n");
    printf("\n");
    printf(YELLOW"CALCULAR RUTA MAS CORTA (DIJKSTRA)\n"RESET);
    printf("\n");
    
    // Pedimos el nodo de origen para calcular las distancias
    leerNodo("\nIngrese el nodo de origen: ", tempOrigen, 10);
    
    // Copiamos a memoria persistente
    strcpy(nodosRuta[contadorRuta], tempOrigen);
    char* origen = nodosRuta[contadorRuta];
    contadorRuta++;
    
    // Ejecutamos Dijkstra desde ese origen
    map* resultado = dijkstra(g, origen);
    
    // Mostramos los resultados de forma bonita
    mostrarDijkstraBonito(resultado, origen);
    
    // Liberamos la memoria del mapa
    map_destroy(resultado);
}

// Esta funcion ejecuta el ejemplo que ya estaba en el codigo original
// Sirve para demostrar como funciona el programa con datos conocidos
void opcionEjemploPredefinido(graph* g) {
    printf("\n");
    printf("\n");
    printf(GREEN"EJECUTANDO EJEMPLO PREDEFINIDO\n"RESET);
    printf("\n");
    printf("\nAgregando aristas del ejemplo...\n");
    
    // Estas son las aristas del ejemplo original
    // Representan un grafo con 6 nodos: A, B, C, D, E, F
    addEdge(g, "A", "C", 2);
    addEdge(g, "A", "B", 5);
    addEdge(g, "C", "B", 7);
    addEdge(g, "B", "D", 8);
    addEdge(g, "C", "D", 4);
    addEdge(g, "C", "E", 8);
    addEdge(g, "E", "D", 6);
    addEdge(g, "E", "F", 3);
    addEdge(g, "D", "F", 4);
    
    printf(GREEN"\n✓ Se agregaron 9 aristas al grafo.\n"RESET);
    
    // Mostramos el grafo
    printf("\n");
    printf("\n");
    printf(" %sGRAFO DEL EJEMPLO%s\n", BLUE, RESET);
    printf("├─────────────────────────────────────────────────┤\n");
    graph_print(g);
    printf("\n");
    
    // Mostramos una representacion visual del grafo
    printf("\n");
    printf("┌─────────────────────────────────────────────────┐\n");
    printf(PURPLE"│           DIAGRAMA DEL GRAFO                    │\n"RESET);
    printf("├─────────────────────────────────────────────────┤\n");
    printf("│                                                 │\n");
    printf("│         (A)───5───(B)                           │\n");
    printf("│          │╲       │                             │\n");
    printf("│          2  ╲     8                             │\n");
    printf("│          │   7    │                             │\n");
    printf("│         (C)───4──(D)───4──(F)                   │\n");
    printf("│          │        │       │                     │\n");
    printf("│          8        6       3                     │\n");
    printf("│          │        │       │                     │\n");
    printf("│         (E)──────────────(E)                    │\n");
    printf("│                                                 │\n");
    printf("└─────────────────────────────────────────────────┘\n");
    
    // Calculamos Dijkstra desde cada nodo
    char* origenes[] = {"A", "B", "C", "D", "E", "F"};
    
    printf("\n");
    printf("\n");
    printf(GREEN"      RESULTADOS DE DIJKSTRA          \n"RESET);
    printf("\n");
    
    // Recorremos cada origen y mostramos las distancias
    for (int i = 0; i < 6; i++) {
        map* resultado = dijkstra(g, origenes[i]);
        mostrarDijkstraBonito(resultado, origenes[i]);
        map_destroy(resultado);
    }
    
    printf(GREEN"✓ Ejemplo completado exitosamente.\n"RESET);
}


//Abimael


int main()
{
    
    //Abimael
    
    // Creamos el grafo vacio al iniciar el programa
    graph* g = createGraph(INITIAL_CAPACITY, wordHash, wordEquals, wordCompare, printString);
    
    // Variable para guardar la opcion que elija el usuario
    int opcion;
    
    // Mensaje de bienvenida
    printf("\n");
    printf("\n");
    printf("\n");
    printf(BOLDYELLOW"BIENVENIDO AL SISTEMA GPS\n"RESET);
    printf(BOLD"Proyecto de Estructuras de Datos\n"RESET);
    printf("\n");
    printf("\n");
    
    // Este es el ciclo principal del menu
    // Se repite hasta que el usuario elija salir (opcion 5)
    do {
        // Mostramos el menu y leemos la opcion
        mostrarMenu();
        scanf("%d", &opcion);
        
        // Segun la opcion elegida, ejecutamos la accion correspondiente
        switch (opcion) {
            case 1:
                // Opcion 1: Mostrar el grafo actual
                printf("\n");
                printf("┌─────────────────────────────────────────────────┐\n");
                printf("│              GRAFO ACTUAL                       │\n");
                printf("├─────────────────────────────────────────────────┤\n");
                graph_print(g);
                printf("└─────────────────────────────────────────────────┘\n");
                break;
                
            case 2:
                // Opcion 2: Agregar una nueva arista
                opcionAgregarArista(g);
                break;
                
            case 3:
                // Opcion 3: Calcular ruta mas corta
                opcionCalcularRuta(g);
                break;
                
            case 4:
                // Opcion 4: Ejecutar el ejemplo predefinido
                opcionEjemploPredefinido(g);
                break;
                
            case 5:
                // Opcion 5: Salir del programa
                printf("\n");
                printf("\n");
                printf(GREEN"Gracias por usar el Sistema GPS\n"RESET);
                printf(GREEN"Hasta luego!\n"RESET);
                printf("\n");
                break;
                
            default:
                // Si el usuario ingresa una opcion invalida
                printf("\n%s⚠ Opcion no valida. Por favor, elija entre 1 y 5.%s\n", RED, RESET);
                break;
        }
        
    } while (opcion != 5);  // Repetir mientras no sea salir
    
    // Liberamos la memoria del grafo antes de terminar
    graph_destroy(g);
    
    // Abimael 
    
    return 0;
}