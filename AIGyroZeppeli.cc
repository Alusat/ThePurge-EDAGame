#include "Player.hh"
#include <queue>
/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME HADES


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */
const int INF = 10000000;
const int N = 15;
const int M = 30;
    using P = pair<int, int>;
    using VB = vector<bool>;
    using VBB = vector<VB>;
    using VC = vector<char>;
    using VP = vector<Pos>;
    using VCC = vector<VC>;
    using VI = vector<int>;
    using VII = vector<VI>;
    using VPP =  vector<vector<Pos>> ; //vector posiciones
    using DP = pair<int, Pos>; //pair distancia pos

const vector<Dir> dirs = {Up,Down,Left,Right};



int Weapon2tier(Pos p){
    if(citizen(cell(p).id).weapon == Bazooka) return 3;
    else if(citizen(cell(p).id).weapon == Gun) return 2;
    else if(citizen(cell(p).id).weapon == Hammer) return 1;
    return 0; //para los builders

}


int bfs_distancia(Pos inici, Pos objetivo) { 
    VII dist(N, VI(M, -1)); //todas las distancias en infinito
    queue<Pos> Q; 
    dist[inici.i][inici.j] = 0;
    Q.push(inici);
    while (not Q.empty()) {
        Pos p = Q.front();
        Q.pop();
        for (Dir d : dirs) {
            Pos p2 = p+d;
            if (pos_ok(p2) and cell(p2).type != Building and dist[p2.i][p2.j] == -1) {
                dist[p2.i][p2.j] = dist[p.i][p.j] + 1;
                //prev[p2.i][p2.j] =  ///??????????????????
                if (p2 == objetivo) {//para ver si hay eso
                    return dist[p2.i][p2.j];
                }
                Q.push(p2);
            }
        }

    }
    return INF; //no existe lo que buscamos
}

Pos bfs_bonus(Pos inici, char objetivo) { 
    VII dist(N, VI(M, -1)); //todas las distancias en infinito
    VII prev(N, VI(M, -1));
    queue<Pos> Q; 
    dist[inici.i][inici.j] = 0;
    Q.push(inici);
    while (not Q.empty()) {
        Pos p = Q.front();
        Q.pop();
        for (Dir d : dirs) {
            Pos p2 = p+d;
            if (pos_ok(p2) and cell(p2).type != Building and dist[p2.i][p2.j] == -1 and (cell(p2).resistance < 0) and citizen(cell(p2).id).player != me()) {
                dist[p2.i][p2.j] = dist[p.i][p.j] + 1;
                //prev[p2.i][p2.j] =  ///??????????????????
                if (cell(p+d).bonus == objetivo) {//para ver si hay eso
                    return p2;
                }
                Q.push(p2);
            }
        }

    }
    return inici; //no existe lo que buscamos
}


Pos bfs_bonus_excepcion(Pos inici, char objetivo, Pos excepcion) { 
    VII dist(N, VI(M, -1)); //todas las distancias en infinito
    VII prev(N, VI(M, -1));
    queue<Pos> Q; 
    dist[inici.i][inici.j] = 0;
    Q.push(inici);
    while (not Q.empty()) {
        Pos p = Q.front();
        Q.pop();
        for (Dir d : dirs) {
            Pos p2 = p+d;
            if (pos_ok(p2) and cell(p2).type != Building and dist[p2.i][p2.j] == -1 and (cell(p2).resistance < 0) and citizen(cell(p2).id).player != me()) {
                dist[p2.i][p2.j] = dist[p.i][p.j] + 1;
                //prev[p2.i][p2.j] =  ///??????????????????
                if (cell(p+d).bonus == objetivo and p2 != excepcion) {//para ver si hay eso
                    return p2;
                }
                Q.push(p2);
            }
        }

    }
    return inici; //no existe lo que buscamos
}


Pos bfs_weapon(Pos inici, char objetivo) { 
    VII dist(N, VI(M, -1)); //todas las distancias en infinito
    VII prev(N, VI(M, -1));
    queue<Pos> Q; 
    dist[inici.i][inici.j] = 0;
    Q.push(inici);
    while (not Q.empty()) {
        Pos p = Q.front();
        Q.pop();
        for (Dir d : dirs) {
            Pos p2 = p+d;
            if (pos_ok(p2) and cell(p2).type != Building and dist[p2.i][p2.j] == -1 and cell(p2).resistance == -1 and citizen(cell(p2).id).player != me()) {
                dist[p2.i][p2.j] = dist[p.i][p.j] + 1;
                if (cell(p+d).weapon == objetivo) {//para ver si hay eso
                    return p2;
                }
                Q.push(p2);
            }
        }

    }
    return inici; //no existe lo que buscamos
}

Pos bfs_enemigo_huir(Pos inici) { //puede tener barricada el enemigo
    VII dist(N, VI(M, -1)); //todas las distancias en infinito
    VII prev(N, VI(M, -1));
    queue<Pos> Q; 
    dist[inici.i][inici.j] = 0;
    Q.push(inici);
    while (not Q.empty()) {
        Pos p = Q.front();
        Q.pop();
        for (Dir d : dirs) {
            Pos p2 = p+d;
            if (pos_ok(p2) and cell(p2).type != Building and dist[p2.i][p2.j] == -1) {
                dist[p2.i][p2.j] = dist[p.i][p.j] + 1;
                //prev[p2.i][p2.j] =  ///??????????????????
                if (citizen(cell(p2).id).player != me() and citizen(cell(p2).id).player != -1 and (Weapon2tier(inici) < Weapon2tier(p2))) {
                  //machup meojor siempre por si me espera un bazooka en barricada
                    return p2;
                }
                Q.push(p2);
            }
        }

    }
    return inici; //no existe lo que buscamos
}


Pos bfs_enemigo_sin_barricada(Pos inici) { //puede tener barricada el enemigo
    VII dist(N, VI(M, -1)); //todas las distancias en infinito
    VII prev(N, VI(M, -1));
    queue<Pos> Q; 
    dist[inici.i][inici.j] = 0;
    Q.push(inici);
    while (not Q.empty()) {
        Pos p = Q.front();
        Q.pop();
        for (Dir d : dirs) {
            Pos p2 = p+d;
            if (pos_ok(p2) and cell(p2).type != Building and dist[p2.i][p2.j] == -1) {
                dist[p2.i][p2.j] = dist[p.i][p.j] + 1;
                //prev[p2.i][p2.j] =  ///??????????????????
                if ((Weapon2tier(inici) >= Weapon2tier(p2)) and citizen(cell(p2).id).player != me() and citizen(cell(p2).id).player != -1 and cell(p2).resistance < 0) {
                  //machup meojor siempre por si me espera un bazooka en barricada
                    return p2;
                }
                Q.push(p2);
            }
        }

    }
    return inici; //no existe lo que buscamos
}
Dir Contrario(Dir d){
  if(d == Up) return dirs[1];
  else if (d == Down) return dirs[0];
  else if (d == Left) return dirs[3];
  return dirs[2];
}

Pos huir(Pos inici) { //retorno algo diferente de pos si hay un enemigo al lado
    int d = 0;
    Pos enemy_pos = bfs_enemigo_huir(inici);
    if(enemy_pos == inici) return inici;    
    if(bfs_distancia(inici, enemy_pos) >= 3) return inici;
    int dist_max = -1;
    int d_buena = -1;
    for(int d = 0; d < 4; ++d){
      if(pos_ok(inici+dirs[d]) and cell(inici+dirs[d]).type != Building and cell(inici+dirs[d]).resistance < 0 and cell(inici+dirs[d]).id == -1) {
        // el weapon2tier de la comparacion para ver si es un warrior 
       int dist_act = bfs_distancia(inici+dirs[d], enemy_pos);
       if(dist_act > dist_max){
          dist_max = dist_act;
          d_buena = d;
       }
      } 
    }
    if(dist_max != -1 and d_buena != -1) {
     return inici+dirs[d_buena]; //puede ser que no sea valida o sea un edificio o algo mirarlo luego
    }
    return inici;
}
Pos bfs_enemigo_con_barricada(Pos inici) { //SI QUE TEINE BARICADA
    VII dist(N, VI(M, -1)); //todas las distancias en infinito
    VII prev(N, VI(M, -1));
    queue<Pos> Q; 
    dist[inici.i][inici.j] = 0;
    Q.push(inici);
    while (not Q.empty()) {
        Pos p = Q.front();
        Q.pop();
        for (Dir d : dirs) {
            Pos p2 = p+d;
            if (pos_ok(p2) and cell(p2).type != Building and dist[p2.i][p2.j] == -1) {
                dist[p2.i][p2.j] = dist[p.i][p.j] + 1;
                //prev[p2.i][p2.j] =  ///??????????????????
                if ((Weapon2tier(inici) >= Weapon2tier(p2)) and citizen(cell(p2).id).player != me() and citizen(cell(p2).id).player != -1) {// barricadas, buen machup
                    return p2;
                }
                Q.push(p2);
            }
        }

    }
    return inici; //no existe lo que buscamos
}


Pos dij(Pos inicio, Pos objetivo) { //devuelve la casilla a la cual moverse, existe seguro por el bfs
    VII dist(N, VI(M, INF)); //todas las distancias en infinito  
    VBB visitados(N, VB(M, false));
    queue<Pos>anteriores;
    dist[inicio.i][inicio.j] = 0;
    priority_queue<DP>pq; 
    pq.push(DP(0,inicio));
    VPP padre (N, VP(M));
    while (not pq.empty()) {
        DP dp = pq.top(); //distancia + posicion
        pq.pop();
        int d = -dp.first;//distancia en negativo, abajo se metia en negativo
        Pos x = dp.second; //x = posicion
        if(d == dist[x.i][x.j]){ //por si los repetidos
            if(x == objetivo){ //sobra el pox_ok pq no sera objetivo en algo incorrecto
              VP V;
              while(x != inicio){
                V.push_back(x);
                x = padre[x.i][x.j];
              }
              /*no se llega a meter la del principio!!
              lo tenemos ordenado de mas reciente a mas antigua recorrida (queremos la mas)
              antigua sin contar la primera*/
              int casillas = V.size();
              /*for(int i = 0; i < casillas; ++i) {
                 cout << "destino dfs: pos" << i << ' ' << V[i].i << ' ' << V[i].j << endl;
              }*/
              if(casillas > 0) return V[casillas-1];//devuelve la primera pos (movimiento que hay que hacer)
            }
            if(not visitados[x.i][x.j]){
              visitados[x.i][x.j] = true;
              for (int i = 0; i < 4; ++i) {
                  int distancia_moverse = 10; //hacer que dependa del tipo de casilla(peor = + alta) 
                  //if(cell(x).id != -1) distancia_moverse = 6;
                  //if (cell(x).bonus == Money) distancia_moverse = 1;
                  int d2 = d+distancia_moverse;
                  Pos x2 = x+dirs[i];
                  //IMPORATNTE CAMBIAR ESTO IMPORATNTE CAMBIAR ESTO IMPORTANTE CAMBIAR ESTO IMPORTANTE CAMBIAR ESTO IMPORTANTE
                  if(pos_ok(x2) and  cell(x2).type != Building  and d2 < dist[x2.i][x2.j] and not(cell(x2).resistance > 0)and citizen(cell(x2).id).player != me()){ //cambiar para no chocarse con gente
                    //cell(x2).id != me() da peor en algunos casos
                    dist[x2.i][x2.j] = d2;
                    pq.push(DP(-d2,x2));
                    padre[x2.i][x2.j] = x;
                  }
              }
            }
        }

    }
    return inicio; //no deberia de poder pasar al haberlo encontrado con el dfs
}
  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    //cout << "YO: " << me() << endl;
     if (is_day()) { //es de dia DIA---DIA---DIA---DIA---DIA---DIA---DIA---DIA---DIA---DIA
        //take care of warriors
        vector<int> w = warriors(me());
        for (int id : w) { // Iterate over all warriors WARRIORS---WARRIORS---WARRIORS---WARRIORS
            //acciones guerreros dia
              Pos p = citizen(id).pos;
              Pos destino = p;
              //QUITAR COMENTARIO
              if(citizen(id).weapon != Bazooka)destino = bfs_weapon(p, Bazooka); //no tengo bazooka, go to bazooka//TODO LO DE ABAJO PASA SI NO TENGO BAZOOKA O NO QUEDAN
              if(destino == p and citizen(id).weapon != Gun and citizen(id).weapon != Bazooka) destino = bfs_weapon(p, Gun);//llego a bazooka y no tengo arma go to pistola
              if(destino == p and citizen(id).weapon == Bazooka and citizen(id).life < 90) destino = bfs_bonus(p, Food);///tengo bazooka, no tengo max vida, go heal. VIDA MAX == 100
              if(destino == p){
                vector<int> b = builders(me());
               Pos destino = bfs_bonus(p, Money);
               bool mas_cercano = true;
               if(b.size() != 1 and destino != p){//no es mi unico builder restante
                int distacia_inicial = bfs_distancia(p, destino);
                int i = 0;
                while(i < b.size() and mas_cercano) {//hay que pasar por todos los builders
                  if(citizen(b[i]).pos != p){
                    int distancia_actual = bfs_distancia(citizen(b[i]).pos, destino); 
                     if(distacia_inicial > distancia_actual) mas_cercano = false;
                  } 
                  ++i;
                }   
               }
            //cout << "ACABADO BFS BUILDERS DIA" << endl;
            //cout << "destino bfs: " << destino.i << ' ' << destino.j << endl;
               if(not mas_cercano) destino = bfs_bonus_excepcion(p, Money, destino);
            //if(citizen(id).life  < 40) destino = bfs_bonus(p, Food); //CURARSE?? CURARSE?? CURARSE?? CURARSE?? CURARSE?? CURARSE?? CURARSE?? CURARSE?? CURARSE?? CURARSE?? CURARSE?? CURARSE?? CURARSE??
              
              } 
              Pos paso = dij(p, destino);
              int d = 0;
              if(paso != p){
                while(d < 4 and (p+dirs[d] != paso)){
                  ++d;
                }
              }
              if(d != 4 and pos_ok(p+dirs[d]))move(id, dirs[d]);
            //if (pos_ok(p+Right))move(id, Right); para probar
        }
        //take care of builders
        //take care of builders
        vector<int> b = builders(me());
        for (int id : b) { // Iterate over all builders BUILDERS---BUILDERS---BUILDERS---BUILDERS
            Pos p = citizen(id).pos;
            Pos destino = p;
	          //if(citizen(id).life < 60) destino = bfs_bonus(p, Food);
            if(destino == p) {
            destino = bfs_bonus(p, Money);
            bool mas_cercano = true;
            if(b.size() != 1 and destino != p){//no es mi unico builder restante
                int distacia_inicial = bfs_distancia(p, destino);
                int i = 0;
                while(i < b.size() and mas_cercano) {//hay que pasar por todos los builders
                  if(citizen(b[i]).pos != p){
                    int distancia_actual = bfs_distancia(citizen(b[i]).pos, destino); 
                     if(distacia_inicial > distancia_actual) mas_cercano = false;
                  } 
                  ++i;
                }   
            }
            //cout << "ACABADO BFS BUILDERS DIA" << endl;
            //cout << "destino bfs: " << destino.i << ' ' << destino.j << endl;
            if(not mas_cercano) destino = bfs_bonus_excepcion(p, Money, destino);
            }
            //if(citizen(id).life  < 60) destino = bfs_bonus(p, Food); //CURARSE?? CURARSE?? CURARSE?? CURARSE?? CURARSE?? CURARSE?? CURARSE?? CURARSE?? CURARSE?? CURARSE?? CURARSE?? CURARSE?? CURARSE??
            Pos paso = dij(p, destino);
            //cout << "ACABADO DIJ BUILDERS DIA" << endl;
	          int d = 0;
            if(paso != p){
              while(d < 4 and (p+dirs[d]) != paso){
                ++d;
              }
            }
            if(d != 4 and pos_ok(p+dirs[d])) move(id, dirs[d]);
            //if (pos_ok(p+Right))move(id, Right);
        }
      }
      else{ //es de noche NOCHE---NOCHE---NOCHE---NOCHE---NOCHE---NOCHE---NOCHE---NOCHE
        //take care of warriors
        vector<int> w = warriors(me());
        for (int id : w) { // Iterate over all warriors WARRIORS---WARRIORS---WARRIORS
            //acciones guerreros noche
            Pos p = citizen(id).pos;
            Pos destino = p;
            //hacer que si esta muy low pille comida
            destino = huir(p); //huye de gente con mejores armas!
            if(destino == p and citizen(id).weapon != Bazooka) destino = bfs_weapon(p, Bazooka);
            if(destino == p and citizen(id).weapon != Bazooka and citizen(id).weapon != Gun)destino = bfs_weapon(p, Gun);
            if(destino == p and citizen(id).life  < 40)
            {
              destino = huir(p); //HUIR PARA GENTE CON MEJORES ARMAS !!
              if(destino == p) destino = bfs_bonus(p, Food);
            } 
            //tengo un arma:
            if(destino == p and citizen(id).weapon == Bazooka)destino = bfs_enemigo_con_barricada(p);
            if(destino == p)destino = bfs_enemigo_sin_barricada(p);
            //cout << "ACABADO BFS WARRIORS NOCHE" << endl;
              Pos paso = dij(p, destino);
              //cout << "ACABADO DFS WARRIORS NOCHE" << endl;
              int d = 0;
              if(paso != p){
                while(d < 4 and (p+dirs[d] != paso)){
                  ++d;
                }
              }
            if(d != 4 and pos_ok(p+dirs[d]))move(id, dirs[d]);
            //if (pos_ok(p+Right))move(id, Right);
            //if (pos_ok(p+Left))move(id, Left);

        }
        //take care of builders
       vector<int> b = builders(me());
        for (int id : b) { // Iterate over all builders BUILDERS---BUILDERS---BUILDERS---BUILDERS
            Pos p = citizen(id).pos;
            //cout <<"posicion actual obrero: " << p.i << ' ' << p.j <<endl;
            if(cell(p).resistance == -1){
              int d = 0;
            Pos destino = huir(p);
              if(destino != p) {
                int k = 0;
                  while(k < 4 and (p+dirs[k]) != destino){
                  ++k;
                }
                if(pos_ok(p+dirs[k])) move(id, dirs[k]);
              }
            //if(citizen(id).life < 60) destino = bfs_bonus(p, Food);
            if(destino == p){
              destino = bfs_bonus(p, Money);
            bool mas_cercano = true;
            if(b.size() != 1 and destino != p){//no es mi unico builder restante
                int distacia_inicial = bfs_distancia(p, destino);
                int i = 0;
                while(i < b.size() and mas_cercano) {//hay que pasar por todos los builders
                  if(citizen(b[i]).pos != p and not (cell(citizen(b[i]).pos).resistance > 0)){ //no tener en cuentas los q estan en barricada
                    int distancia_actual = bfs_distancia(citizen(b[i]).pos, destino); 
                     if(distacia_inicial > distancia_actual) mas_cercano = false;
                  } 
                  ++i;
                }   
            }
            //cout << "ACABADO BFS BUILDERS DIA" << endl;
            //cout << "destino bfs: " << destino.i << ' ' << destino.j << endl;
            if(not mas_cercano) destino = bfs_bonus_excepcion(p, Money, destino);
            }
            Pos paso = dij(p, destino);
            //cout << "ACABADO DIJ BUILDERS DIA" << endl;
	          d = 0;
            if(paso != p){
              while(d < 4 and (p+dirs[d]) != paso){
                ++d;
              }
            }
            if(d != 4 and pos_ok(p+dirs[d])) move(id, dirs[d]);
            //if (pos_ok(p+Right))move(id, Right);
        }
        }
      }
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);


//ARREGLAR HUIR  TIPO QUITAR LO DE CONSTRUIR Y VER SI FUNCIONA
//Y QUEDARSE STUCK EN BARRICADAS TIPO INTENTA MOVERSE AL LADO DE LA QUE YA HAY OTRO