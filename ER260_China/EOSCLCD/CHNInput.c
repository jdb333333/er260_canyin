//---------------------------------------------------------------------------

#include "king.h"
#include "exthead.h"
#include "exth.h"


extern CONST WORD code_buf[6768];


/****************************************************************
  将汉字输入法进行如下修改
  --------------------------
  1即2机3记4几5级
  6及7集8既9鸡0己
  拼:ji
  ***************************************************************/

#define EN_QUWEI    0   //ccr2016-07-04是否允许区位输入法
#define USE_X_ONLY  1   //ccr2016-07-04使用'X'切换输入法,不使用shift(cCHANGEASC)改变字母大小写

//---------------------------------------------------------------------------
#if defined(CASE_FORHANZI)

#define  PYorBH   1		// =1使用拼音输入法，=0 使用笔画输入法

char HZFound[DISPMAX*2];//存放找到的重码汉字

CONST MODEREC INPUTMODE[3]={
    {cInputMode1,1},//A
    {cInputMode2,4},//qu
#if (PYorBH)
    {cInputMode3,6},//ping
#else
    {cInputMode4,6},//5bihua
#endif
};


#define cPYCODE     26
#if (PYorBH)
CONST MODEREC PYCODE[cPYCODE]={
    {"iu",  'q' & 0x1f},
    {"ua",  'w' & 0x1f},
    {"ia",  'w' & 0x1f},
    {"uan", 'r' & 0x1f},
    {"ue",  't' & 0x1f},
    {"uai", 'y' & 0x1f},
    {"ing", 'y' & 0x1f},
    {"uo",  'o' & 0x1f},
    {"un",  'p' & 0x1f},
    {"iong",'s' & 0x1f},
    {"ong", 's' & 0x1f},
    {"uang",'d' & 0x1f},
    {"iang",'d' & 0x1f},
    {"en",  'f' & 0x1f},
    {"eng", 'g' & 0x1f},
    {"ang", 'h' & 0x1f},
    {"an",  'j' & 0x1f},
    {"ao",  'k' & 0x1f},
    {"ai",  'l' & 0x1f},
    {"ei",  'z' & 0x1f},
    {"ie",  'x' & 0x1f},
    {"iao", 'c' & 0x1f},
    {"ui",  'v' & 0x1f},
    {"ou",  'b' & 0x1f},
    {"in",  'n' & 0x1f},
    {"ian", 'm' & 0x1f},
};
#else
WORD   SkipBihua;   // 笔画输入法的屏蔽值  //
#endif

char    SelMax;
char    SelIdx;
short     HZPage,ScanIdx;

char    QPCode[7];
char    SPCode[2];
short   ScanDir;


#endif

#if EN_EXASCII //ccr2014-03-26>>>>>>
char    EXAsc_Buf[4];  //保存输入的扩展ASCII字符
#endif//<<<<<<

BYTE    InputNum;   // code number of input
BYTE    ASCIIMode; //ccr2014-03-28,保存当前ASCII输入方式：UPASCII，LOASCII，SPASCII，EXASCII，分别表示大写ASCII，小写ASCII，特殊ASCII字符，扩展ASCII字符

WORD    HANZI;      //0-Empty input; <0xa0,ASCII input; >0xa0a0, HANZI input
BYTE    CHNMode;    //0-ACSII mode,1-Quwei Mode 2-Pyin mode

char    SaveSTLine[SCREENWD];//

//==============================================================================
//get the input key from input area
char GetInputKey(short idx)
{
    return(ApplVar.ScreenMap[SCREENLN][idx+INPUTPOS]);
}

/**
 *
 *
 * @author EutronSoftware (2016-02-17)
 *
 * @param keyno :按键位置码
 *
 * @return WORD :=0x01xx时,按键位置无对应字符,xx中为按键位置码
 *
 */
WORD GetNumric(BYTE keyno)
{
    //ccr20131223>>>>>

    BYTE i;

    if (keyno>=64)
        return(0x0100+keyno);

#if (defined(CASE_FORHANZI) || EN_EXASCII)
    if (CHNMode==QUWEI_MODE || ASCIIMode==EXASCII)
    {
        i = NUMASCII[keyno];
        if (i>='0' && i<='9')
            return i;
    }
#endif

    if (ASCIIMode==SPASCII)
        i = NUMASCII[keyno];
    else
    {
        i = ASCIIKEY[keyno];
        if (ASCIIMode==UPASCII && i>='a' && i<='z')
            i &=(~0x20);
    }

    if (i==0)
        return(0x0100+keyno);//按键位置无对应字符
    else
        return i;
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}

#if defined(CASE_FORHANZI)
//恢复被汉字重码覆盖的屏幕区的内容

//..............................................................................
WORD QWInput(BYTE keyno)
{
    char sCh;
    WORD sQWCode;
    short i;
    char x=DISPPOSX,y=DISPPOSY;//重码汉字显示位置

    sCh = GetNumric(keyno);
    if (sCh>='0' && sCh<='9')
    {
        if (TESTBIT(ApplVar.ScreenStat,HZSELECT))
            ChnInput(2);
        else if (InputNum>=INPUTMODE[CHNMode].MaxInput)
            return 0;

        DispCharXY(sCh,INPUTPOS+InputNum,STATELINE);
        QPCode[InputNum] = sCh;
        InputNum++;

        QPCode[InputNum] = 0;
        HZPage = 0;
        //keyno = cNEXT;
        keyno = cDOWN;
    }
    switch (keyno)
    {
    case cUP:
        if (InputNum<=1)
            return 0;
        HZPage-=2;
        if (HZPage<0)
        {
            HZPage = 1;
            return 0;
        }
    case cDOWN:
        if (InputNum==0)
            return 0;
        RESETBIT(ApplVar.ScreenStat,HZSELECT);
        sQWCode = 0;
        for (i=0;i<InputNum;i++)
            sQWCode = sQWCode*10 + (QPCode[i] & 0x0f);
        switch (InputNum)
        {
        case 2:
        case 3:
            if (InputNum==3)
            {
                sCh = (sQWCode % 10) * 10 + 0xa1;
                sQWCode = (sQWCode/10 + 0xa0);
            }
            else
            {
                sCh = 0xa1;
                sQWCode = sQWCode+0xa0;
            }
            if (HZPage<2 && InputNum==3 || HZPage<10 && InputNum==2)
            {//重码汉字显示两行,满行显示5个

                //保护屏幕内容
                BackupScreen(1);
                DispStrXY(Msg[SELECT1HZ].str,0,0);
                memset(HZFound,' ',sizeof(HZFound));
                for (i=0;i<DISPMAX;i++)
                {//显示重码汉字
                    if (i==DISPMAX/2)
                    {//显示第二行
                        x=DISPPOSX;y=DISPPOSY+1;
                    }
                    sQWCode = (sQWCode & 0xff) + ((HZPage * DISPMAX + i + sCh & 0xff)<<8);
                    if ((sQWCode & 0xff00)>0xfe00)
                    {//不是合法的汉字
                        break;
                    }
                    else
                    {//显示汉字
                        DispCharXY(' ',x,y);x++;
                        DispCharXY(sQWCode,x,y);x+=2;
                        CWORD(HZFound[i*2])=sQWCode;
                    }
                }
                HZPage++;
                break;
            }
            return 0;
        case 4:
            RecallScreen();
            SETBIT(ApplVar.ScreenStat,HZSELECT);
            return((sQWCode /100 + 0xa0)+((sQWCode % 100 + 0xa0)<<8));
        default:
            i = 0;
            break;
        }
        SelMax = i;
        for (;i<DISPMAX;i++)
        {
            if (i==DISPMAX/2)
            {//显示第二行
                x=DISPPOSX;y=DISPPOSY+1;
            }
            DispCharXY(' ',x,y);x++;
            DispCharXY(' ',x,y);x++;
            DispCharXY(' ',x,y);x++;
        }
        DispCharXY('>',DISPPOSX,DISPPOSY);
        SelIdx = 0;
        break;
    }
    return 0;
}
//..............................................................................
#if (PYorBH) //  拼音输入处理  程序
//Change QPyin code into SPCode
void QP2SP()
{
    WORD sP;
    short i;

    SPCode[1] = 0;
    if (InputNum==1)
    {
        SPCode[0] = QPCode[0]  & 0x1f;
        return;
    }
    else
    {
        sP = *((WORD *)QPCode);
        if (sP == 0x687a)       //'zh'
        {
            SPCode[0] = 'v';
            sP = 2;
        }
        else if (sP == 0x6863 ) //'ch'
        {
            SPCode[0] = 'i';
            sP = 2;
        }
        else if (sP == 0x6873)  //'sh'
        {
            SPCode[0] = 'u';
            sP = 2;
        }
        else
        {
            SPCode[0] = QPCode[0];
            sP = 1;
        }
        SPCode[0] &= 0x1f;
        if (sP==InputNum)
            return;

        if (InputNum==2)
            for (i=0;i<cPYCODE;i++)
                if (!strcmp(QPCode,PYCODE[i].Name))
                {
                    SPCode[0] = PYCODE[i].MaxInput;
                    return;
                }

        if (sP+1==InputNum)
        {
            SPCode[1] = QPCode[InputNum-1] & 0x1f;
            return;
        }
        for (i=0;i<cPYCODE;i++)
            if (!strcmp(QPCode+sP,PYCODE[i].Name))
            {
                SPCode[1] = PYCODE[i].MaxInput;
                return;
            }
        SPCode[1] = 0xff;
        return;
    }
}

WORD PYInput(BYTE keyno)
{
    WORD sCh;
    WORD sQWCode;
    char* code;
    short i;
    char x=DISPPOSX,y=DISPPOSY;//重码汉字显示位置

    sCh = GetNumric(keyno);
    if (sCh<0x0100)
    {//按键为字符或数字
        if (sCh<='9')
        {
            if (sCh>='0' && sCh<=DISPMAX+'0')
            {
                sCh = sCh - '0';
                if (sCh<SelMax)
                {
                    SETBIT(ApplVar.ScreenStat,HZSELECT);

                    if (sCh>=DISPMAX/2)
                    {//挑选第2行的重码汉字
                        x = (sCh-DISPMAX/2)*3+DISPPOSX+1;
                        y=DISPPOSY+1;
                    }
                    else
                    {//挑选第1行的重码汉字
                        x = sCh*3+DISPPOSX+1;
                        y=DISPPOSY;
                    }
                    sCh=CWORD(HZFound[sCh*2]); //GetChFr(x,y);
                    RecallScreen();
                    return sCh;
                }
                else
                    return 0;
            }
            else
                return 0;
        }
        else
            if (sCh=='@' || sCh=='~')
            return sCh;

        if (TESTBIT(ApplVar.ScreenStat,HZSELECT))
            ChnInput(2);
        else if (InputNum>=INPUTMODE[CHNMode].MaxInput)
            return 0;

        sCh |= 0x20;
        DispCharXY(sCh,INPUTPOS+InputNum,STATELINE);
        QPCode[InputNum] = sCh;
        InputNum++;
        QPCode[InputNum] = 0;
        QP2SP();
        ScanDir = 1;
        HZPage = ScanIdx = 0;
        //keyno = cNEXT;
        keyno = cDOWN;
    }
    switch (keyno)
    {
    case cUP:
        if (ScanDir==1)
            HZPage = ScanIdx;
        if (HZPage<0)
            return 0;
        ScanDir = -1;
    case cDOWN:
        if (InputNum==0  ||  HZPage>=6768)
            return 0;
        //if (keyno==cNEXT)
        if (keyno == cDOWN)
        {
            if (ScanDir==-1)
                HZPage = ScanIdx;
            ScanDir = 1;
        }
        if (HZPage<0)
            HZPage = 0;
        RESETBIT(ApplVar.ScreenStat,HZSELECT);

        //保护屏幕内容
        BackupScreen(1);
        DispStrXY(Msg[SELECT1HZ].str,0,0);
        memset(HZFound,' ',sizeof(HZFound));
        for (i=0;i<DISPMAX;i++)
        {
            if (i==DISPMAX/2)
            {
                x=DISPPOSX;y=DISPPOSY+1;
            }
            for (;(HZPage>=0 && HZPage<6768);)
            {
                code = (char*)code_buf + HZPage*2;
                if ((code[0]&0x1f) == SPCode[0] &&
                    ((code[1]&0x1f) == SPCode[1] || !SPCode[1]))
                {
                    sQWCode = (HZPage/94+176) + ((HZPage % 94+161)<<8);
                    DispCharXY(i+'0',x++,y);
                    DispCharXY(sQWCode,x,y);x+=2;
                    CWORD(HZFound[i*2])=sQWCode;

                    if (!i)
                        ScanIdx = HZPage -  ScanDir;
                    break;
                }
                HZPage +=ScanDir;
            }
            if (HZPage<0 || HZPage>=6768)
                break;
            HZPage += ScanDir;
        }    //cctest050819

        if (i)
        {
            SelMax = i;
            for (;i<DISPMAX;i++)
            {//清除重码汉字区无汉字显示区域
                if (i==DISPMAX/2)
                {
                    x=DISPPOSX;y=DISPPOSY+1;
                }
                DispCharXY(' ',x++,y);
                DispCharXY(' ',x++,y);
                DispCharXY(' ',x++,y);
            }
            SelIdx = 0;
        }
        break;
    }
    return 0;
}

#else//  笔画输入处理  程序//

void QP2SP()
{
    WORD sP;
    short i;

    SkipBihua = sP = 0;
    for (i=InputNum-1;i>=0;i--)
    {
        sP <<= 3;
        SkipBihua <<= 3;
        sP |= (QPCode[i] & 0x07);
        SkipBihua |= 7;
    }

    CWORD(SPCode) = sP;
}

WORD PYInput(BYTE keyno)
{
    WORD sCh;
    WORD sQWCode;
    char* code;
    short i;
    char x=DISPPOSX,y=DISPPOSY;

    sCh = GetNumric(keyno);
    if (sCh<0x0100)
    {
        if (sCh<=DISPMAX+'0' && InputNum>=DISPMAX &&  !TESTBIT(ApplVar.ScreenStat,HZSELECT))
        {
            if (sCh>='0' && sCh<=DISPMAX+'0')
            {
                sCh = sCh - '0';
                if (sCh<SelMax)
                {
                    if (sCh>=DISPMAX/2)
                    {//挑选第2行的重码汉字
                        x = (sCh-DISPMAX/2)*3+DISPPOSX+1
                        y=DISPPOSY+1;
                    }
                    else
                    {//挑选第1行的重码汉字
                        x = sCh*3+DISPPOSX+1
                        y=DISPPOSY;
                    }

                    SETBIT(ApplVar.ScreenStat,HZSELECT);
                    sCh=CWORD(HZFound[sCh*2]); //GetChFr(x,y);
                    RecallScreen();
                    return sCh;
                }
                else
                    return 0;
            }
            else
                return 0;
        }
        else
        {
            if (sCh=='@' || sCh=='~' || (sCh>='A' && sCh<='Z') || (sCh>='a' && sCh<='z'))
            {
                RecallScreen();
                return sCh;
            }
        }

        if (TESTBIT(ApplVar.ScreenStat,HZSELECT))
            ChnInput(2);
        else if (InputNum>=INPUTMODE[CHNMode].MaxInput)
            return 0;

        sCh |= 0x20;
        DispCharXY(sCh,INPUTPOS+InputNum,STATELINE);
        QPCode[InputNum] = sCh;
        InputNum++;
        QPCode[InputNum] = 0;
        QP2SP();
        ScanDir = 1;
        HZPage = ScanIdx = 0;
        //keyno = cNEXT;
        keyno = cDOWN;
    }
    switch (keyno)
    {
    case cUP:
        if (ScanDir==1)
            HZPage = ScanIdx;
        if (HZPage<0)
            return 0;
        ScanDir = -1;
    case cDOWN:
        if (InputNum==0  ||  HZPage>=6768)
            return 0;
        //if (keyno==cNEXT)
        if (keyno==cDOWN)
        {
            if (ScanDir==-1)
                HZPage = ScanIdx;
            ScanDir = 1;
        }
        if (HZPage<0)
            HZPage = 0;
        RESETBIT(ApplVar.ScreenStat,HZSELECT);
        BackupScreen(1);
        DispStrXY(Msg[SELECT1HZ].str,0,0);
        memset(HZFound,' ',sizeof(HZFound));
        for (i=0;i<DISPMAX;i++)
        {
            if (i==DISPMAX/2)
            {
                x=DISPPOSX;y=DISPPOSY+1;
            }
            for (;(HZPage>=0 && HZPage<6768);)
            {
                code = (char*)code_buf + HZPage*2;
                sQWCode = ((WORD)code[1] * 256 + code[0]) & SkipBihua;
                if (sQWCode == CWORD(SPCode))
                {
                    sQWCode = (HZPage/94+176) + ((HZPage % 94+161)<<8);
                    DispCharXY(i+'0',x++,y);
                    DispCharXY(sQWCode,x,y);x+=2;
                    CWORD(HZFund[i*2])=sQWCode;

                    if (!i)
                        ScanIdx = HZPage -  ScanDir;
                    break;
                }
                HZPage +=ScanDir;
            }
            if (HZPage<0 || HZPage>=6768)
                break;
            HZPage += ScanDir;
        }

        if (i)
        {
            SelMax = i;
            for (;i<DISPMAX;i++)
            {
                if (i==DISPMAX/2)
                {
                    x=DISPPOSX;y=DISPPOSY+1;
                }
                DispCharXY(' ',x++,y);
                DispCharXY(' ',x++,y);
                DispCharXY(' ',x++,y);
            }
            SelIdx = 0;
        }
        break;
    }
    return 0;
}
#endif

#endif

#if EN_EXASCII //ccr2014-03-26>>>>>>
/**
 * 扩展ASCII输入处理:用来通过输入3为数字输入ASCII字符
 *
 * @author EutronSoftware (2014-03-28)
 *
 * @param keyno
 *
 * @return WORD
 */
BYTE InputEXASCII(BYTE keyno)
{
    char sCh;
    WORD sEXASCII;
    char x=DISPPOSX,y=DISPPOSY;//重码汉字显示位置

    sCh = GetNumric(keyno);
    if (sCh>='0' && sCh<='9')
    {
        if (InputNum>=3)
            InputNum = 0;

        DispCharXY(sCh,INPUTPOS+InputNum,STATELINE);
        EXAsc_Buf[InputNum] = sCh;
        InputNum++;

        EXAsc_Buf[InputNum] = 0;
        if (InputNum==3)
        {
            sEXASCII = atoi(EXAsc_Buf);

            EXAsc_Buf[0]=EXAsc_Buf[1]=EXAsc_Buf[2]=' ';
            EXAsc_Buf[3]=0;
            DispStrXY(EXAsc_Buf,INPUTPOS,STATELINE);//清除所输入的内容
            InputNum = 0;

            if (sEXASCII<256)
                return sEXASCII;
        }
        return 0;
    }
    else
        return sCh;
}
#endif//<<<<<<<

/*process user input*****************************************
return:
    0－没有输入  //
    0x01xx－低字节xx为按键位置码 //
    0x00xx－低字节xx为ASCII字符 //
    >0xa0a0－为汉字 //
    '00'－为双00 //
*************************************************************/
WORD CheckInput()
{
#if defined(CASE_FORHANZI)
    static BYTE LastMode=0xff;//ccr2015-10-08 实现中英文之间的快速切换
#endif
    WORD W;
    BYTE keyno;
//    char x=DISPPOSX,y=DISPPOSY;

    if (!KbHit())
        return 0;
    ApplVar.KeyNo = keyno = Getch();        /* read key */
    if (TESTBIT(ApplVar.ScreenStat,HZINPUTMODE))
    {
        switch (GetFirmkeyID(keyno))
        {

#if defined(CASE_FORHANZI)
        case ID_CLEAR:
            if (InputNum>0)
            {
                ChnInput(2);
                return 0;
            }
            else
            {
                ChnInput(3);    //ccr2016-05-31
                RecallScreen(); //ccr2016-05-31
                return (0x0100+keyno);
            }
        case ID_SELECT:

            if (CHNMode==ASC_MODE)
                return(0x0100+keyno);
            if (TESTBIT(ApplVar.ScreenStat,HZSELECT))
            {//已经挑选了一个汉字,再按选择键,继续挑选
                BackupScreen(1);
                DispStrXY(Msg[SELECT1HZ].str,0,0);
                for (keyno=0;keyno<DISPMAX;keyno++)
                {
                    if (HZFound[keyno*2]!=' ')
                    {
                        if (keyno<DISPMAX/2)
                        {
                            DispCharXY(keyno+'0',keyno * 3 + DISPPOSX,DISPPOSY);
                            DispCharXY(CWORD(HZFound[keyno*2]),keyno * 3 + DISPPOSX+1,DISPPOSY);
                        }
                        else
                        {
                            DispCharXY(keyno+'0',(keyno-DISPMAX/2) * 3 + DISPPOSX,DISPPOSY+1);
                            DispCharXY(CWORD(HZFound[keyno*2]),(keyno-DISPMAX/2) * 3 + DISPPOSX+1,DISPPOSY+1);
                        }
                    }
                }
                RESETBIT(ApplVar.ScreenStat,HZSELECT);
            }
            else
            {//选择新的汉字
                if (SelIdx<SelMax)
                {
                    if (CHNMode<=QUWEI_MODE)
                        keyno = ' ';
                    else
                        keyno = SelIdx+'0';
                    if (SelIdx<DISPMAX/2)
                        DispCharXY(keyno,SelIdx * 3 + DISPPOSX,DISPPOSY);
                    else
                        DispCharXY(keyno,(SelIdx-DISPMAX/2) * 3 + DISPPOSX,DISPPOSY+1);
                }
                SelIdx++;
            }
            if (SelIdx>=SelMax)
                SelIdx = 0;

            if (SelIdx<DISPMAX/2)
                DispCharXY('>',SelIdx * 3 + DISPPOSX,DISPPOSY);
            else
                DispCharXY('>',(SelIdx-DISPMAX/2) * 3 + DISPPOSX,DISPPOSY+1);

            return 0;
        case ID_DELETE:
            if (InputNum>0)
            {
                InputNum--;
                QPCode[InputNum] = 0;
                DispCharXY(' ',InputNum + INPUTPOS,STATELINE);
                ScanDir = 1;
                HZPage = ScanIdx = 0;
                if (InputNum>0)
                    switch (CHNMode)
                    {
                    case QUWEI_MODE:
                        //QWInput(cNEXT);
                        QWInput(cDOWN);
                        break;
                    case PYBH_MODE:
                        QP2SP();
                        //PYInput(cNEXT);
                        PYInput(cDOWN);
                        break;
                    }
                return 0;
            }
            else
            {
                RecallScreen();
                return(0x0100+keyno);
            }

        case ID_ENTER:
            if (SelIdx<SelMax && !TESTBIT(ApplVar.ScreenStat,HZSELECT))
            {//没有挑选汉字时,挑选一个汉字作为输入
                SETBIT(ApplVar.ScreenStat,HZSELECT);
/*
                if (SelIdx>=DISPMAX/2)
                {//挑选第2行的重码汉字
                    x = (SelIdx-DISPMAX/2)*3+DISPPOSX+1;
                    y=DISPPOSY+1;
                }
                else
                {//挑选第1行的重码汉字
                    x = SelIdx*3+DISPPOSX+1;
                    y=DISPPOSY;
                }
 */
                W=CWORD(HZFound[SelIdx*2]); //GetChFr(x,y);
                RecallScreen();
                return W;
            }
            else if (CHNMode==ASC_MODE || TESTBIT(ApplVar.ScreenStat,HZSELECT) || SelMax==0)
            {//已经选取了一个汉字,又无新的输入时
                ChnInput(3);//自动退出汉字模式
                return(0x0100+keyno);//将确认按键提交给其它功能处理
            }
            else
                return 0;

        case ID_xINPUT://切换输入法
            RecallScreen();
            //ccr2015-10-08>>>>>>>>>
            if (LastMode!=0xff)
                CHNMode=LastMode;
            else
            {
#if (USE_X_ONLY==1) //ccr2016-07-04>>>>>>>>>>>>>>>>>>>>>>>
                if (CHNMode==ASC_MODE)
                {
                    ASCIIMode++;
                    if (ASCIIMode>MAXASCII)
                    {
                        ASCIIMode = 0;
                        CHNMode--;
                    }
                }
                else
                    CHNMode--;
#else
                CHNMode--;
#endif
            }
#if EN_QUWEI==0
            if (CHNMode==QUWEI_MODE) CHNMode--;
#endif
            LastMode=0xff;
            if (CHNMode>PYBH_MODE)
                CHNMode = PYBH_MODE;
            //ccr2015-10-08<<<<<
            InputNum = 0;
            RESETBIT(ApplVar.ScreenStat,HZSELECT);
//ccr2015-10-08            if (CHNMode==PYBH_MODE)//为拼音输入法
//ccr2015-10-08                ASCIIMode=LOASCII;
            ClearLine(STATELINE);

            SelIdx = SelMax = 0;
            DispStrXY((BYTE*)INPUTMODE[CHNMode].Name,0,STATELINE);

            //ccr20131223>>>>>
            if (CHNMode==ASC_MODE)
            {

                switch (ASCIIMode)
                {
                default: ASCIIMode= LOASCII;//ccr2016-07-04
                case LOASCII:  DispCharXY('a',0,STATELINE);break;
                case UPASCII:  DispCharXY('A',0,STATELINE);break;
                case SPASCII:  DispCharXY('*',0,STATELINE);break;
#if EN_EXASCII //ccr2014-03-26>>>>>>
                case EXASCII:
                    EXAsc_Buf[0]=EXAsc_Buf[1]=EXAsc_Buf[2]=' ';EXAsc_Buf[3]=0;
                    DispStrXY(EXAsc_Buf,INPUTPOS,STATELINE);//清除所输入的内容
                    DispCharXY('@',0,STATELINE);InputNum=0;EXAsc_Buf[0]=0;
                    break;
#endif//<<<<<<<
                }
            }//<<<<<<<<<<<<<<<<<
            return 0;
#else
        case ID_CLEAR:
#if EN_EXASCII //ccr2014-03-26>>>>>>
            if (InputNum)
            {
                EXAsc_Buf[0]=EXAsc_Buf[1]=EXAsc_Buf[2]=' ';
                EXAsc_Buf[3]=0;
                DispStrXY(EXAsc_Buf,INPUTPOS,STATELINE);//清除所输入的内容
                InputNum = 0;
                return 0;
            }
#endif//<<<<<<<
        case ID_SELECT:
        case ID_DELETE:
            return(0x0100+keyno);
        case ID_ENTER:
            ChnInput(3);//ccr2016-06-12 自动退出ASCII模式
            return(0x0100+keyno);
        case ID_xINPUT:
            CHNMode = ASC_MODE;
            RESETBIT(ApplVar.ScreenStat,HZSELECT);
#if (USE_X_ONLY==1) //ccr2016-07-04>>>>>>>>>>>>>>>>>>>>>>>
            ASCIIMode++;
            switch (ASCIIMode)
            {
            default: ASCIIMode= LOASCII;//ccr2016-07-04
            case LOASCII:  DispCharXY('a',0,STATELINE);break;
            case UPASCII:  DispCharXY('A',0,STATELINE);break;
            case SPASCII:  DispCharXY('*',0,STATELINE);break;
#if EN_EXASCII //ccr2014-03-26>>>>>>
            case EXASCII:
                EXAsc_Buf[0]=EXAsc_Buf[1]=EXAsc_Buf[2]=' ';EXAsc_Buf[3]=0;
                DispStrXY(EXAsc_Buf,INPUTPOS,STATELINE);//清除所输入的内容
                DispCharXY('@',0,STATELINE);InputNum=0;EXAsc_Buf[0]=0;
                break;
#endif//<<<<<<<
            }
            return 0;
#endif//ccr2016-07-04<<<<<<<<<<<<

/*ccr20131223 解决英文版本下切换大小写输入的问题
            ClearLine(STATELINE);
            DispStrXY((BYTE*)INPUTMODE[CHNMode].Name,0xff,STATELINE);
            return 0;
*/
#endif
#if (USE_X_ONLY==0) //ccr2016-07-04>>>>>>>>>>>>>>>>>>>>>>>
        case ID_xASCII://切换ASCII大小写(=ID_xDEPT)
#if defined(CASE_FORHANZI)       //ccr2015-10-08>>>>>>>>>>>>>
            if (LastMode==0xff && CHNMode!=ASC_MODE)
            {
                LastMode=CHNMode;
                ClearLine(STATELINE);
            }
            else if (CHNMode==ASC_MODE)
                ASCIIMode++;

            CHNMode=ASC_MODE;
#endif//ccr2015-10-08<<<<<<<<<<<<<<<
            if (CHNMode==ASC_MODE)
            {
#if !defined(CASE_FORHANZI)       //ccr2015-10-08>>>>>>>>>>>>>
                ASCIIMode++;
#endif//ccr2015-10-08<<<<<<<<<<<<<<<
                switch (ASCIIMode)
                {
                default:  ASCIIMode=LOASCII;//ccr2016-07-04
                case LOASCII:  DispCharXY('a',0,STATELINE);break;
                case SPASCII:  DispCharXY('*',0,STATELINE);break;
                case UPASCII:  DispCharXY('A',0,STATELINE);break;
#if EN_EXASCII //ccr2014-03-26>>>>>>
                case EXASCII:
                    EXAsc_Buf[0]=EXAsc_Buf[1]=EXAsc_Buf[2]=' ';EXAsc_Buf[3]=0;
                    DispStrXY(EXAsc_Buf,INPUTPOS,STATELINE);//清除所输入的内容
                    DispCharXY('@',0,STATELINE);InputNum=0;EXAsc_Buf[0]=0;
                    break;
#endif//<<<<<<<
                }
                return 0;
            }
            else
            {
#if defined(CASE_FORHANZI)
                RecallScreen();
#endif
                return(0x0100+keyno);
            }
#endif//ccr2016-07-04<<<<<<<<<<<<
        case ID_CANCEL:
            if (InputNum==0)//ccr20160114
                KeyFrHost=ApplVar.AP.FirmKeys[ID_CANCEL];

#if defined(CASE_FORHANZI)
            InputNum = SelIdx = SelMax = 0;
            RecallScreen();
#endif

#if EN_EXASCII //ccr2014-03-26>>>>>>
            if (InputNum)
            {
                EXAsc_Buf[0]=EXAsc_Buf[1]=EXAsc_Buf[2]=' ';EXAsc_Buf[3]=0;
                DispStrXY(EXAsc_Buf,INPUTPOS,STATELINE);//清除所输入的内容
                InputNum = 0;
            }
#endif//<<<<<<<
            RESETBIT(ApplVar.ScreenStat,HZINPUTMODE);
            memcpy(ApplVar.ScreenMap[SCREENLN],SaveSTLine,SCREENWD);
            mRefreshLine(STATELINE);
            return 0;
        }

#if defined(CASE_FORHANZI)
        switch (CHNMode)
        {
        case ASC_MODE:
            return GetNumric(keyno);
        case QUWEI_MODE:
            return  QWInput(keyno);
        case PYBH_MODE:
            return  PYInput(keyno);
        }
#else
        if (CHNMode==ASC_MODE)
        {
#if EN_EXASCII //ccr2014-03-26>>>>>>
            if (ASCIIMode==EXASCII)
               return InputEXASCII(keyno);
            else
#endif//<<<<<<<
                return GetNumric(keyno);
        }
#endif
    }

    if (keyno<64)
    {
        W = ApplVar.AP.KeyTable[keyno];
        if (W>='0' && W<='9' || W=='.')
            return W;
        else if (W == ZERO2 && !TESTBIT(ApplVar.MyFlags,HANZIMODE))
            return 0x3030;
    }
    return (0x0100 | keyno);
}
/************************************************************************************
    func=0,用来检测是否有输入；返回为0时，表示没有输入 //
    func＝1，CHNInput读取汉字输入数据,返回值>256时，表示输入了一个汉字； //
    func＝2，收款机进入汉字输入状态；此时如果光标位置处在屏幕的最后一行， //
            将屏幕上滚一行，光标位置同时也上移一行，打开状态行，显示汉字输入状态信息。 //
    func=3，收款机退出汉字输入状态；关闭状态行。 //
**********************************************************************************/
WORD ChnInput(BYTE func)
{
    switch (func)
    {
    case 0://用来检测是否有输入；返回为0时，表示没有输入
        HANZI = CheckInput();
    case 1://读取汉字输入数据返回值>256时，表示输入了一个汉字
        return  HANZI;
    case 2://收款机进入汉字输入状态
        if (!TESTBIT(ApplVar.ScreenStat,HZINPUTMODE))
        {
            memcpy(SaveSTLine,ApplVar.ScreenMap[SCREENLN],SCREENWD);
            OpenMess(1);
        }
        SETBIT(ApplVar.ScreenStat,HZSELECT);    //ccr2016-06-12@@@@@ 初次进入,标示已经选择过汉字
        ClearLine(STATELINE);
#if defined(CASE_FORHANZI)
        InputNum = SelIdx = SelMax = 0;
        DispStrXY((BYTE*)INPUTMODE[CHNMode].Name,0xff,STATELINE);
#endif
        //ccr20131223>>>>>
        if (CHNMode==ASC_MODE)
        {
            switch (ASCIIMode)
            {
            default: ASCIIMode=LOASCII;//ccr2016-07-04
            case LOASCII:  DispCharXY('a',0,STATELINE);break;
            case UPASCII:  DispCharXY('A',0,STATELINE);break;
            case SPASCII:  DispCharXY('*',0,STATELINE);break;
#if EN_EXASCII //ccr2014-03-26>>>>>>
            case EXASCII:
                EXAsc_Buf[0]=EXAsc_Buf[1]=EXAsc_Buf[2]=' ';EXAsc_Buf[3]=0;
                DispStrXY(EXAsc_Buf,INPUTPOS,STATELINE);//清除所输入的内容
                DispCharXY('@',0,STATELINE);InputNum=0;EXAsc_Buf[0]=0;
                break;
#endif//<<<<<<<
            }
        }//<<<<<<<<<<<<<<<<<

        SETBIT(ApplVar.ScreenStat,HZINPUTMODE);

        return 0;
    case 3://收款机退出汉字输入状态；关闭状态行
        if (TESTBIT(ApplVar.ScreenStat,HZINPUTMODE))
        {
#if defined(CASE_FORHANZI)
            RecallScreen();
#endif
            RESETBIT(ApplVar.ScreenStat,HZINPUTMODE);
            memcpy(ApplVar.ScreenMap[SCREENLN],SaveSTLine,SCREENWD);
            mRefreshLine(STATELINE);
        }
//            OpenMess(0);
        return 0;
    default:
        return 0;
    }
}
//设置当前编辑的内容的显示位置
void SetEditPosition(BYTE x,BYTE y,BYTE len,char *source)
{
    Editor.DispX=x;//new SETUPDISPX-SETINPUTWD
    Editor.DispY=y;
    Editor.DispW=(SCREENWD-x);
    //保证从第一个字符开始显示
    if (len<=Editor.DispW)
    {
        Editor.EditOffs=0;
        Editor.DispFrom = ENTRYSIZE-1 - Editor.DispW;
    }
    else
    {
        Editor.DispFrom = ENTRYSIZE-1 - len;
        Editor.EditOffs= 0;//ccr2017-03-16 len-Editor.DispW;
    }
    Editor.EditCount=len;
    Editor.BuffSize=ENTRYSIZE-1;
    Editor.EditBuff=EntryBuffer;
    Editor.Source=source;
    if ((ApplVar.CentralLock!=RG) && ((ApplVar.CentralLock & 0xff)!=MG))
        GotoXY(SCREENWD,y);//将光标定位在最右端
}

/********************************************************
 * 初始化LineEditor. 编辑区的右界固定为屏幕的右边界
 *
 * @author EutronSoftware (2016-01-20)
 *
 * @param x :编辑区在屏幕上的显示位置
 * @param y
 */
void InitLineEditor(short x,short y)
{
    CursX = SCREENWD;
    CursY = y;

    Editor.EditBuff = EntryBuffer;
    Editor.Source=NULL;
    Editor.BuffSize = ENTRYSIZE-1;
    EntryBuffer[ENTRYSIZE-1]=0;
    Editor.EditOffs=0;
    Editor.EditCount = Appl_EntryCounter;
    Editor.DispX = x;
    Editor.DispY = y;
    Editor.DispW = SCREENWD-x;
    Editor.DispFrom = ENTRYSIZE-Editor.DispW-1;    //GotoXY(x,y);
}

/*行编辑器：用来对保存在EntryBuffer中的输入信息进行编辑 //
  此编辑器只能在汉字模式下（MyFlags,HANZIMODE）
Input:
    ch:字符、汉字、按键； //
    ch.high = 0, ch中为ASCII字符 //
    ch.high>0xa0，ch中为汉字 //
    ch.high=1,ch.low为按键 //
Return:
    =0xff,没有对信息进行编辑 //
    其他值，为编辑后信息长度 //
*/

#define CurrentPOS   (Editor.BuffSize-Editor.EditOffs-1)

BYTE LineEditor(union KEY ch)
{
    short sLp,oldX,newX,dleta;


    //ccr2016-05-24 oldX = Editor.BuffSize - (Editor.DispW-(Editor.DispFrom-Editor.DispX)+Editor.EditOffs);

    if (ch.C.High==1)
    {//input is a function key

//        CursX = Editor.DispFrom;
        switch (GetFirmkeyID(ch.C.Low))
        {
        case ID_ENTER:// 为输入确认键  //
            if (TESTBIT(ApplVar.MyFlags,HANZIMODE))
            {
                if (TESTBIT (ApplVar.ScreenStat,HZINPUTMODE))
                    ChnInput(3);
                break;
            }
            else
                return 0xff;
        case ID_CLEAR:        // 12
            if (ApplVar.ErrorNumber)
            {//ccr060118 清除错误提示 //
                ApplVar.ErrorNumber=0;
//ccr2016-12-01				mRefreshLine(STATELINE);//20130930  恢复显示此前的显示     ClsState2();
                if (TESTBIT(ApplVar.MyFlags,HANZIMODE))
                    ChnInput(2);
                break;
            }
#if defined(CASE_FORHANZI)
            else if (InputNum == 0 && Editor.EditCount>0)
#else
            else if (Editor.EditCount>0)
#endif
            {
                //Editor.DispFrom=
                CursX = SCREENWD;//                GotoXY(SCREENWD,Editor.DispY);
                memset(Editor.EditBuff,' ',Editor.BuffSize);
                if (!TESTBIT(ApplVar.MyFlags,HANZIMODE))
                    Editor.EditBuff[Editor.BuffSize-1] = '0';
                Editor.EditBuff[Editor.BuffSize] = 0;
                ApplVar.DecimalPoint = 0;//ccr090617
                Editor.EditCount = 0;
                Editor.EditOffs= 0;
                Editor.DispFrom=Editor.BuffSize-Editor.DispW;
#if (SCRSTATDBL)
                if (ApplVar.CentralLock==RG || ApplVar.CentralLock==MG)
                    DispOnStatDBL(Editor.EditBuff+Editor.BuffSize-SCRDBLWD,0);
                else
#endif
                    DispStrXY(Editor.EditBuff+Editor.DispFrom,Editor.DispX,Editor.DispY);//RG-DISPLAY
                if (Editor.Source && TESTBIT(ApplVar.MyFlags,HANZIMODE))//ccr2015-03-11
                    Editor.Source[0] = 0; //ccr2015-03-11 按清除键时,将所要输入的内容清除
                break;
            }
            else
                return 0xff;
#if (defined(CASE_ER220)||defined(CASE_ER100) || defined(CASE_ER520U))
        case ID_UP:     // 255
#else
        case ID_LEFT://cUP:		// 255
#endif      //ccr20150108左右移动编辑位置似乎不起作用,
            //ccr20150108原因:在KeyInput函数中执行了Editor.DispFrom = CursX;Editor.DispY = CursY;
            if (ApplVar.CentralLock==RG || ApplVar.CentralLock==MG)
                return 0xff;
            if (Editor.EditOffs>=Editor.EditCount) break;
            newX = 1 + TestHZRight(Editor.EditBuff,CurrentPOS);//计算左移字符数
            Editor.EditOffs+=newX;

            if (CursX<=Editor.DispX+newX)
            {
                if (Editor.EditOffs>=Editor.EditCount)
                     Editor.EditOffs = Editor.EditCount;
                CursX = Editor.DispX;
                Editor.DispFrom=CurrentPOS+1;
            }
            else
            {
                  CursX-=newX;
                  if (Editor.EditOffs>=Editor.EditCount)
                  {
                     Editor.EditOffs = Editor.EditCount;
                     Editor.DispFrom=CurrentPOS - (CursX-Editor.DispX);
                  }
            }
            DispStrXY(Editor.EditBuff+Editor.DispFrom,Editor.DispX,Editor.DispY);
            break;
#if (defined(CASE_ER220)||defined(CASE_ER100) || defined(CASE_ER520U))
        case ID_DOWN:     // 255
#else
        case ID_RIGHT://cDOWN:		// 255
#endif
            if (ApplVar.CentralLock==RG || ApplVar.CentralLock==MG)
                return 0xff;
            if (Editor.EditOffs==0)   break;
            newX = 1;
#if defined(CASE_FORHANZI)
            if (Editor.EditBuff[CurrentPOS+1]>0x80)
                newX++;
#endif
            if (Editor.EditOffs>newX)
                Editor.EditOffs-=newX;
            else
                Editor.EditOffs=0;

            if (CursX+newX<=SCREENWD)//ccr2016-01-19Editor.DispX+Editor.DispW)
            {
                CursX += newX;
                Editor.DispFrom=CurrentPOS-(CursX-Editor.DispX)+1;
            }
            else //(sCurX>DispX+DispW)
            {
                CursX = SCREENWD;
                Editor.DispFrom=CurrentPOS-Editor.DispW+1;
            }
            DispStrXY(Editor.EditBuff+Editor.DispFrom,Editor.DispX,Editor.DispY);
            break;
        case ID_DELETE://删除Editor.EditOffs位置的字符
            if (Editor.EditCount>0)
            {
                if (Editor.EditOffs<Editor.EditCount)
                {//删除Editor.EditOffs位置的字符
                    oldX=CurrentPOS;
                    newX=oldX-1;
#if defined(CASE_FORHANZI)
                    if (Editor.EditCount>=2 && TestHZRight(Editor.EditBuff,oldX))//ccr2015-05-08 0xa0 饸饹
                    {
                        newX--;
                    }
#endif
                    dleta = Editor.EditCount-Editor.EditOffs +(newX-oldX);
                    for (sLp=0;sLp<dleta;sLp++)
                        Editor.EditBuff[oldX--]=Editor.EditBuff[newX--];
                    Editor.EditCount-=(oldX-newX);
                    if (oldX>0)
                        for (;oldX!=newX;oldX--)
                            Editor.EditBuff[oldX]=' ';
                }
                else
                {//删除Editor.EditOffs-1位置的字符
                    oldX=CurrentPOS+1;
                    newX=oldX-1;
                    dleta = 1;
#if defined(CASE_FORHANZI)
                    if (Editor.EditBuff[oldX]>0x80 && Editor.EditCount>=2)//ccr2015-05-08 0xa0 饸饹
                    {
                        oldX++;
                        dleta = 2;
                    }
#endif
                    for (sLp=0;sLp<dleta;sLp++)
                        Editor.EditBuff[oldX--]=Editor.EditBuff[newX--];
                    if (oldX>0)
                        Editor.EditBuff[oldX]=' ';
                    Editor.EditCount-=dleta;
                    Editor.EditOffs-=dleta;

                    if (CursX+dleta<=SCREENWD)//ccr2016-01-19Editor.DispX+Editor.DispW)
                    {
                        CursX+=dleta;
                        Editor.DispFrom=CurrentPOS-(CursX-Editor.DispX)+1;
                    }
                    else //(sCurX>DispX+DispW)
                    {
                        CursX = SCREENWD;
                        Editor.DispFrom=CurrentPOS-Editor.DispW+1;
                    }
                }
                DispStrXY(Editor.EditBuff+Editor.DispFrom,Editor.DispX,Editor.DispY);
            }
            break;
        default:
            return 0xff;
        }
    }
    else
    {//input is a character (HANZI or ASCII)
        //ccr2015-03-10 cc 2006-08-14 for Caption>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        if (!Editor.EditOffs)
        {
            newX = Editor.BuffSize-Editor.DispW-Editor.EditOffs;
            //ccr20150107为什么要显示一次? 导致每输入一个字符,会显示一次老数据,重复显示!!!!
//ccr2016-03-01            DispStrXY(Editor.EditBuff+newX, Editor.DispX+TestHZRight(Editor.EditBuff, newX),Editor.DispY);  //RG-DISPLAY
        }
        //cc 2006-08-14 for Caption<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

        if (Editor.EditCount>=Appl_MaxEntry)
        {
            if (Appl_MaxEntry && ApplVar.DispKey.Code == ApplVar.AP.FirmKeys[ID_ENTER])
            {
                //ClearEntry(false);
                oldX = Editor.BuffSize;
                ApplVar.DispKey.Code = 0;
            }
            else
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI18);
                return 0xff;
            }
        }
        if (!Editor.EditCount)
            memset(Editor.EditBuff,' ',Editor.BuffSize);
        if (!TESTBIT(ApplVar.MyFlags,HANZIMODE))
        {
            if (ch.C.Low == '.')
            {
                if (!ApplVar.DecimalPoint)//ccr2015-08-18 && !NO_DECIMAL)//ccr2015-04-20禁止止输入小数
                {
                    ApplVar.DecimalPoint = 1;
                    if (Editor.EditCount==0)
                    {
                        Editor.EditBuff[Editor.BuffSize-1] = '0';
                        Editor.EditCount = 1;
                    }
                }
                else
                {
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI01);//The count of point must be 0
                    return 0xff;
                }
            }
            else if (ApplVar.DecimalPoint)
            {
                ApplVar.DecimalPoint++;
                if (ch.Code == 0x3030)
                    ApplVar.DecimalPoint++;
                if (ApplVar.DecimalPoint > 4)
                {
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
                    return 0xff;
                }
            }
        }

        newX = 1;
        Editor.EditCount++;
        //ccr20160630>>>>>>
        if (ch.C.High>0x20)// 为汉字字符 //
        {
            if (Editor.EditCount<Appl_MaxEntry)//ccr2016-05-24 && oldX>1)
            {// 编辑去可以容纳完整汉字 //
                Editor.EditCount++;
                newX++;
            }
#if defined(CASE_FORHANZI)
            else
                ch.Code &= 0x007f;
#else
            else
                ch.C.High = 0;
#endif
        }
        memcpy(Editor.EditBuff,Editor.EditBuff+newX,CurrentPOS);
        oldX = CurrentPOS;

        if (ch.C.High>0x20 && oldX>1)//如果CASE_FORHANZI=0,双'00'输入可能出错!!!!!
            Editor.EditBuff[oldX--]=ch.C.High;

        Editor.EditBuff[oldX]=ch.C.Low;

        if (!TESTBIT(ApplVar.MyFlags,HANZIMODE) || TESTBIT(ApplVar.MyFlags,PWDMODE))//ccr2015-03-10
        {
//			if (Editor.EditCount<=Appl_MaxEntry)
            {
                memset(SysBuf,' ',Editor.DispW);

                memcpy(SysBuf,Editor.EditBuff + Editor.BuffSize - Editor.DispW,Editor.DispW);

                SysBuf[Editor.DispW] = 0;

                if (TESTBIT(ApplVar.MyFlags,PWDMODE) || (ApplVar.CentralLock & 0x0f00))        /* secret code entry ? */
                {
                    for (oldX=0;oldX<Editor.DispW;oldX++)
                        if (SysBuf[oldX]>' ')
                            SysBuf[oldX]='*';
                        else if ((ApplVar.CentralLock & 0xf00)==0x200)
                            SysBuf[oldX] = '?';
                        else
                            SysBuf[oldX]='_';
                }


                if (TESTBIT(ApplVar.MyFlags,PWDMODE))
                {
                    if ((ApplVar.CentralLock & 0x0f00) || Appl_ProgLine<1)        /* secret code entry ? */
                        DispStrXY(SysBuf,Editor.DispX,STATELINE);
                    else
                        DispStrXY(SysBuf,Editor.DispX,Editor.DispY);
                }
                else
                {
                    if ((ApplVar.CentralLock==SET || ApplVar.CentralLock==(SETUPMG | MG)) && Appl_ProgLine>1)//Appl_ProgLine>1时,输入的内容显示在屏幕内容区
                    {
                        if (Editor.EditOffs<=Editor.DispW)
                            DispStrXY(Editor.EditBuff+Editor.BuffSize-Editor.DispW,Editor.DispX,Editor.DispY);
                        else
                            DispStrXY(Editor.EditBuff+CurrentPOS,Editor.DispX,Editor.DispY);
                    }
                    else
                    {
#if (SCRSTATDBL)
                        if (ApplVar.CentralLock==RG || ApplVar.CentralLock==MG)
                            DispOnStatDBL(Editor.EditBuff+Editor.BuffSize-SCRDBLWD,0);
                        else
#endif
                            DispSt2(Editor.EditBuff+Editor.BuffSize-Editor.DispW);  //RG-DISPLAY

                    }
                    if (ApplVar.CentralLock==RG || ApplVar.CentralLock==MG)
                    {
                        LCD10_SetFlag(FLAG_NULL);
                        if (Editor.DispW>DISLEN)
                            PutsO(SysBuf+Editor.DispW-DISLEN);//C-DISPLAY 将输入的内容同时显示在客显上
                        else
                            PutsO(SysBuf);//C-DISPLAY 将输入的内容同时显示在客显上
                    }
                }
            }
//		    else
//		    {
//		    	ApplVar.ErrorNumber=ERROR_ID(CWXXI18);
//		    	return 0xff;
//		    }
        }
        else
        {
            if (Editor.EditOffs<=Editor.DispW)
                DispStrXY(Editor.EditBuff+Editor.BuffSize-Editor.DispW,Editor.DispX,Editor.DispY);
            else
                DispStrXY(Editor.EditBuff+CurrentPOS,Editor.DispX,Editor.DispY);

/*ccr2016-01-19            newX = Editor.BuffSize-Editor.DispW-Editor.EditOffs;
            sCurX = TestHZRight(Editor.EditBuff,newX);
            if (sCurX)
                DispCharXY(' ',Editor.DispX,Editor.DispY);
            DispStrXY(Editor.EditBuff+newX+sCurX,Editor.DispX+sCurX,Editor.DispY);*/
        }
    }
    return Editor.EditCount;
}

//============================================================================
// 在当前光标位置输入字符串数据编辑器                                       //
// 返回－1时，表示输入的是退出键, 返回值>0,为输入长度                       //
// buf=输入数据存放区,width=允许输入数据长度, hzInput=是否需要进入汉字输入    //
// 输入的数据靠右填入buf中前width个位置，输入数据不足width个时，靠左补足空格//
// 如width=6,输入‘123’，buf中的数据为'   123';                            //
//============================================================================
short GetStringCHN(char *buf,short width,BYTE hzInput)
{
    WORD keyno ;
    BYTE saveRet,sSaveLine,dispW ;
    int x,y ;

    SetLoop();

    memset(buf,' ',width);
    x=SCREENWD-width ;
    if(x<0)
    {
    	x=0;
        dispW = SCREENWD;
    }
    else
        dispW = width;
    y=CursY ;

    DispStrXY(buf,x,y);
    GotoXY(SCREENWD,y);    //ccr光标必须定在编辑区的右端//
    Editor.EditBuff=buf ;	// 初始化输入数据区  //
    Editor.Source=buf ;
    Editor.BuffSize=width ;
    *((char*)(buf+width))=0 ;//// set the end flag of input    ////????????????
    Appl_MaxEntry=width ;		//// Max entry for input
    Editor.EditOffs=0 ;	//// the first input
    Editor.EditCount=0 ;
    Editor.DispX=x ;		//locationg for display
    Editor.DispY=y ;
   	Editor.DispW=dispW ;
    Editor.DispFrom = 0;
    //ccr指定编辑区的宽度//
    if (!TESTBIT(ApplVar.MyFlags,PWDMODE))//ccr 050523
	    SETBIT(ApplVar.MyFlags,HANZIMODE);

	if (hzInput)
	    ChnInput(2);// 初始化汉字输入方法,进入汉字输入输入状态 ////

	sSaveLine = Appl_ProgLine;
	if (ApplVar.CentralLock == SET && TESTBIT(ApplVar.MyFlags,PWDMODE))//// input password mode
		Appl_ProgLine = 1;//display the password on screen area
	ApplVar.DispKey.Code = 0;
    for(;;)
    {
        while((keyno=ChnInput(0))==0)//// 测试是否有输入  ////
		{// 处理键盘遥控状态下的从计算机来的键盘数据 ////
		}
		if (keyno==(0x0100+cEXIT))
		{// 为退出键 ////
            RESETBIT(ApplVar.MyFlags,(HANZIMODE | PWDMODE));//// 清除汉字输入和密码输入标志  ////
            ResetLoop();
			return -1;				//ccr 050523 送出退出键  ////
		}

		if (keyno==(0x0100+cSHIFT) && hzInput && !TESTBIT(ApplVar.ScreenStat,HZINPUTMODE))
		{// 为进入汉字输入方式键 ////
            ChnInput(2);
			continue;
		}


	    if (!TESTBIT(ApplVar.MyFlags,PWDMODE))//ccr 050523
    	    SETBIT(ApplVar.MyFlags,HANZIMODE);//  不是密码输入状态时,进入汉字输入状态  ////
        if((keyno&0xff00)==0x0100&&(keyno&0xff)==ApplVar.AP.FirmKeys[ID_ENTER])
        {//// 为功能键或确认键时,退出输入 ////
        	ApplVar.ErrorNumber=0;
            ChnInput(3);
            RESETBIT(ApplVar.MyFlags,(HANZIMODE | PWDMODE));
            if (Editor.EditCount)//ccr2016-10-24
                ScreenFeed(1);
            //ccr 040422
			Appl_ProgLine = sSaveLine;
            ResetLoop();
            return(Editor.EditCount);
        }
        if (keyno==0x3030)// 双<00>键 //
       	{
			keyno = '0';
	        saveRet=LineEditor(*((union KEY*)&keyno));// 调用编辑器,对输入内容进行编辑  //
       	}
        saveRet=LineEditor(*((union KEY*)&keyno));// 调用编辑器,对输入内容进行编辑  //
        ApplVar.ErrorNumber = 0;// !!!!清除由LineEditor报告的错误，否则，当出错时，ChnInput无法正常输入  //
    }
}




