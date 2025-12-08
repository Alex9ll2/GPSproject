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
#define INPUT_SIZE 100

//gcc main.c graph.c map.c set.c pq.c utils.c -o app

void mostrarMenu() {
    printf("\n");
    printf("╔══════════════════════════════════════╗\n");
    printf("║%s       SISTEMA GPS - MENU PRINCIPAL   %s║\n", CYAN, RESET);
    printf("╠══════════════════════════════════════╣\n");
    printf("║%s  1. Ver el grafo actual              %s║\n", BOLD, RESET);
    printf("║%s  2. Agregar una arista               %s║\n", BOLD, RESET);
    printf("║%s  3. Remover una arista               %s║\n", BOLD, RESET);
    printf("║%s  4. Remover un Vertice               %s║\n", BOLD, RESET);
    printf("║%s  5. Calcular ruta mas corta          %s║\n", BOLD, RESET);
    printf("║%s  6. Ejecutar ejemplo predefinido     %s║\n", BOLD, RESET);
    printf("║%s  7. Salir del programa               %s║\n", RED, RESET);
    printf("╚══════════════════════════════════════╝\n");
    printf("Seleccione una opcion: ");
}

// lee una cadena de texto del usuario
// recibe el mensaje a mostrar y donde guardar la respuesta
void leerVertice(char* mensaje, char* destino, int tamano) 
{
    printf("%s", mensaje);

    if (fgets(destino, tamano, stdin) == NULL) 
    {
        destino[0] = '\0';
        return;
    }

    size_t len = strlen(destino);

    if (len > 0 && destino[len-1] == '\n') 
    {
      destino[len-1] = '\0';
    }
}

// lee un numero entero del usuario
// recibe el mensaje a mostrar y retorna el numero leido
int leerPeso(char* mensaje) {
    int peso;
    printf("%s", mensaje);
    scanf("%d", &peso);
    return peso;
}

// opcion del menu para agregar una arista al grafo
// pide al usuario el vtx origen, vtx destino y peso
void opcionAgregarArista(graph* g) {
    // variables para guardar los datos que ingresa el usuario
    static char vtxs[100][INPUT_SIZE];  // ruardamos los vtxs para que no se pierdan
    static int contadorVertices = 0;

    
    char tempOrigen[INPUT_SIZE];
    char tempDestino[INPUT_SIZE];
    int peso;
    
    printf("\n--- AGREGAR NUEVA ARISTA ---\n");
    
    // Pedimos el vtx de origen
    leerVertice("Ingrese el vertice de origen: ", tempOrigen, INPUT_SIZE);
    
    // Pedimos el vtx de destino
    leerVertice("Ingrese el vertice de destino: ", tempDestino, INPUT_SIZE);
    
    // Pedimos el peso (distancia o costo)
    peso = leerPeso("Ingrese el peso de la arista: ");
    
    // Copiamos los vtxs a memoria persistente
    strcpy(vtxs[contadorVertices], tempOrigen);
    char* origen = vtxs[contadorVertices];
    contadorVertices++;
    
    strcpy(vtxs[contadorVertices], tempDestino);
    char* destino = vtxs[contadorVertices];
    contadorVertices++;
    
    // Agregamos la arista al grafo
    addEdge(g, origen, destino, peso);
    
    printf("\n%s✓ Arista agregada exitosamente: %s ←→ %s (peso: %d)\n%s", GREEN, origen, destino, peso, RESET);
}


void opcionRemoverArista(graph* g)
{
  char tempFrom[INPUT_SIZE];
  char tempTo[INPUT_SIZE];
  printf("\n");
  printf("\n");
  printf(RED"ELIMINAR ARISTA\n"RESET);  
  leerVertice("Ingrese el origen de la arista a eliminar: ", tempFrom, INPUT_SIZE);
  leerVertice("Ingrese el destino de la arista a eliminar: ", tempTo, INPUT_SIZE);
  
  if(removeEdge(g, tempFrom, tempTo))
  {
    printf("Se eliminó la arista correctamente\n");
    return;
  }

  printf("Arista no encontrada\n");
}


void opcionRemoverVertice(graph* g)
{
  char tempVertex[INPUT_SIZE];
  printf("\n");
  printf("\n");
  printf(RED"ELIMINAR VERTICE\n"RESET);  
  leerVertice("Ingrese el vertice a eliminar: ", tempVertex, INPUT_SIZE);
  
  if(graph_removeVertex(g, tempVertex))
  {
    printf("Se eliminó el vertice correctamente\n");
    return;
  }

  printf("Vertice no encontrado\n");
}

// mostrar el grafo de forma bonita y facil de entender
// recibe el grafo y el mapa de adyacencia interno
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

// mostrar los resultados de dijkstra de forma amigable
// recibe el mapa con las distancias y el nombre del origen
void mostrarDijkstraBonito(map* resultado, char* origen) {
    printf("\n");
    printf("\n");
    printf("%s         DISTANCIAS DESDE VERTICE: %-10s       %s\n", PURPLE, origen, RESET);
    printf("\n");
    printf("│   Destino    │   Distancia                     \n");
    printf("├──────────────┼─────────────\n");
    
    // Recorremos el mapa y mostramos cada distancia
    map_iterator* it = map_iter_create(resultado);
    
    while (map_iter_has_next(it)) {
        void* vtx = map_iter_next(it);
        int* distancia = (int*) map_get(resultado, vtx);
        
        // Si la distancia es muy grande (infinito), mostramos "∞"
        if (*distancia == INT_MAX || *distancia < 0) {
            printf("│      %-8s│   No alcanzable (∞)            \n", (char*)vtx);
        } else {
            printf("│      %-8s│   %d                             \n", (char*)vtx, *distancia);
        }
    }
    
    map_iter_destroy(it);
    
    printf("\n");
}


void opcionCalcularRuta(graph* g) {
    char tempOrigen[INPUT_SIZE];
    
    printf("\n");
    printf("\n");
    printf(YELLOW"CALCULAR RUTA MAS CORTA (DIJKSTRA)\n"RESET);
    printf("\n");
    
    leerVertice("\nIngrese el vertice de origen: ", tempOrigen, INPUT_SIZE);
    
    map* resultado = dijkstra(g, tempOrigen);

    if(resultado == NULL)
    {
      printf("No se encontró el vertice\n");
      return;
    }
    
    mostrarDijkstraBonito(resultado, tempOrigen);
    
    map_destroy(resultado);
}

// ejemplo que sirve para demostrar como funciona el programa con datos conocidos
void opcionEjemploPredefinido(graph* g) {
    printf("\n");
    printf("\n");
    printf(GREEN"EJECUTANDO EJEMPLO PREDEFINIDO\n"RESET);
    printf("\n");
    printf("\nAgregando aristas del ejemplo...\n");
    
    // Estas son las aristas del ejemplo original
    // Representan un grafo con 6 vtxs: A, B, C, D, E, F
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
    
    // Calculamos Dijkstra desde cada vtx
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


int main()
{
    graph* g = createGraph(INITIAL_CAPACITY, wordHash, wordEquals, wordCompare, printString);
    
    int opcion;
    
    printf("\n");
    printf("\n");
    printf("\n");
    printf(BOLDYELLOW"BIENVENIDO AL SISTEMA GPS\n"RESET);
    printf(BOLD"Proyecto de Estructuras de Datos\n"RESET);
    printf("\n");
    printf("\n");
    
    do {
        mostrarMenu();
        scanf("%d", &opcion);
        getchar();
        
        switch (opcion) {
            case 1:
                //mostrar el grafo actual
                printf("\n");
                printf("┌─────────────────────────────────────────────────┐\n");
                printf("│              GRAFO ACTUAL                       │\n");
                printf("└─────────────────────────────────────────────────┘\n");
                graph_print(g);
                break;
                
            case 2:
                // agregar una nueva arista
                opcionAgregarArista(g);
                break;

            case 3:
                // remover arista
                opcionRemoverArista(g);
                break;

            case 4:
                // remover vtx
                opcionRemoverVertice(g);
                break;
                
            case 5:
                // calcular ruta mas corta
                opcionCalcularRuta(g);
                break;
                
            case 6:
                // ejecutar el ejemplo predefinido
                opcionEjemploPredefinido(g);
                break;
                
            case 7:
                // salir del programa
                printf("\n");
                printf("\n");
                printf(GREEN"Gracias por usar el Sistema GPS\n"RESET);
                printf(GREEN"Hasta luego!\n"RESET);
                printf("\n");
                break;
                
            default:
                printf("\n%s⚠ Opcion no valida. Por favor, elija entre 1 y 5.%s\n", RED, RESET);
                break;
        }
        
    } while (opcion != 7);  
    
    graph_destroy(g);
    
    return 0;
}