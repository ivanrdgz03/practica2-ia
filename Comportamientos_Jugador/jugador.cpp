#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>

stateN0 ComportamientoJugador::applyAction(const stateN0 &state, const Action &accion) const
{
	stateN0 newState = state;



	switch (accion)
	{
	case actWALK:
		switch (newState.jugador.brujula)
		{
		case 0:
			newState.jugador.f--;
			break;
		case 1:
			newState.jugador.c++;
			break;
		case 2:
			newState.jugador.f++;
			break;
		case 3:
			newState.jugador.c--;
			break;
		}
		break;
	case actTURN_SR:
		newState.jugador.brujula = (Orientacion)((newState.jugador.brujula + 1) % 8);
		break;
	case actTURN_L:
		newState.jugador.brujula = (Orientacion)((newState.jugador.brujula + 6) % 8);
		break;
	case actIDLE:
		break;
	default:
		throw("Acción no reconocida");
	}

	return newState;
}

bool ComportamientoJugador::busquedaN0(const Sensores &sensores)
{
	
}

Action ComportamientoJugador::nivel0(const Sensores &sensores)
{
	Action accion = actIDLE;

	current_state.st.jugador.f = sensores.posF;
	current_state.st.jugador.c = sensores.posC;
	current_state.st.jugador.brujula = sensores.sentido;

	current_state.st.colaborador.f = sensores.CLBposF;
	current_state.st.colaborador.c = sensores.CLBposC;
	current_state.st.colaborador.brujula = sensores.CLBsentido;

	if (hayPlan && !plan.empty())
	{
		accion = plan.front();
		plan.pop_front();
	}
	else if (busquedaN0(sensores))
	{
		accion = plan.front();
		plan.pop_front();
	}

	if (plan.empty())
		hayPlan = false;

	return accion;
}

Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;

	if (sensores.nivel != 4)
		switch (sensores.nivel)
		{
		case 0:
			accion = nivel0(sensores);
			break;
		case 1:
		case 2:
		case 3:
		default:
			throw("Nivel no implementado");
			break;
		}
	else
	{
		throw("Nivel no implementado");
	}
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}
