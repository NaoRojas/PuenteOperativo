/*
 ============================================================================
 Name        : ProyectoSO.c
 Author      : Nao Diego
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include "Puente.c"
#include "Cola.c"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
struct Puente PUENTE;
struct PuntosCardinales OESTE;
struct PuntosCardinales ESTE;
struct Cola COLA_OESTE;
struct Cola COLA_ESTE;

void enQueue(struct Auto* a, struct Cola* c);

struct Auto* deQueue(struct Cola* c);

void bloquear();

void desbloquear();

void* semaforo(void* arg);

void* Puente_A_Terabithia(void* arg);

void* creandoAutosEste(void* arg);

void* creandoAutosOeste(void* arg);

void La_Ladrona_de_Libros();

void toString(struct Cola* c);

int main(int arg, char const * argv[]) {

    srand(time(NULL));
    La_Ladrona_de_Libros();

    //Se recupera los datos de un archivo de texto
    printf("-------------------------------------------------------------- \n");
    printf("------------------------MENU PRINCIPAL------------------------ \n");
    printf("-------------------------------------------------------------- \n");
    printf("1.\t	Normal \n");
    printf("2.\t	Semáforo \n");
    printf("3.\t	Oficial de Tránsito \n");
    printf("-------------------------------------------------------------- \n");
    printf("-------------------------------------------------------------- \n");
    printf("DIGITE UNA OPCION:  ");
    int c;
    scanf("%d",&c);
    system("clear");
    switch(c){
    	case 1:{
    		printf("-------------------------------------------------------------- \n");
    		printf("------------------------MODO NORMAL--------------------------- \n");
    		printf("-------------------------------------------------------------- \n");

    	};break;
    	case 2:{
    		printf("-------------------------------------------------------------- \n");
    		printf("------------------------MODO SEMAFORO------------------------- \n");
    		printf("-------------------------------------------------------------- \n");
    		pthread_t tid;
    		    pthread_attr_t attr;
    		    pthread_attr_init(&attr);
    		    pthread_create(&tid, &attr, &Puente_A_Terabithia, NULL);
    		//
    		    pthread_t tid2;
    		    pthread_attr_t attr2;
    		    pthread_attr_init(&attr2);
    		    pthread_create(&tid2, &attr2, &semaforo, NULL);

    		    pthread_t tid3;
    		    pthread_attr_t attr3;
    		    pthread_attr_init(&attr3);
    		    pthread_create(&tid3, &attr3, &creandoAutosEste, NULL);

    		    pthread_t tid4;
    		    pthread_attr_t attr4;
    		    pthread_attr_init(&attr4);
    		    pthread_create(&tid4, &attr4, &creandoAutosOeste, NULL);

    		    pthread_join(tid, NULL);
    		    pthread_join(tid2, NULL);
    		    pthread_join(tid3, NULL);
    		    pthread_join(tid4, NULL);


    	};break;
    	case 3:{
    		printf("-------------------------------------------------------------- \n");
    		printf("-------------------MODO OFICIAL DE TRANSITO------------------- \n");
    		printf("-------------------------------------------------------------- \n");

    	};break;
    	default:{
    		printf("-------------------------------------------------------------- \n");
    		printf("----------------POR FAVOR ESCOJA UNA OPCION------------------- \n");
    		printf("-------------------------------------------------------------- \n");
    	};break;

    }

    return 0;
}

void toString(struct Cola* c) {
    struct Nodo* actual = c->ppio;
    while (actual) {
        printf("%s, %d, %.2f\n",
                actual->ptrObj->nombre, actual->ptrObj->prioridad, actual->ptrObj->velocidad);
        actual = actual->sig;
    }
    printf("------------------------------------------------------------\n");
}

struct Auto* deQueue(struct Cola* c) {
    struct Nodo* eliminar;
    struct Auto* aux = NULL;
    pthread_mutex_lock(&mutex);
    if (c->ppio) {
        aux = c->ppio->ptrObj;
        eliminar = c->ppio;
        c->ppio = c->ppio->sig;
    }
    pthread_mutex_unlock(&mutex);
    return aux;
}

void enQueue(struct Auto* a, struct Cola* c) {
    struct Nodo* nuevo = (struct Nodo*) malloc(sizeof (struct Nodo));
    nuevo->ptrObj = a;
    pthread_mutex_lock(&mutex);
    if (!c->ppio) {
        nuevo->sig = c->ppio;
        c->ppio = nuevo;
    } else {
        if (c->ppio->ptrObj->prioridad >= a->prioridad) {
            nuevo->sig = c->ppio;
            c->ppio = nuevo;
        } else {
            struct Nodo* actual = c->ppio;
            while (actual->sig) {
                if (actual->sig->ptrObj->prioridad >= a->prioridad)
                    break;
                actual = actual->sig;
            }
            nuevo->sig = actual->sig;
            actual->sig = nuevo;
        }
    }
    pthread_mutex_unlock(&mutex);
}

void* Puente_A_Terabithia(void* arg) {
    struct Auto* aux2 = (struct Auto*) malloc(sizeof (struct Auto*));
    //El auto que siempre va a salir de la cola
    double t;//El timpo de que va a tomar el auto en el puente
    int d;//Direccion del puente
    struct Cola* colitaE = &COLA_ESTE;
    struct Cola* colitaO = &COLA_OESTE;
    while (1) {
        d = PUENTE.direccion;
        if (d) {
            aux2 = deQueue(colitaE);
        } else {
            aux2 = deQueue(colitaO);
        }
        if (aux2) {
            fflush(stdout);
            if (aux2->direccion) {
                printf("Esta cruzando desde el Este: %s, %d, %2.f\n",
                        aux2->nombre, aux2->prioridad, aux2->velocidad);
            } else {
                printf("Esta cruzando desde el Oeste: %s, %d, %2.f\n",
                        aux2->nombre, aux2->prioridad, aux2->velocidad);
            }
            t = PUENTE.longitud / aux2->velocidad;
            sleep(t);
        } else {
            sleep(2);
        }
    }
}

void* creandoAutosOeste(void* arg) {
    int num_pth = OESTE.k_carros_e;//Cantidad de carros del este
    int cont = 0;
    double randCrear;
    double rand;
    pthread_mutex_lock(&mutex);
    struct Cola* colita = &COLA_OESTE;
    int k_amb = OESTE.k_amb;
    int pro_lle = OESTE.pro_lle;
    pthread_mutex_unlock(&mutex);
    for (int i = 0; i < num_pth; i++) {
        struct Auto * a = (struct Auto*) malloc(sizeof (struct Auto));
        if (cont == k_amb) {
            a->prioridad = 1;
            printf("Esta llegando una ambulancia desde el Oeste\n");
            a->nombre = "Ambulancia"; //se crea una ambulacia cuando se crean 12 autos
            cont = 0;
        } else {
            a->prioridad = 2;
            a->nombre = "Automoviles";
            cont++;
        }
        a->velocidad = (drand48() * 18 + 12);
        a->direccion = 0;
        enQueue(a, colita);
        rand = drand48() * 2.0; //numero random entre 0 y 1 para la formula de crear autos
        randCrear = -pro_lle * log(1 - rand);
        sleep(randCrear);
    }
}

void* creandoAutosEste(void* arg) {
    int num_pth = ESTE.k_carros_e;
    int cont = 0;
    double randCrear;
    double rand;
    struct Cola* colita = &COLA_ESTE;
    int k_amb = ESTE.k_amb;
    int pro_lle = ESTE.pro_lle;
    for (int i = 0; i < num_pth; i++) {
        struct Auto * a = (struct Auto*) malloc(sizeof (struct Auto));
        if (cont == k_amb) {
            a->prioridad = 1;
            fflush(stdout);
            printf("Esta llegando una ambulancia desde el Este\n");
            a->nombre = "Ambulancia";
            cont = 0;
        } else {
            a->prioridad = 2;
            a->nombre = "Automoviles";
            cont++;
        }
        a->velocidad = (drand48() * 4 + 4);
        a->direccion = 1;
        enQueue(a, colita);
        rand = drand48() * 2.0;
        randCrear = -pro_lle * log(1 - rand);
        sleep(randCrear);
    }
}

void La_Ladrona_de_Libros() {
    FILE *fichero;
    fichero = fopen("data/data.txt", "r");
    char textoExtraido[60];
    int number[15];
    if (fichero) {
        fgets(textoExtraido, 60, fichero);
        for (int i = 0; i < 15; i++)
            fscanf(fichero, "%d", &number[i]);
    }
    fclose(fichero);
    PUENTE.longitud = number[0]; //Longitud de puente
    PUENTE.direccion = 1;
    //Datos este
    ESTE.k_carros_e = number[1]; //Cantidad de carros
    ESTE.pro_lle = number[2]; //Promedio de llegada
    ESTE.tie_sem = number[3]; //Tiempo de Semaforo
    ESTE.vel_min = number[4]; //Velocidad Minima
    ESTE.vel_max = number[5]; //Velocidad Maxima
    ESTE.k_veh_x_pas = number[6]; //Cantidad de vehiculos por paso
    ESTE.k_amb = number[7]; //Prcentaje de ambulancias
    ESTE.k_amb = (ESTE.k_amb * 300) / 100;
    //Datos Oeste
    OESTE.k_carros_e = number[1]; //Cantidad de carros
    OESTE.pro_lle = number[2]; //Promedio de llegada
    OESTE.tie_sem = number[3]; //Tiempo de Semaforo
    OESTE.vel_min = number[4]; //Velocidad Minima
    OESTE.vel_max = number[5]; //Velocidad Maxima
    OESTE.k_veh_x_pas = number[6]; //Cantidad de vehiculos por paso
    OESTE.k_amb = number[7]; //Prcentaje de ambulancias
    OESTE.k_amb = (ESTE.k_amb * 288) / 100;

    COLA_OESTE.ppio = NULL;
    COLA_ESTE.ppio = NULL;
}

void* semaforo(void* arg) {
    int tie_semE = ESTE.pro_lle;
    int tie_semO = OESTE.pro_lle;
    while (1) {
        fflush(stdout);
        printf("Cambio de semaforo: Oeste -> Este\n");
        PUENTE.direccion = 0;
        sleep(tie_semO);
        fflush(stdout);
        printf("Cambio de semaforo: Este -> Oeste\n");
        PUENTE.direccion = 1;
        sleep(tie_semE);
    }
}
