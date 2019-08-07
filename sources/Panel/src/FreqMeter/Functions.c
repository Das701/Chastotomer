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

char ErrQue[20];		//������� ������
char QueIndex=0;		//������ ������� ������

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

	for(i = 1; i < *data[0]; i++)					//���� �����
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
		TransfWord(0b01010000011);			//�������� ����������� ������� ����������� ������
		Flags.SoftStart = 1;				//��������� ����� ���������� ������������ �������
		break; 

	    case Read:
		ReadSyst();
		break; 

	    case RST:	//�����
		_asm
		RESET
		_endasm
		break; 

	    case CLS:	//������� ���������
		ArrayCnt = 0;
		QueIndex=0;
		break; 

	    case IDN:	//����������������� �����
		ROMStringTX("OAO MNIPI, �3-96, Relf V 1.1\r");
		break; 

	    case ERRor:		//������ ������� ������
		Err();
		QueIndex=0;
		break; 

		default:
		ErrQueue(4);				//������ �������
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

	if(InpSTR[StartInd] == '?')	{StartInd++; ArrayCnt = 1;}		//��������� �������
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
			if(InpSTR[StartInd] == '?')	StartInd += 2;		//��������� �������
			else StartInd++;
			ArrayCnt = StringToBin();
			if(ArrayCnt == 0x4000)		ErrQueue(3);			//������������� ��������
			if(!((ArrayCnt > 0)&&(ArrayCnt < 1001)))
			{
				ArrayCnt = 0;
				ErrQueue(1);					//��� ���������
				return;
			}
			break;
		}
	}

	TransfWord(0b01010000011);			//�������� ����������� ������� ����������� ������
	Flags.SoftStart = 1;				//��������� ����� ���������� ������������ �������
}
//-----------------------------------------------------,
//-----------------------------------------------------
void ErrQueue (char n)	//������� ������
{
	if(QueIndex<19)
	{
		if((ErrQue[QueIndex-1]!=n)||(n==0))	//���� ���������� ������ ���������� �� �������
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
		ErrQueue(4);				//������ �������
		return;
		break; 
	}

	if(InpSTR[StartInd] == '?')	{quest = 1; StartInd++;}		//��������� �������

	arg = FindAndCheck(Point_2[Comm], 0, 1);
	Chan = FindAndCheck(String45, Ch_a, Ch_b);

	if(Chan == 255)
	{
		if(Channel>1)	{ErrQueue(2);	return;}		//������, ���� ������ ����� �
		else Chan = Channel;		//���� ����� ������ �� ������, �� ���������� �������
	}
	else Chan -= Ch_a;

	if(Chan > 1)	{ErrQueue(3);	return;}//��������� ������ (�� ������� ������ �)//������, ���� ������ ����� �

	Point8 +=	Chan;

	if(!quest)
	{
		if(arg>1)	{ErrQueue(3);	return;}

		*Point8 = arg;										//���������
		TransfAll();	//���������� ���� ���������� �� ����� �����������
	}
	else
	{
		ROMStringTX(((Point_2[Comm][(*Point8+1)<<1])));	//������
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
			ErrQueue(4);				//������ �������
			return;
			break; 
		}

	if(InpSTR[StartInd] == '?')	{quest = 1; StartInd++;}		//��������� �������

	arg = FindAndCheck(Point_3[Comm], 0, 2)		;//���������� ������������ ������

	if(!quest)
	{
		if(arg == 255)	{ErrQueue(3);	return;}	//������������� ��������

		*Point8 = arg;				//���������
						
		TransfAll();	//���������� ���� ���������� �� ����� �����������
	}
	else
	{
		ROMStringTX(((Point_3[Comm][(*Point8+1)<<1])));	//������
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
	if(InpSTR[StartInd] == '?')	{quest = 1; StartInd++;}		//��������� �������

	switch(MesMode)
	{
//Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---
		case Slope:

		arg = FindAndCheck(String34, Positive, Negative);
		Chan = FindAndCheck(String45, Ch_a, Ch_c);

		if(Chan == 255)						//��������� ������ (���� � ������� �� ���� �� ������)
		{
			if(Channel>1)	{ErrQueue(2);	return;}//������, ���� ������ ����� �
			else Chan = Channel;				//���� ����� ������ �� ������, �� ���������� �������
		}
		else Chan -= Ch_a;

		if(Chan > 1)	{ErrQueue(3);	return;}	//��������� ������ (� ������� ������ �)//������, ���� ������ ����� �

		if(!quest)						//��������� ����������� �������
		{
			if(arg == 255)	{ErrQueue(0);	return;}	//������, ���� �� ������ ��������

			Rise[Chan] = arg;						//���������
			TransfAll();	//���������� ���� ���������� �� ����� �����������
		}
		else
		{
			ROMStringTX(String34[(Rise[Chan]+1)<<1]);	//������
			ROMStringTX("\r");
		}

		break;
//Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---Slope---

//Level_M---Level_M---Level_M---Level_M---Level_M---Level_M---Level_M---Level_M---Level_M---Level_M---
 	  	case Level_M:

		if(InpSTR[StartInd] != LineFeed)	//��������� ����� ������
		{
			StartInd++;
			LevTemp = StringToBin()/2;		//���������� ������������ ������
			Chan = FindAndCheck(String45, Ch_a, Ch_c);
		}

		if(Chan == 255)						//��������� ������ (���� � ������� �� ���� �� ������)
		{
			if(Channel>1)	{ErrQueue(2); return;}	//������, ���� ������ ����� �
			else Chan = Channel;			//���� ����� ������ �� ������, �� ���������� �������
		}
		else Chan -= Ch_a;

		if(Chan > 1)	{ErrQueue(3); return;}		//��������� ������ (� ������� ������ �)//������, ���� ������ ����� �

		if(!quest)							//���������
		{

			if((LevTemp < 4001) && (LevTemp > -4001))
			{
				if((LevTemp < 401) && (LevTemp > -401))	Kin[Chan] = 0;					//�������� ����
				else									{Kin[Chan] = 1; LevTemp /=10;}	//�������� ���
			
				TransfWord(0b00010000000 | Kin[Chan]);					//��������

				Level[Chan] = LevTemp;
				TransfWord(LevTemp | 0x400);	//�������� ����������� ������ �� ������ �������������
			}
			else	{ErrQueue(1);	return;}
		}
		else
		{
			LevTemp = Level[Chan];
			if(Kin[Chan])LevTemp *= 10;
//			FloatToBinDec(LevTemp*2e-3, 3, 4);		//����������� ����� � �������-����������� ����

			RES_Temp.Data = LevTemp*2;						//���������� ��������� ���������
			RES_Temp.Order = 0;
			BinToBinDec(&RES_Temp);
			RAMStringTX(Decades+4);	//������
			ROMStringTX("\r");
		}
		
		break;
//Level_M---Level_M---Level_M---Level_M---Level_M---Level_M---Level_M---Level_M---Level_M---Level_M---

//Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---
 	  	case Auto_M:

		TransfWord(0b01111000011);	//�������� ����������� ������� ����
		Flags.Auto = 1;				//��������� ����� ���������� ������ ����
		Flags.RefrCreen_2 = 1;		//��������� ����� ��������������� ���������� ������

		break; 
//Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---Auto_M---

//Esl---Ttl---Esl---Ttl---Esl---Ttl---Esl---Ttl---Esl---Ttl---Esl---Ttl---Esl---Ttl---Esl---Ttl---
 	  	case Esl:
	  	case Ttl:

		Chan = FindAndCheck(String45, Ch_a, Ch_c);

		if(Chan == 255)					//��������� ������ (���� � ������� �� ���� �� ������)
		{
			if(Channel>1)	{ErrQueue(2); return;}		//������, ���� ������ ����� �
			else Chan = Channel;					//���� ����� ������ �� ������, �� ���������� �������
		}
		else Chan -= Ch_a;

		if(Chan == 2)	{ErrQueue(3);	return;}	//������, ���� ������ ����� �


		Kin[Chan] = 1;								//�������� ���
		TransfWord(0b00010000001);					//�������� ���

		if(MesMode == Esl)		Level[Chan] = -62;  //���
		else 					Level[Chan] = 60;	//���
		TransfWord(Level[Chan] | 0x400);		//�������� ����������� ������ 

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

	if(InpSTR[StartInd] == '?')		//��������� �������
	{
		ROMStringTX(String39[((Mode[0]>>4)+1)<<1]);					//�����
		ROMStringTX(",");

		MesMode = Mode[0]&0b1111;
		if((Channel==Chan_B)&&(Mode[0]>47))
		{
			if(Mode[0]==(HANd+48))		MesMode = WIDe;
			if(Mode[0]==(PERiod+1+48))	MesMode = PERiod;
		}

		ROMStringTX(Point_1[Mode[0]>>4][(MesMode+1)<<1]);	//��������
		ROMStringTX(",");
		ROMStringTX(String46[((MeasTime[0]+N_1m)+1)<<1]);			//T
		ROMStringTX(",");
		ROMStringTX(String46[((MeasTime[0]+N_1)+1)<<1]);			//N
		ROMStringTX(",");
		ROMStringTX(String46[((Notch[0]+N_10n)+1)<<1]);				//Tau
		ROMStringTX(",");

		Chan = Channel+Ch_a;											//��������� ������
		if(Mode[0]==LRATio)			Chan = Channel+Ch_a_c;				//��������� ��� LRATio
		if(Mode[0]==HRATio)			Chan = Channel+Ch_a_b;				//��������� ��� HRATio
		if((Mode[0]==(HANd+48))&&(Channel==Chan_B))		Chan = Ch_b_a;	//��� B(tA)
		if((Mode[0]==(PERiod+1+48))&&(Channel==Chan_B))	Chan = Ch_b_c;	//��� B(TC)
		if(Mode[0]==(WIDe+48))		Chan = ConvTab3[Channel];			//��������� ��� WIDe
		if(Mode[0]==(PERiod+48))	Chan = ConvTab4[Channel];			//��������� ��� PERiod
		ROMStringTX(String45[(Chan+1)<<1]);							//�����(�)

		ROMStringTX("\r");
	}
	else		//��������� ���������
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
				StartInd--;		//��� ��� ����
				case DIRect:
			//Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---
		
				MeasPer = FindAndCheck(String46, N_1m, N_100);
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
		
				Mode[0] = 0;									//��������� �������
				if(Chan != 255)	Channel = Chan-Ch_a;			//��������� ������ ���������
				if(MeasPer != 255)	MeasTime[0] = MeasPer - N_1m;	//��������� ������������ ���������
	
				break;
			//Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---Frequnce---
		
				case LRATio:
			//LRATio---LRATio---LRATio---LRATio---LRATio---LRATio---LRATio---LRATio---LRATio---
		
				NAV = FindAndCheck(String46, N_1, N_100k);
				Chan = FindAndCheck(String45, Ch_a_c, Ch_b_a);
	
				Mode[0] = 1;									//��������� ������
				if(Chan != 255)	Channel = (Chan-Ch_a_c)%3;	//��������� ������ ���������
				if(NAV != 255)	MeasTime[0] = NAV - N_1;		//��������� ���������� �������� ����������
	
				break;
			//LRATio---LRATio---LRATio---LRATio---LRATio---LRATio---LRATio---LRATio---LRATio---
		
				case HRATio:
			//HRATio---//HRATio---//HRATio---//HRATio---//HRATio---//HRATio---//HRATio---//HRATio---
		
				MeasPer = FindAndCheck(String46, N_1m, N_100);
				Chan = FindAndCheck(String45, Ch_a_b, Ch_b_c);
	
				Mode[0] = 2;										//��������� ������
				if(Chan != 255)		Channel = (Chan-Ch_a_b)%3;	//��������� ������ ���������
				if(MeasPer != 255)	MeasTime[0] = MeasPer - N_1m;	//��������� ������������ ���������
	
				break;
			//HRATio---//HRATio---//HRATio---//HRATio---//HRATio---//HRATio---//HRATio---//HRATio---
		
				case _1_T:
			//_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---
	
				Tau = FindAndCheck(String46, N_10n, N_1m);
				NAV = FindAndCheck(String46, N_1, N_100k);
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
	
				Mode[0] = 3;										//F=1/T
				if(Chan != 255)		Channel = Chan-Ch_a;			//��������� ������ ���������
				if(NAV != 255)		MeasTime[0] = NAV - N_1;		//��������� ������������ ���������
				if(Tau != 255)		Notch[0] = Tau-N_10n;			//��������� ����� �������
	
				break;
			//_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---_1_T---
		
				case TACHometr:
			//TACHometr---TACHometr---TACHometr---TACHometr---TACHometr---TACHometr---TACHometr---
		
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
		
				Mode[0] = 4;										//��������
				if(Chan != 255)		Channel = Chan-Ch_a;			//��������� ������ ���������
	
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
				StartInd--;		//��� ��� ����
				case DIRect:
			//Period---Period---Period---Period---Period---Period---Period---Period---Period---
	
				Tau = FindAndCheck(String46, N_10n, N_1m);
				NAV = FindAndCheck(String46, N_1, N_100k);
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
	
				Mode[0] = 16;									//��������� �������
				if(Chan != 255)	Channel = Chan-Ch_a;			//��������� ������ ���������
				if(NAV != 255)		MeasTime[0] = NAV - N_1;	//��������� ������������ ���������
				if(Tau != 255)		Notch[0] = Tau-N_10n;		//��������� ����� �������
	
				break;
			//Period---Period---Period---Period---Period---Period---Period---Period---Period---
	
	
				case _1_F:
			//_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---_1_F---
	
				MeasPer = FindAndCheck(String46, N_1m, N_100);
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
	
				Mode[0] = 17;									//��������� �������
				if(Chan != 255)	Channel = Chan-Ch_a;			//��������� ������ ���������
				if(MeasPer != 255)	MeasTime[0] = MeasPer - N_1m;//��������� ������������ ���������
	
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
				StartInd--;		//��� ��� ����
				case DIRect:
			//Width---Width---Width---Width---Width---Width---Width---Width---Width---Width---
	
				Tau = FindAndCheck(String46, N_10n, N_1m);
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
	
				Mode[0] = 32;									//��������� ������������
				if(Chan != 255)		Channel = Chan-Ch_a;		//��������� ������ ���������
				if(Tau != 255)		Notch[0] = Tau-N_10n;		//��������� ����� �������
	
				break;
			//Width---Width---Width---Width---Width---Width---Width---Width---Width---Width---
	
				case AVErage:
			//AVErage---AVErage---AVErage---AVErage---AVErage---AVErage---AVErage---AVErage---
	
				Tau = FindAndCheck(String46, N_10n, N_1m);
				NAV = FindAndCheck(String46, N_1, N_100k);
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
	
				Mode[0] = 33;									//��������� ������������ � �����������
				if(Chan != 255)		Channel = Chan-Ch_a;		//��������� ������ ���������
				if(NAV != 255)		MeasTime[0] = NAV - N_1;	//��������� ������������ ���������
				if(Tau != 255)		Notch[0] = Tau-N_10n;		//��������� ����� �������
	
				break;
			//AVErage---AVErage---AVErage---AVErage---AVErage---AVErage---AVErage---AVErage---
	
				case HDEFinition:
			//HDEFinition---HDEFinition---HDEFinition---HDEFinition---HDEFinition---HDEFinition---
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
		
				Mode[0] = 34;										//��������� ������������ � ���������� ���������
				if(Chan != 255)		Channel = Chan-Ch_a;			//��������� ������ ���������
	
				break;
			//HDEFinition---HDEFinition---HDEFinition---HDEFinition---HDEFinition---HDEFinition---
	
				case INTerval:
			//INTerval---INTerval---INTerval---INTerval---INTerval---INTerval---INTerval---
	
				Tau = FindAndCheck(String46, N_10n, N_1m);
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
	
				Mode[0] = 35;									//��������� ���������� ���������
				if(Chan != 255)		Channel = Chan-Ch_a;		//��������� ������ ���������
				if(Tau != 255)		Notch[0] = Tau-N_10n;		//��������� ����� �������
	
				break;
			//INTerval---INTerval---INTerval---INTerval---INTerval---INTerval---INTerval---
	
				case DCYCle:
			//DCYCle---DCYCle---DCYCle---DCYCle---DCYCle---DCYCle---DCYCle---DCYCle---DCYCle---
	
				Tau = FindAndCheck(String46, N_10n, N_1m);
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
	
				Mode[0] = 36;									//��������� ����������
				if(Chan != 255)		Channel = Chan-Ch_a;		//��������� ������ ���������
				if(Tau != 255)		Notch[0] = Tau-N_10n;		//��������� ����� �������
	
				break;
			//DCYCle---DCYCle---DCYCle---DCYCle---DCYCle---DCYCle---DCYCle---DCYCle---DCYCle---
	
				case PHASe:
			//PHASe---PHASe---PHASe---PHASe---PHASe---PHASe---PHASe---PHASe---PHASe---PHASe---
	
				Tau = FindAndCheck(String46, N_10n, N_1m);
				Chan = FindAndCheck(String45, Ch_a, Ch_c);
	
				Mode[0] = 37;									//��������� ����
				if(Chan != 255)		Channel = Chan-Ch_a;		//��������� ������ ���������
				if(Tau != 255)		Notch[0] = Tau-N_10n;		//��������� ����� �������
	
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
		
				Mode[0] = 48;										//���� ��������� � ������ ��������
				if(Chan != 255)		Channel = Chan-Ch_a;			//��������� ������ ���������
	
				break;
			//HANd---HANd---HANd---HANd---HANd---HANd---HANd---HANd---HANd---HANd---
	
				case WIDe:
			//WIDe---WIDe---WIDe---WIDe---WIDe---WIDe---WIDe---WIDe---WIDe---WIDe---
	
				Chan = FindAndCheck(String45, Ch_a_c, Ch_b_c);
		
				if(Chan == Ch_b_a)	Mode[0] = 48;					//���� ��������� �� ����� ��������(��� ��������� � ������ ������)
				else 				Mode[0] = 49;					//���� ��������� �� ����� ��������
	
				if(Chan != 255)		Channel = ConvTab[Chan-Ch_a_c];	//��������� ������ ���������
	
				break;
			//WIDe---WIDe---WIDe---WIDe---WIDe---WIDe---WIDe---WIDe---WIDe---WIDe---
	
				case PERiod:
			//PERiod---PERiod---PERiod---PERiod---PERiod---PERiod---PERiod---PERiod---
	
				NAV = FindAndCheck(String46, N_1, N_100k);
				Chan = FindAndCheck(String45, Ch_a_c, Ch_b_c);
		
				if(Chan == Ch_b_c)	Mode[0] = 51;					//���� ��������� �� ������ �������� (��� ��������� � ��������� ������)
				else 				Mode[0] = 50;					//���� ��������� �� ������ ��������
	
				if(Chan != 255)	Channel = ConvTab[Chan-Ch_a_c];	//��������� ������ ���������
				if(NAV != 255)		MeasTime[0] = NAV - N_1;	//��������� ������������ ���������
	
				break;
			//PERiod---PERiod---PERiod---PERiod---PERiod---PERiod---PERiod---PERiod---
			}
			break;
//CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---CNT---
	
			default:
			ErrQueue(4);
			return;
		}
	TransfAll();	//���������� ���� ���������� �� ����� �����������
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


			//������� �������� �������
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
	if(!((Temp >= '0')&&(Temp <= '9')))		return 0x4000;	//����� ���� �� ���������� �����//!((InpSTR[StartInd+i] > 0x2F) && (InpSTR[StartInd+i] < 0x3A))

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
//������� ��������� ���������� Data � ������ ��������� ������� ComArray
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
//������� �������� ����� �������� ����
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
void TransfAll (void) 	//�������� �� ����� ����������� ���� ��������
{
	TransfWord (0b00000000000 | Channel);			//����� �
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
//������� ������ ��������� �� �������
void MesErr (char ER)
{
	Text_To_Display(0, 0, "������");
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
	//���������� Dat � ������� deg
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
	//���������� Dat � ������� deg
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
	//����������� �� ���������� ���������� ���������
void IndRes(void)
{
	signed char TempR, TempOr;

	INTCONbits.TMR0IE = 0;		//������ ���������� �� ������ TMR0
	Nomber_Image_Char(72, 3, &RES_Dat, SignDig);			//����������� �� ���������� ���������� ���������
	INTCONbits.TMR0IE = 1;		//���������� ���������� �� ������ TMR0

	if(Compare(Mode[0], CNT_Const)) TempR = 3;
	else TempR = Mode[0]>>4;


	if(Mode[0]==34 && Flags.Test)	Text_To_Display(238, 5, "   ");
	else							Text_To_Display(238, 5, MeasLable[TempR][RES_Dat.IndOrder+4]);		//���������� ������� ���������
}
//-----------------------------------------------------

char FindAllDiction	(char Ind)
{
	char Temp, flag = 1, i;

	for(i=0; i<18; i++)
	{
		StartInd = Ind;
		Temp = FindString(Point_5[i]);	//���������� ������������ ������
		if(Temp != 255)		flag = 0;	//����������
	}

	if(flag==0)
	{
		StartInd = Ind-1;
		return 0; //����������
	}
	else 	return 1; //��� ����������
}

//-----------------------------------------------------
char FindAndCheck (rom char *rom *Pointer, char min, char max)
{
	char TempInd, Temp;

	if(InpSTR[StartInd] != LineFeed)	//��������� ����� ������
	{
		StartInd++;
		TempInd = StartInd;
		Temp = FindString(Pointer);	//���������� ����� ������
		if(Temp == 255)		{if(FindAllDiction(TempInd)) 	{ErrQueue(0);	return 255;}}	//�������������� ������
		else {if(!((Temp <= max)&&(Temp >= min)))			{ErrQueue(3);	StartInd = TempInd-1;	return 255;}}	//������������ ��������

		return Temp;
	}

	return 255;
}
//-----------------------------------------------------
/*//-----------------------------------------------------*/




