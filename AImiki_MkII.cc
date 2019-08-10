#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME AntMan_v1


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
// Retorna un vector con la posicion de la comida dentro del cuadrante del jugador
 vector<Pos> posicion_comida(const int id){
      vector <Pos> celdas_bonus;
      if(me() == 0){
            for(int i = 0; i < 13; ++i){
                for(int j = 0; j < 13; ++j){
                    Cell celda = cell(i,j);
                    if(celda.bonus != None and not estan_lado(Pos(i,j),id)) celdas_bonus.push_back(Pos(i,j));
                }
           }
     }else if( me() == 1){
            for(int i = 0; i < 13; ++i){
                for(int j = 24; j >= 12; --j){
                    Cell celda = cell(i,j);
                    if(celda.bonus != None and not estan_lado(Pos(i,j),id)) celdas_bonus.push_back(Pos(i,j));
                }
            }
     }else if( me() == 2){
            for(int i = 24; i >= 12; --i){
                for(int j = 24; j >= 12; --j){
                    Cell celda = cell(i,j);
                    if(celda.bonus != None and not estan_lado(Pos(i,j),id)) celdas_bonus.push_back(Pos(i,j));
                }
           }
     }else if( me() == 3){
            for(int i = 24; i >= 12; --i){
                for(int j = 0; j < 13; ++j){
                    Cell celda = cell(i,j);
                    if(celda.bonus != None and not estan_lado(Pos(i,j),id)) celdas_bonus.push_back(Pos(i,j));
                }
            }
     }
     return celdas_bonus;
}
// Retorna la posicion de la comida de todo el tablero
vector<Pos> posicion_comida_tablero(const int id){
  vector <Pos> celdas_bonus;
  if(me() == 0){
        celdas_bonus = posicion_comida(id);
        for(int i = 0; i < 25; ++i){
            for(int j = 0; j < 25; ++j){
                Cell celda = cell(i,j);
                if(celda.bonus != None and not estan_lado(Pos(i,j),id) and not puedo_ir(Pos(i,j))) celdas_bonus.push_back(Pos(i,j));
            }
       }
 }else if( me() == 1){
        celdas_bonus = posicion_comida(id);
        for(int i = 0; i < 25; ++i){
            for(int j = 24; j >= 0; --j){
                Cell celda = cell(i,j);
                if(celda.bonus != None and not estan_lado(Pos(i,j),id)  and not puedo_ir(Pos(i,j))) celdas_bonus.push_back(Pos(i,j));
            }
        }
 }else if( me() == 2){
        celdas_bonus = posicion_comida(id);
        for(int i = 24; i >= 0; --i){
            for(int j = 24; j >= 0; --j){
                Cell celda = cell(i,j);
                if(celda.bonus != None and not estan_lado(Pos(i,j),id)  and not puedo_ir(Pos(i,j))) celdas_bonus.push_back(Pos(i,j));
            }
       }
 }else if( me() == 3){
        celdas_bonus = posicion_comida(id);
        for(int i = 24; i >= 0; --i){
            for(int j = 0; j < 25; ++j){
                Cell celda = cell(i,j);
                if(celda.bonus != None and not estan_lado(Pos(i,j),id) and not puedo_ir(Pos(i,j))) celdas_bonus.push_back(Pos(i,j));
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
           //if(distancia[(comida.i*25)+comida.j] != 0) return distancia[(comida.i*25)+comida.j];
           q.push(s);
        }
      }
     }
   }
   int camino = distancia[(comida.i*25)+comida.j];
   return camino;
 }
//Retorna la posicion de la hormiga trabajadora enemiga mas cercana.
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
          if(cell(s).id != -1 and ant(cell(s).id).player != me() and ant(cell(s).id).type == Worker) return s;
       }
     }
    }
  }
  return hormiga;
}
//Retorna la posicion de la hormiga no reina enemiga mas cercana.
Pos hormiga_soldado(const Grafo &g,const Pos hormiga){
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
          if(round() < 10 and cell(s).id != -1 and ant(cell(s).id).player != me() and ant(cell(s).id).type != Queen) return s;
          else if(cell(s).id != -1 and ant(cell(s).id).player != me() and ant(cell(s).id).type != Queen and puedo_ir(s)) return s;
       }
     }
    }
  }
  return hormiga;
}
// Retorna la posicion de la celda con bonus mas cercana dentro del cuadrante.
Pos busca_comida(const Grafo &g,const Pos hormiga){
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
          if(cell(s).bonus != None and puedo_ir(s)) return s;
       }
     }
    }
  }
  return hormiga; // Solo pasara si no hay comida en el tablero, solo esta para que no me salga el warning
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
 //Devuelve true si la celda en 'dir' a 'pos' esta vacia,contiene un enemigo o es del tablero, falso si no.
  bool posible_soldado(const Dir direccion,Pos pos_actual){
    if(direccion == Up and pos_actual.i - 1 >= 0){
      Cell celda = cell(pos_actual.i - 1, pos_actual.j);
      if(celda.type == Soil and (celda.id == -1 or (ant(celda.id).player != me() and ant(celda.id).type == Worker)))return true;
   }else if(direccion == Down and pos_actual.i + 1 <=24){
      Cell celda = cell(pos_actual.i + 1, pos_actual.j);
      if(celda.type == Soil and (celda.id == -1 or (ant(celda.id).player != me() and ant(celda.id).type == Worker)))return true;
   }else if(direccion == Right and pos_actual.j + 1 <= 24){
     Cell celda = cell(pos_actual.i, pos_actual.j + 1);
     if(celda.type == Soil and (celda.id == -1 or (ant(celda.id).player != me() and ant(celda.id).type == Worker)))return true;
   }else if(direccion == Left and pos_actual.j - 1 >= 0){
      Cell celda = cell(pos_actual.i, pos_actual.j - 1);
    if(celda.type == Soil and (celda.id == -1 or (ant(celda.id).player != me() and ant(celda.id).type == Worker)))return true;
    }
    return false;
  }
  //Devuelve true si la celda en 'dir' a 'pos' esta vacia,contiene un enemigo o es del tablero, falso si no.
   bool defensa_soldado(const Dir direccion,Pos pos_actual){
     if(direccion == Up and pos_actual.i - 1 >= 0){
       Cell celda = cell(pos_actual.i - 1, pos_actual.j);
       if(celda.type == Soil and (celda.id == -1 or (ant(celda.id).player != me() and ant(celda.id).type != Queen)))return true;
    }else if(direccion == Down and pos_actual.i + 1 <=24){
       Cell celda = cell(pos_actual.i + 1, pos_actual.j);
       if(celda.type == Soil and (celda.id == -1 or (ant(celda.id).player != me() and ant(celda.id).type == Queen)))return true;
    }else if(direccion == Right and pos_actual.j + 1 <= 24){
      Cell celda = cell(pos_actual.i, pos_actual.j + 1);
      if(celda.type == Soil and (celda.id == -1 or (ant(celda.id).player != me() and ant(celda.id).type == Queen)))return true;
    }else if(direccion == Left and pos_actual.j - 1 >= 0){
       Cell celda = cell(pos_actual.i, pos_actual.j - 1);
     if(celda.type == Soil and (celda.id == -1 or (ant(celda.id).player != me() and ant(celda.id).type == Queen)))return true;
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
  //Retorna true si puede poner un huevo de soldado, falso si no.
  bool huevo_soldado(const int id){
    Ant reina = ant(id);
    for(unsigned int i = 0; i < reina.reserve.size(); ++i){
      if(reina.reserve[i] < 3) return false;
    }
    return true;
  }
  //Retorna 'true' si la hormiga 'id' esta al lado de la reina, falso si no lo esta.
  bool estan_lado(const Pos posicion,const int reina){
    Pos queen = ant(reina).pos;
    if((queen.i + 1 == posicion.i and queen.j == posicion.j) or (queen.i - 1 == posicion.i and posicion.j == queen.j) or (queen.j + 1 == posicion.j and queen.i==posicion.i) or (queen.j - 1 == posicion.j and queen.i==posicion.i)){
      return true;
    }else if((queen.i + 1 == posicion.i and queen.j == posicion.j-1) or (queen.i - 1 == posicion.i and posicion.j == queen.j-1) or (queen.j + 1 == posicion.j and queen.i-1==posicion.i) or (queen.j + 1 == posicion.j and queen.i+1==posicion.i)){
      return true;
    }
    return false;
  }
// Retorna true si la 'posicion' esta dentro del cuadrante del jugador.
  bool puedo_ir(const Pos posicion){
    if(me() == 0){
      if(posicion.i > 12 or posicion.j > 12) return false;
    }else if(me() == 1){
      if(posicion.i > 12 or posicion.j <= 12) return false;
    }else if(me() == 2){
      if(posicion.i <= 12 or posicion.j <= 12) return false;
    }else if(me() == 3){
      if(posicion.i <= 12 or posicion.j > 12) return false;
    }
    return true;
  }
  /**
   * Play method, invoked once per each round.
   */
   //----------------------------------------------------------FUNCION---------------------------------------------------
  virtual void play () {
    vector <int> soldados = soldiers(me());
    vector <int> trabajadores = workers(me());
    vector <int> reina = queens(me());
    vector <Pos> comida;
    if (round() < 150)  comida = posicion_comida(reina[0]);
    else comida = posicion_comida_tablero(reina[0]);
    vector < vector<bool> > Tablero (25,vector<bool>(25));
    if(round() == 0) Inicializar();

    //------------------------------------------REINA(S)-------------------------------------------------------------------
        for(unsigned int i = 0; i < reina.size(); ++i){
          Ant queen = ant(reina[i]);
          if(cell(queen.pos.i+1,queen.pos.j).id != -1 and ant(cell(queen.pos.i+1,queen.pos.j).id).player != me()) move(queen.id,Down);
          else if(cell(queen.pos.i-1,queen.pos.j).id != -1 and ant(cell(queen.pos.i-1,queen.pos.j).id).player != me()) move(queen.id,Up);
          else if(cell(queen.pos.i,queen.pos.j+1).id != -1 and ant(cell(queen.pos.i,queen.pos.j+1).id).player != me()) move(queen.id,Right);
          else if(cell(queen.pos.i,queen.pos.j-1).id != -1 and ant(cell(queen.pos.i,queen.pos.j-1).id).player != me()) move(queen.id,Left);
          else if(huevo_soldado(queen.id) and round() > 140){
            if(es_posible(Up,queen.pos)) lay(queen.id,Up,Soldier);
            else if(es_posible(Down,queen.pos)) lay(queen.id,Down,Soldier);
            else if(es_posible(Left,queen.pos)) lay(queen.id,Left,Soldier);
            else if(es_posible(Right,queen.pos)) lay(queen.id,Right,Soldier);
            else if(cell(queen.pos).bonus != None) take(queen.id);
          }else if(huevo_trabajador(queen.id)){
            if(es_posible(Up,queen.pos)) lay(queen.id,Up,Worker);
            else if(es_posible(Down,queen.pos)) lay(queen.id,Down,Worker);
            else if(es_posible(Left,queen.pos)) lay(queen.id,Left,Worker);
            else if(es_posible(Right,queen.pos)) lay(queen.id,Right,Worker);
            else if(cell(queen.pos).bonus != None) take(queen.id);
          }else if(cell(queen.pos).bonus != None){
            take(queen.id);
          }else if(round() % 2 == 0){
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
              Pos comida = busca_comida(g,queen.pos);
              int distancia = bfs(g,queen.pos,comida);
              if(puedo_ir(comida)){
                if(queen.pos.i-1 > -1 and bfs(g,Pos(queen.pos.i-1,queen.pos.j),comida) < distancia and not esta_llena(queen.pos.i-1,queen.pos.j,Tablero) and es_posible(Up,queen.pos)){
                  move(queen.id,Up);
                  celda_llena(queen.pos.i-1,queen.pos.j,Tablero);
                }else if(queen.pos.i + 1 < 25 and bfs(g,Pos(queen.pos.i+1,queen.pos.j),comida) < distancia and not esta_llena(queen.pos.i+1,queen.pos.j,Tablero) and es_posible(Down,queen.pos)){
                  move(queen.id,Down);
                  celda_llena(queen.pos.i+1,queen.pos.j,Tablero);
                }else if(queen.pos.j-1 > -1 and bfs(g,Pos(queen.pos.i,queen.pos.j-1),comida) < distancia and not esta_llena(queen.pos.i,queen.pos.j-1,Tablero) and es_posible(Left,queen.pos)){
                  move(queen.id,Left);
                  celda_llena(queen.pos.i,queen.pos.j-1,Tablero);
                }else if(queen.pos.j+1 < 25 and bfs(g,Pos(queen.pos.i,queen.pos.j+1),comida) < distancia and not esta_llena(queen.pos.i,queen.pos.j+1,Tablero) and es_posible(Right,queen.pos)){
                  move(queen.id,Right);
                  celda_llena(queen.pos.i,queen.pos.j+1,Tablero);
                }
              }
            }
          }
//----------------------------------------SOLDADOS---------------------------------------------------------------------------------

  for(unsigned int i = soldados.size()/2; i < soldados.size(); ++i){ //Movimiento de los soldados defensores
    Ant hormiga = ant(soldados[i]);
    if(puedo_ir(hormiga.pos)){
      Pos enemigo = hormiga_soldado(g,hormiga.pos);
      int distancia = bfs(g,hormiga.pos,enemigo);
      if(puedo_ir(enemigo) or round() < 15){
        if(hormiga.pos.i-1 > -1 and bfs(g,Pos(hormiga.pos.i-1,hormiga.pos.j),enemigo) < distancia and not esta_llena(hormiga.pos.i-1,hormiga.pos.j,Tablero) and defensa_soldado(Up,hormiga.pos)){
          move(hormiga.id,Up);
          celda_llena(hormiga.pos.i-1,hormiga.pos.j,Tablero);
        }else if(hormiga.pos.i + 1 < 25 and bfs(g,Pos(hormiga.pos.i+1,hormiga.pos.j),enemigo) < distancia and not esta_llena(hormiga.pos.i+1,hormiga.pos.j,Tablero) and defensa_soldado(Down,hormiga.pos)){
          move(hormiga.id,Down);
          celda_llena(hormiga.pos.i+1,hormiga.pos.j,Tablero);
        }else if(hormiga.pos.j-1 > -1 and bfs(g,Pos(hormiga.pos.i,hormiga.pos.j-1),enemigo) < distancia and not esta_llena(hormiga.pos.i,hormiga.pos.j-1,Tablero) and defensa_soldado(Left,hormiga.pos)){
          move(hormiga.id,Left);
          celda_llena(hormiga.pos.i,hormiga.pos.j-1,Tablero);
        }else if(hormiga.pos.j+1 < 25 and bfs(g,Pos(hormiga.pos.i,hormiga.pos.j+1),enemigo) < distancia and not esta_llena(hormiga.pos.i,hormiga.pos.j+1,Tablero) and defensa_soldado(Right,hormiga.pos)){
          move(hormiga.id,Right);
          celda_llena(hormiga.pos.i,hormiga.pos.j+1,Tablero);
        }
      }
    }else{
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
  }

  for(unsigned int i = 0; i < soldados.size()/2; ++i){ //Movimiento de los soldados atacantes
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

unsigned int c = 0;
for(unsigned int i = 0; i < trabajadores.size(); i++){ // ESTE BUCLE DICE A LAS HORMIGAS CON BONUS, QUE SI ESTAN AL LADO DE LA REINA QUE DEJEN LA COMIDA
  Ant hormiga = ant(trabajadores[i]);

  if(hormiga.bonus != None){
    if((estan_lado(hormiga.pos,reina[0]) and cell(hormiga.pos).bonus == None) or ant(trabajadores[i]).life == 1){
      leave(trabajadores[i]);
    }else{
      Ant queen = ant(reina[0]);
      int distancia = bfs(g,hormiga.pos,queen.pos);
      if(hormiga.pos.i-1 > -1 and bfs(g,Pos(hormiga.pos.i-1,hormiga.pos.j),queen.pos) < distancia and not esta_llena(hormiga.pos.i-1,hormiga.pos.j,Tablero) and es_posible(Up,hormiga.pos) and cell(hormiga.pos.i-1,hormiga.pos.j).bonus == None){
          move(hormiga.id,Up);
          celda_llena(hormiga.pos.i-1,hormiga.pos.j,Tablero);
      }else if(hormiga.pos.i + 1 < 25 and bfs(g,Pos(hormiga.pos.i+1,hormiga.pos.j),queen.pos) < distancia and not esta_llena(hormiga.pos.i+1,hormiga.pos.j,Tablero) and es_posible(Down,hormiga.pos) and cell(hormiga.pos.i+1,hormiga.pos.j).bonus == None){
          move(hormiga.id,Down);
          celda_llena(hormiga.pos.i+1,hormiga.pos.j,Tablero);
      }else if(hormiga.pos.j-1 > -1 and bfs(g,Pos(hormiga.pos.i,hormiga.pos.j-1),queen.pos) < distancia and not esta_llena(hormiga.pos.i,hormiga.pos.j-1,Tablero) and es_posible(Left,hormiga.pos) and cell(hormiga.pos.i,hormiga.pos.j-1).bonus == None){
          move(hormiga.id,Left);
          celda_llena(hormiga.pos.i,hormiga.pos.j-1,Tablero);
      }else if(hormiga.pos.j+1 < 25 and bfs(g,Pos(hormiga.pos.i,hormiga.pos.j+1),queen.pos) < distancia and not esta_llena(hormiga.pos.i,hormiga.pos.j+1,Tablero) and es_posible(Right,hormiga.pos) and cell(hormiga.pos.i,hormiga.pos.j+1).bonus == None){
          move(hormiga.id,Right);
          celda_llena(hormiga.pos.i,hormiga.pos.j+1,Tablero);
      }
    }
  }else{
    if(not estan_lado(hormiga.pos,reina[0])){
      Pos posicion = hormiga.pos;
      Cell celda_actual = cell(posicion);
      if(celda_actual.bonus != None)take(hormiga.id);
    }
    if(c < comida.size()){
      int distancia = bfs(g,hormiga.pos,comida[c]);
      if(hormiga.pos.i-1 > -1 and bfs(g,Pos(hormiga.pos.i-1,hormiga.pos.j),comida[c]) < distancia and not esta_llena(hormiga.pos.i-1,hormiga.pos.j,Tablero) and es_posible(Up,hormiga.pos)){
          move(hormiga.id,Up);
          celda_llena(hormiga.pos.i-1,hormiga.pos.j,Tablero);
      }else if(hormiga.pos.i + 1 < 25 and bfs(g,Pos(hormiga.pos.i+1,hormiga.pos.j),comida[c]) < distancia and not esta_llena(hormiga.pos.i+1,hormiga.pos.j,Tablero) and es_posible(Down,hormiga.pos)){
          move(hormiga.id,Down);
          celda_llena(hormiga.pos.i+1,hormiga.pos.j,Tablero);
      }else if(hormiga.pos.j-1 > -1 and bfs(g,Pos(hormiga.pos.i,hormiga.pos.j-1),comida[c]) < distancia and not esta_llena(hormiga.pos.i,hormiga.pos.j-1,Tablero) and es_posible(Left,hormiga.pos)){
          move(hormiga.id,Left);
          celda_llena(hormiga.pos.i,hormiga.pos.j-1,Tablero);
      }else if(hormiga.pos.j+1 < 25 and bfs(g,Pos(hormiga.pos.i,hormiga.pos.j+1),comida[c]) < distancia and not esta_llena(hormiga.pos.i,hormiga.pos.j+1,Tablero) and es_posible(Right,hormiga.pos)){
          move(hormiga.id,Right);
          celda_llena(hormiga.pos.i,hormiga.pos.j+1,Tablero);
      }
      ++c;
    }
    int rand = random(0,100);
    if (rand >= 50){
      if (es_posible(Up,hormiga.pos) and not esta_llena(hormiga.pos.i-1,hormiga.pos.j,Tablero) and puedo_ir(Pos(hormiga.pos.i-1,hormiga.pos.j))){
          move(hormiga.id,Up);
          celda_llena(hormiga.pos.i-1,hormiga.pos.j,Tablero);
      }else if(es_posible(Down,hormiga.pos) and not esta_llena(hormiga.pos.i+1,hormiga.pos.j,Tablero) and puedo_ir(Pos(hormiga.pos.i+1,hormiga.pos.j))){
          move(hormiga.id,Down);
          celda_llena(hormiga.pos.i+1,hormiga.pos.j,Tablero);
      }else if(es_posible(Left,hormiga.pos) and not esta_llena(hormiga.pos.i,hormiga.pos.j-1,Tablero) and puedo_ir(Pos(hormiga.pos.i,hormiga.pos.j-1))){
          move(hormiga.id,Left);
          celda_llena(hormiga.pos.i,hormiga.pos.j-1,Tablero);
      }else if(es_posible(Right,hormiga.pos) and not esta_llena(hormiga.pos.i,hormiga.pos.j+1,Tablero) and puedo_ir(Pos(hormiga.pos.i,hormiga.pos.j+1))){
          move(hormiga.id,Right);
          celda_llena(hormiga.pos.i,hormiga.pos.j+1,Tablero);
        }
      }else{
        if (es_posible(Right,hormiga.pos) and not esta_llena(hormiga.pos.i,hormiga.pos.j+1,Tablero) and puedo_ir(Pos(hormiga.pos.i,hormiga.pos.j+1))){
          move(hormiga.id,Right);
          celda_llena(hormiga.pos.i,hormiga.pos.j+1,Tablero);
        }else if(es_posible(Left,hormiga.pos) and not esta_llena(hormiga.pos.i,hormiga.pos.j-1,Tablero) and puedo_ir(Pos(hormiga.pos.i,hormiga.pos.j-1))){
          move(hormiga.id,Left);
          celda_llena(hormiga.pos.i,hormiga.pos.j-1,Tablero);
        }else if(es_posible(Down,hormiga.pos) and not esta_llena(hormiga.pos.i+1,hormiga.pos.j,Tablero) and puedo_ir(Pos(hormiga.pos.i+1,hormiga.pos.j))){
          move(hormiga.id,Down);
          celda_llena(hormiga.pos.i+1,hormiga.pos.j,Tablero);
        }else if(es_posible(Up,hormiga.pos) and not esta_llena(hormiga.pos.i-1,hormiga.pos.j,Tablero) and puedo_ir(Pos(hormiga.pos.i-1,hormiga.pos.j))){
          move(hormiga.id,Up);
          celda_llena(hormiga.pos.i-1,hormiga.pos.j,Tablero);
        }
      }
  }
}
  }
};
/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
