#include<iostream>
#include<stdlib.h>
#include<time.h>
#include <thread>
#include <chrono>
#include <math.h>
#include <vector>
#include <algorithm>


using namespace std;

const int M = 10;

struct Processo {
    int id;
    int tempo_processador;
    int prioridade;
    Processo& operator=(const Processo& other);
};

Processo& Processo::operator=(const Processo& other) {
    if (this != &other) {
        id = other.id;
        tempo_processador = other.tempo_processador;
        prioridade = other.prioridade;
    }
    return *this;
}

struct Node {
    Processo Proc;
    Node* next;
    Node& operator=(const Node& other);
};

Node& Node::operator=(const Node& other) {
    if (this != &other) {
        Proc = other.Proc;
    }
    return *this;
}

class Queue {
private:
    Node* head;
    Node* tail;
    int n;

public:
    Queue() {
        this->head = NULL;
        this->tail = NULL;
        n = 0;
    }
    ~Queue() {

    }
    Queue& operator=(const Queue& other);
    void enqueue(Processo PProc);
    Processo dequeue();
    bool isEmpty();
    int size();
    Processo first();
};

Queue& Queue::operator=(const Queue& other) {
    if (this != &other) {
        while (!isEmpty()) {
            dequeue();
        }

        Node* current = other.head;
        while (current != nullptr) {
            enqueue(current->Proc);
            current = current->next;
        }
    }
    return *this;
}

void atribui_random(Processo Procs[]);

Processo Queue::first() {
    return this->head->Proc;
}

Processo Queue::dequeue() {
    Processo r = this->head->Proc;
    if (size() == 0) {
        cerr << "Fila vazia." << endl;
    }
    else if (size() == 1) {
        delete this->head;
        this->head = NULL;
        this->tail = NULL;
    }
    else {
        Node* t = this->head;
        this->head = this->head->next;
        delete t;
    }
    if (size() > 0)
        n--;
    return r;
}

int Queue::size() {
    return this->n;
}

void Queue::enqueue(Processo PProc) {
    Node* t = new Node;
    t->Proc = PProc;
    t->next = NULL;

    if (isEmpty() == true) {
        this->head = t;
        this->tail = t;
    }
    else {
        this->tail->next = t;
        this->tail = t;
    }
    n++;
}

bool Queue::isEmpty() {
    if (n != 0)
        return false;
    return true;
}

void cadastra_processo_manual(Queue& Fila) {
    Processo Proc;
    cout << "----------- Cadastro de Processos -----------" << endl
        << " | Insira o ID do processo: [de 1 a 1111]" << endl;
    int id_processo;
    cin >> id_processo;
    while (id_processo < 0 || id_processo > 1111) {
        cerr << " | ID invalido. Digite novamente: " << endl;
        cin >> id_processo;
    }
    cout << " | Insira o tempo de processador [de 1 a 10]: " << endl;
    int tempo_processador;
    cin >> tempo_processador;
    while (tempo_processador < 1 || tempo_processador > 10) {
        cerr << " | Tempo invalido. Digite novamente: " << endl;
        cin >> tempo_processador;
    }
    cout << " | Insira a prioridade: [de 1 a 6]" << endl;
    int prioridade_processo;
    cin >> prioridade_processo;
    while (prioridade_processo < 1 || prioridade_processo > 6) {
        cerr << " | Prioridade invalida. Digite novamente: " << endl;
        cin >> prioridade_processo;
    }
    Proc.id = id_processo;
    Proc.prioridade = prioridade_processo;
    Proc.tempo_processador = tempo_processador;
    Fila.enqueue(Proc);
}

void cadastra_fifo_auto(Queue& Fila, int qtdProcessosAuto) {

    Processo Procs[M];
    atribui_random(Procs);
    for (int i = 0; i < qtdProcessosAuto; i++) {
        Fila.enqueue(Procs[i]);
    }
}

void executa_fifo(Queue& Fila, bool cadastroAuto = false) {
    bool primeiraExecucao = false;
    while (!Fila.isEmpty()) {
        if (!primeiraExecucao) {
            cout << "----------- FIFO ----------- " << endl;
            primeiraExecucao = true;
        }

        Processo Atual = Fila.dequeue();
        cout << "> Processo em execucao " << endl;
        cout << "\tID: " << Atual.id << endl
            << "\tTempo de Processador: " << Atual.tempo_processador << endl
            << "\tPrioridade: " << Atual.prioridade << endl << endl;
        this_thread::sleep_for(chrono::milliseconds(Atual.tempo_processador)); // 1k = 1s
    }
    cout << endl << endl;
}

void prioridade() {
    Processo Procs[M];
    for (int i = 0; i < M; i++) {
        Procs[i].id = i;
        Procs[i].tempo_processador = i;
        Procs[i].prioridade = i;
    }
    cout << "Preparando para executar" << endl;
    int prioridade_base = 0;
    int maior_indice = 0;
    for (int i = 0; i < M; i++) {
    }
}

void executa_sjf(Queue FilaCopy, bool cadastroAuto = false) {
    bool primeiraExecucao = false;

    std::vector<Processo> processos;

    while (!FilaCopy.isEmpty()) {
        processos.push_back(FilaCopy.dequeue());
    }

    std::sort(processos.begin(), processos.end(), [](const Processo& a, const Processo& b) {
        return a.tempo_processador < b.tempo_processador;
        });

    for (const Processo& atual : processos) {
        if (!primeiraExecucao) {
            cout << "----------- SJF ----------- " << endl;
            primeiraExecucao = true;
        }

        cout << "> Processo em execucao " << endl;
        cout << "\tID: " << atual.id << endl
            << "\tTempo de Processador: " << atual.tempo_processador << endl
            << "\tPrioridade: " << atual.prioridade << endl << endl;
        this_thread::sleep_for(chrono::milliseconds(atual.tempo_processador));
    }

    cout << endl << endl;
}


void escalona(Queue& FilaFIFO, Queue& FilaSJF, bool cadastroAuto = false) {
    executa_fifo(FilaFIFO, cadastroAuto);
    executa_sjf(FilaSJF, cadastroAuto);
}

void menu() {
    int opc;
    int opcCadastro;
    bool cadastroAuto = false;
    int qtdProcessosAuto = 0;
    int total_de_processos = 0;
    int qtdExecMain = 0;
    Queue FilaFIFO;
    Queue FilaSJF;
    do {
        qtdExecMain++;
        if (qtdExecMain == 3) {
            return;
        }

        cout << "   Menu" << endl
            << "1 | Cadastrar Processos" << endl
            << "2 | Escalonar Processos" << endl;
        cin >> opc;
        switch (opc) {
        case 1:
            cout << " | [1] Cadastrar manualmente\t[2] Cadastrar automaticamente" << endl;
            cin >> opcCadastro;
            switch (opcCadastro) {
            case 1:
                cadastra_processo_manual(FilaFIFO);
                total_de_processos++;
                break;
            case 2:
                cadastroAuto = true;
                cout << " | Quantos processos deseja cadastrar?" << endl;
                cin >> qtdProcessosAuto;
                while (qtdProcessosAuto < 1 || qtdProcessosAuto > M || ((total_de_processos + qtdProcessosAuto) >= M)) {
                    if (qtdProcessosAuto >= M) {
                        cerr << " | Quantidade maxima de processos atingida" << endl;
                        break;
                    }
                    cerr << " | Quantidade invalida. Digite novamente" << endl;
                    cin >> qtdProcessosAuto;
                }
                total_de_processos += qtdProcessosAuto;
                cadastra_fifo_auto(FilaFIFO, qtdProcessosAuto);
                cadastra_fifo_auto(FilaSJF, qtdProcessosAuto);
                break;
            default:
                cerr << "Opcao Invalida!" << endl;
            }
            break;
        case 2:
            escalona(FilaFIFO, FilaSJF, cadastroAuto);
            total_de_processos = 0;
            break;
        }
    } while (opc != 0);
}

void atribui_random(Processo Procs[]) {
    srand((unsigned)time(NULL));
    const int id_valor = 1111;
    const int prioridade_valor = 6;
    const int tempo_valor = 10;
    for (int i = 0; i < M; i++) {
        int id_random = 0 + (rand() % id_valor);
        int prioridade_random = 0 + (rand() % prioridade_valor);
        int tempo_random = 0 + (rand() % tempo_valor) * 100;

        if (id_random == 0)
            id_random++;
        if (prioridade_random == 0)
            prioridade_random++;
        if (tempo_random == 0)
            tempo_random++;

        Procs[i].id = id_random;
        Procs[i].prioridade = prioridade_random;
        Procs[i].tempo_processador = tempo_random;
    }
} 

int main()
{
    menu();

    return 0;
}