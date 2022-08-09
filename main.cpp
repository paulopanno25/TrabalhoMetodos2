#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <memory.h>
#include <vector>
#include <algorithm>
#include <climits>
#include <sstream>
#include <string>


#include "estruturas.hpp"

#define MAX(X,Y) ((X > Y) ? X : Y)

using namespace std; 

using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::vector;
using std::fill_n; using std::istringstream;


int main(){
    int seed=0;
    double tempo_melhor, tempo_total, limite=300.00;
    
    lerArquivo();

    GerarVizinhosBT(QuantidadeDeVizinhancas, limite, vetorGeraldeInformacoes, tempo_melhor, tempo_total);

    return 0;

}

void lerArquivo(){

    string filename("j1201_4.sm");
    string word;
    vector<string> words;
    vector<string> words2;
    int i=0, temporario=0, temporario2=0, auxiliar=0, x=0,p=0;
    DadosDasTarefas DadosDasTarefas[MAX_TAREFAS]; 

    ifstream input_file("j1201_4.sm");
    
    if(!input_file.is_open()){
        cerr << "Não foi possivel abrir o arquivo- '" << filename << "'" << endl;
        

    }else{  
        input_file >> word;
        string space_delimiter = " ";
    
        while( !input_file.eof()){
            getline(input_file,word);
            size_t pos = 0;
            
            // Pega a quantidade de recursos e armazena
            if(i==8){
               
                while ((pos = word.find(space_delimiter)) != string::npos) { //Separa a string por ' '
                    words.push_back(word.substr(0, pos));
                    word.erase(0, pos + space_delimiter.length());
                }
               
                sscanf(words[22].data(), "%d", &QuantidadeDeRecursos); //converte string para inteiro

            }

            //Pega a quantidade de tarefas e armazena
            if(i==14){
               
                pos = 0;
                while ((pos = word.find(space_delimiter)) != string::npos) { //Separa a string por ' '
                    words2.push_back(word.substr(0, pos));
                    word.erase(0, pos + space_delimiter.length());
                }
               sscanf(words2[8].data(), "%d", &QuantidadeDeTarefas);
               if(QuantidadeDeTarefas==0){
                   sscanf(words2[9].data(), "%d", &QuantidadeDeTarefas);
               } //converte string para inteiro
            }
     
            //Pega a quantidade de sucessores e seus ids e armazena
            if(i>=18){
                
                if(i<=(18+(QuantidadeDeTarefas+1))){
                   
                    istringstream ss(word);
                   
                    x=0;
                    while( ss){
                    ss >> entrada[x];
                    x++;

                    }

                    DadosDasTarefas[temporario].Id = entrada[0];
                    DadosDasTarefas[temporario].QuantidadeDeSucessores = entrada[2];
                    p=3;
                    for(int z=0; z<DadosDasTarefas[temporario].QuantidadeDeSucessores; z++){
                        DadosDasTarefas[temporario].IdSucessores[z] = entrada[p];
                        p++;
                    }
                    temporario++;             
                }                                
        
            }

            //Pega a duração e a quantidade de recurso que cada tarefa consome
            if((i>=(18+(QuantidadeDeTarefas+6)))){
                if((i<=((18+(QuantidadeDeTarefas+6)+(QuantidadeDeTarefas+1))))){

                     istringstream ss(word);
                   
                    x=0;
                    while( ss){
                    ss >> entrada[x];
                    x++;

                    }

                    DadosDasTarefas[temporario2].DuracaoDaTarefa = entrada[2];
                    
                    p=3;
                    for(int z=0; z<QuantidadeDeRecursos; z++){
                        DadosDasTarefas[temporario2].QuantidadeDeRecursosConsumidos[z] = entrada[p];
                        p++;
                    }      
                    temporario2++;
                }   
            }
        
            //Pega Quanto de cada Recurso tem disponivel
            if((i==((18+(QuantidadeDeTarefas+6)+(QuantidadeDeTarefas+1))+4))){
            x=0; 
            istringstream ss(word);
            while( ss){
                ss >> QuantidadeDeCadaRecursoDisponivel[x];
                x++;
                }
            }
            
            i++; 

        }      
        input_file.close();
    }

    PreencherMatrizDeSucessores(DadosDasTarefas);
   
    OrdemDeExecucao(DadosDasTarefas);      
}

void PreencherMatrizDeSucessores(DadosDasTarefas infoTarefas[]){
    int i,j,aux;

    //Zera Matriz
    for (i = 0; i <  QuantidadeDeTarefas+2; i++){ //linha
        for (j = 0; j < QuantidadeDeTarefas+2; j++){ //coluna
            Matriz[i][j]=0;
        }
    }
    
    //Preenche Matriz
    for (i = 0; i <QuantidadeDeTarefas+2; i++){ //linha
        for (j = 0; j <QuantidadeDeTarefas+2; j++){ //coluna
           //percorre e encontra ids de sucessores que estejam na linha
           for(aux=0;aux<(infoTarefas[i].QuantidadeDeSucessores);aux++){
            if(infoTarefas[i].IdSucessores[aux]==(j+1)){
                Matriz[i][j]=1;
            }
          } 
        }
    }
}

void OrdemDeExecucao(DadosDasTarefas infoTarefas[]){
    int flag,aux,aux2=0,f,aux3;
    int MAXmakespan=0;
    
    //Zera vetor de ordems
    for(int j=0;j<(QuantidadeDeTarefas+2);j++){

        infoTarefas[j].Ordem = 0; 
        infoTarefas[j].IdDoAntecessor = 0;  
        vetIndObjOrd[j]=0; 
        
    }
    
    for(int i=0; i<(QuantidadeDeTarefas+1); i++){
        for(int j=0; j<(QuantidadeDeTarefas+2); j++){
            if(Matriz[i][j]!=0){
                
                if(i!=0){//função que verifica se existe alguma tarefa a ser realizada antes e qual a sua ordem.
                    for(aux=0;aux<(QuantidadeDeTarefas+1);aux++){
                        if(Matriz[aux][j]!=0){
                            
                           if(infoTarefas[aux].Ordem<infoTarefas[j].Ordem){
                                infoTarefas[j].Ordem;
                           }else{
                            infoTarefas[j].Ordem=infoTarefas[aux].Ordem+1;
                            infoTarefas[j].IdDoAntecessor = aux+1;
                           }
                            
                        }
                    }
                }else{
                    infoTarefas[j].Ordem=i;
                }
            }
        }
    }
   
    //Cria vetor de antecessores(Vetor de indices ordenados) e vetor de ids
    for(int j=0;j<(QuantidadeDeTarefas+2);j++){ 
            vetIndObjOrd[j] = infoTarefas[j].Ordem;
            vetIndObjOrd2[j] = infoTarefas[j].Id;

    }

    //Ordena o vetor de ids das tarefas por antecessores
    for(int prev=0; prev<(QuantidadeDeTarefas+2);prev++){
        for(int next = prev+1; next<(QuantidadeDeTarefas+2);next++){
            aux2 =vetIndObjOrd[prev];
            aux3 = vetIndObjOrd2[prev];
            if(vetIndObjOrd[next]<vetIndObjOrd[prev]){
                vetIndObjOrd[prev]=vetIndObjOrd[next];
                vetIndObjOrd2[prev]=vetIndObjOrd2[next];
                vetIndObjOrd[next]=aux2;
                vetIndObjOrd2[next]=aux3;
            }
        }
    }
    int qtd=0;
    QuantidadeDeVizinhancas=0;
    for(int q=0;q<QuantidadeDeTarefas+2;q++){
        for(int vordem=0;vordem<QuantidadeDeTarefas+2;vordem++){
            if(vetIndObjOrd[vordem] == q){
            qtd++;
            }
        }
        Tam_ordenacao[q] = qtd;
        if(qtd!=0){
            QuantidadeDeVizinhancas++;
        }
        qtd=0;
        //cout << "Qtd " << Tam_ordenacao[q]<<endl;
    }

   memset(Posicao_Vet_Tam_Ordenacao, 0, sizeof(Posicao_Vet_Tam_Ordenacao));
    int maux=0;
    for(int g=0;g<QuantidadeDeTarefas+2;g++){
        if(vetIndObjOrd[g]!=maux){
            Posicao_Vet_Tam_Ordenacao[maux+1]=g;
            maux++;
        }
        
    }

    for(int p=0; p < (QuantidadeDeTarefas+2);p++){
        MAXmakespan += infoTarefas[p].DuracaoDaTarefa;
    }
    MaiorMKSPpossivel = MAXmakespan;

   HeuConstAleGul(infoTarefas, 10, MAXmakespan);//10% de aleatoriedade

}

void HeuConstAleGul(DadosDasTarefas infoTarefas[], const int percentual, int MAXmakespan){
    int tam, pos, aux, PontoInicioTarefa=0, PontoInicioTarefa2=0;
    int vetAux[MAX_TAREFAS];
    int decrescedorDeTempo, decrescedorDeTempo1;
    TempoERecursos tempERecursos[MAXmakespan];
    int MatrizDeTempo[QuantidadeDeTarefas+2][MAXmakespan];
   
    memcpy(&vetAux, &vetIndObjOrd2, sizeof(vetIndObjOrd2));
   
    tam = MAX(1, (percentual / 100.0) * (QuantidadeDeTarefas+2));
   
    for(int j = 0; j < tam; j++)
    {
        pos = j + rand()%((QuantidadeDeTarefas+2) - j);
        aux = vetAux[pos];
        vetAux[pos] = vetAux[j];
        vetAux[j] = aux;
    }
    memset(vetDeExecucao, 0, sizeof(vetDeExecucao));
    memset(Contador, 0, sizeof(Contador));
    
    vetIndObjOrd[0] = -1;
    vetDeExecucao[0]= -1;
    int cont=0;
   
     for(int k=0;k<(QuantidadeDeTarefas+2);k++){
        for(int z=0;z<(MAXmakespan);z++){
            MatrizDeTempo[k][z] =0;
        }
    }

    for(int m=0;m<QuantidadeDeTarefas+2;m++){
        infoTarefas[m].TempoDeInicio = 0;
    }

    for(int z=0; z<(MAXmakespan);z++){
        for(int h=0; h<QuantidadeDeRecursos;h++){
            tempERecursos[z].RecursosUsadosNoTempo[h] = QuantidadeDeCadaRecursoDisponivel[h]; 
        }   
    } 

    for(int prev=1; prev<(QuantidadeDeTarefas+2);prev++){
        for(int next = prev+1; next<(QuantidadeDeTarefas+2);next++){
            int aux2[QuantidadeDeRecursos];

            if(((vetIndObjOrd[prev]) == 0 && vetIndObjOrd[vetAux[next]-1] == 0 ) && (vetIndObjOrd2[prev] != vetIndObjOrd2[vetAux[next]])
            && vetDeExecucao[infoTarefas[vetIndObjOrd2[prev]-1].IdDoAntecessor] != 1){
             
                cont=0;

                for(int i = 0; i < QuantidadeDeRecursos; i++){ 
                    if(infoTarefas[vetIndObjOrd2[prev]-1].QuantidadeDeRecursosConsumidos[i] + 
                       infoTarefas[vetIndObjOrd2[vetAux[next]-1]].QuantidadeDeRecursosConsumidos[i] <= tempERecursos[0].RecursosUsadosNoTempo[i]
                       && vetDeExecucao[vetIndObjOrd2[prev]-1] != 1 && vetIndObjOrd2[vetAux[next]-1] != 1){

                       cont+=1;
                        
                       }
                       
                }
                if(cont == QuantidadeDeRecursos){

                    vetDeExecucao[vetIndObjOrd2[prev]-1] = 1;
                    vetDeExecucao[vetIndObjOrd2[vetAux[next]-1]] = 1;
                    

                    decrescedorDeTempo = (infoTarefas[vetIndObjOrd2[prev]-1].DuracaoDaTarefa)-1;
                    decrescedorDeTempo1 = (infoTarefas[vetIndObjOrd2[vetAux[next]-1]].DuracaoDaTarefa)-1;

                    PontoInicioTarefa = 0;
                    
                    PontoInicioTarefa2 = 0;
                    
                    
                   for(int v=decrescedorDeTempo1; v>-1;v--){
                            MatrizDeTempo [(infoTarefas[vetIndObjOrd2[vetAux[next]-1]].Id)-1][PontoInicioTarefa2]=1;
                           PontoInicioTarefa2++;
                   }

                   for(int v=decrescedorDeTempo; v>-1;v--){
                            MatrizDeTempo [(infoTarefas[vetIndObjOrd2[prev]-1].Id)-1][PontoInicioTarefa]=1;
                           PontoInicioTarefa++;
                   }

                }
                    //Guarda a quantidade de recursos disponivel por tempo que começam em zero;
                    for(int z=0; z<(MAXmakespan);z++){
                        for(int h=0; h<QuantidadeDeRecursos;h++){
                            tempERecursos[z].RecursosUsadosNoTempo[h] = QuantidadeDeCadaRecursoDisponivel[h]; 
                        }
                        for(int k=0;k<(QuantidadeDeTarefas+2);k++){
                            if(MatrizDeTempo[k][z]==1){
                                
                                for(int g=0; g<QuantidadeDeRecursos;g++){
                                    tempERecursos[z].RecursosUsadosNoTempo[g] -= (infoTarefas[k].QuantidadeDeRecursosConsumidos[g]);
                                }
                            }
                        }
                    }
            }

            if(vetDeExecucao[infoTarefas[vetIndObjOrd2[prev]-1].IdDoAntecessor] == 1 || /*(vetDeExecucao[infoTarefas[vetIndObjOrd2[prev]-1].IdDoAntecessor] == -1 &&*/ 
            vetDeExecucao[infoTarefas[vetIndObjOrd2[prev]-1].Id-1 ] != 1){
                cont=0;
                

                if(vetDeExecucao[infoTarefas[vetIndObjOrd2[prev]-1].Id-1] == 0 ){
                    
                    
                    for(int y=0; y<MAXmakespan;y++){ 
                        cont=0;
                    for(int i = 0; i < QuantidadeDeRecursos; i++){
                            
                            if((tempERecursos[y].RecursosUsadosNoTempo[i] - infoTarefas[vetIndObjOrd2[prev]-1].QuantidadeDeRecursosConsumidos[i]) >= 0){
                        
                                cont+=1;
                                
                            }
                        }  

                        if(infoTarefas[vetIndObjOrd2[prev]-1].Ordem==0){
                            infoTarefas[vetIndObjOrd2[prev]-1].IdDoAntecessor=1;
                        }


                            if(cont == QuantidadeDeRecursos && y>(infoTarefas[(infoTarefas[vetIndObjOrd2[prev]-1].IdDoAntecessor)-1].TempoDeInicio +
                                                                    infoTarefas[(infoTarefas[vetIndObjOrd2[prev]-1].IdDoAntecessor)-1].DuracaoDaTarefa )-1){                                
                                
                                int verificaRecursos=0;
                                int inicio=y,incioVerificado=y;
                                int Auxp=0;
                                
                                for(int a=inicio; a<inicio+(infoTarefas[vetIndObjOrd2[prev]-1].DuracaoDaTarefa) ; a++){
                                    for (int w=0;w<(QuantidadeDeRecursos);w++){
                                        Auxp = tempERecursos[a].RecursosUsadosNoTempo[w];
                                        if(( Auxp -=  infoTarefas[vetIndObjOrd2[prev]-1].QuantidadeDeRecursosConsumidos[w])>=0){
                                                verificaRecursos+=1;
                                        }else{
                                            incioVerificado++;
                                        }
                                        
                                    }
                                }

                                if(verificaRecursos<infoTarefas[vetIndObjOrd2[prev]-1].DuracaoDaTarefa){
                                    y=incioVerificado-1;
                                }
                  
                                for(int a=y; a<y+(infoTarefas[vetIndObjOrd2[prev]-1].DuracaoDaTarefa) ; a++){
                                    MatrizDeTempo [(infoTarefas[vetIndObjOrd2[prev]-1].Id)-1][a] = 1;
                                    for (int w=0;w<(QuantidadeDeRecursos);w++){
                                        tempERecursos[a].RecursosUsadosNoTempo[w] -=  infoTarefas[vetIndObjOrd2[prev]-1].QuantidadeDeRecursosConsumidos[w];
                                    }
                                }
                                
                                vetDeExecucao[vetIndObjOrd2[prev]-1] = 1;
                                infoTarefas[vetIndObjOrd2[prev]-1].TempoDeInicio = y;
                                y=MAXmakespan;        
                        }
                    }
                }
            
            }
        }
    }

    memcpy(vetorGeraldeInformacoes, infoTarefas, sizeof(DadosDasTarefas)*MAX_TAREFAS);

}

void GerarVizinhosBT(const int tam_list, const double tempo_max, DadosDasTarefas infoTarefas[], double &tempo_melhor, double &tempo_total){

clock_t hI, hF;
    DadosDasTarefas s_vizinha[MAX_TAREFAS];
    int flag, pos=0, mocOri, foOri, melViz, melObj, melID, melPosicao, qtd_lista;
    int lista_tabu[QuantidadeDeTarefas+2];
    
    printf("\n\n>>> EXECUTANDO A BT...\n\n");
    hI = clock();
    HeuConstAleGul(infoTarefas ,10, MaiorMKSPpossivel);
    CalculoFo(infoTarefas);
    hF = clock();
    tempo_melhor = ((double)(hF - hI))/CLOCKS_PER_SEC;

    tempo_total = tempo_melhor;

    // Criar a lista tabu
    int lista_nova_ordenacao[QuantidadeDeTarefas+2];

    for(int i=0; i<QuantidadeDeTarefas+2;i++){
        lista_nova_ordenacao[i]=vetIndObjOrd2[i];
        lista_tabu[i]=vetIndObjOrd2[i];
    }

    foOri = FO;

   while(tempo_total < tempo_max){
    int x=0;
    for(int u=0;u<QuantidadeDeVizinhancas;u++){
        
        for(int i=0; i<QuantidadeDeTarefas+2;i++){
            lista_nova_ordenacao[i]=vetIndObjOrd2[i];
        }

        int Nova_FO = FO;
        x = Tam_ordenacao[u];
     

        if(Posicao_Vet_Tam_Ordenacao[u]==0){
            Posicao_Vet_Tam_Ordenacao[u]=1;
            Tam_ordenacao[u]=Tam_ordenacao[u]-1;
        }

        for(int vizin=Posicao_Vet_Tam_Ordenacao[u]; vizin < Posicao_Vet_Tam_Ordenacao[u]+Tam_ordenacao[u]-1; vizin++){
            
            int aux=vetIndObjOrd2[vizin];
            int aux2;
            for(int vizin2 = Posicao_Vet_Tam_Ordenacao[u];vizin2<Posicao_Vet_Tam_Ordenacao[u]+Tam_ordenacao[u]-1;vizin2++){
                
                aux=vetIndObjOrd2[vizin2+1];
                aux2 = vetIndObjOrd2[vizin2];
                vetIndObjOrd2[vizin2]=aux;
                vetIndObjOrd2[vizin2+1]=aux2;

                }


                HeuConstAleGul(infoTarefas ,10, MaiorMKSPpossivel);
                CalculoFo(infoTarefas);

                if(FO>Nova_FO){
                for(int y=0;y<QuantidadeDeTarefas+2;y++){
                    vetIndObjOrd2[y] = lista_nova_ordenacao[y];
                    } 
                    FO = Nova_FO;
                }else 
                if(Nova_FO>FO){
                    for(int y=0;y<QuantidadeDeTarefas+2;y++){
                    lista_tabu[y] = vetIndObjOrd2[y];
                }

            }

            HeuConstAleGul(infoTarefas ,10, MaiorMKSPpossivel);
            CalculoFo(infoTarefas);

            if(FO>Nova_FO){
               for(int y=0;y<QuantidadeDeTarefas+2;y++){
                vetIndObjOrd2[y] = lista_nova_ordenacao[y];
                } 
                FO = Nova_FO;
            }else if(Nova_FO>FO){
                for(int y=0;y<QuantidadeDeTarefas+2;y++){
                   lista_tabu[y] = vetIndObjOrd2[y];
                }
                hF = clock();
                tempo_melhor = ((double)(hF - hI))/CLOCKS_PER_SEC;
            }    
        }
    }
        for(int y=0;y<QuantidadeDeTarefas+2;y++){
            vetIndObjOrd2[y] = lista_tabu[y];
        }
    
        hF = clock();
        tempo_total = ((double)hF - hI)/CLOCKS_PER_SEC;   

    }

    EscreverSolucao(infoTarefas, FO);
    printf("FO: %d\tTempo: %.2f\n", FO, tempo_melhor);

}

void CalculoFo(DadosDasTarefas infoTarefas[]){
    int Max=0, aux=0, vio_pre=0,vio_rec=0;
    for(int i=0; i<(QuantidadeDeTarefas+2); i++){
        aux = infoTarefas[i].TempoDeInicio + infoTarefas[i].DuracaoDaTarefa;
        if(aux>Max){
            Max = aux;
        }
    }
    infoTarefas[QuantidadeDeTarefas+1].TempoDeInicio = Max;
    FO = Max;

    FO += (10*vio_pre);
    
}

void TempoDeExecucao(DadosDasTarefas infoTarefas[]){
    clock_t h;
    double tempo;
    const int repeticoes = 1000;
    h = clock();
    for(int r = 0; r < repeticoes; r++)
        HeuConstAleGul(infoTarefas, 10,10); // 10% de aleatoriedade
    h = clock() - h;
    tempo = (double)h/CLOCKS_PER_SEC;

   printf("Construtiva Aleatoria Gulosa...: %.5f seg.\n", tempo);
}

void EscreverSolucao(DadosDasTarefas infoTarefas[], int makespan){
    int i=0;
    fstream arquivo;
    arquivo.open("Resultados.txt", ios::app);

    arquivo << "FO: " << FO<< endl;
    arquivo << "Makespan: " << makespan << endl;
    arquivo << "------------------------------------\n" << endl;
    
    arquivo.close();
}


void LerDados(){
    string filename("j601_2.sol");
    string word;
    vector<string> words;
    int i=0, temporario=0,x=0;
    DadosDasTarefas solucao[MAX_TAREFAS];
    
    ifstream input_file("j601_2.sol");
    
    if(!input_file.is_open()){
        cerr << "Não foi possivel abrir o arquivo- '" << filename << "'" << endl;
        

    }else{  
        input_file >> word;
        string space_delimiter = " ";
    
        while( !input_file.eof()){
            getline(input_file,word);
            size_t pos = 0;
            // Pega a FO
            if(i==0){
                while ((pos = word.find(space_delimiter)) != string::npos) { //Separa a string por ' '
                    words.push_back(word.substr(0, pos));
                    word.erase(0, pos + space_delimiter.length());
                }
               
                sscanf(word.data(), "%d", &fo); //converte string para inteiro
                
            }
            
            // Pega a Makespan
            if(i==1){
                while ((pos = word.find(space_delimiter)) != string::npos) { //Separa a string por ' '
                    words.push_back(word.substr(0, pos));
                    word.erase(0, pos + space_delimiter.length());
                }
               
                sscanf(word.data(), "%d", &Mkspan); //converte string para inteiro
                
            }
     
            //Pega o id e o tempo de inicio
            if(i>=4){
                
                if(i<=(4+(QuantidadeDeTarefas+1))){
                   
                    istringstream ss(word);
                   
                    x=0;
                    while( ss){
                    ss >> entrada[x];
                    x++;

                    }

                    solucao[temporario].Id = entrada[0];
                    solucao[temporario].TempoDeInicio = entrada[1];
                                        
                    temporario++;

                   
                }                                
                    
                
            }
            
            i++; 

        }
            
        input_file.close();

    }

}
