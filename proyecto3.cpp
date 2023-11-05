/**
 * Proyecto 3 - fase 2
 * Temario 11: Tiempo de conteo de votos por departamento
 * Integrantes: - Francis Aguilar #22243
 *              - Angela Garcia #22869
 *              - Diego Garcia #22404         
 *              - Paula Barillas #22764
 * 
 * Implementar concepto de hilos paralelos, punteros, subrutinas y metodos de sincronizacion(join, mutez, variables de condicion, semaforos, barreras) 
 * 
 * **/

//librerias 
#include <stdio.h>
#include <stdlib.h>
#include <iostream>     //cout, cin, endl
#include <pthread.h>	//pthread_
#include <omp.h>
#include <ctime>
#include <semaphore.h>  //semaforo -> para manejar la sincronizacion y acceso a recursos

//esto es lo de allocate que es lo de new, para como asignar los cosos en la memoria
int *votos; 

using namespace std; //para no escribir en todo std::
clock_t start_t, end_t;

//----funciones----

//metodo tipo unsigned, porque de esta forma solo acepta 0 y numeros positivos 
unsigned int Votos(unsigned int NDepartamentos){    
    
    int VotoT=0;
    int VotoDt=0;
    votos = new int[NDepartamentos]; //Crear arreglo
    string departamentos[] = {"Alta Verapaz", "Baja Verapaz", "Chimaltenango", "Chiquimula", "El Progreso", "Escuintla", "Ciudad de Guatemala", "Huehuetenango", "Izabal",
        "Jalapa", "Jutiapa", "Peten", "Quetzaltenango", "Quiche", "Retalhuleu", "Sacatepequez", "San Marcos", "Santa Rosa", "Solola", "Suchitepequez", "Totonicapan", "Zacapa"
    };
    
    string *departamentos_dir; // se declara el puntero 
    departamentos_dir = &departamentos[0]; // el puntero apunta al primer elemento del array 
    
    //instancia del semaforo
    sem_t sem;
    //inicializa el semaforo, le asigna un nombre y le hace referencia a u direccion y se define el tipo de semaforo
    //sem_init(&semaf, tipo_sem, valor_inicial)
    sem_init(&sem, 0,1 );

    #pragma omp parallel for ordered
    for (int i=0; i <NDepartamentos; ++i){
        #pragma omp ordered
        std::cout << "Thread " << omp_get_thread_num() << " is processing iteration " << i << std::endl;
    
        start_t = clock(); //Se activa el contador de tiempo después de haber pedido los votos que hubo en cada departamento


        //seccion de bloquear el semaforo (sem_wait, decrementa el semaforo)
        sem_wait(&sem);
            {
                //pedir el numero de votos del departamento 
                //VotoDt
                cout<<"Ingrese el valor total de votos de "<< *departamentos_dir <<": ";
                departamentos_dir++;
                cin>>VotoDt;
                votos[i] = VotoDt;
                
            }
        //desbloquea el semaforo (sem_post, incrementa el semaforo)
        sem_post(&sem);
        
        }

    printf("\n---------------------------------------------------------------------------------------------------------------------------\n");

    end_t = clock(); //Se termina el conteo del tiempo
    for (int i = 0; i < NDepartamentos; ++i) {
        cout << "Se procesaron "<<  votos[i] << " en el departamento "<< departamentos[i] <<endl;    
    }

    printf("\n");
    #pragma omp parallel for reduction(+:VotoT)
    for (int i = 0; i < NDepartamentos; ++i) {
        VotoT += votos[i]; // Sumar todos los votos
    }

    //libera cualquier recurso asignado del semaforo
    sem_destroy(&sem);

    return VotoT;

}



//----main----
int main(){

	double total_t;

    int NumDepartamentos = 22;
    cout << Votos(NumDepartamentos) << " votos totales en el pais "<< endl;

    total_t = difftime(end_t,start_t)/CLOCKS_PER_SEC;
    cout << "Tiempo de trabajo: " << total_t << endl;
    delete[] votos;

    //mostrar el tiempo 
    return 0;

}
