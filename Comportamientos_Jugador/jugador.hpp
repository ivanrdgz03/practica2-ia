#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>
#include <algorithm>

struct stateN0{
  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaAccionColaborador;

  bool operator==(const stateN0& other) const{
    return jugador == other.jugador && colaborador == other.colaborador;
  }
};

struct nodeN0{
  stateN0 st;
  list<Action> secuencia;

  bool operator==(const nodeN0& other) const{
    return st == other.st;
  }

  bool operator<(const nodeN0& other) const{
    return ((st.jugador.f < other.st.jugador.f)||
    (st.jugador.f == other.st.jugador.f && st.jugador.c < other.st.jugador.c)||
    (st.jugador.f == other.st.jugador.f && st.jugador.c == other.st.jugador.c &&
    st.jugador.brujula < other.st.jugador.brujula));
  }
};

class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Constructor nivel 4???
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      hayPlan = false;

      current_state.st.jugador = {0,0};
      current_state.st.colaborador = {0,0};
      current_state.st.ultimaAccionColaborador = actIDLE;
      current_state.secuencia.clear();


    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);


  private:
    bool hayPlan;
    list<Action> plan;
    nodeN0 current_state;

    bool Find(const stateN0& state, const list<nodeN0>& lista) const;
    void visualizarPlan(const stateN0& st, const list<Action>& plan);
    void reseteoMatriz(vector<vector<unsigned char>>& matriz);
    bool casillaTransitable(const ubicacion& u) const;
    stateN0 applyAction(const stateN0& state, const Action& accion) const;
    bool busquedaN0(const stateN0& inicio, const ubicacion& final, const vector<vector<unsigned char>>& mapa);
    Action nivel0(const Sensores& sensores);

    bool busquedaN1(const stateN0& inicio, const ubicacion& final, const vector<vector<unsigned char>>& mapa);
    Action nivel1(const Sensores& sensores);





};

#endif
