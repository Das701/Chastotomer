//#include <p18f4520.h>
//#include <timers.h>

//__CONFIG(1, FOSC_INTIO67 & FCMEN_OFF & IESO_OFF);
void Time (void);
void KeyRefresh(char);
void IntFlags (void);
void IntFlagsHi (void);
void Int_0 (void);
void Int_1 (void);
void ParamIndicate(char, char, char);
void MeasureScreen (void);
void ChannelPrest (void);
void Menu (void);
void ModeImage (void);
void MeasureTimer (void);
void TransfWord (unsigned int);
void ReceiveWord(void);
void Selection (char);
void CalibrateMode (void);
void UsRX(void);

unsigned long Cnt=0, Interpol;
signed long  Interpols;

unsigned int Timer1, Slider = 0, ReceiveData = 512;
float Fl_Result;

int ENC_max, ENC_min, EncoderCount_16, *EncoderPointer_16 = &EncoderCount_16;//
char EncoderCount = 0, *EncoderPointer = &EncoderCount, Timer3;
char a, b, c, ModeIndex = 0, ModeScreen=1, ModeScreenPre, Channel_2, ChannelIndex, ErrReg=0;
char FuncKeyIndex;
char Keyboard_Previous, Keyboard_Previous_2, KeyTemp, Keyboard = 0, SignDig;
 
short N1 = 439, N2 = 774;
short ArrayCnt = 0;

char InpSTR[128], InpComm[16], CommIndex, STRIndex = 0, StartInd;
//char Kostyl;
char Decades[16];
//double d=5.0;

short Timer2=0;

//char buf[50];

#define LineFeed	10

#include "Pointers.c"
#include "Functions.c"
#include "GP1157.c"

#define KeyTRIS		0b00001111
#define DispTRIS	0b00000000


#define row_1		PORTDbits.RD4
#define row_2		PORTDbits.RD5
#define row_3		PORTDbits.RD6

#define column_1	PORTDbits.RD0
#define column_2	PORTDbits.RD1
#define column_3	PORTDbits.RD2
#define column_4	PORTDbits.RD3

#define  Jerk_2		PORTBbits.RB2
#define  Jerk		PORTBbits.RB3
#define ENC1		PORTBbits.RB0
#define ENC2		PORTBbits.RB1
#define ENCBUT		PORTCbits.RC2

#define V13			PORTBbits.RB5
#define RDY2		PORTBbits.RB4
#define BOUT		PORTBbits.RB3
#define BSHT		PORTCbits.RC4
#define MDO			PORTCbits.RC3
#define ISHT		PORTCbits.RC5

#define BUSY		PORTBbits.RB2

#define Key_1		0
#define Key_2		1
#define Key_3		2
#define Key_4		3
#define Key_5		4
#define Key_6		5
#define AutoKey		6
#define TestKey		7
#define ChannelsKey	8
#define ModesKey	9
#define IndicateKey	10
#define EncKey		11
#define Free		12
//#define Encoder_Key	7
/*
//-------------------------------------------------------
#pragma code INTERRUPT_VECTOR = 0x000018			//Размещаем функцию по 18 адресу
void inter(void)
{
  _asm GOTO IntFlags _endasm
}
#pragma code
//-------------------------------------------------------
#pragma interrupt IntFlags		//Указываем функцию, в которой будет сохраняться контекст прерывания
void IntFlags (void) 	//Проверка флагов прерывания
{
	if(INTCONbits.TMR0IF && INTCONbits.TMR0IE)		Time();
//	if(PIE1bits.RCIE && PIR1bits.RCIF)				UsRX();
//	if(INTCON3bits.INT1IF && INTCON3bits.INT1IE)	Int_1();
//	if(INTCONbits.INT0IF && INTCONbits.INT0IE)		Int_0();

}
//-----------------------------------------------------
*/
//-------------------------------------------------------
#pragma code INTERRUPT_VECTOR_HI = 0x08			//Размещаем функцию по 8 адресу
void interHi(void)
{
  _asm GOTO IntFlagsHi _endasm
}
#pragma code
//-------------------------------------------------------
#pragma interrupt IntFlagsHi		//Указываем функцию, в которой будет сохраняться контекст прерывания
void IntFlagsHi (void) 	//Проверка флагов прерывания
{
	if(INTCONbits.TMR0IF && INTCONbits.TMR0IE)		Time();
	if(PIE1bits.RCIE && PIR1bits.RCIF)				UsRX();
	if(INTCON3bits.INT1IF && INTCON3bits.INT1IE)	Int_1();
	if(INTCONbits.INT0IF && INTCONbits.INT0IE)		Int_0();
}
//-----------------------------------------------------
//-----------------------------------------------------
void Int_0 (void) 	//Обработка прерывания от 
{
 	INTCONbits.INT0IF = 0;
	INTCON3bits.INT1IF = 0;

	INTCONbits.INT0IE=0;
	INTCON3bits.INT1IE=1;

	if(ENC2)	INTCON2bits.INTEDG1=0;
	else		INTCON2bits.INTEDG1=1;


	if(INTCON2bits.INTEDG0)
	{
		if(ENC2)
		{
			if(*EncoderPointer>ENC_min)
			{
				(*EncoderPointer)--;
				Flags.ENC_Event=1;				//Установка флага события энкодера
			}
			else	(*EncoderPointer) = ENC_min;
			if(*EncoderPointer_16>ENC_min)
			{
				(*EncoderPointer_16)--;
				Flags.ENC_Event=1;				//Установка флага события энкодера
			}
			else (*EncoderPointer_16) = ENC_min;
		}
		else
		{
			if(*EncoderPointer<ENC_max)	
			{
				(*EncoderPointer)++;
				Flags.ENC_Event=1;				//Установка флага события энкодера
			}
			else (*EncoderPointer) = ENC_max;
			if(*EncoderPointer_16<ENC_max)
			{
				(*EncoderPointer_16)++;
				Flags.ENC_Event=1;				//Установка флага события энкодера
			}
			else (*EncoderPointer_16) = ENC_max;
		}
		Flags.Control_Event=1;
	}
}
//-----------------------------------------------------
//-----------------------------------------------------
void Int_1 (void) 	//Обработка прерывания от 
{
	INTCON3bits.INT1IF = 0;
 	INTCONbits.INT0IF = 0;

	INTCONbits.INT0IE=1;
	INTCON3bits.INT1IE=0;

	if(ENC1)	INTCON2bits.INTEDG0=0;
	else		INTCON2bits.INTEDG0=1;
}
//-----------------------------------------------------
//-----------------------------------------------------
void UsRX(void)
{
	char Temp;

	if(RCSTAbits.OERR)	//Исправление ошибки переполнения
	{
		RCREG;
		RCREG;
		RCSTAbits.CREN = 0;
		RCSTAbits.CREN = 1;
		ErrReg |= 32;
		return;
	}

	if(RCSTAbits.FERR)
	{
		ErrReg |= 64;
		RCREG;	//Исправление ошибки кадра
		return;
	}

	Temp = RCREG;

	if(Temp>0x40 && Temp<0x5B)	Temp += 0x20;	//Преобразование к нижнему регистру

	InpSTR[STRIndex] = Temp;

	if(STRIndex<127) STRIndex++;

	if(Temp != LineFeed) return;

	for(; STRIndex<128; STRIndex++)	InpSTR[STRIndex] = 0;	//Заполнение нулями остатка массива

	STRIndex = 0;
	StartInd = 0;
	CommIndex = 0;

	Flags.ReceiveFlag = 1;		//Флаг готовности оьработки принятой строки

//	BinToBinDec(123456789, 9);

}
//-----------------------------------------------------
//-----------------------------------------------------
void Time (void) 	//Обработка прерывания от таймера TMR0
{
	char Counter;

	INTCONbits.TMR0IF = 0;	//Сброс флага прерывания от TMR0

	TMR0H = 0x3C;
	TMR0L = 0xB0;



	Slider -=ConstTime[MeasTime[0]];
	if(Slider > 4095) Slider = 0;//328	 ConstTime[MeasTime[0]];

	if(Timer1!=0)	Timer1--;
	if(Timer2!=0)	Timer2--;
	if(Timer3!=0)	Timer3--;

	if(V13)
	{
		Timer3 = 2;
		if(Slider == 0)Slider = 4096;
	}
	else Slider = 0;

	ReceiveWord();

	Cnt++;

	Counter = 0;
	KeyTemp = Free;

	row_1 = 0;
	row_2 = 1;
	row_3 = 1;
	Delay10TCYx(2);

	if(column_1 == 0)	{KeyTemp = Key_2;		Counter++;}
	if(column_2 == 0)	{KeyTemp = ChannelsKey;	Counter++;}
	if(column_3 == 0)	{KeyTemp = Key_1;		Counter++;}
	if(column_4 == 0)	{KeyTemp = 12;			Counter++;}

	row_1 = 1;
	row_2 = 0;
	row_3 = 1;
	Delay10TCYx(2);

	if(column_1 == 0)	{KeyTemp = Key_6;	Counter++;}
	if(column_2 == 0)	{KeyTemp = Key_5;	Counter++;}
	if(column_3 == 0)	{KeyTemp = Key_4;	Counter++;}
	if(column_4 == 0)	{KeyTemp = Key_3;	Counter++;}

	row_1 = 1;
	row_2 = 1;
	row_3 = 0;
	Delay10TCYx(2);

	if(column_1 == 0)	{KeyTemp = TestKey;		Counter++;}
	if(column_2 == 0)	{KeyTemp = AutoKey;		Counter++;}
	if(column_3 == 0)	{KeyTemp = IndicateKey;	Counter++;}
	if(column_4 == 0)	{KeyTemp = ModesKey;	Counter++;}

	if(ENCBUT == 0)		KeyTemp = EncKey;



	if(KeyTemp == Free)		Keyboard_Previous = KeyTemp;
	else
	{
		if(Flags.SoftStart)
		{
			Flags.SoftStart = 0;				//Сброс флага активности программного запуска
			TransfWord(0b01010000000);			//Передача частотомеру команды ВНУТРЕННИЙ РЕЖИМ ЗАПУСКА
			Starting[0]=0;
		}
	}



	if(Keyboard_Previous != KeyTemp)
	{
		Flags.Control_Event = 1;
		Flags.Key_Event = 1;

		if(Keyboard_Previous_2 != KeyTemp)	Flags.Change_Event = 1;
		Keyboard_Previous_2 = KeyTemp;
		Keyboard_Previous = KeyTemp;
		Keyboard = KeyTemp;

		if(Keyboard < MenuSize[ModeIndex])	Flags.FunKey_Event = 1;	//Если была нажата функциональная клавиша, то устанавливаем соответствующий флаг
	}
	if(Counter > 1) ErrReg |= 1;
	else			ErrReg &= ~1; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
//-----------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------
void KeyRefresh(char FKey)
{
	char a, b, TempMode;

	//***************************
	if(Flags.Test && (ModeIndex == 0))	TempMode = 1;			//Если выбран режим ТЕСТ, то при нажатии на клавишу КАНАЛЫ рисуем клавиши РЕЖИМа
	else 								TempMode = ModeIndex;

	for(a=0; a<6; a++)
	{
		Level_1[a] = F0[TempMode][a];				//Копируем в массив указателей на структуры клавиш массив константных указателей
	}

	MenuSize[0]	=	6;							//Настраиваем количество клавиш
	MenuSize[1]	=	6;

		//*****************
    if((ModeIndex == 0) && !Flags.Test)            //Если выбран режим КАНАЛЫ (в режиме измерения)
    {
        if(Channel == Chan_B)   MenuSize[0]   = 0;   // и активным каналом является канал В, то указываем нулевое количество кнопок (т.е. их не рисуем)
    }

	if(Channel != Chan_B)
	{
    	if((Mode[0] == 3)||(Mode[0] == 1))	Level_1[5] = &STR14_1;		//Меняем клавишу ВРЕМЯ на N
    	if(Mode[0] == 17)
		{
			Level_1[4] = &STR15;		//Меняем клавишу МЕТКИ на ВРЕМЯ
			MenuSize[1] = 5;			//Уменьшаем количество клавиш до пяти
		}
	}



	if(TempMode == 1)	//Если выбран режим РЕЖИМ
	{
		if((Mode[0] == 33)||(Mode[0] == 16))
		{
		 	Level_1[5] = &STR14_1;		//Меняем клавишу ВРЕМЯ на N
			if(Mode[0] == 33)	Level_1[5] = &STR14_2;	//Меняем клавишу N в режиме длительности с усреднением
		}
		if(((Mode[0] & 240) == 32) && (Mode[0]!=33)) 		MenuSize[1] = 5;			//В режиме ДЛИТЕЛЬНОСТЬ уменьшаем количество клавиш до пяти
		if((Mode[0] > 47)&&(Mode[0] < 52))					MenuSize[1] = 4;	//В режиме СЧЕТ,уменьшаем количество клавиш до четырех
		if(Mode[0] == 34)	MenuSize[1] = 4;			//Уменьшаем количество клавиш до четырех (в режиме с интерполятором)

		if(Mode[0]== 50)
		{
			MenuSize[1] = 5;	//В режиме СЧЕТ, РУЧНОЙ увеличиваем количество клавиш до пяти
			Level_1[4] = &STR14_1;
		}

		switch(Channel)							//Осуществляем более детальную настройку клавиш
		{
			case Chan_A:		//Канал А
			Level_1[0] = &STR10;		//Режим измерения частоты
			Level_1[2] = &STR12;		//Режим измерения длительности
			Level_1[3] = &STR13;		//Режим счета
			break;
	
			case Chan_B:		//Канал В
			MenuSize[1] = 3;			//Уменьшаем количество клавиш до трех
			Level_1[0] = &STR10_1;		//Режим измерения частоты
			Level_1[1] = &STR13_1;		//Режим счета
			Level_1[2] = &STR15;		//Время

			if(Compare(Mode[0], N_B_Const))	//Если выбран режим счет по периоду или отношение частот
			Level_1[2] = &STR14_1;		//Меняем клавишу ВРЕМЯ на N

			if((Mode[0] == 48) || (Mode[0] == 49))	//Если выбран режим счет по длительности
			MenuSize[1] = 2;			//Уменьшаем количество клавиш до двух

			break;

    		case Chan_C:		//Канал С
			Level_1[0] = &STR10_2;		//Режим измерения частоты
			Level_1[2] = &STR12_1;		//Режим измерения длительности
			Level_1[3] = &STR13_2;		//Режим счета
			break; 

			default:
			break; 
		}
	}
	//***************************

	for(a=0; a<MenuSize[TempMode]; a++)		//Рисуем все функциональные клавиши согласно указателю Level_1
	{
		Image(a*36+38, 7, Level_1[a]->KeysImg);
	}
	if(FKey<MenuSize[TempMode])
	{
		Command_To_Display(MIXTURE_XOR);
		Image(FKey*36+38, 7, NOT);
		Command_To_Display(MIXTURE_NORM);
	}
	//***************************

	Rectangle (MenuSize[TempMode]*36+37, 255, 7, 7, 0);	//На индикаторе затираем последние функциональные клавиши
}
//-----------------------------------------------------
//-----------------------------------------------------
void ParamIndicate(char x, char y, char Chan)
{
	switch(Chan)
	{
		case Chan_A:
		a = Text_To_Display(x, y, "A: ");   
		break;
	
		case Chan_C:
		a = Text_To_Display(x, y, "C: ");  
		break;
           
		case Chan_B:
		a = Text_To_Display(x, y, "B: ");
		break;
          
		default:
		break; 
	}

	if(Flags.Test)		
	{
		Text_To_Display(a, y, "-----Режим тест-----  ");   
		return;
	}

	if(Chan == Chan_B)	
	{
		a = Text_To_Display(a, y, ",D:");
		Rectangle (a, 194, y, y, 0);
 		return;
	}

	switch(OpenClose[Chan])
	{
		case 0:
		a = Image(a, y, image_1);
		break;
	
		case 1:
		a = Image(a, y, image_2);
		break;
           
		default:
		break; 
	}

Rectangle (a, a+2, y, y, 0);
a +=2;

	Command_To_Display(SET_2);
	switch(Rin[Chan])
	{
		case 0:
		a = Text_To_Display(a, y, "1M"); 
		break;
	
		case 1:
		a = Text_To_Display(a, y, "50");
		break;
           
		default:
		break; 
	}
	Write_To_Display(0xEA);  
	Command_To_Display(SET_1);

		Write_To_Display(',');
		a +=12;

	switch(LF[Chan])
	{
		case 0:
		a = Text_To_Display(a, y, "ФНЧ,");  
		break;

		default:
		break; 
	}

	switch(Rise[Chan])
	{
		case 0:
		a = Image(a, y, image_4);
		break;
	
		case 1:
		a = Image(a, y, image_5);
		break;
           
		default:
		break; 
	}

	Rectangle (a, a+3, y, y, 0);
	a +=4;


	if(Kin[Chan])	a = Image(a, y, image_3);

	Rectangle (a, a+3, y, y, 0);
	a +=4;

		if(Kin[Chan] == 0)	a = Nomber(a, y, 4, Level[Chan]*2);  //
		else				a = Nomber(a, y, 4, Level[Chan]*20);  //
		Rectangle (a, a+3, y, y, 0);
		a +=4;
		a = Text_To_Display(a, y, "mV."); 

	Rectangle (a, 194, y, y, 0);
}
//-----------------------------------------------------
//-----------------------------------------------------
	//Функция отображения на индикаторе изображения, символизирующего текущий режим работы
void ModeImage (void)
{
	switch(Mode[0]>>4)
	{
		case 0:
		Image(0, 0, FreqLab);
		break;
		
		case 1:
		Image(0, 0, PerLab);
		break;
	
	  	case 2:
		Image(0, 0, TauLab);
		break; 
	
	    case 3:
		Image(0, 0, CountLab);
		break; 
	
		default:
		break; 
	}
}
//-----------------------------------------------------
//-----------------------------------------------------
	//Функция отображения на индикаторе времени измерения
void MeasureTimer (void)	
{
		if(Compare(Mode[0], Ave_Const)||((Mode[0] == 2)&&(Channel == Chan_B)))								//Нужно ли отображать число усреднений?
		{
			a = Text_To_Display(X0+1, 6, String10_1[MeasTime[0]]);	//Пишем число усреднений
				Slider = 3968;			//Ничего не пишем
		}
		else
		{
			if((Mode[0] < 3)|| (Mode[0] == 17))		a = Text_To_Display(X0+1, 6, String11[MeasTime[0]]);	//Пишем время измерения
			else
			{
				a = X0+1;
				Slider = 3968;			//Ничего не пишем
			}
		}
		if(Mode[0] == 33)
		{
			Slider = 3968;
		}

		Rectangle (X0+1, X1-1-((Slider>>7)&31), Y1>>3, Y1>>3, 0b01111110);
		Rectangle (X1-((Slider>>7)&31), X1-1, Y1>>3, Y1>>3, 0b01000010);

		Rectangle (a, X1-1, 6, 6, 0);
		Line (X0, X0, Y0, Y2);
		Line (X1, X1, Y0, Y2);
		Line (X0, X1, Y0, Y0);
}
//-----------------------------------------------------
//-----------------------------------------------------
	//Функция навигации и обработки клавиш меню
void Menu (void)
{
	if(Flags.Test && (ModeIndex == 0))	ModeIndex = 1;			//Если выбран режим ТЕСТ и выбран режим КАНАЛЫ, то при переключаемся на режим РЕЖИМ

	if((Flags.FunKey_Event || Flags.ENC_Event) && (Keyboard < MenuSize[ModeIndex]))
	{
		FuncKeyIndex = Keyboard;
	//***************************

					//Отрабатываем первое нажатие функциональной клавиши
		if(Flags.Change_Event && Flags.FunKey_Event)
		{
			Flags.Change_Event = 0;		//Сброс флага нажатия новой клавиши
			Flags.FunKey_Event = 0;		//Сброс флага нажатия функциональной клавиши
			ENC_max = Level_1[FuncKeyIndex]->MAX_1;		//Устанавливаем максимальное значение энкодера
			ENC_min = Level_1[FuncKeyIndex]->MIN_1;		//Устанавливаем минимальное значение энкодера
			EncoderPointer = &(Level_1[FuncKeyIndex]->Data[ChannelIndex]);			//Указываем энкодеру с какой переменной он будет работать


			a = Text_To_Display(40, 1, Level_1[FuncKeyIndex]->String1_lng); 		//Вывод длинного названия клавиши
			Rectangle (a, 255, 1, 1, 0);

			Selection (Level_1[FuncKeyIndex]->Data[ChannelIndex]);
		}
	//***************************

						//Отрабатываем последующие нажатия функциональной клавиши		
		if((Flags.Change_Event ==0) && Flags.FunKey_Event) 
		{
			Flags.FunKey_Event = 0;		//Сброс флага нажатия функциональной клавиши

			a = Level_1[FuncKeyIndex]->Data[ChannelIndex];

			if(a < Level_1[FuncKeyIndex]->MAX_1)	a += 1;
			else a = Level_1[FuncKeyIndex]->MIN_1;

			Selection (a);

		}
	//***************************

						//Отрабатываем события энкодера
		if((Flags.Change_Event ==0) && Flags.ENC_Event) 
		{
			Flags.ENC_Event = 0;		//Сброс флага изменения состояния энкодера

			Selection (Level_1[FuncKeyIndex]->Data[ChannelIndex]);

		}
	//***************************
	}
}
//-----------------------------------------------------
//-----------------------------------------------------
void Selection (char data)	//Функция ограничения, сохранения и отображения переменной, а так же вывода соответствующей ей команды на частотомер
{
	char Temp;
	if((data < Level_1[FuncKeyIndex]->MIN_1) || (data > Level_1[FuncKeyIndex]->MAX_1))	//Убеждаемся что переменная попадает в заданный интервал
	{
		data = Level_1[FuncKeyIndex]->MIN_1;
		Level_1[FuncKeyIndex]->Data[ChannelIndex] = data;		//Если не попадает, то делаем ее минимальной
	}
	Level_1[FuncKeyIndex]->Data[ChannelIndex] = data;			//Сохраняем переменную

	Temp = Level_1[FuncKeyIndex]->Command;						//Считываем команду

	if(Temp==0x0D)		//Фикс. уровень синхр
	{
			Flags.RefrCreen_2 = 1;							//Установка флага второстепенного обновления экрана
			Kin[Channel] = 1;								//Делитель вкл
			TransfWord(0b00010000001);						//Делитель вкл

			if(Synch[Channel]==0)	Level[Channel] = 60;	//ТТЛ
			else 					Level[Channel] = -62;	//ЭСЛ

			TransfWord(Level[Channel] | 0x400);				//Передача частотомеру данных об уровне синхронизации
	}
	else
	{
		if(Temp>0x0E)
		{
			Flags.DataReceive = 0;				//Сброс флага принятых данных
	 		TransfWord((Temp & 0x0F) | 0x3C0);	//Передача команды частотомеру
		}
		else TransfWord((data & 63) | (Temp<<6) & 1023);	//Передача команды частотомеру
	}




	Cursor_Set(1, 3);				//Установка курсора
	Write_To_Display(':');			//Запись символа )
	data = Text_To_Display(8, 3, Level_1[FuncKeyIndex]->StateLable[data & 15]);//Отображаем соответствующую переменной строку
	Rectangle (data, 70, 3, 3, 0);		//Доочищаем строку

	KeyRefresh(FuncKeyIndex);		//Обновляем изображение клавиш, одна из которых в нажатом виде
}
//-----------------------------------------------------
//-----------------------------------------------------
void MeasureScreen (void)
{	//***************************
	if(Flags.RefrCreen)
	{
		if(!(Compare(Mode[0], TEST_Const)))		//Отключение режима ТЕСТ (если в текущем режиме он запрещен)
		{
			Flags.Test = 0;	
			TransfWord(0b01111000000);			//Передача частотомеру команды ИЗМЕРЕНИЕ
		}

		Flags.RefrCreen = 0;

		Flags.Key_Event = 0;							//Сброс флага события клавиатуры
		Flags.FunKey_Event = 0;
		Flags.Hold = 0;								//Сброс флага удержания

		EncoderPointer = &EncoderCount;		//До разрешения изменения настроек переводим указатель энкодера на безопасный регистр, стобы ненароком не напортить предыдущий регистр

		Rectangle (195, 254, 0, 2, 0);			//Очистка выделенной области

		ENC_max = 400;
		ENC_min = -400;

		FuncKeyIndex = 0;

		Rectangle (0, 60, 2, 4, 0);			//Очистка выделенной области

		KeyRefresh(255);				//Прорисовываем клавиши в ненажатом виде

		Timer2=0;			//Очищаем для обновления

		C0=20; C1=20; C2=20; C3=20; C4=20; C5=20; C6=20; C7=20; C8=20, C9=20;		//Заставим число на индикаторе целиком обновиться
	}
	//***************************
	if(Timer2==0)
	{

		Timer2 = 20;					//Задержка таймера 1с
		Flags.RefrCreen_2 = 1;			//Установка флага второстепенного обновления экрана (обновление индикации параметров)
	
		ModeImage();		//Отображения на индикаторе изображения, символизирующего текущий режим работы



		a = Text_To_Display(0, 3, (ModeString[Channel][Mode[0]>>4][Mode[0]&15]));	//Индикация подрежима работы
		Rectangle (a, 70, 3, 3, 0);					//Очистка строки

		if(((Mode[0] > 31) && (Mode[0] < 34)) || (Mode[0] == 36) || (Mode[0] == 16) || (Mode[0] == 3) || ((Mode[0] == 48) && Flags.Test))
		{
			a = Text_To_Display(0, 4, "\x98t=");	//Индикация меток времени 
			a = Text_To_Display(a, 4, String10[Notch[0]]);
			Rectangle (a, 70, 4, 4, 0);				//Доочистка строки 
		}
		else Rectangle (0, 70, 4, 4, 0);			//Очистка строки
	}
	//***************************
   	if((Channel != Chan_B) && !Flags.Test)	//Проверка условий правки уровня синхронизации
	{
		if(Flags.Key_Event && (Keyboard == EncKey)  && (Starting[0] != 2))		//Установка уровня в ноль (если не выбран ручной запуск)
		{
			Flags.Key_Event = 0;							//Сброс флага события клавиатуры
			Flags.RefrCreen_2 = 1;							//Установка флага второстепенного обновления экрана
			Level[Channel] = 0;
			TransfWord(Level[Channel] | 0x400);				//Передача частотомеру данных об уровне синхронизации
		}
		{//***************************
	   	if(Flags.ENC_Event)	//Проверка флага события энкодера
		{
			Flags.ENC_Event = 0;							//Сброс флага события энкодера
			Flags.RefrCreen_2 = 1;							//Установка флага второстепенного обновления экрана
	
			TransfWord(Level[Channel] | 0x400);				//Передача частотомеру данных об уровне синхронизации
		}
		}//***************************
	}
//***************************
	if(Flags.DataReceive && Flags.Hold)		//Проверка флага данных и идентификатора калибровочной константы
	{
		Flags.DataReceive = 0;				//Сброс флага данных
		Timer2 = 60;			//Задержка таймера 3с

		switch(ReceiveData&7)
		{
			case 2:
			a = Text_To_Display(40, 1, "Макс ");	//Индикация максимального значения
			Nomber(a, 1, 4, ((ReceiveData>>3)-512)<<1);
			break;

			case 3:
			a = Text_To_Display(110, 1, "Мин ");	//Индикация минимального значения
			Nomber(a, 1, 4, ((ReceiveData>>3)-512)<<1);
			break;

			case 4:
			Level[Channel] = (ReceiveData>>3)-512;	//Сохранение значения уровня
			Flags.Hold = 0;								//Сброс флага удержания
			break;

			default:
			break; 
		}
	}
//***************************
    if(Flags.RefrCreen_2 == 1)											//Проверка флага второстепенного обновления экрана
	{
		Flags.RefrCreen_2 = 0;											//Сброс флага второстепенного обновления экрана
		Flags.Hold = 0;								//Сброс флага удержания

		if(Channel<2)	EncoderPointer_16 = &Level[Channel];			//Указываем энкодеру с какой переменной он будет работать
		else	EncoderPointer_16 = &EncoderCount_16;					//Указываем на безопасный регистр, если активен канал В

		if(Flags.Auto)
		{
			Text_To_Display(40, 0, " Установка уровня синхр. ");  
			Text_To_Display(40, 1, "                         "); 
			Timer2 = 600;							//Задержка таймера 20с
			Flags.Auto = 0;							//Сброс флага режима АВТО
			Flags.Hold = 1;							//Установка флага удержания
			Rectangle (72, 237, 3, 5, 0);			//Очистка области результата
			C0=20; C1=20; C2=20; C3=20; C4=20; C5=20; C6=20; C7=20; C8=20, C9=20;		//Заставим число на индикаторе целиком обновиться
			Flags.RefrCreen_2 = 0;														//Сброс флага второстепенного обновления экрана
			Flags.DataReceive = 0;				//Сброс флага данных
		}

		else ParamIndicate(40,0, Channel);	

									//Отображаем параметры активного канала
		Channel_2 = TwinChanMode[Channel][Mode[0]>>4][Mode[0] & 15];	//По таблице проверяем необходимость отображать параметры второго канала

		if(Channel_2 != 255)	ParamIndicate(40, 1, Channel_2);		//Если активны два канала, то отображаем параметры и второго канала
		else Rectangle (40, 194, 1, 1, 0);								//Если активен только один, затираем информацию на индикаторе

//***************************
	//Отображение выбранного ОГ и источника запуска
		a = 0;
		if(Starting[0] != 0)	
		{
			Text_To_Display(197, a, String20[Starting[0]-1]); 
			a++;
		}

		if(Generator[0] != 0)	
		{
			Text_To_Display(197, a, String21); 
			a++;
		}
		if((Generator[0] == 0)&&(Starting[0] == 0))	 Rectangle (195, 254, 0, 2, 0);			//Очистка выделенной области


		if(a != 0)	
		{
			a = (a<<3)+2;
			Line (195, 195, 0, a);
			Line (254, 254, 0, a);
			Line (195, 254, a, a);
		}
//***************************
	}
//***************************

	if(!Flags.Hold)				//Если установлен флаг удержания, то результат измерения не отображаем
	{
		MeasureTimer();			//Отображение на индикаторе времени измерения

//***************************
		//Калибровка режима с интерполятором
	if((Mode[0] == 34) && Flags.Test)
	{
		if((Interpol>200)&&(Interpol<700))  //10.04.18 
		N1 = Interpol;
		if((Interpol>700)&&(Interpol<1000))
		N2 = Interpol;

		RES_Dat.Data = Interpol;
		RES_Dat.Order = 0;

//		Notch[0] = 0;
	}
//***************************

		IndRes();				//Отображение на индикаторе результата измерения

		if(Timer3!=0)	Text_To_Display(249, 3, "*");
		else			Text_To_Display(249, 3, " ");
	}

	if((Memory[0] == TRUE)&&!(Compare(Mode[0], M_Const)))	Text_To_Display(240, 3, "M");
	else													Text_To_Display(240, 3, " ");

//***************************
//***************************
	if(Flags.Key_Event && (Keyboard == TestKey))
	{
		Flags.Key_Event = 0;		//Сброс флага события клавиатуры

		if(Compare(Mode[0], TEST_Const))
		{

			ModeIndex = 1;					//Функциональные клавиши РЕЖИМЫ
			KeyRefresh(255);				//Прорисовываем клавиши в ненажатом виде
			Flags.RefrCreen_2 = 1;			//Установка флага второстепенного обновления экрана (обновление индикации параметров)

			if(Flags.Test==1)
			{
				Flags.Test = 0;				//Отключение режима ТЕСТ (если был включен)
				TransfWord(0b01111000000);	//Передача частотомеру команды ИЗМЕРЕНИЕ
			}
			else
			{
				Flags.Test = 1;				//Включение режима ТЕСТ
				ModeIndex = 1;				//Отобразим клавиши РЕЖИМЫ
				TransfWord(0b01111000001);	//Передача частотомеру команды ТЕСТ
			}
		}
	}
//***************************
//***************************
	if(Flags.Key_Event && (Keyboard >= ChannelsKey) && (Keyboard <= IndicateKey))
	{
		Flags.Key_Event = 0;		//Сброс флага события клавиатуры

		if(((ModeIndex == 0) && Keyboard == ChannelsKey))			//Переключение канала
		{
			if(Channel == 0)	Channel = 2;
			else Channel--;

			TransfWord(Channel & 63);					//Передача команды частотомеру (Номер канала)
		}
		else	ModeIndex = Keyboard - ChannelsKey;

		KeyRefresh(255);				//Прорисовываем клавиши в ненажатом виде
		Flags.RefrCreen_2 = 1;			//Установка флага второстепенного обновления экрана (обновление индикации параметров)
	}
//***************************
//***************************
	if(Flags.Key_Event && (Keyboard == AutoKey) && !Flags.Hold && !Flags.Test && (Channel != Chan_B))
	{
		Flags.Key_Event = 0;		//Сброс флага события клавиатуры
		TransfWord(0b01111000011);	//Передача частотомеру команды АВТО
		Flags.Auto = 1;				//Установка флага активности режима АВТО
		Flags.RefrCreen_2 = 1;		//Установка флага второстепенного обновления экрана (обновление индикации параметров)
	}
//***************************
//***************************
	if(Flags.FunKey_Event)
	{
		Flags.RefrCreen = 1;
		ModeScreen = CHANNEL_PREST;
	}
//***************************

/*	//	if((ReceiveData>>10)==5)
		{
			char TempR;
			TempR = Nomber(0, 0, 4, Interpol);
			Rectangle (TempR, 35, 0, 0, 0);
		}
*/

	if((Mode[0]== 33)&&(MeasTime[0] == 0))	//В режиме длительность с усреднением исключаем число усреднений 1, заменяя на 10
	{
		MeasTime[0] = 1;						//Отключение режима ТЕСТ (если если выбран не подходящий режим)
		TransfWord(0b01000000001);			//Передача частотомеру команды ИЗМЕРЕНИЕ
	}

}
//-----------------------------------------------------
//-----------------------------------------------------
			//Функция передачи слова на плату частотомера
void TransfWord (unsigned int Data)
{
	char i;
	int CNT=0;

	while(BUSY)
	{
		CNT++;
		if(CNT>20000)
		{
			ErrReg |= 2;	//Установка ошибки связи	
			return;
		}
	}

	ErrReg &= ~2;	//Сброс ошибки связи

	ISHT = 0;
	MDO = 0;

	for(i=0; i<11; i++)
	{
		if(Data & 1024) MDO = 1;
		else MDO = 0;

		_asm
		nop
		nop
		nop
		_endasm

		ISHT = 1;

		_asm
		nop
		nop
		nop
		_endasm
		ISHT = 0;

	Data <<=1;
	}
		MDO = 0;

		_asm
		nop
		nop
		nop
		_endasm

		ISHT = 1;

		_asm
		nop
		nop
		nop
		_endasm

		MDO = 1;

		_asm
		nop
		nop
		nop
		_endasm

		MDO = 0;

		_asm
		nop
		nop
		nop
		_endasm

		ISHT = 0;
}
//-----------------------------------------------------
//-----------------------------------------------------
			//Функция приема слова с платы частотомера
void ReceiveWord(void)
{
	char i, limit = 0;
	signed long Receive_0 = 0, Receive_1 = 0;
	signed char ResOrder = 0;
	float TempFl, TempFl_2;

	if(!BOUT)
	{
		if(!RDY2)
		{
			while((!RDY2)	&&	limit<64)
			{
		
				Receive_1 <<=1;
				if(Receive_0 & 0x80000000)		Receive_1 |= 1;
				Receive_0 <<=1;
				limit++;
		
				BSHT = 1;
		
				_asm
				nop
				nop
				nop
				_endasm
		
				if(BOUT) Receive_0 |= 1;
				else Receive_0 &= ~1;
		
				BSHT = 0;
				_asm
				nop
				nop
				nop
				_endasm
			}

			if(BOUT) Receive_0 |= 1;
			else Receive_0 &= ~1;

/*			if(limit==64)
			{
				ErrReg |= 2;	//Установка ошибки связи	
				return;
			}*/

			Receive_0 = (Receive_0>>1)&0x3FFFFFFF;

			Interpol = (Receive_0>>3)&1023;
	//		Interpol = (Receive_1>>3)&1023;

		}
		else		return;
	}
	else
	{
		if(BOUT) Receive_0 |= 1;
		else Receive_0 &= ~1;

		while(limit<13)
		{
			Receive_0 <<=1;
			limit++;
		
			BSHT = 1;
		
			_asm 
			nop
			nop
			nop
			_endasm
		
			if(BOUT) Receive_0 |= 1;
			else Receive_0 &= ~1;
		
			BSHT = 0;
			_asm
			nop
			nop
			nop
			_endasm
		}

		BSHT = 1;
		
		_asm
		nop
		nop
		nop
		nop
		nop
		nop
		_endasm
	
		BSHT = 0;

	 	ReceiveData = Receive_0&0x1FFF;

		if(BOUT)
		{
			ErrReg |= 2;	//Установка ошибки связи	
			return;
		}
		if((Receive_0&0x7)==7)
		{
			ErrReg |= (Receive_0>>3)&0b11100;			//Флаги ошибки с платы частотомера
		}

		Flags.DataReceive = 1;					//Установка флага принятого сообщения;
		return;
	}

//Receive_0=999999999;

	if((Flags.Test == 0)&&(Channel == Chan_B))		//Для канала B
	{
		Receive_0 *= 16;
	}

	SignDig = Order(Receive_0);			//Количество отображаемых символов


	if(Compare(Mode[0], NormalMode)||((Mode[0] == 2)&&(Channel == Chan_B)))		//Результат не требующий выполнения деления
	{
		if(Mode[0] == 0)
		{
			ResOrder = 3-MeasTime[0];					//Множитель для прямого измерения частоты
		}

		if(Mode[0] == 1)	ResOrder = 0-MeasTime[0];	//)отношения частот А/С (C/A)
       
        if(Mode[0] == 2)	ResOrder = 0-MeasTime[0];	//)отношения частот А/С (C/A)    //ya

		if(Mode[0] == 50)	ResOrder = 0;				//счет за число периодов
	
		if((Mode[0] == 16)||(Mode[0] == 33))
		{
			ResOrder = -8 - MeasTime[0] + Notch[0];		//Множитель для периода и длительности с усреднением
	
			if((Mode[0] == 33) && !Flags.Test)	Receive_0 *= 2;		//Для длительности с усреднением результат умножаем на 2
		}
	
		if((Mode[0] == 32)||(Mode[0] == 35))
		{
			ResOrder = -8 + Notch[0];					//Множитель	для длительности и интервала
		}
	
		if((Mode[0] == 34) && !Flags.Test)				//Для режима длительности с интерполятором
		{

			ResOrder = -9;
		//10.04.18	Receive_0 = 50.0*(N2-2*N1+Interpol)/(N2-N1);//Fl_Result;

            Interpols=Interpol-N1;
            Receive_0 = 20.0+((Interpols*80)/(N2-N1));//Fl_Result;
        //10.04.18
		}

		RES_Dat.Data = Receive_0;
		RES_Dat.Order = ResOrder;

	}
	else
	{

		if(Mode[0] == 37)													//Для режима ФАЗА
		{
			if(!Receive_1)			//Проверяем делитель на нуль
			{
				ResOrder = 0;			
				Fl_Result = 2e9;
			}
			else
			{
				Fl_Result = Receive_0;
				Fl_Result /= Receive_1;
				if(Fl_Result>0.5)	Fl_Result -= 1.0;
				Fl_Result *=360*FloatDegree(SignDig-1);
				ResOrder = 0-(SignDig-1);
			}
		}
		if(Mode[0] == 36)												//Для режима скважность
		{
			if(!Receive_1)			//Проверяем делитель на нуль
			{
				ResOrder = 0;			
				Fl_Result = 2e9;
			}
			else
			{
				Fl_Result = Receive_0*FloatDegree(SignDig*2-3);
				Fl_Result /= Receive_1;
				ResOrder = 0-(SignDig*2-3);
			}
		}

		if(Mode[0] == 2)												//Для режима отношения к частоте В
		{
			if(!Receive_0)			//Проверяем делитель на нуль
			{
				ResOrder = 0;			
				Fl_Result = 2e9;
			}
			else
			{
				Fl_Result = Receive_1*FloatDegree(SignDig*2-1);
				Fl_Result /= Receive_0*16;
				ResOrder = 0-(SignDig*2-1);
			}
		}
		if((Mode[0] == 3)||(Mode[0] == 17))	//17						//Для режимов 1/T и 1/f
		{
			if(!Receive_0)			//Проверяем делитель на нуль
			{
				ResOrder = 0;			
				Fl_Result = 2e9;
			}
			else
			{
				Fl_Result = FloatDegree(SignDig*2-1);//(SignDig*2-1)
				Fl_Result /= Receive_0;
		
				if(Mode[0] == 3)	ResOrder = 8 + MeasTime[0] - Notch[0] - (SignDig*2-1);	//1/T	-1*(Notch[0] - 8 - MeasTime[0])
				else				ResOrder = MeasTime[0]-3-(SignDig*2-1);					//1/f
			}
		}
		RES_Dat.Data = Fl_Result;
		RES_Dat.Order = ResOrder;
	}

	Flags.TransceiveFlag = 1;	//Флаг готовности передачи результата измерения
}
//-----------------------------------------------------
//-----------------------------------------------------
void ChannelPrest (void)
{
	char TempR;
		//*****************

	if(Flags.RefrCreen)			//Начальная инициализация
	{
		Flags.RefrCreen = 0;
		KeyRefresh(255);						//Обновляем изображение клавиш в ненажатом виде

		EncoderPointer = &EncoderCount;			//До разрешения изменения настроек переводим указатель энкодера на безопасный регистр, стобы ненароком не напортить предыдущий регистр
		EncoderPointer_16 = &EncoderCount_16;	//До разрешения изменения настроек переводим указатель энкодера на безопасный регистр, стобы ненароком не напортить предыдущий регистр
		Rectangle (36, 255, 1, 1, 0);
		Flags.Change_Event = 1;					//Установка флага нажатия новой клавиши (чтобы сразу засветить функциональную клавишу)
		Timer1 = 160;							//Время выдержки таймера 8 секунд

		Rectangle (0, 70, 2, 4, 0);				//Очистка выделенной области
		Rectangle (195, 254, 0, 2, 0);			//Очистка выделенной области

		C0=20; C1=20; C2=20; C3=20; C4=20; C5=20; C6=20; C7=20; C8=20, C9=20;		//Заставим число на индикаторе целиком обновиться
	}
	//*****************
	if(Flags.Control_Event)
	{
		Flags.Control_Event = 0;			//Сброс флага события от органов управления
		Timer1 = 160;						//Время выдержки таймера 8 секунд
	}
	//*****************
	if((Timer1 == 0) || (Keyboard > Key_6))				//Выход из режима
	{
		Flags.FunKey_Event = 0;
		Flags.Key_Event = 0;
		Flags.RefrCreen = 1;				//Установка флага обновления экрана
 	    ModeScreen = MEASURE_SCREEN;
	}
	//*****************

	if(!Flags.Test && (ModeIndex == 0))	ChannelIndex = Channel;			//Если мы не находимся в режиме настройки параметров каналов, то аргумент ChannelIndex будет нулевым
	else ChannelIndex = 0;

	Menu();												//Вызов функции навигации обработки клавиш меню
	ModeImage();										//Отображения на индикаторе изображения, символизирующего текущий режим работы


	IndRes();				//Отображение на индикаторе результата измерения

	if((Memory[0] == TRUE)&&!(Compare(Mode[0], M_Const)))	Text_To_Display(240, 3, "M");
	else													Text_To_Display(240, 3, " ");

	ParamIndicate(40,0, Channel);						//Отображаем настройки канала
	MeasureTimer();										//Отображение на индикаторе времени измерения

	if(!(Compare(Mode[0], TEST_Const)))
	{
		Flags.Test = 0;						//Отключение режима ТЕСТ (если если выбран не подходящий режим)
		TransfWord(0b01111000000);			//Передача частотомеру команды ИЗМЕРЕНИЕ
	}
}
//-----------------------------------------------------
//-----------------------------------------------------
void CalibrateMode (void)
{
		//*****************

	if(Flags.RefrCreen)			//Начальная инициализация
	{
		Flags.RefrCreen = 0;

		EncoderPointer = &EncoderCount;			//До разрешения изменения настроек переводим указатель энкодера на безопасный регистр, стобы ненароком не напортить предыдущий регистр
		EncoderPointer_16 = &Calibration;		//До разрешения изменения настроек переводим указатель энкодера на безопасный регистр, стобы ненароком не напортить предыдущий регистр

		ENC_max = 1023;
		ENC_min = 0;

		Flags.ENC_Event = 0;					//Сброс флага события энкодера
		Flags.Key_Event = 0;					//Сброс флага события клавиатуры

		Command_To_Display(CLEAR_DYSP);			//Очистка индикатора

		Text_To_Display(50, 0, "---Режим \"калибровка\"---");	

		Timer1 =40;								//Время выдержки таймера 2 секунды

		while((Timer1 != 0) && (Flags.DataReceive == 0));

		if((Flags.DataReceive == 1) && ((ReceiveData&7) == 1))	//Проверка флага данных и идентификатора калибровочной константы
		{
			Calibration = ReceiveData>>3;	//Выделяем десять младших бит
			Flags.ENC_Event = 1;				//Обманный маневр (так мы отобразим результат измерения на индикаторе)
		}
 	    else
		{
			ModeScreen = MEASURE_SCREEN;
			Timer1 =40;							//Время выдержки таймера 2 секунды
			Text_To_Display(10, 4, "Нет ответа");	
			while(Timer1 != 0);
			Command_To_Display(CLEAR_DYSP);		//Очистка индикатора
			TransfWord(0b01111000000);			//Передача частотомеру команды ИЗМЕРЕНИЕ
			return;
		}

		Flags.DataReceive = 0;					//Сброс флага принятых данных

		C0=20; C1=20; C2=20; C3=20; C4=20; C5=20; C6=20; C7=20; C8=20, C9=20;		//Заставим число на индикаторе целиком обновиться
		

		Text_To_Display(0, 2, "Нажмите ЭНК. для сохранения.");
		Text_To_Display(0, 7, "Нажмите любую клавишу для выхода.");
	}
		//*****************

	//***************************
    if(Flags.ENC_Event)									//Проверка флага события энкодера
	{
		Flags.ENC_Event = 0;							//Сброс флага события энкодера
		Flags.Control_Event = 0;						//Сброс флага события клавиатуры
		TransfWord(Calibration | 0x400);				//Передача частотомеру данных

		RES_Dat.Data = Calibration;						//Отображаем результат измерения
		RES_Dat.Order = 0;
		Nomber_Image_Char(72, 3, &RES_Dat, SignDig);
	}
	//***************************

	//***************************
    if(Flags.Key_Event && (Keyboard != EncKey))			//Проверка флага события клавиатуры && !Flags.ENC_Event
	{
		Flags.Key_Event = 0;							//Сброс флага события клавиатуры

		TransfWord(0b01111000000);						//Передача частотомеру команды ИЗМЕРЕНИЕ (выход без сохранения)
		ModeScreen = MEASURE_SCREEN;					//Выход из режима
		Command_To_Display(CLEAR_DYSP);					//Очистка индикатора
		TransfWord(0b01111000000);						//Передача частотомеру команды ИЗМЕРЕНИЕ
	}
	//***************************
	
	//*****************
	if(Flags.Key_Event && (Keyboard == EncKey))				//Выход из режима по нажатии кнопки энкодера
	{
		Flags.Key_Event = 0;							//Сброс флага события клавиатуры
 	    ModeScreen = MEASURE_SCREEN;
		TransfWord(0b01111000100);	//Передача частотомеру команды ВЫХОД С СОХРАНЕНИЕМ

		Timer1 = 20;							//Время выдержки таймера 1 секунда
		Text_To_Display(10, 4, "Сохранено");
		while(Timer1 != 0);
		Command_To_Display(CLEAR_DYSP);			//Очистка индикатора
		TransfWord(0b01111000000);						//Передача частотомеру команды ИЗМЕРЕНИЕ
	}
	//*****************
}

//-----------------------------------------------------
void main(void)
{
    HAL::Init();
    Display::Init();
	char i;
//FloatToBinDec(1.23456789E+4, 3, 9);




	PORTA = 0;
	PORTB = 0; 
	PORTC = 0; 

	TRISA = 0;
	TRISB = 0b00111111;
	TRISC = 0b10000101;
	TRISD = 0b00001111;

	OSCCON = 0b01101100;
	OSCTUNE = 0b01000000;

	Delay10KTCYx(255);
	Delay10KTCYx(255);

	OpenTimer0( TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_4 );	//задержка таймера

	INTCONbits.GIE = 1; 	//enable global interrupts
	INTCONbits.PEIE = 1; 
	INTCON2 = 0b00000000;
	INTCONbits.INT0IE=1;
	INTCON3bits.INT1IE=1;

/*
	INTCON3bits.INT1IP=1;			// INT1 High priority
	IPR1bits.RCIP=1;				// EUSART High priority
	IPR1bits.TXIP=1;

//	INTCON2bits.TMR0IP=1;			//Высокий приоритет TMR0
	INTCON2bits.TMR0IP=0;			//Низкий приоритет TMR0
*/

	INTCON2bits.INTEDG0=1;
	INTCON2bits.INTEDG1=1;

//	RCONbits.IPEN = 1;

//--------------------------
	//Настройка USART
	SPBRG = 25;		//Скорость 9600кб/с
	RCSTAbits.SPEN = 1;
	TXSTAbits.SYNC = 0;
	PIR1bits.RCIF = 0;
	PIE1bits.RCIE = 1;		//Разрешение прерывания от модуля

		//Включение передатчика
	TXSTAbits.TXEN = 1;

		//Включение приемника
	RCSTAbits.CREN = 1;

//--------------------------
	Init_Display();


	PIE2bits.HLVDIE = 0;
	HLVDCON = 0b00011110;
	PIR2bits.HLVDIF = 0;

	Flags.RefrCreen = 1;
	Flags.Auto = 0;

     Text_To_Display(255,0," OAO MNIPI,Ч3-XX,Cher V 1.1");	
   Delay10KTCYx(255); 
   Delay10KTCYx(255);
   Delay10KTCYx(255);
   Delay10KTCYx(255); 

	Command_To_Display(CLEAR_DYSP);
		C0=20; C1=20; C2=20; C3=20; C4=20; C5=20; C6=20; C7=20; C8=20, C9=20;		//Заставим число на индикаторе целиком обновиться

	Flags.Test = 0;

	//for(i=0;i<15; i++)
	//{
	//	TransfWord (StartSet[i]);								//Передача частотомеру данных начальной установки
	//}

  

    Delay10KTCYx(255);
	ErrReg &= 0b00011100;

	while(1)
	{	
		if(PIR2bits.HLVDIF)
		{
			PIR2bits.HLVDIF = 0;
			Delay10KTCYx(100);
			Init_Display();
			Flags.RefrCreen = 1;
		}


	if((ArrayCnt!=0)&&Flags.TransceiveFlag)
	{
		Flags.TransceiveFlag = 0;

		ArrayCnt -= 1;

		BinToBinDec(&RES_Dat);
		RAMStringTX(Decades);	//Передача на компьютер

		while(!PIR1bits.TXIF);
		TXREG = '\n';

		if(ArrayCnt!=0)	TransfWord(0b01010000011);						//Передача частотомеру команды ПРОГРАММНЫЙ ЗАПУСК
	}


	if(Flags.ReceiveFlag)
	{
		Flags.ReceiveFlag = 0;
		Processing();
	}

		if(ModeScreen != ModeScreenPre)		//Если переменная изменилась, то устанавливаем флаг обновления экрана
		{
			Flags.RefrCreen = 1;
			ModeScreenPre = ModeScreen;
		}

		if(ErrReg) MesErr (ErrReg);

		switch(ModeScreen)
		{
			case MEASURE_SCREEN:
			MeasureScreen();
			break;

			case CHANNEL_PREST:
			ChannelPrest();
			break;

			case CALIBRATE_MODE:
			CalibrateMode();
			break;

			default:
			break; 
		}
	}
}
/*//-----------------------------------------------------*/