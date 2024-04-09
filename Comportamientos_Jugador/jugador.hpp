#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>

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
    return ((st.jugador.f < other.st.jugador.f)||(st.jugador.f == other.st.jugador.f && st.jugador.c < other.st.jugador.c));
  }
};

class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
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

    stateN0 applyAction(const stateN0& state, const Action& accion) const;
    bool busquedaN0(const Sensores& sensores);
    Action nivel0(const Sensores& sensores);





};

#endif
