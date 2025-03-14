# Projeto1_STR

# Sistema Simplificado de Controle de Elevador

Este repositório é referente ao Projeto 01 da disciplina Sistemas em Tempo Real, no período 2024.2, pela Universidade Federal de Campina Grande (UFCG).

---

## Equipe

- Jayne Emilly Silva de Melo (121210548)
- Victor Hugo Melquíades Klein (119110066)

---

## Descrição

Este projeto desenvolve um sistema de simulação de um elevador que opera com múltiplas threads em ambiente Linux, utilizando linguagem C e mecanismos de semáforos para gerenciar a concorrência. O foco é criar um sistema que organiza chamadas de um elevador em um prédio de cinco andares, otimizando a movimentação e evitando a colisão de dados entre as threads.


---

## Objetivos

1. Implementação de múltiplas threads operando simultaneamente no sistema operacional Linux.
2. Utilização de semáforos para a sincronização e controle de acesso aos recursos compartilhados.
3. Otimização de uma fila de chamadas para assegurar a resposta em tempo real às solicitações dos usuários.
4. Simulação da operacionalidade de um elevador conforme as chamadas registradas.
---

## Funcionalidades


1. Administração de Chamadas: O sistema permite que um número indefinido de usuários (threads) faça chamadas ao elevador de andares variados.
2. Controle de Fila: Utiliza semáforos para garantir que a fila de chamadas seja acessada de maneira segura e ordenada.
3. Dinâmica do Elevador: A thread do elevador responde às chamadas na ordem em que são recebidas, simulando a espera por embarque e desembarque de passageiros.
---

## Modelagem

O sistema é desenvolvido para operar no sistema operacional Linux, utilizando a linguagem C para implementação.
O sistema simula um cenário onde múltiplos usuários (representados por threads) podem solicitar o elevador em diferentes andares de um prédio de cinco andares. Essas solicitações são gerenciadas através de uma fila que organiza as chamadas em ordem de chegada (FIFO - First In, First Out). Semáforos são empregados para garantir que não haja conflitos entre as threads ao acessar a fila de chamadas, proporcionando um controle seguro e eficaz sobre as operações do elevador.


## Vídeo de apresentação
Link de acesso ao vídeo no YouTube: https://www.youtube.com/watch?v=AlNSlRAmw44 
