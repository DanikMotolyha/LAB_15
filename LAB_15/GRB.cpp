#include "GRB.h"
#define GRB_ERROR_SERIES 600
using namespace std;
#include <iostream>
namespace GRB
{
#define NS(n) Rule::Chain::N(n)
#define TS(n) Rule::Chain::T(n)
	Greibach greibach(NS('S'), TS('$'),		// стартовый символ, дно стека
		6,									// количество правил
		// S - Стартовый символ он же вход в функцию
		Rule(NS('S'), GRB_ERROR_SERIES + 0,	// неверная структура программы
			3,		// S→ m{NrE;};|tfi(F){NrE;};S|m{NrE;};S
			Rule::Chain(8, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';')),
			Rule::Chain(14, TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS('}'), TS(';'), NS('S')),
			Rule::Chain(10, TS('m'), TS('{'), NS('N'), TS('r'), NS('E'), TS(';'), TS(';'), TS('}'), TS(';'), NS('S'))
		),
		// N - Последовательность операторов программы
		Rule(NS('N'), GRB_ERROR_SERIES + 1,	// неверная структура программы
			10,			// N→ dti; | rE; | i = E; | dtfi(F); | dti; N | rE; N | i = E; N | dtfi(F); N
			// дополняю для лексемы p она же print
			// то есть еще будет + N -> pi;|pl; 
			Rule::Chain(4, TS('d'), TS('t'), TS('i'), TS(';')),
			Rule::Chain(3, TS('r'), NS('E'), TS(';')),
			Rule::Chain(4, TS('i'), TS('='), NS('E'), TS(';')),
			Rule::Chain(8, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';')),
			Rule::Chain(5, TS('d'), TS('t'), TS('i'), TS(';'), NS('N')),
			Rule::Chain(4, TS('r'), NS('E'), TS(';'), NS('N')),
			Rule::Chain(5, TS('i'), TS('='), NS('E'), TS(';'), NS('N')),
			Rule::Chain(9, TS('d'), TS('t'), TS('f'), TS('i'), TS('('), NS('F'), TS(')'), TS(';'), NS('N')),
			//----------- добавил две строчки для print но по своему хорошо и плохо
			Rule::Chain(3, TS('p'), NS('E'), TS(';')),
			Rule::Chain(4, TS('p'), NS('E'), TS(';'), NS('N'))
		),
		// E - Выражениe
		Rule(NS('E'), GRB_ERROR_SERIES + 2,	// неверная структура программы
			8,				// E->i|l|(E)|i(W)|iM|lM|(E)M|i(W)M			
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('('), NS('E'), TS(')')),
			Rule::Chain(4, TS('i'), TS('('), NS('W'), TS(')')),
			Rule::Chain(2, TS('i'), NS('M')),
			Rule::Chain(2, TS('l'), NS('M')),
			Rule::Chain(4, TS('('), NS('E'), TS(')'), NS('M')),
			Rule::Chain(5, TS('i'), TS('('), NS('W'), TS(')'), NS('M'))
		),
		// M - 
		Rule(NS('M'), GRB_ERROR_SERIES + 2,	// неверная структура программы
			2,		// M→vE|vEM
			Rule::Chain(2, TS('v'), NS('E')),
			Rule::Chain(3, TS('v'), NS('E'), NS('M'))
		),
		// F - Параметры
		Rule(NS('F'), GRB_ERROR_SERIES + 3,	// неверная структура программы
			2,		// F→ti|ti,F
			Rule::Chain(2, TS('t'), TS('i')),
			Rule::Chain(4, TS('t'), TS('i'), TS(','), NS('F'))
		),
		// W - 
		Rule(NS('W'), GRB_ERROR_SERIES + 4,	// неверная структура программы
			4,		// W→i|l|i,W|l,W 
			Rule::Chain(1, TS('i')),
			Rule::Chain(1, TS('l')),
			Rule::Chain(3, TS('i'), TS(','), NS('W')),
			Rule::Chain(3, TS('l'), TS(','), NS('W'))
		)
	);
	Rule::Chain::Chain(short psize, GRBALPHABET s, ...)
	{
		nt = new GRBALPHABET[size = psize];
		int* p = (int*)&s;
		for (short i = 0; i < psize; i++) nt[i] = (GRBALPHABET)p[i];
	}
	Rule::Rule(GRBALPHABET pnn, int piderror, short psize, Chain c, ...)
	{
		nn = pnn;
		iderror = piderror;
		chains = new Chain[size = psize];
		Chain* p = &c;
		for (int i = 0; i < size; i++) chains[i] = p[i];
	}
	Greibach::Greibach(GRBALPHABET pstartN, GRBALPHABET pstbottom, short psize, Rule r, ...)
	{
		startN = pstartN;
		stbottomT = pstbottom;
		rules = new Rule[size = psize];
		Rule* p = &r;
		for (int i = 0; i < size; i++) rules[i] = p[i];
	}
	Greibach getGreibach()
	{
		return greibach;
	}
	short Greibach::getRule(GRBALPHABET pnn, Rule& prule)
	{
		short rc = -1;
		short k = 0;
		while (k < size && rules[k].nn != pnn) k++;
		if (k < size) prule = rules[rc = k];
		return rc;
	}
	Rule Greibach::getRule(short n)
	{
		Rule rc;
		if (n < size) rc = rules[n];
		return rc;
	}
	char* Rule::getCRule(char* b, short nchain)		// получить правило в виде N-> цепочка
	{
		char bchain[200];
		b[0] = Chain::alphabet_to_char(nn); b[1] = '-'; b[2] = '>'; b[3] = 0x00;
		chains[nchain].getCChain(bchain);
		strcat_s(b, sizeof(bchain) + 5, bchain);
		return b;
	}
	short Rule::getNextChain(GRBALPHABET t, Rule::Chain& pchain, short j)
	{
		short rc = -1;
		while (j < size &&  chains[j].nt[0] != t) ++j;
		rc = (j < size ? j : -1);
		if (rc >= 0) pchain = chains[rc];
		return rc;
	}
	char* Rule::Chain::getCChain(char* b)		// получить правую сторону правила
	{
		for (int i = 0; i < size; i++) b[i] = Chain::alphabet_to_char(nt[i]);
		b[size] = 0x00;
		return b;
	}
}