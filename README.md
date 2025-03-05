# SO1-Projeto-01

Este repositório contém a implementação de algoritmos de escalonamento de processos em C++ para o projeto 01 da disciplina de Sistemas Operacionais (SO1). O programa lê um arquivo de entrada com dados de processos e aplica três algoritmos de escalonamento:

- **FCFS (First-Come, First-Served)**
- **SJF (Shortest Job First, não preemptivo)**
- **RR (Round Robin) com quantum igual a 2**

Para cada algoritmo, o programa calcula métricas importantes:
- **Tempo de Resposta**
- **Tempo de Espera**
- **Tempo de Retorno**

As médias dessas métricas são calculadas e exibidas para facilitar a análise do desempenho de cada algoritmo.

## Requisitos

- **C++17** (ou superior) – Necessário para suporte a recursos como structured bindings.
- Um compilador compatível (por exemplo, GCC ou Clang).
- Sistema operacional **Linux** (as instruções a seguir são para este ambiente).
- O arquivo `processos.txt`, que deve conter os dados dos processos (tempo de chegada e duração).

## Instalação

### 1. Clonando o Repositório

Abra o terminal e execute:

```bash
git clone https://github.com/VitorMSAraujo/SO1-Projeto-01.git
cd SO1-Projeto-01
```

### 2. Compilação

Utilize um compilador com suporte a C++17. Por exemplo, usando o GCC:

```bash
g++ -std=c++17 -o so1_projeto01 main.cpp
```

> **Nota:** Substitua `main.cpp` pelo nome do seu arquivo fonte, caso seja diferente.

### 3. Execução

Após a compilação, execute o programa:

```bash
./so1_projeto01
```

## Configuração da Locale pt_BR.UTF-8

O programa está configurado para utilizar a locale `pt_BR.UTF-8`, permitindo que números com partes decimais sejam exibidos com vírgula em vez de ponto.

### Caso a sua máquina Linux não possua essa locale, siga os passos abaixo:

#### Em distribuições baseadas em Debian/Ubuntu

1. **Abra o terminal.**
2. Execute o comando para gerar a locale:

   ```bash
   sudo locale-gen pt_BR.UTF-8
   ```

3. Atualize as configurações de locale:

   ```bash
   sudo update-locale
   ```

4. Verifique a instalação:

   ```bash
   locale -a | grep pt_BR.UTF-8
   ```

   Se o comando retornar `pt_BR.UTF-8`, a locale foi instalada corretamente.

#### Em distribuições baseadas em RedHat/Fedora

1. **Abra o terminal.**
2. Execute o comando para criar a locale:

   ```bash
   sudo localedef -i pt_BR -f UTF-8 pt_BR.UTF-8
   ```

3. Verifique a instalação:

   ```bash
   locale -a | grep pt_BR.UTF-8
   ```

Após instalar a locale, o programa utilizará as configurações regionais definidas (por exemplo, a vírgula como separador decimal).

## Estrutura do Projeto

- **main.cpp**: Código-fonte principal contendo a implementação dos algoritmos de escalonamento.
- **processos.txt**: Arquivo de entrada com os dados dos processos.
- **README.md**: Este arquivo, com explicações e instruções para o projeto.


