#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>
#include <algorithm>
#include <queue>
#include <set>
#include <iostream>
#include <cmath>
#include "motorlib/util.h"

struct objetos
{
  bool zapatillas;
  bool bikini;

  bool operator==(const objetos &other) const
  {
    return ((zapatillas == other.zapatillas) && (bikini == other.bikini));
  }
  bool operator<(const objetos &other) const
  {
    if (zapatillas < other.zapatillas)
      return true;
    else if (zapatillas == other.zapatillas && bikini < other.bikini)
      return true;
    else
      return false;
  }
};

struct state
{
  ubicacion jugador;
  ubicacion colaborador;
  objetos objetos_jugador, objetos_colaborador;
  Action ultimaAccionColaborador;

  bool operator==(const state &other) const
  {
    return (jugador == other.jugador && colaborador == other.colaborador && objetos_colaborador == other.objetos_colaborador && objetos_jugador == other.objetos_jugador);
  }
  bool operator<(const state &other) const
  {
    if (jugador.f < other.jugador.f)
      return true;
    else if (jugador.f == other.jugador.f && jugador.c < other.jugador.c)
      return true;
    else if (jugador.f == other.jugador.f && jugador.c == other.jugador.c && jugador.brujula < other.jugador.brujula)
      return true;
    else if (jugador.f == other.jugador.f && jugador.c == other.jugador.c && jugador.brujula == other.jugador.brujula && colaborador.f < other.colaborador.f)
      return true;
    else if (jugador.f == other.jugador.f && jugador.c == other.jugador.c && jugador.brujula == other.jugador.brujula && colaborador.f == other.colaborador.f && colaborador.c < other.colaborador.c)
      return true;
    else if (jugador.f == other.jugador.f && jugador.c == other.jugador.c && jugador.brujula == other.jugador.brujula && colaborador.f == other.colaborador.f && colaborador.c == other.colaborador.c && colaborador.brujula < other.colaborador.brujula)
      return true;
    else if (jugador.f == other.jugador.f && jugador.c == other.jugador.c && jugador.brujula == other.jugador.brujula && colaborador.f == other.colaborador.f && colaborador.c == other.colaborador.c && colaborador.brujula == other.colaborador.brujula && objetos_jugador < other.objetos_jugador)
      return true;
    else if (jugador.f == other.jugador.f && jugador.c == other.jugador.c && jugador.brujula == other.jugador.brujula && colaborador.f == other.colaborador.f && colaborador.c == other.colaborador.c && colaborador.brujula == other.colaborador.brujula && objetos_jugador == other.objetos_jugador && objetos_colaborador < other.objetos_colaborador)
      return true;
    else if (jugador.f == other.jugador.f && jugador.c == other.jugador.c && jugador.brujula == other.jugador.brujula && colaborador.f == other.colaborador.f && colaborador.c == other.colaborador.c && colaborador.brujula == other.colaborador.brujula && objetos_jugador == other.objetos_jugador && objetos_colaborador == other.objetos_colaborador && ultimaAccionColaborador < other.ultimaAccionColaborador)
      return true;
    else
      return false;
  }
};

struct node
{
  state st;
  unsigned int coste;
  unsigned int heuristica;
  list<Action> secuencia;

  bool operator==(const node &other) const
  {
    return (st == other.st);
  }

  bool operator<(const node &other) const
  {
    return ((coste + heuristica) > (other.coste + other.heuristica)||((coste + heuristica) == (other.coste + other.heuristica) && secuencia.size() > other.secuencia.size()));
  }
};

struct stateJugador
{
  ubicacion jugador;
  objetos objetos_jugador;

  bool operator==(const stateJugador &other) const
  {
    return (jugador == other.jugador && objetos_jugador == other.objetos_jugador);
  }
  bool operator<(const stateJugador &other) const
  {
    if (jugador.f < other.jugador.f)
      return true;
    else if (jugador.f == other.jugador.f && jugador.c < other.jugador.c)
      return true;
    else if (jugador.f == other.jugador.f && jugador.c == other.jugador.c && jugador.brujula < other.jugador.brujula)
      return true;
    else if (jugador.f == other.jugador.f && jugador.c == other.jugador.c && jugador.brujula == other.jugador.brujula && objetos_jugador < other.objetos_jugador)
      return true;
    else
      return false;
  }
};

struct nodeJugador
{
  stateJugador st;
  unsigned int coste;
  list<Action> secuencia;

  bool operator==(const nodeJugador &other) const
  {
    return (st == other.st);
  }

  bool operator<(const nodeJugador &other) const
  {
    return (coste > other.coste);
  }
};

struct stateJugador4
{
  ubicacion jugador;
  objetos objetos_jugador;

  bool operator==(const stateJugador4 &other) const
  {
    return (jugador == other.jugador && objetos_jugador == other.objetos_jugador);
  }
  bool operator<(const stateJugador4 &other) const
  {
    if (jugador.f < other.jugador.f)
      return true;
    else if (jugador.f == other.jugador.f && jugador.c < other.jugador.c)
      return true;
    else if (jugador.f == other.jugador.f && jugador.c == other.jugador.c && jugador.brujula < other.jugador.brujula)
      return true;
    else if (jugador.f == other.jugador.f && jugador.c == other.jugador.c && jugador.brujula == other.jugador.brujula && objetos_jugador < other.objetos_jugador)
      return true;
    else
      return false;
  }
};

struct nodeJugador4
{
  stateJugador4 st;
  unsigned int coste;
  unsigned int heuristica;
  list<Action> secuencia;

  bool operator==(const nodeJugador4 &other) const
  {
    return (st == other.st);
  }

  bool operator<(const nodeJugador4 &other) const
  {
    return ((coste + heuristica) > (other.coste + other.heuristica));
  }
};

class ComportamientoJugador : public Comportamiento
{
public:
  ComportamientoJugador(unsigned int size) : Comportamiento(size)
  {
    hayPlan = false;
    lastAction = actIDLE;
    bien_situado = false;
    current_state.st.jugador = {0, 0};
    current_state.st.colaborador = {0, 0};
    current_state.st.ultimaAccionColaborador = act_CLB_STOP;
    current_state.secuencia.clear();
    current_state.coste = 0;
    current_state.st.objetos_jugador = {false, false};
    current_state.st.objetos_colaborador = {false, false};

    current_state_jugador.st.jugador = {0, 0};
    current_state_jugador.st.objetos_jugador = {false, false};
    current_state_jugador.secuencia.clear();
    current_state_jugador.coste = 0;

    current_state4.st.jugador = {0, 0};
    current_state4.st.objetos_jugador = {false, false};
    current_state4.secuencia.clear();
    current_state4.coste = 0;
    current_state4.heuristica = 0;
    colab = {0, 0};
    mov_sin_recarga = 0;
  }
  ComportamientoJugador(std::vector<std::vector<unsigned char>> mapaR) : Comportamiento(mapaR)
  {
    hayPlan = false;

    current_state.st.jugador = {0, 0};
    current_state.st.colaborador = {0, 0};
    current_state.st.ultimaAccionColaborador = act_CLB_STOP;
    current_state.secuencia.clear();
    current_state.coste = 0;
    current_state.st.objetos_jugador = {false, false};
    current_state.st.objetos_colaborador = {false, false};

    current_state_jugador.st.jugador = {0, 0};
    current_state_jugador.st.objetos_jugador = {false, false};
    current_state_jugador.secuencia.clear();
    current_state_jugador.coste = 0;
  }
  ComportamientoJugador(const ComportamientoJugador &comport) : Comportamiento(comport) {}
  ~ComportamientoJugador() {}

  Action think(Sensores sensores);
  int interact(Action accion, int valor);

private:
  const vector<pair<int, int>> sensores_norte = {{0, 0},
                                                 {-1, -1},
                                                 {-1, 0},
                                                 {-1, 1},
                                                 {-2, -2},
                                                 {-2, -1},
                                                 {-2, 0},
                                                 {-2, 1},
                                                 {-2, 2},
                                                 {-3, -3},
                                                 {-3, -2},
                                                 {-3, -1},
                                                 {-3, 0},
                                                 {-3, 1},
                                                 {-3, 2},
                                                 {-3, 3}};
  const vector<pair<int, int>> sensores_noreste = {{0, 0},
                                                   {-1, 0},
                                                   {-1, 1},
                                                   {0, 1},
                                                   {-2, 0},
                                                   {-2, 1},
                                                   {-2, 2},
                                                   {-1, 2},
                                                   {0, 2},
                                                   {-3, 0},
                                                   {-3, 1},
                                                   {-3, 2},
                                                   {-3, 3},
                                                   {-2, 3},
                                                   {-1, 3},
                                                   {0, 3}};

  bool hayPlan, bien_situado;
  list<Action> plan;
  node current_state;
  unsigned int mov_sin_recarga = 0;
  ubicacion colab;
  nodeJugador current_state_jugador;
  nodeJugador4 current_state4;
  Action lastAction;

  void visualizarPlan(const state &st, const list<Action> &plan);
  void reseteoMatriz(vector<vector<unsigned char>> &matriz);
  bool casillaTransitable(const ubicacion &u) const;
  state applyAction(const state &state, const Action &accion) const;
  bool busquedaN0(const stateJugador &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa, const Sensores &sensores);
  Action nivel0(const Sensores &sensores);

  ubicacion obtener_coordenadas(const ubicacion &u, const unsigned int &pos) const;
  bool colaboradorEnSensor(const state &st, const Sensores &sensores) const;
  bool busquedaN1(const state &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa, const Sensores &sensores);
  Action nivel1(const Sensores &sensores);

  Action nivel2(const Sensores &sensores);
  bool busquedaN2(const stateJugador &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa, const Sensores &sensores);
  unsigned int calculoCoste(const state &st, const Action &accion) const;
  unsigned int calculoCoste(const stateJugador &st, const Action &accion) const;
  stateJugador applyAction(const stateJugador &st, const Action &accion, const Sensores &sensores) const;
  void visualizarPlan(const stateJugador &st, const list<Action> &plan, const Sensores &sensores);

  bool busquedaN3(const state &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa, const Sensores &sensores);
  Action nivel3(const Sensores &sensores);
  ubicacion NextCasilla(const ubicacion &pos) const;
  unsigned int calcularHeuristica(const ubicacion &actual, const ubicacion &destino, const Sensores &sensores) const;

  Action nivel4(const Sensores &sensores);
  bool busquedaN4(const stateJugador4 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa, const Sensores &sensores, const ubicacion &colab);
  void guardar_mapa(const Sensores &sensores, vector<vector<unsigned char>> &mapa);
  pair<unsigned int, unsigned int> obtener_coordenadas(const unsigned int &pos) const;
  bool personajesSinColisionar(const state &st, const state &inicio, const Sensores &sensores, const vector<vector<unsigned char>> &mapa) const;
  unsigned int calculoCoste(const stateJugador4 &st, const Action &accion) const;
  stateJugador4 applyAction(const stateJugador4 &st, const Action &accion, const ubicacion &colab) const;
};

#endif
