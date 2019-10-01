#pragma once

using namespace std;
#include <iostream>
#include "Error.h"  // обработка ошибок
#include "In.h"		// ввод исходного файла
#include "Log.h"	// работа с протоколом
#include "Parm.h"	// обработка параметров
#include "FST.h"	// конечный автомат для лексического анализатора
#include "LT.h"		// таблица лексем
#include "IT.h"		// таблица идентификаторов
#include "GRB.h"	// структура для грамматики в форме Грейбаха
#include "MFST.h"	// магазинный конечный автомат