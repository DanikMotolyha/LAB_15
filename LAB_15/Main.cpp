
#include "Header.h"

int wmain(int agrc, wchar_t* agrv[])
{
	setlocale(LC_ALL, "RUSSIAN");
	Log::LOG log = Log::INITLOG;
	try
	{
		Parm::PARM parm = Parm::getparm(agrc, agrv);
		log = Log::getlog(parm.log);
		Log::WriteLine(log, "Текст: ", "без ошибок ", "");
		WriteLog(log);
		Log::WriteParm(log, parm);
		In::IN in = In::getin(parm.in);
		Log::WriteIn(log, in);
		Log::WriteText(log, in);
		LT::LexTable lt = LT::Create();
		LT::LexTable *lt_s = &lt;
		IT::IdTable it = IT::Create();
		IT::IdTable *it_s = &it;
		LT::table(in, lt_s, it_s);
		Log::WriteLT(lt, log, it);

		Log::WriteLT_ALL(lt, log);
		Log::WriteIT_ALL(it, log);
		MFST_TRACE_START				// отладка
		MFST::Mfst mfst(lt, GRB::getGreibach());
		mfst.start();

		mfst.savededucation();

		mfst.printrules();


		In::Delete(in);
		LT::Delete(lt);
		IT::Delete(it);
		Log::Close(log);
	}
	catch (Error::ERROR e)
	{
		if(e.id != 100)
		Log::WriteError(log, e);
		else { cout << "Ошибка " << e.id << "  " << e.message; system("pause"); }
	}
	cout << "End of program, press F" << endl; system("pause");
}
