#include <iostream>
#include <fstream>
#include <vector>
#include "nodo.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

using namespace std;
vector<vector<NODO> > solucion_random(vector<vector<NODO> > aux, vector<NODO> pts);
vector<vector<NODO> > vecino_random(vector<vector<NODO> > aux);
vector<NODO> puntos;
int funcion_eval(vector<vector<NODO> > aux, vector<NODO> pts, int tiempo_maximo, int penalty);

int main(int argc, char const *argv[])
{   
    //Parámetros del algoritmo:
    int max_iter = 100;//cantidad de iteraciones antes del termino del algoritmo
    int Temp = 50; //temperatura inicial y actual.
    int Temp_min = 10;//temperatura minima a la que llegará
    int var_Temp = 3;//cuanto se reducira la temperatura cuando se deba.
    int max_iter_temp = 5; //cantidad de iteraciones que se consideran antes de modificar la temperatura.
    int penalty = 1000;//penalizacion.
    
    //Resto de variables.
    int iter_temp = 0;//contador de iteraciones de temp.
    int psc, psn, psbest; //puntaje de la solución actual y de la mejor solucion
    int iter = 0;//contador de iteraciones.

    char linea[128];
    int cant_nodos, cant_rutas, score;
    float tmax, x, y;//tiempo maximo, puntos x e y

    srand48(time(NULL));
    
    ifstream fe("input.txt"); //NOMBRE DE LA INSTANCIA A PROBAR.
    
    vector<vector <NODO> > sc; //[[ruta1],[ruta2]...]
    vector<vector <NODO> > sbest; //mantiene la mejor solución hasta ahora.
    vector<vector <NODO> > sn; //vecino random

    fe >> linea;
    fe >> cant_nodos;
    fe >> linea;
    fe >> cant_rutas;
    fe >> linea;
    fe >> tmax;
    
    int n_linea = 1;

    while(!fe.eof()){ //Lectura de datos y almecenamiento de estos en un vector.

        fe >> x;
        fe >> y;
        fe >> score;
        NODO n(x,y,score,n_linea);
        puntos.push_back(n);

        if (n_linea == 1){
            vector <NODO> aux;
            aux.push_back(n);
            for (int i=0; i<cant_rutas; i++){
                sc.push_back(aux);
            }
        }

        if (n_linea == cant_nodos){
            for(int i=0; i<cant_rutas; i++){
                sc[i].push_back(n);
            }
        }
        n_linea++;
    }

    sbest = sc;
    psc = psbest = funcion_eval(sc, puntos, tmax, penalty);
    
    //inicio del algoritmo.
    while(iter != max_iter){ 
        while(iter_temp != max_iter_temp){
            sn = vecino_random(sc);
            
            psn = funcion_eval(sn, puntos, tmax, penalty);
            
            if(psn > psc){ //si sn es mejor que sc, se cambia automaticamente
                sc = sn;
                psc = psn;
            }

            else{ //se verifica si la prob de aceptacion es mejor y se cambia de ser mejor.
                float r = drand48();
                float e = exp(float(psn - psc)/Temp);
                if(r < e){
                    sc = sn;
                    psc = psn;
                }
            }

            if(psc > psbest){ //si la actual es mejor que sbest se cambian.
                sbest = sc;
                psbest = psc;
            }
            
            iter_temp++; //se suma 1 a las interaciones antes de cambiar la temperatura
        }

        iter_temp = 0;//se reinician para la siguiente iteracion con la nueva temperatura.
        
        if(Temp > Temp_min){//se baja la temperatura si es que aun no llega al minimo.
            Temp -= var_Temp;
        }
        
        iter++;//se le suma 1 a la iteraciones totales del algoritmo.
    }

    cout << "Puntaje: " << psbest << endl; //puntaje

    float t_ruta = 0, posx = 0, posy = 0;

    for(int i=0; i<int(sbest.size()); i++){ //calcula el tiempo.
        for(int j=0; j<int(sbest[i].size()-1); j++){
            posx = pow(float(sbest[i][j+1].posicion[0] - sbest[i][j].posicion[0]),2);
            posy = pow(float(sbest[i][j+1].posicion[1] - sbest[i][j].posicion[1]),2);
            t_ruta = sqrt(posx + posy);
        }

        cout << t_ruta << " ";

        //imprimir los nodos visitados de la ruta.
        for(int j=0; j<int(sbest[i].size()); j++){
            cout << sbest[i][j].numero << " ";
        }

        cout << endl;
        t_ruta = 0;

    }

    fe.close();

    return 0;

}

int funcion_eval(vector<vector<NODO> > aux, vector<NODO> pts,  int tiempo_maximo, int penalty){//suma el puntaje de todos los puntos visitados dentro del vector de puntos totales.
    
    float t_ruta = 0, t_excedido = 0, posx = 0, posy = 0; //tiempo de la ruta i.
    int puntaje = 0;
    bool excedido = false;
    for(int i=0; i<int(aux.size()); i++){//por cada ruta
        for(int j=0; j<int(aux[i].size()-1); j++){//revisar que su tiempo no supere al maximo.
            posx = pow(aux[i][j+1].posicion[0] - aux[i][j].posicion[0],2);
            posy = pow(aux[i][j+1].posicion[1] - aux[i][j].posicion[1],2);
            t_ruta += sqrt(posx + posy);
        }

        if(t_ruta > tiempo_maximo){ //si el tiempo de la ruta supera al maximo el equipo es descalificado con puntaje 0.
            excedido = true;
            t_excedido += t_ruta - tiempo_maximo;
            break;
        }

        t_ruta = 0;
    }

    for(int i=0; i<int(aux.size()); i++){//se marcan los que se visitan
        for(int k=0; k<int(aux[i].size()); k++){
            for(int j=0; j<int(pts.size()); j++){
                if(aux[i][k].posicion[0]==pts[j].posicion[0] && aux[i][k].posicion[1]==pts[j].posicion[1]){
                    pts[j].visitado = 1;
                }
            }
        }
    }
    
    for(int i=1; i<int(pts.size()-1); i++){
        if(pts[i].visitado == 1){
            puntaje = puntaje + pts[i].score;
        }
    }

    if(excedido){
        puntaje = puntaje - (t_excedido*penalty);
    }

    return puntaje;
}

vector<vector<NODO> > vecino_random(vector<vector<NODO> > aux){//genera un vecino rando a travez de los moviemientos de insert, dell y swap.
    int ruta, nodo; //ruta random a modificar y pos del nodo a agregar o sacar segun dependa.
    int pto; //punto random que se agregara.
    int mov; //movimiento random: 0:insert, 1:sacar, 2:swap.
    int sw1, sw2;//nodos que se haran swap.

    ruta = rand()%aux.size();
    bool mov_valido = true;
    
    while(mov_valido){//busca que se pueda hacer el movimiento.
        mov = rand()%3;
        
        if(mov == 0 && aux[ruta].size() < puntos.size()){//insertar y hay espacio
            mov_valido = false;
        }

        else if(mov == 1 && aux[ruta].size() > 2){//sacar y que haya 1 para poder sacar (sin contar inicio y fin)
            mov_valido = false;
        }

        else if(mov == 2 && aux[ruta].size()>4){//swap y que hayan 2 para hacer swap(sin contar inicio y fin)
            mov_valido = false;
        }
        
    }

    if(mov == 0){ //insert
        pto = 1 + rand()%(puntos.size()-2);//punto random a agregar
        
        if(aux[ruta].size() == 2){//si tiene solo el inicio y final, se agrega entre ellos.
            aux[ruta].insert(aux[ruta].begin()+1,puntos[pto]);
        }

        else{//sino, se ve donde se agregará.
            bool flag = true;
            bool rep = false;
            
            while(flag){//buscar un nodo para agregar que no se repita.
                for(int i=0; i<int(aux[ruta].size()); i++){//se busca que no se repita, sino se elige otro.
                    if(aux[ruta][i].posicion[0] == puntos[pto].posicion[0] && aux[ruta][i].posicion[1] == puntos[pto].posicion[1]){
                        rep = true;
                        break;
                    }
                }
                if(rep){
                    pto = 1 + rand()%(puntos.size()-2);
                }
                else{
                    flag = false;
                }

                rep = false;
            }
            
            nodo = 1 + rand()%(aux[ruta].size()-2);//pos aleatorea donde agregar.
            aux[ruta].insert(aux[ruta].begin()+nodo,puntos[pto]);
        }
    }

    else if(mov == 1){ //sacar
        nodo = 1 + rand()%(aux[ruta].size()-2);
        aux[ruta].erase(aux[ruta].begin()+nodo);//se elimina el nodo en la pos dada.
    }

    else{ //swap
        sw1 = 1 + rand()%(aux[ruta].size()-2);//primer nodo a intercambiar
        bool flag = true;
        while(flag){
            sw2 = 1 + rand()%(aux[ruta].size()-2);//segundo nodo a intercambiar
            if(sw1 != sw2){
                flag = false;
            }
        }

        NODO temporal(aux[ruta][sw1].posicion[0], aux[ruta][sw1].posicion[1], aux[ruta][sw1].score, aux[ruta][sw1].numero);
        temporal.visitado = aux[ruta][sw1].visitado;
        aux[ruta][sw1] = aux[ruta][sw2];
        aux[ruta][sw2] = temporal;
    }

    return aux;
}