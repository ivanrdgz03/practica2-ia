#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>

unsigned int ComportamientoJugador::calculoCoste(state &st, const Action &accion)
{
	unsigned int coste = 0;
	if(mapaResultado[st.jugador.f][st.jugador.c] == 'K')
		st.objetos_jugador.bikini = true;
	else if(mapaResultado[st.jugador.f][st.jugador.c] == 'D')
		st.objetos_jugador.zapatillas = true;
	if(mapaResultado[st.colaborador.f][st.colaborador.c] == 'K')
		st.objetos_colaborador.bikini = true;
	else if(mapaResultado[st.colaborador.f][st.colaborador.c] == 'D')
		st.objetos_colaborador.zapatillas = true;
		
	switch (accion)
	{
	case actWALK:
		switch (mapaResultado[st.jugador.f][st.jugador.c])
		{
		case 'T':
			coste += 2;
			break;
		case 'B':
			if (st.objetos_jugador.zapatillas)
				coste += 15;
			else
				coste += 50;
			break;
		case 'A':
			if (st.objetos_jugador.bikini)
				coste += 10;
			else
				coste += 100;
			break;
		default:
			coste += 1;
			break;
		}
		break;
	case act_CLB_WALK:
		switch (mapaResultado[st.colaborador.f][st.colaborador.c])
		{
		case 'T':
			coste += 2;
			break;
		case 'B':
			if (st.objetos_colaborador.zapatillas)
				coste += 15;
			else
				coste += 50;
			break;
		case 'A':
			if (st.objetos_colaborador.bikini)
				coste += 10;
			else
				coste += 100;
			break;
		default:
			coste += 1;
			break;
		}
		break;
	case actRUN:
		switch (mapaResultado[st.jugador.f][st.jugador.c])
		{
		case 'T':
			coste += 3;
			break;
		case 'B':
			if (st.objetos_jugador.zapatillas)
				coste += 25;
			else
				coste += 75;
			break;
		case 'A':
			if (st.objetos_jugador.bikini)
				coste += 15;
			else
				coste += 150;
			break;
		default:
			coste += 1;
			break;
		}
	case actTURN_L:
		switch (mapaResultado[st.jugador.f][st.jugador.c])
		{
		case 'T':
			coste += 2;
			break;
		case 'B':
			if (st.objetos_jugador.zapatillas)
				coste += 1;
			else
				coste += 7;
			break;
		case 'A':
			if (st.objetos_jugador.bikini)
				coste += 5;
			else
				coste += 30;
			break;
		default:
			coste += 1;
			break;
		}
		break;
	case actTURN_SR:
		switch (mapaResultado[st.jugador.f][st.jugador.c])
		{
		case 'B':
			if (st.objetos_jugador.zapatillas)
				coste += 1;
			else
				coste += 5;
			break;
		case 'A':
			if (st.objetos_jugador.bikini)
				coste += 2;
			else
				coste += 10;
			break;
		default:
			coste += 1;
			break;
		}
		break;
	case act_CLB_TURN_SR:
		switch (mapaResultado[st.colaborador.f][st.colaborador.c])
		{
		case 'B':
			if (st.objetos_colaborador.zapatillas)
				coste += 1;
			else
				coste += 5;
			break;
		case 'A':
			if (st.objetos_colaborador.bikini)
				coste += 2;
			else
				coste += 10;
			break;
		default:
			coste += 1;
			break;
		}
		break;
	}
	if(st.ultimaAccionColaborador != actIDLE && st.ultimaAccionColaborador != act_CLB_STOP && accion != act_CLB_STOP && accion != act_CLB_TURN_SR && accion != act_CLB_WALK)
		coste += calculoCoste(st, st.ultimaAccionColaborador);
	return coste;
}

bool ComportamientoJugador::Find(const state &state, const list<node> &lista) const
{
	for (const node &n : lista)
		if (n.st == state)
			return true;
	return false;
}

void ComportamientoJugador::visualizarPlan(const state &st, const list<Action> &plan)
{
	reseteoMatriz(mapaConPlan);
	state cst = st;

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

state ComportamientoJugador::applyAction(const state &st, const Action &accion) const
{
	state newState = st;
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
	if (accion != act_CLB_STOP && accion != act_CLB_TURN_SR && accion != act_CLB_WALK && st.ultimaAccionColaborador != actIDLE)
		newState = applyAction(newState, st.ultimaAccionColaborador);
	if (accion == actRUN && ((applyAction(st, actWALK) == st) || !casillaTransitable(newState.jugador)))
		newState = st;
	else if (accion == actWALK && !casillaTransitable(newState.jugador))
		newState = st;
	else if (accion == act_CLB_WALK && !casillaTransitable(newState.colaborador))
		newState = st;
	if (!casillaTransitable(newState.colaborador))
		newState = st;
	if (newState.jugador.f == newState.colaborador.f && newState.jugador.c == newState.colaborador.c)
		newState = st;
	return newState;
}

bool ComportamientoJugador::busquedaN0(const state &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa)
{
	node currentNode;
	currentNode.st = current_state.st;
	list<node> frontier;
	set<node> explored;
	bool solutionFound = (currentNode.st.jugador.f == final.f && currentNode.st.jugador.c == final.c);

	frontier.push_back(currentNode);

	while (!frontier.empty() && !solutionFound)
	{
		frontier.pop_front();
		explored.insert(currentNode);

		node child_walk = currentNode;
		child_walk.st = applyAction(currentNode.st, actWALK);
		child_walk.secuencia.push_back(actWALK);

		if (child_walk.st.jugador.f == final.f && child_walk.st.jugador.c == final.c)
		{
			currentNode = child_walk;
			solutionFound = true;
		}
		if (!(child_walk == currentNode))
			frontier.push_back(child_walk);

		if (!solutionFound)
		{
			node child_run = currentNode;
			child_run.st = applyAction(currentNode.st, actRUN);
			child_run.secuencia.push_back(actRUN);
			if (child_run.st.jugador.f == final.f && child_run.st.jugador.c == final.c)
			{
				currentNode = child_run;
				solutionFound = true;
			}
			if (!(child_run == currentNode))
				frontier.push_back(child_run);
		}
		if (!solutionFound)
		{
			node child_turn_sr = currentNode, child_turn_l = currentNode;
			child_turn_sr.st = applyAction(currentNode.st, actTURN_SR);
			child_turn_sr.secuencia.push_back(actTURN_SR);
			frontier.push_back(child_turn_sr);

			child_turn_l.st = applyAction(currentNode.st, actTURN_L);
			child_turn_l.secuencia.push_back(actTURN_L);

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
		plan = currentNode.secuencia;
		visualizarPlan(inicio, plan);
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

bool ComportamientoJugador::colaboradorEnSensor(const state &st, const Sensores &sensores) const
{
	for (int i = 1; i < sensores.agentes.size(); i++)
		if (obtener_coordenadas(st.jugador, i).f == st.colaborador.f && obtener_coordenadas(st.jugador, i).c == st.colaborador.c)
			return true;

	return false;
}

bool ComportamientoJugador::busquedaN1(const state &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa, const Sensores &sensores)
{
	node currentNode;
	currentNode.st = current_state.st;
	list<node> frontier;
	set<node> explored;
	bool solutionFound = (currentNode.st.colaborador.f == final.f && currentNode.st.colaborador.c == final.c);

	frontier.push_back(currentNode);

	while (!frontier.empty() && !solutionFound)
	{
		frontier.pop_front();
		explored.insert(currentNode);

		if (colaboradorEnSensor(currentNode.st, sensores))
		{
			node child_colab_walk = currentNode;
			child_colab_walk.st = applyAction(currentNode.st, act_CLB_WALK);
			child_colab_walk.secuencia.push_back(act_CLB_WALK);
			if (child_colab_walk.st.colaborador.f == final.f && child_colab_walk.st.colaborador.c == final.c)
			{
				currentNode = child_colab_walk;
				solutionFound = true;
			}
			if (!(child_colab_walk == currentNode))
				frontier.push_back(child_colab_walk);

			if (!solutionFound)
			{
				node child_colab_turn = currentNode;
				child_colab_turn.st = applyAction(currentNode.st, act_CLB_TURN_SR);
				child_colab_turn.secuencia.push_back(act_CLB_TURN_SR);
				if (!(child_colab_turn == currentNode))
					frontier.push_back(child_colab_turn);

				node child_colab_stop = currentNode;
				child_colab_stop.st = applyAction(currentNode.st, act_CLB_STOP);
				child_colab_stop.secuencia.push_back(act_CLB_STOP);

				frontier.push_back(child_colab_stop);
			}
		}
		if (!solutionFound)
		{
			node child_walk = currentNode;
			child_walk.st = applyAction(currentNode.st, actWALK);
			child_walk.secuencia.push_back(actWALK);

			if (child_walk.st.colaborador.f == final.f && child_walk.st.colaborador.c == final.c)
			{
				currentNode = child_walk;
				solutionFound = true;
			}
			if (!(child_walk == currentNode))
				frontier.push_back(child_walk);

			node child_run = currentNode;
			child_run.st = applyAction(currentNode.st, actRUN);
			child_run.secuencia.push_back(actRUN);
			if (child_run.st.colaborador.f == final.f && child_run.st.colaborador.c == final.c)
			{
				currentNode = child_run;
				solutionFound = true;
			}
			if (!(child_run == currentNode))
				frontier.push_back(child_run);

			node child_turn_sr = currentNode, child_turn_l = currentNode;
			child_turn_sr.st = applyAction(currentNode.st, actTURN_SR);
			child_turn_sr.secuencia.push_back(actTURN_SR);

			frontier.push_back(child_turn_sr);

			child_turn_l.st = applyAction(currentNode.st, actTURN_L);
			child_turn_l.secuencia.push_back(actTURN_L);

			frontier.push_back(child_turn_l);

			node child_idle = currentNode;
			child_idle.st = applyAction(currentNode.st, actIDLE);
			child_idle.secuencia.push_back(actIDLE);

			frontier.push_back(child_idle);
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
		plan = currentNode.secuencia;
		visualizarPlan(inicio, plan);
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
			cout << "Se realiza la planificacion con " << plan.size() << " movimientos" << endl;
			accion = plan.front();
			plan.pop_front();
			hayPlan = true;
		}
	}

	if (plan.empty())
		hayPlan = false;

	return accion;
}

bool ComportamientoJugador::busquedaN2(const state &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa, const Sensores &sensores)
{
	node currentNode;
	currentNode.st = current_state.st;
	priority_queue<node, vector<node>, functor> frontier;
	set<node> explored;
	bool solutionFound = (currentNode.st.jugador.f == final.f && currentNode.st.jugador.c == final.c);

	frontier.push(currentNode);

	while (!frontier.empty() && !solutionFound)
	{
		frontier.pop();
		explored.insert(currentNode);

			node child_walk = currentNode;
			child_walk.coste += calculoCoste(child_walk.st, actWALK);
			child_walk.st = applyAction(currentNode.st, actWALK);
			child_walk.secuencia.push_back(actWALK);

			if (child_walk.st.jugador.f == final.f && child_walk.st.jugador.c == final.c)
			{
				currentNode = child_walk;
				solutionFound = true;
			}
			if (!(child_walk == currentNode))
				frontier.push(child_walk);

			node child_run = currentNode;
			child_run.coste += calculoCoste(child_run.st, actRUN);
			child_run.st = applyAction(currentNode.st, actRUN);
			child_run.secuencia.push_back(actRUN);
			if (child_run.st.jugador.f == final.f && child_run.st.jugador.c == final.c)
			{
				currentNode = child_run;
				solutionFound = true;
			}
			if (!(child_run == currentNode))
				frontier.push(child_run);

			node child_turn_sr = currentNode, child_turn_l = currentNode;
			child_turn_sr.coste += calculoCoste(child_turn_sr.st, actTURN_SR);
			child_turn_sr.st = applyAction(currentNode.st, actTURN_SR);
			child_turn_sr.secuencia.push_back(actTURN_SR);

			frontier.push(child_turn_sr);

			child_turn_l.coste += calculoCoste(child_turn_l.st, actTURN_L);
			child_turn_l.st = applyAction(currentNode.st, actTURN_L);
			child_turn_l.secuencia.push_back(actTURN_L);

			frontier.push(child_turn_l);		

		if (!solutionFound && !frontier.empty())
		{
			currentNode = frontier.top();
			while (!frontier.empty() && (explored.find(currentNode) != explored.end()))
			{
				frontier.pop();
				if (!frontier.empty())
					currentNode = frontier.top();
			}
		}
	}
	if (solutionFound)
	{
		plan = currentNode.secuencia;
		visualizarPlan(inicio, plan);
	}
	return solutionFound;
}

bool ComportamientoJugador::busquedaN3(const state &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa, const Sensores &sensores)
{
	node currentNode;
	currentNode.st = current_state.st;
	priority_queue<node, vector<node>, functor> frontier;
	set<node> explored;
	bool solutionFound = (currentNode.st.colaborador.f == final.f && currentNode.st.colaborador.c == final.c);

	frontier.push(currentNode);

	while (!frontier.empty() && !solutionFound)
	{
		frontier.pop();
		explored.insert(currentNode);

		if (colaboradorEnSensor(currentNode.st, sensores))
		{
			node child_colab_walk = currentNode;
			child_colab_walk.coste += calculoCoste(child_colab_walk.st, act_CLB_WALK);
			child_colab_walk.st = applyAction(currentNode.st, act_CLB_WALK);
			child_colab_walk.secuencia.push_back(act_CLB_WALK);
			if (child_colab_walk.st.colaborador.f == final.f && child_colab_walk.st.colaborador.c == final.c)
			{
				currentNode = child_colab_walk;
				solutionFound = true;
			}
			if (!(child_colab_walk == currentNode))
				frontier.push(child_colab_walk);

			if (!solutionFound)
			{
				node child_colab_turn = currentNode;
				child_colab_turn.coste += calculoCoste(child_colab_turn.st, act_CLB_TURN_SR);
				child_colab_turn.st = applyAction(currentNode.st, act_CLB_TURN_SR);
				child_colab_turn.secuencia.push_back(act_CLB_TURN_SR);
				if (!(child_colab_turn == currentNode))
					frontier.push(child_colab_turn);

				node child_colab_stop = currentNode;
				child_colab_stop.st = applyAction(currentNode.st, act_CLB_STOP);
				child_colab_stop.secuencia.push_back(act_CLB_STOP);

				frontier.push(child_colab_stop);
			}
		}
		if (!solutionFound)
		{
			node child_walk = currentNode;
			child_walk.coste += calculoCoste(child_walk.st, actWALK);
			child_walk.st = applyAction(currentNode.st, actWALK);
			child_walk.secuencia.push_back(actWALK);

			if (child_walk.st.colaborador.f == final.f && child_walk.st.colaborador.c == final.c)
			{
				currentNode = child_walk;
				solutionFound = true;
			}
			if (!(child_walk == currentNode))
				frontier.push(child_walk);

			node child_run = currentNode;
			child_run.coste += calculoCoste(child_run.st, actRUN);
			child_run.st = applyAction(currentNode.st, actRUN);
			child_run.secuencia.push_back(actRUN);
			if (child_run.st.colaborador.f == final.f && child_run.st.colaborador.c == final.c)
			{
				currentNode = child_run;
				solutionFound = true;
			}
			if (!(child_run == currentNode))
				frontier.push(child_run);

			node child_turn_sr = currentNode, child_turn_l = currentNode;
			child_turn_sr.coste += calculoCoste(child_turn_sr.st, actTURN_SR);
			child_turn_sr.st = applyAction(currentNode.st, actTURN_SR);
			child_turn_sr.secuencia.push_back(actTURN_SR);

			frontier.push(child_turn_sr);

			child_turn_l.coste += calculoCoste(child_turn_l.st, actTURN_L);
			child_turn_l.st = applyAction(currentNode.st, actTURN_L);
			child_turn_l.secuencia.push_back(actTURN_L);

			frontier.push(child_turn_l);

			node child_idle = currentNode;
			child_idle.st = applyAction(currentNode.st, actIDLE);
			child_idle.secuencia.push_back(actIDLE);

			frontier.push(child_idle);
		}

		if (!solutionFound && !frontier.empty())
		{
			currentNode = frontier.top();
			while (!frontier.empty() && (explored.find(currentNode) != explored.end()))
			{
				frontier.pop();
				if (!frontier.empty())
					currentNode = frontier.top();
			}
		}
	}
	if (solutionFound)
	{
		plan = currentNode.secuencia;
		visualizarPlan(inicio, plan);
	}
	return solutionFound;
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
			cout << "Se realiza la planificacion con " << plan.size() << " movimientos" << endl;
			accion = plan.front();
			plan.pop_front();
			hayPlan = true;
		}
	}

	if (plan.empty())
		hayPlan = false;

	return accion;
}

Action ComportamientoJugador::nivel2(const Sensores &sensores)
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
			cout << "Se realiza la planificacion con " << plan.size() << " movimientos" << endl;
			accion = plan.front();
			plan.pop_front();
			hayPlan = true;
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
			accion = nivel2(sensores);
			break;
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
