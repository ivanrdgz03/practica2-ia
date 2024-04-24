#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>

bool ComportamientoJugador::Find(const stateN0 &state, const list<nodeN0> &lista) const
{
	for (const nodeN0 &n : lista)
		if (n.st == state)
			return true;
	return false;
}

void ComportamientoJugador::visualizarPlan(const stateN0 &st, const list<Action> &plan)
{
	reseteoMatriz(mapaConPlan);
	stateN0 cst = st;

	auto it = plan.begin();
	while (it != plan.end())
	{
		if ((*it != act_CLB_WALK) && (*it != act_CLB_TURN_SR) && (*it != act_CLB_STOP))
			switch (cst.ultimaAccionColaborador)
			{
			case act_CLB_WALK:
				cst.colaborador = applyAction(cst, act_CLB_WALK).colaborador;
				mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
				break;
			case act_CLB_TURN_SR:
				cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
				break;
			}
		switch (*it)
		{
		case actRUN:
			cst.jugador = applyAction(cst, actWALK).jugador;
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 3;
			cst.jugador = applyAction(cst, actWALK).jugador;
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actWALK:
			cst.jugador = applyAction(cst, actWALK).jugador;
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actTURN_SR:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 1) % 8);
			break;
		case actTURN_L:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
			break;
		case act_CLB_WALK:
			cst.colaborador = applyAction(cst, act_CLB_WALK).colaborador;
			cst.ultimaAccionColaborador = act_CLB_WALK;
			break;
		case act_CLB_TURN_SR:
			cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
			cst.ultimaAccionColaborador = act_CLB_TURN_SR;
			break;
		case act_CLB_STOP:
			cst.ultimaAccionColaborador = act_CLB_STOP;
			break;
		}
		it++;
	}
}

void ComportamientoJugador::reseteoMatriz(vector<vector<unsigned char>> &matriz)
{
	for (vector<unsigned char> &fila : matriz)
		for (unsigned char &valor : fila)
			valor = 0;
}

bool ComportamientoJugador::casillaTransitable(const ubicacion &u) const
{
	return mapaResultado[u.f][u.c] != 'M' && mapaResultado[u.f][u.c] != 'P';
}

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
			newState.jugador.f--;
			break;
		case 2:
			newState.jugador.c++;
			break;
		case 3:
			newState.jugador.c++;
			newState.jugador.f++;
			break;
		case 4:
			newState.jugador.f++;
			break;
		case 5:
			newState.jugador.c--;
			newState.jugador.f++;
			break;
		case 6:
			newState.jugador.c--;
			break;
		case 7:
			newState.jugador.c--;
			newState.jugador.f--;
			break;
		}
		break;
	case actRUN:
		switch (newState.jugador.brujula)
		{
		case 0:
			newState.jugador.f -= 2;
			break;
		case 1:
			newState.jugador.c += 2;
			newState.jugador.f -= 2;
			break;
		case 2:
			newState.jugador.c += 2;
			break;
		case 3:
			newState.jugador.c += 2;
			newState.jugador.f += 2;
			break;
		case 4:
			newState.jugador.f += 2;
			break;
		case 5:
			newState.jugador.c -= 2;
			newState.jugador.f += 2;
			break;
		case 6:
			newState.jugador.c -= 2;
			break;
		case 7:
			newState.jugador.c -= 2;
			newState.jugador.f -= 2;
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
	case act_CLB_TURN_SR:
		newState.ultimaAccionColaborador = act_CLB_TURN_SR;
		newState.colaborador.brujula = (Orientacion)((newState.colaborador.brujula + 1) % 8);
		break;
	case act_CLB_STOP:
		newState.ultimaAccionColaborador = act_CLB_STOP;
		break;
	case act_CLB_WALK:
		newState.ultimaAccionColaborador = act_CLB_WALK;
		switch (newState.colaborador.brujula)
		{
		case 0:
			newState.colaborador.f--;
			break;
		case 1:
			newState.colaborador.c++;
			newState.colaborador.f--;
			break;
		case 2:
			newState.colaborador.c++;
			break;
		case 3:
			newState.colaborador.c++;
			newState.colaborador.f++;
			break;
		case 4:
			newState.colaborador.f++;
			break;
		case 5:
			newState.colaborador.c--;
			newState.colaborador.f++;
			break;
		case 6:
			newState.colaborador.c--;
			break;
		case 7:
			newState.colaborador.c--;
			newState.colaborador.f--;
			break;
		}
		break;
	default:
		throw("Acción no reconocida");
	}
	if (accion != act_CLB_STOP && accion != act_CLB_TURN_SR && accion != act_CLB_WALK && state.ultimaAccionColaborador != actIDLE)
		newState = applyAction(newState, state.ultimaAccionColaborador);
	if (accion == actRUN && ((applyAction(state, actWALK) == state) || !casillaTransitable(newState.jugador)))
		newState = state;
	else if (accion == actWALK && !casillaTransitable(newState.jugador))
		newState = state;
	else if (accion == act_CLB_WALK && !casillaTransitable(newState.colaborador))
		newState = state;
	if (!casillaTransitable(newState.colaborador))
		newState = state;
	return newState;
}

bool ComportamientoJugador::busquedaN0(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
{
	nodeN0 currentNode;
	currentNode.st = current_state.st;
	list<nodeN0> frontier;
	set<nodeN0> explored;
	bool solutionFound = (currentNode.st.jugador.f == final.f && currentNode.st.jugador.c == final.c);

	frontier.push_back(currentNode);

	while (!frontier.empty() && !solutionFound)
	{
		frontier.pop_front();
		explored.insert(currentNode);

		nodeN0 child_walk = currentNode;
		child_walk.st = applyAction(currentNode.st, actWALK);
		child_walk.secuencia.push_back(actWALK);

		if (child_walk.st.jugador.f == final.f && child_walk.st.jugador.c == final.c)
		{
			currentNode = child_walk;
			solutionFound = true;
		}

		else if (explored.find(child_walk) == explored.end())
			frontier.push_back(child_walk);

		if (!solutionFound)
		{
			nodeN0 child_run = currentNode;
			child_run.st = applyAction(currentNode.st, actRUN);
			child_run.secuencia.push_back(actRUN);
			if (child_run.st.jugador.f == final.f && child_run.st.jugador.c == final.c)
			{
				currentNode = child_run;
				solutionFound = true;
			}
			else if (explored.find(child_run) == explored.end())
				frontier.push_back(child_run);
		}
		if (!solutionFound)
		{
			nodeN0 child_turn_sr = currentNode, child_turn_l = currentNode;
			child_turn_sr.st = applyAction(currentNode.st, actTURN_SR);
			child_turn_sr.secuencia.push_back(actTURN_SR);

			if (explored.find(child_turn_sr) == explored.end())
				frontier.push_back(child_turn_sr);

			child_turn_l.st = applyAction(currentNode.st, actTURN_L);
			child_turn_l.secuencia.push_back(actTURN_L);

			if (explored.find(child_turn_l) == explored.end())
				frontier.push_back(child_turn_l);
		}
		if (!solutionFound && !frontier.empty())
		{
			currentNode = frontier.front();
			while (!frontier.empty() && (explored.find(currentNode) != explored.end()))
			{
				frontier.pop_front();
				if (!frontier.empty())
					currentNode = frontier.front();
			}
		}
	}
	if (solutionFound)
	{
		visualizarPlan(inicio, plan);
		plan = currentNode.secuencia;
	}
	return solutionFound;
}

ubicacion ComportamientoJugador::obtener_coordenadas(const ubicacion &u, const unsigned int &pos) const
{
	ubicacion salida;
	switch (u.brujula)
	{
	case norte:
		salida = {u.f + sensores_norte[pos].first, u.c + sensores_norte[pos].second, u.brujula};
		break;
	case sur:
		salida = {u.f - sensores_norte[pos].first, u.c - sensores_norte[pos].second, u.brujula};
		break;
	case este:
		salida = {u.f + sensores_norte[pos].second, u.c - sensores_norte[pos].first, u.brujula};
		break;
	case oeste:
		salida = {u.f - sensores_norte[pos].second, u.c + sensores_norte[pos].first, u.brujula};
		break;
	case noreste:
		salida = {u.f + sensores_noreste[pos].first,
				  u.c + sensores_noreste[pos].second, u.brujula};
		break;
	case noroeste:
		salida = {u.f - sensores_noreste[pos].second,
				  u.c + sensores_noreste[pos].first, u.brujula};
		break;
	case sureste:
		salida = {u.f + sensores_noreste[pos].second,
				  u.c - sensores_noreste[pos].first, u.brujula};
		break;
	case suroeste:
		salida = {u.f - sensores_noreste[pos].first,
				  u.c - sensores_noreste[pos].second, u.brujula};
		break;
	}
	return salida;
}

bool ComportamientoJugador::colaboradorEnSensor(const stateN0 &st, const Sensores &sensores) const
{
	for (int i = 1; i < sensores.agentes.size(); i++)
		if (obtener_coordenadas(st.jugador, i).f == st.colaborador.f && obtener_coordenadas(st.jugador, i).c == st.colaborador.c)
			return true;

	return false;
}

bool ComportamientoJugador::busquedaN1(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa, const Sensores &sensores)
{
	nodeN0 currentNode;
	currentNode.st = current_state.st;
	list<nodeN0> frontier;
	set<nodeN0> explored;
	bool solutionFound = (currentNode.st.colaborador.f == final.f && currentNode.st.colaborador.c == final.c);

	frontier.push_back(currentNode);

	while (!frontier.empty() && !solutionFound)
	{
		frontier.pop_front();
		explored.insert(currentNode);

		if (colaboradorEnSensor(currentNode.st, sensores))
		{
			nodeN0 child_colab_walk = currentNode;
			child_colab_walk.st = applyAction(currentNode.st, act_CLB_WALK);
			child_colab_walk.secuencia.push_back(act_CLB_WALK);
			if (child_colab_walk.st.colaborador.f == final.f && child_colab_walk.st.colaborador.c == final.c)
			{
				currentNode = child_colab_walk;
				solutionFound = true;
			}
			else if (explored.find(child_colab_walk) == explored.end())
				frontier.push_back(child_colab_walk);

			if (!solutionFound)
			{
				nodeN0 child_colab_turn = currentNode;
				child_colab_turn.st = applyAction(currentNode.st, act_CLB_TURN_SR);
				child_colab_turn.secuencia.push_back(act_CLB_TURN_SR);

				if (explored.find(child_colab_turn) == explored.end())
					frontier.push_back(child_colab_turn);

				nodeN0 child_colab_stop = currentNode;
				child_colab_stop.st = applyAction(currentNode.st, act_CLB_STOP);
				child_colab_stop.secuencia.push_back(act_CLB_STOP);

				if (explored.find(child_colab_stop) == explored.end())
					frontier.push_back(child_colab_stop);
			}
		}
		if (!solutionFound)
		{
			nodeN0 child_walk = currentNode;
			child_walk.st = applyAction(currentNode.st, actWALK);
			child_walk.secuencia.push_back(actWALK);

			if (child_walk.st.colaborador.f == final.f && child_walk.st.colaborador.c == final.c)
			{
				currentNode = child_walk;
				solutionFound = true;
			}

			else if (explored.find(child_walk) == explored.end())
				frontier.push_back(child_walk);
		
			nodeN0 child_run = currentNode;
			child_run.st = applyAction(currentNode.st, actRUN);
			child_run.secuencia.push_back(actRUN);
			if (child_run.st.colaborador.f == final.f && child_run.st.colaborador.c == final.c)
			{
				currentNode = child_run;
				solutionFound = true;
			}
			else if (explored.find(child_run) == explored.end())
				frontier.push_back(child_run);
		
			nodeN0 child_turn_sr = currentNode, child_turn_l = currentNode;
			child_turn_sr.st = applyAction(currentNode.st, actTURN_SR);
			child_turn_sr.secuencia.push_back(actTURN_SR);

			if (explored.find(child_turn_sr) == explored.end())
				frontier.push_back(child_turn_sr);

			child_turn_l.st = applyAction(currentNode.st, actTURN_L);
			child_turn_l.secuencia.push_back(actTURN_L);

			if (explored.find(child_turn_l) == explored.end())
				frontier.push_back(child_turn_l);
		}

		if (!solutionFound && !frontier.empty())
		{
			currentNode = frontier.front();
			while (!frontier.empty() && (explored.find(currentNode) != explored.end()))
			{
				frontier.pop_front();
				if (!frontier.empty())
					currentNode = frontier.front();
			}
		}
	}
	if (solutionFound)
	{
		visualizarPlan(inicio, plan);
		plan = currentNode.secuencia;
	}
	return solutionFound;
}

Action ComportamientoJugador::nivel1(const Sensores &sensores)
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
	else if (busquedaN1(current_state.st, {sensores.destinoF, sensores.destinoC}, mapaResultado, sensores))
	{
		if (!plan.empty())
		{
			accion = plan.front();
			plan.pop_front();
		}
	}
	else
		cout << "no se encontró el camino";

	if (plan.empty())
		hayPlan = false;

	return accion;
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
	else if (busquedaN0(current_state.st, {sensores.destinoF, sensores.destinoC}, mapaResultado))
	{
		if (!plan.empty())
		{
			accion = plan.front();
			plan.pop_front();
		}
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
			accion = nivel1(sensores);
			break;
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
