#include <iostream>     // Para entrada e saída (cout, cerr)
#include <fstream>      // Para manipulação de arquivos (ifstream)
#include <sstream>      // Para manipulação de streams de string (istringstream)
#include <string>       // Para utilização da classe string
#include <vector>       // Para utilização do container vector
#include <deque>        // Para utilização do container deque (fila de processos no RR)
#include <utility>      // Para utilização do pair
#include <algorithm>    // Para funções de ordenação (sort, stable_sort) e min
#include <climits>      // Para a constante INT_MAX
#include <locale>       // Para manipulação de locale (ex.: pt_BR.UTF-8)

using namespace std;

// Função para calcular e imprimir as médias dos tempos de resposta, espera e retorno
void calcularMedias(string algoritmo, vector<int> &tempoResposta, vector<int> &tempoEspera, vector<int> &tempoRetorno) {

    float mediaResposta = 0, mediaEspera = 0, mediaRetorno = 0;     // Médias de Resposta, Espera e Retorno
    int divisor = tempoResposta.size();       // Número total de processos (usado como divisor para média)

    // Soma os tempos de resposta, espera e retorno de todos os processos
    for (int i = 0; i < divisor; i++) {      
        mediaResposta += tempoResposta[i];
        mediaEspera += tempoEspera[i];
        mediaRetorno += tempoRetorno[i];
    }

    // Calcula as médias dividindo pelo número total de processos
    mediaResposta = mediaResposta / divisor;
    mediaEspera = mediaEspera / divisor;
    mediaRetorno = mediaRetorno / divisor;

    // Imprime qual algoritmo foi utilizado e as médias calculadas
    if (algoritmo == "FCFS") cout << "FCFS: ";      // Printa o algoritmo e as médias
    if (algoritmo == "SJF") cout << "SJF: ";
    if (algoritmo == "RR") cout << "RR: ";

    // Exibe as médias com uma casa decimal
    printf("%.1f %.1f %.1f\n", mediaRetorno, mediaResposta, mediaEspera);
}

int main() {
    // Configuração de locale para pt_BR.UTF-8 (altera, por exemplo, o separador decimal para vírgula)
    std::locale::global(std::locale("pt_BR.UTF-8"));
    std::cout.imbue(std::locale());
    
    // Abertura do arquivo de processos
    ifstream arquivo("processos.txt");   // Abre o arquivo de processos

    if (!arquivo) { 
        cerr << "Erro ao abrir o arquivo!" << endl;
        return 1;
    }

    // Variáveis para armazenar os tempos de chegada e duração de cada processo
    int tCheg, tDurac, qtddProcessos = 0;
    vector<int> tempoChegada;   // Armazena o tempo de chegada dos processos
    vector<int> tempoDuracao;   // Armazena o tempo de duração (burst time) dos processos

    // Leitura do arquivo linha por linha
    string linha;
    while (getline(arquivo, linha)) { 
        // Cria um stream a partir da linha lida
        istringstream stream(linha); 
        
        // Tenta ler dois números (tempo de chegada e duração)
        if (stream >> tCheg >> tDurac) {        // Se conseguir ler 2 números, armazena na lista de processos
            tempoChegada.push_back(tCheg);      // Armazena o tempo de chegada do processo
            tempoDuracao.push_back(tDurac);       // Armazena o tempo de duração do processo
            qtddProcessos += 1;                   // Incrementa a quantidade de processos
        } else {
            cerr << "Erro ao processar a linha: " << linha << endl;
        }
    }

    // Variáveis para armazenar as métricas dos processos
    vector<int> tempoResposta;   // Tempo de resposta de cada processo
    vector<int> tempoEspera;     // Tempo de espera de cada processo
    vector<int> tempoRetorno;    // Tempo de retorno de cada processo


    // ===================== FCFS =====================
    {
        // FCFS: First-Come, First-Served
        vector<pair<int, int>> processos;  // Cada par contém {tempo de chegada, duração}
        int tempoAtual = 0; // Tempo atual do sistema

        // Mapeia os processos para o vetor 'processos'
        for (int i = 0; i < qtddProcessos; i++) {
            processos.push_back({tempoChegada[i], tempoDuracao[i]});
        }

        // Ordena os processos por tempo de chegada (stable_sort preserva a ordem de chegada em caso de empate)
        stable_sort(processos.begin(), processos.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });

        // Processa cada processo na ordem de chegada
        for (int i = 0; i < qtddProcessos; i++) {
            int chegada = processos[i].first;   // Tempo de chegada do processo atual
            int duracao = processos[i].second;    // Duração (tempo de execução) do processo atual

            // Se for o primeiro processo ou se o processo chegou depois do tempoAtual,
            // atualiza o tempoAtual para o tempo de chegada
            if (i == 0 || chegada > tempoAtual) {
                tempoAtual = chegada;
            }

            // Calcula o tempo final de execução do processo atual
            int tempoFinal = tempoAtual + duracao;
            // Calcula os tempos de resposta, espera e retorno e armazena nos respectivos vetores
            tempoResposta.push_back(tempoAtual - chegada);                 // Tempo de resposta
            tempoEspera.push_back(tempoFinal - chegada - duracao);           // Tempo de espera
            tempoRetorno.push_back(tempoFinal - chegada);                    // Tempo de retorno

            // Atualiza o tempoAtual para o fim da execução do processo atual
            tempoAtual = tempoFinal;
        }

        // Chama a função para calcular e exibir as médias das métricas para FCFS
        calcularMedias("FCFS", tempoResposta, tempoEspera, tempoRetorno);
        // Limpa os vetores de métricas para reutilizá-los em outros algoritmos
        tempoResposta.clear();
        tempoEspera.clear();
        tempoRetorno.clear();
    }
    
    // ===================== SJF =====================
    {
        // SJF: Shortest Job First (não preemptivo)
        vector<pair<int, int>> processos;   // Cada par: {tempo de chegada, duração}
        int tempoAtual = 0;                   // Tempo atual do sistema
        vector<bool> concluido(qtddProcessos, false);  // Vetor para marcar processos que já foram concluídos

        // Mapeia os processos
        for (int i = 0; i < qtddProcessos; i++) {
            processos.push_back({tempoChegada[i], tempoDuracao[i]});
        }

        int processosFinalizados = 0;  // Contador de processos finalizados
        // Loop até que todos os processos sejam concluídos
        while (processosFinalizados < qtddProcessos) {
            int indiceMenorDuracao = -1;  // Índice do processo com menor duração que já chegou
            int menorDuracao = INT_MAX;   // Valor inicial para a menor duração (usado para comparação)

            // Percorre todos os processos para encontrar o processo que está pronto (já chegou)
            // e que possui a menor duração
            for (int i = 0; i < qtddProcessos; i++) {
                if (!concluido[i] && processos[i].first <= tempoAtual) {
                    if (processos[i].second < menorDuracao) {
                        menorDuracao = processos[i].second;
                        indiceMenorDuracao = i;
                    }
                }
            }

            // Se nenhum processo estiver pronto, avança o tempo atual
            if (indiceMenorDuracao == -1) {
                tempoAtual++;
                continue;
            }

            // Recupera os dados do processo escolhido
            int chegada = processos[indiceMenorDuracao].first;
            int duracao = processos[indiceMenorDuracao].second;
            int tempoFinal = tempoAtual + duracao;  // Calcula o tempo de término do processo

            // Calcula os tempos e armazena nos vetores correspondentes
            tempoResposta.push_back(tempoAtual - chegada);   // Tempo de resposta
            tempoEspera.push_back(tempoAtual - chegada);       // Tempo de espera
            tempoRetorno.push_back(tempoFinal - chegada);      // Tempo de retorno

            // Marca o processo como concluído e atualiza o tempo e o contador de finalizados
            concluido[indiceMenorDuracao] = true;
            tempoAtual = tempoFinal;
            processosFinalizados++;
        }

        // Calcula e exibe as médias para SJF
        calcularMedias("SJF", tempoResposta, tempoEspera, tempoRetorno);
        // Limpa os vetores de métricas para reutilização
        tempoResposta.clear();
        tempoEspera.clear();
        tempoRetorno.clear();
    }

    // ===================== RR, q = 2 =====================
    {
        // Round Robin (RR) com quantum igual a 2 unidades
        vector<pair<int, int>> processos;         // Cada par: {tempo de chegada, duração}
        deque<int> filaProntos;                     // Fila (deque) com os índices dos processos prontos para execução
        vector<int> tempoRestante(qtddProcessos);     // Vetor que guarda o tempo restante de execução para cada processo
        vector<int> tempoFinal(qtddProcessos, -1);      // Vetor para registrar o tempo final de cada processo (-1 indica que ainda não terminou)
        vector<int> primeiraExecucao(qtddProcessos, -1); // Vetor para registrar o momento da primeira execução de cada processo
        int tempoAtual = 0;                         // Tempo atual do sistema
        int processosFinalizados = 0;               // Contador de processos finalizados

        // Inicializa os processos e o tempo restante de cada processo
        for (int i = 0; i < qtddProcessos; i++) {
            processos.push_back({tempoChegada[i], tempoDuracao[i]});
            tempoRestante[i] = tempoDuracao[i];
        }

        // Cria um vetor de índices para ordenar os processos por tempo de chegada
        vector<int> indices(qtddProcessos);
        for (int i = 0; i < qtddProcessos; i++) 
            indices[i] = i;
        // Ordena os índices com base no tempo de chegada dos processos
        sort(indices.begin(), indices.end(), [&](int a, int b) {
            return processos[a].first < processos[b].first;
        });

        // Insere na fila os processos que já chegaram (tempo de chegada <= tempoAtual)
        int idx = 0;
        while (idx < qtddProcessos && processos[indices[idx]].first <= tempoAtual) {
            filaProntos.push_back(indices[idx]);
            idx++;
        }

        // Loop principal do algoritmo Round Robin
        while (processosFinalizados < qtddProcessos) {
            if (!filaProntos.empty()) {
                // Pega o índice do processo no início da fila e remove-o da fila
                int atual = filaProntos.front();
                filaProntos.pop_front();

                // Se for a primeira vez que o processo é executado, registra o tempo de resposta
                if (primeiraExecucao[atual] == -1) {
                    primeiraExecucao[atual] = tempoAtual;
                }

                // Determina quanto tempo o processo vai executar nesta rodada:
                // o mínimo entre o quantum (2) e o tempo restante do processo
                int tempoExecucao = min(2, tempoRestante[atual]);
                // Atualiza o tempo restante para o processo
                tempoRestante[atual] -= tempoExecucao;
                // Avança o tempo atual de acordo com o tempo de execução
                tempoAtual += tempoExecucao;

                // Durante a execução, verifica se novos processos chegaram e os adiciona à fila
                while (idx < qtddProcessos && processos[indices[idx]].first <= tempoAtual) {
                    filaProntos.push_back(indices[idx]);
                    idx++;
                }

                // Se o processo ainda não terminou, o reposiciona no final da fila
                if (tempoRestante[atual] > 0) {
                    filaProntos.push_back(atual);
                } else {
                    // Se terminou, registra o tempo final de execução do processo
                    tempoFinal[atual] = tempoAtual;
                    processosFinalizados++;
                }
            } else {
                // Se a fila de prontos estiver vazia, significa que nenhum processo chegou
                // então avança o tempo até o próximo processo que ainda não chegou
                if (idx < qtddProcessos) {
                    tempoAtual = processos[indices[idx]].first;
                    while (idx < qtddProcessos && processos[indices[idx]].first <= tempoAtual) {
                        filaProntos.push_back(indices[idx]);
                        idx++;
                    }
                }
            }
        }

        // Calcula as métricas para cada processo:
        // Tempo de retorno = tempo final - tempo de chegada
        // Tempo de resposta = tempo da primeira execução - tempo de chegada
        // Tempo de espera = tempo de retorno - duração (burst time)
        for (int i = 0; i < qtddProcessos; i++) {
            tempoRetorno.push_back(tempoFinal[i] - processos[i].first);
            tempoResposta.push_back(primeiraExecucao[i] - processos[i].first);
            tempoEspera.push_back(tempoRetorno[i] - processos[i].second);
        }

        // Calcula e exibe as médias para Round Robin (RR)
        calcularMedias("RR", tempoResposta, tempoEspera, tempoRetorno);
    }
    
    return 0;
}
