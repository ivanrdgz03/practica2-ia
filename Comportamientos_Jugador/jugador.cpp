#include "../Comportamientos_Jugador/jugador.hpp"

ubicacion ComportamientoJugador::NextCasilla(const ubicacion &pos) const
{
	ubicacion salida = pos;
	switch (pos.brujula)
	{
	case norte:
		salida.f = pos.f - 1;
		break;
	case noreste:
		salida.f = pos.f - 1;
		salida.c = pos.c + 1;
		break;
	case este:
		salida.c = pos.c + 1;
		break;
	case sureste:
		salida.f = pos.f + 1;
		salida.c = pos.c + 1;
		break;
	case sur:
		salida.f = pos.f + 1;
		break;
	case suroeste:
		salida.f = pos.f + 1;
		salida.c = pos.c - 1;
		break;
	case oeste:
		salida.c = pos.c - 1;
		break;
	case noroeste:
		salida.f = pos.f - 1;
		salida.c = pos.c - 1;
		break;
	default:
		break;
	}

	return salida;
}

unsigned int ComportamientoJugador::calculoCoste(const state &st, const Action &accion) const
{
	unsigned int coste = 0;
	const unsigned char terrenoJugador = mapaResultado[st.jugador.f][st.jugador.c];
	const unsigned char terrenoColaborador = mapaResultado[st.colaborador.f][st.colaborador.c];
	switch (accion)
	{
	case actWALK:
		switch (terrenoJugador)
		{
		case 'T':
			coste += 2;
			break;
		case 'B':
			coste += st.objetos_jugador.zapatillas ? 15 : 50;
			break;
		case 'A':
			coste += st.objetos_jugador.bikini ? 10 : 100;
			break;
		default:
			coste += 1;
			break;
		}
		break;
	case act_CLB_WALK:
		switch (terrenoColaborador)
		{
		case 'T':
			coste += 2;
			break;
		case 'B':
			coste += st.objetos_colaborador.zapatillas ? 15 : 50;
			break;
		case 'A':
			coste += st.objetos_colaborador.bikini ? 10 : 100;
			break;
		default:
			coste += 1;
			break;
		}
		break;
	case actRUN:
		switch (terrenoJugador)
		{
		case 'T':
			coste += 3;
			break;
		case 'B':
			coste += st.objetos_jugador.zapatillas ? 25 : 75;
			break;
		case 'A':
			coste += st.objetos_jugador.bikini ? 15 : 150;
			break;
		default:
			coste += 1;
			break;
		}
		break;
	case actTURN_L:
		switch (terrenoJugador)
		{
		case 'T':
			coste += 2;
			break;
		case 'B':
			coste += st.objetos_jugador.zapatillas ? 1 : 7;
			break;
		case 'A':
			coste += st.objetos_jugador.bikini ? 5 : 30;
			break;
		default:
			coste += 1;
			break;
		}
		break;
	case actTURN_SR:
		switch (terrenoJugador)
		{
		case 'B':
			coste += st.objetos_jugador.zapatillas ? 1 : 5;
			break;
		case 'A':
			coste += st.objetos_jugador.bikini ? 2 : 10;
			break;
		default:
			coste += 1;
			break;
		}
		break;
	case act_CLB_TURN_SR:
		switch (terrenoColaborador)
		{
		case 'B':
			coste += st.objetos_colaborador.zapatillas ? 1 : 5;
			break;
		case 'A':
			coste += st.objetos_colaborador.bikini ? 2 : 10;
			break;
		default:
			coste += 1;
			break;
		}
		break;
	}
	if (st.ultimaAccionColaborador != actIDLE && st.ultimaAccionColaborador != act_CLB_STOP && accion != act_CLB_STOP && accion != act_CLB_TURN_SR && accion != act_CLB_WALK)
		coste += calculoCoste(st, st.ultimaAccionColaborador);

	return coste;
}

unsigned int ComportamientoJugador::calcularHeuristica(const ubicacion &actual, const ubicacion &destino, const Sensores &sensores) const
{
	return max(abs(actual.f - destino.f), abs(actual.c - destino.c));
}

unsigned int ComportamientoJugador::calculoCoste(const stateJugador &st, const Action &accion) const
{
	unsigned int coste = 0;
	const unsigned char terreno = mapaResultado[st.jugador.f][st.jugador.c];
	switch (accion)
	{
	case actWALK:
		switch (terreno)
		{
		case 'T':
			coste += 2;
			break;
		case 'B':
			coste += st.objetos_jugador.zapatillas ? 15 : 50;
			break;
		case 'A':
			coste += st.objetos_jugador.bikini ? 10 : 100;
			break;
		default:
			coste += 1;
			break;
		}
		break;
	case actRUN:
		switch (terreno)
		{
		case 'T':
			coste += 3;
			break;
		case 'B':
			coste += st.objetos_jugador.zapatillas ? 25 : 75;
			break;
		case 'A':
			coste += st.objetos_jugador.bikini ? 15 : 150;
			break;
		default:
			coste += 1;
			break;
		}
		break;
	case actTURN_L:
		switch (terreno)
		{
		case 'T':
			coste += 2;
			break;
		case 'B':
			coste += st.objetos_jugador.zapatillas ? 1 : 7;
			break;
		case 'A':
			coste += st.objetos_jugador.bikini ? 5 : 30;
			break;
		default:
			coste += 1;
			break;
		}
		break;
	case actTURN_SR:
		switch (terreno)
		{
		case 'B':
			coste += st.objetos_jugador.zapatillas ? 1 : 5;
			break;
		case 'A':
			coste += st.objetos_jugador.bikini ? 2 : 10;
			break;
		default:
			coste += 1;
			break;
		}
		break;
	}

	return coste;
}

unsigned int ComportamientoJugador::calculoCoste(const stateJugador4 &st, const Action &accion) const
{
	unsigned int coste = 0;
	const unsigned char terreno = mapaResultado[st.jugador.f][st.jugador.c];
	switch (accion)
	{
	case actWALK:
		switch (terreno)
		{
		case 'T':
			coste += 2;
			break;
		case 'B':
			coste += st.objetos_jugador.zapatillas ? 15 : 50;
			break;
		case 'A':
			coste += st.objetos_jugador.bikini ? 10 : 100;
			break;
		default:
			coste += 1;
			break;
		}
		break;
	case actRUN:
		switch (terreno)
		{
		case 'T':
			coste += 3;
			break;
		case 'B':
			coste += st.objetos_jugador.zapatillas ? 25 : 75;
			break;
		case 'A':
			coste += st.objetos_jugador.bikini ? 15 : 150;
			break;
		default:
			coste += 1;
			break;
		}
		break;
	case actTURN_L:
		switch (terreno)
		{
		case 'T':
			coste += 2;
			break;
		case 'B':
			coste += st.objetos_jugador.zapatillas ? 1 : 7;
			break;
		case 'A':
			coste += st.objetos_jugador.bikini ? 5 : 30;
			break;
		default:
			coste += 1;
			break;
		}
		break;
	case actTURN_SR:
		switch (terreno)
		{
		case 'B':
			coste += st.objetos_jugador.zapatillas ? 1 : 5;
			break;
		case 'A':
			coste += st.objetos_jugador.bikini ? 2 : 10;
			break;
		default:
			coste += 1;
			break;
		}
		break;
	}

	return coste;
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
				cst.colaborador = NextCasilla(cst.colaborador);
				mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
				break;
			case act_CLB_TURN_SR:
				cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
				break;
			}
		switch (*it)
		{
		case actRUN:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 3;
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actWALK:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actTURN_SR:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 1) % 8);
			break;
		case actTURN_L:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
			break;
		case act_CLB_WALK:
			cst.colaborador = NextCasilla(cst.colaborador);
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

void ComportamientoJugador::visualizarPlan(const stateJugador &st, const list<Action> &plan, const Sensores &sensores)
{
	reseteoMatriz(mapaConPlan);
	stateJugador cst = st;
	ubicacion aux;

	auto it = plan.begin();
	while (it != plan.end())
	{
		switch (*it)
		{
		case actRUN:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 3;
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actWALK:
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		case actTURN_SR:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 1) % 8);
			break;
		case actTURN_L:
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
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
	if (u.f < 0 || u.f >= mapaResultado.size() || u.c < 0 || u.c >= mapaResultado[0].size())
		return false;
	return ((mapaResultado[u.f][u.c] != 'M') && (mapaResultado[u.f][u.c] != 'P'));
}

state ComportamientoJugador::applyAction(const state &st, const Action &accion) const
{
	state newState = st;
	ubicacion aux, aux2;

	switch (accion)
	{
	case actWALK:
		aux = NextCasilla(newState.jugador);
		if (casillaTransitable(aux) && (aux.f != newState.colaborador.f || aux.c != newState.colaborador.c))
			newState.jugador = aux;
		else
			return st;
		break;
	case actRUN:
		aux = NextCasilla(newState.jugador);
		aux2 = NextCasilla(aux);
		if (casillaTransitable(aux) && casillaTransitable(aux2) && (aux.f != newState.colaborador.f || aux.c != newState.colaborador.c) && (aux2.f != newState.colaborador.f || aux2.c != newState.colaborador.c))
			newState.jugador = aux2;
		else
			return st;
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
		aux = NextCasilla(newState.colaborador);
		if (casillaTransitable(aux) && (aux.f != newState.jugador.f || aux.c != newState.jugador.c))
		{
			newState.colaborador = aux;
			newState.ultimaAccionColaborador = act_CLB_WALK;
		}
		else
			return st;
		break;
	default:
		throw("Acción no reconocida");
	}
	if (accion != act_CLB_STOP && accion != act_CLB_TURN_SR && accion != act_CLB_WALK && (!(newState == st) || accion == actIDLE) && st.ultimaAccionColaborador != act_CLB_STOP)
	{
		state auxState = applyAction(newState, st.ultimaAccionColaborador);
		if (!(newState == auxState))
		{
			newState = auxState;
		}
		else
		{
			return st;
		}
	}

	if (mapaResultado[newState.jugador.f][newState.jugador.c] == 'K')
	{
		newState.objetos_jugador.bikini = true;
		newState.objetos_jugador.zapatillas = false;
	}
	else if (mapaResultado[newState.jugador.f][newState.jugador.c] == 'D')
	{
		newState.objetos_jugador.zapatillas = true;
		newState.objetos_jugador.bikini = false;
	}
	if (mapaResultado[newState.colaborador.f][newState.colaborador.c] == 'K')
	{
		newState.objetos_colaborador.bikini = true;
		newState.objetos_colaborador.zapatillas = false;
	}
	else if (mapaResultado[newState.colaborador.f][newState.colaborador.c] == 'D')
	{
		newState.objetos_colaborador.zapatillas = true;
		newState.objetos_colaborador.bikini = false;
	}

	return newState;
}

stateJugador ComportamientoJugador::applyAction(const stateJugador &st, const Action &accion, const Sensores &sensores) const
{
	stateJugador newState = st;
	ubicacion aux, aux2;
	switch (accion)
	{
	case actWALK:
		aux = NextCasilla(newState.jugador);
		if (casillaTransitable(aux) && (aux.f != sensores.CLBposF || aux.c != sensores.CLBposC))
			newState.jugador = aux;
		break;
	case actRUN:
		aux = NextCasilla(newState.jugador);
		aux2 = NextCasilla(aux);
		if (casillaTransitable(aux) && casillaTransitable(aux2) && (aux.f != sensores.CLBposF || aux.c != sensores.CLBposC) && (aux2.f != sensores.CLBposF || aux2.c != sensores.CLBposC))
			newState.jugador = aux2;
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

	if (mapaResultado[newState.jugador.f][newState.jugador.c] == 'K')
	{
		newState.objetos_jugador.bikini = true;
		newState.objetos_jugador.zapatillas = false;
	}
	else if (mapaResultado[st.jugador.f][st.jugador.c] == 'D')
	{
		newState.objetos_jugador.zapatillas = true;
		newState.objetos_jugador.bikini = false;
	}

	return newState;
}

stateJugador4 ComportamientoJugador::applyAction(const stateJugador4 &st, const Action &accion, const ubicacion &colab) const
{
	stateJugador4 newState = st;
	ubicacion aux, aux2;
	if (colab.f != 12 && colab.c != 12)
		cout << "hola";
	switch (accion)
	{
	case actWALK:
		aux = NextCasilla(newState.jugador);
		if (casillaTransitable(aux) && (aux.f != colab.f || aux.c != colab.c))
			newState.jugador = aux;
		break;
	case actRUN:
		aux = NextCasilla(newState.jugador);
		aux2 = NextCasilla(aux);
		if (casillaTransitable(aux) && casillaTransitable(aux2) && (aux.f != colab.f || aux.c != colab.c) && (aux2.f != colab.f || aux2.c != colab.c))
			newState.jugador = aux2;
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

	if (mapaResultado[newState.jugador.f][newState.jugador.c] == 'K')
	{
		newState.objetos_jugador.bikini = true;
		newState.objetos_jugador.zapatillas = false;
	}
	else if (mapaResultado[st.jugador.f][st.jugador.c] == 'D')
	{
		newState.objetos_jugador.zapatillas = true;
		newState.objetos_jugador.bikini = false;
	}

	return newState;
}

bool ComportamientoJugador::busquedaN0(const stateJugador &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa, const Sensores &sensores)
{
	nodeJugador currentNode;
	currentNode.st = inicio;
	currentNode.coste = 0;
	list<nodeJugador> frontier;
	set<stateJugador> explored;
	bool solutionFound = (currentNode.st.jugador.f == final.f && currentNode.st.jugador.c == final.c);

	frontier.push_back(currentNode);

	while (!frontier.empty() && !solutionFound)
	{
		frontier.pop_front();
		explored.insert(currentNode.st);

		nodeJugador child_walk = currentNode;
		child_walk.st = applyAction(currentNode.st, actWALK, sensores);

		if (child_walk.st.jugador.f == final.f && child_walk.st.jugador.c == final.c)
		{
			child_walk.secuencia.push_back(actWALK);
			currentNode = child_walk;
			solutionFound = true;
		}
		else if (!(child_walk == currentNode) && (explored.find(child_walk.st) == explored.end()))
		{
			child_walk.secuencia.push_back(actWALK);
			frontier.push_back(child_walk);
		}

		if (!solutionFound)
		{
			nodeJugador child_run = currentNode;
			child_run.st = applyAction(currentNode.st, actRUN, sensores);
			if (child_run.st.jugador.f == final.f && child_run.st.jugador.c == final.c)
			{
				child_run.secuencia.push_back(actRUN);
				currentNode = child_run;
				solutionFound = true;
			}
			if (!(child_run == currentNode) && (explored.find(child_run.st) == explored.end()))
			{
				child_run.secuencia.push_back(actRUN);
				frontier.push_back(child_run);
			}
		}
		if (!solutionFound)
		{
			nodeJugador child_turn_sr = currentNode, child_turn_l = currentNode;
			child_turn_sr.st = applyAction(currentNode.st, actTURN_SR, sensores);
			if (explored.find(child_turn_sr.st) == explored.end())
			{
				child_turn_sr.secuencia.push_back(actTURN_SR);
				frontier.push_back(child_turn_sr);
			}

			child_turn_l.st = applyAction(currentNode.st, actTURN_L, sensores);
			if (explored.find(child_turn_l.st) == explored.end())
			{
				child_turn_l.secuencia.push_back(actTURN_L);
				frontier.push_back(child_turn_l);
			}
		}
		if (!solutionFound && !frontier.empty())
		{
			currentNode = frontier.front();
			while (!frontier.empty() && (explored.find(currentNode.st) != explored.end()))
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
		visualizarPlan(inicio, plan, sensores);
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
	currentNode.st = inicio;
	list<node> frontier;
	set<state> explored;
	bool solutionFound = (currentNode.st.colaborador.f == final.f && currentNode.st.colaborador.c == final.c);

	frontier.push_back(currentNode);

	while (!frontier.empty() && !solutionFound)
	{
		frontier.pop_front();
		explored.insert(currentNode.st);

		if (colaboradorEnSensor(currentNode.st, sensores))
		{
			node child_colab_walk = currentNode;
			child_colab_walk.st = applyAction(currentNode.st, act_CLB_WALK);
			if (child_colab_walk.st.colaborador.f == final.f && child_colab_walk.st.colaborador.c == final.c)
			{
				child_colab_walk.secuencia.push_back(act_CLB_WALK);
				currentNode = child_colab_walk;
				solutionFound = true;
			}
			if (!(child_colab_walk == currentNode) && (explored.find(child_colab_walk.st) == explored.end()))
			{
				child_colab_walk.secuencia.push_back(act_CLB_WALK);
				frontier.push_back(child_colab_walk);
			}

			if (!solutionFound)
			{
				node child_colab_turn = currentNode;
				child_colab_turn.st = applyAction(currentNode.st, act_CLB_TURN_SR);
				if (!(child_colab_turn == currentNode) && (explored.find(child_colab_turn.st) == explored.end()))
				{
					child_colab_turn.secuencia.push_back(act_CLB_TURN_SR);
					frontier.push_back(child_colab_turn);
				}
			}
		}
		if (!solutionFound)
		{
			if (currentNode.st.ultimaAccionColaborador != act_CLB_STOP)
			{
				node child_idle = currentNode;
				child_idle.st = applyAction(currentNode.st, actIDLE);
				if (child_idle.st.colaborador.f == final.f && child_idle.st.colaborador.c == final.c)
				{
					child_idle.secuencia.push_back(actIDLE);
					currentNode = child_idle;
					solutionFound = true;
				}
				else if (explored.find(child_idle.st) == explored.end())
				{
					child_idle.secuencia.push_back(actIDLE);
					frontier.push_back(child_idle);
				}
			}
			if (!solutionFound)
			{
				node child_walk = currentNode;
				child_walk.st = applyAction(currentNode.st, actWALK);

				if (child_walk.st.colaborador.f == final.f && child_walk.st.colaborador.c == final.c)
				{
					child_walk.secuencia.push_back(actWALK);
					currentNode = child_walk;
					solutionFound = true;
				}
				if (!(child_walk == currentNode) && explored.find(child_walk.st) == explored.end())
				{
					child_walk.secuencia.push_back(actWALK);
					frontier.push_back(child_walk);
				}
				if (!solutionFound)
				{
					node child_run = currentNode;
					child_run.st = applyAction(currentNode.st, actRUN);
					if (child_run.st.colaborador.f == final.f && child_run.st.colaborador.c == final.c)
					{
						child_run.secuencia.push_back(actRUN);
						currentNode = child_run;
						solutionFound = true;
					}
					if (!(child_run == currentNode) && explored.find(child_run.st) == explored.end())
					{
						child_run.secuencia.push_back(actRUN);
						frontier.push_back(child_run);
					}
					if (!solutionFound)
					{
						node child_turn_sr = currentNode, child_turn_l = currentNode;
						child_turn_sr.st = applyAction(currentNode.st, actTURN_SR);
						if (explored.find(child_turn_sr.st) == explored.end())
						{
							child_turn_sr.secuencia.push_back(actTURN_SR);
							frontier.push_back(child_turn_sr);
						}

						child_turn_l.st = applyAction(currentNode.st, actTURN_L);

						if (explored.find(child_turn_l.st) == explored.end())
						{
							child_turn_l.secuencia.push_back(actTURN_L);
							frontier.push_back(child_turn_l);
						}
					}
				}
			}
		}

		if (!solutionFound && !frontier.empty())
		{
			currentNode = frontier.front();
			while (!frontier.empty() && (explored.find(currentNode.st) != explored.end()))
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
			accion = plan.front();
			plan.pop_front();
			hayPlan = true;
		}
	}

	if (plan.empty())
		hayPlan = false;

	return accion;
}

bool ComportamientoJugador::busquedaN2(const stateJugador &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa, const Sensores &sensores)
{
	nodeJugador currentNode;
	currentNode.st = inicio;
	currentNode.coste = 0;
	priority_queue<nodeJugador> frontier;
	set<stateJugador> explored;
	bool solutionFound = (currentNode.st.jugador.f == final.f && currentNode.st.jugador.c == final.c);

	frontier.push(currentNode);

	while (!frontier.empty() && !solutionFound)
	{
		frontier.pop();
		explored.insert(currentNode.st);
		if (currentNode.st.jugador.f == final.f && currentNode.st.jugador.c == final.c)
		{
			solutionFound = true;
		}
		else
		{
			nodeJugador child_walk = currentNode;
			child_walk.coste += calculoCoste(child_walk.st, actWALK);
			child_walk.st = applyAction(child_walk.st, actWALK, sensores);

			if (!(child_walk == currentNode) && child_walk.coste <= sensores.bateria && (explored.find(child_walk.st) == explored.end()))
			{
				child_walk.secuencia.push_back(actWALK);
				frontier.push(child_walk);
			}

			if (!solutionFound)
			{
				nodeJugador child_run = currentNode;
				child_run.coste += calculoCoste(child_run.st, actRUN);
				child_run.st = applyAction(child_run.st, actRUN, sensores);

				if (!(child_run == currentNode) && child_run.coste <= sensores.bateria && (explored.find(child_run.st) == explored.end()))
				{
					child_run.secuencia.push_back(actRUN);
					frontier.push(child_run);
				}
				if (!solutionFound)
				{
					nodeJugador child_turn_sr = currentNode, child_turn_l = currentNode;
					child_turn_sr.coste += calculoCoste(child_turn_sr.st, actTURN_SR);
					child_turn_sr.st = applyAction(child_turn_sr.st, actTURN_SR, sensores);

					if (explored.find(child_turn_sr.st) == explored.end() && child_turn_sr.coste <= sensores.bateria)
					{
						child_turn_sr.secuencia.push_back(actTURN_SR);
						frontier.push(child_turn_sr);
					}

					child_turn_l.coste += calculoCoste(child_turn_l.st, actTURN_L);
					child_turn_l.st = applyAction(child_turn_l.st, actTURN_L, sensores);

					if (explored.find(child_turn_l.st) == explored.end() && child_turn_l.coste <= sensores.bateria)
					{
						child_turn_l.secuencia.push_back(actTURN_L);
						frontier.push(child_turn_l);
					}
				}
			}
		}
		if (!solutionFound && !frontier.empty())
		{
			currentNode = frontier.top();

			while (!frontier.empty() && (explored.find(currentNode.st) != explored.end()))
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
		visualizarPlan(inicio, plan, sensores);
		/*cout << "Coste del camino: " << currentNode.coste << endl;
		cout << "Nodos abiertos: " << frontier.size() << endl;
		cout << "Nodos cerrados: " << explored.size() << endl;*/
	}
	return solutionFound;
}

bool ComportamientoJugador::busquedaN3(const state &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa, const Sensores &sensores)
{
	node currentNode;
	currentNode.st = inicio;
	currentNode.coste = 0;
	priority_queue<node> frontier;
	set<state> explored;
	bool solutionFound = (currentNode.st.colaborador.f == final.f && currentNode.st.colaborador.c == final.c);

	frontier.push(currentNode);

	while (!frontier.empty() && !solutionFound)
	{
		frontier.pop();
		explored.insert(currentNode.st);

		if (currentNode.st.colaborador.f == final.f && currentNode.st.colaborador.c == final.c)
		{
			solutionFound = true;
		}
		else
		{
			if (colaboradorEnSensor(currentNode.st, sensores))
			{
				node child_colab_walk = currentNode;
				child_colab_walk.coste += calculoCoste(child_colab_walk.st, act_CLB_WALK);
				child_colab_walk.st = applyAction(currentNode.st, act_CLB_WALK);

				if (!(child_colab_walk == currentNode) && (explored.find(child_colab_walk.st) == explored.end()))
				{
					child_colab_walk.heuristica = calcularHeuristica(child_colab_walk.st.colaborador, final, sensores);
					child_colab_walk.secuencia.push_back(act_CLB_WALK);
					frontier.push(child_colab_walk);
				}

				if (!solutionFound)
				{
					node child_colab_turn = currentNode;
					child_colab_turn.coste += calculoCoste(child_colab_turn.st, act_CLB_TURN_SR);
					child_colab_turn.st = applyAction(currentNode.st, act_CLB_TURN_SR);
					if (!(child_colab_turn == currentNode) && (explored.find(child_colab_turn.st) == explored.end()))
					{
						child_colab_turn.heuristica = calcularHeuristica(child_colab_turn.st.colaborador, final, sensores);
						child_colab_turn.secuencia.push_back(act_CLB_TURN_SR);
						frontier.push(child_colab_turn);
					}
					if (currentNode.st.ultimaAccionColaborador != act_CLB_STOP)
					{
						node child_colab_stop = currentNode;
						child_colab_stop.st = applyAction(currentNode.st, act_CLB_STOP);
						if (explored.find(child_colab_stop.st) == explored.end())
						{
							child_colab_stop.heuristica = calcularHeuristica(child_colab_stop.st.colaborador, final, sensores);
							child_colab_stop.secuencia.push_back(act_CLB_STOP);
							frontier.push(child_colab_stop);
						}
					}
				}
			}
			if (!solutionFound)
			{
				if (currentNode.st.ultimaAccionColaborador != act_CLB_STOP)
				{
					node child_idle = currentNode;
					child_idle.coste += calculoCoste(child_idle.st, actIDLE);
					child_idle.st = applyAction(currentNode.st, actIDLE);

					if (explored.find(child_idle.st) == explored.end())
					{
						child_idle.heuristica = calcularHeuristica(child_idle.st.colaborador, final, sensores);
						child_idle.secuencia.push_back(actIDLE);
						frontier.push(child_idle);
					}
				}
				if (!solutionFound)
				{
					node child_walk = currentNode;
					child_walk.coste += calculoCoste(child_walk.st, actWALK);
					child_walk.st = applyAction(currentNode.st, actWALK);

					if (!(child_walk == currentNode) && explored.find(child_walk.st) == explored.end())
					{
						child_walk.heuristica = calcularHeuristica(child_walk.st.colaborador, final, sensores);
						child_walk.secuencia.push_back(actWALK);
						frontier.push(child_walk);
					}
					if (!solutionFound)
					{
						node child_run = currentNode;
						child_run.coste += calculoCoste(child_run.st, actRUN);
						child_run.st = applyAction(currentNode.st, actRUN);

						if (!(child_run == currentNode) && explored.find(child_run.st) == explored.end())
						{
							child_run.heuristica = calcularHeuristica(child_run.st.colaborador, final, sensores);
							child_run.secuencia.push_back(actRUN);
							frontier.push(child_run);
						}
						if (!solutionFound)
						{
							node child_turn_sr = currentNode, child_turn_l = currentNode;
							child_turn_sr.coste += calculoCoste(child_turn_sr.st, actTURN_SR);
							child_turn_sr.st = applyAction(currentNode.st, actTURN_SR);
							if (explored.find(child_turn_sr.st) == explored.end())
							{
								child_turn_sr.heuristica = calcularHeuristica(child_turn_sr.st.colaborador, final, sensores);
								child_turn_sr.secuencia.push_back(actTURN_SR);
								frontier.push(child_turn_sr);
							}
							child_turn_l.coste += calculoCoste(child_turn_l.st, actTURN_L);
							child_turn_l.st = applyAction(currentNode.st, actTURN_L);

							if (explored.find(child_turn_l.st) == explored.end())
							{
								child_turn_l.heuristica = calcularHeuristica(child_turn_l.st.colaborador, final, sensores);
								child_turn_l.secuencia.push_back(actTURN_L);
								frontier.push(child_turn_l);
							}
						}
					}
				}
			}
		}
		if (!solutionFound && !frontier.empty())
		{
			currentNode = frontier.top();
			while (!frontier.empty() && (explored.find(currentNode.st) != explored.end()))
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

	current_state_jugador.st.jugador.f = sensores.posF;
	current_state_jugador.st.jugador.c = sensores.posC;
	current_state_jugador.st.jugador.brujula = sensores.sentido;

	if (hayPlan && !plan.empty())
	{
		accion = plan.front();
		plan.pop_front();
	}
	else if (busquedaN0(current_state_jugador.st, {sensores.destinoF, sensores.destinoC}, mapaResultado, sensores))
	{
		if (!plan.empty())
		{
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

	current_state_jugador.st.jugador.f = sensores.posF;
	current_state_jugador.st.jugador.c = sensores.posC;
	current_state_jugador.st.jugador.brujula = sensores.sentido;

	if (mapaResultado[current_state_jugador.st.jugador.f][current_state_jugador.st.jugador.c] == 'K')
	{
		current_state_jugador.st.objetos_jugador.bikini = true;
		current_state_jugador.st.objetos_jugador.zapatillas = false;
	}
	else if (mapaResultado[current_state_jugador.st.jugador.f][current_state_jugador.st.jugador.c] == 'D')
	{
		current_state_jugador.st.objetos_jugador.bikini = false;
		current_state_jugador.st.objetos_jugador.zapatillas = true;
	}

	if (hayPlan && !plan.empty())
	{
		accion = plan.front();
		plan.pop_front();
	}
	else if (busquedaN2(current_state_jugador.st, {sensores.destinoF, sensores.destinoC}, mapaResultado, sensores))
	{
		if (!plan.empty())
		{
			accion = plan.front();
			plan.pop_front();
			hayPlan = true;
		}
	}

	if (plan.empty())
		hayPlan = false;

	return accion;
}
Action ComportamientoJugador::nivel3(const Sensores &sensores)
{
	Action accion = actIDLE;

	current_state.st.jugador.f = sensores.posF;
	current_state.st.jugador.c = sensores.posC;
	current_state.st.jugador.brujula = sensores.sentido;

	current_state.st.colaborador.f = sensores.CLBposF;
	current_state.st.colaborador.c = sensores.CLBposC;
	current_state.st.colaborador.brujula = sensores.CLBsentido;

	if (mapaResultado[current_state.st.jugador.f][current_state.st.jugador.c] == 'K')
	{
		current_state.st.objetos_jugador.bikini = true;
		current_state.st.objetos_jugador.zapatillas = false;
	}
	else if (mapaResultado[current_state.st.jugador.f][current_state.st.jugador.c] == 'D')
	{
		current_state_jugador.st.objetos_jugador.bikini = false;
		current_state_jugador.st.objetos_jugador.zapatillas = true;
	}
	if (mapaResultado[current_state.st.colaborador.f][current_state.st.colaborador.c] == 'K')
	{
		current_state.st.objetos_colaborador.bikini = true;
		current_state.st.objetos_colaborador.zapatillas = false;
	}
	else if (mapaResultado[current_state.st.colaborador.f][current_state.st.colaborador.c] == 'D')
	{
		current_state.st.objetos_colaborador.zapatillas = true;
		current_state.st.objetos_colaborador.bikini = false;
	}

	if (hayPlan && !plan.empty())
	{
		accion = plan.front();
		plan.pop_front();
	}
	else if (busquedaN3(current_state.st, {sensores.destinoF, sensores.destinoC}, mapaResultado, sensores))
	{
		if (!plan.empty())
		{
			accion = plan.front();
			plan.pop_front();
			hayPlan = true;
		}
	}

	if (plan.empty())
		hayPlan = false;

	return accion;
}

void ComportamientoJugador::guardar_mapa(const Sensores &sensores, vector<vector<unsigned char>> &mapa)
{
	for (unsigned int i = 0; i < sensores.terreno.size(); i++)
		if (sensores.terreno[i] != '?')
		{
			pair<unsigned int, unsigned int> pos = obtener_coordenadas(i);
			mapa[pos.first][pos.second] = sensores.terreno[i];
		}
}

pair<unsigned int, unsigned int> ComportamientoJugador::obtener_coordenadas(const unsigned int &pos) const
{
	pair<unsigned int, unsigned int> salida = {0, 0};
	switch (current_state4.st.jugador.brujula)
	{
	case norte:
		salida = {current_state4.st.jugador.f + sensores_norte[pos].first, current_state4.st.jugador.c + sensores_norte[pos].second};
		break;
	case sur:
		salida = {current_state4.st.jugador.f - sensores_norte[pos].first, current_state4.st.jugador.c - sensores_norte[pos].second};
		break;
	case este:
		salida = {current_state4.st.jugador.f + sensores_norte[pos].second, current_state4.st.jugador.c - sensores_norte[pos].first};
		break;
	case oeste:
		salida = {current_state4.st.jugador.f - sensores_norte[pos].second, current_state4.st.jugador.c + sensores_norte[pos].first};
		break;
	case noreste:
		salida = {current_state4.st.jugador.f + sensores_noreste[pos].first,
				  current_state4.st.jugador.c + sensores_noreste[pos].second};
		break;
	case noroeste:
		salida = {current_state4.st.jugador.f - sensores_noreste[pos].second,
				  current_state4.st.jugador.c + sensores_noreste[pos].first};
		break;
	case sureste:
		salida = {current_state4.st.jugador.f + sensores_noreste[pos].second,
				  current_state4.st.jugador.c - sensores_noreste[pos].first};
		break;
	case suroeste:
		salida = {current_state4.st.jugador.f - sensores_noreste[pos].first,
				  current_state4.st.jugador.c - sensores_noreste[pos].second};
		break;
	}
	return salida;
}
bool ComportamientoJugador::personajesSinColisionar(const state &st, const state &inicio, const Sensores &sensores, const vector<vector<unsigned char>> &mapa) const
{
	if (st.jugador.f == st.colaborador.f && st.jugador.c == st.colaborador.c)
		return false;
	else if (!casillaTransitable(st.jugador) || !casillaTransitable(st.colaborador))
		return false;
	else if (mapaResultado[st.jugador.f][st.jugador.c] == '?' || (mapaResultado[st.colaborador.f][st.colaborador.c] == '?' && !(st.colaborador == inicio.colaborador)))
		return false;
	else
		return true;
}
bool ComportamientoJugador::busquedaN4(const stateJugador4 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa, const Sensores &sensores, const ubicacion &colab)
{
	nodeJugador4 currentNode;
	currentNode.st = inicio;
	currentNode.coste = 0;
	priority_queue<nodeJugador4> frontier;
	set<stateJugador4> explored;
	bool solutionFound = (currentNode.st.jugador.f == final.f && currentNode.st.jugador.c == final.c);

	frontier.push(currentNode);

	while (!frontier.empty() && !solutionFound)
	{
		frontier.pop();
		explored.insert(currentNode.st);
		if (currentNode.st.jugador.f == final.f && currentNode.st.jugador.c == final.c)
		{
			solutionFound = true;
		}
		else
		{
			nodeJugador4 child_walk = currentNode;
			child_walk.coste += calculoCoste(child_walk.st, actWALK);
			child_walk.st = applyAction(child_walk.st, actWALK, colab);

			if (!(child_walk == currentNode) && child_walk.coste <= sensores.bateria && (explored.find(child_walk.st) == explored.end()))
			{
				child_walk.heuristica = calcularHeuristica(child_walk.st.jugador, final, sensores);
				child_walk.secuencia.push_back(actWALK);
				frontier.push(child_walk);
			}

			if (!solutionFound)
			{
				nodeJugador4 child_run = currentNode;
				child_run.coste += calculoCoste(child_run.st, actRUN);
				child_run.st = applyAction(child_run.st, actRUN, colab);

				if (!(child_run == currentNode) && child_run.coste <= sensores.bateria && (explored.find(child_run.st) == explored.end()))
				{
					child_run.heuristica = calcularHeuristica(child_run.st.jugador, final, sensores);
					child_run.secuencia.push_back(actRUN);
					frontier.push(child_run);
				}
				if (!solutionFound)
				{
					nodeJugador4 child_turn_sr = currentNode, child_turn_l = currentNode;
					child_turn_sr.coste += calculoCoste(child_turn_sr.st, actTURN_SR);
					child_turn_sr.st = applyAction(child_turn_sr.st, actTURN_SR, colab);

					if (explored.find(child_turn_sr.st) == explored.end() && child_turn_sr.coste <= sensores.bateria)
					{
						child_turn_sr.heuristica = calcularHeuristica(child_turn_sr.st.jugador, final, sensores);
						child_turn_sr.secuencia.push_back(actTURN_SR);
						frontier.push(child_turn_sr);
					}

					child_turn_l.coste += calculoCoste(child_turn_l.st, actTURN_L);
					child_turn_l.st = applyAction(child_turn_l.st, actTURN_L, colab);

					if (explored.find(child_turn_l.st) == explored.end() && child_turn_l.coste <= sensores.bateria)
					{
						child_turn_l.heuristica = calcularHeuristica(child_turn_l.st.jugador, final, sensores);
						child_turn_l.secuencia.push_back(actTURN_L);
						frontier.push(child_turn_l);
					}
				}
			}
		}
		if (!solutionFound && !frontier.empty())
		{
			currentNode = frontier.top();

			while (!frontier.empty() && (explored.find(currentNode.st) != explored.end()))
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
		// visualizarPlan(inicio, plan, sensores);
		/*cout << "Coste del camino: " << currentNode.coste << endl;
		cout << "Nodos abiertos: " << frontier.size() << endl;
		cout << "Nodos cerrados: " << explored.size() << endl;*/
	}
	return solutionFound;
}

Action ComportamientoJugador::nivel4(const Sensores &sensores)
{
	Action salida = actIDLE;
	if (sensores.colision)
		bien_situado = false;
	if (lastAction == actWHEREIS)
	{
		bien_situado = true;
		current_state4.st.jugador.f = sensores.posF;
		current_state4.st.jugador.c = sensores.posC;
		current_state4.st.jugador.brujula = sensores.sentido;

		current_state4.st.objetos_jugador.bikini = false;
		current_state4.st.objetos_jugador.zapatillas = false;

		colab.brujula = sensores.CLBsentido;
		colab.f = sensores.CLBposF;
		colab.c = sensores.CLBposC;
	}
	if (!bien_situado)
		return actWHEREIS;

	if (lastAction == actWALK || lastAction == actRUN || lastAction == actTURN_L || lastAction == actTURN_SR || lastAction == actWHEREIS)
	{
		guardar_mapa(sensores, mapaResultado);
		if (mapaResultado[current_state4.st.jugador.f][current_state4.st.jugador.c] == 'K')
		{
			current_state4.st.objetos_jugador.bikini = true;
			current_state4.st.objetos_jugador.zapatillas = false;
		}
		else if (mapaResultado[current_state4.st.jugador.f][current_state4.st.jugador.c] == 'D')
		{
			current_state4.st.objetos_jugador.zapatillas = true;
			current_state4.st.objetos_jugador.bikini = false;
		}
	}
	bool condicion_recarga = ((sensores.bateria < 3000 && sensores.vida > 1500) || sensores.bateria < 1000);
	plan.clear();
	if (mapaResultado[current_state4.st.jugador.f][current_state4.st.jugador.c] == 'X' && condicion_recarga){
		mov_sin_recarga = 0;
		return actIDLE;
	}
	else if (condicion_recarga && mov_sin_recarga > 200)
	{
		bool encontrado = false;
		for (int i = 0; i < mapaResultado.size() && !encontrado; i++)
			for (int j = 0; j < mapaResultado[0].size() && !encontrado; j++)
				if (mapaResultado[i][j] == 'X')
				{
					encontrado = true;
					busquedaN4(current_state4.st, {i, j}, mapaResultado, sensores, colab);
				}
	}else mov_sin_recarga++;
	
	if(plan.empty())
		busquedaN4(current_state4.st, {sensores.destinoF, sensores.destinoC}, mapaResultado, sensores, colab);
	

	if (!plan.empty())
	{
		salida = plan.front();
		plan.pop_front();
		hayPlan = true;
	}

	else
		hayPlan = false;

	current_state4.st = applyAction(current_state4.st, salida, colab);

	return salida;
}
Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;

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
		accion = nivel3(sensores);
		break;
	case 4:
		accion = nivel4(sensores);
		break;
	default:
		throw("Nivel no implementado");
	}

	lastAction = accion;
	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}
