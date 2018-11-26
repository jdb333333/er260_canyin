#define FIRMKEY_C
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"
#include "message.h"

/****************************************************************/

void SuspendECR();
BYTE CheckMode();
void (*SetupList)(BYTE keyno);

extern BYTE GrapNo;
/****************************************************************/

//设置菜单下的二级菜单组织表,保存各二级菜单的项目数
CONST MenuDefine SubMenuItems[SUBMENTMAX+1] = {
    {MENUFILES,SUBMENTMAX},     // 一级菜单的项目描述
                                //以下为二级菜单项目描述
    {SETPLU,    MENUPLUMAX},    // "资料管理",
    {SETSYSFLAG,MENUSYSMAX},    // "系统设置",
    {SETKEYB,   MENUKEYBMAX},   // "键盘功能",
    {SETHEAD,   MENUTICMAX},    // "票头票尾",
    {SETCOMPUTER,MENUHOSTMAX},  // "通讯",
    {SETBARCODE,MENUCOMMAX},    // "外接设备",
    {SETPBINF,  MENUTBLMAX},    // "餐饮设置",
    {SETREPORT, MENUREPMAX},    // "报表管理",
    {SETOFF,    MENUDISCMAX},   // "折扣促销",
    {SETTEND,    MENUMANMAX},   // "销售功能",
    {SETVIPIC,     MENUICCMAX},    // "会员卡管理",
#if !defined(DD_LOCK)
    {SETSETPWD,  MENUPWDMAX},    // "密码权限",
#endif
    {SETTEST,   MENUSERVICEMAX},    // "维护(Service)",
};
//打印设置系统菜单
void PrintSetupMenu(void)
{
    int mIdx,sIdx,i,j;
    char sStr[PRTLEN+3];

#if defined(CASE_FORHANZI)
    strcpy(sStr,"设置");
    strcpy(sStr+4,cMESSSETMENU);
#else
    strcpy(sStr,"SETUP ");
    strcpy(sStr+6,cMESSSETMENU);
#endif
    PrintLine2('.');
    PrintStr_Center(sStr,true);
    PrintLine2('=');

    for (mIdx=1;mIdx<=SubMenuItems[0].MenuItems;mIdx++)
    {
        //打印上机菜单名称
        memset(sStr,' ',sizeof(sStr));
        if (mIdx>9) sStr[3]=mIdx/10+'0';
        sStr[4]=mIdx % 10+'0';
        sStr[5]='.';
        strncpy(sStr+6,Msg[SubMenuItems[0].MenuFrom+mIdx-1].str,sizeof(sStr)-6);
        sStr[PRTLEN+3-1]=0;
        RJPrint(0,sStr+3);

        for (sIdx=1;sIdx<=SubMenuItems[mIdx].MenuItems;sIdx++)
        {//打印下级菜单名称
            if (sIdx>9)
                sStr[6]=sIdx/10+'0';
            else
                sStr[6]=' ';
            sStr[7]=sIdx % 10+'0';
            sStr[8]='.';
            i=SubMenuItems[mIdx].MenuFrom+sIdx-1;
            strncpy(sStr+9,Msg[i].str,sizeof(sStr)-9);
            sStr[PRTLEN-1]=0;
            RJPrint(0,sStr);
#if (0)//   //打印系统参数项目
            if  (i==SETSYSFLAG)
            {
                memset(SysBuf,' ',PRTLEN);
                SysBuf[6]='>';
                for (i=1;i<=SYSUSED;i++)
                {
                    if (i>9)
                        SysBuf[7]=i/10+'0';
                    else
                        SysBuf[7]=' ';
                    SysBuf[8]=i % 10+'0';
                    SysBuf[9]='.';
                    ListRecord.Number=i-1;
                    ReadFlagsList();
                    strcpy(SysBuf+10,ListRecord.Name);

                    SysBuf[PRTLEN-1]=0;
                    RJPrint(0,SysBuf);
                }
            }
#endif
        }
    }
    RFeed(3);
    SETBIT(ApplVar.MyFlags,PRNHEADMUST);

}

/****************************************************************/
//显示设置模式下的主菜单
void DisplaySetup1stMenu()
{
        SetList.Cols = 1;   // 2; 20060105
        SetList.Rows = SCREENLN-TESTBIT(ApplVar.ScreenStat,STATEON);
        SetList.MsgIdx = SubMenuItems[0].MenuFrom;    // 20060105 SETDEPT;
        SetList.RecMax = SubMenuItems[0].MenuItems;    //MENUMAX;
        SetList.Rec1st = 0;         // first item of menu
        SetList.Var = &Appl_ProgType;
        SetList.isSub = FALSE;  // main menu
        SetList.SubNo = SubMenuItems[1].MenuFrom;      //SETSYSFLAG
        SetList.SubMax = SubMenuItems[1].MenuItems;

        SetupList = &DisplaySetup;
        ClsState12();
        DisplaySetup('1');   //显示设置模式下的主菜单
        Appl_ProgStart = 0;
        Appl_ProgNumber = 0;
        Appl_ProgLine = 0;
        Appl_BitNumber = 0;

        Appl_MaxEntry=5;
}

//显示系统设置下的主菜单
void DisplaySetup2ndMenu(int menuID)
{
        SetList.MsgIdx = SubMenuItems[menuID].MenuFrom;//SETSYSFLAG;//子菜单信息表
        SetList.RecMax = SubMenuItems[menuID].MenuItems;//MENUSYSMAX;
        SetList.Rec1st = 0;
        SetList.Var = &Appl_ProgType;
        SetList.isSub = TRUE;   //sub menu
        SetList.SubNo = SubMenuItems[menuID].MenuFrom;//SETSYSFLAG;        //reset sub menu
        SetList.SubMax = SubMenuItems[menuID].MenuItems;
        SetupList = &DisplaySetup;
        ClsState12();
        DisplaySetup('1');
        Appl_MaxEntry=5;

        Appl_ProgStart = 0;
        Appl_ProgNumber = 0;
        Appl_ProgLine = 0;
        Appl_BitNumber = 0;
}

/**** 获取功能键在ApplVar.AP.FirmKey中的序号 ************
  返回0xff:不是功能键
******************************************************/
BYTE GetFirmkeyID(BYTE keyno)
{
    int i;
    for (i = 0; i < FIRMKEYS; i++)
    {
        if (keyno == ApplVar.AP.FirmKeys[i])
        {
            return i;
        }
    }
   return 0xff;
}
/* 空函数 */
void    DisplayNull(BYTE keyno)
{
}
//显示菜单,提供菜单翻滚和选择功能,最多支持两级菜单
//keyIDX=0xff,将*(SetList.Var)中的值所指定的默认的菜单项显示在第一行
//keyIDX='0'..'9',ID_LEFT,ID_RIGHT,ID_UP,ID_DOWN,ID_SELECT
void    DisplaySetup(BYTE keyIDX)
{
    short i,j,sDispW;
    BYTE sRefresh,y,sOld,sListVar;

    if (SetList.Cols==0)
        SetList.Cols = 1;
    sRefresh = false;
    sDispW = SCREENWD / SetList.Cols;

    SETBIT(ApplVar.MyFlags,MENUMODE);

    sListVar = *(SetList.Var);

    sOld = sListVar;
    j = 0;
    if (keyIDX>'0' && keyIDX<=SetList.RecMax+'0')
    {//通过输入数值直接挑选菜单项,
        SetList.Rec1st  = keyIDX -'0'-1;
        sListVar = keyIDX-'0';

        sRefresh = true;

        if (sListVar>SetList.RecMax)
        {
            SetList.Rec1st = 0;
            sListVar = 1;
        }
    }
    else
    {
        switch (keyIDX)
        {
        case ID_LEFT:
            //选择菜单的上一项
            if (CursY==0 && CursX==0 || sListVar<=SetList.Rec1st)//at the top line of screen ccr081210
            {
                if (SetList.Rec1st>(SetList.Cols  * SetList.Rows))
                {
                    SetList.Rec1st -= SetList.Cols  * SetList.Rows;
                }
                else
                    SetList.Rec1st = 0;
                sListVar = SetList.Rec1st + 1;
                sRefresh = true;
            }
            else
                sListVar--;

            break;

        case ID_RIGHT:
            //选择菜单的下一项
            if (CursY==(SetList.Rows-1) && CursX == (SCREENWD-sDispW))
            {// on the bottom line of screen
                if (SetList.Rec1st+SetList.Cols  * SetList.Rows<SetList.RecMax)
                {
                    SetList.Rec1st += SetList.Cols  * SetList.Rows;
                }
                else
                    SetList.Rec1st = 0;
                sListVar = SetList.Rec1st + 1;
                sRefresh = true;
            }
            else
            {
                sListVar++;
                if (sListVar>SetList.RecMax)
                {
                    SetList.Rec1st = 0;
                    sListVar = 1;
                    sRefresh = true;
                }
                else if (sListVar<= SetList.Rec1st)//ccr081210
                {
                    sListVar = SetList.Rec1st + 1;
                    sRefresh = true;
                }
            }
            break;

        case ID_UP:
            //向上滚动一页(屏幕行数)
            if (SetList.Rec1st>(SetList.Cols  * SetList.Rows))
                SetList.Rec1st -= SetList.Cols  * SetList.Rows;
            else
                SetList.Rec1st = 0;
        case ID_SELECT://向下滚动一页
        case ID_DOWN://向下滚动一页

            if (keyIDX==ID_DOWN || keyIDX==ID_SELECT)
            {
                if (SetList.Rec1st+SetList.Cols  * SetList.Rows<SetList.RecMax)
                    SetList.Rec1st += SetList.Cols  * SetList.Rows;
                else
                    SetList.Rec1st = 0;
            }
            sListVar = SetList.Rec1st + 1;
            sRefresh = true;
            break;

        case 0xff://把默认的菜单项显示在第一行
            sRefresh = true;
            SetList.Rec1st = sListVar -1;
            break;
        default:
            return;
        }
    }

    if (!SetList.isSub)
    {//生成二级菜单信息
        SetList.SubNo = SubMenuItems[sListVar].MenuFrom;
        SetList.SubMax = SubMenuItems[sListVar].MenuItems;
        SetList.MainVar = sListVar;       //test
    }
    *(SetList.Var) = sListVar;

    if (sRefresh)
    {//显示菜单
        sOld = 0;
        mClearScreen();
        sRefresh = y = 0;
        mSetInvAttr();
        for (i=0;i<SetList.Cols * SetList.Rows;i++)
        {
            if (SetList.Rec1st + i >=SetList.RecMax)
                break;
            memset(SysBuf,' ',SCREENWD);
            WORDtoASC(SysBuf+1,SetList.Rec1st + i + 1);
            SysBuf[2] = '.';
            CopyFrStr(SysBuf+3, Msg[SetList.MsgIdx+sListVar+i-1].str);
            SysBuf[sDispW] = 0;
            DispStrXY(SysBuf,sRefresh,y);
            sRefresh += sDispW;
            if (sRefresh>(SCREENWD-sDispW))
            {
                sRefresh = 0;
                y++;
            }
            mClearInvAttr();
        }
    }

    if (sOld)
        j = 0x102; //0x1用来控制显示两次:第一次去掉上一菜单项的反显,第二次把当前菜单项变为反显
    else
    {
        j = 1;
        sOld = sListVar;
    }

    mClearInvAttr();

    for (;(j & 0xff)>0;j--)
    {
        i = sOld -SetList.Rec1st - 1;
        sRefresh = (i % SetList.Cols) * sDispW;
        y = i / SetList.Cols;
        if (j==1)
            break;
        memset(SysBuf,' ',SCREENWD);
        WORDtoASC(SysBuf+1,sOld);
        SysBuf[2] = '.';
        CopyFrStr(SysBuf+3, Msg[SetList.MsgIdx+sOld-1].str);
        SysBuf[sDispW] = 0;
        DispStrXY(SysBuf,sRefresh,y);
        sOld = sListVar;
        mSetInvAttr();
    }

    mClearInvAttr();
    GotoXY(sRefresh,y);
#if (0)//ccr2016-02-22不在状态行显示确认等 >>>>>>>>

    ClsState12();

    mSetInvAttr();

    memset(SysBuf,' ',SCREENWD);
    SysBuf[5] = 0;

//状态行显示不了全部汉字        strcpy(SysBuf, Msg[SetList.MsgIdx+sListVar-1].str);
//        SysBuf[SCREENWD-10] = 0;

    j=3;
    if (SetList.isSub)   //sub menu
    {
        if (SetList.MainVar>9)
            SysBuf[1] = SetList.MainVar/10+'0';
        SysBuf[2] = SetList.MainVar%10 + '0';
        SysBuf[3] = '-';
        j = 4;
    }
    if (sListVar>9)
        SysBuf[j++] =sListVar/10 + '0';
     SysBuf[j] = sListVar%10 + '0';


    DispSt1(SysBuf);

    mClearInvAttr();
    DispStrXY(Msg[QUERENF].str,SCREENWD-6,STATELINE);
#endif//<<<<<<<<<<<<<<<<<<<<<<
    RESETBIT(ApplVar.MyFlags,CLSSCREEN);//ccr201107221
}

//获取菜单输入功能数值,重新显示菜单,将当前选择的菜单项显示在第一行
BYTE GetInputForMenu(BYTE numkey)
{
    if (Appl_ProgLine==0 && Appl_ProgStart==0)
    {
        if (Appl_EntryCounter)
        {
            StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
            GetWordEntry();
        }
        else
            ApplVar.NumberEntry = 1;

        if (ApplVar.NumberEntry>SetList.RecMax)
        {//当输入的菜单序号大于菜单项目数时,自动取末尾
#if (PLUCODE_X==1)
            if (ApplVar.CentralLock == X)//ccr2014-09-09 PLUCODE_X
            {
                Appl_MaxEntry=14;//ccr2014-09-09 PLUCODE_X
                return true;
            }
#endif
            ApplVar.NumberEntry %= 10;//把最后输入的数字作为要选择的菜单项
            //把ApplVar.NumberEntry作为当前的输入,供下一次选择菜单使用
            if (ApplVar.NumberEntry>SetList.RecMax)
                ApplVar.NumberEntry = SetList.RecMax;
            MemSet(EntryBuffer, ENTRYSIZE, ' ');
            EntryBuffer[ENTRYSIZE - 1] = 0;
            EntryBuffer[ENTRYSIZE - 2] = ApplVar.NumberEntry + '0';
            Appl_EntryCounter = 1;
            Editor.EditCount = 1;
            Editor.EditOffs = 1;
#if (SCRSTATDBL)//ccr2016-08-15>>>>>>>>>>>
            DispOnStatDBL(EntryBuffer + ENTRYSIZE - SCRDBLWD - 1,0);
            DispOnStatDBL(ApplVar.ModeHead,0);
#else
            DispSt2(EntryBuffer + ENTRYSIZE - INPUTWIDTH - 1);
#endif//ccr2016-08-15<<<<<<<<<<<<<<<<
        }

        Appl_MaxEntry=14;//ccr2014-09-09 PLUCODE_X

        DisplaySetup(ApplVar.NumberEntry+'0');

        Appl_ProgStart = 0;
        Appl_ProgNumber = 0;
        Appl_ProgLine = 0;
        Appl_BitNumber = 0;

        return TRUE;
    }
    return FALSE;
}
//hf end <<<<<<<<<<<<<<<<<<<<<<<<<


void DisplayRecNo(BYTE keyno)
{
}

/*************************************************************************
  显示和修改设置项的数据
  *************************************************************************/
short DisplayOption()
{
    short   RandomL;
    WORD    listItem;//ccr2016-01-18
    BYTE    sSaveConfig=Appl_BitNumber;

    RESETBIT(ApplVar.MyFlags,PWDMODE+HANZIMODE);

    mEnCursor(ENABLE);

    if (Appl_ProgLine>2 && Appl_ProgType!=SETHEAD && Appl_ProgType!=SETTRAIL && Appl_ProgType!=SETMODIF && Appl_ProgType!=SETSHEAD)
        GotoXY(SCREENWD,ApplVar.sCursor.y);

    for (;;)
    {
        Editor.DispX=SETUPDISPX;//!!!!SETUPDISPX-SETINPUTWD
        Editor.DispW=SETINPUTWD;

        Appl_ProgLinePre = Appl_ProgLine;
        // if add a new setup,update this line if needed;
        memset(ProgLineMes,' ',sizeof(ProgLineMes));//ccr100707
        if (Appl_ProgLine == 1)
        {//对设置项目的界面和设置参数进行初始化
            switch (Appl_ProgType)
            {
            case SETKEYB:
                SetKeyboard();
//                Appl_ProgLine = 2;
//                VirtualInputKey(ApplVar.AP.FirmKeys[ID_CANCEL]);
                return 1;
            case SETDATE:
            case SETTIME:
            case SETIP:
            case SETCOMPUTER://Like Date
            case SETBARCODE:
            case SETBALANCE:
//ccr2016-01-22            case SETPORT4:
            case SETCHARGIC:
            case SETCLEARIC:
            case SETINITIC:
            case SETCHIPPOINT:
            case SETVIPIC:
            case SETCONFIGPLU:
            case SETCONFIGDEPT:
            case SETADDNEWVIP:
            case SETPOINTTYPE:
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
            case SETEJOURNAL:
#endif
                //这些设置无须输入记录号,直接进入设置
                mClearScreen();
                DispStrXY(Msg[TCHBJI].str,0,SCREENLN-2); //change by lqw.. not change wise158 already..
                break;
            case SETPRINTIC://"打印卡信息"
            case SETPRINTVIP://"打会员信息"
            case SETCLEANVIP://"清除会员"
            case SETPRINTKEYB://"打印键盘"
                mClearScreen();
                DispStrXY(Msg[Appl_ProgType].str,0,0);
                DispStrXY(Msg[TCHBJI].str,0,SCREENLN-2);
                break;
            default:
                if (Appl_ProgNumber<0)
                    Appl_ProgNumber = 0;
/*hf 20060105 ???
                    if (Appl_EntryCounter)
                    {
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
                    Appl_ProgType = 0;
                    return 0;
                    }
*/
                if (Appl_ProgStart == 2)
                {
                    CopyFrStr(ProgLineMes+1, Msg[JLHAO].str);
                    WORDtoASC(ProgLineMes + INPUTWIDTH - 1, Appl_ProgNumber + 1);
                    DispSt2(ProgLineMes);
                    mClearScreen();
                }
                else
                {//对于单品,部类等,通过 ListItems 来选择需要修改的数据记录
                    switch (listItem=ListItems(Appl_ProgType,0,It_SELEITEM,true))
                    {
                    case It_PLU_MG:    //退出设置
                    case It_EXIT:    //退出设置
                        //mClearScreen();
                        return true;//break;
                    case 0:         //无Items
                        mClearScreen();
                        break;
                    default://启用以下程序段时,直接进入设置,此时必须屏蔽KeyInput中的GetListItem调用
                        if (Appl_ProgType==SETSYSFLAG)
                        {
                            Appl_ProgLine=listItem-1+2;
                            Appl_ProgNumber=1;
                        }
                        else
                        {
                            Appl_ProgLine=2;
                            Appl_ProgNumber=listItem-1;
                        }
                        Appl_BitNumber = 0;
                        Appl_EntryCounter = 0;
                        ApplVar.ErrorNumber=0;
                        Appl_ProgStart = 1;
                        break;
                    }

                }
                break;
            }
        }
        if (Appl_ProgLine > 1 && Appl_EntryCounter)
        {//表示为输入了新的数据
            sSaveConfig |= 0x80;
        }
        switch (Appl_ProgType)
        {//显示或修改设置项的数据
        case SETKEYMACRO://ccr2016-03-15定义键盘宏
            ProgKeyMacro();
            break;
        case SETPLU://1:	/* plu */
            ProgPlu();
            break;
        case SETDEPT://2:	/* dept */
            ProgDept();
            break;
        case SETGROUP://3:	/* ApplVar.Group */
            ProgGroup();
            break;
        case SETTEND://4:	/* tend */
            ProgTend();
            break;
        case SETPORA://5:	/* pora */
            ProgPoRa();
            break;
        case SETCORR://6:	/* correc */
            ProgCorrec();
            break;
        case SETDISC://7:	/* disc */
            ProgDisc();
            break;
        case SETCURR://8:	/* ApplVar.Curr */
            ProgCurr();
            break;
        case SETDRAWER://9:	/* drawer */
            ProgDraw();
            break;
        case SETPBF://10:	 /* pb functions */
            ProgPbF();
            break;
#if !defined(CASE_FORHANZI)
        case SETTAX://11:	 /* tax */
            ProgTax();
            break;
#endif
        case SETCLERK://12:	 /* clerk */
            ProgClerk();
            break;
        case SETZONES://13:	 /* zones */
            ProgZone();
            break;
        case SETMODIF://14:	 /* modifiers */
            ProgModi();
            break;
        case SETHEAD://15:	/* program header */
            ProgHeader();
            break;
        case SETTRAIL://16:
            ProgTrailer();
            break;
        case SETSHEAD://17:	    /* slip header */
            ProgSlipHead();
            break;
        case SETREPORT://18:
            ProgReport();
            break;
//        case SETFIXCAP://19:	/* fixed captions */
//        case SETERRMSG://20:	/* system & error message */
//        case SETWEEKCAP://21:	/* day of week captions */
//        case SETMONTHCAP://22:	/* month captions */
        case SETREPTYPE://23:	/* report types */
            ProgSysMes();
            break;
        case SETSYSFLAG://24:	 /* system flags */
            ProgSysFlag();
            break;
        case SETKEYB://25:	/* keyboard code and manager */
            break;
        case SETKEYMASK://26:	/* keyswitch disable */
            GetOpt(7, ApplVar.AP.Config.KeyMask, sizeof(ApplVar.AP.Config.KeyMask) * 8);
            break;
        case SETPBINF://27:
            ProgPbInfo();
            break;
        case SETSALER://28:
            ProgSalPer();
            break;
        case SETDATE:
            ProgDate();
            break;
        case SETTIME:
            ProgTime();
            break;
        case SETOFF:
            ProgOFF();
            break;
        case SETCOMPUTER:
            ProgHost();//ccr2016-01-21 ProgPort(0);
            break;
        case SETBARCODE:
            ProgBarcodeReader();//ccr2016-01-21 ProgPort(1);
            break;
        case SETBALANCE:
            ProgBalance();
            break;
/*ccr2016-01-22        case SETPORT4:
            ProgLineMes[0]=0;
            DispStrXY(Msg[BAOLIU].str,0,0);
//			ProgPort(3);
            break;
ccr2016-01-22 */
        case SETGRAP:
            ProgPrnGraph();
            break;
        case SETVIPIC:
#if (DD_CHIPC==1 || DD_ECRVIP==1)
            ProgIC();    //ccr chipcard 2004-06-28
#else
            ProgLineMes[0]=0;
            DispStrXY(Msg[BAOLIU].str,0,0);
#endif
            break;
        case SETPROM:
            ProgPromotion();
            break;
#if (agreeNumber)
        case SETAGREE:
            ProgAgree();
            break;
#endif
        case SETKP:
            ProgKPrn();
            break;
        case SETEXTPRINTER:
            ProgExtPrinter();
            break;
        case SETSP:            //lyq added for set slip 20030323 start
            ProgSlip();
            break;
        case SETBLOCKIC:
#if	(DD_CHIPC==1)
            ProgICBlock();
#else
            ProgLineMes[0]=0;
            DispStrXY(Msg[BAOLIU].str,0,0);
#endif
            break;  //ccr chipcard 2004-07-01
        case SETPRINTIC://"打印卡信息"
            PrintICMessage();
            SETBIT(ApplVar.MyFlags,PRNHEADMUST);
            KeyFrHost=ApplVar.AP.FirmKeys[ID_CANCEL];
            break;
        case SETPRINTVIP://"打会员信息"
            PrintECRVIP(0);
            SETBIT(ApplVar.MyFlags,PRNHEADMUST);
            KeyFrHost=ApplVar.AP.FirmKeys[ID_CANCEL];
            break;
        case SETPRINTKEYB://"打印键盘"
            PrintKeyboard();
            SETBIT(ApplVar.MyFlags,PRNHEADMUST);
            KeyFrHost=ApplVar.AP.FirmKeys[ID_CANCEL];
            break;
        case SETCLEANVIP://"清除会员"
            if (WaitForYesNo(Msg[SETCLEANVIP].str,false))
                InitECRVIP();

            SETBIT(ApplVar.MyFlags,PRNHEADMUST);
            KeyFrHost=ApplVar.AP.FirmKeys[ID_CANCEL];
            break;
            //ccr chipcard>>>>>>>>>>>>>>>>>>>>>>>>>
        case SETCHARGIC:
#if	(DD_CHIPC==1)//ccr050316
            ProgChargeChip();
#else
            ProgLineMes[0]=0;
            DispStrXY(Msg[BAOLIU].str,0,0);
#endif
            break;
        case SETCLEARIC:
#if	(DD_CHIPC==1)//ccr050316
            ProgClearChip();
#else
            ProgLineMes[0]=0;
            DispStrXY(Msg[BAOLIU].str,0,0);
#endif
            break;
        case SETINITIC:
#if	(DD_CHIPC==1)//ccr050316
            ProgInitialChip();
#else
            ProgLineMes[0]=0;
            DispStrXY(Msg[BAOLIU].str,0,0);
#endif
            break;
        case SETCHIPPOINT:
#if (DD_ECRVIP==1)
            if (TESTBIT(ECRVIP.ECRVIPState,VIP_LOGIN))
                ProgECRVIPPoints();
            else
#endif
#if (DD_CHIPC==1)
                ProgPoints();
#else
                strcpy(ProgLineMes,Msg[BAOLIU].str);
#endif
            break;
            //ccr chicard<<<<<<<<<<<<<<<<<<<<<<<<<<<

        case SETIP:
#if (defined(TCPIP) || defined(CASE_GPRS))
            ProgIP();    //ccr chipcard 2004-06-28
#else
            ProgLineMes[0]=0;
            DispStrXY(Msg[BAOLIU].str,0,0);
#endif
            break;

#if !defined(DD_LOCK)// ccr100707>>>>>>>>>>
        case SETSETPWD:
        case SETXPWD:
        case SETZPWD:
        case SETMGPWD://ccr2015-01-28
            Save_Config(true);
            return 1;
#endif
        case SETCONFIGPLU:
            ProgPLUFields();
            break;
        case SETCONFIGDEPT:
            ProgDEPTFields();
            break;
        case SETPOINTTYPE:
            ProgPointType();
            break;
        case SETADDNEWVIP:
            ProgNewVIP();
            break;
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
        case SETEJOURNAL:
            ProgEJournal();
            break;
#endif
        default:
            Appl_ProgType = 0;
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            break;
        }
        //BitNumber的值有变化时,表示修改了选择项的值
        if (!ApplVar.ErrorNumber && sSaveConfig!=Appl_BitNumber)
            Save_Config(true);//将设置区的变量存入掉电保护区

        // if add a new setup,update this line if needed;
        if (Appl_ProgLine    //>0,为合法的记录设置操作,而以下的设置无记录的概念
            || SetList.isSub == SETPLU  //为销售模式下增加新单品,修改完一条记录后直接退出
            || Appl_ProgType == SETSYSFLAG
            || Appl_ProgType == SETKEYB
            || Appl_ProgType == SETKEYMASK
            || Appl_ProgType == SETPRINTKEYB
            || Appl_ProgType == SETPRINTIC //打印完IC卡信息后立即退出
            || Appl_ProgType == SETPRINTVIP //打印完会员信息后立即退出
            || Appl_ProgType == SETCLEANVIP //清除会员后立即退出
            || Appl_ProgType >= SETCLEARIC)//ccr chipcard 040805
            break;
        Appl_ProgLine = 1;//重新开始对当前记录的设置
    }
    if (Appl_ProgStart == 2)
        ProgLineMes[sizeof(ProgLineMes) - 1] = 0;
    return 1;
}


//CheckFirmKey return true(1) If the input from keyboard processed by CheckFirmKey
//else return false(0);
//只在非销售模式下在会调用CheckFirmKey
WORD CheckFirmKey()
{
    WORD  i;
    BYTE    keyno,sType;;

    keyno = ApplVar.KeyNo;
    if (keyno==ApplVar.AP.FirmKeys[ID_CLEAR])    /* firm key clear key */
    {
        ApplVar.Key.Code = CLEAR;
        return 0;
    }

    if (((ApplVar.CentralLock & 0xff00)==PWDFOR) || ApplVar.ErrorNumber)
        return TRUE;
    if (Appl_EntryCounter)
    {
        ApplVar.Entry.Sign=0;
        StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
        GetWordEntry();
    }
#if (PLUCODE_X==1)
    if (Plu_XReport(keyno))
        return true;
#endif
    if (Appl_EntryCounter && Appl_ProgType == SETKEYB && Appl_ProgLine == 2)
        i = ID_ENTER;    /* simulate enter key */
    else if (Appl_ProgType == SETKEYB && Appl_ProgLine == 4)
    {
        i = ID_ENTER;
        Appl_ProgLine--;
    }
    else
    {
        i = GetFirmkeyID(keyno);
    }
    if (ApplVar.FuncOnEnter!=0 && i!=ID_CANCEL)
    {
        ApplVar.KeyNo = 0;
        return true;
    }
    if ((ApplVar.FRegi || ApplVar.FInv) && i < FIRMKEYS
         && SetList.isSub != SETPLU)//不是销售中增加单品处理
    {
        ApplVar.ErrorNumber=ERROR_ID(CWXXI14);
        return 1;
    }
    if (!Appl_EntryCounter)
    {
        if (ApplVar.NumberEntry<0)//special use for fCANCEL
            ApplVar.NumberEntry = abs(ApplVar.NumberEntry);
        else
            ApplVar.NumberEntry = 0;
    }

    switch (i)//(ApplVar.AP.FirmKeys[i])
    {
    case ID_SELECT:     /* program type key */
        if (ApplVar.CentralLock & 0xf00)
        {
            if ((ApplVar.CentralLock & 0xf00)!=PWDFOR)
            {
                ClsArea(PROMDISPX,SCREENLN,SCREENWD-PROMDISPX);//ccr2015-01-28
                Appl_ProgType -= (SetList.SubNo-1);//ccr2015-01-28
            }
            ApplVar.CentralLock = SET;
            return TRUE;
        }
        if (ApplVar.CentralLock==X || ApplVar.CentralLock==Z)
        {// use fPRGTYPE key to select a ApplVar.Report file, display the file name at State2
            ApplVar.ReportNumber++;
            if (ApplVar.ReportNumber>SetList.RecMax)
                ApplVar.ReportNumber = 1;

            if (Appl_EntryCounter)
            {
                if (ApplVar.NumberEntry==0 || ApplVar.NumberEntry>SetList.RecMax)
                {
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
                    break;
                }
            }
            else
                ApplVar.NumberEntry = ApplVar.ReportNumber;

            ApplVar.ReportNumber = ApplVar.NumberEntry;
            DisplaySetup(ID_SELECT);   //ID_SELECT翻页显示

            ClearEntry(false);
            return 1;
        }
        else if (Appl_ProgType>0 && Appl_ProgLine>0 && Appl_ProgStart>0)// && Appl_BitNumber)
        {
            /* change option on/off */
            if (Appl_BitNumber)     /* invert option ? */
                Appl_EntryCounter = 1;   /* set invert by setting entrycounter */
            else
            {
                ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
                break;
            }
            Appl_BitNumber++;
            Appl_ProgStart = 1;
            if (!DisplayOption())
                return 0;
            if (!ApplVar.ErrorNumber)
            {
                ClsArea(Editor.DispX,CursY,Editor.DispW);
                DispStrXY(ProgLineMes,Editor.DispX,0xff);

//ccr2016-06-07                ClsArea(SETUPDISPX,CursY,SETINPUTWD);//!!!!SETUPDISPX-SETINPUTWD
//ccr2016-06-07                DispStrXY(ProgLineMes,SETUPDISPX,0xff);
            }
            break;
        }
#if (0)//ccr20130418>>>>>>>>>>>>
        else if (ApplVar.MultiplyCount == 1)
        {
            i = 0;
            switch (ApplVar.NumberEntry)
            {
            case 91:    /* Receipt Number */
                memcpy(ApplVar.ReceiptNumber, ApplVar.Qty1.Value,RECNUM);
                i++;
                break;
            case 94:  /* Training Number */
                memcpy(ApplVar.ReceiptNumber+RECNUM, ApplVar.Qty1.Value,TRAINNUM);
                i++;
                break;
            case 95:            /* Z- Counter */
                memcpy(ApplVar.ZCount, ApplVar.Qty1.Value,ZSIZE);
                i++;
                break;
            case 96:                   /* Auto Check Number*/
                BCDValueToULong(ApplVar.Qty1.Value, &ApplVar.AutoCheck) ;
                i++;
                break;
            }
            if (i)
            {
                Bell(0);
                break;
            }
        }
#endif//<<<<<<<<<<<<<<<<<<
        if (ApplVar.NumberEntry > 102 && ApplVar.NumberEntry < 108)  /* Check ApplVar.Plu File */
        {
            if (ApplVar.NumberEntry == 107 && ApplVar.CentralLock != SET)
                ApplVar.ErrorNumber=ERROR_ID(CWXXI01);    /* repair only in Z or SET */
            else
                CheckPluFile(ApplVar.NumberEntry);
            break;
        }

        if (ApplVar.NumberEntry)
        {
            Appl_ProgType = ApplVar.NumberEntry-1;
            Appl_ProgStart = 0;
        }

        if (!Appl_ProgType || !Appl_ProgStart)// if add a new setup,update this line ;
        {
            (Appl_ProgType >= SetList.RecMax || !Appl_ProgType)?(Appl_ProgType = 1):(Appl_ProgType++);
        }
        Appl_MaxEntry=14;//ccr2014-09-09 PLUCODE_X

        DisplaySetup(ID_SELECT); ////ID_SELECT翻页显示,

        Appl_ProgStart = 0;
        Appl_ProgNumber = 0;
        Appl_ProgLine = 0;
        Appl_BitNumber = 0;  //
        break;
    case ID_ENTER:     /* enter key */
        RESETBIT(ApplVar.MyFlags,MENUMODE);
        if (ApplVar.CentralLock == X || ApplVar.CentralLock == Z)
        {/* select report for X/Z by the input */
            if (Appl_EntryCounter)
            {
                if (ApplVar.NumberEntry==0 || ApplVar.NumberEntry>SetList.RecMax)
                {
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
                    break;
                }

                ApplVar.ReportNumber = ApplVar.NumberEntry;
            }
            if (ApplVar.CentralLock == X)
            {
                ProcessXFunc(ApplVar.ReportNumber-1+MENUXREPORT);
            }
            else// (ApplVar.CentralLock == Z)
            {
                ProcessZFunc(ApplVar.ReportNumber-1+MENUZREPORT);
            }
            ApplVar.ReportNumber = 1;
            DisplaySetup(0xff);//重新显示Z/X报表菜单,将默认的菜单项显示在第一行
            break;
        }
        else if (ApplVar.CentralLock==SET)        //hf 20060622
        {
            if (!SetList.isSub)  //main menu
            {
                //进入下一级菜单
                SetList.MsgIdx = SetList.SubNo;//子菜单信息表
                SetList.RecMax = SetList.SubMax;
                SetList.Rec1st = 0;
                SetList.Var = &Appl_ProgType;
                SetList.isSub = TRUE;   //sub menu
                SetupList = &DisplaySetup;
                ClsState12();
                DisplaySetup('1');
                Appl_MaxEntry=14;//ccr2014-09-09 PLUCODE_X

                Appl_ProgStart = 0;

                break;
            }
            else  if (SetList.isSub!=SETPLU)//不是销售模式下的增加PLU
            {//在设置功能的二级菜单下,选择一个设置项按确认,进入收款机文件数据的设置功能///
                if (!Appl_ProgStart)
                {//初始化与设置有关的控制变量
                    //生成实际设置功能号,进入Appl_ProgType指定的文件设置功能
                    Appl_ProgType = *(SetList.Var) + (SetList.SubNo-1);

                    Appl_ProgNumber = 0;
                    Appl_ProgLine = 0;
                    Appl_BitNumber = 0;
                    Appl_EntryCounter = 0;
                }
            }

            if (Appl_ProgType)//ccr chipcard
            {
                if (Appl_ProgLine==1 && Appl_EntryCounter)// && Appl_ProgStart>0)
                {//根据输入数据选择记录号
                    /* program number key,select spacified PLU/DEPT/.... */
                    if (!Appl_ProgStart)
                    {
                        CheckMultiply();
                        break;
                    }
                    ApplVar.Entry.Sign = 0;
                    if (Appl_ProgType == SETPLU && ApplVar.AP.Plu.RandomSize)
                    {  //输入单品编码+确认后,根据编码查找单品
                        ApplVar.PluNumber = 0;
                        if (Appl_EntryCounter <= (ApplVar.AP.Plu.RandomSize * 2))
                        {

                            DispStrXY(PLUCODE,0,0);//ccr2016-01-18
                            DispStrXY(EntryBuffer + ENTRYSIZE - Appl_EntryCounter - 1,SCREENWD-14,1);//ccr2016-01-18

                            ApplVar.PluNumber = GetPluNumber(1, ApplVar.Entry.Value);
                            if (!ApplVar.PluNumber)       /* not found then add ? */
                            {//检查单品不存在
                                if (!CheckRandomPlu(0, 0))       /* 不存在时提示是否增加add */
                                {
                                    KeyFrHost=ApplVar.AP.FirmKeys[ID_CANCEL];//ccr2016-06-12 不增加单品时,回到设置菜单
                                    break;
                                }
                                ApplVar.PluNumber = GetPluNumber(0,ApplVar.Entry.Value);
                            }
                            else if (ApplVar.CentralLock != X)  /* ask for delete ? */
                            {
                                ApplVar.NumberEntry = ApplVar.AP.Plu.RNumber;//ccr2016-06-12
                                CheckRandomPlu(1, 0);   /* delete? */
                                if (ApplVar.AP.Plu.RNumber<ApplVar.NumberEntry)//ccr2016-06-12 删除了单品后,回到设置菜单
                                    KeyFrHost=ApplVar.AP.FirmKeys[ID_CANCEL];//ccr2016-06-12
                            }
                            ApplVar.NumberEntry = ApplVar.PluNumber;
                            ClearLine(0);ClearLine(1);// mClearScreen();
                        }
                        else
                            ApplVar.NumberEntry = 0;
                    }
                    /* 此段代码无用>>>>>>>>
                    if (Appl_ProgType==SETDATE || Appl_ProgType==SETTIME || Appl_ProgType==SETIP)
                    {
                        Appl_ProgLine = 3;//set date or time
                        DisplayOption();
                        ClearEntry(false);
                        return 1;
                    }<<<<<<<<<< */
                    if (ApplVar.NumberEntry)
                        Appl_ProgNumber = ApplVar.NumberEntry - 1;
                    else
                    {//报告输入错误
                        ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
                        break;
                    }

                    Appl_BitNumber = 0;
                    Appl_EntryCounter = 0;
                    ApplVar.ErrorNumber=0;
                    Appl_ProgStart = 1;
                }
                keyno = 0;
                switch (Appl_ProgType)
                {
                case SETCLEARPLU:
                    if (WaitForYesNo(Msg[SETCLEARPLU].str,false))
                        ApplVar.AP.Plu.RNumber=0;
                    keyno=3;// 对于无具体设置内容的设置功能,执行完功能后,必须运行以下程序段,准确回到主菜单
                    break;
#if (CASE_USBDISK)//SETUP
                case SETINSTALL:
					MenuMain(MENUUSBMAINIDX);//新增加的数据导出菜单处理程序

                    // 回到主菜单>>>>>>
                    DisplaySetup2ndMenu(MENUID_SERVICE);
                    ClearEntry(false);
                    return 1;
#endif

#if (defined(CASE_GPRS))//ccr2014-11-11 NEWSETUP Step-6
                case SETGPRS:
					MenuMain(MENUGPRSSEND);//新增加的GPRS菜单处理程序

                    // 回到主菜单>>>>>>
                    DisplaySetup2ndMenu(MENUID_COMM);
                    ClearEntry(false);
                    return 1;
#endif
                case SETCLRREP://总清报表
                    if (WaitForYesNo(Msg[ZQBBIAO].str,true))
                        ClearAllReport();
                    keyno=3;// 对于无具体设置内容的设置功能,执行完功能后,必须运行以下程序段,准确回到主菜单>>>>>>
                    break;
                case SETKEYTYPE:
                    i = ListItems(SETREST,true,It_SELEKEYTYPE,true);
                    if (i==(SETREST-SETREST+1))
                    {
                        if (ApplVar.AP.Pb.Number>10)//ccr081210,程序具备餐饮功能
                            i = CANYIN;
                    }
                    else if (i==(SETMARKET-SETREST+1))
                        i = CHAOSHI;
                    if (i==CANYIN || i==CHAOSHI)
                    {
                        if (WaitForYesNo(Msg[i].str,true))
                        {
                            if (i==CANYIN)
                                memcpy((char*)&ApplVar.AP.KeyTable,(char*)&KeyTablePBF,sizeof(KeyTablePBF));
                            else
                                memcpy((char*)&ApplVar.AP.KeyTable,(char*)&Default.KeyTable,sizeof(Default.KeyTable));
                            RESETBIT(ApplVar.MyFlags, PRNONPB);
                        }
                    }
                    keyno=3;// 对于无具体设置内容的设置功能,执行完功能后,必须运行以下程序段,准确回到主菜单
                    break;
                //以下为特殊功能>>>>>>
#if !defined(DD_LOCK)//ccr2016-02-15>>>>>>>>>>
                case SETSETPWD:
                        if (!SetPassword(Msg[SETSETPWD].str,ApplVar.AP.ModePwd.PwdSET))
                            ApplVar.ErrorNumber=ERROR_ID(CWXXI36);

                        keyno=3;// 对于无具体设置内容的设置功能,执行完功能后,必须运行以下程序段,准确回到主菜单
                        break;
                case SETXPWD:
                    if (!SetPassword(Msg[SETXPWD].str,ApplVar.AP.ModePwd.PwdX))
                        ApplVar.ErrorNumber=ERROR_ID(CWXXI36);
                    keyno=3;// 对于无具体设置内容的设置功能,执行完功能后,必须运行以下程序段,准确回到主菜单
                    break;
                case SETZPWD:
                    if (!SetPassword(Msg[SETZPWD].str,ApplVar.AP.ModePwd.PwdZ))
                        ApplVar.ErrorNumber=ERROR_ID(CWXXI36);

                    keyno=3;// 对于无具体设置内容的设置功能,执行完功能后,必须运行以下程序段,准确回到主菜单
                    break;
                case SETMGPWD:
                    if (!SetPassword(Msg[SETMGPWD].str,ApplVar.AP.ModePwd.PwdMG))
                        ApplVar.ErrorNumber=ERROR_ID(CWXXI36);

                    keyno=3;// 对于无具体设置内容的设置功能,执行完功能后,必须运行以下程序段,准确回到主菜单
                    break;
#endif
                case SETPLULIST: PrintDept_PLU(); keyno=3;break;
                case SETPRNMENU: PrintSetupMenu();
                                 PrintXZMenu(X);
                                 PrintXZMenu(Z);
                                 PrintMGMenu();
                                 RFeed(6);//走到撕纸位值
                                 keyno=3;break;
                case SETPRTGRAP: PrintGraphics(); keyno=3;break;
                case SETPRTJOUR: ExploreLog();    keyno=3;break;

                //以下为测试功能,即老版本的300功能下的测试
                case SETTSTVER:  HardTest(BIT12);   keyno=3;break;
                case SETPRINTFAT:HardTest(BIT13);   keyno=3;break;
                case SETTEST:
                        do {
                            i = ListItems(SETTEST,0,It_SELETEST,true);
                            if (i>0 && i<=MENUTSTMAX)
                            {
                                i +=(SETTSTDISP-1);
                                switch (i)
                                {
                                case SETTSTDISP: HardTest(BIT1);   break;
                                case SETTSTCUST: HardTest(BIT14);   break;
                                case SETTSTPRT:  HardTest(BIT0);   break;
                                case SETTSTCLK:  HardTest(BIT3);   break;
                                case SETTSTMEM:  HardTest(BIT2);  break;
                                case SETTSTBELL: HardTest(BIT4);  break;
                                case SETTSTDRAW: HardTest(BIT5);  break;
                                case SETTSTKBD:  HardTest(BIT6);  break;
                                case SETTSTCOMM: HardTest(BIT10);break;
                                case SETTSTAUTO: HardTest(BIT11);break;
                                }
                            }
                        } while (i!=It_EXIT);
                        // 回到主菜单>>>>>>
                        //DisplaySetup2ndMenu(MENUID_SERVICE);
                        //ClearEntry(false);
                        //return 1;
                        keyno=3;
                        break;
                //<<<<<<<<<<<<<<<<<<<<<<<<<<<
                }
                if (keyno)
                {//为特殊功能
                    if (keyno==3)     //为系统测试功能
                    {   //针对执行了对屏幕进行保护或者屏幕内容没有被破坏的功能
                        // 对于无具体设置内容的设置功能,执行完功能后,必须运行以下程序段,准确回到主菜单>>>>>>
                        Appl_ProgType -= (SetList.SubNo-1);   //恢复为逻辑序号,用来控制DisplaySetup
                        Appl_ProgStart = 0;
                        Appl_ProgNumber = 0;
                        Appl_ProgLine = 0;
                        Appl_BitNumber = 0;
                        ClearEntry(false);
                        return true;
                    }
                    else
                        ApplVar.FuncOnEnter = 0;
                    KeyFrHost = ApplVar.AP.FirmKeys[ID_CANCEL];//当屏幕内容被破坏,用来控制显示主菜单
                    return true;
                }
            }// end of "if (ApplVar.CentralLock==SET && Appl_ProgType && Appl_EntryCounter)"
        }
        SetupList = &DisplayRecNo;
        if (!Appl_EntryCounter)
        {//没有输入数据时
            if (!Appl_ProgType || TESTBIT(ApplVar.MyFlags,SETCONFIRM))
            {
                break;
            }
            else //ccr040809
            { //不是对询问进行确认操作
                Appl_ProgLine++;
                Appl_BitNumber = 0;
                if (Appl_ProgLine>2)
                {
                    ScreenFeed(1);
                    if (CursY==SCREENLN-1)
                    {
                        ScrollScreen(1);
                        GotoXY(0,SCREENLN-2);
                    }
                    Editor.DispY = CursY;//ccr2016-08-29
                }
            }
        }
        else if (Appl_BitNumber || !Appl_ProgType)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            break;
        }
        Appl_ProgStart = 1;     //进入一个文件的设置状态

        if (!DisplayOption())//
            return 0;
        //当为销售模式下增加新单品的数据时,执行完DisplayOption后,如果Appl_ProgLine=0,说明数据输入完毕
        if (!ApplVar.ErrorNumber && Appl_ProgLine>0 && Appl_ProgType!=SETKEYB)
        {
            if (Appl_ProgLine>1)
            {//Appl_ProgLine>1时,输入的内容显示在屏幕内容区
                //显示当前设置项的数据
                ClsArea(Editor.DispX,Editor.DispY,Editor.DispW);//!!!!SETUPDISPX-SETINPUTWD
                DispStrXY(ProgLineMes,Editor.DispX,Editor.DispY);
            }
            if (Appl_EntryCounter)
            {
                Appl_ProgLine++;
                ScreenFeed(1);
                if (CursY==SCREENLN-1)
                {
                    ScrollScreen(1);
                    GotoXY(0,SCREENLN-2);
                }
                ClearEntry(false);
                if (!DisplayOption())//display next line for setup
                    return 0;
                if (!TESTBIT(ApplVar.MyFlags,CLSSCREEN))
                {
                    DispStrXY(ProgLineMes,Editor.DispX,0xff);//!!!!SETUPDISPX-SETINPUTWD
                }
            }
        }
        break;
    case ID_DUMP:   /* dump program key */
        ProgramDump();
        break;
    case ID_CANCEL://Appl_ProgType>0,退出当前菜单或设置功能
        RESETBIT(ApplVar.MyFlags,PWDMODE+HANZIMODE+SETCONFIRM+MENUMODE);//ccr040809
        //ccr chipcard>>>>>>>>>>>>>>>>>>>>>>>>>>
#if	(DD_CHIPC==1 || DD_ECRVIP==1)
        if (ApplVar.CentralLock == (SETUPMG | MG))
        {//cancel the verify of password mode,return to REGIS mode directly
            ApplVar.CentralLock = MG;
			Appl_ProgType = 0;
            ApplVar.IC.ICState &= IC_NOTREMOVED;
            ClearEntry(false);
            mClearScreen();
#if (SCRSTATDBL)
            DispOnStatDBL(ApplVar.ModeHead,0);
#else
            ClearLine(STATELINE);
            DispSt0(ApplVar.ModeHead);//RG-DISPLAY
#endif
            return MG;
        }
#endif
        //ccr chipcard<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
        if (ApplVar.CentralLock & PWDFOR)
        {//cancel the verify of password mode,return to REGIS mode directly
            ApplVar.CentralLock = RG;
            ClearEntry(false);
#if (SCRSTATDBL)
            DispOnStatDBL(ApplVar.ModeHead,0);
#else
            ClearLine(STATELINE);
            DispSt0(ApplVar.ModeHead);//RG-DISPLAY
#endif
            return RG;
        }
        SetupList = &DisplaySetup;
        if (Appl_ProgLine==0 && Appl_ProgStart==0)
        {//return to SET mode
            if (ApplVar.CentralLock==X || ApplVar.CentralLock==Z)
            {
                Appl_ProgType = 0;
                DisplaySetup(0xff);//将默认的菜单项显示在第一行
//                ClearLine(STATELINE);
//                DispSt0(ApplVar.ModeHead);
//                DispSt1(Msg[ANXZJIAN].str);
                Appl_MaxEntry=14;//ccr2014-09-09 PLUCODE_X
                break;
            }
            else
            {
                DisplaySetup1stMenu();

                ApplVar.FuncOnEnter = 0;
                ApplVar.CentralLock = SET;
                ApplVar.ModeHead = (char*)Msg[SHEZHI].str;

                DispSt0(ApplVar.ModeHead);

                Appl_MaxEntry=4;
                break;
            }
        }
        else
        {//return to last set mode

            if (Appl_ProgType && Appl_ProgLine>1 &&
                Appl_ProgType<SETVIPIC && Appl_ProgType>SETPRINTIC && //Like Date
                Appl_ProgType<SETCLEARIC && Appl_ProgType>SETCHIPPOINT && //Like Date
                Appl_ProgType<SETCOMPUTER && Appl_ProgType>SETIP && //Like Date
                Appl_ProgType!=SETDATE && Appl_ProgType!=SETTIME  && Appl_ProgType!=SETSYSFLAG)
            {//从正在设置的文件中退出,
                Appl_ProgNumber = 0;
                Appl_ProgLine = 0;
                Appl_BitNumber = 0;
                Appl_EntryCounter = 0;
                ApplVar.KeyNo = ApplVar.AP.FirmKeys[ID_ENTER];//fPRTYPE;
                CheckFirmKey();//返回到当前文件的第一个记录等待设置
            }
            else
            {
                mEnCursor(DISABLE);//关闭光标
                if (SetList.isSub && !Appl_ProgType)  //in setup option
                {//返回到主菜单
                    SetList.MsgIdx = SubMenuItems[0].MenuFrom;    // 20060105 SETDEPT;
                    SetList.RecMax = SubMenuItems[0].MenuItems;    //MENUMAX;
                    SetList.Rec1st = 0;         // first item of menu
                    SetList.Var = &Appl_ProgType;
                    SetList.isSub = FALSE;  // main menu
                    SetList.SubNo = SubMenuItems[1].MenuFrom;        //reset sub menu
                    SetList.SubMax = SubMenuItems[1].MenuItems;    //reset sub menu
                    Appl_ProgType = 1;
                }
                else
                {// 返回到当前菜单 //
#if defined(CASE_INDIA)
                    if ((Appl_ProgType==SETPLU || Appl_ProgType==SETDEPT) && Appl_ProgStart)
                    {
                        SavePlu_DeptEditLOG();
                    }
#endif
                    Appl_ProgType -= (SetList.SubNo-1);//恢复为逻辑序号,用来控制DisplaySetup
                }
                ApplVar.ErrorNumber=0;
                Appl_EntryCounter = 0;
                DisplaySetup(Appl_ProgType+'0');

                Appl_ProgStart = 0;
                Appl_ProgNumber = 0;
                Appl_ProgLine = 0;
                Appl_BitNumber = 0;
            }
            return 1;
        }
    case ID_UP:
    case ID_LEFT:
        if (!TESTBIT(ApplVar.MyFlags,MENUMODE)
            && ApplVar.CentralLock == SET && Appl_ProgStart && Appl_ProgLinePre>2
           )
        {   /*********ccr20130425 处在编辑记录数据的状态下,返回编辑上一个域的数据******/
            /*  在设置文件记录数据时,为了增加修改数据的灵活性,增加返回到上一个数据域 */
            /*   的功能:使用<-键放回到上一个数据域并对其进行修改                     */
            /*************************************************************************/

            Appl_ProgLine = Appl_ProgLinePre-1;//指向记录的前一个域

            if (ApplVar.sCursor.y>0)
            {//清除当前数据域的显示行
                ClearLine(ApplVar.sCursor.y);
                ApplVar.sCursor.y--;
                Editor.DispY = ApplVar.sCursor.y;//ccr20160601
            }
            ClearLine(ApplVar.sCursor.y);//清除上一个域的数据的显示
            Appl_EntryCounter=0;
            Appl_BitNumber = 0;
            if (DisplayOption() && !ApplVar.ErrorNumber)
            {
                DispStrXY(ProgLineMes,Editor.DispX,0xff);//!!!!SETUPDISPX-SETINPUTWD
            }
            return true;
        }
    case ID_DOWN:
    case ID_RIGHT:
        if (TESTBIT(ApplVar.MyFlags,MENUMODE))
        {
            (*SetupList)(i);
            return true;
        }
        else
            return false;
    default:
        if (ApplVar.Key.Code > CLERK-1 && ApplVar.Key.Code < OFFER)
            return 0;   /* clerk key always accepted */
        if (ApplVar.Key.C.High) /* function or entry */
            return 1;
        return 0;
    }
    if (Appl_ProgLine!=2 && !TESTBIT(ApplVar.MyFlags,HANZIMODE))//ccr2015-03-10 not in LineEditor
        ClearEntry(false);
    return 1;
}
//
BOOL CheckPassword()
{
    char *sMode;

    switch (ApplVar.CentralLock & 0xff)
    {
    case X:
        sMode = ApplVar.AP.ModePwd.PwdX;
        break;
    case Z:
        sMode = ApplVar.AP.ModePwd.PwdZ;
        break;
    case SET:
        sMode = ApplVar.AP.ModePwd.PwdSET;
        break;
    case MG:
        sMode = ApplVar.AP.ModePwd.PwdMG;
        break;
    default:
        return TRUE;
    }
    if (sMode[0]==0)
        return TRUE;
    else
        return(!strcmp(PwdInput,sMode));
}

//chang the work mode if Press MODELOCK key
//CheckMode return true(1) or simulated ApplVar.KeyNo If the input from keyboard processed by CheckMode
//else return false(0);
BYTE CheckMode()
{
    BYTE keyno;
    short   i;
    BYTE IssueMust=0;//是否要运行ReceiptIssue,进行了PLU编程（增加PLU）后，回到销售时,第一章票据上无EJ文档编号


#if defined(DD_LOCK)//ccr090526>>>>>>>>>>>>>>
    if (ApplVar.KeyNo==OFF)
        SuspendECR();
#endif//<<<<<<<<<<<<<<<<<<<<<<<<<<

    keyno=ApplVar.KeyNo;
    if (ApplVar.KeyNo == ApplVar.AP.FirmKeys[ID_RJFEED])
    {
        JFeed();
        return TRUE;
    }

    if (ApplVar.ErrorNumber)
        return FALSE;


#if defined(DD_LOCK)//ccr090526>>>>>>>>>>>>>>>>
#ifdef TR
    if (ApplVar.KeyNo==SET || ApplVar.KeyNo==X || ApplVar.KeyNo==Z || ApplVar.KeyNo==RG || ApplVar.KeyNo==MG || ApplVar.KeyNo==TR)
#else
    if (ApplVar.KeyNo==SET || ApplVar.KeyNo==X || ApplVar.KeyNo==Z || ApplVar.KeyNo==RG || ApplVar.KeyNo==MG)
#endif
    {//为方式锁键
        RESETBIT(ApplVar.ArrowsAlfa,BIT7);

        ApplVar.FuncOnEnter = 0;
        Editor.Source = 0;//ccr2015-03-11
        if (ApplVar.CentralLock == SET && Appl_ProgLine>0 || TESTBIT(ApplVar.MyFlags,PWDMODE))
        {//在设置时和输入口令时，禁止模式切换  //
            ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
            return 0;
        }
        ////ccr2017-02-27>>>>当切换到营业档时,不报告仍在销售中的错误
        if (ApplVar.FRegi && ApplVar.KeyNo!=RG && ApplVar.KeyNo!=MG)  //on the saling mode,can't chang the mode
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI14);
            return 0;
        }
        if (ApplVar.CentralLock==MG && ApplVar.FInv)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI47);
            return 0;
        }
        //ccr2017-01-24>>>>>>>>>>>>>>>
        if (ApplVar.CentralLock == SET && (ApplVar.KeyNo==RG || ApplVar.KeyNo==MG) && TESTBIT(ApplVar.MyFlags,PRNTRAI))
        {// 当从其它模式切换到销售模式时，先打印票头 //
            IssueMust=true;
        }//ccr2017-01-24<<<<<<<<<<<<<
        PwdInput[0]=0;
//			Collect_Data(LOCKLOG);	//lyq2003
#ifdef TR
        SETBIT(ApplVar.Clerk.Options, BIT7);    /* training clerk ? */
        ApplVar.FTrain = 0; /* reset training */
        RESETBIT(DOT,BIT1);//ccr2017-02-27
#endif
        switch (ApplVar.KeyNo)
        {
#ifdef TR
        case TR:
            RESETBIT(ApplVar.Clerk.Options, BIT7);  /* training clerk ? */
            ApplVar.FTrain = 1;   /* invert */
            SETBIT(DOT,BIT1);
#endif
        case RG:
            Save_Config(true);
            mClearScreen();
            keyno = RG;
            ApplVar.CentralLock = RG;
            ApplVar.ModeHead = (char*)Msg[SHOUKUAN].str;
            Appl_MaxEntry=ENTRYSIZE-2;

            if (TESTBIT(CLERKFIX, BIT0))    /* clerk compulsory or secret, no key */
                ApplVar.ClerkNumber = 0;
#if (salNumber)
            if (TESTBIT(KEYTONE, BIT6))
                ApplVar.SalPerNumber = 0;
#endif
            Appl_ProgType = Appl_ProgLine = 0;
            break;
        case MG:
            Save_Config(true);
            mClearScreen();
            keyno = MG;
            ApplVar.ModeHead = (char*)Msg[JINLI].str;
            ApplVar.CentralLock = MG;
            if (TESTBIT(CLERKFIX, BIT0))    /* clerk compulsory or secret, no key */
                ApplVar.ClerkNumber = 0;
#if (salNumber)
            if (TESTBIT(KEYTONE, BIT6))
                ApplVar.SalPerNumber = 0;
#endif
            if (!ApplVar.FRegi)//ccr2017-02-27
                MGFunctions();//ccr2016-02-16
            Appl_MaxEntry=ENTRYSIZE-2;
            Appl_ProgType = Appl_ProgLine = 0;
            break;
        case X:
            Save_Config(true);
            keyno = X;
            ApplVar.CentralLock = X;
            ApplVar.ModeHead = (char*)Msg[XBBIAO].str;
            DisplayXZMenu(X);
            Appl_MaxEntry=14;//ccr2014-09-09 PLUCODE_X
            break;
        case Z:
            Save_Config(true);
            keyno = Z;
            ApplVar.CentralLock = Z;
            ApplVar.ModeHead = (char*)Msg[ZBBIAO].str;
            DisplayXZMenu(Z);
            Appl_MaxEntry=2;
            break;
        case SET:
            Save_Config(true);
            if (ApplVar.AP.KeyTable[MAXKEYB-1]==0xffff)
                for (i=1; i <= ApplVar.AP.Pb.NumberOfPb; i++)
                {
                    PbTotal(i, 0);
                    if (ApplVar.PB.Block)
                    {//有挂起的桌台时,禁止进入SET   //
                        mClearScreen();
                        strcpy(SysBuf,cMESS_TABLEHOLD);
                        WORDtoASCL(SysBuf+6,i);
                        DispStrXY(SysBuf,0,0);
                        ApplVar.ErrorNumber=ERROR_ID(CWXXI14);
                        return 0;
                    }
                }
            RESETBIT(ApplVar.IC.ICState,IC_INSERT);//ccr040809
            DisplaySetup1stMenu();
            keyno = SET;
            ApplVar.CentralLock = SET;
            ApplVar.ModeHead = (char*)Msg[SHEZHI].str;
            break;
        default:
            return 0;
        }
        //ccr2017-01-24>>>>>>>>>>>>>>>>>
        if (IssueMust)
        {// 当从其它模式切换到销售模式时，先打印票头 //

            i = 0;
            if (!ApplVar.FReceipt)
            {
                i=1;
                ApplVar.FReceipt = 1;
            }
            ReceiptIssue(1);
            RESETBIT(ApplVar.MyFlags,PRNTRAI);
            if (i)
                ApplVar.FReceipt = 0;
        }//ccr2017-01-24<<<<<<<<<<<<<<<<<<<

#if (SCRSTATDBL)
        DispOnStatDBL(ApplVar.ModeHead,0);
#else
        ClearLine(STATELINE);
        DispSt0(ApplVar.ModeHead);//RG-DISPLAY
#endif

        RESETBIT(ApplVar.MyFlags,PWDMODE+HANZIMODE);

        ClearEntry(false);
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
        CheckEJ();
#endif
        if (ApplVar.CentralLock!=SET)
        //ccr2016-11-18>>>>>>>>
        {//确保XZ报表有票头
            if (TESTBIT(ApplVar.MyFlags,PRNHEADMUST))
                PrePrintHead(PREHEADER);
        }//ccr2016-11-18<<<<<<<

        return keyno;
    }
#endif//<<<<<<<<<<<<<<<<<<
    i = GetFirmkeyID(keyno);

    if (i>=FIRMKEYS)
        return FALSE;

    if (i!=ID_DATE || Appl_EntryCounter>0)
        InActive = 0;//Disable display datetime

    if (ApplVar.FuncOnEnter)
    {//进入了指定功能下的参数输入流程,CheckMode只接受取消或者确认参数的功能
        if (i==ID_CANCEL)
        {//退出参数输入
            mClearScreen();
            ApplVar.FuncOnEnter = 0;
            ApplVar.Key.Code = 0;
            ClearEntry(false);
            Appl_ProgLine = 0;
            Appl_ProgType -= (SetList.SubNo-1);//恢复为逻辑序号,用来控制DisplaySetup
            DisplaySetup(Appl_ProgType+'0');
            return true;
        }
        else if (i!=ID_ENTER)
        {//不是确认参数输入,退出
            ApplVar.Key.Code=0;
            return true;
        }
    }                                              //lyq added 2003\10\30 end

    switch (i)
    {
    case ID_CANCEL:
        if ((ApplVar.CentralLock & 0xf00) && (ApplVar.CentralLock & 0xf00)!=PWDFOR)
        {
            ClsArea(PROMDISPX,SCREENLN,SCREENWD-PROMDISPX);//ccr2015-01-28
            Appl_ProgType -= (SetList.SubNo-1);//ccr2015-01-28
            ApplVar.CentralLock = SET;
            return TRUE;
        }
        return false;
    case ID_xDEPT:    /* Alfa Shift 1,Xchange ASCII--QUWEI */
        if (ApplVar.CentralLock == RG || ApplVar.CentralLock ==MG)
        {
            //在销售状态下,切换部类
            INVERTBIT(ApplVar.ArrowsAlfa,BIT7);
            memset(SysBuf,' ',SCREENWD);
            CopyFrStr(SysBuf,Msg[SETDEPT].str);
            if (TESTBIT(ApplVar.ArrowsAlfa,BIT7))
            {
                WORDtoASC(SysBuf+7,ApplVar.DeptKeys+1);  SysBuf[8] = '-'; WORDtoASCL(SysBuf+9,ApplVar.DeptKeys*2);
            }
            else
            {
                WORDtoASC(SysBuf+7,1);   SysBuf[8] = '-'; WORDtoASCL(SysBuf+9,ApplVar.DeptKeys);
            }
#if (SCRSTATDBL)
            DispOnStatDBL(SysBuf,0);
#else
            DispSt2(SysBuf);//RG-DISPLAY
#endif
            return true;
        }
        return false;

    case ID_xINPUT://在汉字输入状态下,切换输入法
        if ((ApplVar.CentralLock == SET || ApplVar.CentralLock ==(SETUPMG | MG)) && TESTBIT(ApplVar.MyFlags,HANZIMODE))
        {
            TESTBIT(ApplVar.ScreenStat,HZINPUTMODE)?ChnInput(3):ChnInput(2);
            return TRUE;
        }
        return false;
    case ID_DATE:     /* date */
        if (ApplVar.CentralLock==RG)
        {
            InActive |= ACTIVE+1;     /* Inactive longer then allowed */
            CheckTime(true);
            InActive ^= 0x80;
            return TRUE;
        }
        else
            return FALSE;
    case ID_xPRINT:
        if (TESTBIT(DOT,BIT5) && !Appl_EntryCounter)
        {
            if (!TESTBIT(ApplVar.MyFlags,ENPRINTER))
            {
#if (SCRSTATDBL)
                DispOnStatDBL((char *)Msg[DYJGUAN].str,0);
#else
                DispSt1C(Msg[DYJGUAN].str);//RG-DISPLAY
#endif
                SETBIT(ApplVar.MyFlags,ENPRINTER);
            }
            else
            {
#if (SCRSTATDBL)
                DispOnStatDBL((char *)Msg[DYJGUAN].str,0);
#else
                DispSt1C(Msg[DYJKAI].str);//RG-DISPLAY
#endif
                RESETBIT(ApplVar.MyFlags,ENPRINTER);
            }
            return TRUE;
        }
        else
            return FALSE;
    case ID_ENTER:
        if (Appl_ProgType
            && !(ApplVar.CentralLock & 0xf00) //不是输入密码状态
            && !ApplVar.FuncOnEnter)//确保能够输入对应功能下所要求的输入
            return FALSE;

/*		if (TESTBIT(ApplVar.MyFlags,SETCONFIRM))//ccr040809
        {//confirm SETUP request.
            RESETBIT(ApplVar.MyFlags,SETCONFIRM);
            return FALSE;
        }*/
        if (ApplVar.CentralLock==SET)
        {//Can chang password for a mode if in SET mode

            switch (ApplVar.NumberEntry)
            {
#if (BARCUSTOMER==1)
            case 101:
                PrintBARCustomer(0);// 打印条码会员清单  //
                ClearEntry(false);
                SETBIT(ApplVar.MyFlags,PRNHEADMUST);
                return TRUE;
#endif
            case 550:       //diale over modem
                Appl_PortNumber = COMPUTER_1;
                ReadPort();
                ClearEntry(false);
                return TRUE;
            case 551:       //diale over modem
                Appl_PortNumber = COMPUTER_1;
                ReadPort();
                ClearEntry(false);
                return TRUE;
/////////////////////////////////////////////////////////////////////

            case 403://Print the message of the flow bill
                mClearScreen();
                DispStrXY("Head=",0,0);
                HEXtoASC(SysBuf,(char*)&ApplVar.FlowHeader.HeadP,3);
                SysBuf[6] = 0;
                DispStrXY(SysBuf,5,0);
                DispStrXY("Read=",12,0);
                HEXtoASC(SysBuf,(char*)&ApplVar.FlowHeader.ReadP,3);
                SysBuf[6] = 0;
                DispStrXY(SysBuf,17,0);

                DispStrXY("From=",0,1);
                HEXtoASC(SysBuf,(char*)&ApplVar.FlowHeader.FromAddr,3);
                SysBuf[6] = 0;
                DispStrXY(SysBuf,5,1);
                DispStrXY("End =",12,1);
                HEXtoASC(SysBuf,(char*)&ApplVar.FlowHeader.EndAddr,3);
                SysBuf[6] = 0;
                DispStrXY(SysBuf,17,1);

                DispStrXY("Next=",0,2);
                HEXtoASC(SysBuf,(char*)&ApplVar.FlowHeader.NextNP,3);
                SysBuf[6] = 0;
                DispStrXY(SysBuf,5,2);
                DispStrXY("MAX =",12,2);
                HEXtoASC(SysBuf,(char*)&ApplVar.FlowHeader.MAXNUMB,3);
                SysBuf[6] = 0;
                DispStrXY(SysBuf,17,2);

                for (i=0;i<3;i++)
                {
                    GotoXY(0,i);
                    GetStrFr(SysBuf);
                    RJPrint(0,SysBuf);
                }

                ClearEntry(false);
                SETBIT(ApplVar.MyFlags,PRNHEADMUST);
                return true;

#if (DD_CHIPC==1)//ccr050316
            case 600:                   //print message of te chipcard
                PrintICMessage();
                SETBIT(ApplVar.MyFlags,PRNHEADMUST);
                return TRUE;
#endif				//
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/*
            case 601://read data from Chipcard
            case 602:
                mClearScreen();
                if (CC_Insert())
                {
                    if (ApplVar.NumberEntry==602 || CC_Open(SysBuf))
                    {
                        if (CC_Read(SysBuf, (ApplVar.NumberEntry-601)*128, sizeof(SysBuf)))
                        {
                            PrnBuffer(SysBuf,sizeof(SysBuf));
                        }
                        else
                            DispStrXY("Error on Read",0,0);
                    }
                    else
                        DispStrXY("Error on Open",0,0);
//					CC_Close();
                }
                else
                    DispStrXY("No ChipCard",0,0);

//				ClearEntry(false);
                return true;
            case 603:
                if (CC_Open(SysBuf))
                {
                    SysBuf[0]=0x26;SysBuf[1] = 0xab;
                    if (CC_VerifyPWD(SysBuf))
                    {
                        keyno = Now.sec;
                        for (i=0;i<100;i++)
                            SysBuf[i]=keyno+i;
                        if (CC_Write(SysBuf, 128, 100))
                        {
//							CC_Open(SysBuf);
                            if (CC_Read(SysBuf, 128, 100))
                            {
                                PrnBuffer(SysBuf,100);
                            }
                            else
                                DispStrXY("Error on Read",0,0);
                        }
                        else
                                DispStrXY("Error on Write",0,0);
                    }
                    else
                        DispStrXY("Error on Password",0,0);
                }
                else
                    DispStrXY("Error on Open",0,0);

//				ClearEntry(false);
                return true;
            case 604:
                    SysBuf[0]=0x26;SysBuf[1] = 0xab;
                    if (CC_VerifyPWD(SysBuf))
                    {
                        SysBuf[0]=0xff;SysBuf[1] = 0xff;
                        CC_WritePWD(SysBuf);
                        DispStrXY("Update Password",0,0);
                    }
                    else
                        DispStrXY("Error on Password",0,0);
                return true;
*/
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

///////////////////////////////////////////////////////////////////
            default:
                if (Appl_EntryCounter)
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI32);       //illegle input
                return 0;
            }
            ClearEntry(false);       //
            return TRUE;
        }
        else
        {
           return 0;
        }
        break;
#if !defined(DD_LOCK)//ccr2015-01-28
    case ID_LOCK:       //change mode
        RESETBIT(ApplVar.ArrowsAlfa,BIT7);
        ApplVar.FuncOnEnter = 0;

        if (Appl_EntryCounter)
        {
            StrToBCDValue(ApplVar.Entry.Value, &EntryBuffer[ENTRYSIZE - 2], BCDLEN);
            GetWordEntry();
        }
        else
            ApplVar.NumberEntry = 0;

        //ccr2017-02-27>>>>当切换到营业档时,不报告仍在销售中的错误
        if (ApplVar.FRegi && (ApplVar.NumberEntry!=1 && ApplVar.NumberEntry!=5))
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI14);
            return 0;
        }//ccr2017-02-27<<<<<<<<<<<<<
        if (ApplVar.CentralLock==MG && ApplVar.FInv)
        {
            ApplVar.ErrorNumber=ERROR_ID(CWXXI47);
            return 0;
        }
        {//select new mode first
            //ccr2017-01-24>>>>>>>>>>>>>>>>>
            if (ApplVar.CentralLock == SET && ApplVar.NumberEntry != 3 && TESTBIT(ApplVar.MyFlags,PRNTRAI))
            {
                IssueMust=true;
            }//ccr2017-01-24<<<<<<<<<<<<<<<<
            PwdInput[0]=0;
            SetupList = &DisplayNull;
//			Collect_Data(LOCKLOG);	//lyq2003

            switch (ApplVar.NumberEntry)
            {
            case 0:
                if (Appl_EntryCounter>0 )
                {
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
                    return 0;
                }
                SuspendECR();
                keyno = 0;
                return 1;
            case 1:
                Save_Config(true);
                mClearScreen();
                keyno = RG;
                ApplVar.CentralLock = RG;
                i = SHOUKUAN;
                Appl_MaxEntry=ENTRYSIZE-2;
                if (TESTBIT(CLERKFIX, BIT0))    /* clerk compulsory or secret, no key */
                    ApplVar.ClerkNumber = 0;
#if (salNumber)
                if (TESTBIT(KEYTONE, BIT6))
                    ApplVar.SalPerNumber = 0;
#endif
                Appl_ProgType = Appl_ProgLine = 0;
                Editor.Source=NULL;//ccr2015-03-11
                RESETBIT(ApplVar.MyFlags,MENUMODE);
                break;
            case 5:
                Save_Config(true);
                if (ApplVar.CentralLock == MG || CheckPWD(Msg[SETMGPWD].str,ApplVar.AP.ModePwd.PwdMG))
                {
                    mClearScreen();
                    keyno = MG;
                    Editor.Source = NULL;//ccr2015-03-11
                    ApplVar.CentralLock = MG;
                    i = JINLI;
                    if (TESTBIT(CLERKFIX, BIT0))    /* clerk compulsory or secret, no key */
                        ApplVar.ClerkNumber = 0;
#if (salNumber)
                    if (TESTBIT(KEYTONE, BIT6))
                        ApplVar.SalPerNumber = 0;
#endif
                    Appl_MaxEntry=ENTRYSIZE-2;
                    Appl_ProgType = Appl_ProgLine = 0;
                    RESETBIT(ApplVar.MyFlags,MENUMODE);
                    break;
                }
                else
                {
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI36);       //illegle input
                    return 0;
                }
            case 2:
                Save_Config(true);
                if (ApplVar.CentralLock == X || CheckPWD(Msg[SETXPWD].str,ApplVar.AP.ModePwd.PwdX))
                {
                    keyno = X;
                    ApplVar.CentralLock = X;
                    DisplayXZMenu(X);
                    i = XBBIAO;
                    Appl_MaxEntry=14;//ccr2014-09-09 PLUCODE_X
                    break;
                }
                else
                {
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI36);       //illegle input
                    return 0;
                }
            case 3:
                Save_Config(true);
                if (ApplVar.CentralLock == Z || CheckPWD(Msg[SETZPWD].str,ApplVar.AP.ModePwd.PwdZ))
                {
                    keyno = Z;
                    ApplVar.CentralLock = Z;

                    DisplayXZMenu(Z);

                    i = ZBBIAO;
                    Appl_MaxEntry=2;
                    break;
                }
                else
                {
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI36);       //illegle input
                    return 0;
                }
            case 4: /* hf 4 modelock display default message*/
                Save_Config(true);
                if (ApplVar.CentralLock == SET || CheckPWD(Msg[SETSETPWD].str,ApplVar.AP.ModePwd.PwdSET))
                {
                    for (i=1; i <= ApplVar.AP.Pb.NumberOfPb; i++)
                    {
                        PbTotal(i, 0);
                        if (ApplVar.PB.Block)
                        {//有挂起的桌台时,禁止进入SET   //
                            mClearScreen();
                            strcpy((char*)ProgLineMes,cMESS_TABLEHOLD);
                            WORDtoASCL(ProgLineMes+6,i);
                            DispStrXY(ProgLineMes,0,0);
                            PRESS_ANY_KEY(1);
                            ApplVar.ErrorNumber=ERROR_ID(CWXXI14);
                        }
                    }
                    RESETBIT(ApplVar.IC.ICState,IC_INSERT);//ccr040809
                    //显示设置模式下的主菜单
                    DisplaySetup1stMenu();

                    keyno = SET;
                    ApplVar.CentralLock = SET;
                    Appl_MaxEntry=5;       // 2; 	//hf 20060105 4;
                    i = SHEZHI;
                    break;
                }
                else
                {
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI36);       //illegle input
                    return 0;
                }
            default:
                if (Appl_EntryCounter)
                    ApplVar.ErrorNumber=ERROR_ID(CWXXI32);       //illegle input
                return 0;
            }
            ApplVar.ModeHead = (char*)Msg[i].str;
            //ccr2017-01-24>>>>>>>>>>>>>>>>>>>>>>>
            if (IssueMust)
            {
                i = 0;
                if (!ApplVar.FReceipt)
                {
                    i=1;
                    ApplVar.FReceipt = 1;
                }
                ReceiptIssue(1);
                RESETBIT(ApplVar.MyFlags,(PRNTRAI));
                if (i)
                    ApplVar.FReceipt = 0;
            }
            //ccr2017-01-24<<<<<<<<<<<<<<<<<<<<<<<<<

#if (SCRSTATDBL)
            if (ApplVar.CentralLock==RG ||  ApplVar.CentralLock==MG)
            {
                DispOnStatDBL(EntryBuffer + ENTRYSIZE - SCRDBLWD - 1,0);
                DispOnStatDBL(ApplVar.ModeHead,0);
            }
            else
#endif
            {
                ClearLine(STATELINE);
                if (ApplVar.CentralLock==RG ||  ApplVar.CentralLock==MG)
                    ClearEntry(true);
                DispSt0(ApplVar.ModeHead);//RG-DISPLAY
            }

            if (ApplVar.CentralLock==SET)
                DispSt1((char*)cMESSPRNMENU);
            else//ccr2016-11-18>>>>>>>>
            {//确保XZ报表有票头
                if (TESTBIT(ApplVar.MyFlags,PRNHEADMUST))
                    PrePrintHead(PREHEADER);
            }//ccr2016-11-18<<<<<<<
            RESETBIT(ApplVar.MyFlags,PWDMODE+HANZIMODE);
            if (ApplVar.CentralLock == MG && !ApplVar.FRegi)//ccr2017-02-27
            {
                MGFunctions();//ccr2016-02-16
                Appl_ProgType = 1;
            }
            //PutsO(Msg[LING].str);
            ClearEntry(false);
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
            CheckEJ();
#endif
            return keyno;
        }
#endif//DD_LOCK

    default:
        return 0;
    }
}

//Suspend ECR if user press MODELOCK,Resume ECR until user press MODELOCK
void SuspendECR()
{
    BYTE keyno,Befl;

    Bios_1(BiosCmd_BuzzS);
    Bios_1(BiosCmd_SaveDisp0);
    Bios_1(BiosCmd_ClrFip0);

    SuspendScreen();
    SETBIT(ApplVar.MyFlags,LOCKMODE);//处理开关机时屏幕显示内容问题  //

    mLightLCDOff(); //
#if !defined(CASE_ASCIIDISPLAY) && !defined(DEBUGBYPC)
    LED_Off();
#endif

    do
    {
        while (!KbHit());   //--------------------------------
        keyno = Getch();        /* read key */
        Bios_1(BiosCmd_AutoClickOff);
#if defined(DD_LOCK)//ccr090526>>>>>>>>>>>>>>>>
#ifdef TR
        if (keyno==SET || keyno == X || keyno == Z || keyno == RG || keyno == MG || keyno==TR)
#else
        if (keyno==SET || keyno == X || keyno == Z || keyno == RG || keyno == MG)
#endif
        {
            ApplVar.KeyNo = keyno;
            break;
        }
#endif//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#if (0)//ccr2016-08-22!defined(DEBUGBYPC)
        /*在锁屏状态下,如果按退出键,则关闭GPIOA,允许使用JLINK下载程序*/
        if (keyno==Default.FirmKeys[ID_CANCEL])
        {
            if (Befl ==0)
                RESETBIT(KEYTONE,BIT0);
            Save_ApplRam();
            mDispStringXY("For JLink!",0,0);
            GPIO_DeInit(GPIOA);
			while(1){};
        }
#endif
    }  while (keyno != ApplVar.AP.FirmKeys[ID_LOCK]);
    RESETBIT(ApplVar.MyFlags,LOCKMODE);

    mLightLCDOn();  //hf
#if !defined(CASE_ASCIIDISPLAY) && !defined(DEBUGBYPC)
        LED_On();
#endif

    if (!TESTBIT(KEYTONE,BIT0))
        Bios_1(BiosCmd_AutoClickOn);

    Bios_1(BiosCmd_RestDisp0);
    Bios_1(BiosCmd_BuzzS);
    for (Befl=0;Befl<SCREENLN-1;Befl++)
        mRefreshLine(Befl);
}

