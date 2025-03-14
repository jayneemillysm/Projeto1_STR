# Projeto1_STR

# Sistema Simplificado de Controle de Elevador

Este repositório é referente ao Projeto 01 da disciplina Sistemas em Tempo Real, no período 2024.2, pela Universidade Federal de Campina Grande (UFCG).

---

## Equipe

- Jayne Emilly Silva de Melo (121210548)
- Victor Hugo Melquíades Klein (119110066)

---

## Descrição

O projeto consiste no desenvolvimento de um simulador de elevador que opera em um ambiente de programação concorrente, usando a linguagem C e semáforos no sistema operacional Linux. O sistema simula um único elevador atendendo múltiplos usuários de forma simultânea em um edifício de cinco andares. O principal objetivo é organizar e gerenciar as chamadas dos usuários para o elevador, assegurando que não ocorram conflitos entre as threads, e que todas as requisições sejam atendidas em ordem.

---

## Objetivos


1. Implementar threads concorrentes no Linux.
2. Utilizar semáforos para controle de acesso e sincronização.
3. Gerenciar uma fila de chamadas de maneira eficiente.
4. Simular a movimentação do elevador baseada em requisições dos usuários.

---

## Funcionalidades


1. Gestão de Chamadas: Usuários (threads) podem fazer chamadas para o elevador de andares aleatórios, e estas chamadas são armazenadas em uma fila compartilhada.

2. Controle Concorrente: O acesso à fila de chamadas é protegido por semáforos, garantindo que múltiplos usuários não modifiquem os dados simultaneamente.

3. Atendimento de Chamadas: O elevador, operando como outra thread, verifica a fila e atende às chamadas na ordem de chegada, seguindo a política FIFO (Primeiro a Entrar, Primeiro a Sair).

4. Simulação de Embarque/Desembarque: Quando o elevador chega ao andar solicitado, ele pausa por alguns segundos para simular o embarque e o desembarque dos usuários.

5. Operação Contínua: O sistema foi projetado para continuar executando e atendendo novas chamadas indefinidamente, sempre respondendo às novas solicitações dos usuários que chegam.

---

## Modelagem

O sistema é desenvolvido para operar no sistema operacional Linux, utilizando a linguagem C para implementação.


## Vídeo de apresentação
Link de acesso ao vídeo no YouTube:
