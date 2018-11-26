/****************************************Copyright (c)**************************************************
*        EUTRON Human Information Equipment Co.,Ltd.
*                    www.eutron.com.cn
* File name:		Print_def.c
* Descriptions:     definitions for printe
*------------------------------------------------------------------------------------------------------
*  Created by:			chen churong
* Created date:        2011-10-01
* Version: 1.0
********************************************************************************************************/
#define _PRINT_DEF_
#include "bios_dir.h"
#include "print.h"

#if !defined(CASE_INNERDOTPRN)
ULONG FontsOffset[PRNWIDTH];//offset of the characters current line

BYTE DotsOfCurrent[DOTSBUFF];//Used for store the dots of current line

BYTE Prn_PoolCurrent = 0;

struct TPRN_POOL GraphForPrint[TEXTSOFGRAPH+1];//保存叠加在图片上的文字,每个图片的高度不应该超过TEXTSOFGRAPH行
struct TPRN_POOL DataFromHost;//received from host


BYTE Graph_TextCount = 0;   // Lines of the text attach for graphic.

struct GRAPH_DEFINE InfoSelf_Graph;//用户自定义图片
BYTE Self_Graph[SELFGRASIZE];//用户自定义图片

BYTE Feed_BlankLines = 0;    // Feed lines after print
BYTE TextIdx_ForGraph = 0;  //lines for graphic printed

WORD Graph_SizeInBytes,
    Graph_DotsLineMax,      //high of the graphic in DOTS-line
    Graph_Remain;           //
BYTE Graph_TextLineMax;      //high of the graphic in TEXT-line
int Graph_LineCurrent = 0;

/////////////////////////////////////////////////////////////////
PRNSTATUS Prn_Status;   //Status of current for Inter
PRNSTATUS Printer_Status;

#if ENCUTTER
CUTSTATUS Cut_Status;
WORD CutMotorMax;       // MAX steps for cutter mootor
#endif

BYTE Prn_LineIndex = 0;     //Dot-line index of print for current line
WORD Prn_BlankAppend = LINE_BLANK*STEPS1LINE;//space lines after 1 line
ULONG Prn_MotorTime;         //speed time for motor
ULONG Prn_HotTime=(300)*uSECOND;            //hot time for Thermal
WORD Prn_VNTCR;
SCHAR Prn_TaskNow = 0xff;  //task num current

SCHAR Prn_TaskNext;  //task num after feed


BYTE Prn_Jphase = 0;
WORD Prn_AccStep = 0,Prn_AccCopy=0;
BYTE Prn_GraphTextMax;      //为需要叠加到图片上的文字
BYTE Prn_LineChars=0;         // chars for current line
WORD Prn_LineDots=0;          // dots for current line

#if GROUPMODE==2
BYTE Prn_GroupHot[GROUPS] ;      //需要加热的组,每一位代表一组.每一组加热两次
WORD Prn_GroupDots[GROUPS];        //各组加热点数统计
BYTE Prn_HotMAX ;                 //需要加热次数
#endif

ULONG Prn_FeedCount=0;       //统计电机连续走动步数
WORD Prn_Delay=0;          //打印机暂停时长
BYTE Prn_HotIndex=0;//index of hot of current
#if defined(CASE_FORHANZI)
ULONG Prn_HZTime;         // times for read hanzi
#endif

///////////////////////////////////////////////////////////////////////
////////// Graphic list /////////////////////////////////////////////////
CONST BYTE * Graphic_Data[GRAPHICMAX]={
    photo_data_eutron,

    photo_data_caffe,

    photo_data_abbigl,

    photo_data_appetito,
    photo_data_carneval,
    photo_data_cornetto,

    photo_data_cucina,
    photo_data_donna,
    photo_data_fiori,
    photo_data_gelato,

    photo_data_jolly,
#if defined(CASE_MALTA)
    photo_data_MALTA,
#else
    photo_data_mamma,
#endif
    photo_data_natale,
    photo_data_pane,

    photo_data_papa,
    photo_data_pasqua,
    photo_data_pesce,
    photo_data_risparmi,

    photo_data_salumi,
    photo_data_selfdef,
    photo_data_sport,
    photo_data_vacanze,

    photo_data_valentin,
#if defined(CASE_ITALIA)
    photo_data_ITALIA,
#elif defined(CASE_MALTA)
    photo_data_MALTA,
#else
    photo_data_panama,
#endif
#if defined(CASE_QRCODE)
    m_byAllCodeWord,
#endif
};
// Graphic describe messgaes
CONST struct GRAPH_DEFINE * gra_info[GRAPHICMAX]={
    &graph_info_eutron,

    &graph_info_caffe,

    &graph_info_abbigl,

    &graph_info_appetito,
    &graph_info_carneval,
    &graph_info_cornetto,

    &graph_info_cucina,
    &graph_info_donna,
    &graph_info_fiori,
    &graph_info_gelato,

    &graph_info_jolly,
    &graph_info_mamma,
    &graph_info_natale,
    &graph_info_pane,

    &graph_info_papa,
    &graph_info_pasqua,
    &graph_info_pesce,
    &graph_info_risparmi,

    &graph_info_salumi,
    &graph_info_selfdef,
    &graph_info_sport,
    &graph_info_vacanze,

    &graph_info_valentin,
#if CASE_ITALIA
    &graph_info_ITALIA,
#elif defined(CASE_MALTA)
    &graph_info_MALTA,
#else
    &graph_info_panama,
#endif
#if defined(CASE_QRCODE)
    &graph_info_QRCODE,
#endif
};

BYTE * pGraBuf;
// struct GRAPH_DEFINE * pGraInfo;
unsigned int Graph_Offset;
unsigned int Graph_Width;
unsigned int Graph_PosX;

/****************************************************************************
// Double scale for byte data
*****************************************************************************/
CONST WORD DoubleByte[]={
    0x0000,0x0003,0x000C,0x000F,0x0030,0x0033,0x003C,0x003F,
    0x00C0,0x00C3,0x00CC,0x00CF,0x00F0,0x00F3,0x00FC,0x00FF,
    0x0300,0x0303,0x030C,0x030F,0x0330,0x0333,0x033C,0x033F,
    0x03C0,0x03C3,0x03CC,0x03CF,0x03F0,0x03F3,0x03FC,0x03FF,
    0x0C00,0x0C03,0x0C0C,0x0C0F,0x0C30,0x0C33,0x0C3C,0x0C3F,
    0x0CC0,0x0CC3,0x0CCC,0x0CCF,0x0CF0,0x0CF3,0x0CFC,0x0CFF,
    0x0F00,0x0F03,0x0F0C,0x0F0F,0x0F30,0x0F33,0x0F3C,0x0F3F,
    0x0FC0,0x0FC3,0x0FCC,0x0FCF,0x0FF0,0x0FF3,0x0FFC,0x0FFF,
    0x3000,0x3003,0x300C,0x300F,0x3030,0x3033,0x303C,0x303F,
    0x30C0,0x30C3,0x30CC,0x30CF,0x30F0,0x30F3,0x30FC,0x30FF,
    0x3300,0x3303,0x330C,0x330F,0x3330,0x3333,0x333C,0x333F,
    0x33C0,0x33C3,0x33CC,0x33CF,0x33F0,0x33F3,0x33FC,0x33FF,
    0x3C00,0x3C03,0x3C0C,0x3C0F,0x3C30,0x3C33,0x3C3C,0x3C3F,
    0x3CC0,0x3CC3,0x3CCC,0x3CCF,0x3CF0,0x3CF3,0x3CFC,0x3CFF,
    0x3F00,0x3F03,0x3F0C,0x3F0F,0x3F30,0x3F33,0x3F3C,0x3F3F,
    0x3FC0,0x3FC3,0x3FCC,0x3FCF,0x3FF0,0x3FF3,0x3FFC,0x3FFF,
    0xC000,0xC003,0xC00C,0xC00F,0xC030,0xC033,0xC03C,0xC03F,
    0xC0C0,0xC0C3,0xC0CC,0xC0CF,0xC0F0,0xC0F3,0xC0FC,0xC0FF,
    0xC300,0xC303,0xC30C,0xC30F,0xC330,0xC333,0xC33C,0xC33F,
    0xC3C0,0xC3C3,0xC3CC,0xC3CF,0xC3F0,0xC3F3,0xC3FC,0xC3FF,
    0xCC00,0xCC03,0xCC0C,0xCC0F,0xCC30,0xCC33,0xCC3C,0xCC3F,
    0xCCC0,0xCCC3,0xCCCC,0xCCCF,0xCCF0,0xCCF3,0xCCFC,0xCCFF,
    0xCF00,0xCF03,0xCF0C,0xCF0F,0xCF30,0xCF33,0xCF3C,0xCF3F,
    0xCFC0,0xCFC3,0xCFCC,0xCFCF,0xCFF0,0xCFF3,0xCFFC,0xCFFF,
    0xF000,0xF003,0xF00C,0xF00F,0xF030,0xF033,0xF03C,0xF03F,
    0xF0C0,0xF0C3,0xF0CC,0xF0CF,0xF0F0,0xF0F3,0xF0FC,0xF0FF,
    0xF300,0xF303,0xF30C,0xF30F,0xF330,0xF333,0xF33C,0xF33F,
    0xF3C0,0xF3C3,0xF3CC,0xF3CF,0xF3F0,0xF3F3,0xF3FC,0xF3FF,
    0xFC00,0xFC03,0xFC0C,0xFC0F,0xFC30,0xFC33,0xFC3C,0xFC3F,
    0xFCC0,0xFCC3,0xFCCC,0xFCCF,0xFCF0,0xFCF3,0xFCFC,0xFCFF,
    0xFF00,0xFF03,0xFF0C,0xFF0F,0xFF30,0xFF33,0xFF3C,0xFF3F,
    0xFFC0,0xFFC3,0xFFCC,0xFFCF,0xFFF0,0xFFF3,0xFFFC,0xFFFF,
};
// command list for print //
#if (0)
CONST BYTE Prn_command[]={

    PRN_RECEIPT_Mode,                                    // PRNCMD_rTEXT        1   //print text on receipt
    PRN_STUB_Mode,                                       // PRNCMD_sTEXT        2   //print text on stub
    PRN_RECEIPT_Mode + PRN_STUB_Mode,                         // PRNCMD_rsTEXT       3   //print text on Receipt & stub

    PRN_DBLW_Mode + PRN_RECEIPT_Mode,                         // PRNCMD_rTEXT_DW     4   //print text on receipt double with mode
    PRN_DBLW_Mode + PRN_STUB_Mode,                            // PRNCMD_sTEXT_DW     5   //print text on stub double with mode
    PRN_DBLW_Mode + PRN_RECEIPT_Mode + PRN_STUB_Mode,              // PRNCMD_rsTEXT_DW    6   //print text on Receipt & stub double with mode

    PRN_DBLH_Mode + PRN_RECEIPT_Mode,                         // PRNCMD_rTEXT_DH     7   //print text on receipt double high mode
    PRN_DBLH_Mode + PRN_STUB_Mode,                            // PRNCMD_sTEXT_DH     8   //print text on stub double high mode
    PRN_DBLH_Mode + PRN_RECEIPT_Mode + PRN_STUB_Mode,              // PRNCMD_rsTEXT_DH    9   //print text on Receipt & stub double high mode

    PRN_DBLH_Mode + PRN_DBLW_Mode + PRN_RECEIPT_Mode,              // RNCMD_rTEXT_DWH    10   //print text on receipt double high & double width mode
    PRN_DBLH_Mode + PRN_DBLW_Mode + PRN_STUB_Mode,                 // RNCMD_sTEXT_DWH    11   //print text on stub double high & double width mode
    PRN_DBLH_Mode + PRN_DBLW_Mode + PRN_RECEIPT_Mode + PRN_STUB_Mode,   // RNCMD_rsTEXT_DWH   12   //print text on Receipt & stub double high & double width mode
    0,                                              // NULL for command   13
    PRN_RECEIPT_Mode + PRN_GRAPHIC_Mode,                      // PRNCMD_rGRA         14   //Print Graphic on receipt
    PRN_STUB_Mode + PRN_GRAPHIC_Mode,                         // PRNCMD_sGRA         15   //Print Graphic on stub
    PRN_RECEIPT_Mode + PRN_STUB_Mode + PRN_GRAPHIC_Mode,           // PRNCMD_RSGRA        16   //Print Graphic on receipt & stub

    PRN_RECEIPT_Mode + PRN_CUTF_Mode,                         // PRNCMD_rCUT         17   //cut receipt paper
    PRN_STUB_Mode + PRN_CUTF_Mode,                            // PRNCMD_sCUT         18   //cut stub paper
    PRN_RECEIPT_Mode + PRN_STUB_Mode + PRN_CUTF_Mode,              // PRNCMD_rsCUT        19   //cut receipt & stub paper
    PRN_RECEIPT_Mode + PRN_CUTRESET_Mode ,                    // PRNCMD_rCUTRESET    20   //cut receipt & stub paper
    PRN_STUB_Mode + PRN_CUTRESET_Mode ,                       // PRNCMD_sCUTRESET    21   //cut receipt & stub paper
};
#endif
// count of bit for a byte
CONST BYTE BitsInByte[]={
		0	,//  0 00000000
		1	,//  1 00000001
		1	,//  2 00000010
		2	,//  3 00000011
		1	,//  4 00000100
		2	,//  5 00000101
		2	,//  6 00000110
		3	,//  7 00000111
		1	,//  8 00001000
		2	,//  9 00001001
		2	,// 10 00001010
		3	,// 11 00001011
		2	,// 12 00001100
		3	,// 13 00001101
		3	,// 14 00001110
		4	,// 15 00001111

		1	,// 16 00010000
		2	,// 17 00010001
		2	,// 18 00010010
		3	,// 19 00010011
		2	,// 20 00010100
		3	,// 21 00010101
		3	,// 22 00010110
		4	,// 23 00010111
		2	,// 24 00011000
		3	,// 25 00011001
		3	,// 26 00011010
		4	,// 27 00011011
		3	,// 28 00011100
		4	,// 29 00011101
		4	,// 30 00011110
		5	,// 31 00011111

		1	,// 32 00100000
		2	,//    00100001
		2	,//    00100010
		3	,//    00100011
		2	,//    00100100
		3	,//    00100101
		3	,//    00100110
		4	,//    00100111
		2	,//    00101000
		3	,//    00101001
		3	,//    00101010
		4	,//    00101011
		3	,//    00101100
		4	,//    00101101
		4	,//    00101110
		5	,// 47 00101111

		2	,// 48 00110000
		3	,//    00110001
		3	,//    00110010
		4	,//    00110011
		3	,//    00110100
		4	,//    00110101
		4	,//    00110110
		5	,//    00110111
		3	,//    00111000
		4	,//    00111001
		4	,//    00111010
		5	,//    00111011
		4	,//    00111100
		5	,//    00111101
		5	,//    00111110
		6	,// 63 00111111

		1	,// 64 01000000
		2	,//    01000001
		2	,//    01000010
		3	,//    01000011
		2	,//    01000100
		3	,//    01000101
		3	,//    01000110
		4	,//    01000111
		2	,//    01001000
		3	,//    01001001
		3	,//    01001010
		4	,//    01001011
		3	,//    01001100
		4	,//    01001101
		4	,//    01001110
		5	,// 79 01001111

		2	,// 80 01010000
		3	,//    01010001
		3	,//    01010010
		4	,//    01010011
		3	,//    01010100
		4	,//    01010101
		4	,//    01010110
		5	,//    01010111
		3	,//    01011000
		4	,//    01011001
		4	,//    01011010
		5	,//    01011011
		4	,//    01011100
		5	,//    01011101
		5	,//    01011110
		6	,// 95 01011111

		2	,// 96 01100000
		3	,//    01100001
		3	,//    01100010
		4	,//    01100011
		3	,//    01100100
		4	,//    01100101
		4	,//    01100110
		5	,//    01100111
		3	,//    01101000
		4	,//    01101001
		4	,//    01101010
		5	,//    01101011
		4	,//    01101100
		5	,//    01101101
		5	,//    01101110
		6	,//111 01101111

		3	,//112 01110000
		4	,//    01110001
		4	,//    01110010
		5	,//    01110011
		4	,//    01110100
		5	,//    01110101
		5	,//    01110110
		6	,//    01110111
		4	,//    01111000
		5	,//    01111001
		5	,//    01111010
		6	,//    01111011
		5	,//    01111100
		6	,//    01111101
		6	,//    01111110
		7	,//127 01111111

		1	,//128 10000000
		2	,//    10000001
		2	,//    10000010
		3	,//    10000011
		2	,//    10000100
		3	,//    10000101
		3	,//    10000110
		4	,//    10000111
		2	,//    10001000
		3	,//    10001001
		3	,//    10001010
		4	,//    10001011
		3	,//    10001100
		4	,//    10001101
		4	,//    10001110
		5	,//143 10001111

		2	,//144 10010000
		3	,//    10010001
		3	,//    10010010
		4	,//    10010011
		3	,//    10010100
		4	,//    10010101
		4	,//    10010110
		5	,//    10010111
		3	,//    10011000
		4	,//    10011001
		4	,//    10011010
		5	,//    10011011
		4	,//    10011100
		5	,//    10011101
		5	,//    10011110
		6	,//159 10011111

		2	,//160 10100000
		3	,//    10100001
		3	,//    10100010
		4	,//    10100011
		3	,//    10100100
		4	,//    10100101
		4	,//    10100110
		5	,//    10100111
		3	,//    10101000
		4	,//    10101001
		4	,//    10101010
		5	,//    10101011
		4	,//    10101100
		5	,//    10101101
		5	,//    10101110
		6	,//175 10101111

		3	,//176 10110000
		4	,//    10110001
		4	,//    10110010
		5	,//    10110011
		4	,//    10110100
		5	,//    10110101
		5	,//    10110110
		6	,//    10110111
		4	,//    10111000
		5	,//    10111001
		5	,//    10111010
		6	,//    10111011
		5	,//    10111100
		6	,//    10111101
		6	,//    10111110
		7	,//191 10111111

		2	,//192 11000000
		3	,//    11000001
		3	,//    11000010
		4	,//    11000011
		3	,//    11000100
		4	,//    11000101
		4	,//    11000110
		5	,//    11000111
		3	,//    11001000
		4	,//    11001001
		4	,//    11001010
		5	,//    11001011
		4	,//    11001100
		5	,//    11001101
		5	,//    11001110
		6	,//207 11001111

		3	,//208 11010000
		4	,//    11010001
		4	,//    11010010
		5	,//    11010011
		4	,//    11010100
		5	,//    11010101
		5	,//    11010110
		6	,//    11010111
		4	,//    11011000
		5	,//    11011001
		5	,//    11011010
		6	,//    11011011
		5	,//    11011100
		6	,//    11011101
		6	,//    11011110
		7	,//223 11011111

		3	,//224 11100000
		4	,//    11100001
		4	,//    11100010
		5	,//    11100011
		4	,//    11100100
		5	,//    11100101
		5	,//    11100110
		6	,//    11100111
		4	,//    11101000
		5	,//    11101001
		5	,//    11101010
		6	,//    11101011
		5	,//    11101100
		6	,//    11101101
		6	,//    11101110
		7	,//239 11101111

		4	,//240 11110000
		5	,//    11110001
		5	,//    11110010
		6	,//    11110011
		5	,//    11110100
		6	,//    11110101
		6	,//    11110110
		7	,//    11110111
		5	,//    11111000
		6	,//    11111001
		6	,//    11111010
		7	,//    11111011
		6	,//    11111100
		7	,//    11111101
		7	,//    11111110
		8	//255 11111111
};
//// Attributes for printer //////////////////////////////////////////////////////////
#if CASE_LTP1245 //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

CONST WORD TAB_PHASEJ[PHASEMAX] = {0x60,0x50,0x90,0xA0};
// BYTE TAB_PHASEJ[] = {0x60,0xC0,0x90,0x30};
CONST ULONG TAB_PRTIME[VMAXSPEED] = {
//    5780*1,
    5780*1,
    3571*1,
    2762*1,
    2314*1,
    2028*1,
    1828*1,
    1675*1,
    1553*1,
    1456*1,
    1374*1,
    1302*1,
    1242*1,
    1191*1,
    1144*1,
    1103*1,
    1065*1,
    1031*1,
    1000*1
};

CONST WORD TABSTROBE24V[] = {
    (148-50)*1,
    (148-50)*1,
    (148-50)*1,
    (220-50)*1,
    (334-50)*1,

    (427-100)*1,
    (500-100)*1,
    (566-100)*1,
    (613-100)*1,
    (639-100)*1,

    (706-100)*1,
    (729-100)*1,
    (752-100)*1,
    (799-100)*1,
    (822-100)*1,

    (845-100)*1,
    (892-100)*1,
    (915-100)*1,
    (938-100)*1,
    (985-100)*1,

    (1000-100)*1,
    (1000-100)*1,
    (1000-100)*1,
    (1000-100)*1,

    (1000-100)*1,
    (1000-100)*1,
    (1000-100)*1,
    (1000-100)*1,

    (1000-100)*1,
    (1000-100)*1,
    (1000-100)*1,
    (1000-100)*1
};

#elif CASE_LTPZ245  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

CONST WORD TAB_PHASEJ[PHASEMAX] = { //    RPH1 RPH2 RPH3 RPH4

    RPH2 | RPH4,			        //Step1 L   H   L   H   .BYTE	01010000B
    RPH1 | RPH4,      		        //Step2 H   L   L   H   .BYTE	10010000B
    RPH1 | RPH3,			        //Step3 H   L   H   L   .BYTE	10100000B
    RPH2 | RPH3,      		        //Step4 L   H   H   L   .BYTE	01100000B

};
// BYTE TAB_PHASEJ[] = {0x60,0xC0,0x90,0x30};
CONST ULONG TAB_PRTIME[VMAXSPEED] = {
//    1852*uSECOND,//start Speed
    1852*uSECOND,//1     540
    1653*uSECOND,//2     605
    1508*uSECOND,//3     663
    1399*uSECOND,//4     715
    1309*uSECOND,//5     764
    1235*uSECOND,//6     810
    1172*uSECOND,//7     853
    1119*uSECOND,//8     894
    1072*uSECOND,//9     933
    1030*uSECOND,//10    971
    993 *uSECOND,//11    1007
    960 *uSECOND,//12    1042
    930 *uSECOND,//13    1075
    903 *uSECOND,//14    1108
    878 *uSECOND,//15    1139
    855 *uSECOND,//16    1170
    833 *uSECOND,//17    1200
};

CONST ULONG TABSTROBE24V[32][6] = {
//[pps] 500	    600	            700	            800	            900	            1000
uSECOND*95	,uSECOND*92 	,uSECOND*89 	,uSECOND*87		,uSECOND*85 	,uSECOND*83  	,//57  //100	,426 /128=0*
uSECOND*95	,uSECOND*92 	,uSECOND*89 	,uSECOND*87		,uSECOND*85 	,uSECOND*83  	,//57  //100	,426 /128=1*
uSECOND*95	,uSECOND*92 	,uSECOND*89 	,uSECOND*87		,uSECOND*85 	,uSECOND*83  	,//57  //100	,426 /128=2*
uSECOND*124	,uSECOND*121	,uSECOND*117	,uSECOND*114	,uSECOND*111	,uSECOND*109  	,//75  //95	,484 /128=3*
uSECOND*154	,uSECOND*149	,uSECOND*145	,uSECOND*142	,uSECOND*138	,uSECOND*135  	,//93  //90	,551 /128=4*
uSECOND*184	,uSECOND*178	,uSECOND*173	,uSECOND*169	,uSECOND*165	,uSECOND*161 	,//111 //85	,627 /128=5
uSECOND*214	,uSECOND*207	,uSECOND*201	,uSECOND*196	,uSECOND*191	,uSECOND*189 	,//130 //80	,715 /128=6
uSECOND*244	,uSECOND*236	,uSECOND*229	,uSECOND*223	,uSECOND*218	,uSECOND*215 	,//148 //75	,815 /128=7
uSECOND*273	,uSECOND*265	,uSECOND*257	,uSECOND*251	,uSECOND*245	,uSECOND*241 	,//166 //70	,928 /128=8
uSECOND*303	,uSECOND*294	,uSECOND*285	,uSECOND*278	,uSECOND*271	,uSECOND*267 	,//184 //65	,1056/128=9
uSECOND*333	,uSECOND*323	,uSECOND*313	,uSECOND*305	,uSECOND*298	,uSECOND*293 	,//202 //60	,1199/128=10
uSECOND*363	,uSECOND*351	,uSECOND*342	,uSECOND*333	,uSECOND*325	,uSECOND*319 	,//220 //55	,1358/128=11
uSECOND*392	,uSECOND*380  	,uSECOND*370  	,uSECOND*360  	,uSECOND*351	,uSECOND*345 	,//238 //50	,1532/128=12
uSECOND*410	,uSECOND*390  	,uSECOND*380  	,uSECOND*370  	,uSECOND*361	,uSECOND*360 	,//248 //50	,1532/128=13*
uSECOND*422	,uSECOND*409	,uSECOND*398	,uSECOND*387	,uSECOND*378	,uSECOND*371 	,//256 //45	,1721/128=14
uSECOND*442	,uSECOND*419	,uSECOND*418	,uSECOND*400	,uSECOND*388	,uSECOND*386 	,//266 //45	,1721/128=15*
uSECOND*452	,uSECOND*438	,uSECOND*426	,uSECOND*415	,uSECOND*405	,uSECOND*397 	,//274 //40	,1921/128=16
uSECOND*482	,uSECOND*467	,uSECOND*454	,uSECOND*442	,uSECOND*431	,uSECOND*423 	,//292 //35	,2131/128=17
uSECOND*482	,uSECOND*467	,uSECOND*454	,uSECOND*442	,uSECOND*431	,uSECOND*423 	,//292 //35	,2131/128=18
uSECOND*511	,uSECOND*496	,uSECOND*482	,uSECOND*469	,uSECOND*458	,uSECOND*450 	,//310 //30	,2345/128=19
uSECOND*511	,uSECOND*496	,uSECOND*482	,uSECOND*469	,uSECOND*458	,uSECOND*450 	,//310 //30	,2345/128=20
uSECOND*541	,uSECOND*525	,uSECOND*510  	,uSECOND*497	,uSECOND*485	,uSECOND*476 	,//328 //25	,2561/128=21
uSECOND*571	,uSECOND*553	,uSECOND*538	,uSECOND*524	,uSECOND*511	,uSECOND*502 	,//346 //20	,2771/128=22
uSECOND*611	,uSECOND*563	,uSECOND*548	,uSECOND*534	,uSECOND*521	,uSECOND*516 	,//356 //20	,2771/128=23*
uSECOND*601	,uSECOND*582	,uSECOND*566	,uSECOND*551	,uSECOND*538	,uSECOND*528 	,//364 //15	,2971/128=24
uSECOND*630 ,uSECOND*611	,uSECOND*594	,uSECOND*578	,uSECOND*565	,uSECOND*554 	,//382 //10	,3158/128=25
uSECOND*660 ,uSECOND*640  	,uSECOND*622	,uSECOND*606	,uSECOND*591	,uSECOND*580 	,//400 //5	,3327/128=26
uSECOND*675 ,uSECOND*650  	,uSECOND*632	,uSECOND*606	,uSECOND*591	,uSECOND*580 	,//400 //5	,3327/128=27*
uSECOND*690 ,uSECOND*669	,uSECOND*650  	,uSECOND*633	,uSECOND*618	,uSECOND*606 	,//418 //0	,3477/128=28
uSECOND*690 ,uSECOND*669	,uSECOND*650  	,uSECOND*633	,uSECOND*618	,uSECOND*606 	,//418 //0	,3477/128=29*
uSECOND*690 ,uSECOND*669	,uSECOND*650  	,uSECOND*633	,uSECOND*618	,uSECOND*606 	,//418 //0	,3477/128=30*
uSECOND*690 ,uSECOND*669	,uSECOND*650  	,uSECOND*633	,uSECOND*618	,uSECOND*606	,//418 //0	,3477/128=31*

};
#elif (CASE_PT486F | CASE_PT723F_RJ) //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#if 0 //CCR 2-2相模式

CONST WORD TAB_PHASEJ[PHASEMAX] = { //    RPH1 RPH2 RPH3 RPH4

    RPH2 | RPH4,			        //Step1 L   H   L   H   .BYTE	01010000B
    RPH1 | RPH4,      		        //Step2 H   L   L   H   .BYTE	10010000B
    RPH1 | RPH3,			        //Step3 H   L   H   L   .BYTE	10100000B
    RPH2 | RPH3,      		        //Step4 L   H   H   L   .BYTE	01100000B

};
#else //HJ 1-2相模式

CONST WORD TAB_PHASEJ[8] = { //    RPH1 RPH2 RPH3 RPH4

    RPH2 | RPH4,			        //Step1 L   H   L   H   .BYTE	01010000B
	       RPH4,
    RPH1 | RPH4,      		        //Step2 H   L   L   H   .BYTE	10010000B
	RPH1,
    RPH1 | RPH3,			        //Step3 H   L   H   L   .BYTE	10100000B
		   RPH3,
    RPH2 | RPH3,      		        //Step4 L   H   H   L   .BYTE	01100000B
	RPH2,
};
#endif

#if 0 //CCR
// BYTE TAB_PHASEJ[] = {0x60,0xC0,0x90,0x30};
CONST ULONG TAB_PRTIME[VMAXSPEED] = {
#if (1)
    2890*uSECOND,      //1
    1786*uSECOND,      //2
    1381*uSECOND,      //3
    1157*uSECOND,      //4
                       //
    1157*uSECOND,      //4
    1157*uSECOND,      //4
    1157*uSECOND,      //4
    1157*uSECOND,      //4
    1157*uSECOND,      //4
    1157*uSECOND,      //4
    1157*uSECOND,      //4
    1157*uSECOND,      //4
    1157*uSECOND,      //4
    1157*uSECOND,      //4
    1157*uSECOND,      //4
    1157*uSECOND,      //4
    1157*uSECOND,      //4

//为保证多组加热,降低走纸速度,确保加热时长
//    1014*uSECOND,    //5
//    914 *uSECOND,    //6
//    838 *uSECOND,    //7
//    777 *uSECOND,    //8
//    728 *uSECOND,    //9
//    687 *uSECOND,    //10
//    651 *uSECOND,    //11
//    621 *uSECOND,    //12
//    600 *uSECOND,    //13
//    572 *uSECOND,    //14
//    552 *uSECOND,    //15
//    533 *uSECOND,    //16
//    516 *uSECOND,    //17
//    500 *uSECOND,    //18

#else
//    1852*uSECOND,//start Speed
    1852*uSECOND,//1     540
    1653*uSECOND,//2     605
    1508*uSECOND,//3     663
    1399*uSECOND,//4     715
    1309*uSECOND,//5     764
    1235*uSECOND,//6     810
    1172*uSECOND,//7     853
    1119*uSECOND,//8     894
    1072*uSECOND,//9     933
    1030*uSECOND,//10    971

    1030*uSECOND,//10    971
    1030*uSECOND,//10    971
    1030*uSECOND,//10    971
    1030*uSECOND,//10    971
    1030*uSECOND,//10    971
    1030*uSECOND,//10    971
    1030*uSECOND,//10    971
//为保证多组加热,降低走纸速度,确保加热时长
//testonly    993 *uSECOND,//11    1007
//testonly    960 *uSECOND,//12    1042
//testonly    930 *uSECOND,//13    1075
//testonly    903 *uSECOND,//14    1108
//testonly    878 *uSECOND,//15    1139
//testonly    855 *uSECOND,//16    1170
//testonly    833 *uSECOND,//17    1200
#endif
};


#else //HJ 1-2相步进表 （前36采集自DEMO板，后30取自CAPD245）

CONST ULONG TAB_PRTIME[66] = {
    5467*uSECOND,      // 1 //
    3468*uSECOND,      // 2 //
    2771*uSECOND,      // 3 //
    2474*uSECOND,      // 4 //

    1847*uSECOND,      // 5 //
    1684*uSECOND,      // 6 //
    1562*uSECOND,      // 7 //
    1460*uSECOND,      // 8 //

    1393*uSECOND,      // 9 //
    1311*uSECOND,      // 10 //
    1251*uSECOND,      // 11 //
    1200*uSECOND,      // 12 //

    1162*uSECOND,      // 13 //
    1112*uSECOND,      // 14 //
    1073*uSECOND,      // 15 //
    1045*uSECOND,      // 16 //

    1019*uSECOND,     // 17 //
    978*uSECOND,      // 18 //
    948*uSECOND,      // 19 //
    919*uSECOND,      // 20 //

    899*uSECOND,      // 21 //
    859*uSECOND,      // 22 //
    828*uSECOND,      // 23 //
    800*uSECOND,      // 24 //

    769*uSECOND,      // 25 //
    739*uSECOND,      // 26 //
    709*uSECOND,      // 27 //
    679*uSECOND,      // 28 //

    659*uSECOND,      // 29 //
    619*uSECOND,      // 30 //
    588*uSECOND,      // 31 //
    560*uSECOND,      // 32 //

    539*uSECOND,      // 33 //
    519*uSECOND,      // 34 //
    509*uSECOND,      // 35 //
    500*uSECOND,      // 36 //

	 ((508-10)*uSECOND)-1,  //	Step 37  1967
	 ((499-10)*uSECOND)-1,  //	Step 38  2004
	 ((490-10)*uSECOND)-1,  //	Step 39  2039
	 ((482-10)*uSECOND)-1,  //	Step 40  2075
	 ((474-10)*uSECOND)-1,  //	Step 41  2109
	 ((467-10)*uSECOND)-1,  //	Step 42  2143
	 ((459-10)*uSECOND)-1,  //	Step 43  2176
	 ((453-10)*uSECOND)-1,  //	Step 44  2209
	 ((446-10)*uSECOND)-1,  //	Step 45  2242
	 ((440-10)*uSECOND)-1,  //	Step 46  2274
	 ((434-10)*uSECOND)-1,  //	Step 47  2305
	 ((428-10)*uSECOND)-1,  //	Step 48  2337
	 ((422-10)*uSECOND)-1,  //	Step 49  2367
	 ((417-10)*uSECOND)-1,  //	Step 50  2398
	 ((412-10)*uSECOND)-1,  //	Step 51  2428
	 ((407-10)*uSECOND)-1,  //	Step 52  2457
	 ((402-10)*uSECOND)-1,  //	Step 53  2487
	 ((398-10)*uSECOND)-1,  //	Step 54  2515
	 ((393-10)*uSECOND)-1,  //	Step 55  2544
	 ((389-10)*uSECOND)-1,  //	Step 56 572
	 ((385-10)*uSECOND)-1,  //	Step 57  2600
	 ((381-10)*uSECOND)-1,  //	Step 58  2628
	 ((377-10)*uSECOND)-1,  //	Step 59  2655
	 ((373-10)*uSECOND)-1,  //	Step 60  2683
	 ((369-10)*uSECOND)-1,  //	Step 61  2709
	 ((365-10)*uSECOND)-1,  //	Step 62  2736
	 ((362-10)*uSECOND)-1,  //	Step 63  2762
	 ((359-10)*uSECOND)-1,  //	Step 64  2788
	 ((355-10)*uSECOND)-1,  //	Step 65  2814
	 ((352-10)*uSECOND)-1,  //	Step 66  2840
};
#endif //HJ

//电池供电加热参数
#if POWERCTRL
CONST ULONG TABSTROBE24VB[32] = { //HJ 每点加热一次时，取表值的2倍值
    uSECOND*640 ,//57  //100,426 /128=0*
    uSECOND*640 ,//57  //100,426 /128=1*
    uSECOND*640 ,//57  //100,426 /128=2*
    uSECOND*640	,//75  //95	,484 /128=3*
    uSECOND*640	,//93  //90	,551 /128=4*
    uSECOND*640	,//111 //85	,627 /128=5
    uSECOND*640	,//130 //80	,715 /128=6
    uSECOND*640	,//148 //75	,815 /128=7
    uSECOND*640	,//166 //70	,928 /128=8
    uSECOND*640	,//184 //65	,1056/128=9
    uSECOND*640	,//202 //60	,1199/128=10
    uSECOND*640	,//220 //55	,1358/128=11
    uSECOND*640	,//238 //50	,1532/128=12
    uSECOND*640	,//248 //50	,1532/128=13*
    uSECOND*640	,//256 //45	,1721/128=14
    uSECOND*640	,//266 //45	,1721/128=15*
    uSECOND*640	,//274 //40	,1921/128=16
    uSECOND*640	,//292 //35	,2131/128=17
    uSECOND*640	,//292 //35	,2131/128=18
    uSECOND*640	,//310 //30	,2345/128=19
    uSECOND*640	,//310 //30	,2345/128=20
    uSECOND*640	,//328 //25	,2561/128=21
    uSECOND*640	,//346 //20	,2771/128=22
    uSECOND*640	,//356 //20	,2771/128=23*
    uSECOND*640	,//364 //15	,2971/128=24
    uSECOND*640 ,//382 //10	,3158/128=25
    uSECOND*640 ,//400 //5	,3327/128=26
    uSECOND*640 ,//400 //5	,3327/128=27*
    uSECOND*640 ,//418 //0	,3477/128=28
    uSECOND*640 ,//418 //0	,3477/128=29*
    uSECOND*640 ,//418 //0	,3477/128=30*
    uSECOND*640 ,//418 //0	,3477/128=31*
};
#endif
#if 0 //CCR
//适配器供电加热参数
CONST ULONG TABSTROBE24V[32][6] = {
//[pps] 500	    600	            700	            800	            900	            1000
uSECOND*95	,uSECOND*92 	,uSECOND*89 	,uSECOND*87		,uSECOND*85 	,uSECOND*83  	,//57  //100	,426 /128=0*
uSECOND*95	,uSECOND*92 	,uSECOND*89 	,uSECOND*87		,uSECOND*85 	,uSECOND*83  	,//57  //100	,426 /128=1*
uSECOND*95	,uSECOND*92 	,uSECOND*89 	,uSECOND*87		,uSECOND*85 	,uSECOND*83  	,//57  //100	,426 /128=2*
uSECOND*124	,uSECOND*121	,uSECOND*117	,uSECOND*114	,uSECOND*111	,uSECOND*109  	,//75  //95	,484 /128=3*
uSECOND*154	,uSECOND*149	,uSECOND*145	,uSECOND*142	,uSECOND*138	,uSECOND*135  	,//93  //90	,551 /128=4*
uSECOND*184	,uSECOND*178	,uSECOND*173	,uSECOND*169	,uSECOND*165	,uSECOND*161 	,//111 //85	,627 /128=5
uSECOND*214	,uSECOND*207	,uSECOND*201	,uSECOND*196	,uSECOND*191	,uSECOND*189 	,//130 //80	,715 /128=6
uSECOND*244	,uSECOND*236	,uSECOND*229	,uSECOND*223	,uSECOND*218	,uSECOND*215 	,//148 //75	,815 /128=7
uSECOND*273	,uSECOND*265	,uSECOND*257	,uSECOND*251	,uSECOND*245	,uSECOND*241 	,//166 //70	,928 /128=8
uSECOND*303	,uSECOND*294	,uSECOND*285	,uSECOND*278	,uSECOND*271	,uSECOND*267 	,//184 //65	,1056/128=9
uSECOND*333	,uSECOND*323	,uSECOND*313	,uSECOND*305	,uSECOND*298	,uSECOND*293 	,//202 //60	,1199/128=10
uSECOND*363	,uSECOND*351	,uSECOND*342	,uSECOND*333	,uSECOND*325	,uSECOND*319 	,//220 //55	,1358/128=11
uSECOND*392	,uSECOND*380  	,uSECOND*370  	,uSECOND*360  	,uSECOND*351	,uSECOND*345 	,//238 //50	,1532/128=12
uSECOND*410	,uSECOND*390  	,uSECOND*380  	,uSECOND*370  	,uSECOND*361	,uSECOND*360 	,//248 //50	,1532/128=13*
uSECOND*422	,uSECOND*409	,uSECOND*398	,uSECOND*387	,uSECOND*378	,uSECOND*371 	,//256 //45	,1721/128=14
uSECOND*442	,uSECOND*419	,uSECOND*418	,uSECOND*400	,uSECOND*388	,uSECOND*386 	,//266 //45	,1721/128=15*
uSECOND*452	,uSECOND*438	,uSECOND*426	,uSECOND*415	,uSECOND*405	,uSECOND*397 	,//274 //40	,1921/128=16
uSECOND*482	,uSECOND*467	,uSECOND*454	,uSECOND*442	,uSECOND*431	,uSECOND*423 	,//292 //35	,2131/128=17
uSECOND*482	,uSECOND*467	,uSECOND*454	,uSECOND*442	,uSECOND*431	,uSECOND*423 	,//292 //35	,2131/128=18
uSECOND*511	,uSECOND*496	,uSECOND*482	,uSECOND*469	,uSECOND*458	,uSECOND*450 	,//310 //30	,2345/128=19
uSECOND*511	,uSECOND*496	,uSECOND*482	,uSECOND*469	,uSECOND*458	,uSECOND*450 	,//310 //30	,2345/128=20
uSECOND*541	,uSECOND*525	,uSECOND*510  	,uSECOND*497	,uSECOND*485	,uSECOND*476 	,//328 //25	,2561/128=21
uSECOND*571	,uSECOND*553	,uSECOND*538	,uSECOND*524	,uSECOND*511	,uSECOND*502 	,//346 //20	,2771/128=22
uSECOND*611	,uSECOND*563	,uSECOND*548	,uSECOND*534	,uSECOND*521	,uSECOND*516 	,//356 //20	,2771/128=23*
uSECOND*601	,uSECOND*582	,uSECOND*566	,uSECOND*551	,uSECOND*538	,uSECOND*528 	,//364 //15	,2971/128=24
uSECOND*630 ,uSECOND*611	,uSECOND*594	,uSECOND*578	,uSECOND*565	,uSECOND*554 	,//382 //10	,3158/128=25
uSECOND*660 ,uSECOND*640  	,uSECOND*622	,uSECOND*606	,uSECOND*591	,uSECOND*580 	,//400 //5	,3327/128=26
uSECOND*675 ,uSECOND*650  	,uSECOND*632	,uSECOND*606	,uSECOND*591	,uSECOND*580 	,//400 //5	,3327/128=27*
uSECOND*690 ,uSECOND*669	,uSECOND*650  	,uSECOND*633	,uSECOND*618	,uSECOND*606 	,//418 //0	,3477/128=28
uSECOND*690 ,uSECOND*669	,uSECOND*650  	,uSECOND*633	,uSECOND*618	,uSECOND*606 	,//418 //0	,3477/128=29*
uSECOND*690 ,uSECOND*669	,uSECOND*650  	,uSECOND*633	,uSECOND*618	,uSECOND*606 	,//418 //0	,3477/128=30*
uSECOND*690 ,uSECOND*669	,uSECOND*650  	,uSECOND*633	,uSECOND*618	,uSECOND*606	,//418 //0	,3477/128=31*

};

#else //HJ 使用CAPD245的加热表
CONST ULONG TABSTROBE24V[32][15] = {
//PPS----400------------600------------800---------1000------------1200-----------1400---------1600------------1800-----------2000----------2200--------------2400----------2600----------2800-----------3000----------3200//温度--|Rx-----|AD0=(Rx*2^12)/(Rx+18k)
//Drive time for PPS: W=2000000/400
(9 ) *uSECOND, (8 )*uSECOND,(8 ) *uSECOND,(7 ) *uSECOND,(7 ) *uSECOND,(6 ) *uSECOND,(6 ) *uSECOND,(6 )  *uSECOND,(5 )  *uSECOND,(5 )  *uSECOND,(5 )  *uSECOND,(5 )  *uSECOND,(5 )  *uSECOND,(5 )  *uSECOND,(5 )  *uSECOND,//96.25  2.32454 469     4   0
(32 )*uSECOND, (29)*uSECOND,(27 )*uSECOND,(25 )*uSECOND,(24 )*uSECOND,(23 )*uSECOND,(22 )*uSECOND,(21 ) *uSECOND,(20 ) *uSECOND,(19 ) *uSECOND,(18 ) *uSECOND,(18 ) *uSECOND,(17 ) *uSECOND,(17 ) *uSECOND,(16 ) *uSECOND,//92.5   2.59419 516     5   1
(56 )*uSECOND, (51)*uSECOND,(47 )*uSECOND,(44 )*uSECOND,(41 )*uSECOND,(39 )*uSECOND,(37 )*uSECOND,(36 ) *uSECOND,(34 ) *uSECOND,(33 ) *uSECOND,(32 ) *uSECOND,(31 ) *uSECOND,(30 ) *uSECOND,(29 ) *uSECOND,(28 ) *uSECOND,//88.75  2.90172 569     5   2
(79 )*uSECOND, (72)*uSECOND,(67 )*uSECOND,(62 )*uSECOND,(59 )*uSECOND,(56 )*uSECOND,(53 )*uSECOND,(51 ) *uSECOND,(49 ) *uSECOND,(47 ) *uSECOND,(45 ) *uSECOND,(44 ) *uSECOND,(43 ) *uSECOND,(41 ) *uSECOND,(40 ) *uSECOND,//85     3.25334 627     5   3
(102)*uSECOND, (93)*uSECOND,(86 )*uSECOND,(81 )*uSECOND,(76 )*uSECOND,(72 )*uSECOND,(69 )*uSECOND,(66 ) *uSECOND,(63 ) *uSECOND,(61 ) *uSECOND,(59 ) *uSECOND,(57 ) *uSECOND,(55 ) *uSECOND,(54 ) *uSECOND,(52 ) *uSECOND,//81.25  3.65640 692     6   4
(126)*uSECOND,(115)*uSECOND,(106)*uSECOND,(99 )*uSECOND,(93 )*uSECOND,(88 )*uSECOND,(84 )*uSECOND,(81 ) *uSECOND,(77 ) *uSECOND,(75 ) *uSECOND,(72 ) *uSECOND,(70 ) *uSECOND,(68 ) *uSECOND,(66 ) *uSECOND,(64 ) *uSECOND,//77.5   4.11969 763     6   5
(149)*uSECOND,(136)*uSECOND,(126)*uSECOND,(118)*uSECOND,(111)*uSECOND,(105)*uSECOND,(100)*uSECOND,(96 ) *uSECOND,(92 ) *uSECOND,(88 ) *uSECOND,(85 ) *uSECOND,(83 ) *uSECOND,(80 ) *uSECOND,(78 ) *uSECOND,(76 ) *uSECOND,//73.75  4.65367 842     7   6
(172)*uSECOND,(158)*uSECOND,(146)*uSECOND,(136)*uSECOND,(128)*uSECOND,(121)*uSECOND,(116)*uSECOND,(110 )*uSECOND,(106 )*uSECOND,(102 )*uSECOND,(99 ) *uSECOND,(96 ) *uSECOND,(93 ) *uSECOND,(90 ) *uSECOND,(88 ) *uSECOND,//70     5.27088 928     8   7
(196)*uSECOND,(179)*uSECOND,(165)*uSECOND,(155)*uSECOND,(145)*uSECOND,(138)*uSECOND,(131)*uSECOND,(125 )*uSECOND,(120 )*uSECOND,(116 )*uSECOND,(112 )*uSECOND,(109 )*uSECOND,(106 )*uSECOND,(103 )*uSECOND,(100 )*uSECOND,//66.25  5.98642 1023    8   8
(219)*uSECOND,(200)*uSECOND,(185)*uSECOND,(173)*uSECOND,(163)*uSECOND,(154)*uSECOND,(147)*uSECOND,(140 )*uSECOND,(135 )*uSECOND,(130 )*uSECOND,(126 )*uSECOND,(122 )*uSECOND,(118 )*uSECOND,(115 )*uSECOND,(112 )*uSECOND,//62.5   6.81847 1126    9   9
(242)*uSECOND,(222)*uSECOND,(205)*uSECOND,(191)*uSECOND,(180)*uSECOND,(171)*uSECOND,(162)*uSECOND,(155 )*uSECOND,(149 )*uSECOND,(144 )*uSECOND,(139 )*uSECOND,(135 )*uSECOND,(131 )*uSECOND,(127 )*uSECOND,(124 )*uSECOND,//58.75  7.78905 1238    10  10
(266)*uSECOND,(243)*uSECOND,(225)*uSECOND,(210)*uSECOND,(197)*uSECOND,(187)*uSECOND,(178)*uSECOND,(170 )*uSECOND,(164 )*uSECOND,(158 )*uSECOND,(152 )*uSECOND,(148 )*uSECOND,(143 )*uSECOND,(140 )*uSECOND,(136 )*uSECOND,//55     8.92491 1358    11  11
(289)*uSECOND,(264)*uSECOND,(244)*uSECOND,(228)*uSECOND,(215)*uSECOND,(203)*uSECOND,(194)*uSECOND,(185 )*uSECOND,(178 )*uSECOND,(172 )*uSECOND,(166 )*uSECOND,(161 )*uSECOND,(156 )*uSECOND,(152 )*uSECOND,(148 )*uSECOND,//51.25  10.2586 1487    12  12
(312)*uSECOND,(286)*uSECOND,(264)*uSECOND,(247)*uSECOND,(232)*uSECOND,(220)*uSECOND,(209)*uSECOND,(200 )*uSECOND,(192 )*uSECOND,(185 )*uSECOND,(179 )*uSECOND,(174 )*uSECOND,(169 )*uSECOND,(164 )*uSECOND,(160 )*uSECOND,//47.5   11.8302 1625    13  13
(336)*uSECOND,(307)*uSECOND,(284)*uSECOND,(265)*uSECOND,(250)*uSECOND,(236)*uSECOND,(225)*uSECOND,(215 )*uSECOND,(207 )*uSECOND,(199 )*uSECOND,(193 )*uSECOND,(187 )*uSECOND,(181 )*uSECOND,(176 )*uSECOND,(172 )*uSECOND,//43.75  13.6886 1770    14  14
(359)*uSECOND,(328)*uSECOND,(304)*uSECOND,(284)*uSECOND,(267)*uSECOND,(253)*uSECOND,(241)*uSECOND,(230 )*uSECOND,(221 )*uSECOND,(213 )*uSECOND,(206 )*uSECOND,(200 )*uSECOND,(194 )*uSECOND,(189 )*uSECOND,(184 )*uSECOND,//40     15.8944 1921    16  15
(383)*uSECOND,(350)*uSECOND,(323)*uSECOND,(302)*uSECOND,(284)*uSECOND,(269)*uSECOND,(256)*uSECOND,(245 )*uSECOND,(235 )*uSECOND,(227 )*uSECOND,(219 )*uSECOND,(212 )*uSECOND,(206 )*uSECOND,(201 )*uSECOND,(196 )*uSECOND,//36.25  18.5227 2078    17  16
(406)*uSECOND,(371)*uSECOND,(343)*uSECOND,(320)*uSECOND,(302)*uSECOND,(286)*uSECOND,(272)*uSECOND,(260 )*uSECOND,(250 )*uSECOND,(241 )*uSECOND,(233 )*uSECOND,(225 )*uSECOND,(219 )*uSECOND,(213 )*uSECOND,(208 )*uSECOND,//32.5   21.6669 2238    18  17
(429)*uSECOND,(392)*uSECOND,(363)*uSECOND,(339)*uSECOND,(319)*uSECOND,(302)*uSECOND,(288)*uSECOND,(275 )*uSECOND,(264 )*uSECOND,(255 )*uSECOND,(246 )*uSECOND,(238 )*uSECOND,(232 )*uSECOND,(225 )*uSECOND,(220 )*uSECOND,//28.75  25.4437 2399    19  18
(453)*uSECOND,(414)*uSECOND,(383)*uSECOND,(357)*uSECOND,(336)*uSECOND,(319)*uSECOND,(303)*uSECOND,(290 )*uSECOND,(279 )*uSECOND,(268 )*uSECOND,(259 )*uSECOND,(251 )*uSECOND,(244 )*uSECOND,(238 )*uSECOND,(232 )*uSECOND,//25     30      2561    21  19
(476)*uSECOND,(435)*uSECOND,(402)*uSECOND,(376)*uSECOND,(354)*uSECOND,(335)*uSECOND,(319)*uSECOND,(305 )*uSECOND,(293 )*uSECOND,(282 )*uSECOND,(273 )*uSECOND,(264 )*uSECOND,(257 )*uSECOND,(250 )*uSECOND,(244 )*uSECOND,//21.25  35.5209 2719    22  20
(499)*uSECOND,(456)*uSECOND,(422)*uSECOND,(394)*uSECOND,(371)*uSECOND,(351)*uSECOND,(335)*uSECOND,(320 )*uSECOND,(307 )*uSECOND,(296 )*uSECOND,(286 )*uSECOND,(277 )*uSECOND,(269 )*uSECOND,(262 )*uSECOND,(256 )*uSECOND,//17.5   42.2417 2873    23  21
(523)*uSECOND,(478)*uSECOND,(442)*uSECOND,(413)*uSECOND,(388)*uSECOND,(368)*uSECOND,(350)*uSECOND,(335 )*uSECOND,(322 )*uSECOND,(310 )*uSECOND,(300 )*uSECOND,(290 )*uSECOND,(282 )*uSECOND,(274 )*uSECOND,(268 )*uSECOND,//13.75  50.4624 3020    24  22
(546)*uSECOND,(499)*uSECOND,(462)*uSECOND,(431)*uSECOND,(406)*uSECOND,(384)*uSECOND,(366)*uSECOND,(350 )*uSECOND,(336 )*uSECOND,(324 )*uSECOND,(313 )*uSECOND,(303 )*uSECOND,(295 )*uSECOND,(287 )*uSECOND,(279 )*uSECOND,//10     60.5676 3158    25  23
(569)*uSECOND,(520)*uSECOND,(481)*uSECOND,(450)*uSECOND,(423)*uSECOND,(401)*uSECOND,(382)*uSECOND,(365 )*uSECOND,(351 )*uSECOND,(338 )*uSECOND,(326 )*uSECOND,(316 )*uSECOND,(307 )*uSECOND,(299 )*uSECOND,(291 )*uSECOND,//6.25   73.0537 3287    26  24 //HJ
(593)*uSECOND,(542)*uSECOND,(501)*uSECOND,(468)*uSECOND,(440)*uSECOND,(417)*uSECOND,(397)*uSECOND,(380 )*uSECOND,(365 )*uSECOND,(352 )*uSECOND,(340 )*uSECOND,(329 )*uSECOND,(320 )*uSECOND,(311 )*uSECOND,(303 )*uSECOND,//2.5    88.5645 3405    27  25
(608)*uSECOND,(556)*uSECOND,(514)*uSECOND,(480)*uSECOND,(452)*uSECOND,(428)*uSECOND,(408)*uSECOND,(390 )*uSECOND,(374 )*uSECOND,(361 )*uSECOND,(349 )*uSECOND,(338 )*uSECOND,(328 )*uSECOND,(319 )*uSECOND,(311 )*uSECOND,//-1.25  107.940 3511    28  26
(671)*uSECOND,(613)*uSECOND,(567)*uSECOND,(529)*uSECOND,(498)*uSECOND,(472)*uSECOND,(449)*uSECOND,(430 )*uSECOND,(413 )*uSECOND,(398 )*uSECOND,(384 )*uSECOND,(372 )*uSECOND,(362 )*uSECOND,(352 )*uSECOND,(343 )*uSECOND,//-5     132.286 3606    29  27
(671)*uSECOND,(613)*uSECOND,(567)*uSECOND,(529)*uSECOND,(498)*uSECOND,(472)*uSECOND,(449)*uSECOND,(430 )*uSECOND,(413 )*uSECOND,(398 )*uSECOND,(384 )*uSECOND,(372 )*uSECOND,(362 )*uSECOND,(352 )*uSECOND,(343 )*uSECOND,//-8.75  163.061 3689    29  28
(671)*uSECOND,(613)*uSECOND,(567)*uSECOND,(529)*uSECOND,(498)*uSECOND,(472)*uSECOND,(449)*uSECOND,(430 )*uSECOND,(413 )*uSECOND,(398 )*uSECOND,(384 )*uSECOND,(372 )*uSECOND,(362 )*uSECOND,(352 )*uSECOND,(343 )*uSECOND,//-12.5  202.210 3762    30  29
(671)*uSECOND,(613)*uSECOND,(567)*uSECOND,(529)*uSECOND,(498)*uSECOND,(472)*uSECOND,(449)*uSECOND,(430 )*uSECOND,(413 )*uSECOND,(398 )*uSECOND,(384 )*uSECOND,(372 )*uSECOND,(362 )*uSECOND,(352 )*uSECOND,(343 )*uSECOND,//-16.25 252.340 3824    30  30
(671)*uSECOND,(613)*uSECOND,(567)*uSECOND,(529)*uSECOND,(498)*uSECOND,(472)*uSECOND,(449)*uSECOND,(430 )*uSECOND,(413 )*uSECOND,(398 )*uSECOND,(384 )*uSECOND,(372 )*uSECOND,(362 )*uSECOND,(352 )*uSECOND,(343 )*uSECOND,//-20    316.971 3876    31  31
};

#endif

#elif CASE_CAPD245  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

CONST WORD TAB_PHASEJ[PHASEMAX] = {//    RPH1 RPH2 RPH3 RPH4

    RPH1 | RPH4,
    RPH1,
    RPH1 | RPH2,
    RPH2,
    RPH2 | RPH3,
    RPH3,
    RPH3 | RPH4,
    RPH4,

};

CONST ULONG TAB_PRTIME[VMAXSPEED] = {
//     ((5000-10)*uSECOND)-1,  //	Step 0   -
	 ((3707-10)*uSECOND)-1,  //	Step 1   270
	 ((2291-10)*uSECOND)-1,  //	Step 2   436
	 ((1769-10)*uSECOND)-1,  //	Step 3   565
	 ((1485-10)*uSECOND)-1,  //	Step 4   673
	 ((1302-10)*uSECOND)-1,  //	Step 5   768
	 ((1172-10)*uSECOND)-1,  //	Step 6   853
	 ((1074-10)*uSECOND)-1,  //	Step 7   931
	 ((996-10)*uSECOND)-1,  //	Step 8   1004
	 ((933-10)*uSECOND)-1,  //	Step 9   1072
	 ((880-10)*uSECOND)-1,  //	Step 10  1136
	 ((836-10)*uSECOND)-1,  //	Step 11  1197
	 ((797-10)*uSECOND)-1,  //	Step 12  1255
	 ((763-10)*uSECOND)-1,  //	Step 13  1310
	 ((733-10)*uSECOND)-1,  //	Step 14  1364
	 ((707-10)*uSECOND)-1,  //	Step 15  1415
	 ((683-10)*uSECOND)-1,  //	Step 16  1465
	 ((661-10)*uSECOND)-1,  //	Step 17  1513
	 ((641-10)*uSECOND)-1,  //	Step 18  1560
	 ((623-10)*uSECOND)-1,  //	Step 19  1605
	 ((606-10)*uSECOND)-1,  //	Step 20  1649
	 ((591-10)*uSECOND)-1,  //	Step 21  1692
	 ((577-10)*uSECOND)-1,  //	Step 22  1734
	 ((563-10)*uSECOND)-1,  //	Step 23  1775
	 ((551-10)*uSECOND)-1,  //	Step 24  1815
	 ((539-10)*uSECOND)-1,  //	Step 25  1854
	 ((528-10)*uSECOND)-1,  //  Step 26  1893
	 ((518-10)*uSECOND)-1,  //	Step 27  1930
	 ((508-10)*uSECOND)-1,  //	Step 28  1967
	 ((499-10)*uSECOND)-1,  //	Step 29  2004
	 ((490-10)*uSECOND)-1,  //	Step 30  2039
	 ((482-10)*uSECOND)-1,  //	Step 31  2075
	 ((474-10)*uSECOND)-1,  //	Step 32  2109
	 ((467-10)*uSECOND)-1,  //	Step 33  2143
	 ((459-10)*uSECOND)-1,  //	Step 34  2176
	 ((453-10)*uSECOND)-1,  //	Step 35  2209
	 ((446-10)*uSECOND)-1,  //	Step 36  2242
	 ((440-10)*uSECOND)-1,  //	Step 37  2274
	 ((434-10)*uSECOND)-1,  //	Step 38  2305
	 ((428-10)*uSECOND)-1,  //	Step 39  2337
	 ((422-10)*uSECOND)-1,  //	Step 40  2367
	 ((417-10)*uSECOND)-1,  //	Step 41  2398
	 ((412-10)*uSECOND)-1,  //	Step 42  2428
	 ((407-10)*uSECOND)-1,  //	Step 43  2457
	 ((402-10)*uSECOND)-1,  //	Step 44  2487
	 ((398-10)*uSECOND)-1,  //	Step 45  2515
	 ((393-10)*uSECOND)-1,  //	Step 46  2544
	 ((389-10)*uSECOND)-1,  //	Step 47  2572
	 ((385-10)*uSECOND)-1,  //	Step 48  2600
	 ((381-10)*uSECOND)-1,  //	Step 49  2628
	 ((377-10)*uSECOND)-1,  //	Step 50  2655
	 ((373-10)*uSECOND)-1,  //	Step 51  2683
	 ((369-10)*uSECOND)-1,  //	Step 52  2709
	 ((365-10)*uSECOND)-1,  //	Step 53  2736
	 ((362-10)*uSECOND)-1,  //	Step 54  2762
	 ((359-10)*uSECOND)-1,  //	Step 55  2788
	 ((355-10)*uSECOND)-1,  //	Step 56  2814
	 ((352-10)*uSECOND)-1,  //	Step 57  2840
	 ((349-10)*uSECOND)-1,  //	Step 58  2865
	 ((346-10)*uSECOND)-1,  //	Step 59  2891
	 ((343-10)*uSECOND)-1,  //	Step 60  2916
	 ((340-10)*uSECOND)-1,  //	Step 61  2940
	 ((337-10)*uSECOND)-1,  //	Step 62  2965
	 ((335-10)*uSECOND)-1,  //	Step 63  2989
	 ((332-10)*uSECOND)-1,  //	Step 64  3013
	 ((329-10)*uSECOND)-1,  //	Step 65  3037
	 ((327-10)*uSECOND)-1,  //	Step 66  3061
	 ((324-10)*uSECOND)-1,  //	Step 67  3085
	 ((322-10)*uSECOND)-1,  //	Step 68  3108
	 ((319-10)*uSECOND)-1,  //	Step 69  3131
	 ((317-10)*uSECOND)-1,  //	Step 70  3154
	 ((315-10)*uSECOND)-1,  //	Step 71  3177
	 ((313-10)*uSECOND)-1,  //	Step 72  3200
};

CONST ULONG TABSTROBE24V[32][15] = {
//PPS-400		600			800			1000		    1200		1400			1600		   1800		      2000		     2200		    2400			2600		2800             3000          3200//温度  |Rx     |AD0=(Rx*2^12)/(Rx+18k)
//Drive time for PPS: W=2000000/400
(9 ) *uSECOND, (8 )*uSECOND,(8 ) *uSECOND,(7 ) *uSECOND,(7 ) *uSECOND,(6 ) *uSECOND,(6 ) *uSECOND,(6 )  *uSECOND,(5 )  *uSECOND,(5 )  *uSECOND,(5 )  *uSECOND,(5 )  *uSECOND,(5 )  *uSECOND,(5 )  *uSECOND,(5 )  *uSECOND,//96.25  2.32454 469     4   0
(32 )*uSECOND, (29)*uSECOND,(27 )*uSECOND,(25 )*uSECOND,(24 )*uSECOND,(23 )*uSECOND,(22 )*uSECOND,(21 ) *uSECOND,(20 ) *uSECOND,(19 ) *uSECOND,(18 ) *uSECOND,(18 ) *uSECOND,(17 ) *uSECOND,(17 ) *uSECOND,(16 ) *uSECOND,//92.5   2.59419 516     5   1
(56 )*uSECOND, (51)*uSECOND,(47 )*uSECOND,(44 )*uSECOND,(41 )*uSECOND,(39 )*uSECOND,(37 )*uSECOND,(36 ) *uSECOND,(34 ) *uSECOND,(33 ) *uSECOND,(32 ) *uSECOND,(31 ) *uSECOND,(30 ) *uSECOND,(29 ) *uSECOND,(28 ) *uSECOND,//88.75  2.90172 569     5   2
(79 )*uSECOND, (72)*uSECOND,(67 )*uSECOND,(62 )*uSECOND,(59 )*uSECOND,(56 )*uSECOND,(53 )*uSECOND,(51 ) *uSECOND,(49 ) *uSECOND,(47 ) *uSECOND,(45 ) *uSECOND,(44 ) *uSECOND,(43 ) *uSECOND,(41 ) *uSECOND,(40 ) *uSECOND,//85     3.25334 627     5   3
(102)*uSECOND, (93)*uSECOND,(86 )*uSECOND,(81 )*uSECOND,(76 )*uSECOND,(72 )*uSECOND,(69 )*uSECOND,(66 ) *uSECOND,(63 ) *uSECOND,(61 ) *uSECOND,(59 ) *uSECOND,(57 ) *uSECOND,(55 ) *uSECOND,(54 ) *uSECOND,(52 ) *uSECOND,//81.25  3.65640 692     6   4
(126)*uSECOND,(115)*uSECOND,(106)*uSECOND,(99 )*uSECOND,(93 )*uSECOND,(88 )*uSECOND,(84 )*uSECOND,(81 ) *uSECOND,(77 ) *uSECOND,(75 ) *uSECOND,(72 ) *uSECOND,(70 ) *uSECOND,(68 ) *uSECOND,(66 ) *uSECOND,(64 ) *uSECOND,//77.5   4.11969 763     6   5
(149)*uSECOND,(136)*uSECOND,(126)*uSECOND,(118)*uSECOND,(111)*uSECOND,(105)*uSECOND,(100)*uSECOND,(96 ) *uSECOND,(92 ) *uSECOND,(88 ) *uSECOND,(85 ) *uSECOND,(83 ) *uSECOND,(80 ) *uSECOND,(78 ) *uSECOND,(76 ) *uSECOND,//73.75  4.65367 842     7   6
(172)*uSECOND,(158)*uSECOND,(146)*uSECOND,(136)*uSECOND,(128)*uSECOND,(121)*uSECOND,(116)*uSECOND,(110 )*uSECOND,(106 )*uSECOND,(102 )*uSECOND,(99 ) *uSECOND,(96 ) *uSECOND,(93 ) *uSECOND,(90 ) *uSECOND,(88 ) *uSECOND,//70     5.27088 928     8   7
(196)*uSECOND,(179)*uSECOND,(165)*uSECOND,(155)*uSECOND,(145)*uSECOND,(138)*uSECOND,(131)*uSECOND,(125 )*uSECOND,(120 )*uSECOND,(116 )*uSECOND,(112 )*uSECOND,(109 )*uSECOND,(106 )*uSECOND,(103 )*uSECOND,(100 )*uSECOND,//66.25  5.98642 1023    8   8
(219)*uSECOND,(200)*uSECOND,(185)*uSECOND,(173)*uSECOND,(163)*uSECOND,(154)*uSECOND,(147)*uSECOND,(140 )*uSECOND,(135 )*uSECOND,(130 )*uSECOND,(126 )*uSECOND,(122 )*uSECOND,(118 )*uSECOND,(115 )*uSECOND,(112 )*uSECOND,//62.5   6.81847 1126    9   9
(242)*uSECOND,(222)*uSECOND,(205)*uSECOND,(191)*uSECOND,(180)*uSECOND,(171)*uSECOND,(162)*uSECOND,(155 )*uSECOND,(149 )*uSECOND,(144 )*uSECOND,(139 )*uSECOND,(135 )*uSECOND,(131 )*uSECOND,(127 )*uSECOND,(124 )*uSECOND,//58.75  7.78905 1238    10  10
(266)*uSECOND,(243)*uSECOND,(225)*uSECOND,(210)*uSECOND,(197)*uSECOND,(187)*uSECOND,(178)*uSECOND,(170 )*uSECOND,(164 )*uSECOND,(158 )*uSECOND,(152 )*uSECOND,(148 )*uSECOND,(143 )*uSECOND,(140 )*uSECOND,(136 )*uSECOND,//55     8.92491 1358    11  11
(289)*uSECOND,(264)*uSECOND,(244)*uSECOND,(228)*uSECOND,(215)*uSECOND,(203)*uSECOND,(194)*uSECOND,(185 )*uSECOND,(178 )*uSECOND,(172 )*uSECOND,(166 )*uSECOND,(161 )*uSECOND,(156 )*uSECOND,(152 )*uSECOND,(148 )*uSECOND,//51.25  10.2586 1487    12  12
(312)*uSECOND,(286)*uSECOND,(264)*uSECOND,(247)*uSECOND,(232)*uSECOND,(220)*uSECOND,(209)*uSECOND,(200 )*uSECOND,(192 )*uSECOND,(185 )*uSECOND,(179 )*uSECOND,(174 )*uSECOND,(169 )*uSECOND,(164 )*uSECOND,(160 )*uSECOND,//47.5   11.8302 1625    13  13
(336)*uSECOND,(307)*uSECOND,(284)*uSECOND,(265)*uSECOND,(250)*uSECOND,(236)*uSECOND,(225)*uSECOND,(215 )*uSECOND,(207 )*uSECOND,(199 )*uSECOND,(193 )*uSECOND,(187 )*uSECOND,(181 )*uSECOND,(176 )*uSECOND,(172 )*uSECOND,//43.75  13.6886 1770    14  14
(359)*uSECOND,(328)*uSECOND,(304)*uSECOND,(284)*uSECOND,(267)*uSECOND,(253)*uSECOND,(241)*uSECOND,(230 )*uSECOND,(221 )*uSECOND,(213 )*uSECOND,(206 )*uSECOND,(200 )*uSECOND,(194 )*uSECOND,(189 )*uSECOND,(184 )*uSECOND,//40     15.8944 1921    16  15
(383)*uSECOND,(350)*uSECOND,(323)*uSECOND,(302)*uSECOND,(284)*uSECOND,(269)*uSECOND,(256)*uSECOND,(245 )*uSECOND,(235 )*uSECOND,(227 )*uSECOND,(219 )*uSECOND,(212 )*uSECOND,(206 )*uSECOND,(201 )*uSECOND,(196 )*uSECOND,//36.25  18.5227 2078    17  16
(406)*uSECOND,(371)*uSECOND,(343)*uSECOND,(320)*uSECOND,(302)*uSECOND,(286)*uSECOND,(272)*uSECOND,(260 )*uSECOND,(250 )*uSECOND,(241 )*uSECOND,(233 )*uSECOND,(225 )*uSECOND,(219 )*uSECOND,(213 )*uSECOND,(208 )*uSECOND,//32.5   21.6669 2238    18  17
(429)*uSECOND,(392)*uSECOND,(363)*uSECOND,(339)*uSECOND,(319)*uSECOND,(302)*uSECOND,(288)*uSECOND,(275 )*uSECOND,(264 )*uSECOND,(255 )*uSECOND,(246 )*uSECOND,(238 )*uSECOND,(232 )*uSECOND,(225 )*uSECOND,(220 )*uSECOND,//28.75  25.4437 2399    19  18
(453)*uSECOND,(414)*uSECOND,(383)*uSECOND,(357)*uSECOND,(336)*uSECOND,(319)*uSECOND,(303)*uSECOND,(290 )*uSECOND,(279 )*uSECOND,(268 )*uSECOND,(259 )*uSECOND,(251 )*uSECOND,(244 )*uSECOND,(238 )*uSECOND,(232 )*uSECOND,//25     30      2561    21  19
(476)*uSECOND,(435)*uSECOND,(402)*uSECOND,(376)*uSECOND,(354)*uSECOND,(335)*uSECOND,(319)*uSECOND,(305 )*uSECOND,(293 )*uSECOND,(282 )*uSECOND,(273 )*uSECOND,(264 )*uSECOND,(257 )*uSECOND,(250 )*uSECOND,(244 )*uSECOND,//21.25  35.5209 2719    22  20
(499)*uSECOND,(456)*uSECOND,(422)*uSECOND,(394)*uSECOND,(371)*uSECOND,(351)*uSECOND,(335)*uSECOND,(320 )*uSECOND,(307 )*uSECOND,(296 )*uSECOND,(286 )*uSECOND,(277 )*uSECOND,(269 )*uSECOND,(262 )*uSECOND,(256 )*uSECOND,//17.5   42.2417 2873    23  21
(523)*uSECOND,(478)*uSECOND,(442)*uSECOND,(413)*uSECOND,(388)*uSECOND,(368)*uSECOND,(350)*uSECOND,(335 )*uSECOND,(322 )*uSECOND,(310 )*uSECOND,(300 )*uSECOND,(290 )*uSECOND,(282 )*uSECOND,(274 )*uSECOND,(268 )*uSECOND,//13.75  50.4624 3020    24  22
(546)*uSECOND,(499)*uSECOND,(462)*uSECOND,(431)*uSECOND,(406)*uSECOND,(384)*uSECOND,(366)*uSECOND,(350 )*uSECOND,(336 )*uSECOND,(324 )*uSECOND,(313 )*uSECOND,(303 )*uSECOND,(295 )*uSECOND,(287 )*uSECOND,(279 )*uSECOND,//10     60.5676 3158    25  23
(569)*uSECOND,(520)*uSECOND,(481)*uSECOND,(450)*uSECOND,(423)*uSECOND,(401)*uSECOND,(382)*uSECOND,(365 )*uSECOND,(351 )*uSECOND,(338 )*uSECOND,(326 )*uSECOND,(316 )*uSECOND,(307 )*uSECOND,(299 )*uSECOND,(291 )*uSECOND,//6.25   73.0537 3287    26  24
(593)*uSECOND,(542)*uSECOND,(501)*uSECOND,(468)*uSECOND,(440)*uSECOND,(417)*uSECOND,(397)*uSECOND,(380 )*uSECOND,(365 )*uSECOND,(352 )*uSECOND,(340 )*uSECOND,(329 )*uSECOND,(320 )*uSECOND,(311 )*uSECOND,(303 )*uSECOND,//2.5    88.5645 3405    27  25
(608)*uSECOND,(556)*uSECOND,(514)*uSECOND,(480)*uSECOND,(452)*uSECOND,(428)*uSECOND,(408)*uSECOND,(390 )*uSECOND,(374 )*uSECOND,(361 )*uSECOND,(349 )*uSECOND,(338 )*uSECOND,(328 )*uSECOND,(319 )*uSECOND,(311 )*uSECOND,//-1.25  107.940 3511    28  26
(671)*uSECOND,(613)*uSECOND,(567)*uSECOND,(529)*uSECOND,(498)*uSECOND,(472)*uSECOND,(449)*uSECOND,(430 )*uSECOND,(413 )*uSECOND,(398 )*uSECOND,(384 )*uSECOND,(372 )*uSECOND,(362 )*uSECOND,(352 )*uSECOND,(343 )*uSECOND,//-5     132.286 3606    29  27
(671)*uSECOND,(613)*uSECOND,(567)*uSECOND,(529)*uSECOND,(498)*uSECOND,(472)*uSECOND,(449)*uSECOND,(430 )*uSECOND,(413 )*uSECOND,(398 )*uSECOND,(384 )*uSECOND,(372 )*uSECOND,(362 )*uSECOND,(352 )*uSECOND,(343 )*uSECOND,//-8.75  163.061 3689    29  28
(671)*uSECOND,(613)*uSECOND,(567)*uSECOND,(529)*uSECOND,(498)*uSECOND,(472)*uSECOND,(449)*uSECOND,(430 )*uSECOND,(413 )*uSECOND,(398 )*uSECOND,(384 )*uSECOND,(372 )*uSECOND,(362 )*uSECOND,(352 )*uSECOND,(343 )*uSECOND,//-12.5  202.210 3762    30  29
(671)*uSECOND,(613)*uSECOND,(567)*uSECOND,(529)*uSECOND,(498)*uSECOND,(472)*uSECOND,(449)*uSECOND,(430 )*uSECOND,(413 )*uSECOND,(398 )*uSECOND,(384 )*uSECOND,(372 )*uSECOND,(362 )*uSECOND,(352 )*uSECOND,(343 )*uSECOND,//-16.25 252.340 3824    30  30
(671)*uSECOND,(613)*uSECOND,(567)*uSECOND,(529)*uSECOND,(498)*uSECOND,(472)*uSECOND,(449)*uSECOND,(430 )*uSECOND,(413 )*uSECOND,(398 )*uSECOND,(384 )*uSECOND,(372 )*uSECOND,(362 )*uSECOND,(352 )*uSECOND,(343 )*uSECOND,//-20    316.971 3876    31  31
};

#elif CASE_CAPD347  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

CONST WORD TAB_PHASEJ[PHASEMAX] = {//    RPH1 RPH2 RPH3 RPH4
    RMCL  | RPH2,			//Step2 L   H   L   L
    RMCL  | 0,      			//Step3 L   L   L   L
    RMCL  | RPH1,			//Step4 H   L   L   L
    RMCL  | RPH1 | RPH2,     //Step1 H   H   L   L
};

// BYTE TAB_PHASEJ[] = {0x60,0xC0,0x90,0x30};
CONST ULONG TAB_PRTIME[VMAXSPEED] = {
  (5653)*uSECOND,	// step 1
  (3494)*uSECOND,	// step 2
  (2698)*uSECOND,	// step 3
  (2265)*uSECOND,	// step 4
  (1986)*uSECOND,	// step 5
  (1787)*uSECOND,	// step 6
  (1637)*uSECOND,	// step 7
  (1519)*uSECOND,	// step 8
  (1423)*uSECOND,	// step 9
  (1343)*uSECOND,	// step 10
  (1274)*uSECOND,	// step 11
  (1215)*uSECOND,	// step 12
  (1164)*uSECOND,	// step 13
  (1118)*uSECOND,	// step 14
  (1078)*uSECOND,	// step 15
  (1041)*uSECOND,	// step 16
  (1008)*uSECOND,	// step 17
  (978 )*uSECOND,	// step 18
  (950 )*uSECOND,	// step 19
  (925 )*uSECOND,	// step 20
  (901 )*uSECOND,	// step 21
  (880 )*uSECOND,	// step 22
  (859 )*uSECOND,	// step 23
  (840 )*uSECOND,	// step 24
  (822 )*uSECOND,	// step 25
  (806 )*uSECOND,	// step 26
  (790 )*uSECOND,	// step 27
  (775 )*uSECOND,	// step 28
  (761 )*uSECOND,	// step 29
  (748 )*uSECOND,	// step 30
  (735 )*uSECOND,	// step 31
  (723 )*uSECOND,	// step 32
  (712 )*uSECOND,	// step 33
  (701 )*uSECOND,	// step 34
  (690 )*uSECOND,	// step 35
  (680 )*uSECOND,	// step 36
  (671 )*uSECOND,	// step 37
  (662 )*uSECOND,	// step 38
  (653 )*uSECOND,	// step 39
  (644 )*uSECOND,	// step 40
  (636 )*uSECOND,	// step 41
  (628 )*uSECOND,	// step 42
  (621 )*uSECOND,	// step 43
  (613 )*uSECOND,	// step 44
  (606 )*uSECOND,	// step 45
  (599 )*uSECOND,	// step 46
  (593 )*uSECOND,	// step 47
  (586 )*uSECOND,	// step 48
  (580 )*uSECOND,	// step 49
  (574 )*uSECOND,	// step 50
  (569 )*uSECOND,	// step 51
  (563 )*uSECOND,	// step 52
  (557 )*uSECOND,	// step 53
  (552 )*uSECOND,	// step 54
  (547 )*uSECOND,	// step 55
  (542 )*uSECOND,	// step 56
  (537 )*uSECOND,	// step 57
  (532 )*uSECOND,	// step 58
  (528 )*uSECOND,	// step 59
  (523 )*uSECOND,	// step 60
  (519 )*uSECOND,	// step 61
  (514 )*uSECOND,	// step 62
  (510 )*uSECOND,	// step 63
  (506 )*uSECOND,	// step 64
  (502 )*uSECOND,	// step 65
  (498 )*uSECOND,	// step 66
  (494 )*uSECOND,	// step 67
  (491 )*uSECOND,	// step 68
  (487 )*uSECOND,	// step 69
  (483 )*uSECOND,	// step 70
  (480 )*uSECOND,	// step 71
  (477 )*uSECOND,	// step 72
  (473 )*uSECOND,	// step 73<-------DEMO板的最高速度
  (470 )*uSECOND,	// step 74
  (467 )*uSECOND,	// step 75
  (464 )*uSECOND,	// step 76
  (461 )*uSECOND,	// step 77
  (458 )*uSECOND,	// step 78
  (455 )*uSECOND,	// step 79
  (452 )*uSECOND,	// step 80
  (449 )*uSECOND,	// step 81
  (446 )*uSECOND,	// step 82
  (443 )*uSECOND,	// step 83
  (441 )*uSECOND,	// step 84
  (438 )*uSECOND,	// step 85
  (435 )*uSECOND,	// step 86
  (433 )*uSECOND,	// step 87
  (430 )*uSECOND,	// step 88
  (428 )*uSECOND,	// step 89
  (425 )*uSECOND,	// step 90
  (423 )*uSECOND,	// step 91
  (421 )*uSECOND,	// step 92
  (418 )*uSECOND,	// step 93
  (416 )*uSECOND,	// step 94
  (414 )*uSECOND,	// step 95
  (412 )*uSECOND,	// step 96
  (410 )*uSECOND,	// step 97
  (407 )*uSECOND,	// step 98
  (405 )*uSECOND,	// step 99
  (403 )*uSECOND,	// step 100
  (401 )*uSECOND,	// step 101
  (399 )*uSECOND,	// step 102
  (397 )*uSECOND,	// step 103
  (395 )*uSECOND,	// step 104
  (393 )*uSECOND,	// step 105
  (392 )*uSECOND,	// step 106
  (390 )*uSECOND,	// step 107
  (388 )*uSECOND,	// step 108
  (386 )*uSECOND,	// step 109
  (384 )*uSECOND,	// step 110
  (382 )*uSECOND,	// step 111
  (381 )*uSECOND,	// step 112
  (379 )*uSECOND,	// step 113
  (377 )*uSECOND,	// step 114
  (376 )*uSECOND,	// step 115
  (374 )*uSECOND,	// step 116
  (372 )*uSECOND,	// step 117
  (371 )*uSECOND,	// step 118
  (369 )*uSECOND,	// step 119
  (368 )*uSECOND,	// step 120
};

CONST ULONG TABSTROBE24V[32][13] = {
//PPS-400			600			800			1000		1200		1400			1600		1800		2000		2200		2400			2600		2720       //温度  |Rx     |AD0=(Rx*2^12)/(Rx+18k)
//Drive time for PPS: W=2000000/400
 219*uSECOND, 194*uSECOND,  168*uSECOND,  152*uSECOND, 141*uSECOND, 133*uSECOND, 128*uSECOND, 123*uSECOND, 120*uSECOND, 117*uSECOND, 114*uSECOND, 112*uSECOND, 111*uSECOND, //96.25  2.32454 469     4   0
 259*uSECOND, 230*uSECOND,  199*uSECOND,  180*uSECOND, 167*uSECOND, 158*uSECOND, 151*uSECOND, 146*uSECOND, 142*uSECOND, 138*uSECOND, 135*uSECOND, 133*uSECOND, 132*uSECOND, //92.5   2.59419 516     5   1
 299*uSECOND, 266*uSECOND,  230*uSECOND,  208*uSECOND, 193*uSECOND, 183*uSECOND, 175*uSECOND, 169*uSECOND, 164*uSECOND, 160*uSECOND, 156*uSECOND, 154*uSECOND, 152*uSECOND, //88.75  2.90172 569     5   2
 340*uSECOND, 302*uSECOND,  261*uSECOND,  236*uSECOND, 219*uSECOND, 207*uSECOND, 198*uSECOND, 191*uSECOND, 186*uSECOND, 181*uSECOND, 178*uSECOND, 174*uSECOND, 173*uSECOND, //85     3.25334 627     5   3
 380*uSECOND, 338*uSECOND,  292*uSECOND,  264*uSECOND, 245*uSECOND, 232*uSECOND, 222*uSECOND, 214*uSECOND, 208*uSECOND, 203*uSECOND, 199*uSECOND, 195*uSECOND, 193*uSECOND, //81.25  3.65640 692     6   4
 421*uSECOND, 374*uSECOND,  323*uSECOND,  292*uSECOND, 271*uSECOND, 257*uSECOND, 246*uSECOND, 237*uSECOND, 230*uSECOND, 224*uSECOND, 220*uSECOND, 216*uSECOND, 214*uSECOND, //77.5   4.11969 763     6   5
 461*uSECOND, 410*uSECOND,  354*uSECOND,  320*uSECOND, 297*uSECOND, 281*uSECOND, 269*uSECOND, 260*uSECOND, 252*uSECOND, 246*uSECOND, 241*uSECOND, 237*uSECOND, 234*uSECOND, //73.75  4.65367 842     7   6
 502*uSECOND, 446*uSECOND,  385*uSECOND,  348*uSECOND, 323*uSECOND, 306*uSECOND, 293*uSECOND, 283*uSECOND, 274*uSECOND, 268*uSECOND, 262*uSECOND, 257*uSECOND, 255*uSECOND, //70     5.27088 928     8   7
 542*uSECOND, 482*uSECOND,  416*uSECOND,  376*uSECOND, 349*uSECOND, 331*uSECOND, 316*uSECOND, 305*uSECOND, 296*uSECOND, 289*uSECOND, 283*uSECOND, 278*uSECOND, 275*uSECOND, //66.25  5.98642 1023    8   8
 583*uSECOND, 518*uSECOND,  447*uSECOND,  404*uSECOND, 376*uSECOND, 355*uSECOND, 340*uSECOND, 328*uSECOND, 319*uSECOND, 311*uSECOND, 304*uSECOND, 299*uSECOND, 296*uSECOND, //62.5   6.81847 1126    9   9
 623*uSECOND, 554*uSECOND,  478*uSECOND,  432*uSECOND, 402*uSECOND, 380*uSECOND, 364*uSECOND, 351*uSECOND, 341*uSECOND, 332*uSECOND, 325*uSECOND, 320*uSECOND, 316*uSECOND, //58.75  7.78905 1238    10  10
 663*uSECOND, 590*uSECOND,  509*uSECOND,  460*uSECOND, 428*uSECOND, 405*uSECOND, 387*uSECOND, 374*uSECOND, 363*uSECOND, 354*uSECOND, 347*uSECOND, 340*uSECOND, 337*uSECOND, //55     8.92491 1358    11  11
 704*uSECOND, 626*uSECOND,  540*uSECOND,  488*uSECOND, 454*uSECOND, 429*uSECOND, 411*uSECOND, 396*uSECOND, 385*uSECOND, 376*uSECOND, 368*uSECOND, 361*uSECOND, 358*uSECOND, //51.25  10.2586 1487    12  12
 744*uSECOND, 662*uSECOND,  571*uSECOND,  516*uSECOND, 480*uSECOND, 454*uSECOND, 434*uSECOND, 419*uSECOND, 407*uSECOND, 397*uSECOND, 389*uSECOND, 382*uSECOND, 378*uSECOND, //47.5   11.8302 1625    13  13
 785*uSECOND, 698*uSECOND,  602*uSECOND,  544*uSECOND, 506*uSECOND, 479*uSECOND, 458*uSECOND, 442*uSECOND, 429*uSECOND, 419*uSECOND, 410*uSECOND, 403*uSECOND, 399*uSECOND, //43.75  13.6886 1770    14  14
 825*uSECOND, 734*uSECOND,  633*uSECOND,  572*uSECOND, 532*uSECOND, 503*uSECOND, 482*uSECOND, 465*uSECOND, 451*uSECOND, 440*uSECOND, 431*uSECOND, 423*uSECOND, 419*uSECOND, //40     15.8944 1921    16  15
 866*uSECOND, 770*uSECOND,  664*uSECOND,  600*uSECOND, 558*uSECOND, 528*uSECOND, 505*uSECOND, 487*uSECOND, 473*uSECOND, 462*uSECOND, 452*uSECOND, 444*uSECOND, 440*uSECOND, //36.25  18.5227 2078    17  16
 906*uSECOND, 806*uSECOND,  695*uSECOND,  629*uSECOND, 584*uSECOND, 552*uSECOND, 529*uSECOND, 510*uSECOND, 495*uSECOND, 483*uSECOND, 473*uSECOND, 465*uSECOND, 460*uSECOND, //32.5   21.6669 2238    18  17
 947*uSECOND, 842*uSECOND,  726*uSECOND,  657*uSECOND, 610*uSECOND, 577*uSECOND, 552*uSECOND, 533*uSECOND, 518*uSECOND, 505*uSECOND, 494*uSECOND, 486*uSECOND, 481*uSECOND, //28.75  25.4437 2399    19  18
 987*uSECOND, 878*uSECOND,  757*uSECOND,  685*uSECOND, 636*uSECOND, 602*uSECOND, 576*uSECOND, 556*uSECOND, 540*uSECOND, 527*uSECOND, 516*uSECOND, 506*uSECOND, 501*uSECOND, //25     30      2561    21  19
1027*uSECOND, 914*uSECOND,  788*uSECOND,  713*uSECOND, 662*uSECOND, 626*uSECOND, 600*uSECOND, 579*uSECOND, 562*uSECOND, 548*uSECOND, 537*uSECOND, 527*uSECOND, 522*uSECOND, //21.25  35.5209 2719    22  20
1068*uSECOND, 950*uSECOND,  819*uSECOND,  741*uSECOND, 688*uSECOND, 651*uSECOND, 623*uSECOND, 601*uSECOND, 584*uSECOND, 570*uSECOND, 558*uSECOND, 548*uSECOND, 542*uSECOND, //17.5   42.2417 2873    23  21
1108*uSECOND, 986*uSECOND,  850*uSECOND,  769*uSECOND, 715*uSECOND, 676*uSECOND, 647*uSECOND, 624*uSECOND, 606*uSECOND, 591*uSECOND, 579*uSECOND, 568*uSECOND, 563*uSECOND, //13.75  50.4624 3020    24  22
1149*uSECOND,1022*uSECOND,  881*uSECOND,  797*uSECOND, 741*uSECOND, 700*uSECOND, 670*uSECOND, 647*uSECOND, 628*uSECOND, 613*uSECOND, 600*uSECOND, 589*uSECOND, 584*uSECOND, //10     60.5676 3158    25  23
1189*uSECOND,1058*uSECOND,  912*uSECOND,  825*uSECOND, 767*uSECOND, 725*uSECOND, 694*uSECOND, 670*uSECOND, 650*uSECOND, 634*uSECOND, 621*uSECOND, 610*uSECOND, 604*uSECOND, //6.25   73.0537 3287    26  24
1230*uSECOND,1094*uSECOND,  943*uSECOND,  853*uSECOND, 793*uSECOND, 750*uSECOND, 718*uSECOND, 692*uSECOND, 672*uSECOND, 656*uSECOND, 642*uSECOND, 631*uSECOND, 625*uSECOND, //2.5    88.5645 3405    27  25
1270*uSECOND,1130*uSECOND,  974*uSECOND,  881*uSECOND, 819*uSECOND, 774*uSECOND, 741*uSECOND, 715*uSECOND, 695*uSECOND, 678*uSECOND, 663*uSECOND, 651*uSECOND, 645*uSECOND, //-1.25  107.940 3511    28  26
1311*uSECOND,1166*uSECOND, 1005*uSECOND,  909*uSECOND, 845*uSECOND, 799*uSECOND, 765*uSECOND, 738*uSECOND, 717*uSECOND, 699*uSECOND, 685*uSECOND, 672*uSECOND, 666*uSECOND, //-5     132.286 3606    29  27
1351*uSECOND,1202*uSECOND, 1036*uSECOND,  937*uSECOND, 871*uSECOND, 824*uSECOND, 788*uSECOND, 761*uSECOND, 739*uSECOND, 721*uSECOND, 706*uSECOND, 693*uSECOND, 686*uSECOND, //-8.75  163.061 3689    29  28
1391*uSECOND,1238*uSECOND, 1067*uSECOND,  965*uSECOND, 897*uSECOND, 848*uSECOND, 812*uSECOND, 784*uSECOND, 761*uSECOND, 742*uSECOND, 727*uSECOND, 714*uSECOND, 707*uSECOND, //-12.5  202.210 3762    30  29
1432*uSECOND,1274*uSECOND, 1098*uSECOND,  993*uSECOND, 923*uSECOND, 873*uSECOND, 836*uSECOND, 806*uSECOND, 783*uSECOND, 764*uSECOND, 748*uSECOND, 734*uSECOND, 727*uSECOND, //-16.25 252.340 3824    30  30
1472*uSECOND,1310*uSECOND, 1129*uSECOND, 1021*uSECOND, 949*uSECOND, 898*uSECOND, 859*uSECOND, 829*uSECOND, 805*uSECOND, 785*uSECOND, 769*uSECOND, 755*uSECOND, 748*uSECOND, //-20    316.971 3876    31  31

/*Drive time for PPS: W=1000000/400
 170*uSECOND, 141*uSECOND, 128*uSECOND, 120*uSECOND, 114*uSECOND, 110*uSECOND, 107*uSECOND, 105*uSECOND, 103*uSECOND, 102*uSECOND, 101*uSECOND, 100*uSECOND, 99 *uSECOND,//96.25  2.32454 469     4   0
 201*uSECOND, 167*uSECOND, 151*uSECOND, 142*uSECOND, 135*uSECOND, 131*uSECOND, 127*uSECOND, 125*uSECOND, 123*uSECOND, 121*uSECOND, 119*uSECOND, 118*uSECOND, 118*uSECOND,//92.5   2.59419 516     5   1
 232*uSECOND, 193*uSECOND, 175*uSECOND, 164*uSECOND, 156*uSECOND, 151*uSECOND, 147*uSECOND, 144*uSECOND, 142*uSECOND, 140*uSECOND, 138*uSECOND, 137*uSECOND, 136*uSECOND,//88.75  2.90172 569     5   2
 264*uSECOND, 219*uSECOND, 198*uSECOND, 186*uSECOND, 178*uSECOND, 172*uSECOND, 167*uSECOND, 164*uSECOND, 161*uSECOND, 159*uSECOND, 157*uSECOND, 155*uSECOND, 154*uSECOND,//85     3.25334 627     5   3
 295*uSECOND, 245*uSECOND, 222*uSECOND, 208*uSECOND, 199*uSECOND, 192*uSECOND, 187*uSECOND, 183*uSECOND, 180*uSECOND, 178*uSECOND, 175*uSECOND, 174*uSECOND, 173*uSECOND,//81.25  3.65640 692     6   4
 326*uSECOND, 271*uSECOND, 246*uSECOND, 230*uSECOND, 220*uSECOND, 212*uSECOND, 207*uSECOND, 203*uSECOND, 199*uSECOND, 196*uSECOND, 194*uSECOND, 192*uSECOND, 191*uSECOND,//77.5   4.11969 763     6   5
 358*uSECOND, 297*uSECOND, 269*uSECOND, 252*uSECOND, 241*uSECOND, 233*uSECOND, 227*uSECOND, 222*uSECOND, 218*uSECOND, 215*uSECOND, 213*uSECOND, 211*uSECOND, 209*uSECOND,//73.75  4.65367 842     7   6
 389*uSECOND, 323*uSECOND, 293*uSECOND, 274*uSECOND, 262*uSECOND, 253*uSECOND, 247*uSECOND, 242*uSECOND, 237*uSECOND, 234*uSECOND, 231*uSECOND, 229*uSECOND, 228*uSECOND,//70     5.27088 928     8   7
 420*uSECOND, 349*uSECOND, 316*uSECOND, 296*uSECOND, 283*uSECOND, 274*uSECOND, 267*uSECOND, 261*uSECOND, 257*uSECOND, 253*uSECOND, 250*uSECOND, 247*uSECOND, 246*uSECOND,//66.25  5.98642 1023    8   8
 452*uSECOND, 376*uSECOND, 340*uSECOND, 319*uSECOND, 304*uSECOND, 294*uSECOND, 286*uSECOND, 281*uSECOND, 276*uSECOND, 272*uSECOND, 269*uSECOND, 266*uSECOND, 264*uSECOND,//62.5   6.81847 1126    9   9
 483*uSECOND, 402*uSECOND, 364*uSECOND, 341*uSECOND, 325*uSECOND, 315*uSECOND, 306*uSECOND, 300*uSECOND, 295*uSECOND, 291*uSECOND, 287*uSECOND, 284*uSECOND, 283*uSECOND,//58.75  7.78905 1238    10  10
 515*uSECOND, 428*uSECOND, 387*uSECOND, 363*uSECOND, 347*uSECOND, 335*uSECOND, 326*uSECOND, 319*uSECOND, 314*uSECOND, 310*uSECOND, 306*uSECOND, 303*uSECOND, 301*uSECOND,//55     8.92491 1358    11  11
 546*uSECOND, 454*uSECOND, 411*uSECOND, 385*uSECOND, 368*uSECOND, 355*uSECOND, 346*uSECOND, 339*uSECOND, 333*uSECOND, 329*uSECOND, 325*uSECOND, 321*uSECOND, 320*uSECOND,//51.25  10.2586 1487    12  12
 577*uSECOND, 480*uSECOND, 434*uSECOND, 407*uSECOND, 389*uSECOND, 376*uSECOND, 366*uSECOND, 358*uSECOND, 352*uSECOND, 347*uSECOND, 343*uSECOND, 340*uSECOND, 338*uSECOND,//47.5   11.8302 1625    13  13
 609*uSECOND, 506*uSECOND, 458*uSECOND, 429*uSECOND, 410*uSECOND, 396*uSECOND, 386*uSECOND, 378*uSECOND, 372*uSECOND, 366*uSECOND, 362*uSECOND, 358*uSECOND, 356*uSECOND,//43.75  13.6886 1770    14  14
 640*uSECOND, 532*uSECOND, 482*uSECOND, 451*uSECOND, 431*uSECOND, 417*uSECOND, 406*uSECOND, 397*uSECOND, 391*uSECOND, 385*uSECOND, 381*uSECOND, 377*uSECOND, 375*uSECOND,//40     15.8944 1921    16  15
 671*uSECOND, 558*uSECOND, 505*uSECOND, 473*uSECOND, 452*uSECOND, 437*uSECOND, 426*uSECOND, 417*uSECOND, 410*uSECOND, 404*uSECOND, 399*uSECOND, 395*uSECOND, 393*uSECOND,//36.25  18.5227 2078    17  16
 703*uSECOND, 584*uSECOND, 529*uSECOND, 495*uSECOND, 473*uSECOND, 457*uSECOND, 446*uSECOND, 436*uSECOND, 429*uSECOND, 423*uSECOND, 418*uSECOND, 414*uSECOND, 411*uSECOND,//32.5   21.6669 2238    18  17
 734*uSECOND, 610*uSECOND, 552*uSECOND, 518*uSECOND, 494*uSECOND, 478*uSECOND, 465*uSECOND, 456*uSECOND, 448*uSECOND, 442*uSECOND, 437*uSECOND, 432*uSECOND, 430*uSECOND,//28.75  25.4437 2399    19  18
 765*uSECOND, 636*uSECOND, 576*uSECOND, 540*uSECOND, 516*uSECOND, 498*uSECOND, 485*uSECOND, 475*uSECOND, 467*uSECOND, 461*uSECOND, 455*uSECOND, 451*uSECOND, 448*uSECOND,//25     30      2561    21  19
 797*uSECOND, 662*uSECOND, 600*uSECOND, 562*uSECOND, 537*uSECOND, 519*uSECOND, 505*uSECOND, 495*uSECOND, 486*uSECOND, 480*uSECOND, 474*uSECOND, 469*uSECOND, 466*uSECOND,//21.25  35.5209 2719    22  20
 828*uSECOND, 688*uSECOND, 623*uSECOND, 584*uSECOND, 558*uSECOND, 539*uSECOND, 525*uSECOND, 514*uSECOND, 506*uSECOND, 498*uSECOND, 492*uSECOND, 487*uSECOND, 485*uSECOND,//17.5   42.2417 2873    23  21
 860*uSECOND, 715*uSECOND, 647*uSECOND, 606*uSECOND, 579*uSECOND, 560*uSECOND, 545*uSECOND, 534*uSECOND, 525*uSECOND, 517*uSECOND, 511*uSECOND, 506*uSECOND, 503*uSECOND,//13.75  50.4624 3020    24  22
 891*uSECOND, 741*uSECOND, 670*uSECOND, 628*uSECOND, 600*uSECOND, 580*uSECOND, 565*uSECOND, 553*uSECOND, 544*uSECOND, 536*uSECOND, 530*uSECOND, 524*uSECOND, 522*uSECOND,//10     60.5676 3158    25  23
 922*uSECOND, 767*uSECOND, 694*uSECOND, 650*uSECOND, 621*uSECOND, 600*uSECOND, 585*uSECOND, 573*uSECOND, 563*uSECOND, 555*uSECOND, 548*uSECOND, 543*uSECOND, 540*uSECOND,//6.25   73.0537 3287    26  24
 954*uSECOND, 793*uSECOND, 718*uSECOND, 672*uSECOND, 642*uSECOND, 621*uSECOND, 605*uSECOND, 592*uSECOND, 582*uSECOND, 574*uSECOND, 567*uSECOND, 561*uSECOND, 558*uSECOND,//2.5    88.5645 3405    27  25
 985*uSECOND, 819*uSECOND, 741*uSECOND, 695*uSECOND, 663*uSECOND, 641*uSECOND, 625*uSECOND, 612*uSECOND, 601*uSECOND, 593*uSECOND, 586*uSECOND, 580*uSECOND, 577*uSECOND,//-1.25  107.940 3511    28  26
1016*uSECOND, 845*uSECOND, 765*uSECOND, 717*uSECOND, 685*uSECOND, 662*uSECOND, 644*uSECOND, 631*uSECOND, 620*uSECOND, 612*uSECOND, 604*uSECOND, 598*uSECOND, 595*uSECOND,//-5     132.286 3606    29  27
1048*uSECOND, 871*uSECOND, 788*uSECOND, 739*uSECOND, 706*uSECOND, 682*uSECOND, 664*uSECOND, 651*uSECOND, 640*uSECOND, 631*uSECOND, 623*uSECOND, 617*uSECOND, 613*uSECOND,//-8.75  163.061 3689    29  28
1079*uSECOND, 897*uSECOND, 812*uSECOND, 761*uSECOND, 727*uSECOND, 702*uSECOND, 684*uSECOND, 670*uSECOND, 659*uSECOND, 649*uSECOND, 642*uSECOND, 635*uSECOND, 632*uSECOND,//-12.5  202.210 3762    30  29
1111*uSECOND, 923*uSECOND, 836*uSECOND, 783*uSECOND, 748*uSECOND, 723*uSECOND, 704*uSECOND, 690*uSECOND, 678*uSECOND, 668*uSECOND, 660*uSECOND, 654*uSECOND, 650*uSECOND,//-16.25 252.340 3824    30  30
1142*uSECOND, 949*uSECOND, 859*uSECOND, 805*uSECOND, 769*uSECOND, 743*uSECOND, 724*uSECOND, 709*uSECOND, 697*uSECOND, 687*uSECOND, 679*uSECOND, 672*uSECOND, 668*uSECOND,//-20    316.971 3876    31  31
*/
};

#if ENCUTTER

CONST WORD TAB_PHASECUT[CPHASEMAX] = {//    RPH1 RPH2 RPH3 RPH4
    CMCL  | CPH1 | CPH2,			    //Step1 H   H   L   L
    CMCL  | CPH1 | CPH4,			    //Step2 H   L   L   H
    CMCL  | CPH1,                       //Step3 H   L   L   L
    CMCL  | CPH3,                       //Step4 L   L   H   L
    CMCL  | 0,                          //Step5 L   L   L   L
    CMCL  | CPH4,                       //Step6 L   L   L   H
    CMCL  | CPH2,                       //Step7 L   H   L   L
    CMCL  | CPH2 | CPH3,                //Step8 L   H   H   L
};

CONST ULONG TAB_CUTTIME[CUTMAXSPEED] = {

  5000* uSECOND, //Start
  3707* uSECOND, //cutter step1
  2291* uSECOND, //cutter step2
  1769* uSECOND, //cutter step3
  1485* uSECOND, //cutter step4
  1302* uSECOND, //cutter step5
  1172* uSECOND, //cutter step6
  1074* uSECOND, //cutter step7
  996 * uSECOND, //cutter step8
  933 * uSECOND, //cutter step9
  880 * uSECOND, //cutter step10
  836 * uSECOND, //cutter step11
  797 * uSECOND, //cutter step12
  763 * uSECOND, //cutter step13
  733 * uSECOND, //cutter step14
  707 * uSECOND, //cutter step15
  683 * uSECOND, //cutter step16
  661 * uSECOND, //cutter step17
  641 * uSECOND, //cutter step18
  623 * uSECOND, //cutter step19
  606 * uSECOND, //cutter step20
  591 * uSECOND, //cutter step21
  577 * uSECOND, //cutter step22
  563 * uSECOND, //cutter step23
  551 * uSECOND, //cutter step24
  539 * uSECOND, //cutter step25
  528 * uSECOND, //cutter step26
  518 * uSECOND, //cutter step27
  508 * uSECOND, //cutter step28
  499 * uSECOND, //cutter step29
  490 * uSECOND, //cutter step30
  482 * uSECOND, //cutter step31
  474 * uSECOND, //cutter step32
  467 * uSECOND, //cutter step33
  459 * uSECOND, //cutter step34
  453 * uSECOND, //cutter step35
  446 * uSECOND, //cutter step36
  440 * uSECOND, //cutter step37
  434 * uSECOND, //cutter step38
  428 * uSECOND, //cutter step39
  422 * uSECOND, //cutter step40
  417 * uSECOND, //cutter step41
  412 * uSECOND, //cutter step42
  407 * uSECOND, //cutter step43
  402 * uSECOND, //cutter step44
  398 * uSECOND, //cutter step45
  393 * uSECOND, //cutter step46
  389 * uSECOND, //cutter step47
  385 * uSECOND, //cutter step48
  381 * uSECOND, //cutter step49
  377 * uSECOND, //cutter step50
  373 * uSECOND, //cutter step51
  369 * uSECOND, //cutter step52
  365 * uSECOND, //cutter step53
  362 * uSECOND, //cutter step54
  359 * uSECOND, //cutter step55
  355 * uSECOND, //cutter step56
  352 * uSECOND, //cutter step57
  349 * uSECOND, //cutter step58
  346 * uSECOND, //cutter step59
  343 * uSECOND, //cutter step60
  340 * uSECOND, //cutter step61
  337 * uSECOND, //cutter step62
  335 * uSECOND, //cutter step63
  332 * uSECOND, //cutter step64
  329 * uSECOND, //cutter step65
  327 * uSECOND, //cutter step66
  324 * uSECOND, //cutter step67
  322 * uSECOND, //cutter step68
  319 * uSECOND, //cutter step69
  317 * uSECOND, //cutter step70
  315 * uSECOND, //cutter step71
  313 * uSECOND //cutter step72
};
#endif

#elif CASE_FTP628MCL401  //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

CONST WORD TAB_PHASEJ[PHASEMAX] = { //    RPH1 RPH2 RPH3 RPH4

	RPH1 | RPH4,
	RPH4,
	RPH2 | RPH4,
	RPH2,
	RPH2 | RPH3,
	RPH3,
	RPH1 | RPH3,
	RPH1,
};
// BYTE TAB_PHASEJ[] = {0x60,0xC0,0x90,0x30};
//HJ 采用CAPD245的步进表
CONST ULONG TAB_PRTIME[73] = {
     ((5000-10)*uSECOND)-1,  //	Step 0   -
	 ((3707-10)*uSECOND)-1,  //	Step 1   270
	 ((2291-10)*uSECOND)-1,  //	Step 2   436
	 ((1769-10)*uSECOND)-1,  //	Step 3   565
	 ((1485-10)*uSECOND)-1,  //	Step 4   673
	 ((1302-10)*uSECOND)-1,  //	Step 5   768
	 ((1172-10)*uSECOND)-1,  //	Step 6   853
	 ((1074-10)*uSECOND)-1,  //	Step 7   931
	 ((996-10)*uSECOND)-1,  //	Step 8   1004
	 ((933-10)*uSECOND)-1,  //	Step 9   1072
	 ((880-10)*uSECOND)-1,  //	Step 10  1136
	 ((836-10)*uSECOND)-1,  //	Step 11  1197
	 ((797-10)*uSECOND)-1,  //	Step 12  1255
	 ((763-10)*uSECOND)-1,  //	Step 13  1310
	 ((733-10)*uSECOND)-1,  //	Step 14  1364
	 ((707-10)*uSECOND)-1,  //	Step 15  1415
	 ((683-10)*uSECOND)-1,  //	Step 16  1465
	 ((661-10)*uSECOND)-1,  //	Step 17  1513
	 ((641-10)*uSECOND)-1,  //	Step 18  1560
	 ((623-10)*uSECOND)-1,  //	Step 19  1605
	 ((606-10)*uSECOND)-1,  //	Step 20  1649
	 ((591-10)*uSECOND)-1,  //	Step 21  1692
	 ((577-10)*uSECOND)-1,  //	Step 22  1734
	 ((563-10)*uSECOND)-1,  //	Step 23  1775
	 ((551-10)*uSECOND)-1,  //	Step 24  1815
	 ((539-10)*uSECOND)-1,  //	Step 25  1854
	 ((528-10)*uSECOND)-1,  //  Step 26  1893
	 ((518-10)*uSECOND)-1,  //	Step 27  1930
	 ((508-10)*uSECOND)-1,  //	Step 28  1967
	 ((499-10)*uSECOND)-1,  //	Step 29  2004
	 ((490-10)*uSECOND)-1,  //	Step 30  2039
	 ((482-10)*uSECOND)-1,  //	Step 31  2075
	 ((474-10)*uSECOND)-1,  //	Step 32  2109
	 ((467-10)*uSECOND)-1,  //	Step 33  2143
	 ((459-10)*uSECOND)-1,  //	Step 34  2176
	 ((453-10)*uSECOND)-1,  //	Step 35  2209
	 ((446-10)*uSECOND)-1,  //	Step 36  2242
	 ((440-10)*uSECOND)-1,  //	Step 37  2274
	 ((434-10)*uSECOND)-1,  //	Step 38  2305
	 ((428-10)*uSECOND)-1,  //	Step 39  2337
	 ((422-10)*uSECOND)-1,  //	Step 40  2367
	 ((417-10)*uSECOND)-1,  //	Step 41  2398
	 ((412-10)*uSECOND)-1,  //	Step 42  2428
	 ((407-10)*uSECOND)-1,  //	Step 43  2457
	 ((402-10)*uSECOND)-1,  //	Step 44  2487
	 ((398-10)*uSECOND)-1,  //	Step 45  2515
	 ((393-10)*uSECOND)-1,  //	Step 46  2544
	 ((389-10)*uSECOND)-1,  //	Step 47  2572
	 ((385-10)*uSECOND)-1,  //	Step 48  2600
	 ((381-10)*uSECOND)-1,  //	Step 49  2628
	 ((377-10)*uSECOND)-1,  //	Step 50  2655
	 ((373-10)*uSECOND)-1,  //	Step 51  2683
	 ((369-10)*uSECOND)-1,  //	Step 52  2709
	 ((365-10)*uSECOND)-1,  //	Step 53  2736
	 ((362-10)*uSECOND)-1,  //	Step 54  2762
	 ((359-10)*uSECOND)-1,  //	Step 55  2788
	 ((355-10)*uSECOND)-1,  //	Step 56  2814
	 ((352-10)*uSECOND)-1,  //	Step 57  2840
	 ((349-10)*uSECOND)-1,  //	Step 58  2865
	 ((346-10)*uSECOND)-1,  //	Step 59  2891
	 ((343-10)*uSECOND)-1,  //	Step 60  2916
	 ((340-10)*uSECOND)-1,  //	Step 61  2940
	 ((337-10)*uSECOND)-1,  //	Step 62  2965
	 ((335-10)*uSECOND)-1,  //	Step 63  2989
	 ((332-10)*uSECOND)-1,  //	Step 64  3013
	 ((329-10)*uSECOND)-1,  //	Step 65  3037
	 ((327-10)*uSECOND)-1,  //	Step 66  3061
	 ((324-10)*uSECOND)-1,  //	Step 67  3085
	 ((322-10)*uSECOND)-1,  //	Step 68  3108
	 ((319-10)*uSECOND)-1,  //	Step 69  3131
	 ((317-10)*uSECOND)-1,  //	Step 70  3154
	 ((315-10)*uSECOND)-1,  //	Step 71  3177
	 ((313-10)*uSECOND)-1,  //	Step 72  3200
};
//HJ 采用CAPD245的加热表
CONST ULONG TABSTROBE24V[32][15] = {
//PPS-400		600			800			1000		    1200		1400			1600		   1800		      2000		     2200		    2400			2600		2800             3000          3200//温度  |Rx     |AD0=(Rx*2^12)/(Rx+18k)
//Drive time for PPS: W=2000000/400
(9 ) *uSECOND, (8 )*uSECOND,(8 ) *uSECOND,(7 ) *uSECOND,(7 ) *uSECOND,(6 ) *uSECOND,(6 ) *uSECOND,(6 )  *uSECOND,(5 )  *uSECOND,(5 )  *uSECOND,(5 )  *uSECOND,(5 )  *uSECOND,(5 )  *uSECOND,(5 )  *uSECOND,(5 )  *uSECOND,//96.25  2.32454 469     4   0
(32 )*uSECOND, (29)*uSECOND,(27 )*uSECOND,(25 )*uSECOND,(24 )*uSECOND,(23 )*uSECOND,(22 )*uSECOND,(21 ) *uSECOND,(20 ) *uSECOND,(19 ) *uSECOND,(18 ) *uSECOND,(18 ) *uSECOND,(17 ) *uSECOND,(17 ) *uSECOND,(16 ) *uSECOND,//92.5   2.59419 516     5   1
(56 )*uSECOND, (51)*uSECOND,(47 )*uSECOND,(44 )*uSECOND,(41 )*uSECOND,(39 )*uSECOND,(37 )*uSECOND,(36 ) *uSECOND,(34 ) *uSECOND,(33 ) *uSECOND,(32 ) *uSECOND,(31 ) *uSECOND,(30 ) *uSECOND,(29 ) *uSECOND,(28 ) *uSECOND,//88.75  2.90172 569     5   2
(79 )*uSECOND, (72)*uSECOND,(67 )*uSECOND,(62 )*uSECOND,(59 )*uSECOND,(56 )*uSECOND,(53 )*uSECOND,(51 ) *uSECOND,(49 ) *uSECOND,(47 ) *uSECOND,(45 ) *uSECOND,(44 ) *uSECOND,(43 ) *uSECOND,(41 ) *uSECOND,(40 ) *uSECOND,//85     3.25334 627     5   3
(102)*uSECOND, (93)*uSECOND,(86 )*uSECOND,(81 )*uSECOND,(76 )*uSECOND,(72 )*uSECOND,(69 )*uSECOND,(66 ) *uSECOND,(63 ) *uSECOND,(61 ) *uSECOND,(59 ) *uSECOND,(57 ) *uSECOND,(55 ) *uSECOND,(54 ) *uSECOND,(52 ) *uSECOND,//81.25  3.65640 692     6   4
(126)*uSECOND,(115)*uSECOND,(106)*uSECOND,(99 )*uSECOND,(93 )*uSECOND,(88 )*uSECOND,(84 )*uSECOND,(81 ) *uSECOND,(77 ) *uSECOND,(75 ) *uSECOND,(72 ) *uSECOND,(70 ) *uSECOND,(68 ) *uSECOND,(66 ) *uSECOND,(64 ) *uSECOND,//77.5   4.11969 763     6   5
(149)*uSECOND,(136)*uSECOND,(126)*uSECOND,(118)*uSECOND,(111)*uSECOND,(105)*uSECOND,(100)*uSECOND,(96 ) *uSECOND,(92 ) *uSECOND,(88 ) *uSECOND,(85 ) *uSECOND,(83 ) *uSECOND,(80 ) *uSECOND,(78 ) *uSECOND,(76 ) *uSECOND,//73.75  4.65367 842     7   6
(172)*uSECOND,(158)*uSECOND,(146)*uSECOND,(136)*uSECOND,(128)*uSECOND,(121)*uSECOND,(116)*uSECOND,(110 )*uSECOND,(106 )*uSECOND,(102 )*uSECOND,(99 ) *uSECOND,(96 ) *uSECOND,(93 ) *uSECOND,(90 ) *uSECOND,(88 ) *uSECOND,//70     5.27088 928     8   7
(196)*uSECOND,(179)*uSECOND,(165)*uSECOND,(155)*uSECOND,(145)*uSECOND,(138)*uSECOND,(131)*uSECOND,(125 )*uSECOND,(120 )*uSECOND,(116 )*uSECOND,(112 )*uSECOND,(109 )*uSECOND,(106 )*uSECOND,(103 )*uSECOND,(100 )*uSECOND,//66.25  5.98642 1023    8   8
(219)*uSECOND,(200)*uSECOND,(185)*uSECOND,(173)*uSECOND,(163)*uSECOND,(154)*uSECOND,(147)*uSECOND,(140 )*uSECOND,(135 )*uSECOND,(130 )*uSECOND,(126 )*uSECOND,(122 )*uSECOND,(118 )*uSECOND,(115 )*uSECOND,(112 )*uSECOND,//62.5   6.81847 1126    9   9
(242)*uSECOND,(222)*uSECOND,(205)*uSECOND,(191)*uSECOND,(180)*uSECOND,(171)*uSECOND,(162)*uSECOND,(155 )*uSECOND,(149 )*uSECOND,(144 )*uSECOND,(139 )*uSECOND,(135 )*uSECOND,(131 )*uSECOND,(127 )*uSECOND,(124 )*uSECOND,//58.75  7.78905 1238    10  10
(266)*uSECOND,(243)*uSECOND,(225)*uSECOND,(210)*uSECOND,(197)*uSECOND,(187)*uSECOND,(178)*uSECOND,(170 )*uSECOND,(164 )*uSECOND,(158 )*uSECOND,(152 )*uSECOND,(148 )*uSECOND,(143 )*uSECOND,(140 )*uSECOND,(136 )*uSECOND,//55     8.92491 1358    11  11
(289)*uSECOND,(264)*uSECOND,(244)*uSECOND,(228)*uSECOND,(215)*uSECOND,(203)*uSECOND,(194)*uSECOND,(185 )*uSECOND,(178 )*uSECOND,(172 )*uSECOND,(166 )*uSECOND,(161 )*uSECOND,(156 )*uSECOND,(152 )*uSECOND,(148 )*uSECOND,//51.25  10.2586 1487    12  12
(312)*uSECOND,(286)*uSECOND,(264)*uSECOND,(247)*uSECOND,(232)*uSECOND,(220)*uSECOND,(209)*uSECOND,(200 )*uSECOND,(192 )*uSECOND,(185 )*uSECOND,(179 )*uSECOND,(174 )*uSECOND,(169 )*uSECOND,(164 )*uSECOND,(160 )*uSECOND,//47.5   11.8302 1625    13  13
(336)*uSECOND,(307)*uSECOND,(284)*uSECOND,(265)*uSECOND,(250)*uSECOND,(236)*uSECOND,(225)*uSECOND,(215 )*uSECOND,(207 )*uSECOND,(199 )*uSECOND,(193 )*uSECOND,(187 )*uSECOND,(181 )*uSECOND,(176 )*uSECOND,(172 )*uSECOND,//43.75  13.6886 1770    14  14
(359)*uSECOND,(328)*uSECOND,(304)*uSECOND,(284)*uSECOND,(267)*uSECOND,(253)*uSECOND,(241)*uSECOND,(230 )*uSECOND,(221 )*uSECOND,(213 )*uSECOND,(206 )*uSECOND,(200 )*uSECOND,(194 )*uSECOND,(189 )*uSECOND,(184 )*uSECOND,//40     15.8944 1921    16  15
(383)*uSECOND,(350)*uSECOND,(323)*uSECOND,(302)*uSECOND,(284)*uSECOND,(269)*uSECOND,(256)*uSECOND,(245 )*uSECOND,(235 )*uSECOND,(227 )*uSECOND,(219 )*uSECOND,(212 )*uSECOND,(206 )*uSECOND,(201 )*uSECOND,(196 )*uSECOND,//36.25  18.5227 2078    17  16
(406)*uSECOND,(371)*uSECOND,(343)*uSECOND,(320)*uSECOND,(302)*uSECOND,(286)*uSECOND,(272)*uSECOND,(260 )*uSECOND,(250 )*uSECOND,(241 )*uSECOND,(233 )*uSECOND,(225 )*uSECOND,(219 )*uSECOND,(213 )*uSECOND,(208 )*uSECOND,//32.5   21.6669 2238    18  17
(429)*uSECOND,(392)*uSECOND,(363)*uSECOND,(339)*uSECOND,(319)*uSECOND,(302)*uSECOND,(288)*uSECOND,(275 )*uSECOND,(264 )*uSECOND,(255 )*uSECOND,(246 )*uSECOND,(238 )*uSECOND,(232 )*uSECOND,(225 )*uSECOND,(220 )*uSECOND,//28.75  25.4437 2399    19  18
(453)*uSECOND,(414)*uSECOND,(383)*uSECOND,(357)*uSECOND,(336)*uSECOND,(319)*uSECOND,(303)*uSECOND,(290 )*uSECOND,(279 )*uSECOND,(268 )*uSECOND,(259 )*uSECOND,(251 )*uSECOND,(244 )*uSECOND,(238 )*uSECOND,(232 )*uSECOND,//25     30      2561    21  19
(476)*uSECOND,(435)*uSECOND,(402)*uSECOND,(376)*uSECOND,(354)*uSECOND,(335)*uSECOND,(319)*uSECOND,(305 )*uSECOND,(293 )*uSECOND,(282 )*uSECOND,(273 )*uSECOND,(264 )*uSECOND,(257 )*uSECOND,(250 )*uSECOND,(244 )*uSECOND,//21.25  35.5209 2719    22  20
(499)*uSECOND,(456)*uSECOND,(422)*uSECOND,(394)*uSECOND,(371)*uSECOND,(351)*uSECOND,(335)*uSECOND,(320 )*uSECOND,(307 )*uSECOND,(296 )*uSECOND,(286 )*uSECOND,(277 )*uSECOND,(269 )*uSECOND,(262 )*uSECOND,(256 )*uSECOND,//17.5   42.2417 2873    23  21
(523)*uSECOND,(478)*uSECOND,(442)*uSECOND,(413)*uSECOND,(388)*uSECOND,(368)*uSECOND,(350)*uSECOND,(335 )*uSECOND,(322 )*uSECOND,(310 )*uSECOND,(300 )*uSECOND,(290 )*uSECOND,(282 )*uSECOND,(274 )*uSECOND,(268 )*uSECOND,//13.75  50.4624 3020    24  22
(546)*uSECOND,(499)*uSECOND,(462)*uSECOND,(431)*uSECOND,(406)*uSECOND,(384)*uSECOND,(366)*uSECOND,(350 )*uSECOND,(336 )*uSECOND,(324 )*uSECOND,(313 )*uSECOND,(303 )*uSECOND,(295 )*uSECOND,(287 )*uSECOND,(279 )*uSECOND,//10     60.5676 3158    25  23
(569)*uSECOND,(520)*uSECOND,(481)*uSECOND,(450)*uSECOND,(423)*uSECOND,(401)*uSECOND,(382)*uSECOND,(365 )*uSECOND,(351 )*uSECOND,(338 )*uSECOND,(326 )*uSECOND,(316 )*uSECOND,(307 )*uSECOND,(299 )*uSECOND,(291 )*uSECOND,//6.25   73.0537 3287    26  24
(593)*uSECOND,(542)*uSECOND,(501)*uSECOND,(468)*uSECOND,(440)*uSECOND,(417)*uSECOND,(397)*uSECOND,(380 )*uSECOND,(365 )*uSECOND,(352 )*uSECOND,(340 )*uSECOND,(329 )*uSECOND,(320 )*uSECOND,(311 )*uSECOND,(303 )*uSECOND,//2.5    88.5645 3405    27  25
(608)*uSECOND,(556)*uSECOND,(514)*uSECOND,(480)*uSECOND,(452)*uSECOND,(428)*uSECOND,(408)*uSECOND,(390 )*uSECOND,(374 )*uSECOND,(361 )*uSECOND,(349 )*uSECOND,(338 )*uSECOND,(328 )*uSECOND,(319 )*uSECOND,(311 )*uSECOND,//-1.25  107.940 3511    28  26
(671)*uSECOND,(613)*uSECOND,(567)*uSECOND,(529)*uSECOND,(498)*uSECOND,(472)*uSECOND,(449)*uSECOND,(430 )*uSECOND,(413 )*uSECOND,(398 )*uSECOND,(384 )*uSECOND,(372 )*uSECOND,(362 )*uSECOND,(352 )*uSECOND,(343 )*uSECOND,//-5     132.286 3606    29  27
(671)*uSECOND,(613)*uSECOND,(567)*uSECOND,(529)*uSECOND,(498)*uSECOND,(472)*uSECOND,(449)*uSECOND,(430 )*uSECOND,(413 )*uSECOND,(398 )*uSECOND,(384 )*uSECOND,(372 )*uSECOND,(362 )*uSECOND,(352 )*uSECOND,(343 )*uSECOND,//-8.75  163.061 3689    29  28
(671)*uSECOND,(613)*uSECOND,(567)*uSECOND,(529)*uSECOND,(498)*uSECOND,(472)*uSECOND,(449)*uSECOND,(430 )*uSECOND,(413 )*uSECOND,(398 )*uSECOND,(384 )*uSECOND,(372 )*uSECOND,(362 )*uSECOND,(352 )*uSECOND,(343 )*uSECOND,//-12.5  202.210 3762    30  29
(671)*uSECOND,(613)*uSECOND,(567)*uSECOND,(529)*uSECOND,(498)*uSECOND,(472)*uSECOND,(449)*uSECOND,(430 )*uSECOND,(413 )*uSECOND,(398 )*uSECOND,(384 )*uSECOND,(372 )*uSECOND,(362 )*uSECOND,(352 )*uSECOND,(343 )*uSECOND,//-16.25 252.340 3824    30  30
(671)*uSECOND,(613)*uSECOND,(567)*uSECOND,(529)*uSECOND,(498)*uSECOND,(472)*uSECOND,(449)*uSECOND,(430 )*uSECOND,(413 )*uSECOND,(398 )*uSECOND,(384 )*uSECOND,(372 )*uSECOND,(362 )*uSECOND,(352 )*uSECOND,(343 )*uSECOND,//-20    316.971 3876    31  31
};

#if ENCUTTER

CONST WORD TAB_PHASECUT[CPHASEMAX] = {//    RPH1 RPH2 RPH3 RPH4
	CPH1,
	CPH1 | CPH3,
	CPH3,
	CPH2 | CPH3,
	CPH2,
	CPH2 | CPH4,
	CPH4,
	CPH1 | CPH4,

/*
	CPH1 | CPH4,
	CPH4,
	CPH2 | CPH4,
	CPH2,
	CPH2 | CPH3,
	CPH3,
	CPH1 | CPH3,
	CPH1,
*/
};
//HJ 采用CAPD245的步进表
CONST ULONG TAB_CUTTIME[73] = {
  5000* uSECOND, //Start
  3707* uSECOND, //cutter step1
  2291* uSECOND, //cutter step2
  1769* uSECOND, //cutter step3
  1485* uSECOND, //cutter step4
  1302* uSECOND, //cutter step5
  1172* uSECOND, //cutter step6
  1074* uSECOND, //cutter step7
  996 * uSECOND, //cutter step8
  933 * uSECOND, //cutter step9
  880 * uSECOND, //cutter step10
  836 * uSECOND, //cutter step11
  797 * uSECOND, //cutter step12
  763 * uSECOND, //cutter step13
  733 * uSECOND, //cutter step14
  707 * uSECOND, //cutter step15
  683 * uSECOND, //cutter step16
  661 * uSECOND, //cutter step17
  641 * uSECOND, //cutter step18
  623 * uSECOND, //cutter step19
  606 * uSECOND, //cutter step20
  591 * uSECOND, //cutter step21
  577 * uSECOND, //cutter step22
  563 * uSECOND, //cutter step23
  551 * uSECOND, //cutter step24
  539 * uSECOND, //cutter step25
  528 * uSECOND, //cutter step26
  518 * uSECOND, //cutter step27
  508 * uSECOND, //cutter step28
  499 * uSECOND, //cutter step29
  490 * uSECOND, //cutter step30
  482 * uSECOND, //cutter step31
  474 * uSECOND, //cutter step32
  467 * uSECOND, //cutter step33
  459 * uSECOND, //cutter step34
  453 * uSECOND, //cutter step35
  446 * uSECOND, //cutter step36
  440 * uSECOND, //cutter step37
  434 * uSECOND, //cutter step38
  428 * uSECOND, //cutter step39
  422 * uSECOND, //cutter step40
  417 * uSECOND, //cutter step41
  412 * uSECOND, //cutter step42
  407 * uSECOND, //cutter step43
  402 * uSECOND, //cutter step44
  398 * uSECOND, //cutter step45
  393 * uSECOND, //cutter step46
  389 * uSECOND, //cutter step47
  385 * uSECOND, //cutter step48
  381 * uSECOND, //cutter step49
  377 * uSECOND, //cutter step50
  373 * uSECOND, //cutter step51
  369 * uSECOND, //cutter step52
  365 * uSECOND, //cutter step53
  362 * uSECOND, //cutter step54
  359 * uSECOND, //cutter step55
  355 * uSECOND, //cutter step56
  352 * uSECOND, //cutter step57
  349 * uSECOND, //cutter step58
  346 * uSECOND, //cutter step59
  343 * uSECOND, //cutter step60
  340 * uSECOND, //cutter step61
  337 * uSECOND, //cutter step62
  335 * uSECOND, //cutter step63
  332 * uSECOND, //cutter step64
  329 * uSECOND, //cutter step65
  327 * uSECOND, //cutter step66
  324 * uSECOND, //cutter step67
  322 * uSECOND, //cutter step68
  319 * uSECOND, //cutter step69
  317 * uSECOND, //cutter step70
  315 * uSECOND, //cutter step71
  313 * uSECOND //cutter step72
};
#endif
#endif
#endif

