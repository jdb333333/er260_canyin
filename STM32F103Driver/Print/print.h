/****************************************Copyright (c)**************************************************
*        EUTRON Human Information Equipment Co.,Ltd.
*                    www.eutron.com.cn
* File name:		Print.H
* Descriptions:     DEFINITIONS FOR PRINTE
*------------------------------------------------------------------------------------------------------
*  Created by:			chen churong
* Created date:        2011-10-01
* Version: 1.0
********************************************************************************************************/

#ifndef __PRINT_H
#define __PRINT_H

#define DEBUG   1   //use for Debug

#if (DEBUG && (!defined(DEBUGBYPC)))
#include "monitor.h"
#endif

#define sDEBUG(msg) UART_PutString(msg)
#define cDEBUG(c)   UART_PutChar(c)

/////////////////////////////////////////////////////////////////////////////////////
#define DEBUGONPC   0   //debug on pc if set to 1

#define FOR_POS     0   //=1时，用做ＰＯＳ软件的打印机，不支持字符串中的倍高字符@和倍宽字符~

#define DCPOWER     1   //1-Use DC Adapter；0-Use battery，此时加热次数与GROUPMODE无关

#define WORDorBYTE  0   //是以WORD(1)还是以BYTE(0)向打印机发送数据
#define GROUPMODE   2   //=0:点阵数据一次发送,按GROUPS规定的组数逐组加热
                        //=1:每次发送一组所允许的最多点阵,点阵数据每加热一次发送一次，与GROUPS无关
                        //=2:点阵数据分组组合成一组所允许的最多点阵后加热,点阵数据一次发送，与GROUPS无关
// SPI_DEV_Init
//#define HZTIMES     (12.78*uSECOND)  // SPI读取3字节汉字点阵所需要的时间(微妙)
#define HZTIMES     (10.1*uSECOND)     //SPI_CLOCKRATE_HIGH:25M SSP0读取3字节汉字点阵所需要的时间(微妙)
//#define HZTIMES      (9.2*uSECOND)  //SPI_CLOCKRATE_33M SSP0读取3字节汉字点阵所需要的时间(微妙)

#define ASCTIMES     (uSECOND)  //处理一个ASCII点阵所需要的时间(微妙)
#define PDATATIME    (62*uSECOND) //送一行打印点阵所需要的时间(微妙)

#define PORTHOST    1//ouhs  0 // port used for link to host
/////////////////////////////////////////////////////////////////////////////////////

///////////// type definitions of printer ////////////////////////////
//#define CASE_LTP1245    0
//#define CASE_CAPD247    0   //Seiko Instruments Inc.(SII)
//#define CASE_CAPD347    0   //Seiko Instruments Inc.(SII)
//#define CASE_LTPZ245   0   //Seiko Instruments Inc.(SII)
//#define CASE_CAPD245   0   //Seiko Instruments Inc.(SII)

#ifdef CASE_ER380
#define CASE_PT486F    0   //Same as FTP628?
#define	CASE_FTP628MCL401	1
#define	CASE_PT723F_RJ	0

#elif defined(CASE_RJ)  //为票据R和存根J双站打印

#define CASE_PT486F    0   //Same as FTP628?
#define	CASE_FTP628MCL401	0
#define	CASE_PT723F_RJ	1   //RJ双站打印

#else

#define CASE_PT486F    1   //Same as FTP628?
#define	CASE_FTP628MCL401	0
#define	CASE_PT723F_RJ	0   //RJ双站打印

#endif

#define ENCUTTER    CASE_FTP628MCL401   //The autocutter machanism is ok

///////////////////////////////////////////////////////////////////////

#if defined(DEBUGBYPC)

#include "TypeDef.h"
#define uSECOND 1

#define CONST

#else

#include "include.h"

#define CONST const
#endif

#define CASE_HANDUP	0	//hand flag,0时无压杆检测

/////////////////////////// task definitions /////////////////////////////////////
#define TASK00_Stop				0
#define TASK01_Start			1
#define TASK02_PrintChar		2
#define TASK03_PrintGraphic     3
#define TASK04_FeedPaper        4
#define TASK05_FeedHalf         5
#define TASK10_StopCutter       10
#define TASK11_ResetCutter      11
#define TASK12_FeedCutter       12
#define TASK13_FeedStepCA       13
#define TASK14_FeedStepE        14
#define TASK15_FeedStepAlpha    15
#define TASK16_CutPaper         16
#define TASK17_FeedStepB        17
#define TASK18_FeedStepC        18
#define TASK19_RevFeed8Line     19


/////////// bit Command of print:ApplVar.Prn_Command ///////////////////////////////////////
#define PRN_DBLW_Mode        BIT0    //double with print mode for text
#define PRN_DBLH_Mode        BIT1    //double high print mode for text
#define PRN_RECEIPT_Mode     BIT2    //print on receipt printer
#define PRN_STUB_Mode        BIT3    //print on stub printer
#define PRN_GRAPHIC_Mode     BIT4    //print a graphic
#define PRN_CUTF_Mode        BIT5    //Cut paper full
#define PRN_CUTH_Mode        BIT6    //Cut paper half
#define PRN_CUTRESET_Mode    BIT7    //reset cutter

//* command list for print*/
#define PRNCMD_rTEXT        1   //print text on receipt
#define PRNCMD_sTEXT        2   //print text on stub
#define PRNCMD_rsTEXT       3   //print text on Receipt & stub

#define PRNCMD_rTEXT_DW     4   //print text on receipt double with mode
#define PRNCMD_sTEXT_DW     5   //print text on stub double with mode
#define PRNCMD_rsTEXT_DW    6   //print text on Receipt & stub double with mode

#define PRNCMD_rTEXT_DH     7   //print text on receipt double high mode
#define PRNCMD_sTEXT_DH     8   //print text on stub double high mode
#define PRNCMD_rsTEXT_DH    9   //print text on Receipt & stub double high mode

#define PRNCMD_rTEXT_DWH    10   //print text on receipt double high & double width mode
#define PRNCMD_sTEXT_DWH    11   //print text on stub double high & double width mode
#define PRNCMD_rsTEXT_DWH   12   //print text on Receipt & stub double high & double width mode

#define PRNCMD_rGRA         14   //Print Graphic on receipt
#define PRNCMD_sGRA         15   //Print Graphic on stub
#define PRNCMD_RSGRA        16   //Print Graphic on receipt & stub

#define PRNCMD_rCUT         17   //cut receipt paper
#define PRNCMD_sCUT         18   //cut stub paper
#define PRNCMD_rsCUT        19   //cut receipt & stub paper

#define PRNCMD_rCUTRESET    20   //reset receipt cutter
#define PRNCMD_sCUTRESET    21   //reset stub cutter

#define PRNCMD_MAX          22  //

//////////// Status for printer /////////////////////////////////////
#define ENDOFPAPER      1   //无纸no paper
#define TOOHOT          2   //太热
#define HANDUP          4   //压杆抬起

///////////////////////////////////////////////////////////////////


#define PB_ONLYMOVEMOTOR 0
#define PB_ACTCMD 1
#define PB_DATA   2

#define PRINT_MAJOR 202
#define PRINT_MINOR 0

#define ErrNOMEM      1
#define ErrFAULT      2
/* definitions for printer*/


//////////// Phase definitions for printer(GPIOF) ///////////////////////////
#ifdef CASE_ER50	//by ouhs

#define RPH1     GPIO_Pin_11       //PA   pf.7
#define RPH2     GPIO_Pin_10       //PB   pf.9
#define RPH3     GPIO_Pin_9       //PA-  pf.6
#define RPH4     GPIO_Pin_8       //PB-  pf.8

#elif CASE_PT723F_RJ //为双站打印

#define RPH2     0x80 	// paper motor
#define RPH1     0x40
#define RPH4     0x20
#define RPH3     0x10

#else //ER220   ER260

#define RPH1     0x80 	// paper motor
#define RPH2     0x40
#define RPH3     0x20
#define RPH4     0x10

#endif



#define RPHMASK  (RPH1 | RPH2 | RPH3 | RPH4)

#if CASE_CAPD245 //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#define GROUPS          6             //groups for hot 分组加热组数
#define STEPS1LINE      4             //steps for feed 1 line
#define PHASEMAX        8

#define VMAXSPEED       72
#define PRNWIDTH        32              //characters can print out of 1 line

#define PHSTART        (RPH1)      		        // L L H H

#define TIMESTART       (5000*uSECOND)      //us
#define TIMESTOP        (2000l*uSECOND)      //us

extern CONST ULONG TABSTROBE24V[32][15];
#elif CASE_PT486F //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#define GROUPS          6             //groups for hot 分组加热组数
//#define STEPS1LINE      2             //steps for feed 1 line
#define STEPS1LINE      4             //HJ steps for feed 1 line

//#define PHASEMAX        4
#define PHASEMAX        8	//HJ

//#define VMAXSPEED       17
#define VMAXSPEED       60	//HJ
#define PRNWIDTH        32              //打印宽度

#define HEATWIDTH		32              //打印机一行加热总字符数

//#define PHSTART        (RPH2 | RPH3)      		        // L L H H
#define PHSTART        (RPH2 | RPH4)      		        // HJ

#define TIMESTART       (2000*uSECOND)      //us
#define TIMESTOP        (2000l*uSECOND)      //us

extern CONST ULONG TABSTROBE24V[32][15]; //HJ
extern CONST ULONG TABSTROBE24VB[32];

#elif CASE_PT723F_RJ  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#define GROUPS          6             //groups for hot 分组加热组数
#define STEPS1LINE      4             //HJ steps for feed 1 line

#define PHASEMAX        8	//HJ

#define VMAXSPEED       50	//HJ
#define PRNWIDTH        20              //打印宽度

#define HEATWIDTH		48     //打印机一行加热总字符数(R+J)

#define PHSTART        (RPH2 | RPH4)      		        // HJ

#define TIMESTART       (2000*uSECOND)      //us
#define TIMESTOP        (2000l*uSECOND)      //us

extern CONST ULONG TABSTROBE24V[32][15]; //HJ
extern CONST ULONG TABSTROBE24VB[32];


#elif	CASE_FTP628MCL401   //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#define GROUPS          6             //groups for hot 分组加热组数
#define STEPS1LINE      4             //steps for feed 1 line
#define PHASEMAX        8

#define VMAXSPEED       26				//HJ 打印加速步数
#define PRNWIDTH        32              //打印宽度

#define HEATWIDTH		32      //打印机一行加热总字符数

#define PHSTART        	RPH1

#define TIMESTART       (5000*uSECOND)      //HJ 5000us
#define TIMESTOP        (2000l*uSECOND)      //us

extern CONST ULONG TABSTROBE24V[32][15];  	//HJ 类似CAPD245的加热表

#define HUPR	 0x02    // handup
#define BM     	 0x04
#define PEJ	     0x08
#define HUPJ     0x10
#define PER      0x20 	 //detect paper


#elif CASE_LTPZ245 //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#define GROUPS          6             //groups for hot 分组加热组数
#define STEPS1LINE      2             //steps for feed 1 line
#define PHASEMAX        4

#define VMAXSPEED       17
#define PRNWIDTH        32              //characters can print out of 1 line

#define PHSTART        (RPH2 | RPH3)      		        // L L H H

#define TIMESTART       (2000*uSECOND)      //us
#define TIMESTOP        (2000l*uSECOND)      //us

extern CONST ULONG TABSTROBE24V[32][6];

#endif //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#if (!CASE_FTP628MCL401)

#define Drive_On(PH)  	{*(WORD *)PRT_PH_ADDR = PH; GPIOB->BRR = PHOEBIT; }
#define Drive_Off()    {*(WORD *)PRT_PH_ADDR = 0; GPIOB->BSRR = PHOEBIT;}

#else

#define RMCL     GPIO_Pin_7      //PB.7

#define Drive_On(PH)   	{*(uint8_t *)PRT_PH_ADDR = PH; GPIOB->BSRRL = RMCL; GPIOB->BSRRH = PHOEBIT; }	//{GPIOF->BSRRH = PH ^ RPHMASK;GPIOF->BSRRL = PH;}
#define Drive_Off()    	{/**(uint8_t *)PRT_PH_ADDR = 0x0;*/ GPIOB->BSRRH = RMCL; /*GPIOB->BSRRL = PHOEBIT;*/} //{GPIOF->BSRRH = RPHMASK;}

#endif

//#define TESTCUTS()    (LPC_GPIO0->FIOPIN & CUTS)
//////////// Hot definitions for printer(GPIO2) ///////////////////////////
#define HOT1_PIN     BIT0       //hot for group 1,FSMC_D0
#define HOT2_PIN     BIT1       //hot for group 2,FSMC_D1
#define HOT3_PIN     BIT2       //hot for group 3,FSMC_D2
#define HOT4_PIN     BIT3       //hot for group 4,FSMC_D3
#define HOT5_PIN     BIT4       //hot for group 5,FSMC_D4
#define HOT6_PIN     BIT5       //hot for group 6,FSMC_D5


#define HOTMASK0  (HOT1_PIN | HOT4_PIN | HOT5_PIN | HOT2_PIN | HOT3_PIN | HOT6_PIN)

#define Hot_On0(bit)  *(WORD *)PRT_DST_ADDR=bit

#define PrnHotON() {GPIOB->BRR = HOTBIT;}     //打开加热
#define PrnHotOFF() {GPIOB->BSRR = HOTBIT;}    //关闭加热

#define Latch_Data()  {GPIOB->BRR = LATCHBIT;__NOP();__NOP();__NOP();GPIOB->BSRR = LATCHBIT;}


#define START_HOT_TIMER()   {StartTimer(TIM3); PrnHotON();}  //起动加热计时器
#define Set_HotTime(t)      {TIM3->ARR = t;START_HOT_TIMER();}
#if (!CASE_FTP628MCL401) //HJ

#if (CASE_PT486F) //HJ

#define PrnPowerON() {GPIOC->BSRR = POWERBIT;} //打开电源
#define PrnPowerOFF() {GPIOC->BRR = POWERBIT;}    //关闭电源

#elif (CASE_PT723F_RJ) //HJ

#define POWER_J_BIT    (GPIO_Pin_11)      // PC.2 打印机电源开关位//
#define PAPER_J_BIT    (GPIO_Pin_8)       //paper end flag PG.6//

#define PrnPowerON() {GPIOC->BSRRL = POWERBIT; GPIOG->BSRRL = POWER_J_BIT;} //打开电源
#define PrnPowerOFF() {GPIOC->BSRRH = POWERBIT; GPIOG->BSRRH = POWER_J_BIT;}    //关闭电源
#endif

#define PRN_DS1         BIT0
#define PRN_DS2         BIT1
#define PRN_DS3         BIT2
#define PRN_DS4         BIT3
#define PRN_DS5         BIT4
#define PRN_DS6         BIT5


#define PAPERBIT    GPIO_Pin_6       //paper end flag PG.6
#define LATCHBIT    GPIO_Pin_4      // PB.4 数据锁存位
#define POWERBIT    GPIO_Pin_2      // PC.2 打印机电源开关位
#define HOTBIT      GPIO_Pin_0      // PB.0 打印机加热开关位
#define PHOEBIT    	GPIO_Pin_1      // PB.1 走纸和切刀电机控制位

//--------------------------------------------------------------------------//

#else //CASE_FTP628MCL401


#define PrnPowerON() {GPIOB->BSRRL = POWERBIT;} //打开电源
#define PrnPowerOFF() {GPIOB->BSRRH = POWERBIT;}    //关闭电源

#define PRN_DS1         0x01
#define PRN_DS2         0x02
#define PRN_DS3         0x04
#define PRN_DS4         0x08
#define PRN_DS5         0x10
#define PRN_DS6         0x20

#define	LATCHBIT	GPIO_Pin_4		// PB.4
#define POWERBIT    GPIO_Pin_6      // PB.6 打印机电源开关位

#define HOTBIT      GPIO_Pin_0      // PB.0 打印机加热开关位
#define PHOEBIT    	GPIO_Pin_1      // PB.1 走纸和切刀电机控制位

#endif

#define FEEDMAX         0x10000000L     //电机允许连续步进的步数


#if (0)//#elif CASE_CAPD247 //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#define STEPS1LINE    2             //steps for feed 1 line
#define PHASEMAX      4

#define CUTMAXSPEED (72+1)
#define VMAXSPEED   120
#define PRNWIDTH    36              //characters can print out of 1 line

#define PHSTART     ( RPH1 | PH2 )

#define CPHSTART     ( CPH1 | CPH4)   //cutter start
#define CPHSTOP      0                      //cutter stop

#define TIMESTART   (5000l*uSECOND)      //us
#define TIMESTOP    (2000l*uSECOND)      //us

#define FEEDMAX     0x10000000L     //电机允许连续步进的步数

//#elif CASE_CAPD347 //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#define GROUPS        0             //groups for hot
#define STEPS1LINE    2             //steps for feed 1 line
#define PHASEMAX      4             //phase for printe motor
#define CPHASEMAX     8             //phase for cutter motor

#define CUTMAXSPEED 73
#define VMAXSPEED   120
#define PRNWIDTH    48              //characters can print out of 1 line

#define PHSTART     (RMCL | RPH1 | RPH2)

#define CPHSTART     (CMCL | CPH1 | CPH2)   //cutter start
#define CPHSTOP      (CPH3 | CPH4)         //cutter stop

#define TIMESTART   (5000l*uSECOND)      //us
#define TIMESTOP    (2000l*uSECOND)      //us

#define FEEDMAX     0x10000000L     //电机允许连续步进的步数

extern CONST ULONG TABSTROBE24V[32][13];

//////////// Phase definitions for Cutter(GPIO1) ///////////////////////////
#define CPH1     BIT25       //Phase 1
#define CPH2     BIT26       //Phase 2
#define CPH3     BIT30       //Phase 3
#define CPH4     BIT31       //Phase 4
//......
#define CMCL     BIT25      //Drive on p3.25

#define CUTS     BIT20        //flag for cuts (GPIO0)

#define CPHMASK  (CMCL | CPH1 | CPH2 | CPH3 | CPH4)

#define CutDrive_On(PH)  {LPC_GPIO1->FIOCLR = PH ^ CPHMASK;LPC_GPIO1->FIOSET = PH;}

#define CutDrive_Off()    LPC_GPIO1->FIOCLR = RMCL

#define FULLCUTSTEPS    620
#define HALFCUTSTEPS    544

#define CUTSTEPA        (230)//*STEPS1LINE)
#define CUTSTEPB_F      (620)//*STEPS1LINE)    //step B for full cute
#define CUTSTEPB_H      (544)//*STEPS1LINE)     //step B for halif cut
#define CUTSTEPC_F      (820)//*STEPS1LINE)     //step C for full cut
#define CUTSTEPC_H      (744)//*STEPS1LINE)     //step C for half cut
#define CUTSTEPD_F      (420)//*STEPS1LINE)     //step D for full cut
#define CUTSTEPD_H      (344)//*STEPS1LINE)     //step D for ahlf cut
#define CUTSTEPE        (30 )//*STEPS1LINE)
#define CUTSTEPALPHA    (200)//*STEPS1LINE)
#endif

#if	ENCUTTER
#if	CASE_FTP628MCL401 //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#define CPH1     0x01 	// cutter motor
#define CPH2     0x02
#define CPH3     0x04
#define CPH4     0x08


#define CMCL     GPIO_Pin_8		 //PB.8

#define CUTS     0x01  	 // cutter
#define CPHMASK  (CPH1 | CPH2 | CPH3 | CPH4)

#define CutDrive_On(PH) {*(uint8_t *)PRT_PH_ADDR = PH; GPIOB->BSRR = CMCL; GPIOB->BRR = PHOEBIT; } //{LPC_GPIO1->FIOCLR = PH ^ CPHMASK;LPC_GPIO1->FIOSET = PH;}

#define CutDrive_Off()  {/**(uint8_t *)PRT_PH_ADDR = 0x0;*/ GPIOB->BRR = CMCL; /*GPIOB->BSRRL = PHOEBIT;*/} // LPC_GPIO1->FIOCLR = RMCL




#define CPHASEMAX     	8             	//phase for cutter motor
#define CUTMAXSPEED 	48				//HJ 切纸加速步数
#define CPHSTART     	(CPH1 | CPH4)   			//cutter start
#define CPHSTOP      	0               //cutter stop

#define FULLCUTSTEPS    500
#define HALFCUTSTEPS    370

//HJ 切纸各过程的步数控制
#define CUTSTEPA        (230)//*STEPS1LINE)
#define CUTSTEPB_F      (500)//*STEPS1LINE)    //step B for full cute
#define CUTSTEPB_H      (370)//*STEPS1LINE)     //step B for halif cut
#define CUTSTEPC_F      (700)//*STEPS1LINE)     //step C for full cut
#define CUTSTEPC_H      (570)//*STEPS1LINE)     //step C for half cut

#define CUTSTEPE        (40 )//*STEPS1LINE)
#define CUTSTEPALPHA    (200)//*STEPS1LINE)
#endif	//CASE_FTP628MCL401//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#endif	//ENCUTTER


#define PRNPAUSEMAX  30000           //打印机过热时,暂停30秒

#define PRNDOTS		(HEATWIDTH*12)		//HJ 加热头可打印的点数//
#define DOTSBUFF	(PRNDOTS/8)					//HJ 加热头可打印的字节数//

#define DOTS_OF_HEAT_LINE     	(HEATWIDTH*12)    //dots of 1 line
#define BYTES_OF_HEAT_LINE     	(DOTS_OF_HEAT_LINE/8)     //size of dot ApplVar.PoolForPrint

#define DOTS_OF_PAPER_LINE     	(PRNWIDTH*12)    //dots of 1 line
#define BYTES_OF_PAPER_LINE     	(DOTS_OF_PAPER_LINE/8)     //size of dot ApplVar.PoolForPrint

#define DOTS1GROUP  (PRNDOTS/GROUPS) //dots for 1 group
#define BYTES1GROUP  (DOTSBUFF/GROUPS) //dots for 1 group

//////// Attribute for print: saved in FontsOffset[]//////////////////////////////////////////
#define DBHFLAG     0x80000000      //double high print mode flag
#define DBWFLAG     0x40000000      //double width print mode flag
#define HANZFLAG    0x20000000      //hanzi flag

//-------------------------------------------------------------------
// PRNSTATUS 用来标示打印机的状态 //
typedef struct
{
    WORD g_bNOPAPER         :1;     //no paper on prnter
    WORD g_bHANDUP          :1;     //hand is put up
    WORD g_bTOOHOT          :1;     //too hot,can't print
    WORD g_bWaitPaper       :1;     //waiting for paper
    WORD g_bPaperIn         :1;     //paper is insert after paper out
    WORD g_bHANDDN          :1;      //hand is preee down
    WORD g_bSuspend         :1;     //printer motor paused for moment

    WORD g_bTextForGraph    :1;     // text print over the graphic
    WORD g_bFull            :1;
    WORD g_bPrintOver       :1;     //not any data in POOL, POOL is empty
    WORD g_bDoublHight      :1;
    WORD g_bPrintGraph      :1;     //  Print graphic mode
    WORD g_bGraphicOver     :1;     //  A task for print graphic finished

    WORD g_bHotOver         :1;     // 加热时间大于步进时间
#if (CASE_FTP628MCL401) //HJ
    WORD g_bHotLatch         :1;     //HJ只在锁存后加热,用来控制二次加热
#endif
} PRNSTATUS;

// PRNSTATUS 用来标示打印机的状态 //
typedef struct
{
    WORD g_bCutOK           :1;     //It's readey for cutter
    WORD g_bFirst           :1;     //First flag for cuttor
    WORD g_bReverse         :1;     //Move reverse
    WORD g_bStop            :1;     //Stop cut motor must
    WORD g_bStepC1st        :1;     //for task18 only

} CUTSTATUS;

/////////////////////////////////////////////////////////
//strcuture of print pool

struct TPRN_POOL
{
    BYTE Length;    //Length of the text for print
    BYTE GraphicNo; //=0,print text;>0,print a graphic and ApplVar.Prn_Command is the  lines of text for graphic
    BYTE Prn_Command;   //command for print or text lines for graphic
    BYTE data[PRNWIDTH];//text for print
};

#define SELFGRASIZE 8192    //max size of self graphic

#if defined(CASE_QRCODE)
#define GRAPHICMAX  25     //MAXIUM number of graphics can be used for
#define QRCODE_GRP_ID  25
#else
#define GRAPHICMAX  24     //MAXIUM number of graphics can be used for
#endif

typedef struct{					// 命令及其处理程序表   //
	BYTE	Command;				// 命令字符   //
    void    (*Procedure)();    	// 命令处理程序  //
} type_Command;
//////////////////////////////////////////////////////////

//#define GRA_SIZE 3204
#define TEXT_HIGH   24      //text high print
#define LINE_BLANK	  4     //blank for line

#define ASCII_SIZE	48  //24*12(24*2 bytes)
#define HZ_SIZE			72  //24*24(24*3 bytes)

#define TEXTSOFGRAPH  8             //允许叠加在图片的文本行数
#define MAX_PRNPOOL 	101         //<250 && >TEXTSOFGRAPH must

#if !defined(CASE_INNERDOTPRN)
//graphic for printe
extern CONST BYTE photo_data_eutron[];
extern CONST BYTE photo_data_caffe[];
extern CONST BYTE photo_data_abbigl[];

extern CONST BYTE photo_data_appetito[];
extern CONST BYTE photo_data_carneval[];
extern CONST BYTE photo_data_cornetto[];
extern CONST BYTE photo_data_cucina[];
extern CONST BYTE photo_data_donna[];
extern CONST BYTE photo_data_fiori[];
extern CONST BYTE photo_data_gelato[];
extern CONST BYTE photo_data_jolly[];
extern CONST BYTE photo_data_mamma[];

extern CONST BYTE photo_data_natale[];
extern CONST BYTE photo_data_pane[];
extern CONST BYTE photo_data_papa[];
extern CONST BYTE photo_data_pasqua[];
extern CONST BYTE photo_data_pesce[];
extern CONST BYTE photo_data_risparmi[];
extern CONST BYTE photo_data_salumi[];
extern CONST BYTE photo_data_selfdef[];
extern CONST BYTE photo_data_sport[];
extern CONST BYTE photo_data_vacanze[];
extern CONST BYTE photo_data_valentin[];
extern CONST BYTE photo_data_panama[];
extern CONST BYTE photo_data_MALTA[];
extern CONST BYTE photo_data_ITALIA[];

//definitions for graphic

extern CONST struct GRAPH_DEFINE graph_info_ITALIA;
extern CONST struct GRAPH_DEFINE graph_info_MALTA;
extern CONST struct GRAPH_DEFINE graph_info_panama;
extern CONST struct GRAPH_DEFINE graph_info_caffe;
extern CONST struct GRAPH_DEFINE graph_info_eutron;
extern CONST struct GRAPH_DEFINE graph_info_abbigl;
extern CONST struct GRAPH_DEFINE graph_info_appetito;
extern CONST struct GRAPH_DEFINE graph_info_carneval;
extern CONST struct GRAPH_DEFINE graph_info_cornetto;
extern CONST struct GRAPH_DEFINE graph_info_cucina;
extern CONST struct GRAPH_DEFINE graph_info_donna;
extern CONST struct GRAPH_DEFINE graph_info_fiori;
extern CONST struct GRAPH_DEFINE graph_info_gelato;
extern CONST struct GRAPH_DEFINE graph_info_jolly;
extern CONST struct GRAPH_DEFINE graph_info_mamma;

extern CONST struct GRAPH_DEFINE graph_info_natale;
extern CONST struct GRAPH_DEFINE graph_info_pane;
extern CONST struct GRAPH_DEFINE graph_info_papa;
extern CONST struct GRAPH_DEFINE graph_info_pasqua;
extern CONST struct GRAPH_DEFINE graph_info_pesce;
extern CONST struct GRAPH_DEFINE graph_info_risparmi;
extern CONST struct GRAPH_DEFINE graph_info_salumi;
extern CONST struct GRAPH_DEFINE graph_info_selfdef;
extern CONST struct GRAPH_DEFINE graph_info_sport;
extern CONST struct GRAPH_DEFINE graph_info_vacanze;
extern CONST struct GRAPH_DEFINE graph_info_valentin;

#if defined(CASE_QRCODE)
extern struct GRAPH_DEFINE graph_info_QRCODE;
extern BYTE m_byAllCodeWord[];
#endif

extern CONST BYTE Prn_command[];
extern CONST BYTE BitsInByte[];
extern CONST WORD TAB_PHASEJ[];

#if ENCUTTER
extern CONST WORD TAB_PHASECUT[];
extern CONST ULONG TAB_CUTTIME[];
#endif
extern CONST ULONG TAB_PRTIME[];


extern CONST BYTE * Graphic_Data[GRAPHICMAX];
extern CONST struct GRAPH_DEFINE * gra_info[] ;

#ifndef _PRINT_DEF_
//definitions for print

extern ULONG FontsOffset[PRNWIDTH];//offset of the characters current line
extern BYTE DotsOfCurrent[DOTSBUFF];//Used for store the dots of current line

extern BYTE Prn_PoolCurrent ;

extern struct TPRN_POOL GraphForPrint[TEXTSOFGRAPH+1];
extern struct TPRN_POOL DataFromHost;//received from host
extern BYTE Graph_TextCount ;   // Lines of the text attach for graphic.
extern struct GRAPH_DEFINE InfoSelf_Graph;
extern BYTE Self_Graph[SELFGRASIZE];

extern BYTE Feed_BlankLines ;    // Feed lines after print
extern BYTE TextIdx_ForGraph ;  //lines for graphic printed

extern WORD Graph_SizeInBytes,
    Graph_DotsLineMax,      //high of the graphic in DOTS-line
    Graph_Remain;           //

extern BYTE Graph_TextLineMax;      //high of the graphic in TEXT-line
extern int Graph_LineCurrent ;

extern PRNSTATUS Prn_Status ;

#if ENCUTTER
extern CUTSTATUS Cut_Status ;
extern WORD CutMotorMax;
#endif

extern BYTE Prn_LineIndex ;     //Dot-line index of print for current line
extern WORD Prn_BlankAppend ;//space lines after 1 line
extern ULONG Prn_MotorTime;         //speed time for motor
extern ULONG Prn_HotTime;            //hot time for Thermal
extern WORD Prn_VNTCR;
extern SCHAR Prn_TaskNow ;  //task num current
extern SCHAR Prn_TaskNext;  //task num after feed
extern BYTE Prn_Jphase ;
extern WORD Prn_AccStep,Prn_AccCopy  ;

extern BYTE Prn_GraphTextMax;      //high of the graphic in TEXT-line
extern BYTE Prn_LineChars;         // chars for current line
extern WORD Prn_LineDots;          // dots for current line
#if GROUPMODE==2
extern BYTE Prn_GroupHot[GROUPS] ;      //需要加热的组,每一位代表一组.每一组加热两次
extern WORD Prn_GroupDots[GROUPS];        //各组加热点数统计
extern BYTE Prn_HotMAX ;                 //需要加热次数
#endif

extern ULONG Prn_FeedCount;       //统计电机连续走动步数
extern WORD Prn_Delay;          //打印机暂停时长
extern BYTE Prn_HotIndex;//index of hot of current
#if defined(CASE_FORHANZI)
extern ULONG Prn_HZTime;         // times for read hanzi
#endif

extern BYTE * pGraBuf;
extern unsigned int Graph_Offset;
extern unsigned int Graph_Width;
extern unsigned int Graph_PosX;
extern BYTE * pGraBuf;


extern CONST WORD DoubleByte[];

#endif
/////////////////////////////////////////////////////////////////
extern void Create384Dots(void);
extern void CreatGraphicWithText(void);
extern void CreateGraphicDots(void);
extern void GetTaskFromPool(void);
extern void Check_Print(void);
extern void Start_When_Ready(const char *);
extern PRNSTATUS Get_Printer_status(void);

int RSPrint(BYTE *buf, int count, BYTE prnCMD);
void Reset_printer(void);

void Stop_Motor(void);
void Start_Motor(BYTE prnCMD);
void Restart_Motor(void);
void Suspend_Motor(void);

#if ENCUTTER
void  Drive_CutMotor(WORD phase);
#endif

#define CWORD(a) (*(WORD*)&a)

#define CLONG(a) (*(ULONG*)&a)

/////////////////////////////////////////////////////////////////////////////
/* 打印机温度检测端口:ADC1-10 */
#define ADC1_DR_ADDRESS    ((uint32_t)0x4001244C)	//stm32f205:((uint32_t)0x4001204C)


/* 打印机数据接口:PCLK-PB3,PDATA-PB5, PLATCH-PB4  */
#define PRNData_SPI                           SPI3
#define PRNData_SPI_CLK                       RCC_APB1Periph_SPI3
#define PRNData_SPI_CLK_INIT                  RCC_APB1PeriphClockCmd
/* PDATA-PB5 */
#define PRNData_SPI_MOSI_PIN                  GPIO_Pin_5
#define PRNData_SPI_MOSI_GPIO_PORT            GPIOB
#define PRNData_SPI_MOSI_GPIO_CLK             RCC_APB2Periph_GPIOB
#define PRNData_SPI_MOSI_SOURCE               GPIO_PinSource5
#define PRNData_SPI_MOSI_AF                   GPIO_AF_SPI3
/* PCLK-PB3 */
#define PRNData_SPI_SCK_PIN                   GPIO_Pin_3
#define PRNData_SPI_SCK_GPIO_PORT             GPIOB
#define PRNData_SPI_SCK_GPIO_CLK              RCC_APB2Periph_GPIOB
#define PRNData_SPI_SCK_SOURCE                GPIO_PinSource3
#define PRNData_SPI_SCK_AF                    GPIO_AF_SPI3

/* PLATCH-PB4 */
#define PRNData_LATCH_PIN                     GPIO_Pin_4
#define PRNData_LATCH_GPIO_PORT               GPIOB
#define PRNData_LATCH_GPIO_CLK                RCC_APB2Periph_GPIOB



/* Exported macro ------------------------------------------------------------*/
#define PRNData_LATCH_LOW()       GPIO_ResetBits(PRNData_LATCH_GPIO_PORT, PRNData_LATCH_PIN)
#define PRNData_LATCH_HIGH()      GPIO_SetBits(PRNData_LATCH_GPIO_PORT, PRNData_LATCH_PIN)

/* Exported functions ------------------------------------------------------- */


#if DEBUGONPC

void pcStop_Motor();
void pcStart_Motor();

void    Set_Hot_Data(BYTE,BYTE);
    #define udelay(a)    {}
void    Drive_Motor(WORD);
void Set_Motor_Speed(ULONG);
void Start_Hot();
void Stop_Motor();
    #define Latch_Data()    {}
    #define disLatch_Data()    {}


#else//DEBUGONPC

#if (WORDorBYTE)
void    Set_Hot_DataW(WORD dot,BYTE pos);
#else
void    Set_Hot_Data(BYTE dot,BYTE pos);
#endif
// Drive printer motor by one step:GPIO1
void  Drive_Motor(WORD phase);
#if GROUPMODE==2
void  Set_Hot_Group(void);
#endif
// Set the speed of motor: RIT
void Set_Motor_Speed(ULONG speed);
// Set the hot time
void Start_Hot(void);
void Set_HotPIN(void);
//Stop print motor

// delay moment if needed
#define udelay(a)    {}

#endif//DEBUGONPC

extern PRNSTATUS Printer_Status;
extern BYTE  TestPrintGoingOn(void);

extern BYTE PowerFlags;		//检测到的电源状态
/*
    Bit0:=0,为适配器供电;=1,为电池供电
    Bit1:=1,为电池供电时,出现报警PWR_WARNING1(电压次低)
    Bit2:=1,为电池供电时,出现报警PWR_WARNING2(电压过低)

    Bit4:=1,纽扣电池出现报警PWR_WARNING1(电压次低)
    Bit5:=1,纽扣电池出现报警PWR_WARNING2(电压过低)

*/
extern BYTE pwrGetStatus(BYTE GetStatus);

extern struct GRB_DEFINE GRB_Define;
#endif
#endif /* __PRINT_H */

