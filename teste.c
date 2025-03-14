#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_ANDARES 5
#define TEMPO_MOVIMENTO 1
#define TEMPO_EMBARQUE 2

// Estrutura para armazenar chamadas dos usuários
typedef struct {
    int id_usuario;
    int origem;
    int destino;
} Chamada;

// Fila de chamadas e controle
//#define MAX_CHAMADAS 100  // Capacidade inicial da fila 
#define MAX_CHAMADAS 10  // Capacidade inicial da fila 
Chamada fila_chamadas[MAX_CHAMADAS];
int write_index = 0, read_index = 0;
sem_t sem_fila;   // Controla acesso à fila
sem_t chamadas_pendentes; // Indica chamadas na fila
pthread_mutex_t fila_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex para fila

// Elevador
int andar_atual = 0;
int usuario_id_global = 1;  // ID dinâmico para novos usuários

// Função do usuário (Thread)
void* usuario(void* arg) {
    int id = *((int*)arg);
    free(arg);

    int origem = rand() % NUM_ANDARES;
    int destino;
    do {
        destino = rand() % NUM_ANDARES;
    } while (origem == destino); // Garante que origem != destino

    // Adiciona a chamada na fila
    sem_wait(&sem_fila);
    pthread_mutex_lock(&fila_mutex);

    fila_chamadas[write_index].id_usuario = id;
    fila_chamadas[write_index].origem = origem;
    fila_chamadas[write_index].destino = destino;
    write_index = (write_index+1) % MAX_CHAMADAS;

    printf("[Usuário %d] Chamando o elevador do andar %d para o andar %d.\n", id, origem, destino);

    pthread_mutex_unlock(&fila_mutex);
    //sem_post(&sem_fila);
    sem_post(&chamadas_pendentes);

    return NULL;
}

// Função do elevador (Thread)
void* elevador(void* arg) {
    printf("[Elevador] Inicializado no andar 0. Aguardando chamadas...\n");

    while (1) {
        if (sem_trywait(&chamadas_pendentes)) {
            printf("[Elevador] Aguardando novas chamadas.\n");
            sem_wait(&chamadas_pendentes);
        }

        //sem_wait(&chamadas_pendentes); // Espera até que haja chamadas

        // Pega a primeira chamada da fila
        Chamada chamada = fila_chamadas[read_index];
        read_index = (read_index+1) % MAX_CHAMADAS;

        pthread_mutex_lock(&fila_mutex);

        printf("[Elevador] Atendendo ao usuário %d: %d -> %d\n", chamada.id_usuario, chamada.origem, chamada.destino);
        /*
        for (int i = 0; i < write_index - 1; i++) {
            fila_chamadas[i] = fila_chamadas[i + 1]; // Reorganiza fila
        }
        write_index--;
        */

        pthread_mutex_unlock(&fila_mutex);
        sem_post(&sem_fila);

        // Movimenta o elevador até a origem do usuário
        while (andar_atual != chamada.origem) {
            andar_atual += (andar_atual < chamada.origem) ? 1 : -1;
            printf("[Elevador] Movendo-se para o andar %d...\n", andar_atual);
            sleep(TEMPO_MOVIMENTO);
        }

        printf("[Elevador] Chegou ao andar %d. Usuário %d embarcou.\n", andar_atual, chamada.id_usuario);
        sleep(TEMPO_EMBARQUE);

        // Movimenta o elevador até o destino
        while (andar_atual != chamada.destino) {
            andar_atual += (andar_atual < chamada.destino) ? 1 : -1;
            printf("[Elevador] Movendo-se para o andar %d...\n", andar_atual);
            sleep(TEMPO_MOVIMENTO);
        }

        printf("[Elevador] Chegou ao andar %d. Usuário %d desembarcou.\n", andar_atual, chamada.id_usuario);
        sleep(TEMPO_EMBARQUE);
    }

    return NULL;
}

// Função para criar novos usuários continuamente
void* gerador_usuarios(void* arg) {
    while (1) {
        int* id = malloc(sizeof(int));
        *id = usuario_id_global++;
        pthread_t usuario_thread;
        pthread_create(&usuario_thread, NULL, usuario, id);
        pthread_detach(usuario_thread);  // Libera a thread automaticamente após execução

        //sleep(1);
        //sleep(rand() % 3 + 1);  // Novo usuário surge entre 1 e 3 segundos
        sleep(rand()%10 + 2*(TEMPO_EMBARQUE + 2*TEMPO_MOVIMENTO) - 5);  // Novo usuário surge em um tempo aproximado de atender outro
    }

    return NULL;
}

int main() {
    srand(time(NULL));

    // Inicializa os semáforos
    sem_init(&sem_fila, 0, MAX_CHAMADAS);
    sem_init(&chamadas_pendentes, 0, 0);

    // Cria a thread do elevador
    pthread_t elevador_thread;
    pthread_create(&elevador_thread, NULL, elevador, NULL);

    // Cria a thread que gera novos usuários continuamente
    pthread_t gerador_thread;
    pthread_create(&gerador_thread, NULL, gerador_usuarios, NULL);

    // Deixar rodando indefinidamente
    pthread_join(elevador_thread, NULL);
    pthread_join(gerador_thread, NULL);

    // Destroi os semáforos (não será executado pois o programa roda indefinidamente)
    sem_destroy(&sem_fila);
    sem_destroy(&chamadas_pendentes);

    return 0;
}
