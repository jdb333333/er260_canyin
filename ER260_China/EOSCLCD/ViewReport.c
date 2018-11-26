
/*********************************************************************************
*  程序编写:陈初荣                                                               *
*  编写日期:2009年7月至7月                                                       *
*  修改日期:2013年9月 (STM32-Cortex-M3) 										 *
*  在导出报表或流水数据时,如果文件存在,就把新数据追加到文件末尾,并不写入标题	 *
*  如果是新文件,则在第一行写入标题  											 *
*  导出功能函数将使用以下共用存储区来作为工作区:								 *
*  1.EntryBuffer:导出报表时,用来暂存报表数据的日期/收款机号/店铺号 			     *
*  2.CCDBuff:供FormatToCSV函数暂存格式化数据		                             *
*  3.SysBuf:临时工作区.导出流水数据时,用来存储流水数据的日期/收款员/营业员等信息 *
*  4.ProgLineMes:临时工作区                                                      *
*********************************************************************************/
#define USBDISKAPP_C


#include "king.h"                               /* Kingtron library and I/O routines */

#include "exthead.h"
#include "exth.h"
#include "flowbill.h"

#include "ViewReport.h"


/***************************************************************
  Definitions for Disk IO
***************************************************************/
BYTE FolderString[15]  = {0};//存放导出文件的保存文件夹(LR******)

//////////////////////////////////////////////////////////////////////////////////////
BYTE TitleFlag;//用来控制是否写入报表标题
//////////////////////////////////////////////////////////////////////////////////////

//ccr20130827>>>>>>>>>
////////////////////////////////////////////////////////////////////////////////////
// 以下为导出报表数据的功能函数集,报表数据的格式采用CSV文件格式,数据项之间用','隔开
// 报表数据的导出将使用PCBffer做数据缓存  //
///////////////////////////////////////////////////////////////////////////////////
#define LEFTALIGN		20		// 第一个统计数据离左端的距离 //
#define NUMWIDTH		13		// 数字域宽度 999999999.99 //

// 打印报表的标题栏,使用SysBuf //
void usbPrintTitlel()
{
	int i;

	if (TitleFlag)
		return;
	TitleFlag = 1;

	memset(SysBuf,' ',sizeof(SysBuf));
    strcpy(SysBuf,cUSBREPNAME);
    i = strlen(SysBuf);
    SysBuf[i++]=',';

	if (ApplVar.Size.Cc)
	{
		strcpy(SysBuf+i, Msg[JIAOYISHU].str);
		i = strlen(SysBuf);
        SysBuf[i++]=',';

	}
	if (ApplVar.Size.Qty)
	{
		strcpy(SysBuf+i, Msg[SHULIANG].str);
        i = strlen(SysBuf);
        SysBuf[i++]=',';
	}
	if (ApplVar.Size.RetQty)
	{
		strcpy(SysBuf+i, Msg[TUIHUO].str);
        i = strlen(SysBuf);
        SysBuf[i++]=',';
	}
	if (ApplVar.Size.Amt)
	{
		strcpy(SysBuf+i, Msg[JINE].str);
        i = strlen(SysBuf);
        SysBuf[i++]=',';
	}
	if (ApplVar.Size.Disc)
	{
		strcpy(SysBuf+i, Msg[SETDISC].str);
        i = strlen(SysBuf);
        SysBuf[i++]=',';
		strcpy(SysBuf+i, Msg[JINGE].str);
        i = strlen(SysBuf);
        SysBuf[i++]=',';
	}
	if (ApplVar.Size.Cost)
	{
		strcpy(SysBuf+i, Msg[CHENBEN].str);
        i = strlen(SysBuf);
        SysBuf[i++]=',';
		strcpy(SysBuf+i, Msg[MAOLI].str);
        i = strlen(SysBuf);
        SysBuf[i++]=',';
	}

	if (ApplVar.Report.Type == 8 || ApplVar.Report.Type == 13)
	{
		if (ApplVar.Report.PointerType == 1)
		{
			strcpy(SysBuf+i, cSetupCaption38);
            i = strlen(SysBuf);
            SysBuf[i++]=',';
		}
		else if (ApplVar.Report.PointerType == 5)
		{
			strcpy(SysBuf+i, cSetupCaption39);
            i = strlen(SysBuf);
            SysBuf[i++]=',';
		}
	}

	if (i)
	{
		strcpy(SysBuf+i,cCSVREPTITL);
		i = strlen(SysBuf);//		CWORD(SysBuf[i])=0x0a0d;
		if (U_WriteFile(SysBuf, i)==-1)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI79);
	}

}

//-------------------------------------------------------------------
//将统计数据以string格式存入CCDBuff,其左端在LEFTALIGN范围内存放有项目名称
void usbPrintTotal()
{
	BYTE saveTaxNumber; 	//cc 20070306
	BCD saveAmt,TaxRate,saveDiscAmt;	//cc 20070306
	int i=LEFTALIGN+NUMWIDTH;

	memset(CCDBuff+i,' ',sizeof(CCDBuff)-i);
	if (ApplVar.Size.Cc)
	{//qty数据
		FormatBCD(CCDBuff+i, &ApplVar.Total.Cc,(BCDLEN * 2) + 0x6400);
        CCDBuff[++i] = ',';
		i += NUMWIDTH;
	}
	if (ApplVar.Size.Qty)
	{//qty数据
		FormatBCD(CCDBuff+i, &ApplVar.Total.Qty,(BCDLEN * 2) + 0x6400);
        CCDBuff[++i] = ',';
		i += NUMWIDTH;
	}
	if (ApplVar.Size.RetQty)
	{//qty数据
		FormatBCD(CCDBuff+i, &ApplVar.Total.RetQty,(BCDLEN * 2) + 0x6400);
        CCDBuff[++i] = ',';
		i += NUMWIDTH;
	}
	if (ApplVar.Size.Amt)
	{
		if (ApplVar.Report.Type == 10)       /* tax report ? */
		{
			FormatBCD_DEC(CCDBuff+i, &ApplVar.Total.Amt,1);//两位小数
            CCDBuff[++i] = ',';
			i += NUMWIDTH;
		}
		else if (ApplVar.Report.Type == 11 && (ApplVar.PbF.Options & 0x0f) == 10)       /* ApplVar.PB info report */
		{
            FormatBCD_DEC(CCDBuff+i, &ApplVar.Total.Amt,2);
            CCDBuff[++i] = ',';
			i += NUMWIDTH;
		}
		else if (!ApplVar.Size.Disc)
		{
			if (ApplVar.Report.Type == 6)    /* drawer report */
			{
//ccr2014-09-03				if (TESTBIT(ApplVar.Draw.Options, BIT1))
//ccr2014-09-03				{
//ccr2014-09-03					FormatBCD(CCDBuff+i, &ApplVar.Total.Amt,(BCDLEN * 2) + 0x6400);
//ccr2014-09-03                    CCDBuff[++i] = ',';
//ccr2014-09-03					i += NUMWIDTH;
//ccr2014-09-03				}
//ccr2014-09-03				else
				{
					if (TESTBIT(ApplVar.Draw.Options, BIT2))
						ApplVar.AmtDecimal = 2;//Decimal=3
					else
						ApplVar.AmtDecimal = 0;//Decimal=2
					ApplVar.AmtDecimal += ApplVar.Draw.Options & BIT0;
//ccr2014-12-11					if(NO_DECIMAL==1 && TESTBIT(COMMA,BIT0))//cc
//ccr2014-12-11						ApplVar.AmtDecimal = 1;//cc,no Decimal

                     if (ApplVar.AmtDecimal == 2)
                        FormatBCD_DEC(CCDBuff+i, &ApplVar.Total.Amt,3);//Decimal Length=3 (bit15)
                    else if (ApplVar.AmtDecimal == 3)
                        FormatBCD_DEC(CCDBuff+i, &ApplVar.Total.Amt,1);//Decimal Length=1 (bit15,bit11)
                    else
                        FormatBCD_DEC(CCDBuff+i, &ApplVar.Total.Amt,2);//Decimal Length=2

                    CCDBuff[++i] = ',';
					i += NUMWIDTH;
					ApplVar.AmtDecimal = NO_DECIMAL;
				}
			}
			else
			{
                FormatBCD_DEC(CCDBuff+i, &ApplVar.Total.Amt,2);//两位小数
                CCDBuff[++i] = ',';
				i += NUMWIDTH;
			}
		}
		else
		{
			ApplVar.Amt = ApplVar.Total.Amt;
			Subtract(&ApplVar.Amt, &ApplVar.Total.Disc);       /* calculate gross */
			if(((ApplVar.Total.Amt.Sign & 0x80)==0) && ((ApplVar.Total.Disc.Sign & 0x80)!=0))
				ApplVar.Amt.Sign = ApplVar.Total.Amt.Sign;

            FormatBCD_DEC(CCDBuff+i, &ApplVar.Amt,2);
            CCDBuff[++i] = ',';
			i += NUMWIDTH;
		}
	}
	if (ApplVar.Size.Disc)
	{
		FormatBCD_DEC(CCDBuff+i, &ApplVar.Total.Disc,2);
        CCDBuff[++i] = ',';
		i += NUMWIDTH;
		FormatBCD_DEC(CCDBuff+i, &ApplVar.Total.Amt,2);
        CCDBuff[++i] = ',';
		i += NUMWIDTH;
	}
	if (ApplVar.Size.Cost)
	{
		if (ApplVar.Report.Type == 10)
		{
            FormatBCD_DEC(CCDBuff+i, &ApplVar.Total.Disc,2);
            CCDBuff[++i] = ',';
			i += NUMWIDTH;

			FormatBCD_DEC(CCDBuff+i, &ApplVar.Total.Cost,2);
            CCDBuff[++i] = ',';
			i += NUMWIDTH;
		}
		else
		{
            FormatBCD_DEC(CCDBuff+i, &ApplVar.Total.Cost,2);
            CCDBuff[++i] = ',';
			i += NUMWIDTH;

            ApplVar.Amt = ApplVar.Total.Amt;

			Subtract(&ApplVar.Amt, &ApplVar.Total.Cost);

			FormatBCD_DEC(CCDBuff+i, &ApplVar.Amt,2);
            CCDBuff[++i] = ',';
			i += NUMWIDTH;
		}
	}
    i -= NUMWIDTH;//pointo ','
	if (ApplVar.Report.Type == 8 || ApplVar.Report.Type == 13)
	{
		if (ApplVar.Report.PointerType == 1)
		{
            i++;
			strcpy(CCDBuff+i, ApplVar.Clerk.Name);
            i = strlen(CCDBuff);
		}
		else if (ApplVar.Report.PointerType == 5)
		{
            i++;
			strcpy(CCDBuff+i, ApplVar.SalPer.Name);
            i = strlen(CCDBuff);
		}
	}

	if (i>LEFTALIGN)
	{
        CCDBuff[i++]=',';//
        strcpy(CCDBuff+i,EntryBuffer);//追加日期/收款机/店铺
        i = strlen(CCDBuff);
		CWORD(CCDBuff[i])=0x0a0d;
		if (U_WriteFile(CCDBuff, i+2)==-1)
            ApplVar.ErrorNumber=ERROR_ID(CWXXI79);
	}
}




void usbPrintReportType()
{
    int i;

    memset(SysBuf, ' ', sizeof(SysBuf));
    memcpy(SysBuf + 4, ApplVar.TXT.ReportType[ApplVar.Report.Type], sizeof(ApplVar.TXT.ReportType[0]));
    i = strlen(SysBuf);
    CWORD(SysBuf[i])=0x0a0d;
    if (U_WriteFile(SysBuf, i+2)==-1)
        ApplVar.ErrorNumber=ERROR_ID(CWXXI79);
}
//获取报表数据项目名称:柜组,存入CCDBuff的左端在LEFTALIGN范围内
void usbPrintGroup(WORD number)
{
    ApplVar.GroupNumber = number;
    ReadGroup();

    if (ApplVar.AP.Group.CapSize)
	{
		ApplVar.Group.Name[sizeof(ApplVar.Group.Name)-1] = 0;
		CopyFrStr(CCDBuff,ApplVar.Group.Name);
        CCDBuff[strlen(ApplVar.Group.Name)]=',';
	}
}
//获取报表数据项目部类名称,存入CCDBuff的左端在LEFTALIGN范围内
void usbPrintDept(WORD number)
{
    ApplVar.DeptNumber = number;
    ReadDept();
/*
    if (ApplVar.AP.Dept.RandomSize)
    {
		if (!SysBuf[0])
		    SysBuf[0] = ' ';
		Htoa(SysBuf + 10, ApplVar.Dept.Random, ApplVar.AP.Dept.RandomSize);
		SysBuf[9] = '#';
    }
*/
    if (ApplVar.AP.Dept.CapSize)
	{
		ApplVar.Dept.Name[sizeof(ApplVar.Dept.Name)-1]=0;
		CopyFrStr(CCDBuff,ApplVar.Dept.Name);
        CCDBuff[strlen(ApplVar.Dept.Name)]=',';
	}
}


void usbPrintPlu(WORD number)
{
    BYTE i;
    char temp[10];
    BCD price;

    ApplVar.PluNumber = number;
    ReadPlu();
/*
    if (ApplVar.AP.Plu.RandomSize)
    {
		Htoa(SysBuf + 10, ApplVar.Plu.Random, ApplVar.AP.Plu.RandomSize);
		SysBuf[9] = SysBuf[0];          // copy number sign
	    PrintStr(SysBuf);
	    if (ApplVar.AP.Plu.CapSize)
			PrintStr(ApplVar.Plu.Name);
    }
	else
*/
	{
    	if (ApplVar.AP.Plu.CapSize)
   		{
			CopyFrStr(CCDBuff,ApplVar.Plu.Name);
            CCDBuff[strlen(ApplVar.Plu.Name)]=',';
   		}
	}
/*
	if (ApplVar.AP.Plu.InvSize && ApplVar.Report.Type == 3)
	{
		if (TESTBIT(ART_LEVEL, BIT6))
	    	PrintQty(Msg[KCSHLIANG].str, &PluInventory);    // qty
	}

    if (ApplVar.AP.Plu.PriceSize)
    {
		price = ZERO;
		strcpy(temp, Msg[JIAGE].str);
		temp[9] = 0;
		for(i = 0; i < ApplVar.AP.Plu.Level; i++)
		{
		    if (ApplVar.AP.Plu.Level > 1)
			Itoa(temp + 8, i + 1);
		    memcpy(price.Value, ApplVar.Plu.Price[i], ApplVar.AP.Plu.PriceSize);
		    PrintAmt(temp, &price);
		}
    }
*/
}


void usbPrintTender(WORD number)
{
    ApplVar.TendNumber = number;
    ReadTender();
    if (ApplVar.AP.Tend.CapSize)
    {
		ApplVar.Tend.Name[sizeof(ApplVar.Tend.Name)-1] = 0;
		CopyFrStr(CCDBuff,ApplVar.Tend.Name);
        CCDBuff[strlen(ApplVar.Tend.Name)]=',';

	} /* print number+name */
}



void usbPrintCorrec(WORD number)
{
	int i,j;

    ApplVar.CorrecNumber = number;
    ReadCorrec();

	if (ApplVar.AP.Correc.CapSize)
    {
	    j=0;
		if (ApplVar.Correc.Name[0]=='@')
			i=1;
		else
			i=0;
		for (;i<ApplVar.AP.Correc.CapSize && ApplVar.Correc.Name[i];i++)
		{
			if (ApplVar.Correc.Name[i] != '~')
			{
				CCDBuff[j] = ApplVar.Correc.Name[i];
				j++;
			}
		}
        CCDBuff[j]=',';
	}
}

void usbPrintDisc(WORD number)
{
    ApplVar.DiscNumber = number;
    ReadDisc();
    if (ApplVar.AP.Disc.CapSize)
   	{
		ApplVar.Disc.Name[sizeof(ApplVar.Disc.Name)-1] = 0;
		CopyFrStr(CCDBuff,ApplVar.Disc.Name);
        CCDBuff[strlen(ApplVar.Disc.Name)]=',';
    }
}


void usbPrintPb(WORD number)
{
    ApplVar.PbNumber = number;
    PbTotal(number+1,0);

    if (ApplVar.AP.Pb.Text && ApplVar.PB.Text[0])
   	{
		ApplVar.PB.Text[sizeof(ApplVar.PB.Text)-1] = 0;
		strcpy(CCDBuff,ApplVar.PB.Text);
    }
    else
    {
        sprintf(CCDBuff,"%s%d",Msg[ZUOHAO].str,number+1);
    }
    CCDBuff[strlen(CCDBuff)]=',';
}


void usbPrintReport(int RepFor,struct REPSIZE *S)
{
    BYTE temp, periods, periodnumber;

    WORD skip, total;
    UnLong save ;     /* unsigned long save; */
	UnLong TempL ;

	if (ApplVar.Report.Type != REPTYPEMAX)          /* ApplVar.Plu inventory ? */ //lyq
	{
	    skip = 0;
		if (ApplVar.Report.PointerType)
	    {
		    skip += PeriodSkip(S, 1);
			S++;
	    }
		if (ApplVar.Report.PointerType > 1)
	    {
		    skip += PeriodSkip(S, ApplVar.AP.Clerk.Number);
			S++;
	    }
		if (ApplVar.Report.PointerType > 2)
	    {
		    skip += PeriodSkip(S, ApplVar.AP.Zone.Number);
			S++;
		}
	    if (ApplVar.Report.PointerType > 3)
		{
			skip += PeriodSkip(S, ApplVar.AP.Day.Number);
		    S++;
	    }
		if (ApplVar.Report.PointerType > 4)
	    {
		    skip += PeriodSkip(S, ApplVar.AP.SalPer.Number);
			S++;
	    }
		memcpy(&ApplVar.Size, S, sizeof(ApplVar.Size));    /* copy CONSTruction to work */
	    if (!ApplVar.Size.Length)     /* not active report */
	    	return;
	    if (!(ApplVar.Size.Periods & (0x01 << ApplVar.Report.Period))) /* not active */
	    	return;
	    temp = 0x01;
		periods = 0;
	    periodnumber = 0;
		do
	    {
		    if (temp & (0x01 << ApplVar.Report.Period))
			    periodnumber = periods;
		    if (ApplVar.Size.Periods & temp)
			    periods++;
		    temp <<= 1;
	    } while (temp < 0x10);
	    RamOffSet = ApplVar.AP.StartAddress[RepFor] + ApplVar.Report.OffSet + skip +
					(WORD)ApplVar.Report.Pointer * ApplVar.Size.Length * periods +
					(WORD)ApplVar.Size.Length * periodnumber +
					(WORD)ApplVar.Report.Start * ApplVar.Report.Size;
	}

	usbPrintTitlel();

    for (total = ApplVar.Report.Start; total < ApplVar.Report.End; total++)
    {
		save = RamOffSet;
		ReadTotal();     /* read correct total */
		if (CheckNotZero(&ApplVar.Total.RetQty))   /* Not Zero then */
		    SETBIT(ApplVar.Total.RetQty.Sign, BIT7); /* Always negative */

		if (TESTBIT(ApplVar.Total.Qty.Sign, BIT2))     /* used ? */
		{
		    ApplVar.Total.Qty.Sign &= 0x83;

		    if(ApplVar.Report.Type!=0)//ApplVar.Report.PointerType != 5 &&  ApplVar.Report.PointerType != 1)
				MemSet(CCDBuff, sizeof(CCDBuff), ' ');//为打印总销售,收款员销售,营业员销售报表

			if (ApplVar.Report.Type != REPTYPEMAX)  /* not inventory ? */ //lyq
			{
			    Add(&ApplVar.SaleQty, &ApplVar.Total.Qty);
				Add(&ApplVar.SaleAmt, &ApplVar.Total.Amt);
			}
		    switch(ApplVar.Report.Type)    {
            case 0:     /* total Sales */
                for (temp = sizeof(CCDBuff)-10;temp>0;temp--)
                {
                    if (CCDBuff[temp] && CCDBuff[temp]!=' ')
                    {
                        CCDBuff[++temp] = ',';//增补CSV格式的分隔符号
                        break;
                    }
                }
			    break;
			case 1:     /* group sales */
			    usbPrintGroup(total);//获取报表数据项目名称
			    break;
			case 2:     /* dept sales*/
			    usbPrintDept(total);
			    break;
			case 3:     /* plu sales*/
			    usbPrintPlu(total);
			    break;
			case 4:     /* tender */
			    usbPrintTender(total);
			    break;
//			case 5:     /* Po & Ra */
//			    PrintPoRa(total);
//			    break;
//			case 6:     /* drawer */
//			    PrintDrawer(total);
//			    break;
			case 7:     /* correction */
			    usbPrintCorrec(total);
			    break;
			case 8:     /* discount*/
			    usbPrintDisc(total);
			    break;
//			case 9:     /* foreign currency */
//			    PrintCurrency(total);
//			    break;
//			case 10: /* tax */
//			    PrintTax(total);
//			    break;
//			case 11: /* pb info */
//			    PrintPbF(total);
//			    break;
			case 13:
				usbPrintPb(total);
				break;
			default:
			    return;
		    }
		    usbPrintTotal();//添加报表数据项

			RamOffSet = save;       /* reset address pointer */

            //ccr20140403 英文版的普通收款机需要保留Grand Total累计量，该计数器在每次Z-report后不清除
            if (ApplVar.Report.Type!=0 || ApplVar.Report.Period!=0 || (TESTBIT(DBLHIPRN,BIT6))) //ccr2016-02-23
                ResetReport();
		}

        RamOffSet = save + ApplVar.Report.Size;
    }
}


void usbPrintPluReport(int RepFor)
{
    ApplVar.Report.End = ApplVar.AP.Plu.Number;
    if (ApplVar.AP.Plu.RandomSize)
		ApplVar.Report.End =ApplVar.AP.Plu.RNumber;     /* calculate bytes to move 2nd part */
    if (!ApplVar.Report.End)
		return;
    ApplVar.Report.OffSet = ApplVar.AP.Plu.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.Plu.RecordSize;
    usbPrintReport(RepFor,ApplVar.AP.Plu.Size);
}


void usbPrintDeptReport()
{
    if (!ApplVar.AP.Dept.Number)
	return;
    ApplVar.Report.End = ApplVar.AP.Dept.Number;
    ApplVar.Report.OffSet = ApplVar.AP.Dept.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.Dept.RecordSize;
    usbPrintReport(AddrDept,ApplVar.AP.Dept.Size);
}

void usbPrintGroupReport()
{
    if (!ApplVar.AP.Group.Number)
	return;
    ApplVar.Report.End = ApplVar.AP.Group.Number;
    ApplVar.Report.OffSet = ApplVar.AP.Group.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.Group.RecordSize;
    usbPrintReport(AddrGroup,ApplVar.AP.Group.Size);
}


void usbPrintTendReport()
{
    if (!ApplVar.AP.Tend.Number)
	return;
    ApplVar.Report.End = ApplVar.AP.Tend.Number;
    ApplVar.Report.OffSet = ApplVar.AP.Tend.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.Tend.RecordSize;
    usbPrintReport(AddrTend,ApplVar.AP.Tend.Size);
}

/*
void usbPrintPoRaReport()
{
    if (!ApplVar.AP.PoRa.Number)
	return;
    ApplVar.Report.End = ApplVar.AP.PoRa.Number;
    ApplVar.Report.OffSet = ApplVar.AP.PoRa.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.PoRa.RecordSize;
    usbPrintReport(AddrPoRa,ApplVar.AP.PoRa.Size);
}


void usbPrintCurrReport()
{
    if (!ApplVar.AP.Curr.Number)
	return;
    ApplVar.Report.End = ApplVar.AP.Curr.Number;
    ApplVar.Report.OffSet = ApplVar.AP.Curr.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.Curr.RecordSize;
    usbPrintReport(AddrCurr,ApplVar.AP.Curr.Size);
}


void usbPrintDrawReport()
{
    if (!ApplVar.AP.Draw.Number)
	return;
    ApplVar.Report.End = ApplVar.AP.Draw.Number;
    ApplVar.Report.OffSet = ApplVar.AP.Draw.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.Draw.RecordSize;
    usbPrintReport(AddrDrawer,ApplVar.AP.Draw.Size);
}
*/

void usbPrintCorrecReport()
{
    if (!ApplVar.AP.Correc.Number)
	return;
    ApplVar.Report.End = ApplVar.AP.Correc.Number;
    ApplVar.Report.OffSet = ApplVar.AP.Correc.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.Correc.RecordSize;
    usbPrintReport(AddrCorr,ApplVar.AP.Correc.Size);
}

void usbPrintDiscReport()
{
    if (!ApplVar.AP.Disc.Number)
	return;
    ApplVar.Report.End = ApplVar.AP.Disc.Number;
    ApplVar.Report.OffSet = ApplVar.AP.Disc.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.Disc.RecordSize;
    usbPrintReport(AddrDisc,ApplVar.AP.Disc.Size);
}

/*
void usbPrintTaxReport()
{
    if (!ApplVar.AP.Tax.Number)
	return;
    ApplVar.Report.End = ApplVar.AP.Tax.Number;
    ApplVar.Report.OffSet = ApplVar.AP.Tax.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.Tax.RecordSize;
    usbPrintReport(AddrTax,ApplVar.AP.Tax.Size);
}


void usbPrintPbFReport()
{
    if (!ApplVar.AP.Pb.Number || !ApplVar.AP.Pb.NumberOfPb)
	return;
    ApplVar.Report.End = ApplVar.AP.Pb.Number;
    ApplVar.Report.OffSet = ApplVar.AP.Pb.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.Pb.RecordSize;
    usbPrintReport(AddrPBf,ApplVar.AP.Pb.Size);
}
*/
void usbPrintPbtReport()
{
    if (!ApplVar.AP.Pb.Number || !ApplVar.AP.Pb.NumberOfPb)
		return;
    ApplVar.Report.End = ApplVar.AP.Pb.NumberOfPb;
    ApplVar.Report.OffSet = ApplVar.AP.Pb.PBTTotalOffset;
    ApplVar.Report.Size = ApplVar.AP.Pb.PBTRecordSize;
    usbPrintReport(AddrPBt,ApplVar.AP.Pb.PBTSize);
}


void usbPrintPointReport()
{
	int i=1;

    ApplVar.SaleAmt = ZERO;
    ApplVar.SaleQty = ZERO;
    switch(ApplVar.Report.Type) {
	case 0://Print TOTALSALES recordes
	    ApplVar.Report.End = 1;
	    ApplVar.Report.Size = ApplVar.AP.Sales.RecordSize;
	    ApplVar.Report.OffSet = 0;
	    usbPrintReport(AddrTotl,ApplVar.AP.Sales.Size);
	    break;
	case 1:
	    usbPrintGroupReport();
	    break;
	case 2:
	    usbPrintDeptReport();
	    break;
	case 3:
	    usbPrintPluReport(AddrPLU);
	    break;
	case 4:
	    usbPrintTendReport();
	    break;
/*
	case 5:
	    PrintPoRaReport();
	    break;
	case 6:
	    PrintDrawReport();
	    break;
*/
	case 7:
	    usbPrintCorrecReport();
	    break;
	case 8:
	    usbPrintDiscReport();
	    break;
/*
	case 9:
	    PrintCurrReport();
	    break;
	case 10:
	    PrintTaxReport();
	    break;
	case 11:
	    PrintPbFReport();
	    break;
	case 12:
	    PrintOpenPb();
	    break;
*/
	case 13:
		usbPrintPbtReport();
		break;
//	case REPTYPEMAX:                         /* open table */
//	    usbPrintPluReport(AddrRPLU);
//	    break;
	default :
	    break;
    }
}

//////////////////////////////////////////////////////////////
//使用CCDBuff保存所生成的一行报表数据
//使用SysBuf和ProgLineMes作为临时数据工作区
void usbGetReport(BYTE repnum)
{
#define TITLELEFT	25  //TITLELEFT之前为日期和时间"2000年01月02日 22:44:23,"

	int i;
    char flag;


    ApplVar.SaveClerk = ApplVar.ClerkNumber;
	ApplVar.SaveSalPer = ApplVar.SalPerNumber;


	//WORDtoASCZero(SysBuf+7, ApplVar.AP.Flag[15]);//收款机号码
	ApplVar.Report.System= 0;	ApplVar.Report.Period= 0;	ApplVar.Report.Start = 0;
	ApplVar.Report.PointerType = 0;	ApplVar.Report.Pointer = 0;
    ApplVar.Report.PointerEnd = 1;

    memset(ProgLineMes,' ',sizeof(ProgLineMes));

	switch (repnum){
	case usbReportType0 :				//"   总  销  售   ",
		ApplVar.Report.Type = 0;
		CopyFrStr(SysBuf,TOTALREP);     //文件名称
		strcpy(ProgLineMes+TITLELEFT,cReportType0);//报表标题
	    break;
	case usbReportType1 :        //" 柜  组  销  售 ",//1
		ApplVar.Report.Type = 1;
		CopyFrStr(SysBuf,GROUPREP);
		strcpy(ProgLineMes+TITLELEFT,cReportType1);
	    break;
	case usbReportType2 :        //" 部  类  销  售 ",//2
		ApplVar.Report.Type = 2;
		CopyFrStr(SysBuf,DEPTREP);
		strcpy(ProgLineMes+TITLELEFT,cReportType2);
	    break;
	case usbReportType3 :        //" 商  品  销  售 ",      /* 3 Used with ApplVar.Plu ApplVar.Report */
		ApplVar.Report.Type = 3;
		CopyFrStr(SysBuf,PLUREP);
		strcpy(ProgLineMes+TITLELEFT,cReportType3);
	    break;
//	case usbReportType4 :        //" 付  款  信  息 ",//4
//		ApplVar.Report.Type = 3;
//		CopyFrStr(SysBuf,TENDREP);
//		strcpy(ProgLineMes,ReportType4);
	    break;
	case usbReportType7 :        //" 更  改  信  息 ",//7
		ApplVar.Report.Type = 7;
		CopyFrStr(SysBuf,CORREPTREP);
		strcpy(ProgLineMes+TITLELEFT,cReportType7);
	    break;
	case usbReportType8 :        //" 折  扣  信  息 ",//8
		ApplVar.Report.Type = 8;
		CopyFrStr(SysBuf,DISCREP);
		strcpy(ProgLineMes+TITLELEFT,cReportType8);
	    break;
//	case usbReportType11:        //" 桌  帐  信  息 ",//11
//	case usbReportType12:        //" 开  台  信  息 ",//12
	case usbReportType13:        //" 桌  台  信  息 ",//13
		ApplVar.Report.Type = 13;
		CopyFrStr(SysBuf,PBTOTALREP);
		strcpy(ProgLineMes+TITLELEFT,cReportType13);
		break;
	case usbReportType17:       	//"    收 款 员 统 计   ",
		ApplVar.Report.Type = 0;
		ApplVar.Report.PointerType = 1;
        ApplVar.Report.PointerEnd = ApplVar.AP.Clerk.Number;

		CopyFrStr(SysBuf,CLERKREP);
		strcpy(ProgLineMes+TITLELEFT,cReportType17);
		break;
	case usbReportType18:  				//"    营 业 员 统 计   ",
		ApplVar.Report.Type = 0;
		ApplVar.Report.PointerType = 5;
        ApplVar.Report.PointerEnd = ApplVar.AP.SalPer.Number;
		CopyFrStr(SysBuf,SALPERREP);
		strcpy(ProgLineMes+TITLELEFT,cReportType18);
		break;
	case usbReportType19:       	//"  收款员 折扣 统 计   ",
		ApplVar.Report.Type = 8;
		ApplVar.Report.PointerType = 1;
        ApplVar.Report.PointerEnd = ApplVar.AP.Clerk.Number;

		CopyFrStr(SysBuf,CLERKREP);
		CopyFrStr(ProgLineMes+TITLELEFT,cSetupCaption38);
		strcpy(ProgLineMes+8+TITLELEFT,cReportType8);
		break;
	case usbReportType20:  				//"  营业员 折扣 统 计   ",
		ApplVar.Report.Type = 8;
		ApplVar.Report.PointerType = 5;
        ApplVar.Report.PointerEnd = ApplVar.AP.SalPer.Number;

		CopyFrStr(SysBuf,SALPERREP);
		CopyFrStr(ProgLineMes+TITLELEFT,cSetupCaption39);
		strcpy(ProgLineMes+8+TITLELEFT,cReportType8);
		break;
	case usbReportType21:       	//"  收款员 桌台 统 计   ",
		ApplVar.Report.Type = 13;
		ApplVar.Report.PointerType = 1;
        ApplVar.Report.PointerEnd = ApplVar.AP.Clerk.Number;

		CopyFrStr(SysBuf,CLERKREP);
		CopyFrStr(ProgLineMes+TITLELEFT,cSetupCaption38);
		strcpy(ProgLineMes+8+TITLELEFT,cReportType13);
		break;
	case usbReportType22:  				//"  营业员 桌台 统 计   ",
		ApplVar.Report.Type = 13;
		ApplVar.Report.PointerType = 5;
        ApplVar.Report.PointerEnd = ApplVar.AP.SalPer.Number;

		CopyFrStr(SysBuf,SALPERREP);
		CopyFrStr(ProgLineMes+TITLELEFT,cSetupCaption39);
		strcpy(ProgLineMes+8+TITLELEFT,cReportType13);
		break;
	default:
		return;

	}

//    if (!SetUpReport())
    {
		for(ApplVar.Report.Pointer = 0;ApplVar.Report.Pointer < ApplVar.Report.PointerEnd; ApplVar.Report.Pointer++)
		{
		    if (ApplVar.Report.PointerType == 1)
		    {//report for clerk(收款员报表)
				ApplVar.ClerkNumber = ApplVar.Report.Pointer + 1;
				ReadClerk();
				if (!TESTBIT(ApplVar.Clerk.Options, BIT7) &&
					TESTBIT(ApplVar.AP.ReportList[repnum].Options, BIT1) &&
					!TESTBIT(CLERKFIX, BIT6))
				       /* all pointers then skip training clerk ? */
				    continue;
		    }
			 memset(CCDBuff,' ',sizeof(CCDBuff));
		    if((ApplVar.Report.PointerType == 5 || ApplVar.Report.PointerType == 1))
	     	{
//			 	if (ApplVar.Report.Type!=8 && ApplVar.Report.Type!=13 && CheckTotal() == 0)
			 	if (ApplVar.Report.Type==0 && CheckTotal() == 0)
			     	continue;

			    if(ApplVar.Report.PointerType == 1)
		    	{
		    		if (ApplVar.AP.Clerk.Number)
		    		{
			            ApplVar.ClerkNumber = ApplVar.Report.Pointer + 1;
			            ReadClerk();
		    	        CopyFrStr(CCDBuff,ApplVar.Clerk.Name);
		    		}
			    }
				else
				{
#if (salNumber)
                    if (ApplVar.AP.SalPer.Number)
                    {
                        ApplVar.SalPerNumber = ApplVar.Report.Pointer + 1;
                        ReadSalPer();
                        CopyFrStr(CCDBuff,ApplVar.SalPer.Name);
                    }
                    else
#endif
                    {
                        CCDBuff[0] = '-';CCDBuff[1] = '-';
                    }
				}
	    	}
            else if (ApplVar.Report.Type == 0)//总销售
            {
                CopyFrStr(CCDBuff,ApplVar.TXT.ReportType[0]);
            }
			usbPrintPointReport();
		    if (ApplVar.ErrorNumber)
				 break;
		}
    }

    ApplVar.ClerkNumber = ApplVar.SaveClerk;
	ReadClerk();
#if (salNumber)
	ApplVar.SalPerNumber = ApplVar.SaveSalPer;
	ReadSalPer();
#endif

}

//////////////////////////////////////////////////////////////////////////////////
//使用EntryBuffer保存日期,收款机号和店铺号,用来追加到每个个报表数据之后.
//因此,必须确保在其它地方没有使用EntryBuffer
//////////////////////////////////////////////////////////////////////////////////
void ViewReport(BYTE repType)
{

	usbGetReport(usbReportType0 );		//"   总  销  售   ",
	usbGetReport(usbReportType1 );        //" 柜  组  销  售 ",//1
	usbGetReport(usbReportType2 );        //" 部  类  销  售 ",//2
	usbGetReport(usbReportType3 );        //" 商  品  销  售 ",
	usbGetReport(usbReportType7 );        //" 更  改  信  息 ",//7
	usbGetReport(usbReportType17);       	//"    收 款 员 统 计   ",
	usbGetReport(usbReportType18);  		//"    营 业 员 统 计   ",
	usbGetReport(usbReportType8 );        //" 折  扣  信  息 ",//8
	usbGetReport(usbReportType19);       	//"    收 款 员 折扣 统 计   ",
	usbGetReport(usbReportType20);  		//"    营 业 员 折扣 统 计   ",
	usbGetReport(usbReportType13);        //" 桌  台  信  息 ",//13
	usbGetReport(usbReportType21);       	//"    收 款 员 桌台 统 计   ",
	usbGetReport(usbReportType22);  		//"    营 业 员 桌台 统 计   ",
}



