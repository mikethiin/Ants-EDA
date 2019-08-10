#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME miki_v2


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }
  /**Definicion de funciones auxiliares**/
  typedef vector<vector<int>> Grafo;
  Grafo g; // 625 son las celdas del tablero y 4 las celdas de alrededor siendo [0]Up, [1]Down,[2]Left,[3]Right
  // Si g[85][3] = 0 significa que la celda [4][10] no tiene una celda de tierra a su derecha, 1 si sí la tiene.


 // Inicializa el Grafo
 void Inicializar(){
   g = Grafo(625,vector<int>(4,0)); // 625 son las celdas del tablero y 4 las celdas de alrededor siendo [0]Up, [1]Down,[2]Left,[3]Right
   // Si g[85][3] = 0 significa que la celda [4][10] no tiene una celda de tierra a su derecha, 1 si sí la tiene.
   for(int i = 0; i < 25; ++i){
     for(int j = 0; j < 25; ++j){
         if(i-1 >= 0 and cell(i-1,j).type == Soil) g[(i*25)+j][0] = 1;
         if(i+1 < 25 and cell(i+1,j).type == Soil) g[(i*25)+j][1] = 1;
         if(j-1 >= 0 and cell(i,j-1).type == Soil) g[(i*25)+j][2] = 1;
         if(j+1 < 25 and cell(i,j+1).type == Soil) g[(i*25)+j][3] = 1;
     }
   }
 }

 vector<Pos> posicion_comida(){
      vector <Pos> celdas_bonus;
      if(me() == 0){
            for(int i = 0; i < 12; ++i){
                for(int j = 0; j < 12; ++j){
                    Cell celda = cell(i,j);
                    if(celda.bonus != None) celdas_bonus.push_back(Pos(i,j));
                }
           }
     }else if( me() == 1){
            for(int i = 0; i < 12; ++i){
                for(int j = 12; j <= 24; ++j){
                    Cell celda = cell(i,j);
                    if(celda.bonus != None) celdas_bonus.push_back(Pos(i,j));
                }
            }
     }else if( me() == 2){
            for(int i = 12; i <= 24; ++i){
                for(int j = 12; j <= 24; ++j){
                    Cell celda = cell(i,j);
                    if(celda.bonus != None) celdas_bonus.push_back(Pos(i,j));
                }
           }
     }else if( me() == 3){
            for(int i = 12; i <= 24; ++i){
                for(int j = 0; j < 12; ++j){
                    Cell celda = cell(i,j);
                    if(celda.bonus != None) celdas_bonus.push_back(Pos(i,j));
                }
            }
     }
     return celdas_bonus;
}

//Retorna la distancia desde la hormiga hasta la comida
 int bfs(const Grafo &g,const Pos hormiga,const Pos comida){
   vector< vector<bool> > visitado(25,vector<bool>(25,false));
   vector <int> distancia(625,-1);
   visitado[hormiga.i][hormiga.j] = true;
   distancia[(hormiga.i*25)+hormiga.j] = 0;
   queue <Pos> q;
   q.push(hormiga);
   while(not q.empty()){
     Pos p = q.front();
     q.pop();
     for(int i=0; i < 4; ++i){
       if(g[(p.i*25)+p.j][i] == 1){
         Pos s(p.i,p.j);
         if(i == 0) s.i -= 1;
         else if(i == 1) s.i += 1;
         else if(i == 2) s.j -= 1;
         else if(i == 3) s.j += 1;
         if(visitado[s.i][s.j] == false){
           visitado[s.i][s.j] = true;
           distancia[(s.i*25)+s.j] = distancia[(p.i*25)+p.j] + 1;
           q.push(s);
        }
      }
     }
   }
   int camino = distancia[(comida.i*25)+comida.j];
   return camino;
 }
//Retorna la posicion de la hormiga enemiga mas cercana.
Pos hormiga_enemiga(const Grafo &g,const Pos hormiga){
  vector< vector<bool> > visitado(25,vector<bool>(25,false));
  visitado[hormiga.i][hormiga.j] = true;
  queue <Pos> q;
  q.push(hormiga);
  while(not q.empty()){
    Pos p = q.front();
    q.pop();
    for(int i=0; i < 4; ++i){
      if(g[(p.i*25)+p.j][i] == 1){
        Pos s(p.i,p.j);
        if(i == 0) s.i -= 1;
        else if(i == 1) s.i += 1;
        else if(i == 2) s.j -= 1;
        else if(i == 3) s.j += 1;
        if(visitado[s.i][s.j] == false){
          visitado[s.i][s.j] = true;
          q.push(s);
          if(cell(s).id != -1 and ant(cell(s).id).player != me() and ant(cell(s).id).type != Queen) return s;
       }
     }
    }
  }
}
//Devuelve true si la celda en 'dir' a 'pos' esta vacia o es del tablero, falso si no.
 bool es_posible(const Dir direccion,Pos pos_actual){
   if(direccion == Up and pos_actual.i - 1 >= 0){
     Cell celda = cell(pos_actual.i - 1, pos_actual.j);
     if(celda.type == Soil and celda.id == -1)return true;
  }else if(direccion == Down and pos_actual.i + 1 <=24){
     Cell celda = cell(pos_actual.i + 1, pos_actual.j);
     if(celda.type == Soil and celda.id == -1)return true;
  }else if(direccion == Right and pos_actual.j + 1 <= 24){
    Cell celda = cell(pos_actual.i, pos_actual.j + 1);
    if(celda.type == Soil and celda.id == -1)return true;
  }else if(direccion == Left and pos_actual.j - 1 >= 0){
     Cell celda = cell(pos_actual.i, pos_actual.j - 1);
   if(celda.type == Soil and celda.id == -1)return true;
   }
   return false;
 }

 //Devuelve true si la celda en 'dir' a 'pos' esta vacia o es del tablero, falso si no.
  bool posible_soldado(const Dir direccion,Pos pos_actual){
    if(direccion == Up and pos_actual.i - 1 >= 0){
      Cell celda = cell(pos_actual.i - 1, pos_actual.j);
      if(celda.type == Soil and (celda.id == -1 or ant(celda.id).player != me()))return true;
   }else if(direccion == Down and pos_actual.i + 1 <=24){
      Cell celda = cell(pos_actual.i + 1, pos_actual.j);
      if(celda.type == Soil and (celda.id == -1 or ant(celda.id).player != me()))return true;
   }else if(direccion == Right and pos_actual.j + 1 <= 24){
     Cell celda = cell(pos_actual.i, pos_actual.j + 1);
     if(celda.type == Soil and (celda.id == -1 or ant(celda.id).player != me()))return true;
   }else if(direccion == Left and pos_actual.j - 1 >= 0){
      Cell celda = cell(pos_actual.i, pos_actual.j - 1);
    if(celda.type == Soil and (celda.id == -1 or ant(celda.id).player != me()))return true;
    }
    return false;
  }
// Esta funcion pone las celdas que van a estar llenas en true.
  void celda_llena(const int i, const int j,vector<vector<bool>> &Tablero){
    Tablero[i][j] = true;
  }
// Esta funcion retorna true si la celda va a estar llena, falso si no.
  bool esta_llena(const int i, const int j, vector<vector<bool>> &Tablero){
    if(Tablero[i][j]) return true;
    return false;
  }
//Retorna true si puede poner un huevo de trabajdor, falso si no.
  bool huevo_trabajador(const int id){
    Ant reina = ant(id);
    for(unsigned int i = 0; i < reina.reserve.size(); ++i){
      if(reina.reserve[i] < 1) return false;
    }
    return true;
  }
  //Retorna 'true' si la hormiga 'id' esta al lado de la reina, falso si no lo esta.
  bool estan_lado(const int id,const int reina){
    Pos posicion = ant(id).pos;
    Pos queen = ant(reina).pos;
    if((queen.i + 1 == posicion.i and queen.j == posicion.j) or (queen.i - 1 == posicion.i and posicion.j == queen.j) or (queen.j + 1 == posicion.j and queen.i==posicion.i) or (queen.j - 1 == posicion.j and queen.i==posicion.i)){
      return true;
    }
    return false;
  }


  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    vector <int> soldados = soldiers(me());
    vector <int> trabajadores = workers(me());
    vector <int> reina = queens(me());
    vector <Pos> comida = posicion_comida();
    vector < vector<bool> > Tablero (25,vector<bool>(25));
    if(round() == 0) Inicializar();

    //------------------------------------------REINA(S)-------------------------------------------------------------------
        for(unsigned int i = 0; i < reina.size(); ++i){
          Ant queen = ant(reina[i]);
          if(huevo_trabajador(queen.id)){
            if(es_posible(Up,queen.pos)) lay(queen.id,Up,Worker);
            else if(es_posible(Down,queen.pos)) lay(queen.id,Down,Worker);
            else if(es_posible(Left,queen.pos)) lay(queen.id,Left,Worker);
            else if(es_posible(Right,queen.pos)) lay(queen.id,Right,Worker);
            else if(cell(queen.pos).bonus != None) take(queen.id);
          }else if(cell(queen.pos).bonus != None) take(queen.id);
          else if( round() % 2 == 0 ){
            Pos posicion = queen.pos;
            if(es_posible(Down,posicion) and cell(posicion.i + 1,posicion.j).bonus != None){
              move(queen.id,Down);
              celda_llena(posicion.i+1,posicion.j,Tablero);
            }else if(es_posible(Up,posicion) and cell(posicion.i - 1,posicion.j).bonus != None){
               move(queen.id,Up);
               celda_llena(posicion.i-1,posicion.j,Tablero);
            }else if(es_posible(Right,posicion) and cell(posicion.i,posicion.j + 1).bonus != None){
               move(queen.id,Right);
               celda_llena(posicion.i,posicion.j+1,Tablero);
            }else if(es_posible(Left,posicion) and cell(posicion.i,posicion.j - 1).bonus != None){
               move(queen.id,Left);
               celda_llena(posicion.i,posicion.j-1,Tablero);
            }
          }
        }
//----------------------------------------SOLDADOS-----------------------------------------------------------------
    for(unsigned int i = 0; i < soldados.size(); ++i){ //Movimiento aleatorio de los soldados
      Ant hormiga = ant(soldados[i]);
      Pos enemigo = hormiga_enemiga(g,hormiga.pos);
      int distancia = bfs(g,hormiga.pos,enemigo);
      if(hormiga.pos.i-1 > -1 and bfs(g,Pos(hormiga.pos.i-1,hormiga.pos.j),enemigo) < distancia and not esta_llena(hormiga.pos.i-1,hormiga.pos.j,Tablero) and posible_soldado(Up,hormiga.pos)){
          move(hormiga.id,Up);
          celda_llena(hormiga.pos.i-1,hormiga.pos.j,Tablero);
      }else if(hormiga.pos.i + 1 < 25 and bfs(g,Pos(hormiga.pos.i+1,hormiga.pos.j),enemigo) < distancia and not esta_llena(hormiga.pos.i+1,hormiga.pos.j,Tablero) and posible_soldado(Down,hormiga.pos)){
          move(hormiga.id,Down);
          celda_llena(hormiga.pos.i+1,hormiga.pos.j,Tablero);
      }else if(hormiga.pos.j-1 > -1 and bfs(g,Pos(hormiga.pos.i,hormiga.pos.j-1),enemigo) < distancia and not esta_llena(hormiga.pos.i,hormiga.pos.j-1,Tablero) and posible_soldado(Left,hormiga.pos)){
          move(hormiga.id,Left);
          celda_llena(hormiga.pos.i,hormiga.pos.j-1,Tablero);
      }else if(hormiga.pos.j+1 < 25 and bfs(g,Pos(hormiga.pos.i,hormiga.pos.j+1),enemigo) < distancia and not esta_llena(hormiga.pos.i,hormiga.pos.j+1,Tablero) and posible_soldado(Right,hormiga.pos)){
          move(hormiga.id,Right);
          celda_llena(hormiga.pos.i,hormiga.pos.j+1,Tablero);
      }
    }
//-------------------------------------------------TRABAJADORAS---------------------------------------------------------------

    for(unsigned int i = 0; i < trabajadores.size(); i++){ // ESTE BUCLE DICE A LAS HORMIGAS CON BONUS, QUE SI ESTAN AL LADO DE LA REINA QUE DEJEN LA COMIDA
      if(ant(trabajadores[i]).bonus != None){
        if(estan_lado(trabajadores[i],reina[0]) or ant(trabajadores[i]).life == 1) leave(trabajadores[i]);
      }
    }
    for(unsigned int i = 0; i < trabajadores.size(); i++){ // ESTE BUCLE DICE A TODAS LAS TRABAJADORAS SIN BONUS QUE COJAN BONUS
      Ant hormiga = ant(trabajadores[i]);
      if(hormiga.bonus == None and not estan_lado(hormiga.id,reina[0])){
        Pos posicion = hormiga.pos;
        Cell celda_actual = cell(posicion);
        if(celda_actual.bonus != None)take(hormiga.id);
      }
    }

    unsigned int h=0,i=0;
    while(i<comida.size() and h < trabajadores.size()){ // Este bucle manda a las hormigas a por comida
      while(h < trabajadores.size() and ant(trabajadores[h]).bonus != None) ++h;
      Ant hormiga = ant(trabajadores[h]);
      int distancia = bfs(g,hormiga.pos,comida[i]);
      if(hormiga.pos.i-1 > -1 and bfs(g,Pos(hormiga.pos.i-1,hormiga.pos.j),comida[i]) < distancia and not esta_llena(hormiga.pos.i-1,hormiga.pos.j,Tablero) and es_posible(Up,hormiga.pos)){
          move(hormiga.id,Up);
          celda_llena(hormiga.pos.i-1,hormiga.pos.j,Tablero);
      }else if(hormiga.pos.i + 1 < 25 and bfs(g,Pos(hormiga.pos.i+1,hormiga.pos.j),comida[i]) < distancia and not esta_llena(hormiga.pos.i+1,hormiga.pos.j,Tablero) and es_posible(Down,hormiga.pos)){
          move(hormiga.id,Down);
          celda_llena(hormiga.pos.i+1,hormiga.pos.j,Tablero);
      }else if(hormiga.pos.j-1 > -1 and bfs(g,Pos(hormiga.pos.i,hormiga.pos.j-1),comida[i]) < distancia and not esta_llena(hormiga.pos.i,hormiga.pos.j-1,Tablero) and es_posible(Left,hormiga.pos)){
          move(hormiga.id,Left);
          celda_llena(hormiga.pos.i,hormiga.pos.j-1,Tablero);
      }else if(hormiga.pos.j+1 < 25 and bfs(g,Pos(hormiga.pos.i,hormiga.pos.j+1),comida[i]) < distancia and not esta_llena(hormiga.pos.i,hormiga.pos.j+1,Tablero) and es_posible(Right,hormiga.pos)){
          move(hormiga.id,Right);
          celda_llena(hormiga.pos.i,hormiga.pos.j+1,Tablero);
      }
      ++i;
      ++h;
    }

    for(unsigned int i = 0; i < trabajadores.size(); ++i){ // ESTE BUCLE DA ORDENES A LAS HORMIGAS DE MOVERSE RANDOM.
      Ant hormiga = ant(trabajadores[i]);
      if(hormiga.bonus == None){
        int rand = random(0,100);
        if (rand >= 50){
          if (es_posible(Up,hormiga.pos) and not esta_llena(hormiga.pos.i-1,hormiga.pos.j,Tablero)){
            move(hormiga.id,Up);
            celda_llena(hormiga.pos.i-1,hormiga.pos.j,Tablero);
          }
          else if(es_posible(Down,hormiga.pos) and not esta_llena(hormiga.pos.i+1,hormiga.pos.j,Tablero)){
            move(hormiga.id,Down);
            celda_llena(hormiga.pos.i+1,hormiga.pos.j,Tablero);
          }
          else if(es_posible(Left,hormiga.pos) and not esta_llena(hormiga.pos.i,hormiga.pos.j-1,Tablero)){
            move(hormiga.id,Left);
            celda_llena(hormiga.pos.i,hormiga.pos.j-1,Tablero);
          }
          else if(es_posible(Right,hormiga.pos) and not esta_llena(hormiga.pos.i,hormiga.pos.j+1,Tablero)){
            move(hormiga.id,Right);
            celda_llena(hormiga.pos.i,hormiga.pos.j+1,Tablero);
          }
        }else{
          if (es_posible(Right,hormiga.pos) and not esta_llena(hormiga.pos.i,hormiga.pos.j+1,Tablero)){
            move(hormiga.id,Right);
            celda_llena(hormiga.pos.i,hormiga.pos.j+1,Tablero);
          }
          else if(es_posible(Left,hormiga.pos) and not esta_llena(hormiga.pos.i,hormiga.pos.j-1,Tablero)){
            move(hormiga.id,Left);
            celda_llena(hormiga.pos.i,hormiga.pos.j-1,Tablero);
          }
          else if(es_posible(Down,hormiga.pos) and not esta_llena(hormiga.pos.i+1,hormiga.pos.j,Tablero)){
            move(hormiga.id,Down);
            celda_llena(hormiga.pos.i+1,hormiga.pos.j,Tablero);
          }
          else if(es_posible(Up,hormiga.pos) and not esta_llena(hormiga.pos.i-1,hormiga.pos.j,Tablero)){
            move(hormiga.id,Up);
            celda_llena(hormiga.pos.i-1,hormiga.pos.j,Tablero);
          }
        }
      }else{
        //Aqui entran si llevan comida encima, por lo que deben ir a dejarla al lado de la reina
        Ant queen = ant(reina[0]);
        int distancia = bfs(g,hormiga.pos,queen.pos);
        if(hormiga.pos.i-1 > -1 and bfs(g,Pos(hormiga.pos.i-1,hormiga.pos.j),queen.pos) < distancia and not esta_llena(hormiga.pos.i-1,hormiga.pos.j,Tablero) and es_posible(Up,hormiga.pos)){
            move(hormiga.id,Up);
            celda_llena(hormiga.pos.i-1,hormiga.pos.j,Tablero);
        }else if(hormiga.pos.i + 1 < 25 and bfs(g,Pos(hormiga.pos.i+1,hormiga.pos.j),queen.pos) < distancia and not esta_llena(hormiga.pos.i+1,hormiga.pos.j,Tablero) and es_posible(Down,hormiga.pos)){
            move(hormiga.id,Down);
            celda_llena(hormiga.pos.i+1,hormiga.pos.j,Tablero);
        }else if(hormiga.pos.j-1 > -1 and bfs(g,Pos(hormiga.pos.i,hormiga.pos.j-1),queen.pos) < distancia and not esta_llena(hormiga.pos.i,hormiga.pos.j-1,Tablero) and es_posible(Left,hormiga.pos)){
            move(hormiga.id,Left);
            celda_llena(hormiga.pos.i,hormiga.pos.j-1,Tablero);
        }else if(hormiga.pos.j+1 < 25 and bfs(g,Pos(hormiga.pos.i,hormiga.pos.j+1),queen.pos) < distancia and not esta_llena(hormiga.pos.i,hormiga.pos.j+1,Tablero) and es_posible(Right,hormiga.pos)){
            move(hormiga.id,Right);
            celda_llena(hormiga.pos.i,hormiga.pos.j+1,Tablero);
        }
      }
    }
  }
};
/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
