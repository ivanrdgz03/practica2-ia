#include "motorlib.hpp"

#include <unistd.h>

extern MonitorJuego monitor;

bool actuacionJugador(unsigned char celdaJ_inicial, unsigned char celdaJ_fin, unsigned char celdaS_inicial, unsigned char celdaS_fin, Action accion, unsigned int x, unsigned int y)
{
  int posibleElAvance;
  unsigned int mix, miy;
  unsigned char celdaRand;
  bool salida = false;
  int gasto_jugador = 0, gasto_colaborador = 0, gasto;
  bool ColaboradorEnVision = monitor.CanISeeColaborador(0);
  Action accionColaborador;

  // Primero evaluo si hay batería suficiente para realizar las acciones del jugador y del colaborador.

  if (ColaboradorEnVision and (accion == act_CLB_WALK or accion == act_CLB_STOP or accion == act_CLB_TURN_SR)){
    accionColaborador = accion;
  }
  else {
    accionColaborador = monitor.get_entidad(1)->getLastAction();
  }

  gasto_colaborador = monitor.get_entidad(0)->fixBateria_sig_accion_colaborador(celdaS_inicial,accionColaborador);
  gasto_jugador     = monitor.get_entidad(0)->fixBateria_sig_accion_jugador(celdaJ_inicial, accion);
  gasto = gasto_jugador + gasto_colaborador;

  //std::cout << "Accion Jugador: " << accion << "   Accion Colaborador: " << accionColaborador << endl;
  //std::cout << "Gasto Jugador: " << gasto_jugador << "   Gasto Colaborador: " << gasto_colaborador << "   Gasto Total: " << gasto << endl;


  if (gasto > monitor.get_entidad(0)->getBateria())
  {
    cout << "La batería necesaria para las acciones seleccionadas superó a la batería disponible\n";
    monitor.get_entidad(0)->setBateria(0);
    
    return false;
  }


  switch (accion)
  {
  case actWALK:
    monitor.get_entidad(1)->SetActionSent(actIDLE);
    if (monitor.getMapa()->casillaOcupada(0) == -1) // Casilla destino desocupada
    {
      switch (celdaJ_fin)
      {
      case 'M': // Muro
        monitor.get_entidad(0)->seAostio();
        std::cout << "El jugador ha chocado con un muro\n";
        break;
      case 'P': // Precipicio
        std::cout << "Se cayó por un precipicio\n";
        monitor.get_entidad(0)->resetEntidad();
        monitor.get_entidad(0)->setHitbox(true);
        monitor.finalizarJuego();
        monitor.setMostrarResultados(true);
        break;
      case 'X': // Casilla Rosa (Recarga)
        // monitor.get_entidad(0)->increaseBateria(10);
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;
      case 'D': // Casilla Morada (Zapatillas)
        // tomo la zapatilla
        monitor.get_entidad(0)->Cogio_Zapatillas(true);
        // pierdo el bikini
        monitor.get_entidad(0)->Cogio_Bikini(false);
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;
      case 'K': // Casilla Amarilla (Bikini)
        // tomo el bikini
        monitor.get_entidad(0)->Cogio_Bikini(true);
        // pierdo las zapatillas
        monitor.get_entidad(0)->Cogio_Zapatillas(false);
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;
      case 'G': // Casilla celeste claro (GPS)
        monitor.get_entidad(0)->notify();
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;

      case 'B': // Arbol
      case 'A': // Agua
      default:
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;
      }
      if (monitor.get_entidad(0)->isMemberObjetivo(x, y) != -1 and monitor.get_entidad(0)->allLessOneObjetivosAlcanzados())
      {
        // acaba de completar todos los objetivos.
        cout << "-----> Casilla objetivo alcanzada por el jugador\n";
        monitor.get_entidad(0)->setCompletoLosObjetivos();
        if (monitor.getLevel() == 4)
        {
          monitor.put_active_objetivos(1);
          monitor.get_entidad(0)->anularAlcanzados();
          monitor.get_entidad(0)->incrMisiones();
          monitor.get_entidad(0)->incrPuntuacion(1);
          for (unsigned int i = 0; i < monitor.numero_entidades(); i++)
          {
            monitor.get_entidad(i)->setObjetivos(monitor.get_active_objetivos());
          }
        }
        else if (monitor.getLevel() == 0 or monitor.getLevel() == 2)
        {
          // El jugador llegó a la casilla objetivo.
          monitor.finalizarJuego();
          monitor.setMostrarResultados(true);
        }
        else {
          cout << "-----> En este nivel es el COLABORADOR el que debe llegar a la casilla objetivo\n";
        }
      }
      // monitor.get_entidad(0)->fixBateria_sig_accion(celdaJ_inicial, accion);
    }
    else
    {
      // Choca contra una entidad
      monitor.get_entidad(0)->seAostio();
      if (monitor.get_entidad(monitor.getMapa()->casillaOcupada(0))->getSubTipo() == aldeano)
      {
        monitor.get_entidad(0)->perderPV(1);
        std::cout << "El jugador ha chocado con un aldeano\n";
      }
      else if (monitor.get_entidad(monitor.getMapa()->casillaOcupada(0))->getSubTipo() == colaborador)
      {
        monitor.get_entidad(0)->perderPV(1);
        std::cout << "El jugador ha chocado con un colaborador\n";
      }
      else if (monitor.get_entidad(monitor.getMapa()->casillaOcupada(0))->getSubTipo() == lobo)
      {
        // Opcion reiniciarse en otro punto del mapa
        /*do
        {
          mix = aleatorio(monitor.getMapa()->getNFils() - 1);
          miy = aleatorio(monitor.getMapa()->getNCols() - 1);

          celdaRand = monitor.getMapa()->getCelda(mix, miy);
        } while (celdaRand == 'P' or celdaRand == 'M' or monitor.getMapa()->entidadEnCelda(mix, miy) != '_');

        monitor.get_entidad(0)->resetEntidad();
        monitor.get_entidad(0)->setPosicion(mix, miy);
        monitor.get_entidad(0)->setOrientacion(static_cast<Orientacion>(aleatorio(7)));
        monitor.get_entidad(0)->setHitbox(true);
        monitor.get_entidad(0)->Cogio_Bikini(false);
        monitor.get_entidad(0)->Cogio_Zapatillas(false);*/

        // Opcion simplemente choca contra el lobo
        monitor.get_entidad(0)->perderPV(1);
        std::cout << "El jugador ha chocado con un lobo\n";
      }
      salida = false;
    }
    break;

  case actRUN:
    monitor.get_entidad(1)->SetActionSent(actIDLE);
    posibleElAvance = monitor.getMapa()->EsPosibleCorrer(0, 2);

    if (posibleElAvance != 0)
    { // No se ha podido avanzar.
      switch (posibleElAvance){
        case 1: // Muro
          monitor.get_entidad(0)->seAostio();
          std::cout << "El jugador ha chocado con un muro\n";
          break;

        case 2: // Precipicio
          std::cout << "Se cayó por un precipicio\n";
          monitor.get_entidad(0)->resetEntidad();
          monitor.get_entidad(0)->setHitbox(true);
          monitor.finalizarJuego();
          monitor.setMostrarResultados(true);
        break;

        case 3: // Colaborador
          monitor.get_entidad(0)->seAostio();
          std::cout << "El jugador ha chocado con el colaborador\n";
          break;

        case 4: // aldeano
          monitor.get_entidad(0)->seAostio();
          std::cout << "El jugador ha chocado con un aldeano\n";
          break;

        case 5: // lobo
          monitor.get_entidad(0)->seAostio();
          std::cout << "El jugador ha chocado con un lobo\n";
          break;
      }

    }
    else
    { // Es posible correr
      switch (celdaJ_fin)
      {
      case 'X': // Casilla Rosa (Recarga)
        // monitor.get_entidad(0)->increaseBateria(10);
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;
      case 'D': // Casilla Morada (Zapatillas)
        // tomo la zapatilla
        monitor.get_entidad(0)->Cogio_Zapatillas(true);
        // pierdo el bikini
        monitor.get_entidad(0)->Cogio_Bikini(false);
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;
      case 'K': // Casilla Amarilla (Bikini)
        // tomo el bikini
        monitor.get_entidad(0)->Cogio_Bikini(true);
        // pierdo las zapatillas
        monitor.get_entidad(0)->Cogio_Zapatillas(false);
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;
      case 'G': // Casilla celeste claro (GPS)
        monitor.get_entidad(0)->notify();
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;

      case 'B': // Arbol
      case 'A': // Agua
      default:
        monitor.get_entidad(0)->setPosicion(x, y);
        salida = true;
        break;
      }
      if (monitor.get_entidad(0)->isMemberObjetivo(x, y) != -1 and monitor.get_entidad(0)->allLessOneObjetivosAlcanzados())
      {
        cout << "-----> Casilla objetivo alcanzada por el jugador\n";
        // acaba de completar todos los objetivos.
        monitor.get_entidad(0)->setCompletoLosObjetivos();
        if (monitor.getLevel() == 4)
        {
          monitor.put_active_objetivos(1);
          monitor.get_entidad(0)->anularAlcanzados();
          monitor.get_entidad(0)->incrMisiones();
          monitor.get_entidad(0)->incrPuntuacion(1);
          for (unsigned int i = 0; i < monitor.numero_entidades(); i++)
          {
            monitor.get_entidad(i)->setObjetivos(monitor.get_active_objetivos());
          }
        }
        else if (monitor.getLevel() == 0 or monitor.getLevel() == 2)
        {
          // El jugador llegó a la casilla objetivo.
          monitor.finalizarJuego();
          monitor.setMostrarResultados(true);
        }
        else {
          cout << "-----> En este nivel es el COLABORADOR el que debe llegar a la casilla objetivo\n";
        }

      }
    }
    break;

  case actTURN_SR:
    monitor.get_entidad(0)->giro45Dch();
    monitor.girarJugadorDerecha(45);
    monitor.get_entidad(1)->SetActionSent(actIDLE);
    salida = false;
    break;

  case actTURN_L:
    monitor.get_entidad(0)->giroIzq();
    monitor.girarJugadorIzquierda(90);
    // monitor.get_entidad(0)->fixBateria_sig_accion(celdaJ_inicial, accion);
    monitor.get_entidad(1)->SetActionSent(actIDLE);
    salida = true;
    break;

  case actWHEREIS:
    monitor.get_entidad(0)->notify();
    // monitor.get_entidad(0)->fixBateria_sig_accion(celdaJ_inicial, celdaS_inicial, accion);
    monitor.get_entidad(1)->SetActionSent(actIDLE);
    salida = true;
    break;

  case actIDLE:
    if (celdaJ_inicial == 'X')
    { // Casilla Rosa (Recarga)
      monitor.get_entidad(0)->increaseBateria(10);
    }
    // monitor.get_entidad(0)->fixBateria_sig_accion(celdaJ_inicial, accion);
    monitor.get_entidad(1)->SetActionSent(actIDLE);

    salida = true;
    break;

  case act_CLB_WALK:
    if (ColaboradorEnVision)
      monitor.get_entidad(1)->SetActionSent(act_CLB_WALK);
    else
    {
      std::cout << "Envío de orden act_CLB_WALK al colaborador fuera de distancia\n";
    }
    salida = true;
    break;


  case act_CLB_TURN_SR:
    if (ColaboradorEnVision)
      monitor.get_entidad(1)->SetActionSent(act_CLB_TURN_SR);
    else
    {
      std::cout << "Envío de orden act_CLB_TURN_SR al colaborador fuera de distancia\n";
    }
    salida = true;
    break;

  case act_CLB_STOP:
    if (ColaboradorEnVision)
      monitor.get_entidad(1)->SetActionSent(act_CLB_STOP);
    else
    {
      std::cout << "Envío de orden act_CLB_STOP al colaborador fuera de distancia\n";
    }
    salida = true;
    break;

  }

  return salida;
}

bool actuacionNPC(unsigned int entidad, unsigned char celda, Action accion, unsigned int x, unsigned int y)
{
  bool out = false;
  int mix, miy;
  unsigned char celdaRand;

  switch (monitor.get_entidad(entidad)->getSubTipo())
  {
  case aldeano: // Acciones para el aldeano
    switch (accion)
    {
    case actWALK:
      if ((celda != 'P' and celda != 'M') and monitor.getMapa()->casillaOcupada(entidad) == -1)
      {
        monitor.get_entidad(entidad)->setPosicion(x, y);
        out = true;
      }
      break;

    /*case actTURN_R:
      monitor.get_entidad(entidad)->giroDch();
      out = true;
      break;*/

    case actTURN_L:
      monitor.get_entidad(entidad)->giroIzq();
      out = true;
      break;

    case actTURN_SR:
      monitor.get_entidad(entidad)->giro45Dch();
      out = true;
      break;

    /*case actTURN_SL:
      monitor.get_entidad(entidad)->giro45Izq();
      out = true;
      break;*/
    }
    break;

  case colaborador: // Acciones para el colaborador
    switch (accion)
    {
    case act_CLB_STOP:
      out = true;
      break;

    case act_CLB_WALK:
      if (monitor.getMapa()->casillaOcupada(1) == -1) // Casilla destino desocupada
      {
        switch (celda)
        {
        case 'M': // Muro
          monitor.get_entidad(0)->seAostio();
          std::cout << "El colaborador ha chocado con un muro\n";
          break;
        case 'P': // Precipicio
          std::cout << "Se cayó por un precipicio el colaborador\n";
          monitor.get_entidad(1)->resetEntidad();
          monitor.get_entidad(1)->setHitbox(true);
          monitor.finalizarJuego();
          monitor.setMostrarResultados(true);
          break;
        case 'X': // Casilla Rosa (Recarga)
          // monitor.get_entidad(0)->increaseBateria(10);
          monitor.get_entidad(1)->setPosicion(x, y);
          out = true;
          break;
        case 'D': // Casilla Morada (Zapatillas)
          // tomo la zapatilla
          monitor.get_entidad(1)->Cogio_Zapatillas(true);
          // pierdo el bikini
          monitor.get_entidad(1)->Cogio_Bikini(false);
          monitor.get_entidad(1)->setPosicion(x, y);
          out = true;
          break;
        case 'K': // Casilla Amarilla (Bikini)
          // tomo el bikini
          monitor.get_entidad(1)->Cogio_Bikini(true);
          // pierdo las zapatillas
          monitor.get_entidad(1)->Cogio_Zapatillas(false);
          monitor.get_entidad(1)->setPosicion(x, y);
          out = true;
          break;
        case 'G': // Casilla celeste claro (GPS)
          monitor.get_entidad(1)->notify();
          monitor.get_entidad(1)->setPosicion(x, y);
          out = true;
          break;

        case 'B': // Arbol
        case 'A': // Agua
        default:
          monitor.get_entidad(1)->setPosicion(x, y);
          out = true;
          break;
        }
        if (monitor.get_entidad(0)->isMemberObjetivo(x, y) != -1 and monitor.get_entidad(0)->allLessOneObjetivosAlcanzados())
        {
          // acaba de completar todos los objetivos.
          monitor.get_entidad(0)->setCompletoLosObjetivos();
          if (monitor.getLevel() == 4)
          {
            monitor.put_active_objetivos(1);
            monitor.get_entidad(0)->anularAlcanzados();
            monitor.get_entidad(0)->incrMisiones();
            monitor.get_entidad(0)->incrPuntuacion(10);
            monitor.get_entidad(1)->SetActionSent(act_CLB_STOP);
            monitor.get_entidad(1)->setLastAction(act_CLB_STOP);
            monitor.get_entidad(0)->SetLlegoOn();
            monitor.get_entidad(1)->SetLlegoOn();

            for (unsigned int i = 0; i < monitor.numero_entidades(); i++)
            {
              monitor.get_entidad(i)->setObjetivos(monitor.get_active_objetivos());
            }
          }
          else if (monitor.getLevel() == 1 or monitor.getLevel() == 3)
          {
            // El colaborador llegó a la casilla objetivo.
            cout << "-----> Casilla Objetivo alcanzada por el colaborador\n";
            monitor.finalizarJuego();
            monitor.setMostrarResultados(true);
          }
        else {
          cout << "-----> En este nivel es el JUGADOR el que debe llegar a la casilla objetivo\n";
        }
        }
      }

      else
      {
        // Choca contra una entidad
        monitor.get_entidad(1)->seAostio();

        SubTipo agente = monitor.get_entidad(monitor.getMapa()->casillaOcupada(1))->getSubTipo();

        if (agente == aldeano or agente == jugador_ or agente == lobo)
        {
          monitor.get_entidad(0)->perderPV(1);
          switch (agente)
          {
          case aldeano:
            std::cout << "El colaborador reiniciado por chocar contra un aldeano\n";
            break;
          case jugador_:
            std::cout << "El colaborador reiniciado por chocar contra el jugador\n";
            break;
          case lobo:
            std::cout << "El colaborador reiniciado por chocar contra un lobo\n";
            break;
          }
          // Opcion reiniciarse en otro punto del mapa
          do
          {
            mix = aleatorio(monitor.getMapa()->getNFils() - 1);
            miy = aleatorio(monitor.getMapa()->getNCols() - 1);

            celdaRand = monitor.getMapa()->getCelda(mix, miy);
          } while (celdaRand == 'P' or celdaRand == 'M' or monitor.getMapa()->entidadEnCelda(mix, miy) != '_');

          monitor.get_entidad(0)->resetSignal(); // Envía señal de reseto del sonambulo
          monitor.get_entidad(1)->resetEntidad();
          monitor.get_entidad(1)->setPosicion(mix, miy);
          monitor.get_entidad(1)->setOrientacion(static_cast<Orientacion>(aleatorio(7)));
          monitor.get_entidad(1)->setHitbox(true);
          monitor.get_entidad(1)->Cogio_Bikini(false);
          monitor.get_entidad(1)->Cogio_Zapatillas(false);
          monitor.get_entidad(1)->setLastAction(act_CLB_STOP);
          monitor.get_entidad(1)->SetLlegoOn();

          // Opcion simplemente choca contra el lobo
          monitor.get_entidad(0)->perderPV(1);
        }
        out = false;
      }
      break;


    case act_CLB_TURN_SR:
      monitor.get_entidad(1)->giro45Dch();
      out = true;
      break;
    }
    break;
  case lobo: // Acciones para el lobo
    switch (accion)
    {
    case actWALK:
      if ((celda != 'P' and celda != 'M') and monitor.getMapa()->casillaOcupada(entidad) == -1)
      {
        monitor.get_entidad(entidad)->setPosicion(x, y);
        out = true;
      }
      break;

    /*case actTURN_R:
      monitor.get_entidad(entidad)->giroDch();
      out = true;
      break;*/

    case actTURN_L:
      monitor.get_entidad(entidad)->giroIzq();
      out = true;
      break;

    case actTURN_SR:
      monitor.get_entidad(entidad)->giro45Dch();
      out = true;
      break;

    /*case actTURN_SL:
      monitor.get_entidad(entidad)->giro45Izq();
      out = true;
      break;*/

    case actWHEREIS: // Esta accion para un lobo es empujar equivalente a un actPUSH
      std::cout << "Recibido un empujón por un lobo\n";
      bool esta_jugador_delante = monitor.getMapa()->casillaOcupada(entidad) == 0;
      monitor.get_entidad(0)->seAostio();
      if (esta_jugador_delante)
      {
        pair<int, int> casilla = monitor.getMapa()->NCasillasDelante(entidad, 2);
        if (monitor.getMapa()->QuienEnCasilla(casilla.first, casilla.second) == -1 and
            monitor.getMapa()->getCelda(casilla.first, casilla.second) != 'M' and
            monitor.getMapa()->getCelda(casilla.first, casilla.second) != 'P')
          if (aleatorio(1) == 0)
          { // Solo ocurre la mitad de las veces que el lobo lo intenta.
            std::cout << "\tEl empujón ha sido efectivo\n";
            monitor.get_entidad(0)->setPosicion(casilla.first, casilla.second);
            monitor.get_entidad(0)->Increment_Empujones();
            monitor.get_entidad(entidad)->giroIzq();
            monitor.get_entidad(entidad)->giro45Izq();
          }
      }
      out = true;
      break;
    }
  }

  return out;
}

pair<int, int> NextCoordenadas(int f, int c, Orientacion brujula)
{
  switch (brujula)
  {
  case norte:
    f = f - 1;
    c = c;
    break;

  case noreste:
    f = f - 1;
    c = c + 1;
    break;

  case este:
    f = f;
    c = c + 1;
    break;

  case sureste:
    f = f + 1;
    c = c + 1;
    break;

  case sur:
    f = f + 1;
    c = c;
    break;

  case suroeste:
    f = f + 1;
    c = c - 1;
    break;

  case oeste:
    f = f;
    c = c - 1;
    break;

  case noroeste:
    f = f - 1;
    c = c - 1;
    break;
  }
  pair<int, int> coordenadas;
  coordenadas.first = f;
  coordenadas.second = c;
  return coordenadas;
}

bool actuacion(unsigned int entidad, Action accion)
{
  bool out = false;
  unsigned char celdaJ_inicial, celdaJ_fin, celdaS_inicial, celdaS_fin, objetivo;
  unsigned int f, c;

  f = monitor.get_entidad(entidad)->getFil();
  c = monitor.get_entidad(entidad)->getCol();

  celdaJ_inicial = monitor.getMapa()->getCelda(f, c);

  // Calculamos cual es la celda justo frente a la entidad
  pair<unsigned int, unsigned int> coord = NextCoordenadas(f, c, monitor.get_entidad(entidad)->getOrientacion());
  if (accion == actRUN) coord = NextCoordenadas(coord.first, coord.second, monitor.get_entidad(entidad)->getOrientacion());
  f = coord.first;
  c = coord.second;
  celdaJ_fin = monitor.getMapa()->getCelda(f, c);

  // Dependiendo el tipo de la entidad actuamos de una forma u otra
  pair<unsigned int, unsigned int> coordS;
  unsigned int sf, sc;
  switch (monitor.get_entidad(entidad)->getSubTipo())
  {
  case jugador_:
    sf = monitor.get_entidad(1)->getFil();
    sc = monitor.get_entidad(1)->getCol();

    celdaS_inicial = monitor.getMapa()->getCelda(sf, sc);

    // Calculamos cual es la celda justo frente a la entidad
    coordS = NextCoordenadas(sf, sc, monitor.get_entidad(1)->getOrientacion());
    sf = coordS.first;
    sc = coordS.second;
    celdaS_fin = monitor.getMapa()->getCelda(sf, sc);
    out = actuacionJugador(celdaJ_inicial, celdaJ_fin, celdaS_inicial, celdaS_fin, accion, f, c);
    break;

  case colaborador:
    out = actuacionNPC(entidad, celdaJ_fin, accion, f, c);
    break;

  case aldeano:
    out = actuacionNPC(entidad, celdaJ_fin, accion, f, c);
    break;

  case lobo:
    out = actuacionNPC(entidad, celdaJ_fin, accion, f, c);
    break;
  }

  return out;
}

void nucleo_motor_juego(MonitorJuego &monitor, int acc)
{
  Action accion;
  unsigned char celdaRand;

  vector<vector<vector<unsigned char>>> estado;

  estado.clear();

  for (unsigned int i = 0; i < monitor.numero_entidades(); i++)
  {
    estado.push_back(monitor.getMapa()->vision(i));
  }

  if (monitor.get_entidad(0)->ready())
  {

    monitor.init_casillas_especiales(monitor.get_entidad(0)->getFil(), monitor.get_entidad(0)->getCol(), monitor.get_entidad(1)->getFil(), monitor.get_entidad(1)->getCol());

    clock_t t0 = clock();
    accion = monitor.get_entidad(0)->think(acc, estado[0], monitor.getLevel());
    clock_t t1 = clock();

    monitor.get_entidad(0)->addTiempo(t1 - t0);
    monitor.get_entidad(0)->setLastAction(accion);
    actuacion(0, accion);
  }
  else
  {
    monitor.get_entidad(0)->perderPV(1);
  }

  for (unsigned int i = 1; i < monitor.numero_entidades(); i++)
  {
    clock_t t0 = clock();
    accion = monitor.get_entidad(i)->think(-1, estado[i], monitor.getLevel());
    clock_t t1 = clock();


    monitor.get_entidad(i)->addTiempo(t1 - t0);
    monitor.get_entidad(i)->setLastAction(accion);
    actuacion(i, accion);
  }

  for (unsigned int i = 0; i < monitor.numero_entidades(); i++)
  {
    monitor.get_entidad(i)->setVision(monitor.getMapa()->vision(i));
  }

  monitor.get_entidad(0)->decBateria_sig_accion();
  monitor.decPasos();

  if (acc != -1)
  {
    // sleep(monitor.getRetardo() / 10);
  }
}

bool lanzar_motor_juego(int &colisiones, int acc)
{
  bool out = false;

  if (monitor.continuarBelkan())
  {
    if (monitor.jugar())
    {
      if ((monitor.getPasos() != 0) and (!monitor.finJuego()))
      {
        nucleo_motor_juego(monitor, acc);
      }
    }

    if (monitor.mostrarResultados())
    {
      std::cout << "Instantes de simulacion no consumidos: " << monitor.get_entidad(0)->getInstantesPendientes() << endl;
      std::cout << "Tiempo Consumido: " << 1.0 * monitor.get_entidad(0)->getTiempo() / CLOCKS_PER_SEC << endl;
      std::cout << "Nivel Final de Bateria: " << monitor.get_entidad(0)->getBateria() << endl;
      std::cout << "Colisiones: " << monitor.get_entidad(0)->getColisiones() << endl;
      std::cout << "Empujones: " << monitor.get_entidad(0)->getEmpujones() << endl;
      std::cout << "Porcentaje de mapa descubierto: " << monitor.CoincidenciaConElMapa() << endl;
      std::cout << "Objetivos encontrados: (" << monitor.get_entidad(0)->getMisiones() << ") " << monitor.get_entidad(0)->getPuntuacion() << endl;
      monitor.setMostrarResultados(false);

      out = true;
    }
  }
  return out;
}

void lanzar_motor_juego2(MonitorJuego &monitor)
{

  // monitor.get_entidad(0)->setObjetivo(monitor.getObjFil(), monitor.getObjCol());

  while (!monitor.finJuego() && monitor.jugar())
  {
    nucleo_motor_juego(monitor, -1);
  }

  if (monitor.mostrarResultados() and monitor.getLevel() < 2)
  {
    std::cout << "Longitud del camino: " << 2999 - monitor.get_entidad(0)->getInstantesPendientes() << endl;
    monitor.setMostrarResultados(false);
  }
  else if (monitor.mostrarResultados() and monitor.getLevel() < 4)
  {
    std::cout << "Coste de Bateria: " << 3000 - monitor.get_entidad(0)->getBateria() << endl;
    monitor.setMostrarResultados(false);
  }
  else if (monitor.mostrarResultados())
  {
    std::cout << "Instantes de simulacion no consumidos: " << monitor.get_entidad(0)->getInstantesPendientes() << endl;
    std::cout << "Tiempo Consumido: " << 1.0 * monitor.get_entidad(0)->getTiempo() / CLOCKS_PER_SEC << endl;
    std::cout << "Nivel Final de Bateria: " << monitor.get_entidad(0)->getBateria() << endl;
    std::cout << "Colisiones: " << monitor.get_entidad(0)->getColisiones() << endl;
    std::cout << "Empujones: " << monitor.get_entidad(0)->getEmpujones() << endl;
    std::cout << "Porcentaje de mapa descubierto: " << monitor.CoincidenciaConElMapa() << endl;
    std::cout << "Objetivos encontrados: (" << monitor.get_entidad(0)->getMisiones() << ") " << monitor.get_entidad(0)->getPuntuacion() << endl;
    monitor.setMostrarResultados(false);
  }
}
