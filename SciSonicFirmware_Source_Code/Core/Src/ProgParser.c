/*
 * ProgParser.c
 *
 *  Created on: 23 нояб. 2019 г.
 *      Author: volodai
 */

#include "ProgParser.h"
#include <stdio.h>
#include <string.h>
#include "MyFlah.h"

Xs Xses[MAXIOVAR];
bool Mvars[MAXMVAR];
Timer_S Timers[MAXTVAR];
bool Coils[MAXCOIL];
uint16_t Counters[MAXCNTVAR];

CntChannels CntChnlsVal;
CntDataStr CntData;
//---------------------------------------------------------------------------
void ParsersInit()
{
  memset(Xses,0,sizeof(Xs)*MAXIOVAR);
  memset(Timers,0,sizeof(Timer_S)*MAXTVAR);
  memset(Mvars,0,sizeof(bool)*MAXMVAR);
  memset(Coils,1,sizeof(bool)*MAXCOIL);
}
//---------------------------------------------------------------------------
void SetUp_PullUp_Mask(uint16_t mask)
{
int n = MAXIVAR+2;
if(n>16) n=16;
bool temp;
for(int i=0; i<n; i++)
{
	temp=(bool)(mask&1);
	mask=mask>>1;
	switch(i)
	{
		case 2:
			{
				if(temp)
					HAL_GPIO_WritePin(NP1_GPIO_Port,NP1_Pin,GPIO_PIN_RESET);
				else
					HAL_GPIO_WritePin(NP1_GPIO_Port,NP1_Pin,GPIO_PIN_SET);
				break;
			}
		case 3:
			{
				if(temp)
					HAL_GPIO_WritePin(NP2_GPIO_Port,NP2_Pin,GPIO_PIN_RESET);
				else
					HAL_GPIO_WritePin(NP2_GPIO_Port,NP2_Pin,GPIO_PIN_SET);
				break;
			}
		case 4:
			{
				if(temp)
					HAL_GPIO_WritePin(NP3_GPIO_Port,NP3_Pin,GPIO_PIN_RESET);
				else
					HAL_GPIO_WritePin(NP3_GPIO_Port,NP3_Pin,GPIO_PIN_SET);
				break;
			}
		case 5:
			{
				if(temp)
					HAL_GPIO_WritePin(NP4_GPIO_Port,NP4_Pin,GPIO_PIN_RESET);
				else
					HAL_GPIO_WritePin(NP4_GPIO_Port,NP4_Pin,GPIO_PIN_SET);
				break;
			}
		case 6:
			{
				if(temp)
					HAL_GPIO_WritePin(NP5_GPIO_Port,NP5_Pin,GPIO_PIN_RESET);
				else
					HAL_GPIO_WritePin(NP5_GPIO_Port,NP5_Pin,GPIO_PIN_SET);
				break;
			}
    }
}
}
//---------------------------------------------------------------------------
void ParseXString(uint8_t * XString, int xLen)
{
	Xparm x;
	uint8_t * str = XString;
	uint8_t temp[4];
	int val=0;
	memcpy(temp,str,3);
	temp[3]=0;
	sscanf((char*)(temp+1),"%d",&val);
	x.InNum = (uint8_t)val;	//MESUT Hangi input olduğunu alıyor
	x.Inletter = temp[0];	//MESUT  F, I, C girislerini seciyor.
	str=XString+3;	// > veya < operasyonunu belirlemek icin kullanılıyor
	x.ccondition = *str;
	str=str+1;		// condition dan sonra değer gelecek, bu % mi yoksa Voltaj değeri mi bakılıyor.
	if(*str=='%')
	{
		x.percent=true;	//% ise percent true yapılıyor
		str=str+1;		//yüzde kaç olduğu alınıyor.
	}
	else x.percent=false;	//% değil ise normal değeri alacak.
	memcpy(temp,str,6);		//conditiondan sonraki değer temp e atanıyor. 12000 milivolt diyelim 5 karakter oradan, bir de B karakteri. toplam 6 karakter.
	for(int i=0; i<6; i++)	//<%15BX01 gibi bir ifadede B ifadesini kontrol etmek için for döngüsüne sokuyor.
	{
		if(temp[i]=='B')
		{
			temp[i]=0;
			sscanf((char*)temp,"%d",&val);
			x.value=(uint8_t)val;	//MESUT x.value condition  dan sonraki deger.
			str=str+i+2;			//hangi input olduğunu belirlemek için oraya gidiliyor
			sscanf((char*)str,"%d",&val);
			x.xnum=(uint8_t)val;
			Xses[x.xnum].ConditionValue=x.value;
			Xses[x.xnum].Condition=x.ccondition;
			if(x.Inletter=='F')
			{
				if(x.InNum==0)
				{
					if(!x.percent) Xses[x.xnum].Input=&Channels.FB1;
					else Xses[x.xnum].Input=&Channels.FB1p;
				}
				else if(x.InNum==1)
				{
					if(!x.percent) Xses[x.xnum].Input=&Channels.FB2;
					else Xses[x.xnum].Input=&Channels.FB2p;
				}
			}
			else if(x.Inletter == 'I')
			{
				switch(x.InNum)
				{
					case 0:
					{
						if(!x.percent) Xses[x.xnum].Input=&Channels.CH1;
						else  Xses[x.xnum].Input=&Channels.CH1p;
						break;
					}
					case 1:
					{
						if(!x.percent) Xses[x.xnum].Input=&Channels.CH2;
						else  Xses[x.xnum].Input=&Channels.CH2p;
						break;
					}
					case 2:
					{
						if(!x.percent) Xses[x.xnum].Input=&Channels.CH3;
						else  Xses[x.xnum].Input=&Channels.CH3p;
						break;
					}
					case 3:
					{
						if(!x.percent) Xses[x.xnum].Input=&Channels.CH4;
						else  Xses[x.xnum].Input=&Channels.CH4p;
						break;
					}
					case 4:
					{
						if(!x.percent) Xses[x.xnum].Input=&Channels.CH5;
						else  Xses[x.xnum].Input=&Channels.CH5p;
						break;
					}
				}
			}
			else if(x.Inletter == 'C')
			{

				Xses[x.xnum].Input=&CntData.CntVal[x.InNum];

				switch(x.InNum)
				{
					case 0:
					{
						Xses[x.xnum].Input=&CntChnlsVal.CNT0;
						break;
					}
					case 1:
					{
						Xses[x.xnum].Input=&CntChnlsVal.CNT1;
						break;
					}
					case 2:
					{
						Xses[x.xnum].Input=&CntChnlsVal.CNT2;
						break;
					}
					case 3:
					{
						Xses[x.xnum].Input=&CntChnlsVal.CNT3;
						break;
					}
					case 4:
					{
						Xses[x.xnum].Input=&CntChnlsVal.CNT4;
						break;
					}
					case 5:
					{
						Xses[x.xnum].Input=&CntChnlsVal.CNT5;
						break;
					}
					case 6:
					{
						Xses[x.xnum].Input=&CntChnlsVal.CNT6;
						break;
					}
					case 7:
					{
						Xses[x.xnum].Input=&CntChnlsVal.CNT7;
						break;
					}
					case 8:
					{
						Xses[x.xnum].Input=&CntChnlsVal.CNT8;
						break;
					}
					case 9:
					{
						Xses[x.xnum].Input=&CntChnlsVal.CNT9;
						break;
					}
					case 10:
					{
						Xses[x.xnum].Input=&CntChnlsVal.CNT10;
						break;
					}
					case 11:
					{
						Xses[x.xnum].Input=&CntChnlsVal.CNT11;
						break;
					}
					case 12:
					{
						Xses[x.xnum].Input=&CntChnlsVal.CNT12;
						break;
					}
					case 13:
					{
						Xses[x.xnum].Input=&CntChnlsVal.CNT13;
						break;
					}
					case 14:
					{
						Xses[x.xnum].Input=&CntChnlsVal.CNT14;
						break;
					}
					default:
					{
						break;
					}
				}
			}
			else
			{
				//Do Nothin. Give Error
			}
			break;
        }
	}
}
//---------------------------------------------------------------------------
void ProcessXs()
{
  for(int i=0; i<MAXIOVAR; i++)
	{
		if(Xses[i].Input!=NULL && (Xses[i].Condition=='>' || Xses[i].Condition=='<' || Xses[i].Condition=='='))
		{
			if(Xses[i].Condition=='>')
			{
			  Xses[i].Value=(bool)((int)*Xses[i].Input > (int)Xses[i].ConditionValue);
			}
			else if(Xses[i].Condition=='<')
			{
			  Xses[i].Value=(bool)((int)*Xses[i].Input < (int)Xses[i].ConditionValue);
			}
			else if(Xses[i].Condition=='=')
			{
			  Xses[i].Value=(bool)((int)*Xses[i].Input == (int)Xses[i].ConditionValue);
			}
		}
	}
}
//---------------------------------------------------------------------------
void ProcessCoils()
{
	if(CareOuts)
	{
	for(int i=0; i<MAXCOIL; i++)
	  {
	   switch(i)
	   {
	   case 0:
	   	   	   {
	   	   		 if(Coils[i])
	   	   			 HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin,GPIO_PIN_SET);
	   	   		 else
	   	   			 HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin,GPIO_PIN_RESET);
	   	   		 break;
	   	   	   }
	   case 1:
	   	   	   {
	   	   		 if(Coils[i])
	   	   			 HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin,GPIO_PIN_SET);
	   	   		 else
	   	   			 HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin,GPIO_PIN_RESET);
	   	   		 break;
	   	   	   }
	   default:
		   	   break;

	   }
	  }
	}
}
//---------------------------------------------------------------------------
void ProcessTimer(Timer_S *Timer)
{
	if(Timer->Type==TN)
	{
	  if(!Timer->En)
		{
			Timer->Q=false;
			Timer->StartTick=HAL_GetTick();
		}
	  else
		{
			if((HAL_GetTick()-Timer->StartTick)>=Timer->Delay)
			{
				Timer->Q=true;
			}
			Timer->Q = (bool)((HAL_GetTick()-Timer->StartTick)>=Timer->Delay);
		}
	}
	else if(Timer->Type==TF)
	{
	  if(Timer->En)
		{
			Timer->Q=true;
			Timer->StartTick=HAL_GetTick();
		}
	  else
		{
			if((HAL_GetTick()-Timer->StartTick)>Timer->Delay) Timer->Q = false;
		}
	}
}
//---------------------------------------------------------------------------
void ProcessTimers()
{
	for(int i=0; i<MAXTVAR; i++)
	{
	  ProcessTimer(&Timers[i]);
	}
}
//---------------------------------------------------------------------------
bool SetTimerParams(Timer_S *Timer, bool En, uint8_t Type, uint32_t Delay)
{
	if(Timer->Type!=Type)
		{
		 memset(Timer,0,sizeof(Timer_S));
		 Timer->Type=Type;
		 Timer->StartTick=HAL_GetTick();
		}
	Timer->Delay=Delay;
	Timer->En=En;
	ProcessTimer(Timer);
	return Timer->Q;
}
//---------------------------------------------------------------------------
bool ProcessProgLine(uint8_t *Progstring, uint32_t len, bool *result)
{
	uint8_t *MyIDX=Progstring;
	uint32_t Mylen=0;
	uint8_t data[6];
	uint8_t letter;
	uint8_t prevop=0;
	int index;
	uint8_t op=0;

	if(MyIDX[0]=='(')
	{
		MyIDX=MyIDX+1;
		while((MyIDX+Mylen)<(Progstring+len))
		{
			if(MyIDX[Mylen]==')')
			{
				bool aa=*result;
				aa = ProcessProgLine(MyIDX,Mylen, &aa);
				if(prevop==OR) *result=*result|aa;
				else if(prevop==AND) *result=*result&aa;
				else if(prevop==0) *result = aa;
				prevop=0;
				MyIDX=MyIDX+Mylen+1;
				Mylen=0;
				break;
			}
			else Mylen=Mylen+1;
		}
	}
	while((MyIDX+Mylen)<(Progstring+len))
	{
		if(MyIDX[0]==TF || MyIDX[0]==TN)
		{
			op = MyIDX[0];
			MyIDX=MyIDX+2;
			memcpy(data,MyIDX,2);
			data[2]=0;
			sscanf((char*)data,"%d",&index);
			MyIDX=MyIDX+3;
			memcpy(data,MyIDX,8);
			data[8]=0;
			for(letter=0; letter<8; letter++)
			{
				if(data[letter]==DELIM)
				{
				   data[letter]=0;
				   MyIDX=MyIDX+letter+1;
				   int val;
				   sscanf((char*)data,"%d",&val);
				   bool aa=*result;
				   *result=SetTimerParams(&Timers[index],aa,op,val);
				   prevop=0;
				   break;
				}
			}
		}
		else if(MyIDX[0]==OR)
		{
		  MyIDX=MyIDX+1;
		  prevop=OR;
		}
		else if(MyIDX[0]==AND)
		{
			MyIDX=MyIDX+1;
			prevop=AND;
		}
		else if(MyIDX[0]==')')
		{
			MyIDX=MyIDX+1;
			Mylen=0;
		}
		else if(MyIDX[0]==OT || MyIDX[0]==NOT)
		{
			op = MyIDX[0];
			MyIDX=MyIDX+1;
			letter = MyIDX[0];
			MyIDX=MyIDX+1;
			memcpy(data,MyIDX,2);
			MyIDX=MyIDX+2;
			data[2]=0;
			sscanf((char*)data,"%d",&index);
			bool val;
			if(letter=='X') val=Xses[index].Value;
			else if(letter=='M') val=Mvars[index];
			else if(letter=='Y') val=Coils[index];
			else if(letter=='T') val=Timers[index].Q;
			else val=false;
			if(op==NOT) val=!val;
			if(prevop==OR) *result=*result|val;
			else if(prevop==AND) *result=*result&val;
			else if(prevop==0) *result=val;
			prevop=0;
		}
		else if(MyIDX[0]==SET || MyIDX[0]==RES || MyIDX[0]==MOV || MyIDX[0]==CNTP || MyIDX[0]==CNTN || MyIDX[0]==CNTE)
		{
			op = MyIDX[0];
			MyIDX=MyIDX+1;
			letter = MyIDX[0];
			MyIDX=MyIDX+1;
			memcpy(data,MyIDX,2);
			data[2]=0;
			sscanf((char*)data,"%d",&index);
			if(op==SET && *result)
			{
				if(letter=='X')
				{
					Xses[index].Value=true;
				}
				else if(letter=='Y')
				{
					Coils[index]=true;
				}
				else if(letter=='M')
				{
					Mvars[index]=true;
				}
				*result = true;
			}
			else if(op==RES && *result)
			{
				if(letter=='X')
				{
					Xses[index].Value=false;
				}
				else if(letter=='Y')
				{
					Coils[index]=false;
				}
				else if(letter=='M')
				{
					Mvars[index]=false;
				}
				*result=false;
			}
			else if(op==MOV)
			{
				if(letter=='X')
				{
					Xses[index].Value=*result;
				}
				else if(letter=='Y')
				{
					Coils[index]=*result;
				}
				else if(letter=='M')
				{
					Mvars[index]=*result;
				}
			}
			else if((op == CNTP || op == CNTN || op == CNTE) && *result)
			{
				int val;
				if(op == CNTE)
				{
					MyIDX=MyIDX+3;
					memcpy(data,MyIDX,8);
					data[8]=0;
					for(letter=0; letter<8; letter++)
					{
						if(data[letter]==DELIM)
						{
							data[letter]=0;
							MyIDX=MyIDX+letter+1;
							sscanf((char*)data,"%d",&val);
							break;
						}
					}
				}
				if(op == CNTP)
				{
					CntData.CntVal[index]++;
				}
				else if(op == CNTN)
				{
					if(CntData.CntVal[index] > 0)
					{
						CntData.CntVal[index]--;
					}
					else
					{
						CntData.CntVal[index] = 0;
					}
				}
				else if(op == CNTE)
				{
					CntData.CntVal[index] = val;
				}
			}
			return *result;
		}
		else
		{
			MyIDX=MyIDX+1;
			Mylen=0;
		}
	}
	return *result;
}
//---------------------------------------------------------------------------
void ProgInit()
{
	if(!FlashIsProgrammed) return;
	if(ProgrammingMode) return;
	ParsersInit();
	//W25qxx_ReadPage(pageData,1,0,256);
	Flash_ReadPage(pageData,1,256);
	uint8_t *data = pageData + 3; //MESUT, Led icin kod yazildi L5\n gibi
	uint16_t count, mask;
	memcpy(&mask, data, 2);
	if(mask>255)
	{
		FlashIsProgrammed=false;
		return;
	}

	SetUp_PullUp_Mask(mask);
	data=data+2; //MESUT pageData[4] pageData[5] dataları kac tane input oldugunu veriyor
	memcpy(&count, data, 2); //data input sayisi 5 ve Count a aktariliyor.

	if(count>MAXIOVAR)
	{
		FlashIsProgrammed=false;
		return;
	}

	data=data+2; //MESUT pageData[6]

	for(int i=0; i<count; i++)
	{
		memcpy(&mask, data, 2);
		if(mask>20)
		{
			FlashIsProgrammed=false;
			return;
		}
		data=data+2;
		ParseXString(data,mask);
		data=data+mask;
	}

	memcpy(&mask, data, 2);
	data=data+2;
	ProgLen=mask;
	pageDataIndex = (uint16_t)(data-pageData);
	ProgInited=true;
}
//---------------------------------------------------------------------------
void ProcessProgramm()
{
	uint8_t * ProgI = pageData+pageDataIndex;
	uint16_t outlen=0;
	uint16_t inlen=0;
	uint16_t index=0;
	uint8_t Page=0;
	bool result=true;

	//W25qxx_ReadPage(pageData,(uint32_t)(Page+1),0,256);
	Flash_ReadPage(pageData,(uint32_t)(Page+1),256);
	uint8_t ProgIDX[256];
	while(index<ProgLen)
	{
		if(ProgI>=(pageData+256)) //страница  закончилась
		{
			Page=Page+1;
			ProgI=pageData;
			//W25qxx_ReadPage(pageData,(uint32_t)(Page+1),0,256);
			Flash_ReadPage(pageData,(uint32_t)(Page+1),256);
			inlen=0;
		}
		if(ProgI[inlen]==ACT_DELIM)
		{
			result=true;
			ProgIDX[outlen]=0;
			ProcessProgLine(ProgIDX,outlen, &result);
			outlen=0;
			ProgI=ProgI+inlen+1;
			//index=index+1;
			inlen=0;
		}
		else
		{
			ProgIDX[outlen]=ProgI[inlen];
			outlen=outlen+1;
			inlen=inlen+1;
		}
		index=index+1;
	}
	if(outlen>0)
	{
		result=true;
		ProgIDX[outlen]=0;
		ProcessProgLine(ProgIDX,outlen, &result);
	}
}
//---------------------------------------------------------------------------
void ProcessIO()
{
	if(ProgrammingMode || !FlashIsProgrammed || !ProgInited)
		{
		HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(NP1_GPIO_Port,NP1_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(NP2_GPIO_Port,NP2_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(NP3_GPIO_Port,NP3_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(NP4_GPIO_Port,NP4_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(NP5_GPIO_Port,NP5_Pin,GPIO_PIN_SET);
		if(!ProgrammingMode && FlashIsProgrammed && !ProgInited)
			ProgInit();
		else if(!ProgrammingMode && !FlashIsProgrammed && !ProgInited)
		{
			TestFlash();
		}
		else
			return;
		}
	ProcessXs();
	ProcessTimers();
	ProcessProgramm();
	ProcessCoils();
}
//---------------------------------------------------------------------------
