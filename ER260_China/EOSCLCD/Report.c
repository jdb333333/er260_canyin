#define REPORT_C 5
#include "king.h"
#include "exthead.h"
#include "exth.h"


/****************************************************************/
void PrintBARCustomer(BYTE xz);
void GetReport(BYTE xzNum);


void PrintReportType()
{
#if (HOR_REPORT)//ccr100702>>>>>>>>>
    if(ApplVar.Report.Type!=3)    //plu report
#endif
	{
    	MemSet(SysBuf, sizeof(SysBuf), ' ');
	    memcpy(SysBuf + 4, ApplVar.TXT.ReportType[ApplVar.Report.Type], sizeof(ApplVar.TXT.ReportType[0]));
    	PrintStr(SysBuf);
	}
#if (HOR_REPORT)//ccr100702>>>>>>>>>
    if(ApplVar.Report.Type==2)   //dept report
    {
        RJPrint(0,Msg[HDEPTREPORT].str) ;
    }
    else if(ApplVar.Report.Type==3)    //plu report
    {
        RJPrint(0,Msg[HPLUREPORT].str) ;
    }
#endif //<<<<<<<<<<
    PrintLine('.');
}

void PrintGroup(WORD number)
{
    ApplVar.GroupNumber = number;
    ReadGroup();

    if (ApplVar.RepComputer)
    {
		*((WORD *)SysBuf) = GROUP + 1;
		memcpy(SysBuf+7, ApplVar.Group.Name, ApplVar.AP.Group.CapSize);
		return;
    }
    if (SysBuf[0]&&ApplVar.AP.Group.CapSize)
	{
		memcpy(SysBuf+9,ApplVar.Group.Name,ApplVar.AP.Group.CapSize);
		PrintStr(SysBuf); /* print number */
	}
	else if (SysBuf[0])
		PrintStr(SysBuf); /* print number */
    else if (ApplVar.AP.Group.CapSize)
		PrintStr(ApplVar.Group.Name);
}

void PrintDept(WORD number)
{
    short i = 0;
    ApplVar.DeptNumber = number;
    ReadDept();
    if (ApplVar.RepComputer)
    {
		*((WORD *)SysBuf) = DEPT + ApplVar.DeptNumber + 1;
		if (ApplVar.AP.Dept.RandomSize)
		{
		    memcpy(SysBuf+7, ApplVar.Dept.Random, ApplVar.AP.Dept.RandomSize);
		    SysBuf[2] = 0;  /* type = random number */
		    SendRecord(SysBuf, 7 + 7);     /* always max size 14 */
		}
		*((WORD *)SysBuf) = DEPT + 1;
		memcpy(SysBuf+7, ApplVar.Dept.Name, ApplVar.AP.Dept.CapSize);
		return;
    }
#if (HOR_REPORT==1)//ccr100702 >>>>>>
    memset(SysBuf,' ',sizeof(SysBuf)) ;
	while(ApplVar.Dept.Name[i]!='\0'&&i<ApplVar.AP.Dept.CapSize)
    {
         SysBuf[i] = ApplVar.Dept.Name[i];
         i++;
    }
#else
    if (ApplVar.AP.Dept.RandomSize)
    {
		if (!SysBuf[0])
		    SysBuf[0] = ' ';
		HEXtoASC(SysBuf + 10, ApplVar.Dept.Random, ApplVar.AP.Dept.RandomSize);
		SysBuf[9] = '#';
    }
    if (SysBuf[0]&&ApplVar.AP.Dept.CapSize)
	{
		while(ApplVar.Dept.Name[i]!='\0'&&i<ApplVar.AP.Dept.CapSize)
        {
             SysBuf[i+7] = ApplVar.Dept.Name[i];
             i++;
        }

#if defined(CASE_FORHANZI)
        if( i > 16 )
#else
        if( i > 10 )
#endif
        {
             PrintStr(SysBuf);
             memset(SysBuf,' ',sizeof(SysBuf));
        }
		PrintStr(SysBuf);
	}
	else if (SysBuf[0])
		PrintStr(SysBuf);
    else
      if (ApplVar.AP.Dept.CapSize)
		PrintStr(ApplVar.Dept.Name);
#endif
//<<<<<<<<<<<<<<<<<<<<
}


void PrintPlu(WORD number)
{
    BYTE i=0;
    char temp[10];
    BCD price;

    ApplVar.PluNumber = number;
    ReadPlu();
    if (ApplVar.RepComputer)
    {
		*((WORD *)SysBuf) = PLU1 + ApplVar.PluNumber + 1;
		if (ApplVar.AP.Plu.RandomSize)
		{
		    memcpy(SysBuf+7, ApplVar.Plu.Random, ApplVar.AP.Plu.RandomSize);
		    SysBuf[2] = 0;  /* type = random number */
		    SendRecord(SysBuf, 7 + 7);     /* always max size 14 */
		}
		*((WORD *)SysBuf) = PLU1 + 1;
		memcpy(SysBuf+7, ApplVar.Plu.Name, ApplVar.AP.Plu.CapSize);
		return;
    }


#if (HOR_REPORT==1)     //ccr100702 >>>>>>>canceled by zengy for new report
    memset(SysBuf,' ',sizeof(SysBuf)) ;
	while(ApplVar.Plu.Name[i]!='\0'&&i<ApplVar.AP.Plu.CapSize)
    {
         SysBuf[i] = ApplVar.Plu.Name[i];
         i++;
    }
#else
    if (ApplVar.AP.Plu.RandomSize)
    {//ccr2014-09-03>>>>>>>>
        temp[0]= SysBuf[0];
        memset(SysBuf,' ',16);
		HEXtoASC(SysBuf, ApplVar.Plu.Random, ApplVar.AP.Plu.RandomSize);
		SysBuf[0] = temp[0];          /* copy number sign */

	    if (ApplVar.AP.Plu.CapSize)
			strcpy(SysBuf+16,ApplVar.Plu.Name);
        PrintStr(SysBuf);
        //<<<<<<<<<<<<<<<<<<<
        memset(SysBuf,' ',sizeof(SysBuf)) ;
        i = 0;
        while(i<ApplVar.AP.Plu.CapSize)
        {
   #if defined(CASE_FORHANZI)
            if(ApplVar.Plu.Name[i]=='\0'||ApplVar.Plu.Name[i]==' ')
   #else
            if(ApplVar.Plu.Name[i]=='\0')
   #endif
                break;
            SysBuf[i] = ApplVar.Plu.Name[i];
            i++;
        }
        if( i > 16 )
        {
            PrintStr(SysBuf);
            memset(SysBuf,' ',sizeof(SysBuf)) ;
        }
    }
	else
	{
    	if (ApplVar.AP.Plu.CapSize)
			strncpy(SysBuf+8,ApplVar.Plu.Name,ApplVar.AP.Plu.CapSize);
	    PrintStr(SysBuf);
	}
    if (ApplVar.AP.Plu.InvSize && ApplVar.Report.Type == REPORT_PLU)
	{
		if (TESTBIT(ART_LEVEL, BIT6))
	     	PrintQty(Msg[KCSHLIANG].str, &ApplVar.PluInventory);    /* qty */
	}
	if (ApplVar.Report.Type == REPORT_INVENTORY) // lyq
	{
	    if (ApplVar.AP.Plu.InvSize)     /* print inventory */
		{
		    PrintQty(Msg[KCSHLIANG].str, &ApplVar.PluInventory);    /* qty */
		    Add(&ApplVar.SaleQty, &ApplVar.PluInventory);
		    if (!TESTBIT(ART_LEVEL, BIT2) && ApplVar.AP.Plu.Cost) /* ApplVar.Cost price active ? */
		    {
				price = ZERO;
				memcpy(price.Value, ApplVar.Plu.Cost, ApplVar.AP.Plu.PriceSize);
				Multiply(&price, &ApplVar.PluInventory);
			    PrintAmt(Msg[JINE].str, &price);   /* qty */
				Add(&ApplVar.SaleAmt, &price);
			}
		}
		return;
	}

    if (ApplVar.AP.Plu.PriceSize)
    {
		price = ZERO;
		strncpy(temp, Msg[JIAGE].str,sizeof(temp));
		temp[9] = 0;
		for(i = 0; i < ApplVar.AP.Plu.Level; i++)
		{
		    if (ApplVar.AP.Plu.Level > 1)
			WORDtoASC(temp + 8, i + 1);
		    memcpy(price.Value, ApplVar.Plu.Price[i], ApplVar.AP.Plu.PriceSize);
		    PrintAmt(temp, &price);
		}
    }
#endif//<<<<<<<<<<<<<<<<<<<<<<
}


void PrintTender(WORD number)
{
    ApplVar.TendNumber = number;
    ReadTender();
    if (ApplVar.RepComputer)
    {
		*((WORD *)SysBuf) = TEND + 1;
		memcpy(SysBuf+7, ApplVar.Tend.Name, ApplVar.AP.Tend.CapSize);
		return;
    }
    if (SysBuf[0]&&ApplVar.AP.Tend.CapSize)
    {
			memcpy(SysBuf+9,ApplVar.Tend.Name,ApplVar.AP.Tend.CapSize);
		PrintStr(SysBuf);
	} /* print number+name */
	else if (SysBuf[0])
		PrintStr(SysBuf); /* print number+name */
	else if (ApplVar.AP.Tend.CapSize)
		PrintStr(ApplVar.Tend.Name);
}


void PrintPoRa(WORD number)
{
    ApplVar.PoRaNumber = number;
    ReadPoRa();
    if (ApplVar.RepComputer)
    {
		*((WORD *)SysBuf) = PORA + 1;
		memcpy(SysBuf+7, ApplVar.PoRa.Name, ApplVar.AP.PoRa.CapSize);
		return;
    }
	if(SysBuf[0]&&ApplVar.AP.PoRa.CapSize)
	{
		memcpy(SysBuf+9, ApplVar.PoRa.Name, ApplVar.AP.PoRa.CapSize);
		PrintStr(SysBuf); /* print number + name */
	}
    else if (SysBuf[0])
		PrintStr(SysBuf); /* print number */
    else if (ApplVar.AP.PoRa.CapSize)
		PrintStr(ApplVar.PoRa.Name);
}


void PrintCurrency(WORD number)
{
    ApplVar.CurrNumber = number;
    ReadCurr();
    if (ApplVar.RepComputer)
    {
		*((WORD *)SysBuf) = CURR + 1;
		memcpy(SysBuf+7, ApplVar.Curr.Name, ApplVar.AP.Curr.CapSize);
		return;
    }
	if(SysBuf[0]&&ApplVar.AP.Curr.CapSize)
	{
		memcpy(SysBuf+9, ApplVar.Curr.Name, ApplVar.AP.Curr.CapSize);
		PrintStr(SysBuf); /* print number + name */
	}
    else if (SysBuf[0])
		PrintStr(SysBuf); /* print number */
    else if (ApplVar.AP.Curr.CapSize)
		PrintStr(ApplVar.Curr.Name);
}


void PrintDrawer(WORD number)
{
    ApplVar.DrawNumber = number;
    ReadDrawer();
    if (ApplVar.RepComputer)
    {
		*((WORD *)SysBuf) = DRAW + 1;
		memcpy(SysBuf+7, ApplVar.Draw.Name, ApplVar.AP.Draw.CapSize);
		return;
    }
	if(SysBuf[0]&&ApplVar.AP.Draw.CapSize)
	{
		memcpy(SysBuf+9, ApplVar.Draw.Name, ApplVar.AP.Draw.CapSize);
		PrintStr(SysBuf); /* print number + name */
	}
    else if (SysBuf[0])
		PrintStr(SysBuf); /* print number */
    else if (ApplVar.AP.Draw.CapSize)
		PrintStr(ApplVar.Draw.Name);
}


void PrintCorrec(WORD number)
{
	short i,j;

    ApplVar.CorrecNumber = number;
    ReadCorrec();

    if (ApplVar.RepComputer)
    {
		*((WORD *)SysBuf) = CORREC + 1;
    	j = 0;
		for (i=0;i<ApplVar.AP.Correc.CapSize;i++)
		{
			if (ApplVar.Correc.Name[i] != '~')
			{
				SysBuf[7+j] = ApplVar.Correc.Name[i];
				j++;
			}
//			memcpy(SysBuf+7, ApplVar.Correc.Name, ApplVar.AP.Correc.CapSize);
		}
		return;
    }
    if (SysBuf[0])
	{
    	if (ApplVar.AP.Correc.CapSize)
    	{
    		j = 9;
			for (i=0;i<ApplVar.AP.Correc.CapSize;i++)
			{
				if (ApplVar.Correc.Name[i] != '~')
				{
					SysBuf[j] = ApplVar.Correc.Name[i];
					j++;
				}
			}
		}
		PrintStr(SysBuf); /* print number */
	}
    else if (ApplVar.AP.Correc.CapSize)
    {
    	j = 0;
		for (i=0;i<ApplVar.AP.Correc.CapSize;i++)
		{
			if (ApplVar.Correc.Name[i] != '~')
			{
				SysBuf[j] = ApplVar.Correc.Name[i];
				j++;
			}
		}
		SysBuf[j] = 0;
		PrintStr(SysBuf);
	}
}

void PrintDisc(WORD number)
{
    ApplVar.DiscNumber = number;
    ReadDisc();

    if (ApplVar.RepComputer)
    {
		*((WORD *)SysBuf) = DISC + 1;
		memcpy(SysBuf+7, ApplVar.Disc.Name, ApplVar.AP.Disc.CapSize);
		return;
    }

    if (SysBuf[0])
	{
		if(ApplVar.AP.Disc.CapSize)
			memcpy(SysBuf+7, ApplVar.Disc.Name, ApplVar.AP.Disc.CapSize);
		PrintStr(SysBuf); /* print number */
	}
    else if (ApplVar.AP.Disc.CapSize)
		PrintStr(ApplVar.Disc.Name);

}


void PrintTax(WORD number)
{

    ApplVar.TaxNumber = number;
    ReadTax();
    if (ApplVar.RepComputer)
    {
		*((WORD *)SysBuf) = TAX + 1;
		memcpy(SysBuf+7, ApplVar.Tax.Name, ApplVar.AP.Tax.CapSize);
    }
    else
    {
		if (SysBuf[0])
		{
			if (ApplVar.AP.Tax.CapSize)
				memcpy(SysBuf+9, ApplVar.Tax.Name, ApplVar.AP.Tax.CapSize);
		    PrintStr(SysBuf); /* print number */
		}
		else if (ApplVar.AP.Tax.CapSize)
		    PrintStr(ApplVar.Tax.Name);
    }

    if (TESTBIT(ApplVar.Tax.Options, BIT0)) /* vat then calculate vat  ? */
    {
		if (ApplVar.Tax.Rate[0] == 0x99 &&
				ApplVar.Tax.Rate[1] == 0x99 &&
					ApplVar.Tax.Rate[2] == 0x99)
			ApplVar.Total.Disc = ZERO;              /* NET is zero */
		else
		{
			memcpy(ApplVar.Total.Qty.Value, ApplVar.Tax.Rate, sizeof(ApplVar.Tax.Rate));
		    ApplVar.Total.Cost = ApplVar.Total.Amt;
			ApplVar.Total.Disc = ApplVar.Total.Amt;
			Add(&ApplVar.Total.Qty, (BCD*)&THOUSAND10);
		    ApplVar.Total.Qty.Sign = 0x04;
			Divide(&ApplVar.Total.Disc, &ApplVar.Total.Qty);

			RoundBcd(&ApplVar.Total.Disc, 0);

			AmtRound(1, &ApplVar.Total.Disc);
		    Subtract(&ApplVar.Total.Cost, &ApplVar.Total.Disc);  /* taxable - net */
		}
    }
    else    /* add On */
    {
		ApplVar.Total.Disc = ApplVar.Total.Amt;     /* ApplVar.Disc -> total NET */
		Add(&ApplVar.Total.Amt, &ApplVar.Total.Cost);   /* ApplVar.Amt -> Net + ApplVar.Tax */
    }

}


void PrintPbF(WORD number)
{
    ApplVar.PbFNumber = number;
    ReadPbF();

    if (ApplVar.RepComputer)
    {
		*((WORD *)SysBuf) = PBF + 1;
		memcpy(SysBuf+7, ApplVar.PbF.Name, ApplVar.AP.Pb.CapSize);
		return;
    }

    if (SysBuf[0])
	{
	    if (ApplVar.AP.Pb.CapSize)
			memcpy(SysBuf+9, ApplVar.PbF.Name, ApplVar.AP.Pb.CapSize);
		PrintStr(SysBuf); /* print number */
	}
	else if (ApplVar.AP.Pb.CapSize)
		PrintStr(ApplVar.PbF.Name);

}

void PrintPb(WORD number)
{
    ApplVar.PbNumber = number;
    PbTotal(number+1,0);

    if (ApplVar.RepComputer)
    {
		*((WORD *)SysBuf) = PBTOT + 1;
		memcpy(SysBuf+7, ApplVar.PB.Text, ApplVar.AP.Pb.Text);
		return;
    }

    if (SysBuf[0])
	{
	    if (ApplVar.AP.Pb.Text)
			memcpy(SysBuf+9, ApplVar.PB.Text, ApplVar.AP.Pb.Text);
		PrintStr(SysBuf); /* print number */
	}
    else if (ApplVar.AP.Pb.CapSize)
		PrintStr(ApplVar.PB.Text);

}

WORD PeriodSkip(struct REPSIZE *S,BYTE number)
{
    BYTE period;
    WORD skip;

    skip = 0;
    period = 0x08;
    do
    {
		if (S->Periods & period)
	    	skip += S->Length * (WORD)number;
		period >>= 1;
    }while(period);
    return skip;
}

/* set ApplVar.ErrorNumber to CWXXI27 if user break */

short CheckBreak()
{
//    if (ApplVar.RepComputer && COMPUTER)
//		ApplVar.ErrorNumber=ERROR_ID(CWXXI27);   /* rs232 error */
//	else
		CheckBreakKey();
    return (ApplVar.ErrorNumber);
}

void PrintRange()
{
	MemSet(SysBuf, sizeof(SysBuf), ' ');
	WORDtoASCZero(SysBuf + 11, ApplVar.Report.Start);
	SysBuf[12] = '-';
	WORDtoASCZero(SysBuf + 17, ApplVar.Report.End);
	SysBuf[18] = 0;

	PrintStr(SysBuf);

}


void ResetReport()
{
	if (ApplVar.FReport == Z && !ApplVar.ErrorNumber && ApplVar.Report.Type != REPORTTYPE_MAX)      /* Z report ? */
    {
		ApplVar.BufCC = 1;  /* set write only sign byte */
		ApplVar.Total.Qty.Sign = 0;
		WriteTotal();
		ApplVar.BufCC = 0;
	}
}



short CheckTotal()
{
    BYTE temp, periods, periodnumber;
    WORD skip;


	struct REPSIZE *Si ;

	ApplVar.Report.Size = ApplVar.AP.Sales.RecordSize;
	ApplVar.Report.OffSet = 0;
    skip = 0;
	Si = ApplVar.AP.Sales.Size;
	if (ApplVar.Report.PointerType)
    {
	    skip += PeriodSkip(Si, 1);
		Si++;
    }
	if (ApplVar.Report.PointerType > 1)
    {
	    skip += PeriodSkip(Si, ApplVar.AP.Clerk.Number);
		Si++;
    }
	if (ApplVar.Report.PointerType > 2)
    {
	    skip += PeriodSkip(Si, ApplVar.AP.Zone.Number);
		Si++;
	}
    if (ApplVar.Report.PointerType > 3)
	{
		skip += PeriodSkip(Si, ApplVar.AP.Day.Number);
	    Si++;
    }
	if (ApplVar.Report.PointerType > 4)
    {
	    skip += PeriodSkip(Si, ApplVar.AP.SalPer.Number);
		Si++;
    }
	memcpy(&ApplVar.Size, Si, sizeof(ApplVar.Size));    /* copy construction to work */
    if (!ApplVar.Size.Length)     /* not active report */
    	return 0;
	if (!(ApplVar.Size.Periods & (0x01 << ApplVar.Report.Period))) /* not active */
	   	return 0;
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
    RamOffSet = ApplVar.AP.StartAddress[AddrTotl] + ApplVar.Report.OffSet + skip +
				(WORD)ApplVar.Report.Pointer * ApplVar.Size.Length * periods +
				(WORD)ApplVar.Size.Length * periodnumber +
				(WORD)ApplVar.Report.Start * ApplVar.Report.Size;
	ReadTotal();
	if(CheckNotZero(&ApplVar.Total.Amt))
		return 1;
	return 0;
}


void PrintReport(short RepFor,struct REPSIZE *S)
{
    BYTE temp, periods, periodnumber;

    WORD skip, total;
    UnLong save ;     /* unsigned long save; */
	UnLong TempL ;

	if (ApplVar.Report.Type != REPORT_INVENTORY)          /* ApplVar.Plu inventory ? */ //lyq
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
		memcpy(&ApplVar.Size, S, sizeof(ApplVar.Size));    /* copy construction to work */
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

    temp = 0;

	if (ApplVar.MultiplyCount && !ApplVar.RepComputer)              /* range entered ? */
	{
        if ((ApplVar.Report.Type == REPORT_PLU || ApplVar.Report.Type == REPORTTYPE_MAX) && ApplVar.AP.Plu.RandomSize)      /* PLU ? */
		{
			save = RamOffSet;               /* save report start */
			RamOffSet = ApplVar.AP.StartAddress[AddrPLU];    /* start plu table */
		    ApplVar.Report.Start = GetPluNumber(0, ApplVar.Qty1.Value);
			if (ApplVar.MultiplyCount == 2)
			{
				RamOffSet = ApplVar.AP.StartAddress[AddrPLU];    /* start plu table */
				total = GetPluNumber(0, ApplVar.Qty2.Value);
			}
			else
				total = ApplVar.Report.Start;
			RamOffSet = save;
		}
		else
		{
			ApplVar.Report.Start = (unsigned short) BCDValueToULong(ApplVar.Qty1.Value, &TempL);
			if (ApplVar.MultiplyCount == 2)
				total = (unsigned short) BCDValueToULong(ApplVar.Qty2.Value, &TempL);      /* end address */
			else
				total = ApplVar.Report.Start;
		}
		if (total <= ApplVar.Report.End)
			ApplVar.Report.End = total;
		if (!ApplVar.Report.Start || ApplVar.Report.End < ApplVar.Report.Start)
			return;
		PrintRange();   /* print Range Line */
		ApplVar.Report.Start--;
        RamOffSet += (WORD)ApplVar.Report.Size * ApplVar.Report.Start;
	}

    for (total = ApplVar.Report.Start; total < ApplVar.Report.End; total++)
    {
		save = RamOffSet;
		if (ApplVar.RepComputer == 'C' && ApplVar.FReport == Z)
		{
            //ccr20140403 英文版的普通收款机需要保留Grand Total累计量，该计数器在每次Z-report后不清除
            if (ApplVar.Report.Type!=0 || ApplVar.Report.Period!=0 || TESTBIT(DBLHIPRN,BIT6)) //ccr2016-02-23
                ResetReport();
		 	RamOffSet = save + ApplVar.Report.Size ;
			continue;
		}
		if (ApplVar.Report.Type == REPORT_INVENTORY)   //lyq       /* inventory then always all plu's */
		{
			if (!ApplVar.AP.Plu.InvSize)            /* not active */
				break;
	    	SETBIT(ApplVar.Total.Qty.Sign, BIT2);
		}
		else
		{
#if (PLUCODE_X == 1)//ccr2014-09-05>>>>>>>>>>>>>>>
            skip = 0;
            if (ApplVar.Report.Type == 3 && ApplVar.RepComputer != 'C' && PLU_X_Report)// && ApplVar.FReport==X)//指定了单品的条码
            {
                skip = 1;// 预设不打印此单品的报表 //
                ApplVar.PluNumber = total;
                ReadPlu();
                if (PLU_X_Report==PLU1)
                {// 指定单品 //
                    if (ApplVar.AP.Plu.RandomSize)
                    {
                        if (!CompareBCDValue(ApplVar.Entry.Value,ApplVar.Plu.Random,ApplVar.AP.Plu.RandomSize))
                            skip = 0;
                    }
                    else
                    {
                        if (ApplVar.NumberEntry-1 == ApplVar.PluNumber)
                            skip = 0;
                    }
                }
                else if (PLU_X_Report==DEPT)
                {// 指定部类号 //
                    if (ApplVar.Plu.Dept==ApplVar.NumberEntry-1)
                        skip = 0;
                }
                else if (PLU_X_Report>DEPT && PLU_X_Report<PLU1)
                {// 指定部类键 //
                    if (ApplVar.Plu.Dept==PLU_X_Report-DEPT)
                        skip = 0;
                }
            }
            if (skip==0)
#endif//<<<<<<<<<<<
            {
                ReadTotal();     /* read correct total */
                if (CheckNotZero(&ApplVar.Total.RetQty))   /* Not Zero then */
                SETBIT(ApplVar.Total.RetQty.Sign, BIT7); /* Always negative */
            }
#if (PLUCODE_X == 1)//ccr2014-09-05>>>>>>>>>>>>>>>
            else
                memset(&ApplVar.Total,0,sizeof(ApplVar.Total));
#endif
		}


		if (TESTBIT(ApplVar.Total.Qty.Sign, BIT2))     /* used ? */
		{
		    ApplVar.Total.Qty.Sign &= 0x83;
		    if (!temp)
		    {
				if (ApplVar.Report.System == 1)
				    PrintPointType();
				else if (!ApplVar.RepComputer)
					PrintReportType();
				temp = 1;
		    }

		    if (!ApplVar.RepComputer)
		    {
				MemSet(SysBuf, sizeof(SysBuf), ' ');
                if (ApplVar.Report.Type != REPORT_PLU && TESTBIT(KEYTONE, BIT2))  /* Not print Number */
								    /* with PLU always */
				    SysBuf[0] = 0;
				else
				{
				    WORDtoASCZero(SysBuf + 5, total + 1);
				    if ((CHARSET & 0xf0) == 0x10 || (SLIP != 4 &&
						((CHARSET & 0x0f) == 3 || (CHARSET & 0x0f) == 7)
						))  /* number sign not available */
						SysBuf[0] = 'N';
				    else
						SysBuf[0] = '#';
				}
		    }
		    else
				MemSet(SysBuf + 7, 24, 0);

			if (ApplVar.Report.Type != REPORT_INVENTORY)  /* not inventory ? */ //lyq
			{
			    Add(&ApplVar.SaleQty, &ApplVar.Total.Qty);
				Add(&ApplVar.SaleAmt, &ApplVar.Total.Amt);
			}
		    switch(ApplVar.Report.Type)    {
            case REPORT_GRANDTOTAL:     /* total Sales */
			    if (ApplVar.RepComputer)
			    {
					*((WORD *)SysBuf) = RTOTAL;
					memcpy(SysBuf + 7, ApplVar.TXT.ReportType[0], sizeof(ApplVar.TXT.ReportType[0]));
			    }
			    break;
            case REPORT_GROUP:     /* group sales */
			    PrintGroup(total);
			    break;
            case REPORT_DEPARTMENT:     /* dept sales*/
			    PrintDept(total);
			    break;
            case REPORT_PLU:     /* plu sales*/
			case REPORTTYPE_MAX:        /* Inventory */		 //lyq modefied 2003/11/1
			    PrintPlu(total);
			    break;
            case REPORT_TENDER:     /* tender */
			    PrintTender(total);
			    break;
            case REPORT_PORA:     /* Po & Ra */
                PrintPoRa(total);
                break;
            case REPORT_DRAWER:     /* drawer */
                PrintDrawer(total);
                break;
            case REPORT_CORRECTION:     /* correction */
                PrintCorrec(total);
                break;
            case REPORT_DISCOUNT:     /* discount*/
                PrintDisc(total);
                break;
            case REPORT_CURRENCY:     /* foreign currency */
                PrintCurrency(total);
                break;
            case REPORT_TAX: /* tax */
                PrintTax(total);
                break;
            case REPORT_PBFUNC: /* pb info */
                PrintPbF(total);
                break;
            case REPORT_TABLES:
				PrintPb(total);
				break;
			default:
			    return;
		    }
		    if (ApplVar.RepComputer)
		    {
				*((WORD *)SysBuf) += total;     /* add current total number */
				SysBuf[2] = 0x80 + ApplVar.Report.Type + 1;  /* type = descriptor */
				SendRecord(SysBuf, 7 + 24); /* always send max size */
				if (ApplVar.Report.Type == REPORT_INVENTORY && ApplVar.AP.Plu.InvSize)   //lyq
				    SendComp(11, &ApplVar.PluInventory);
		    }
 		    if (ApplVar.Report.Type != REPORT_INVENTORY)  //lyq            /* inventory  ? */
			    PrintTotal();

			if (ApplVar.Report.System == 4 &&
                (ApplVar.Report.Type == REPORT_GROUP || ApplVar.Report.Type == REPORT_DEPARTMENT))
			{
				Divide(&ApplVar.Total.Amt, &ApplVar.SubTotal);
				RoundBcd(&ApplVar.Total.Amt, 4);
				ApplVar.Total.Amt.Sign &= 0x80;
				SETDECIMAL(ApplVar.Total.Amt.Sign, 0x02);
				MemSet(SysBuf, 24, ' ');
				FormatQty(SysBuf+19, &ApplVar.Total.Amt);

				SysBuf[4] = '_';
				SysBuf[5] = SysBuf[13];
				SysBuf[6] = '_';
				SysBuf[7] = SysBuf[14];
				SysBuf[8] = '_';
				SysBuf[9] = SysBuf[15];
				SysBuf[10] = '_';
				SysBuf[11] = SysBuf[16];
				SysBuf[12] = '_';
				SysBuf[13] = SysBuf[17];
				SysBuf[14] = '_';
				SysBuf[15] = SysBuf[18];
				SysBuf[16] = '_';
				SysBuf[17] = SysBuf[19];
				SysBuf[18] = '_';
				SysBuf[19] = '%';
				PrintStr(SysBuf);
			}
			if (CheckBreak())
				return;
			RamOffSet = save;       /* reset address pointer */

            //ccr20140403 英文版的普通收款机需要保留Grand Total累计量，该计数器在每次Z-report后不清除
            if (ApplVar.Report.Type!=0 || ApplVar.Report.Period!=0 || (TESTBIT(DBLHIPRN,BIT6))) //ccr2016-02-23
                ResetReport();
		}
		else if (CheckBreak())
				return;
        RamOffSet = save + ApplVar.Report.Size;
    }

    if (!ApplVar.RepComputer && temp)
    {
		PrintLine('-');
    }
}


void PrintPluReport(short RepFor)
{
    ApplVar.Report.End = ApplVar.AP.Plu.Number;
    if (ApplVar.AP.Plu.RandomSize)
		ApplVar.Report.End =ApplVar.AP.Plu.RNumber;     /* calculate bytes to move 2nd part */
    if (!ApplVar.Report.End)
		return;
    ApplVar.Report.OffSet = ApplVar.AP.Plu.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.Plu.RecordSize;
    PrintReport(RepFor,ApplVar.AP.Plu.Size);
}
//ccr2014-09-09>>>>>>>>>>>>>>>>>>>
#if (PLUCODE_X)
BYTE Plu_XReport(BYTE keyno)
{
    if (ApplVar.CentralLock == X )
    {
        if (ApplVar.AP.KeyTable[keyno]>DEPT && ApplVar.AP.KeyTable[keyno]<PLU1)
        {
            ApplVar.NumberEntry = ApplVar.AP.KeyTable[keyno]-DEPT;
            if (TESTBIT(ApplVar.ArrowsAlfa,BIT7))
                    ApplVar.NumberEntry +=ApplVar.DeptKeys;

            keyno = DEPT_Key_ID;
            Appl_EntryCounter = 1;
        }
        if (keyno == PLU_Key_ID || keyno == DEPT_Key_ID)
        {// 在X档下,打印指定PLU的报表数据 //
            if (Appl_EntryCounter)
            {
                if (keyno == PLU_Key_ID)
                    PLU_X_Report = PLU1;
                else
                    PLU_X_Report = DEPT;

                ApplVar.ReportNumber = PLU_X_IDX;
                ApplVar.RepComputer = 0;
                ApplVar.FReport = ApplVar.CentralLock;
                GetReport(ApplVar.ReportNumber);
                ApplVar.FReport = 0;
                PLU_X_Report = 0;
                ClearEntry(0);
                return true;
            }
            else
                return false;
        }
    }
    return false;
}
#endif
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void PrintDeptReport()
{
    if (!ApplVar.AP.Dept.Number)
	return;
    ApplVar.Report.End = ApplVar.AP.Dept.Number;
    ApplVar.Report.OffSet = ApplVar.AP.Dept.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.Dept.RecordSize;
    PrintReport(AddrDept,ApplVar.AP.Dept.Size);
}

void PrintGroupReport()
{
    if (!ApplVar.AP.Group.Number)
	return;
    ApplVar.Report.End = ApplVar.AP.Group.Number;
    ApplVar.Report.OffSet = ApplVar.AP.Group.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.Group.RecordSize;
    PrintReport(AddrGroup,ApplVar.AP.Group.Size);
}


void PrintTendReport()
{
    if (!ApplVar.AP.Tend.Number)
	return;
    ApplVar.Report.End = ApplVar.AP.Tend.Number;
    ApplVar.Report.OffSet = ApplVar.AP.Tend.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.Tend.RecordSize;
    PrintReport(AddrTend,ApplVar.AP.Tend.Size);
}

void PrintPoRaReport()
{
    if (!ApplVar.AP.PoRa.Number)
	return;
    ApplVar.Report.End = ApplVar.AP.PoRa.Number;
    ApplVar.Report.OffSet = ApplVar.AP.PoRa.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.PoRa.RecordSize;
    PrintReport(AddrPoRa,ApplVar.AP.PoRa.Size);
}


void PrintCurrReport()
{
    if (!ApplVar.AP.Curr.Number)
	return;
    ApplVar.Report.End = ApplVar.AP.Curr.Number;
    ApplVar.Report.OffSet = ApplVar.AP.Curr.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.Curr.RecordSize;
    PrintReport(AddrCurr,ApplVar.AP.Curr.Size);
}


void PrintDrawReport()
{
    if (!ApplVar.AP.Draw.Number)
	return;
    ApplVar.Report.End = ApplVar.AP.Draw.Number;
    ApplVar.Report.OffSet = ApplVar.AP.Draw.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.Draw.RecordSize;
    PrintReport(AddrDrawer,ApplVar.AP.Draw.Size);
}

void PrintCorrecReport()
{
    if (!ApplVar.AP.Correc.Number)
	return;
    ApplVar.Report.End = ApplVar.AP.Correc.Number;
    ApplVar.Report.OffSet = ApplVar.AP.Correc.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.Correc.RecordSize;
    PrintReport(AddrCorr,ApplVar.AP.Correc.Size);
}

void PrintDiscReport()
{
    if (!ApplVar.AP.Disc.Number)
	return;
    ApplVar.Report.End = ApplVar.AP.Disc.Number;
    ApplVar.Report.OffSet = ApplVar.AP.Disc.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.Disc.RecordSize;
    PrintReport(AddrDisc,ApplVar.AP.Disc.Size);
}

void PrintTaxReport()
{
    if (!ApplVar.AP.Tax.Number)
	return;
    ApplVar.Report.End = ApplVar.AP.Tax.Number;
    ApplVar.Report.OffSet = ApplVar.AP.Tax.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.Tax.RecordSize;
    PrintReport(AddrTax,ApplVar.AP.Tax.Size);
}


void PrintPbFReport()
{
    if (!ApplVar.AP.Pb.Number || !ApplVar.AP.Pb.NumberOfPb)
	return;
    ApplVar.Report.End = ApplVar.AP.Pb.Number;
    ApplVar.Report.OffSet = ApplVar.AP.Pb.TotalOffSet;
    ApplVar.Report.Size = ApplVar.AP.Pb.RecordSize;
    PrintReport(AddrPBf,ApplVar.AP.Pb.Size);
}

void PrintPbtReport()
{
    if (!ApplVar.AP.Pb.Number || !ApplVar.AP.Pb.NumberOfPb)
		return;
    ApplVar.Report.End = ApplVar.AP.Pb.NumberOfPb;
    ApplVar.Report.OffSet = ApplVar.AP.Pb.PBTTotalOffset;
    ApplVar.Report.Size = ApplVar.AP.Pb.PBTRecordSize;
    PrintReport(AddrPBt,ApplVar.AP.Pb.PBTSize);
}


void PrintOpenPb()
{
	BYTE printed;
	UnLong TempL ;
	if (!ApplVar.AP.Pb.NumberOfPb)	   /* not active */
		return;

    ApplVar.Entry = ZERO;
	ApplVar.Report.End = ApplVar.AP.Pb.NumberOfPb;
	if (ApplVar.MultiplyCount && !ApplVar.RepComputer)
	{
		ApplVar.PbNumber = (unsigned short) BCDValueToULong(ApplVar.Qty1.Value, &TempL);
		if (ApplVar.MultiplyCount == 2)
		{
			ApplVar.Report.End = (unsigned short) BCDValueToULong(ApplVar.Qty2.Value, &TempL);                 /* end address */
			if (ApplVar.AP.Pb.NumberOfPb < ApplVar.Report.End)
				ApplVar.Report.End = ApplVar.AP.Pb.NumberOfPb;
		}
		else
			ApplVar.Report.End = ApplVar.PbNumber;  /* One total only */
		ApplVar.Report.Start = ApplVar.PbNumber;
	}
	else
		ApplVar.PbNumber = 1;
	printed = 0;
    for(; ApplVar.PbNumber <= ApplVar.Report.End; ApplVar.PbNumber++)
    {
		PbTotal(ApplVar.PbNumber, 0);   /* read */
		if (ApplVar.PB.Block)        /* check if open */
		{
		    if (ApplVar.Report.PointerType == 1 && ApplVar.Report.Pointer != (ApplVar.PB.Clerk - 1))
			continue;
				AmtRound(0, &ApplVar.PB.Amt);   /* round total amount */
		    if (ApplVar.RepComputer && ApplVar.RepComputer != 'C')      /* 'C' is only reset */
		    {
				*((WORD *)SysBuf) = PBTOT + ApplVar.PbNumber;
				SysBuf[3] = ApplVar.Report.Period;
				SysBuf[4] = ApplVar.Report.PointerType;
				if (ApplVar.Report.PointerType == 1)
				    *((WORD *)(SysBuf + 5)) = ApplVar.PB.Clerk - 1;
				else
				    *((WORD *)(SysBuf + 5)) = 0;//0xffff;//用来表示为开台挂帐,用来区分报表数据  //
				if (ApplVar.AP.Pb.Random & 0x0f)
				{
				    SysBuf[2] = 0;  /* type = random number */
				    memcpy(SysBuf+7, ApplVar.PB.Random, 7);
				    SendRecord(SysBuf, 7 + 7);     /* always max size 14 */
				}
				if (ApplVar.AP.Pb.Text)
				{
				    SysBuf[2] = 1;  /* type = text */
				    memcpy(SysBuf+7, ApplVar.PB.Text, ApplVar.AP.Pb.Text);
				    SendRecord(SysBuf, 7 + 24);     /* always max size 24 */
				}
				SendComp(7, &ApplVar.PB.Amt);
		    }
		    else if(!ApplVar.RepComputer)
		    {
				if (!printed)
			    {
					if (ApplVar.MultiplyCount)
						PrintRange();           /* print entered range */
				    if (ApplVar.Report.System == 1)
					{
						if (ApplVar.Report.PointerType == 1)
							PrintPointType();
					}
					else
						PrintReportHeader();
					printed = 1;
				}
				else
					ApplVar.MultiplyCount = 0;
				if (ApplVar.AP.Pb.Random & 0x0f)
				{
				    memcpy(ApplVar.Entry.Value, ApplVar.PB.Random, 7);
				}
				else
				    WORDtoBCD(ApplVar.Entry.Value, ApplVar.PbNumber);
				PrintQty(Msg[ZUOHAO].str, &ApplVar.Entry);
				if (ApplVar.AP.Pb.Text)
				    PrintStr(ApplVar.PB.Text);
				PrintAmt(Msg[JINE].str, &ApplVar.PB.Amt);       /* Skip 2 spaces in front */
		    }
		    if (CheckBreak())
			return;

		    Add(&ApplVar.SaleAmt, &ApplVar.PB.Amt);
		    if (ApplVar.FReport == Z)
		    {
				if (ApplVar.Report.PointerType == 1 || ApplVar.MultiplyCount) /* clerk report */
				   PbTotal(ApplVar.PbNumber, 2);   /* reset ApplVar.PB of current ApplVar.Clerk */
				else
				{
				    MemSet(&ApplVar.PB, sizeof(ApplVar.PB), 0);
				    PbTotal(ApplVar.PbNumber, 1);    /* write back empty table */
				}
		    }
		}
    }
//ccr2016-03-29	if (!ApplVar.RepComputer)
//ccr2016-03-29	    PrintLine('=');
    if (ApplVar.Report.PointerType != 1  && !ApplVar.MultiplyCount && /* not clerk report */
		ApplVar.FReport == Z)
    {
		if (ApplVar.AP.StartAddress[AddrTrack] && ApplVar.AP.Pb.BlockSize) /* trackbuffer ? */
		{
		    MemSet(SysBuf, ApplVar.AP.Pb.BlockSize, 0);
		    RamOffSet = ApplVar.AP.StartAddress[AddrTrack];
		    for (ApplVar.EmptyBlock = 0; ApplVar.EmptyBlock < ApplVar.AP.Pb.NumberOfBlocks; ApplVar.EmptyBlock++)
			WriteRam(SysBuf, ApplVar.AP.Pb.BlockSize);
		    ApplVar.EmptyBlock = 0;
		}
    }
    ApplVar.PbNumber = 0;
}

void PrintPointReport()
{
    ApplVar.SaleAmt = ZERO;
    ApplVar.SaleQty = ZERO;
    switch(ApplVar.Report.Type) {
	case REPORT_GRANDTOTAL://销售总计Print TOTALSALES recordes
        ApplVar.Report.End = 1;
        ApplVar.Report.Size = ApplVar.AP.Sales.RecordSize;
        ApplVar.Report.OffSet = 0;
        PrintReport(AddrTotl,ApplVar.AP.Sales.Size);
        break;
    case REPORT_GROUP://柜组统计
        PrintGroupReport();
        break;
    case REPORT_DEPARTMENT://部类统计
        PrintDeptReport();
        break;
    case REPORT_PLU://单品统计
        PrintPluReport(AddrPLU);
        break;
    case REPORT_TENDER://付款统计
        PrintTendReport();
        break;
    case REPORT_PORA://出入金统计
        PrintPoRaReport();
        break;
    case REPORT_DRAWER://钱箱统计
        PrintDrawReport();
        break;
    case REPORT_CORRECTION://更改统计
        PrintCorrecReport();
        break;
    case REPORT_DISCOUNT://折扣统计
        PrintDiscReport();
        break;
    case REPORT_CURRENCY://外币统计
        PrintCurrReport();
        break;
    case REPORT_TAX://计税统计
        PrintTaxReport();
        break;
    case REPORT_PBFUNC://桌帐统计
        PrintPbFReport();
        break;
    case REPORT_BALANCE://餐饮统计 /* open table */
        PrintOpenPb();
        break;
    case REPORT_TABLES:
        PrintPbtReport();
        break;
    case REPORT_INVENTORY://商品库存 /* ApplVar.Plu inventory */
        PrintPluReport(AddrRPLU);
        break;
	default :
	    break;
    }

    if ((ApplVar.MultiplyCount && ApplVar.Report.Type && (ApplVar.Report.Type == REPORT_BALANCE || ApplVar.Report.Type < REPORT_TENDER))
        || ApplVar.Report.Type == REPORT_INVENTORY)   /* Inventory ? */   //lyq
    {
        if (ApplVar.Size.Qty || ApplVar.Report.Type == REPORT_INVENTORY)
			PrintQty(Msg[SHULIANG].str, &ApplVar.SaleQty);
		PrintAmt(Msg[JINE].str, &ApplVar.SaleAmt);
		PrintLine('-');
    }
	ApplVar.MultiplyCount = 0;
}

/* you must set up the period, pointertype and pointer number */

void GetSystemReport(BYTE clearall )
{
    if (!ApplVar.RepComputer && !clearall)
    {
/*		if (!ApplVar.ClerkNumber)
		{
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI33);
		    return;
		}
		if (ApplVar.AP.SalPer.Number && !ApplVar.SalPerNumber)
		{
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI34);
		    return;
		}
*/
		if (!Appl_EntryCounter || Appl_EntryCounter > 5)
		{
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI26);
		    return;
		}

		ApplVar.PrintLayOut = 0x0a;
		CheckSlip();
    }
    if (!clearall)
    {
		MemSet(&ApplVar.Report, sizeof(ApplVar.Report), 0);
	    ApplVar.Report.Type = EntryBuffer[ENTRYSIZE - 3] & 0x0f;
		ApplVar.Report.Type *= 10;
	    ApplVar.Report.Type += EntryBuffer[ENTRYSIZE - 2] & 0x0f;
		ApplVar.Report.PointerType = EntryBuffer[ENTRYSIZE - 4] & 0x0f;
		ApplVar.Report.Period = EntryBuffer[ENTRYSIZE - 5] & 0x0f;
    }
    ApplVar.Report.System = 1;
    if (!SetUpReport() || clearall)
    {
		if (clearall)
		    ApplVar.ErrorNumber=0;
		else if (!ApplVar.RepComputer)
		{

		    if (TESTBIT(KPPRICE, BIT4))
				PrintHead1(1);       /* print rest of header */
			else
			    PrintHead1(PREHEADER);       /* print rest of header */
			PrintReportHeader();
            if (ApplVar.Report.Type < REPORT_BALANCE)
				PrintReportType();
		}
		if (Appl_EntryCounter < 5)
		    ApplVar.Report.Pointer = 0;
		else
		    ApplVar.Report.PointerEnd = ApplVar.Report.Pointer + 1;
		for(;ApplVar.Report.Pointer < ApplVar.Report.PointerEnd; ApplVar.Report.Pointer++)
		{
		    if (ApplVar.Report.PointerType == 1)
		    {
				ApplVar.ClerkNumber = ApplVar.Report.Pointer + 1;
				ReadClerk();
				if (!ApplVar.RepComputer &&	TRAINING &&
						Appl_EntryCounter < 5 &&	!TESTBIT(CLERKFIX, BIT6))
				       /* all pointers then skip training clerk ? */
				    continue;
		    }
		    PrintPointReport();
		    if (ApplVar.ErrorNumber)  /* break */
				break;
		}
		if (!clearall)
	    	ReportEnd(1);
    }
}

/**
 * 打印用户报表
 *
 * @author EutronSoftware (2016-10-18)
 *
 * @param xzNum
 */
void GetReport(BYTE xzNum)
{
    BYTE i, j, repnum;
    int l;

	if (!xzNum || xzNum>XZNUM+1)
	{
	    ApplVar.ErrorNumber=ERROR_ID(CWXXI26);
	    return;
    }

#if (BARCUSTOMER==1)
	if (xzNum==XZNUM)	// 条码会员报表  //
	{
		PrintBARCustomer(ApplVar.FReport);
		return;
	}
#endif

   	if (xzNum == XZNUM+1)
    	repnum = XZNUM;
   	else
		repnum = XZTitle[xzNum-1].Index-1;

    MemSet(&ApplVar.Report, sizeof(ApplVar.Report), 0);

    if (repnum == XZNUM)
    {
	    if (!TESTBIT(KEYTONE, BIT3))    /* print power up message ? */
			ApplVar.PrintLayOut = 0x03;     /* r & j */
		else
			ApplVar.PrintLayOut = 0x02;     /* r & j */
    }
    else
    {
		ApplVar.Report.PointerType = ApplVar.AP.ReportList[repnum].PointerType;
		ApplVar.PrintLayOut = ApplVar.AP.ReportList[repnum].PrintLayOut;
		ApplVar.Report.Period = ApplVar.AP.ReportList[repnum].Period;
    }
    if (!ApplVar.RepComputer)
    {//为在收款机上打印报表
		if (ApplVar.FReport != SET)
		{
		    if (TRAINING) /* Training ? */
		    {
				if (ApplVar.Report.PointerType != 1)
				{
				    ApplVar.ErrorNumber=ERROR_ID(CWXXI32);
				    return;
				}
				else if (TESTBIT(ApplVar.AP.ReportList[repnum].Options, BIT1))   /* all pointers ?*/
				{
				    ApplVar.ErrorNumber=ERROR_ID(CWXXI32);
				    return;
				}
		    }
		}
		if (CheckSlip())
		    return;
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
        if (TestSaveEJ())
            RefreshEJStart(true);
#endif
		if (TESTBIT(ApplVar.PrintLayOut,BIT1))     /* print on receipt ? */
		{
			if (TESTBIT(KPPRICE, BIT4))     /* print on receipt ? */
				PrintHead1(1);
			else
				PrintHead1(PREHEADER);
		}

		MemSet(SysBuf, sizeof(SysBuf), ' ');

		PrintStr(SysBuf);
		if (repnum == XZNUM)
		    strcpy(SysBuf, ApplVar.TXT.ReportType[REPORTTYPE_MAX]);//ccr2016-04-11, sizeof(ApplVar.TXT.ReportType[0]));
		else
		    strcpy(SysBuf, ApplVar.AP.ReportList[repnum].Name);//ccr2016-04-11, 16);
        //ccr2016-04-11>>>>>>>>>>>>>>>>>>>>>
        l = strlen(SysBuf);
        if (l>REPWIDTH) l=REPWIDTH;
        SysBuf[l++]='-';
        if (ApplVar.FReport==Z)
            SysBuf[l++]='Z';
        else
            SysBuf[l++]='X';
        SysBuf[l]=0;
        //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


		i = ApplVar.PrintLayOut;
		ApplVar.PrintLayOut &= 0xfc;            /* print Slip */

		PrintStr(SysBuf);

		ApplVar.PrintLayOut = i & 0xe7;
		SETBIT(ApplVar.PrintLayOut, BIT2);      /* Double Height */

		PrintStr_Center(SysBuf,true);//ccr2016-04-11

		ApplVar.PrintLayOut = i;
		PrintLine('=');

		if (repnum == XZNUM)        /* Pb Invoice report ? */
		{
		    if (!SetUpReport())
		    {
				StorePbInvoice(2);      /* Print Pb Invoice report */
				ReportEnd(0);
				return;
		    }
		}

		if (TESTBIT(ApplVar.AP.ReportList[repnum].Options, BIT0))   /* open drawer ?*/
		    OpenDrawer();
    }
    if (!SetUpReport())
    {
#if defined(TEND_PREFIX)//ccr2016-08-08
        Prefix1 = PREFIX_1;
        Prefix2 = PREFIX_2;
#endif

		if (ApplVar.RepComputer || TESTBIT(ApplVar.AP.ReportList[repnum].Options, BIT1))   /* all pointers ?*/
		    ApplVar.Report.Pointer = 0;
		else
		    ApplVar.Report.PointerEnd = ApplVar.Report.Pointer + 1;
		for(;ApplVar.Report.Pointer < ApplVar.Report.PointerEnd; ApplVar.Report.Pointer++)
		{
		    if (ApplVar.Report.PointerType == 1)
		    {//report for clerk
				ApplVar.ClerkNumber = ApplVar.Report.Pointer + 1;
				ReadClerk();
				if (!ApplVar.RepComputer &&	TRAINING &&
					TESTBIT(ApplVar.AP.ReportList[repnum].Options, BIT1) &&
					!TESTBIT(CLERKFIX, BIT6))
				       /* all pointers then skip training clerk ? */
				    continue;
		    }
			if (ApplVar.RepComputer != 'C')
			{
			     if((ApplVar.Report.PointerType == 5 || ApplVar.Report.PointerType == 1) && CheckTotal() == 0) //lyq20040115
			     	continue;
			     PrintPointType();
			}
			     /* % comparison ? */
			if (!ApplVar.RepComputer && TESTBIT(ApplVar.AP.ReportList[repnum].Options, BIT4))
			{
				i = ApplVar.PrintLayOut;
				ApplVar.PrintLayOut = 0;
                ApplVar.Report.Type = REPORT_GRANDTOTAL;                /* get total sales from this pointer */
				j = ApplVar.FReport;
				ApplVar.FReport = X;            /* simulate X */
				PrintPointReport();
				ApplVar.FReport = j;
				ApplVar.SubTotal = ApplVar.SaleAmt;             /* comparison total */
				ApplVar.Report.System = 4;              /* set percentage report */
				ApplVar.PrintLayOut = i;
			}
		    for (i = 0; i < 16; i++)
		    {
				ApplVar.Report.Type = ApplVar.AP.ReportList[repnum].Link[i];
				if (!ApplVar.Report.Type)   /* end list */
				    break;
				ApplVar.Report.Type--;
				if (ApplVar.MultiplyCount && !ApplVar.RepComputer)      /* range report? */
					i = 16;         /* only print range !! */
				PrintPointReport();
				if (ApplVar.ErrorNumber)
				    break;
		    }
		    if (ApplVar.ErrorNumber)
			 break;
		}
		ReportEnd(!TESTBIT(ApplVar.AP.ReportList[repnum].Options, BIT3));   /* add Z-Count */
		if (TESTBIT(ApplVar.AP.ReportList[repnum].Options, BIT2) && ApplVar.FReport == Z)
		{
			if ((ApplVar.Report.PointerType == 1) && TRAINING)
		    	MemSet(ApplVar.ReceiptNumber+RECNUM, TRAINNUM, 0);     /* reset training receiptnumber ?*/
			else
		    	MemSet(ApplVar.ReceiptNumber, RECNUM, 0);     /* reset receiptnumber ?*/
		}
	   	if (xzNum==1 && ApplVar.FReport==Z)
   			RESETBIT(ApplVar.MyFlags,ZREPORT);
    }
}
#if (BARCUSTOMER==1)
//打印条码会员报表,xz用来标识是X报表还是Z报表,xz=0时，打印全部会员清单 //
void PrintBARCustomer(BYTE xz)
{
	    if(ApplVar.AP.Plu.RandomSize == 7 && TESTBIT(ART_LEVEL, BIT3))
	    {// 条码会员的编码长度必须是 13位  //
	      	SETBIT(ApplVar.PrintLayOut, BIT2);
		    RJPrint(0,Msg[BARREPORT].str);//  会 员 消 费 表  //
	     	RESETBIT(ApplVar.PrintLayOut, BIT2);
	     	RFeed(1);
			PrintLine2('=');
	    	memset(SysBuf,' ',PRTLEN);
	    	CopyFrStr(SysBuf+5,Msg[TMHYHAO].str);// 会员号  //
	    	strcpy(SysBuf+PRTLEN-9,Msg[XFJE].str);
	    	RJPrint(0,SysBuf);
			PrintLine2('=');
		    for (ApplVar.PluNumber = 0; ApplVar.PluNumber < ApplVar.AP.Plu.RNumber; ApplVar.PluNumber++)
		    {
				memset(&ApplVar.Plu,0,sizeof(ApplVar.Plu));

		    	ReadPlu();

				if (ApplVar.Plu.Random[6]!=0x09 || ApplVar.Plu.Random[5]!=0x99)
					continue;
				ApplVar.Qty = ZERO;
				ApplVar.Price = ZERO;
				memcpy(&ApplVar.Price.Value,ApplVar.Plu.Price[0],5);
				if (!CheckNotZero(&ApplVar.Price) && xz!=0)
					continue;// 没有销售时，不打印 //

				memcpy(ApplVar.Qty.Value,ApplVar.Plu.Random,ApplVar.AP.Plu.RandomSize);

				strcpy(ProgLineMes,FormatQtyStr(0,&ApplVar.Qty,15));
				RJPrint(0,FormatAmtStr(ProgLineMes,&ApplVar.Price,PRTLEN));

				if (xz==Z)
				{
					memset(ApplVar.Plu.Price[0],0,5);
					WritePlu();
				}
			    if (KbHit())
			    {//any key for stop
			    	Getch();
			    	break;
			    }
		    }
			ApplVar.BufKp = 0;
		    SETBIT(ApplVar.MyFlags,PRNTRAI);
			PrintLine2('=');
			if (xz!=0)
			{
				ApplVar.FReceipt = 1;
			    ReceiptIssue(1);
			}
	    }
}
#endif
/**
 * 显示汇总数据
 *
 * @author EutronSoftware (2016-03-08)
 */
void DispGrandTotal()
{
    BYTE temp, periods, periodnumber;
    WORD skip, total;
    UnLong save ;     /* unsigned long save; */


    ApplVar.SaleAmt = ZERO;
    ApplVar.SaleQty = ZERO;

    ApplVar.Report.System=0;
    ApplVar.Report.Period = 0;
    ApplVar.Report.Type =0;
    ApplVar.Report.Start = 0;
    ApplVar.Report.End = 1;
    ApplVar.Report.PointerType = 0;
    ApplVar.Report.Pointer=0;
    ApplVar.Report.PointerEnd=1;
    ApplVar.Report.Size = ApplVar.AP.Sales.RecordSize;
    ApplVar.Report.OffSet = 0;

    mClearScreen();
    memcpy(&ApplVar.Size, &ApplVar.AP.Sales.Size, sizeof(ApplVar.Size));    /* copy construction to work */
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
    RamOffSet = ApplVar.AP.StartAddress[AddrTotl] + ApplVar.Report.OffSet +
                (WORD)ApplVar.Report.Pointer * ApplVar.Size.Length * periods +
                (WORD)ApplVar.Size.Length * periodnumber +
                (WORD)ApplVar.Report.Start * ApplVar.Report.Size;

    ReadTotal();     /* read correct total */
    if (CheckNotZero(&ApplVar.Total.RetQty))   /* Not Zero then */
        SETBIT(ApplVar.Total.RetQty.Sign, BIT7); /* Always negative */


    if (TESTBIT(ApplVar.Total.Qty.Sign, BIT2))     /* used ? */
    {
        ApplVar.Total.Qty.Sign &= 0x83;
        Add(&ApplVar.SaleQty, &ApplVar.Total.Qty);
        Add(&ApplVar.SaleAmt, &ApplVar.Total.Amt);
        temp=0;
        if (ApplVar.Size.Amt)
        {
            ApplVar.Amt = ApplVar.Total.Amt;
            Subtract(&ApplVar.Amt, &ApplVar.Total.Disc);       /* calculate gross */
            if (((ApplVar.Total.Amt.Sign & 0x80)==0) && ((ApplVar.Total.Disc.Sign & 0x80)!=0))
                ApplVar.Amt.Sign = ApplVar.Total.Amt.Sign;
            DispStrXY(DispAmtStr(Msg[JINE].str, &ApplVar.Amt,SCREENWD),0,temp++);
        }
        if (ApplVar.Size.Disc)
        {
            DispStrXY(DispAmtStr(Msg[SETDISC].str, &ApplVar.Total.Disc,SCREENWD),0,temp++);
            DispStrXY(DispAmtStr((char*)cMsgNET, &ApplVar.Total.Amt,SCREENWD),0,temp++);
        }
#if (SCREENLN>6)
        if (ApplVar.Size.RetQty)
        {
            DispStrXY(DispQtyStr(Msg[TUIHUO].str, &ApplVar.Total.RetQty,SCREENWD),0,temp++);
        }
#endif
    }

}

