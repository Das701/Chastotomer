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
#pragma code INTERRUPT_VECTOR = 0x000018			//��������� ������� �� 18 ������
void inter(void)
{
  _asm GOTO IntFlags _endasm
}
#pragma code
//-------------------------------------------------------
#pragma interrupt IntFlags		//��������� �������, � ������� ����� ����������� �������� ����������
void IntFlags (void) 	//�������� ������ ����������
{
	if(INTCONbits.TMR0IF && INTCONbits.TMR0IE)		Time();
//	if(PIE1bits.RCIE && PIR1bits.RCIF)				UsRX();
//	if(INTCON3bits.INT1IF && INTCON3bits.INT1IE)	Int_1();
//	if(INTCONbits.INT0IF && INTCONbits.INT0IE)		Int_0();

}
//-----------------------------------------------------
*/
//-------------------------------------------------------
#pragma code INTERRUPT_VECTOR_HI = 0x08			//��������� ������� �� 8 ������
void interHi(void)
{
  _asm GOTO IntFlagsHi _endasm
}
#pragma code
//-------------------------------------------------------
#pragma interrupt IntFlagsHi		//��������� �������, � ������� ����� ����������� �������� ����������
void IntFlagsHi (void) 	//�������� ������ ����������
{
	if(INTCONbits.TMR0IF && INTCONbits.TMR0IE)		Time();
	if(PIE1bits.RCIE && PIR1bits.RCIF)				UsRX();
	if(INTCON3bits.INT1IF && INTCON3bits.INT1IE)	Int_1();
	if(INTCONbits.INT0IF && INTCONbits.INT0IE)		Int_0();
}
//-----------------------------------------------------
//-----------------------------------------------------
void Int_0 (void) 	//��������� ���������� �� 
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
				Flags.ENC_Event=1;				//��������� ����� ������� ��������
			}
			else	(*EncoderPointer) = ENC_min;
			if(*EncoderPointer_16>ENC_min)
			{
				(*EncoderPointer_16)--;
				Flags.ENC_Event=1;				//��������� ����� ������� ��������
			}
			else (*EncoderPointer_16) = ENC_min;
		}
		else
		{
			if(*EncoderPointer<ENC_max)	
			{
				(*EncoderPointer)++;
				Flags.ENC_Event=1;				//��������� ����� ������� ��������
			}
			else (*EncoderPointer) = ENC_max;
			if(*EncoderPointer_16<ENC_max)
			{
				(*EncoderPointer_16)++;
				Flags.ENC_Event=1;				//��������� ����� ������� ��������
			}
			else (*EncoderPointer_16) = ENC_max;
		}
		Flags.Control_Event=1;
	}
}
//-----------------------------------------------------
//-----------------------------------------------------
void Int_1 (void) 	//��������� ���������� �� 
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

	if(RCSTAbits.OERR)	//����������� ������ ������������
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
		RCREG;	//����������� ������ �����
		return;
	}

	Temp = RCREG;

	if(Temp>0x40 && Temp<0x5B)	Temp += 0x20;	//�������������� � ������� ��������

	InpSTR[STRIndex] = Temp;

	if(STRIndex<127) STRIndex++;

	if(Temp != LineFeed) return;

	for(; STRIndex<128; STRIndex++)	InpSTR[STRIndex] = 0;	//���������� ������ ������� �������

	STRIndex = 0;
	StartInd = 0;
	CommIndex = 0;

	Flags.ReceiveFlag = 1;		//���� ���������� ��������� �������� ������

//	BinToBinDec(123456789, 9);

}
//-----------------------------------------------------
//-----------------------------------------------------
void Time (void) 	//��������� ���������� �� ������� TMR0
{
	char Counter;

	INTCONbits.TMR0IF = 0;	//����� ����� ���������� �� TMR0

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
			Flags.SoftStart = 0;				//����� ����� ���������� ������������ �������
			TransfWord(0b01010000000);			//�������� ����������� ������� ���������� ����� �������
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

		if(Keyboard < MenuSize[ModeIndex])	Flags.FunKey_Event = 1;	//���� ���� ������ �������������� �������, �� ������������� ��������������� ����
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
	if(Flags.Test && (ModeIndex == 0))	TempMode = 1;			//���� ������ ����� ����, �� ��� ������� �� ������� ������ ������ ������� ������
	else 								TempMode = ModeIndex;

	for(a=0; a<6; a++)
	{
		Level_1[a] = F0[TempMode][a];				//�������� � ������ ���������� �� ��������� ������ ������ ����������� ����������
	}

	MenuSize[0]	=	6;							//����������� ���������� ������
	MenuSize[1]	=	6;

		//*****************
    if((ModeIndex == 0) && !Flags.Test)            //���� ������ ����� ������ (� ������ ���������)
    {
        if(Channel == Chan_B)   MenuSize[0]   = 0;   // � �������� ������� �������� ����� �, �� ��������� ������� ���������� ������ (�.�. �� �� ������)
    }

	if(Channel != Chan_B)
	{
    	if((Mode[0] == 3)||(Mode[0] == 1))	Level_1[5] = &STR14_1;		//������ ������� ����� �� N
    	if(Mode[0] == 17)
		{
			Level_1[4] = &STR15;		//������ ������� ����� �� �����
			MenuSize[1] = 5;			//��������� ���������� ������ �� ����
		}
	}



	if(TempMode == 1)	//���� ������ ����� �����
	{
		if((Mode[0] == 33)||(Mode[0] == 16))
		{
		 	Level_1[5] = &STR14_1;		//������ ������� ����� �� N
			if(Mode[0] == 33)	Level_1[5] = &STR14_2;	//������ ������� N � ������ ������������ � �����������
		}
		if(((Mode[0] & 240) == 32) && (Mode[0]!=33)) 		MenuSize[1] = 5;			//� ������ ������������ ��������� ���������� ������ �� ����
		if((Mode[0] > 47)&&(Mode[0] < 52))					MenuSize[1] = 4;	//� ������ ����,��������� ���������� ������ �� �������
		if(Mode[0] == 34)	MenuSize[1] = 4;			//��������� ���������� ������ �� ������� (� ������ � ��������������)

		if(Mode[0]== 50)
		{
			MenuSize[1] = 5;	//� ������ ����, ������ ����������� ���������� ������ �� ����
			Level_1[4] = &STR14_1;
		}

		switch(Channel)							//������������ ����� ��������� ��������� ������
		{
			case Chan_A:		//����� �
			Level_1[0] = &STR10;		//����� ��������� �������
			Level_1[2] = &STR12;		//����� ��������� ������������
			Level_1[3] = &STR13;		//����� �����
			break;
	
			case Chan_B:		//����� �
			MenuSize[1] = 3;			//��������� ���������� ������ �� ����
			Level_1[0] = &STR10_1;		//����� ��������� �������
			Level_1[1] = &STR13_1;		//����� �����
			Level_1[2] = &STR15;		//�����

			if(Compare(Mode[0], N_B_Const))	//���� ������ ����� ���� �� ������� ��� ��������� ������
			Level_1[2] = &STR14_1;		//������ ������� ����� �� N

			if((Mode[0] == 48) || (Mode[0] == 49))	//���� ������ ����� ���� �� ������������
			MenuSize[1] = 2;			//��������� ���������� ������ �� ����

			break;

    		case Chan_C:		//����� �
			Level_1[0] = &STR10_2;		//����� ��������� �������
			Level_1[2] = &STR12_1;		//����� ��������� ������������
			Level_1[3] = &STR13_2;		//����� �����
			break; 

			default:
			break; 
		}
	}
	//***************************

	for(a=0; a<MenuSize[TempMode]; a++)		//������ ��� �������������� ������� �������� ��������� Level_1
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

	Rectangle (MenuSize[TempMode]*36+37, 255, 7, 7, 0);	//�� ���������� �������� ��������� �������������� �������
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
		Text_To_Display(a, y, "-----����� ����-----  ");   
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
		a = Text_To_Display(a, y, "���,");  
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
	//������� ����������� �� ���������� �����������, ���������������� ������� ����� ������
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
	//������� ����������� �� ���������� ������� ���������
void MeasureTimer (void)	
{
		if(Compare(Mode[0], Ave_Const)||((Mode[0] == 2)&&(Channel == Chan_B)))								//����� �� ���������� ����� ����������?
		{
			a = Text_To_Display(X0+1, 6, String10_1[MeasTime[0]]);	//����� ����� ����������
				Slider = 3968;			//������ �� �����
		}
		else
		{
			if((Mode[0] < 3)|| (Mode[0] == 17))		a = Text_To_Display(X0+1, 6, String11[MeasTime[0]]);	//����� ����� ���������
			else
			{
				a = X0+1;
				Slider = 3968;			//������ �� �����
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
	//������� ��������� � ��������� ������ ����
void Menu (void)
{
	if(Flags.Test && (ModeIndex == 0))	ModeIndex = 1;			//���� ������ ����� ���� � ������ ����� ������, �� ��� ������������� �� ����� �����

	if((Flags.FunKey_Event || Flags.ENC_Event) && (Keyboard < MenuSize[ModeIndex]))
	{
		FuncKeyIndex = Keyboard;
	//***************************

					//������������ ������ ������� �������������� �������
		if(Flags.Change_Event && Flags.FunKey_Event)
		{
			Flags.Change_Event = 0;		//����� ����� ������� ����� �������
			Flags.FunKey_Event = 0;		//����� ����� ������� �������������� �������
			ENC_max = Level_1[FuncKeyIndex]->MAX_1;		//������������� ������������ �������� ��������
			ENC_min = Level_1[FuncKeyIndex]->MIN_1;		//������������� ����������� �������� ��������
			EncoderPointer = &(Level_1[FuncKeyIndex]->Data[ChannelIndex]);			//��������� �������� � ����� ���������� �� ����� ��������


			a = Text_To_Display(40, 1, Level_1[FuncKeyIndex]->String1_lng); 		//����� �������� �������� �������
			Rectangle (a, 255, 1, 1, 0);

			Selection (Level_1[FuncKeyIndex]->Data[ChannelIndex]);
		}
	//***************************

						//������������ ����������� ������� �������������� �������		
		if((Flags.Change_Event ==0) && Flags.FunKey_Event) 
		{
			Flags.FunKey_Event = 0;		//����� ����� ������� �������������� �������

			a = Level_1[FuncKeyIndex]->Data[ChannelIndex];

			if(a < Level_1[FuncKeyIndex]->MAX_1)	a += 1;
			else a = Level_1[FuncKeyIndex]->MIN_1;

			Selection (a);

		}
	//***************************

						//������������ ������� ��������
		if((Flags.Change_Event ==0) && Flags.ENC_Event) 
		{
			Flags.ENC_Event = 0;		//����� ����� ��������� ��������� ��������

			Selection (Level_1[FuncKeyIndex]->Data[ChannelIndex]);

		}
	//***************************
	}
}
//-----------------------------------------------------
//-----------------------------------------------------
void Selection (char data)	//������� �����������, ���������� � ����������� ����������, � ��� �� ������ ��������������� �� ������� �� ����������
{
	char Temp;
	if((data < Level_1[FuncKeyIndex]->MIN_1) || (data > Level_1[FuncKeyIndex]->MAX_1))	//���������� ��� ���������� �������� � �������� ��������
	{
		data = Level_1[FuncKeyIndex]->MIN_1;
		Level_1[FuncKeyIndex]->Data[ChannelIndex] = data;		//���� �� ��������, �� ������ �� �����������
	}
	Level_1[FuncKeyIndex]->Data[ChannelIndex] = data;			//��������� ����������

	Temp = Level_1[FuncKeyIndex]->Command;						//��������� �������

	if(Temp==0x0D)		//����. ������� �����
	{
			Flags.RefrCreen_2 = 1;							//��������� ����� ��������������� ���������� ������
			Kin[Channel] = 1;								//�������� ���
			TransfWord(0b00010000001);						//�������� ���

			if(Synch[Channel]==0)	Level[Channel] = 60;	//���
			else 					Level[Channel] = -62;	//���

			TransfWord(Level[Channel] | 0x400);				//�������� ����������� ������ �� ������ �������������
	}
	else
	{
		if(Temp>0x0E)
		{
			Flags.DataReceive = 0;				//����� ����� �������� ������
	 		TransfWord((Temp & 0x0F) | 0x3C0);	//�������� ������� �����������
		}
		else TransfWord((data & 63) | (Temp<<6) & 1023);	//�������� ������� �����������
	}




	Cursor_Set(1, 3);				//��������� �������
	Write_To_Display(':');			//������ ������� )
	data = Text_To_Display(8, 3, Level_1[FuncKeyIndex]->StateLable[data & 15]);//���������� ��������������� ���������� ������
	Rectangle (data, 70, 3, 3, 0);		//��������� ������

	KeyRefresh(FuncKeyIndex);		//��������� ����������� ������, ���� �� ������� � ������� ����
}
//-----------------------------------------------------
//-----------------------------------------------------
void MeasureScreen (void)
{	//***************************
	if(Flags.RefrCreen)
	{
		if(!(Compare(Mode[0], TEST_Const)))		//���������� ������ ���� (���� � ������� ������ �� ��������)
		{
			Flags.Test = 0;	
			TransfWord(0b01111000000);			//�������� ����������� ������� ���������
		}

		Flags.RefrCreen = 0;

		Flags.Key_Event = 0;							//����� ����� ������� ����������
		Flags.FunKey_Event = 0;
		Flags.Hold = 0;								//����� ����� ���������

		EncoderPointer = &EncoderCount;		//�� ���������� ��������� �������� ��������� ��������� �������� �� ���������� �������, ����� ��������� �� ��������� ���������� �������

		Rectangle (195, 254, 0, 2, 0);			//������� ���������� �������

		ENC_max = 400;
		ENC_min = -400;

		FuncKeyIndex = 0;

		Rectangle (0, 60, 2, 4, 0);			//������� ���������� �������

		KeyRefresh(255);				//������������� ������� � ��������� ����

		Timer2=0;			//������� ��� ����������

		C0=20; C1=20; C2=20; C3=20; C4=20; C5=20; C6=20; C7=20; C8=20, C9=20;		//�������� ����� �� ���������� ������� ����������
	}
	//***************************
	if(Timer2==0)
	{

		Timer2 = 20;					//�������� ������� 1�
		Flags.RefrCreen_2 = 1;			//��������� ����� ��������������� ���������� ������ (���������� ��������� ����������)
	
		ModeImage();		//����������� �� ���������� �����������, ���������������� ������� ����� ������



		a = Text_To_Display(0, 3, (ModeString[Channel][Mode[0]>>4][Mode[0]&15]));	//��������� ��������� ������
		Rectangle (a, 70, 3, 3, 0);					//������� ������

		if(((Mode[0] > 31) && (Mode[0] < 34)) || (Mode[0] == 36) || (Mode[0] == 16) || (Mode[0] == 3) || ((Mode[0] == 48) && Flags.Test))
		{
			a = Text_To_Display(0, 4, "\x98t=");	//��������� ����� ������� 
			a = Text_To_Display(a, 4, String10[Notch[0]]);
			Rectangle (a, 70, 4, 4, 0);				//��������� ������ 
		}
		else Rectangle (0, 70, 4, 4, 0);			//������� ������
	}
	//***************************
   	if((Channel != Chan_B) && !Flags.Test)	//�������� ������� ������ ������ �������������
	{
		if(Flags.Key_Event && (Keyboard == EncKey)  && (Starting[0] != 2))		//��������� ������ � ���� (���� �� ������ ������ ������)
		{
			Flags.Key_Event = 0;							//����� ����� ������� ����������
			Flags.RefrCreen_2 = 1;							//��������� ����� ��������������� ���������� ������
			Level[Channel] = 0;
			TransfWord(Level[Channel] | 0x400);				//�������� ����������� ������ �� ������ �������������
		}
		{//***************************
	   	if(Flags.ENC_Event)	//�������� ����� ������� ��������
		{
			Flags.ENC_Event = 0;							//����� ����� ������� ��������
			Flags.RefrCreen_2 = 1;							//��������� ����� ��������������� ���������� ������
	
			TransfWord(Level[Channel] | 0x400);				//�������� ����������� ������ �� ������ �������������
		}
		}//***************************
	}
//***************************
	if(Flags.DataReceive && Flags.Hold)		//�������� ����� ������ � �������������� ������������� ���������
	{
		Flags.DataReceive = 0;				//����� ����� ������
		Timer2 = 60;			//�������� ������� 3�

		switch(ReceiveData&7)
		{
			case 2:
			a = Text_To_Display(40, 1, "���� ");	//��������� ������������� ��������
			Nomber(a, 1, 4, ((ReceiveData>>3)-512)<<1);
			break;

			case 3:
			a = Text_To_Display(110, 1, "��� ");	//��������� ������������ ��������
			Nomber(a, 1, 4, ((ReceiveData>>3)-512)<<1);
			break;

			case 4:
			Level[Channel] = (ReceiveData>>3)-512;	//���������� �������� ������
			Flags.Hold = 0;								//����� ����� ���������
			break;

			default:
			break; 
		}
	}
//***************************
    if(Flags.RefrCreen_2 == 1)											//�������� ����� ��������������� ���������� ������
	{
		Flags.RefrCreen_2 = 0;											//����� ����� ��������������� ���������� ������
		Flags.Hold = 0;								//����� ����� ���������

		if(Channel<2)	EncoderPointer_16 = &Level[Channel];			//��������� �������� � ����� ���������� �� ����� ��������
		else	EncoderPointer_16 = &EncoderCount_16;					//��������� �� ���������� �������, ���� ������� ����� �

		if(Flags.Auto)
		{
			Text_To_Display(40, 0, " ��������� ������ �����. ");  
			Text_To_Display(40, 1, "                         "); 
			Timer2 = 600;							//�������� ������� 20�
			Flags.Auto = 0;							//����� ����� ������ ����
			Flags.Hold = 1;							//��������� ����� ���������
			Rectangle (72, 237, 3, 5, 0);			//������� ������� ����������
			C0=20; C1=20; C2=20; C3=20; C4=20; C5=20; C6=20; C7=20; C8=20, C9=20;		//�������� ����� �� ���������� ������� ����������
			Flags.RefrCreen_2 = 0;														//����� ����� ��������������� ���������� ������
			Flags.DataReceive = 0;				//����� ����� ������
		}

		else ParamIndicate(40,0, Channel);	

									//���������� ��������� ��������� ������
		Channel_2 = TwinChanMode[Channel][Mode[0]>>4][Mode[0] & 15];	//�� ������� ��������� ������������� ���������� ��������� ������� ������

		if(Channel_2 != 255)	ParamIndicate(40, 1, Channel_2);		//���� ������� ��� ������, �� ���������� ��������� � ������� ������
		else Rectangle (40, 194, 1, 1, 0);								//���� ������� ������ ����, �������� ���������� �� ����������

//***************************
	//����������� ���������� �� � ��������� �������
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
		if((Generator[0] == 0)&&(Starting[0] == 0))	 Rectangle (195, 254, 0, 2, 0);			//������� ���������� �������


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

	if(!Flags.Hold)				//���� ���������� ���� ���������, �� ��������� ��������� �� ����������
	{
		MeasureTimer();			//����������� �� ���������� ������� ���������

//***************************
		//���������� ������ � ��������������
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

		IndRes();				//����������� �� ���������� ���������� ���������

		if(Timer3!=0)	Text_To_Display(249, 3, "*");
		else			Text_To_Display(249, 3, " ");
	}

	if((Memory[0] == TRUE)&&!(Compare(Mode[0], M_Const)))	Text_To_Display(240, 3, "M");
	else													Text_To_Display(240, 3, " ");

//***************************
//***************************
	if(Flags.Key_Event && (Keyboard == TestKey))
	{
		Flags.Key_Event = 0;		//����� ����� ������� ����������

		if(Compare(Mode[0], TEST_Const))
		{

			ModeIndex = 1;					//�������������� ������� ������
			KeyRefresh(255);				//������������� ������� � ��������� ����
			Flags.RefrCreen_2 = 1;			//��������� ����� ��������������� ���������� ������ (���������� ��������� ����������)

			if(Flags.Test==1)
			{
				Flags.Test = 0;				//���������� ������ ���� (���� ��� �������)
				TransfWord(0b01111000000);	//�������� ����������� ������� ���������
			}
			else
			{
				Flags.Test = 1;				//��������� ������ ����
				ModeIndex = 1;				//��������� ������� ������
				TransfWord(0b01111000001);	//�������� ����������� ������� ����
			}
		}
	}
//***************************
//***************************
	if(Flags.Key_Event && (Keyboard >= ChannelsKey) && (Keyboard <= IndicateKey))
	{
		Flags.Key_Event = 0;		//����� ����� ������� ����������

		if(((ModeIndex == 0) && Keyboard == ChannelsKey))			//������������ ������
		{
			if(Channel == 0)	Channel = 2;
			else Channel--;

			TransfWord(Channel & 63);					//�������� ������� ����������� (����� ������)
		}
		else	ModeIndex = Keyboard - ChannelsKey;

		KeyRefresh(255);				//������������� ������� � ��������� ����
		Flags.RefrCreen_2 = 1;			//��������� ����� ��������������� ���������� ������ (���������� ��������� ����������)
	}
//***************************
//***************************
	if(Flags.Key_Event && (Keyboard == AutoKey) && !Flags.Hold && !Flags.Test && (Channel != Chan_B))
	{
		Flags.Key_Event = 0;		//����� ����� ������� ����������
		TransfWord(0b01111000011);	//�������� ����������� ������� ����
		Flags.Auto = 1;				//��������� ����� ���������� ������ ����
		Flags.RefrCreen_2 = 1;		//��������� ����� ��������������� ���������� ������ (���������� ��������� ����������)
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

	if((Mode[0]== 33)&&(MeasTime[0] == 0))	//� ������ ������������ � ����������� ��������� ����� ���������� 1, ������� �� 10
	{
		MeasTime[0] = 1;						//���������� ������ ���� (���� ���� ������ �� ���������� �����)
		TransfWord(0b01000000001);			//�������� ����������� ������� ���������
	}

}
//-----------------------------------------------------
//-----------------------------------------------------
			//������� �������� ����� �� ����� �����������
void TransfWord (unsigned int Data)
{
	char i;
	int CNT=0;

	while(BUSY)
	{
		CNT++;
		if(CNT>20000)
		{
			ErrReg |= 2;	//��������� ������ �����	
			return;
		}
	}

	ErrReg &= ~2;	//����� ������ �����

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
			//������� ������ ����� � ����� �����������
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
				ErrReg |= 2;	//��������� ������ �����	
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
			ErrReg |= 2;	//��������� ������ �����	
			return;
		}
		if((Receive_0&0x7)==7)
		{
			ErrReg |= (Receive_0>>3)&0b11100;			//����� ������ � ����� �����������
		}

		Flags.DataReceive = 1;					//��������� ����� ��������� ���������;
		return;
	}

//Receive_0=999999999;

	if((Flags.Test == 0)&&(Channel == Chan_B))		//��� ������ B
	{
		Receive_0 *= 16;
	}

	SignDig = Order(Receive_0);			//���������� ������������ ��������


	if(Compare(Mode[0], NormalMode)||((Mode[0] == 2)&&(Channel == Chan_B)))		//��������� �� ��������� ���������� �������
	{
		if(Mode[0] == 0)
		{
			ResOrder = 3-MeasTime[0];					//��������� ��� ������� ��������� �������
		}

		if(Mode[0] == 1)	ResOrder = 0-MeasTime[0];	//)��������� ������ �/� (C/A)
       
        if(Mode[0] == 2)	ResOrder = 0-MeasTime[0];	//)��������� ������ �/� (C/A)    //ya

		if(Mode[0] == 50)	ResOrder = 0;				//���� �� ����� ��������
	
		if((Mode[0] == 16)||(Mode[0] == 33))
		{
			ResOrder = -8 - MeasTime[0] + Notch[0];		//��������� ��� ������� � ������������ � �����������
	
			if((Mode[0] == 33) && !Flags.Test)	Receive_0 *= 2;		//��� ������������ � ����������� ��������� �������� �� 2
		}
	
		if((Mode[0] == 32)||(Mode[0] == 35))
		{
			ResOrder = -8 + Notch[0];					//���������	��� ������������ � ���������
		}
	
		if((Mode[0] == 34) && !Flags.Test)				//��� ������ ������������ � ��������������
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

		if(Mode[0] == 37)													//��� ������ ����
		{
			if(!Receive_1)			//��������� �������� �� ����
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
		if(Mode[0] == 36)												//��� ������ ����������
		{
			if(!Receive_1)			//��������� �������� �� ����
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

		if(Mode[0] == 2)												//��� ������ ��������� � ������� �
		{
			if(!Receive_0)			//��������� �������� �� ����
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
		if((Mode[0] == 3)||(Mode[0] == 17))	//17						//��� ������� 1/T � 1/f
		{
			if(!Receive_0)			//��������� �������� �� ����
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

	Flags.TransceiveFlag = 1;	//���� ���������� �������� ���������� ���������
}
//-----------------------------------------------------
//-----------------------------------------------------
void ChannelPrest (void)
{
	char TempR;
		//*****************

	if(Flags.RefrCreen)			//��������� �������������
	{
		Flags.RefrCreen = 0;
		KeyRefresh(255);						//��������� ����������� ������ � ��������� ����

		EncoderPointer = &EncoderCount;			//�� ���������� ��������� �������� ��������� ��������� �������� �� ���������� �������, ����� ��������� �� ��������� ���������� �������
		EncoderPointer_16 = &EncoderCount_16;	//�� ���������� ��������� �������� ��������� ��������� �������� �� ���������� �������, ����� ��������� �� ��������� ���������� �������
		Rectangle (36, 255, 1, 1, 0);
		Flags.Change_Event = 1;					//��������� ����� ������� ����� ������� (����� ����� ��������� �������������� �������)
		Timer1 = 160;							//����� �������� ������� 8 ������

		Rectangle (0, 70, 2, 4, 0);				//������� ���������� �������
		Rectangle (195, 254, 0, 2, 0);			//������� ���������� �������

		C0=20; C1=20; C2=20; C3=20; C4=20; C5=20; C6=20; C7=20; C8=20, C9=20;		//�������� ����� �� ���������� ������� ����������
	}
	//*****************
	if(Flags.Control_Event)
	{
		Flags.Control_Event = 0;			//����� ����� ������� �� ������� ����������
		Timer1 = 160;						//����� �������� ������� 8 ������
	}
	//*****************
	if((Timer1 == 0) || (Keyboard > Key_6))				//����� �� ������
	{
		Flags.FunKey_Event = 0;
		Flags.Key_Event = 0;
		Flags.RefrCreen = 1;				//��������� ����� ���������� ������
 	    ModeScreen = MEASURE_SCREEN;
	}
	//*****************

	if(!Flags.Test && (ModeIndex == 0))	ChannelIndex = Channel;			//���� �� �� ��������� � ������ ��������� ���������� �������, �� �������� ChannelIndex ����� �������
	else ChannelIndex = 0;

	Menu();												//����� ������� ��������� ��������� ������ ����
	ModeImage();										//����������� �� ���������� �����������, ���������������� ������� ����� ������


	IndRes();				//����������� �� ���������� ���������� ���������

	if((Memory[0] == TRUE)&&!(Compare(Mode[0], M_Const)))	Text_To_Display(240, 3, "M");
	else													Text_To_Display(240, 3, " ");

	ParamIndicate(40,0, Channel);						//���������� ��������� ������
	MeasureTimer();										//����������� �� ���������� ������� ���������

	if(!(Compare(Mode[0], TEST_Const)))
	{
		Flags.Test = 0;						//���������� ������ ���� (���� ���� ������ �� ���������� �����)
		TransfWord(0b01111000000);			//�������� ����������� ������� ���������
	}
}
//-----------------------------------------------------
//-----------------------------------------------------
void CalibrateMode (void)
{
		//*****************

	if(Flags.RefrCreen)			//��������� �������������
	{
		Flags.RefrCreen = 0;

		EncoderPointer = &EncoderCount;			//�� ���������� ��������� �������� ��������� ��������� �������� �� ���������� �������, ����� ��������� �� ��������� ���������� �������
		EncoderPointer_16 = &Calibration;		//�� ���������� ��������� �������� ��������� ��������� �������� �� ���������� �������, ����� ��������� �� ��������� ���������� �������

		ENC_max = 1023;
		ENC_min = 0;

		Flags.ENC_Event = 0;					//����� ����� ������� ��������
		Flags.Key_Event = 0;					//����� ����� ������� ����������

		Command_To_Display(CLEAR_DYSP);			//������� ����������

		Text_To_Display(50, 0, "---����� \"����������\"---");	

		Timer1 =40;								//����� �������� ������� 2 �������

		while((Timer1 != 0) && (Flags.DataReceive == 0));

		if((Flags.DataReceive == 1) && ((ReceiveData&7) == 1))	//�������� ����� ������ � �������������� ������������� ���������
		{
			Calibration = ReceiveData>>3;	//�������� ������ ������� ���
			Flags.ENC_Event = 1;				//�������� ������ (��� �� ��������� ��������� ��������� �� ����������)
		}
 	    else
		{
			ModeScreen = MEASURE_SCREEN;
			Timer1 =40;							//����� �������� ������� 2 �������
			Text_To_Display(10, 4, "��� ������");	
			while(Timer1 != 0);
			Command_To_Display(CLEAR_DYSP);		//������� ����������
			TransfWord(0b01111000000);			//�������� ����������� ������� ���������
			return;
		}

		Flags.DataReceive = 0;					//����� ����� �������� ������

		C0=20; C1=20; C2=20; C3=20; C4=20; C5=20; C6=20; C7=20; C8=20, C9=20;		//�������� ����� �� ���������� ������� ����������
		

		Text_To_Display(0, 2, "������� ���. ��� ����������.");
		Text_To_Display(0, 7, "������� ����� ������� ��� ������.");
	}
		//*****************

	//***************************
    if(Flags.ENC_Event)									//�������� ����� ������� ��������
	{
		Flags.ENC_Event = 0;							//����� ����� ������� ��������
		Flags.Control_Event = 0;						//����� ����� ������� ����������
		TransfWord(Calibration | 0x400);				//�������� ����������� ������

		RES_Dat.Data = Calibration;						//���������� ��������� ���������
		RES_Dat.Order = 0;
		Nomber_Image_Char(72, 3, &RES_Dat, SignDig);
	}
	//***************************

	//***************************
    if(Flags.Key_Event && (Keyboard != EncKey))			//�������� ����� ������� ���������� && !Flags.ENC_Event
	{
		Flags.Key_Event = 0;							//����� ����� ������� ����������

		TransfWord(0b01111000000);						//�������� ����������� ������� ��������� (����� ��� ����������)
		ModeScreen = MEASURE_SCREEN;					//����� �� ������
		Command_To_Display(CLEAR_DYSP);					//������� ����������
		TransfWord(0b01111000000);						//�������� ����������� ������� ���������
	}
	//***************************
	
	//*****************
	if(Flags.Key_Event && (Keyboard == EncKey))				//����� �� ������ �� ������� ������ ��������
	{
		Flags.Key_Event = 0;							//����� ����� ������� ����������
 	    ModeScreen = MEASURE_SCREEN;
		TransfWord(0b01111000100);	//�������� ����������� ������� ����� � �����������

		Timer1 = 20;							//����� �������� ������� 1 �������
		Text_To_Display(10, 4, "���������");
		while(Timer1 != 0);
		Command_To_Display(CLEAR_DYSP);			//������� ����������
		TransfWord(0b01111000000);						//�������� ����������� ������� ���������
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

	OpenTimer0( TIMER_INT_ON & T0_16BIT & T0_SOURCE_INT & T0_PS_1_4 );	//�������� �������

	INTCONbits.GIE = 1; 	//enable global interrupts
	INTCONbits.PEIE = 1; 
	INTCON2 = 0b00000000;
	INTCONbits.INT0IE=1;
	INTCON3bits.INT1IE=1;

/*
	INTCON3bits.INT1IP=1;			// INT1 High priority
	IPR1bits.RCIP=1;				// EUSART High priority
	IPR1bits.TXIP=1;

//	INTCON2bits.TMR0IP=1;			//������� ��������� TMR0
	INTCON2bits.TMR0IP=0;			//������ ��������� TMR0
*/

	INTCON2bits.INTEDG0=1;
	INTCON2bits.INTEDG1=1;

//	RCONbits.IPEN = 1;

//--------------------------
	//��������� USART
	SPBRG = 25;		//�������� 9600��/�
	RCSTAbits.SPEN = 1;
	TXSTAbits.SYNC = 0;
	PIR1bits.RCIF = 0;
	PIE1bits.RCIE = 1;		//���������� ���������� �� ������

		//��������� �����������
	TXSTAbits.TXEN = 1;

		//��������� ���������
	RCSTAbits.CREN = 1;

//--------------------------
	Init_Display();


	PIE2bits.HLVDIE = 0;
	HLVDCON = 0b00011110;
	PIR2bits.HLVDIF = 0;

	Flags.RefrCreen = 1;
	Flags.Auto = 0;

     Text_To_Display(255,0," OAO MNIPI,�3-XX,Cher V 1.1");	
   Delay10KTCYx(255); 
   Delay10KTCYx(255);
   Delay10KTCYx(255);
   Delay10KTCYx(255); 

	Command_To_Display(CLEAR_DYSP);
		C0=20; C1=20; C2=20; C3=20; C4=20; C5=20; C6=20; C7=20; C8=20, C9=20;		//�������� ����� �� ���������� ������� ����������

	Flags.Test = 0;

	//for(i=0;i<15; i++)
	//{
	//	TransfWord (StartSet[i]);								//�������� ����������� ������ ��������� ���������
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
		RAMStringTX(Decades);	//�������� �� ���������

		while(!PIR1bits.TXIF);
		TXREG = '\n';

		if(ArrayCnt!=0)	TransfWord(0b01010000011);						//�������� ����������� ������� ����������� ������
	}


	if(Flags.ReceiveFlag)
	{
		Flags.ReceiveFlag = 0;
		Processing();
	}

		if(ModeScreen != ModeScreenPre)		//���� ���������� ����������, �� ������������� ���� ���������� ������
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