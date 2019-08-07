char Text_To_Display (char x, char y,near rom  unsigned char * data);
void Command_To_Display (near rom unsigned char * data);
unsigned char Nomber(unsigned char x, unsigned char y, unsigned char length, int data);
void Rectangle (char X, char Xend, char Y, char Yend, char data);

void Nomber_Image_Char(unsigned char x, unsigned char y, struct RES * DDat, char sign);

#include <delays.h>
//#include "GP1157.c"

void main(void);
char Compare (char Data, near rom char * ComArray);
void ROMStringTX (unsigned near rom unsigned char * data);
void RAMStringTX (unsigned char * data);
char FindString (rom char *rom *data);
void Processing (void);
void SymbolFind (char);
void RegRW(void);
void Err(void);
void ReadSyst(void);
void InpSyst(void);
void InitSyst(void);
void TrigSyst(void);
signed short StringToBin(void);
void ConfSyst(void);
char Order (long Data);
void TransfAll (void);
void MesErr (char ER);
void FloatToBinDec (float Data, char Integ, char Length);
long degree(long Dat, char deg);
float FloatDegree(signed char deg);
void IndRes(void);
char FindAllDiction	(char Ind);
char FindAndCheck (rom char *rom *Pointer, char min, char max);
void ErrQueue (char n);
void BinToBinDec (struct RES * Res);

char ErrQue[20];		//Очередь ошибок
char QueIndex=0;		//Индекс очереди ошибок

	rom char ConvTab[6] = 	{Chan_A, Chan_C, Chan_B, Chan_B, Chan_B, Chan_B};
	rom char ConvTab3[3] = 	{Ch_a_c, Ch_c_a, Ch_b_c};
	rom char ConvTab4[3] = 	{Ch_a_c, Ch_c_a, Ch_b_a};

	rom char **Point;
	char Temp, *Point8, CommInd;

//-----------------------------------------------------
void RAMStringTX (unsigned char * data)
{
	while(*data)
	{
		while(!PIR1bits.TXIF);
		if(*data != 0x16) TXREG = *data;
		data++;
	}
}
//-----------------------------------------------------
//-----------------------------------------------------
void ROMStringTX (unsigned near rom unsigned char * data)
{
	while(*data)
	{
		while(!PIR1bits.TXIF);
		TXREG = *data;
		data++;
	}
}
//-----------------------------------------------------

//-----------------------------------------------------
char FindString (rom char *rom *data)
{
	char i, ans, temp ;
	short j;

	for(i = 1; i < *data[0]; i++)					//Цикл строк
	{
		ans = 0;

		for(j = 0; *(data[i]+j); j++)
		{
			if(*(data[i]+j) != InpSTR[StartInd + j])	ans = 1;
		}

		temp = InpSTR[StartInd + j];

		if(!((temp == ':')||(temp == ' ')||(temp == ',')||(temp == '?')||(temp == LineFeed))) ans = 1;


		if(!ans)
		{
			StartInd += j;
			return (i>>1) - 1;
		}
	}

	return 255;
}
//-----------------------------------------------------
//-----------------------------------------------------
void SymbolFind (char Symb)
{
	char i=0;

	while(((StartInd + i)<128)&&((InpSTR[StartInd + i] != LineFeed)))
	{
		if(InpSTR[StartInd + i] == Symb)
		{
			StartInd = i;
			return 1;
		}
		i++;
	}
	return 0;
}
//-----------------------------------------------------
//-----------------------------------------------------
void Processing (void)
{
	Temp = FindString(String24);

	switch(Temp)
	{
		case Input:
		InpSyst();
		break;
	
		case Initiate:
		InitSyst();
		break;

	   	case Trigger:
		TrigSyst();
		break; 
	
	    case Configure:
		ConfSyst();
		break; 
	
	    case Measure:
		ConfSyst();
		ArrayCnt = 1;
		TransfWord(0b01010000011);			//Передача частотомеру команды ПРОГРАММНЫЙ ЗАПУСК
		Flags.SoftStart = 1;				//Установка флага активности программного запуска
		break; 

	    case Read:
		ReadSyst();
		break; 

	    case RST:	//Сброс
		_asm
		RESET
		_endasm
		break; 

	    case CLS:	//Очистка состояния
		ArrayCnt = 0;
		QueIndex=0;
		break; 

	    case IDN:	//Идентификационный номер
		ROMStringTX("OAO MNIPI, Ч3-96, Relf V 1.1\r");
		break; 

	    case ERRor:		//Запрос очереди ошибок
		Err();
		QueIndex=0;
		break; 

		default:
		ErrQueue(4);				//Ошибка команды
		return;
		break; 
	}

//	(*FunctPoint)();
}
//-----------------------------------------------------

//-----------------------------------------------------
void ReadSyst(void)
{
	char Comm;

	if(InpSTR[StartInd] == '?')	{StartInd++; ArrayCnt = 1;}		//Обработка запроса
	else
	{
		Comm = FindAndCheck(String40, SCALar, ARRay);
	
		switch(Comm)
		{
			case 255:
			case SCALar:
			ArrayCnt = 1;
			break;
	
			default:
			if(InpSTR[StartInd] == '?')	StartInd += 2;		//Обработка запроса
			else StartInd++;
			ArrayCnt = StringToBin();
			if(ArrayCnt == 0x4000)		ErrQueue(3);			//Недозволенный параметр
			if(!((ArrayCnt > 0)&&(ArrayCnt < 1001)))
			{
				ArrayCnt = 0;
				ErrQueue(1);					//Вне диапазона
				return;
			}
			break;
		}
	}

	TransfWord(0b01010000011);			//Передача частотомеру команды ПРОГРАММНЫЙ ЗАПУСК
	Flags.SoftStart = 1;				//Установка флага активности программного запуска
}
//-----------------------------------------------------,
//-----------------------------------------------------
void ErrQueue (char n)	//Очередь ошибок
{
	if(QueIndex<19)
	{
		if((ErrQue[QueIndex-1]!=n)||(n==0))	//Если предыдущая ошибка отличается от текущей
		{
			ErrQue[QueIndex] = n;
			QueIndex++;
		}
	}
}
//-----------------------------------------------------
//-----------------------------------------------------
void Err(void)
{
	signed char i;
	if(QueIndex)
	{
		for(i=0;i<QueIndex;i++)
		{
			ROMStringTX(StringErr[ErrQue[i]]);
			ROMStringTX(",");
		}
	}
	else ROMStringTX("No Error");

	ROMStringTX("\r");
}
//-----------------------------------------------------

//-----------------------------------------------------
void InpSyst(void)
{
	char arg, Comm, Chan, quest=0;

	StartInd++;
	Comm = FindString(String25);

	switch(Comm)
	{
		case Coupling:
		Point8 = OpenClose;
		break;

		case Filter:
		Point8 = LF;
		break;

	  	case Impedance:
		Point8 = Rin;
		break; 
	
	 	case Divider:
		Point8 = Kin;
		break; 

		default:
		ErrQueue(4);				//Ошибка команды
		return;
		break; 
	}

	if(InpSTR[StartInd] == '?')	{quest = 1; StartInd++;}		//Обработка запроса

	arg = FindAndCheck(Point_2[Comm], 0, 1);
	Chan = FindAndCheck(String45, Ch_a, Ch_b);

	if(Chan == 255)
	{
		if(Channel>1)	{ErrQueue(2);	return;}		//Ошибка, если выбран канал В
		else Chan = Channel;		//Если номер канала не указан, то используем текущий
	}
	else Chan -= Ch_a;

	if(Chan > 1)	{ErrQueue(3);	return;}//Обработка канала (на предмет канала В)//Ошибка, если выбран канал В

	Point8 +=	Chan;

	if(!quest)
	{
		if(arg>1)	{ErrQueue(3);	return;}

		*Point8 = arg;										//Установка
		TransfAll();	//Обновление всех параметров на плате частотомера
	}
	else
	{
		ROMStringTX(((Point_2[Comm][(*Point8+1)<<1])));	//Запрос
		ROMStringTX("\r");
	}
}
//-----------------------------------------------------

//-----------------------------------------------------
void InitSyst(void)
{
	char arg, quest=0, Comm;

		StartInd++;
		Comm = FindString(String30);

		switch(Comm)
		{
			case Source:
			Point8 = Starting;
			break;

	 	  	case Timer:
			Point8 = IndTime;
			break; 

			default:
			ErrQueue(4);				//Ошибка команды
			return;
			break; 
		}

	if(InpSTR[StartInd] == '?')	{quest = 1; StartInd++;}		//Обработка запроса

	arg = FindAndCheck(Point_3[Comm], 0, 2)		;//Определяем записываемые данные

	if(!quest)
	{
		if(arg == 255)	{ErrQueue(3);	return;}	//Недозволенный параметр

		*Point8 = arg;				//Установка
						
		TransfAll();	//Обновление всех параметров на плате частотомера
	}
	else
	{
		ROMStringTX(((Point_3[Comm][(*Point8+1)<<1])));	//Запрос
		ROMStringTX("\r");
	}
}
//-----------------------------------------------------
//-----------------------------------------------------
void TrigSyst(void)
{
	char arg, Chan=255, MesMode, quest=0;
	signed short LevTemp=0x4000;
	
	StartInd++;
	MesMode = FindString(String33);
	if(InpSTR[StartInd] == '?')	{quest = 1; StartInd++;}		//Обработка запроса

	switch(MesMode)
	{
//Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---
		case Slope:

		arg = FindAndCheck(String34, Positive, Negative);
		Chan = FindAndCheck(String45, Ch_a, Ch_c);

		if(Chan == 255)						//Обработка канала (если в команде он явно не указан)
		{
			if(Channel>1)	{ErrQueue(2);	return;}//Ошибка, если выбран канал В
			else Chan = Channel;				//Если номер канала не указан, то используем текущий
		}
		else Chan -= Ch_a;

		if(Chan > 1)	{ErrQueue(3);	return;}	//Обработка канала (а предмет канала В)//Ошибка, если выбран канал В

		if(!quest)						//Обработка возможности запроса
		{
			if(arg == 255)	{ErrQueue(0);	return;}	//Ошибка, если не указан аргумент

			Rise[Chan] = arg;						//Установка
			TransfAll();	//Обновление всех параметров на плате частотомера
		}
		else
		{
			ROMStringTX(String34[(Rise[Chan]+1)<<1]);	//Запрос
			ROMStringTX("\r");
		}

		break;
//Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---

//Level_M---Level_M---Level_M---Level_M---Level_M---Level_M---Level_M---Level_M---Level_M---Level_M---
 	  	case Level_M:

		if(InpSTR[StartInd] != LineFeed)	//Проверяем конец строки
		{
			StartInd++;
			LevTemp = StringToBin()/2;		//Определяем записываемые данные
			Chan = FindAndCheck(String45, Ch_a, Ch_c);
		}

		if(Chan == 255)						//Обработка канала (если в команде он явно не указан)
		{
			if(Channel>1)	{ErrQueue(2); return;}	//Ошибка, если выбран канал В
			else Chan = Channel;			//Если номер канала не указан, то используем текущий
		}
		else Chan -= Ch_a;

		if(Chan > 1)	{ErrQueue(3); return;}		//Обработка канала (а предмет канала В)//Ошибка, если выбран канал В

		if(!quest)							//Установка
		{

			if((LevTemp < 4001) && (LevTemp > -4001))
			{
				if((LevTemp < 401) && (LevTemp > -401))	Kin[Chan] = 0;					//Делитель откл
				else									{Kin[Chan] = 1; LevTemp /=10;}	//Делитель вкл
			
				TransfWord(0b00010000000 | Kin[Chan]);					//Делитель

				Level[Chan] = LevTemp;
				TransfWord(LevTemp | 0x400);	//Передача частотомеру данных об уровне синхронизации
			}
			else	{ErrQueue(1);	return;}
		}
		else
		{
			LevTemp = Level[Chan];
			if(Kin[Chan])LevTemp *= 10;
//			FloatToBinDec(LevTemp*2e-3, 3, 4);		//Преобразуем число к двоично-десятичному виду

			RES_Temp.Data = LevTemp*2;						//Отображаем результат измерения
			RES_Temp.Order = 0;
			BinToBinDec(&RES_Temp);
			RAMStringTX(Decades+4);	//Запрос
			ROMStringTX("\r");
		}
		
		break;
//Level_M---Level_M---Level_M---Level_M---Level_M---Level_M---Level_M---Level_M---Level_M---Level_M---

//Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---
 	  	case Auto_M:

		TransfWord(0b01111000011);	//Передача частотомеру команды АВТО
		Flags.Auto = 1;				//Установка флага активности режима АВТО
		Flags.RefrCreen_2 = 1;		//Установка флага второстепенного обновления экрана

		break; 
//Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---

//Esl---Ttl---Esl---Ttl---Esl---Ttl---Esl---Ttl---Esl---Ttl---Esl---Ttl---Esl---Ttl---Esl---Ttl---
 	  	case Esl:
	  	case Ttl:

		Chan = FindAndCheck(String45, Ch_a, Ch_c);

		if(Chan == 255)					//Обработка канала (если в команде он явно не указан)
		{
			if(Channel>1)	{ErrQueue(2); return;}		//Ошибка, если выбран канал В
			else Chan = Channel;					//Если номер канала не указан, то используем текущий
		}
		else Chan -= Ch_a;

		if(Chan == 2)	{ErrQueue(3);	return;}	//Ошибка, если выбран канал В


		Kin[Chan] = 1;								//Делитель вкл
		TransfWord(0b00010000001);					//Делитель вкл

		if(MesMode == Esl)		Level[Chan] = -62;  //ЭСЛ
		else 					Level[Chan] = 60;	//ТТЛ
		TransfWord(Level[Chan] | 0x400);		//Передача частотомеру уровня 

		break;
//Esl---Ttl---Esl---Ttl---Esl---Ttl---Esl---Ttl---Esl---Ttl---Esl---Ttl---Esl---Ttl---Esl---Ttl---

		default:
		break; 
	}
}
//-----------------------------------------------------

//-----------------------------------------------------
void ConfSyst(void)
{
	char Tau=255, MeasPer=255, MesMode=255, NAV=255, ALT=255, Chan=255, TempInd;//

	if(InpSTR[StartInd] == '?')		//Обработка запроса
	{
		ROMStringTX(String39[((Mode[0]>>4)+1)<<1]);					//Режим
		ROMStringTX(",");

		MesMode = Mode[0]&0b1111;
		if((Channel==Chan_B)&&(Mode[0]>47))
		{
			if(Mode[0]==(HANd+48))		MesMode = WIDe;
			if(Mode[0]==(PERiod+1+48))	MesMode = PERiod;
		}

		ROMStringTX(Point_1[Mode[0]>>4][(MesMode+1)<<1]);	//Подрежим
		ROMStringTX(",");
		ROMStringTX(String46[((MeasTime[0]+N_1m)+1)<<1]);			//T
		ROMStringTX(",");
		ROMStringTX(String46[((MeasTime[0]+N_1)+1)<<1]);			//N
		ROMStringTX(",");
		ROMStringTX(String46[((Notch[0]+N_10n)+1)<<1]);				//Tau
		ROMStringTX(",");

		Chan = Channel+Ch_a;											//Одиночные каналы
		if(Mode[0]==LRATio)			Chan = Channel+Ch_a_c;				//Отношение для LRATio
		if(Mode[0]==HRATio)			Chan = Channel+Ch_a_b;				//Отношение для HRATio
		if((Mode[0]==(HANd+48))&&(Channel==Chan_B))		Chan = Ch_b_a;	//Для B(tA)
		if((Mode[0]==(PERiod+1+48))&&(Channel==Chan_B))	Chan = Ch_b_c;	//Для B(TC)
		if(Mode[0]==(WIDe+48))		Chan = ConvTab3[Channel];			//Отношение для WIDe
		if(Mode[0]==(PERiod+48))	Chan = ConvTab4[Channel];			//Отношение для PERiod
		ROMStringTX(String45[(Chan+1)<<1]);							//Канал(ы)

		ROMStringTX("\r");
	}
	else		//Обработка установок
	{
		StartInd++;
		MesMode = FindString(String39);

		switch(MesMode)
		{
//Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---
			case Frequnce:
	
			StartInd++;
			MesMode = FindString(String35);
			switch(MesMode)
			{
				case 255:
				StartInd--;		//Тут так надо
				case DIRect:
			//Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---
		
				MeasPer = FindAndCheck(String46, N_1m, N_100);
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
		
				Mode[0] = 0;									//Измерение частоты
				if(Chan != 255)	Channel = Chan-Ch_a;			//Установка канала измерения
				if(MeasPer != 255)	MeasTime[0] = MeasPer - N_1m;	//Установка длительности измерения
	
				break;
			//Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---
		
				case LRATio:
			//LRATio---LRATio---LRATio---LRATio---LRATio---LRATio---LRATio---LRATio---LRATio---
		
				NAV = FindAndCheck(String46, N_1, N_100k);
				Chan = FindAndCheck(String45, Ch_a_c, Ch_b_a);
	
				Mode[0] = 1;									//Отношение частот
				if(Chan != 255)	Channel = (Chan-Ch_a_c)%3;	//Установка канала измерения
				if(NAV != 255)	MeasTime[0] = NAV - N_1;		//Установка количества периодов усреднения
	
				break;
			//LRATio---LRATio---LRATio---LRATio---LRATio---LRATio---LRATio---LRATio---LRATio---
		
				case HRATio:
			//HRATio---//HRATio---//HRATio---//HRATio---//HRATio---//HRATio---//HRATio---//HRATio---
		
				MeasPer = FindAndCheck(String46, N_1m, N_100);
				Chan = FindAndCheck(String45, Ch_a_b, Ch_b_c);
	
				Mode[0] = 2;										//Отношение частот
				if(Chan != 255)		Channel = (Chan-Ch_a_b)%3;	//Установка канала измерения
				if(MeasPer != 255)	MeasTime[0] = MeasPer - N_1m;	//Установка длительности измерения
	
				break;
			//HRATio---//HRATio---//HRATio---//HRATio---//HRATio---//HRATio---//HRATio---//HRATio---
		
				case _1_T:
			//_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---
	
				Tau = FindAndCheck(String46, N_10n, N_1m);
				NAV = FindAndCheck(String46, N_1, N_100k);
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
	
				Mode[0] = 3;										//F=1/T
				if(Chan != 255)		Channel = Chan-Ch_a;			//Установка канала измерения
				if(NAV != 255)		MeasTime[0] = NAV - N_1;		//Установка длительности измерения
				if(Tau != 255)		Notch[0] = Tau-N_10n;			//Установка меток времени
	
				break;
			//_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---
		
				case TACHometr:
			//TACHometr---TACHometr---TACHometr---TACHometr---TACHometr---TACHometr---TACHometr---
		
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
		
				Mode[0] = 4;										//Тахометр
				if(Chan != 255)		Channel = Chan-Ch_a;			//Установка канала измерения
	
				break;
			//TACHometr---TACHometr---TACHometr---TACHometr---TACHometr---TACHometr---TACHometr---
			}
			break;
//Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---
			case Period:
//Period---Period---Period---Period---Period---Period---Period---Period---Period------Period------Period---
			StartInd++;
			MesMode = FindString(String36);
			switch(MesMode)
			{
				case 255:
				StartInd--;		//Тут так надо
				case DIRect:
			//Period---Period---Period---Period---Period---Period---Period---Period---Period---
	
				Tau = FindAndCheck(String46, N_10n, N_1m);
				NAV = FindAndCheck(String46, N_1, N_100k);
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
	
				Mode[0] = 16;									//Измерение периода
				if(Chan != 255)	Channel = Chan-Ch_a;			//Установка канала измерения
				if(NAV != 255)		MeasTime[0] = NAV - N_1;	//Установка длительности измерения
				if(Tau != 255)		Notch[0] = Tau-N_10n;		//Установка меток времени
	
				break;
			//Period---Period---Period---Period---Period---Period---Period---Period---Period---
	
	
				case _1_F:
			//_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---
	
				MeasPer = FindAndCheck(String46, N_1m, N_100);
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
	
				Mode[0] = 17;									//Измерение периода
				if(Chan != 255)	Channel = Chan-Ch_a;			//Установка канала измерения
				if(MeasPer != 255)	MeasTime[0] = MeasPer - N_1m;//Установка длительности измерения
	
				break;
			//_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---
			}
			break;
//Period---Period---Period---Period---Period---Period---Period---Period---Period------Period------Period---
			case Width:	
//Width---Width---Width---Width---Width---Width---Width---Width---Width---Width---Width---Width---Width---
			StartInd++;
			MesMode = FindString(String37);
			switch(MesMode)
			{
				case 255:
				StartInd--;		//Тут так надо
				case DIRect:
			//Width---Width---Width---Width---Width---Width---Width---Width---Width---Width---
	
				Tau = FindAndCheck(String46, N_10n, N_1m);
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
	
				Mode[0] = 32;									//Измерение длительности
				if(Chan != 255)		Channel = Chan-Ch_a;		//Установка канала измерения
				if(Tau != 255)		Notch[0] = Tau-N_10n;		//Установка меток времени
	
				break;
			//Width---Width---Width---Width---Width---Width---Width---Width---Width---Width---
	
				case AVErage:
			//AVErage---AVErage---AVErage---AVErage---AVErage---AVErage---AVErage---AVErage---
	
				Tau = FindAndCheck(String46, N_10n, N_1m);
				NAV = FindAndCheck(String46, N_1, N_100k);
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
	
				Mode[0] = 33;									//Измерение длительности с усреднением
				if(Chan != 255)		Channel = Chan-Ch_a;		//Установка канала измерения
				if(NAV != 255)		MeasTime[0] = NAV - N_1;	//Установка длительности измерения
				if(Tau != 255)		Notch[0] = Tau-N_10n;		//Установка меток времени
	
				break;
			//AVErage---AVErage---AVErage---AVErage---AVErage---AVErage---AVErage---AVErage---
	
				case HDEFinition:
			//HDEFinition---HDEFinition---HDEFinition---HDEFinition---HDEFinition---HDEFinition---
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
		
				Mode[0] = 34;										//Измерение длительности с повышенной точностью
				if(Chan != 255)		Channel = Chan-Ch_a;			//Установка канала измерения
	
				break;
			//HDEFinition---HDEFinition---HDEFinition---HDEFinition---HDEFinition---HDEFinition---
	
				case INTerval:
			//INTerval---INTerval---INTerval---INTerval---INTerval---INTerval---INTerval---
	
				Tau = FindAndCheck(String46, N_10n, N_1m);
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
	
				Mode[0] = 35;									//Измерение временного интервала
				if(Chan != 255)		Channel = Chan-Ch_a;		//Установка канала измерения
				if(Tau != 255)		Notch[0] = Tau-N_10n;		//Установка меток времени
	
				break;
			//INTerval---INTerval---INTerval---INTerval---INTerval---INTerval---INTerval---
	
				case DCYCle:
			//DCYCle---DCYCle---DCYCle---DCYCle---DCYCle---DCYCle---DCYCle---DCYCle---DCYCle---
	
				Tau = FindAndCheck(String46, N_10n, N_1m);
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
	
				Mode[0] = 36;									//Измерение скважности
				if(Chan != 255)		Channel = Chan-Ch_a;		//Установка канала измерения
				if(Tau != 255)		Notch[0] = Tau-N_10n;		//Установка меток времени
	
				break;
			//DCYCle---DCYCle---DCYCle---DCYCle---DCYCle---DCYCle---DCYCle---DCYCle---DCYCle---
	
				case PHASe:
			//PHASe---PHASe---PHASe---PHASe---PHASe---PHASe---PHASe---PHASe---PHASe---PHASe---
	
				Tau = FindAndCheck(String46, N_10n, N_1m);
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
	
				Mode[0] = 37;									//Измерение фазы
				if(Chan != 255)		Channel = Chan-Ch_a;		//Установка канала измерения
				if(Tau != 255)		Notch[0] = Tau-N_10n;		//Установка меток времени
	
				break;
			//PHASe---PHASe---PHASe---PHASe---PHASe---PHASe---PHASe---PHASe---PHASe---PHASe---
			}
			break;
//Width---Width---Width---Width---Width---Width---Width---Width---Width---Width---Width---Width---Width---
			case Cnt_M:	
//CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---
			StartInd++;
			MesMode = FindString(String38);
			switch(MesMode)
			{
				case HANd:
			//HANd---HANd---HANd---HANd---HANd---HANd---HANd---HANd---HANd---HANd---
	
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
		
				Mode[0] = 48;										//Счет импульсов с ручным запуском
				if(Chan != 255)		Channel = Chan-Ch_a;			//Установка канала измерения
	
				break;
			//HANd---HANd---HANd---HANd---HANd---HANd---HANd---HANd---HANd---HANd---
	
				case WIDe:
			//WIDe---WIDe---WIDe---WIDe---WIDe---WIDe---WIDe---WIDe---WIDe---WIDe---
	
				Chan = FindAndCheck(String45, Ch_a_c, Ch_b_c);
		
				if(Chan == Ch_b_a)	Mode[0] = 48;					//Счет импульсов за время импульса(это находится в ручном режиме)
				else 				Mode[0] = 49;					//Счет импульсов за время импульса
	
				if(Chan != 255)		Channel = ConvTab[Chan-Ch_a_c];	//Установка канала измерения
	
				break;
			//WIDe---WIDe---WIDe---WIDe---WIDe---WIDe---WIDe---WIDe---WIDe---WIDe---
	
				case PERiod:
			//PERiod---PERiod---PERiod---PERiod---PERiod---PERiod---PERiod---PERiod---
	
				NAV = FindAndCheck(String46, N_1, N_100k);
				Chan = FindAndCheck(String45, Ch_a_c, Ch_b_c);
		
				if(Chan == Ch_b_c)	Mode[0] = 51;					//Счет импульсов за период импульса (это находится в отдельном режиме)
				else 				Mode[0] = 50;					//Счет импульсов за период импульса
	
				if(Chan != 255)	Channel = ConvTab[Chan-Ch_a_c];	//Установка канала измерения
				if(NAV != 255)		MeasTime[0] = NAV - N_1;	//Установка длительности измерения
	
				break;
			//PERiod---PERiod---PERiod---PERiod---PERiod---PERiod---PERiod---PERiod---
			}
			break;
//CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---
	
			default:
			ErrQueue(4);
			return;
		}
	TransfAll();	//Обновление всех параметров на плате частотомера
	}
}
//-----------------------------------------------------

//--------------------------------------------------------------------
void BinToBinDec (struct RES * Res)// signed long Data, char DecPoint
{

	signed char ST, DecPoint;
	char a, b, not=0;
	unsigned long Temp, Temp2;
 	long Data = Res->Data;

//************************
	if((Data>999999999)||(Data<-99999999))
	{
		Decades[0] = 'O';
		Decades[1] = 'v';
		Decades[2] = 'e';
		Decades[3] = 'r';
		Decades[4] = 'f';
		Decades[5] = 'l';
		Decades[6] = 'o';
		Decades[7] = 'w';
		Decades[8] = 0;
		return;
	}
//************************


	Temp = 100000000;
	Temp2 = Data;

	a=0;
	if(Data<0)
	{
		not = 1;
		a=1;
		Data *= -1;
		Temp /=10;
		Temp2 = Data;
	}



	ST = Res->Order%3;
	if(ST<0)	ST += 3;
	b = Order(Data)-1;

	DecPoint = b-(b+ST)%3;

	Res->IndOrder = (b+Res->Order)/3;

	if((b+Res->Order)%3)
	{
		if((b+Res->Order)<0)	Res->IndOrder -= 1;
	}

	if(DecPoint<0)	{DecPoint +=3; Res->IndOrder += 1;}//)&&(a<2)
	DecPoint = 9-DecPoint;


	for(b=0;b<10;b++)	Decades[b] = ' ';


			//Выводим значащие символы
	for(; a<10; a++)
	{
		if(a==DecPoint)
		{
			Decades[a] = '.';
			if(Decades[a-1] == 0x16) Decades[a-1] = '0';
		}
		else
		{
			if(Temp)b = Temp2/Temp;
			else b = 0;

			if((Data>=Temp) || (a>=DecPoint))
			{
				Decades[a] = b + 0x30;
				if(not == 1)
				{
					Decades[a-1] = '-';
					not = 0;
				}
			}
			else Decades[a] = 0x16;

			Temp2 = Temp2%Temp;
			Temp /=10;
		}
	}
	if(DecPoint==9)
	{
		Decades[9] = 0x16;
		if(Data==0)	Decades[8] = '0';
	}
	Decades[10] = 'E';
	if(Res->IndOrder<0)	Decades[11] = '-';
	else	Decades[11] = '+';

	Decades[12] = '0';
	ST = Res->IndOrder*3;
	if(ST<0) ST *= -1;


	Decades[13] = ST+0x30;
	Decades[14] = 0;

}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
signed short StringToBin (void)
{
	char Temp=0, i=0, Minus = 0;
	short Ans=0, Factor;
	
	if(InpSTR[StartInd] == '-')
	{
		Minus = 1;
		StartInd++;
	}

	Temp = InpSTR[StartInd + i];
	if(!((Temp >= '0')&&(Temp <= '9')))		return 0x4000;	//выход если не обнаружена цифра//!((InpSTR[StartInd+i] > 0x2F) && (InpSTR[StartInd+i] < 0x3A))

	while((Temp != LineFeed) && (Temp >= '0')&&(Temp <= '9') && (StartInd + i)<128)//(Temp != ',')
	{
		Temp = InpSTR[StartInd + i];
		i++;
	}
	i--;

	Temp = StartInd;
	StartInd += i;
	Factor = 1;

	while(i)
	{
		Ans += (InpSTR[Temp+i-1] - 0x30) * Factor;
		Factor *= 10;
		i--;
	}

	if(Minus)	Ans *= -1;
	return Ans;
}
//--------------------------------------------------------------------
//-----------------------------------------------------
//Функция сравнения переменной Data с каждым элементом массива ComArray
char Compare (char Data, near rom unsigned char * ComArray)
{
	char Temp = *ComArray;


	while(Temp)
	{
		ComArray++;
		if(*ComArray == Data)
		{
			Temp = 1;
			break;
		}
	Temp--;
	}
	return Temp;
}
//-----------------------------------------------------
//-----------------------------------------------------
//Функция подсчета числа значащих цифр
char Order (long Data)
{
	char Temp;
	for(Temp = 0;Data>9; Temp++)		//	for(Temp = 0;Data!=0; Temp++)
	{
		Data/=10;
	}

	return Temp+1;
}
//-----------------------------------------------------
//-----------------------------------------------------
void TransfAll (void) 	//Передача на плату частотомера всех настроек
{
	TransfWord (0b00000000000 | Channel);			//Канал А
	TransfWord (0b00001000000 | Rin[Channel]);			//
	TransfWord (0b00010000000 | Kin[Channel]);			//
	TransfWord (0b00011000000 | OpenClose[Channel]);	//
	TransfWord (0b00100000000 | Rise[Channel]);			//
	TransfWord (0b00101000000 | LF[Channel]);			//
	TransfWord (0b00110000000 | Mode[0]);				//
	TransfWord (0b00111000000 | Notch[0]);				//
	TransfWord (0b01000000000 | MeasTime[0]);			//
	TransfWord (0b01001000000 | Generator[0]);			//
	TransfWord (0b01010000000 | Starting[0]);			//
	TransfWord (0b01011000000 | IndTime[0]);			//
	TransfWord (0b01100000000 | Memory[0]);				//
	TransfWord (0b01101000000 | Synch[Channel]);		//

	TransfWord (0b00110000000 | Mode[0]);				//
}
//*-----------------------------------------------------*/
//-----------------------------------------------------
//Функция вывода сообщений об ошибках
void MesErr (char ER)
{
	Text_To_Display(0, 0, "Ошибка");
	Rectangle (0, 9, 1, 1, 0);
	ER = Nomber(10, 1, 3, ER);
	Rectangle (ER, 35, 1, 1, 0);
	Delay10KTCYx(255);
	Delay10KTCYx(255);
	Delay10KTCYx(255);
	Delay10KTCYx(255);
	Delay10KTCYx(255);
	ErrReg = 0;
}
//-----------------------------------------------------

//-----------------------------------------------------
	//Возведение Dat в степень deg
long degree(long Dat, char deg)
{
	while(deg)
	{
		Dat *= 10;
		deg--;
	}
	return Dat;
}
//-----------------------------------------------------
//-----------------------------------------------------
	//Возведение Dat в степень deg
float FloatDegree(signed char deg)
{
	float Dat = 1;
	
		if(deg>0)
		{
			while(deg)
			{
				Dat *= 10;
				deg--;
			}
		}
		else
		{
			while(deg)
			{
				Dat /= 10;
				deg++;
			}
		}
	return Dat;
}
//-----------------------------------------------------
//-----------------------------------------------------
	//Отображение на индикаторе результата измерения
void IndRes(void)
{
	signed char TempR, TempOr;

	INTCONbits.TMR0IE = 0;		//Запрет прерывания от модуля TMR0
	Nomber_Image_Char(72, 3, &RES_Dat, SignDig);			//Отображение на индикаторе результата измерения
	INTCONbits.TMR0IE = 1;		//Разрешение прерывания от модуля TMR0

	if(Compare(Mode[0], CNT_Const)) TempR = 3;
	else TempR = Mode[0]>>4;


	if(Mode[0]==34 && Flags.Test)	Text_To_Display(238, 5, "   ");
	else							Text_To_Display(238, 5, MeasLable[TempR][RES_Dat.IndOrder+4]);		//Отображаем единицы измерения
}
//-----------------------------------------------------

char FindAllDiction	(char Ind)
{
	char Temp, flag = 1, i;

	for(i=0; i<18; i++)
	{
		StartInd = Ind;
		Temp = FindString(Point_5[i]);	//Определяем записываемые данные
		if(Temp != 255)		flag = 0;	//Совпадение
	}

	if(flag==0)
	{
		StartInd = Ind-1;
		return 0; //Совпадение
	}
	else 	return 1; //Нет совпадения
}

//-----------------------------------------------------
char FindAndCheck (rom char *rom *Pointer, char min, char max)
{
	char TempInd, Temp;

	if(InpSTR[StartInd] != LineFeed)	//Проверяем конец строки
	{
		StartInd++;
		TempInd = StartInd;
		Temp = FindString(Pointer);	//Определяем номер канала
		if(Temp == 255)		{if(FindAllDiction(TempInd)) 	{ErrQueue(0);	return 255;}}	//Синтаксическая ошибка
		else {if(!((Temp <= max)&&(Temp >= min)))			{ErrQueue(3);	StartInd = TempInd-1;	return 255;}}	//Недопустимый параметр

		return Temp;
	}

	return 255;
}
//-----------------------------------------------------
/*//-----------------------------------------------------*/




