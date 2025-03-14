
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define NUM_ANDARES 5          // Número total de andares
#define TEMPO_MOVIMENTO 1      // Tempo que o elevador leva para se mover um andar (segundos)
#define TEMPO_EMBARQUE 2       // Tempo para o embarque e desembarque de usuários (segundos)

// Estrutura para armazenar chamadas de usuários
typedef struct {
    int id_usuario;  // ID único do usuário
    int origem;      // Andar de onde o usuário chama o elevador
    int destino;     // Andar para onde o usuário deseja ir
} Chamada;

// Definição da fila de chamadas como buffer circular
#define MAX_CHAMADAS 100  // Capacidade da fila de chamadas
Chamada fila_chamadas[MAX_CHAMADAS];
int write_index = 0, read_index = 0;  // Índices para leitura e escrita na fila

// Semáforos e mutex para controle de concorrência
sem_t sem_fila;   // Controla o acesso seguro à fila de chamadas
sem_t chamadas_pendentes; // Indica se há chamadas na fila
pthread_mutex_t fila_mutex = PTHREAD_MUTEX_INITIALIZER; // Mutex para garantir exclusão mútua na fila

// Estado do elevador
int andar_atual = 0;  // Posição atual do elevador
int usuario_id_global = 1;  // ID global para novos usuários

/**
 * Função: usuario
 * ----------------
 * Simula um usuário chamando o elevador. Escolhe aleatoriamente
 * um andar de origem e um destino diferentes, adicionando a chamada à fila.
 *
 * Argumentos:
 *    - arg: ponteiro para um inteiro representando o ID do usuário.
 */
void* usuario(void* arg) {
    int id = *((int*)arg);
    free(arg);

    // Escolhe um andar de origem aleatório
    int origem = rand() % NUM_ANDARES;
    int destino;
    do {
        destino = rand() % NUM_ANDARES;
    } while (origem == destino); // Garante que origem != destino

    // Adiciona a chamada na fila
    sem_wait(&sem_fila); // Aguarda permissão para acessar a fila
    pthread_mutex_lock(&fila_mutex); // Bloqueia a fila para evitar concorrência

    // Armazena a chamada na fila circular
    fila_chamadas[write_index].id_usuario = id;
    fila_chamadas[write_index].origem = origem;
    fila_chamadas[write_index].destino = destino;
    write_index = (write_index + 1) % MAX_CHAMADAS; // Incremento circular

    printf("[Usuário %d] Chamando o elevador do andar %d para o andar %d.\n", id, origem, destino);

    pthread_mutex_unlock(&fila_mutex); // Libera a fila
    sem_post(&chamadas_pendentes); // Sinaliza que há uma nova chamada pendente

    return NULL;
}

/**
 * Função: elevador
 * -----------------
 * Simula o funcionamento do elevador. O elevador aguarda chamadas e,
 * ao receber uma, se move até o andar do usuário, permite o embarque
 * e depois se move até o destino.
 *
 * Argumentos:
 *    - arg: não utilizado, mas necessário para as funções da biblioteca.
 */
void* elevador(void* arg) {
    printf("[Elevador] Inicializado no andar 0. Aguardando chamadas...\n");

    while (1) {
        sem_wait(&chamadas_pendentes); // Espera até que haja chamadas pendentes

        // Obtém a primeira chamada da fila circular
        Chamada chamada = fila_chamadas[read_index];
        read_index = (read_index + 1) % MAX_CHAMADAS;

        printf("[Elevador] Atendendo ao usuário %d: %d -> %d\n", chamada.id_usuario, chamada.origem, chamada.destino);

        // Movimenta o elevador até o andar de origem do usuário
        while (andar_atual != chamada.origem) {
            andar_atual += (andar_atual < chamada.origem) ? 1 : -1;
            printf("[Elevador] Movendo-se para o andar %d...\n", andar_atual);
            sleep(TEMPO_MOVIMENTO);
        }

        // Aguarda pelo embarque do usuário
        printf("[Elevador] Chegou ao andar %d. Usuário %d embarcou.\n", andar_atual, chamada.id_usuario);
        sleep(TEMPO_EMBARQUE);

        // Movimenta o elevador até o destino
        while (andar_atual != chamada.destino) {
            andar_atual += (andar_atual < chamada.destino) ? 1 : -1;
            printf("[Elevador] Movendo-se para o andar %d...\n", andar_atual);
            sleep(TEMPO_MOVIMENTO);
        }

        // Aguarda pelo desembarque do usuário
        printf("[Elevador] Chegou ao andar %d. Usuário %d desembarcou.\n", andar_atual, chamada.id_usuario);
        sleep(TEMPO_EMBARQUE);
    }

    return NULL;
}

/**
 * Função: gerador_usuarios
 * -------------------------
 * Cria continuamente novos usuários e os adiciona ao sistema,
 * simulando chamadas ao elevador em intervalos de tempo aleatórios.
 *
 * Argumentos:
 *    - arg: não utilizado, mas necessário para as funções da biblioteca.
 */
void* gerador_usuarios(void* arg) {
    while (1) {
        int* id = malloc(sizeof(int));  // Aloca memória para o ID do usuário
        *id = usuario_id_global++;
        pthread_t usuario_thread;
        pthread_create(&usuario_thread, NULL, usuario, id);
        pthread_detach(usuario_thread);  // Libera a thread automaticamente após execução

        // Tempo de criação ajustado para simular um fluxo contínuo
        sleep(rand() % 10 + 2 * (TEMPO_EMBARQUE + 2 * TEMPO_MOVIMENTO) - 5);
    }
    return NULL;
}

/**
 * Função: main
 * -------------
 * Inicializa o sistema do elevador, cria as threads necessárias
 * e mantém o programa executando indefinidamente.
 */
int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    // Inicializa os semáforos
    sem_init(&sem_fila, 0, MAX_CHAMADAS);
    sem_init(&chamadas_pendentes, 0, 0);

    // Cria a thread do elevador
    pthread_t elevador_thread;
    pthread_create(&elevador_thread, NULL, elevador, NULL);

    // Cria a thread que gera novos usuários continuamente
    pthread_t gerador_thread;
    pthread_create(&gerador_thread, NULL, gerador_usuarios, NULL);

    // Mantém o programa executando indefinidamente
    pthread_join(elevador_thread, NULL);
    pthread_join(gerador_thread, NULL);

    // Destroi os semáforos (não será executado pois o programa roda indefinidamente)
    sem_destroy(&sem_fila);
    sem_destroy(&chamadas_pendentes);

    return 0;
}

