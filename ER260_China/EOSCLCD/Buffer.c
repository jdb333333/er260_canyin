#define BUFFER_C
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"

/****************************************************************/

void SetPrintLayOut(BYTE playout)
{
	if (ApplVar.PbNumber)           /* ApplVar.PB active ? */
		ApplVar.PrintLayOut = ApplVar.PbF.Print;
	else
		ApplVar.PrintLayOut = playout;
	if (TESTBIT(ApplVar.BufRec, BIT3) && (SLIP == 2 || SLIP == 3 || SLIP == 6))
	{       /* Standard printer or DITRON Sprint as  slip ? */
		ApplVar.PrintLayOut |= 0x08;    /* set slip active */
	}
	ApplVar.PrintLayOut &= ApplVar.BufRec;
}

void ProcessDeptRecord(struct TRANSRECORD *rec,BYTE lastrefund,BYTE discount)
{
	if (rec->Key.Code > PLU1) /* plu ? */
	{
		ApplVar.PluNumber = rec->Key.Code - PLU1 - 1;
		ReadPlu();
		ApplVar.DeptNumber = ApplVar.PluDept;
	}
	else
		ApplVar.DeptNumber = rec->Key.Code - DEPT - 1;
	ReadDept();
	if (ApplVar.BufCC || ApplVar.FTrvoid)   /* reset CC bit ? */
	{
		if (ApplVar.FTrvoid)            /* Transaction void ? */
		{
			ApplVar.Qty = rec->Qty;
			ApplVar.Qty.Sign ^= 0x80;
			ApplVar.Amt = rec->Amt;
			ApplVar.Amt.Value[BCDLEN-1] = 0;        /* clear PriceLevel */
			Multiply(&ApplVar.Amt, &ApplVar.Qty);
			RoundBcd(&ApplVar.Amt, 0);
			if (ApplVar.Qty.Sign & 0x03)     /* ApplVar.Qty with decimal point */
				AmtRound(1, &ApplVar.Amt);
			if (lastrefund)
				ApplVar.RetQty = ApplVar.Qty;
			else
				ApplVar.RetQty = ZERO;
			if (discount)
			{
				//cc 2006-06-05 for SubDisc>>>>>>>>>>>>>>>
				//ApplVar.Amt.Sign ^= 0x80;
				//ApplVar.DiscAmt = ApplVar.Amt;
				if(rec->Group == 0)
				{
					ApplVar.Amt.Sign ^= 0x80;
					ApplVar.DiscAmt = ApplVar.Amt;
				}
				else
				{
					ApplVar.Amt = ZERO;
					ApplVar.DiscAmt = ZERO;
				}
				//cc 2006-06-05 for SubDisc<<<<<<<<<<<<<<
				ApplVar.Qty = ZERO;
			}
			else
			{
				ApplVar.DiscAmt = ZERO;
				if (rec->Key.Code > PLU1 && !discount) /* plu ? */
		    	{
				    ApplVar.MixQty = ZERO;
					CheckMixMatch(2);
				}
	    	}
			ApplVar.Cost = ZERO;
			if ((rec->Key.Code > PLU1) && ApplVar.AP.Plu.Cost && !TESTBIT(ART_LEVEL, BIT2)) /* plu with costprice ? */
			{
		    	memcpy(ApplVar.Cost.Value, ApplVar.Plu.Cost, ApplVar.AP.Plu.PriceSize);
				if (discount)
					ApplVar.Cost.Sign ^= 0x80;
				else
				{
					Multiply(&ApplVar.Cost, &ApplVar.Qty);
			    	RoundBcd(&ApplVar.Cost, 0);
				}
			}
		}
		ApplVar.GroupNumber = ApplVar.Dept.Group;
		AddGroupTotal();
		AddDeptTotal();
		if (rec->Key.Code > PLU1) /* plu ? */
		{
			AddPluTotal();
		    if (ApplVar.FTrvoid && ApplVar.AP.Plu.InvSize)     /* inventory ? */
		    {
				Subtract(&ApplVar.PluInventory, &ApplVar.Qty);
				WritePluInventory();
		    }
		}
	}
}

void CheckModifier(BYTE cmd,BYTE i)      /* check modifier for ApplVar.KP or second receipt */
{
    BYTE j, count;

    if (i < (ApplVar.RGItems.RGNumber - 1))
    {
		if ((ApplVar.RGItems.RGBuf[i + 1].Key.Code < MODI) ||
	    	(ApplVar.RGItems.RGBuf[i + 1].Key.Code > MODI + 1000)) /* modifier ? */
	    	return;    /* no modifier */
    }
    else
		return;     /* last entry in buffer */

    count = ApplVar.RGItems.RGBuf[i + 1].Key.Code - MODI;
    memcpy(ApplVar.ModNum, &ApplVar.RGItems.RGBuf[i + 1].Qty, sizeof(ApplVar.ModNum));
    MemSet(SysBuf, sizeof(SysBuf), ' ');
    for(j = 0; j < count; j++)
    {
	ApplVar.ModiNumber = ApplVar.ModNum[j];
	ReadModi();
	strcpy(SysBuf + 5, ApplVar.Modi.Name);
	if (ApplVar.BufRec)
	    PrintStr(SysBuf + 3);
	if (!ApplVar.BufKp)
	    continue;
	if (cmd == KP_BLACK)       /* previous red then black*/
	    PrintKp(KP_NORMAL, SysBuf);
	else
	    PrintKp(KP_BLACK, SysBuf);     /* print modifier red */
     }
}


/* if ApplVar.BufCC then add customer count at end of transaction */
/* if ApplVar.BufRec then second receipt(复制收据) */
/* if ApplVar.BufKp then sent ApplVar.KP */
/* if BufVoid then TransAction void */
void ProBuf()
{
    char temp[17];
    BYTE i, *item, cmd, lastrefund;
    WORD ticket;
	struct TRANSRECORD rec;
	UnLong TempL ;
	lastrefund = 0;

    for(i = 0; i < ApplVar.RGItems.RGNumber; i++)
    {
		ApplVar.FPluEnq = 0;
		if (i && ApplVar.RGRec.Key.Code > PLU1 &&
				TESTBIT(ApplVar.AP.Options.Plu, BIT0) && ApplVar.Plu.Link)
						/* ApplVar.Plu Link on previous item ? */
		{
			ApplVar.RGRec.Key.Code = GetLinkNumber();
			if (ApplVar.RGRec.Key.Code &&
					ApplVar.RGRec.Key.Code != ApplVar.RGItems.RGBuf[i-1].Key.Code)  /* Random Code found ? */
			{
			 /*  ApplVar.Price Level is stored in ApplVar.RGRec.Amt.Value[BCDLEN-1] */

				ApplVar.PluNumber = ApplVar.RGRec.Key.Code - PLU1 - 1;
				ReadPlu();      /* get correct price */
				MemSet(&ApplVar.RGRec.Amt, BCDLEN, 0);          /* Clear previous except Level*/
				memcpy(ApplVar.RGRec.Amt.Value,
				ApplVar.Plu.Price[ApplVar.RGRec.Amt.Value[BCDLEN-1]], ApplVar.AP.Plu.PriceSize);
				i--;
				ApplVar.FPluEnq = 2;    /* Set Linked PLU for mixmatch */
			}
			else
				ApplVar.RGRec = ApplVar.RGItems.RGBuf[i];
		}
		else
			ApplVar.RGRec = ApplVar.RGItems.RGBuf[i];
		if (ApplVar.RGRec.Key.Code == 299)
		{//为取消交易
		    if (ApplVar.BufRec)    /* second receipt */
		    {
				ApplVar.SubTotal = ApplVar.SaleAmt;
				SetPrintLayOut(PLAYOUT);
#if defined(TEND_PREFIX)//ccr2016-08-08
                Prefix1 = PREFIX_1;
                Prefix2 = PREFIX_2;
#endif
				RegiEnd();
				ApplVar.FSale = 0;
		    }
		    continue;
		}
		else if (ApplVar.RGRec.Key.Code > DEPT)
		{
			ProcessDeptRecord(&ApplVar.RGRec, lastrefund, 0);
			rec = ApplVar.RGRec;
		    if (ApplVar.BufRec || ApplVar.BufKp)    /* second receipt or ApplVar.KP?*/
		    {
				if (!CheckNotZero(&ApplVar.RGRec.Qty))    /* if zero qty */
				    continue;
				ApplVar.Qty = ApplVar.RGRec.Qty;
				ApplVar.Amt = ApplVar.RGRec.Amt;
				ApplVar.Amt.Value[BCDLEN-1] = 0;        /* clear PriceLevel */
				ApplVar.MultiplyCount = 1;
				ApplVar.Price = ApplVar.Amt;
				Multiply(&ApplVar.Amt, &ApplVar.Qty);
				RoundBcd(&ApplVar.Amt, 0);
				if (ApplVar.Qty.Sign & 0x03)     /* ApplVar.Qty with decimal point */
				    AmtRound(1, &ApplVar.Amt);
				if (ApplVar.BufRec)    /* second receipt ?  */
				{
					SetPrintLayOut(ApplVar.Dept.Print);
				    //PrintMultiply();
				    if (ApplVar.RGRec.Key.Code > PLU1) /* plu ? */
						item = (BYTE *)GetPluPrint(temp, KP_NORMAL);
				    else
						item = (BYTE *)ApplVar.Dept.Name;
				    PrintQtyAmt(&ApplVar.Qty, (char *)item,(BCD *)&ApplVar.Amt,true);
					Add(&ApplVar.SubTotal, &ApplVar.Amt);   /* add to subtotal for slippage */
				    if (ApplVar.RGRec.Key.Code > PLU1 && ApplVar.FPluEnq != 2) /* plu ? */
				    {
						ApplVar.MixQty = ZERO;
						ApplVar.DiscAmt = ZERO;
						CheckMixMatch(0);
						Add(&ApplVar.SubTotal, &ApplVar.DiscAmt);       /* add to subtotal for slippage */
						ApplVar.DiscAmt = ZERO;
				    }
				    CheckModifier(0, i);
				    ApplVar.FSale = 1;
				}
				if (!ApplVar.BufKp)
				    continue;
				ApplVar.KP.Next |= ApplVar.Dept.Kp;
				if(!(ApplVar.Dept.Kp & ApplVar.KP.Current))
				    continue;
				if (ApplVar.KP.Error & ApplVar.KP.Current)
				    continue;        /* already error on ? */
				if (!(ApplVar.KP.Used & ApplVar.KP.Current))
				{
				    ApplVar.KP.Used |= ApplVar.KP.Current;
				    if ((ApplVar.KP.Prt->type & 0x0f) > 1)
				    {
						if (!CheckOnLine(ApplVar.KP.Prt->port-1))
						{
						    ApplVar.KP.Error |= ApplVar.KP.Current;
						    continue;
						}
				    }
				    if (!(ApplVar.KP.Prt->type & 0x80) || ApplVar.FKpGroup)    /* no single tickets */
                        KpHeader();
				}
				if (ApplVar.KP.Prt->type & 0x40 && !(ApplVar.Qty.Sign & 0x83) && !ApplVar.FKpGroup)    /* single ticket with qty 1 */
				{
					ticket = (WORD)BCDValueToULong(ApplVar.Qty.Value, &TempL);
				    ApplVar.Qty = ONE;
				}
				else
				    ticket = 1;
				Add(&ApplVar.SubTotal, &ApplVar.Amt);       /* add subtotal ApplVar.KP receipt */
				while (ticket)
				{
				    ticket--;
				    if ((ApplVar.KP.Prt->type & 0x80) && !ApplVar.FKpGroup)    /* single tickets ?*/
						KpHeader();
				    if (TESTBIT(ApplVar.Qty.Sign, BIT7))    /* negative */
				    {
						PrintKp(KP_RED, Msg[HUANHUO].str);
						cmd = KP_RED;                    /* print red */
				    }
				    else
						cmd = KP_DBLWIDTH;//ccr2017-02-07
				    PrintArticle(cmd, 1);
				    if (!TESTBIT(KPPRICE,BIT0) &&      /* print price only when different ? */
						ApplVar.RGRec.Key.Code > PLU1)  /* PLU ? */
				    {
						ApplVar.Qty1 = ZERO;
						memcpy(ApplVar.Qty1.Value, ApplVar.Plu.Price[0], ApplVar.AP.Plu.PriceSize);
						if (!CompareBCD(&ApplVar.Qty1, &ApplVar.Price))   /* price the same */
						{
						    CheckModifier(cmd, i);
						    if (ApplVar.KP.Prt->type & 0x10)    /* Print Line after article ?*/
						    {
								MemSet(SysBuf, sizeof(SysBuf), '-');    /* print line */
								PrintKp(cmd, SysBuf);
						    }
						    if ((ApplVar.KP.Prt->type & 0x80) && !ApplVar.FKpGroup)    /* single tickets ?*/
								KpEnd(1);
						    continue;
						}
				    }
//				    MemSet(SysBuf, sizeof(SysBuf), ' ');    /* print price */
//				    if (ApplVar.KP.Prt->type & 0x40 && !(ApplVar.Qty.Sign & 0x83) && !ApplVar.FKpGroup)    /* single ticket with qty 1 */
//						FormatAmt(SysBuf + 30, &ApplVar.Price);
//				    else
//						FormatAmt(SysBuf + 30, &ApplVar.Amt);
//				    PrintKp(cmd, SysBuf + 10);
				    CheckModifier(0, i);
				    if (ApplVar.KP.Prt->type & 0x10)    /* Print Line after article ?*/
				    {
						MemSet(SysBuf, sizeof(SysBuf), '-');    /* print line */
						PrintKp(cmd, SysBuf);
				    }
				    if ((ApplVar.KP.Prt->type & 0x80) && !ApplVar.FKpGroup)    /* single tickets ?*/
						KpEnd(1);
				}       /* end ticket */
		    }
		}
		else if (ApplVar.RGRec.Key.Code > MODI && ApplVar.RGRec.Key.Code < MODI + 1000)   /* modifier */
	    	continue;       /* only stored for Kp and second receipt */
		else if (!ApplVar.BufCC && !ApplVar.BufRec && !ApplVar.FTrvoid) /* only ApplVar.KP ? */
	    	continue;
		else
		{
			if (ApplVar.FTrvoid)            /* Transaction void ? */
			{
				ApplVar.Qty = ApplVar.RGRec.Qty;
				ApplVar.Amt = ApplVar.RGRec.Amt;
				ApplVar.Amt.Sign ^= 0x80;       /* invert sign for adding in report */
				ApplVar.Qty.Sign ^= 0x80;
				ApplVar.RetQty = ZERO;
				ApplVar.DiscAmt = ZERO;
				ApplVar.Cost = ZERO;
			}
			if (ApplVar.RGRec.Key.Code < CORREC + 100)  /* correction */
			{
				ApplVar.CorrecNumber = ApplVar.RGRec.Key.Code - CORREC - 1;
			    ReadCorrec();
				if (ApplVar.BufCC || ApplVar.FTrvoid)    /* reset CC bit ? */
				{
					if (ApplVar.FTrvoid)            /* Transaction void ? */
					{
						if ((ApplVar.Correc.Options & 0x07) == 2)               /* refund ? */
							lastrefund = 2;
					}
					if(!TESTBIT(ApplVar.SubDiscFlag,BIT1))		/* cc 2006-10-11 for SubDisc */
				    	AddCorrecTotal();
				}
			}
	    	else if (ApplVar.RGRec.Key.Code < DISC + 100)  /* discount */
			{
				ApplVar.DiscNumber = ApplVar.RGRec.Key.Code - DISC - 1;
	    		ReadDisc();
				if (ApplVar.BufRec && CheckNotZero(&ApplVar.RGRec.Amt))  /* second receipt */
	    		{
					short j;
					SetPrintLayOut(ApplVar.Disc.Print);

					//lyq2003 modify for print format start
						MemSet(ProgLineMes, sizeof(ProgLineMes), ' ');
						j = strlen(ApplVar.Disc.Name);
						if (j>10)
							j = 10;
						strncpy(ProgLineMes,ApplVar.Disc.Name,j);
						ProgLineMes[j]=' ';
					   	if (!TESTBIT(ApplVar.Disc.Options, BIT2) && /* percentage ? */
					   		TESTBIT(ApplVar.Disc.Options, BIT0 + BIT1))  /* and open, open/fixed */
					   	{
						   	ApplVar.RGRec.Qty.Sign = 0x02;
							FormatQty(ProgLineMes + 18, &ApplVar.RGRec.Qty);	 //lyq modified 2003\11\4
							ProgLineMes[20] = '%';
							j = 21;
							ProgLineMes[j] = 0;
						}
						if(CheckNotZero(&ApplVar.RGRec.Amt))
					    {
					    	PrintAmt(ProgLineMes, &ApplVar.RGRec.Amt);
							Add(&ApplVar.SubTotal, &ApplVar.RGRec.Amt);             /* add for slip page */
						}
					//lyq2003 modify for print format end


					Add(&ApplVar.SubTotal, &ApplVar.RGRec.Amt);             /* add for slip page */
					ApplVar.FSale = 1;
				}
				if (ApplVar.BufCC || ApplVar.FTrvoid)   /* reset CC bit ? */
				{
					if (ApplVar.FTrvoid)            /* Transaction void ? */
					{
						ApplVar.Qty = ONE;
						ApplVar.Qty.Sign ^= 0x80;
					}
					if(!TESTBIT(ApplVar.SubDiscFlag,BIT1))		/* cc 2006-10-11 for SubDisc */
	    				AddDiscTotal();
					if (ApplVar.FTrvoid && (TESTBIT(ApplVar.Disc.Options, BIT4)))           /* Transaction void & item discount ? */
					{
						rec.Amt = ApplVar.Amt;
						rec.Qty = ONE;
						ProcessDeptRecord(&rec, lastrefund, 1);
					}
				}
    		}
			else if (ApplVar.RGRec.Key.Code < DRAW + 100)    /* Drawer, Tip ApplVar.Total ? */
			{
    			ApplVar.DrawNumber = ApplVar.RGRec.Key.Code - DRAW - 1;
				ReadDrawer();
    			if (ApplVar.BufRec && ApplVar.RGRec.Qty.Value[0] == 0x01)    /* second receipt */
				{
					SetPrintLayOut(PLAYOUT);
					ApplVar.RGRec.Amt.Sign ^= 0x80; /* positive on receipt  */
									/* negative in report ! */
					PrintAmt(ApplVar.Draw.Name, &ApplVar.RGRec.Amt);
					Add(&ApplVar.SubTotal, &ApplVar.RGRec.Amt);             /* add for slip page */
				}
    			if (ApplVar.FTrvoid)    /* TRVoid ? */
					AddDrawerTotal();
			}
			else if (ApplVar.RGRec.Key.Code < PBF + 100)   /* pb function */
			{
				if (ApplVar.FTrvoid)
				{
					ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
					return;
				}
				ApplVar.PbFNumber = ApplVar.RGRec.Key.Code - PBF - 1;
				ReadPbF();
			    if (ApplVar.BufRec)
				{
					SetPrintLayOut(ApplVar.PbF.Print);
					temp[0] = ApplVar.PbF.Options & 0x0f;
		    		if (TESTBIT(ApplVar.PbF.Options, BIT7) &&
			    		(temp[0] == 4 || temp[0] == 5 || temp[0] == 7))  /* print or split */
					ApplVar.FNoTime = 1;    /* set don't print time */
					switch (ApplVar.PbF.Options & 0x0f)
		    		{
			    	case 0:
			    		if (ApplVar.PbNumber)
						{
						    PrintAmt(ApplVar.PbF.Name, &ApplVar.RGRec.Amt);
							break;
						}
					case 1:
				    case 4:     /* print Pb */
				    case 5:     /* print Pb SI */
					case 7:
						ApplVar.PbNumber = *((WORD *) ApplVar.RGRec.Qty.Value);
					    PbTotal(ApplVar.PbNumber, 0);   /* read */
						ApplVar.Entry = ApplVar.RGRec.Amt;
						ApplVar.PB.Lines = 0;
					    PrintPbHeader();
						if ((ApplVar.PbF.Options & 0x0f) == 7)
					    {
						    PrintMessage(FKFZHANG);   /* split */
						    ApplVar.FPb = 0;
					    }
						break;
					case 8:
						PrintMessage(ZHUANRUDAO);
						PrintQty(Msg[ZUOHAO].str, &ApplVar.RGRec.Amt);
					    break;
				    case 10:        /* covers */
					    PrintQty(Msg[RENSHU].str, &ApplVar.RGRec.Amt);
						break;
					default:
                        if  (CheckNotZero(&ApplVar.RGRec.Amt))   //ccr2016-07-20为0时不打印"确认结账"/FINALIZE
			    			PrintAmt(ApplVar.PbF.Name, &ApplVar.RGRec.Amt);
					    break;
					}
				}
			    if (ApplVar.BufCC)
			    AddPbFTotal();
			}
			else if (ApplVar.RGRec.Key.Code < PORA + 100)    /* ApplVar.PoRa */
		    {
			    ApplVar.PoRaNumber = ApplVar.RGRec.Key.Code - PORA - 1;
				ReadPoRa();
			    if (ApplVar.BufRec)    /* second receipt */
				{
#if defined(TEND_PREFIX)//ccr2016-08-08
                    Prefix1 = PREFIX_1;
                    Prefix2 = PREFIX_2;
#endif
					SetPrintLayOut(ApplVar.PoRa.Print);
					PrintAmt(ApplVar.PoRa.Name, &ApplVar.RGRec.Amt);
					Add(&ApplVar.SubTotal, &ApplVar.RGRec.Amt);             /* add for slip page */
					ApplVar.FSale = 1;
				}
			    if (ApplVar.BufCC || ApplVar.FTrvoid)    /* reset CC bit ? */
				{
					AddPoRaTotal();
					if (!TESTBIT(ApplVar.PoRa.Options, BIT1) || ApplVar.FPb)
					{
						if (ApplVar.PoRa.Drawer)
					    {
						    ApplVar.DrawNumber = ApplVar.PoRa.Drawer - 1;
						    AddDrawerTotal();
					    }
						if (ApplVar.FTrvoid)
							ApplVar.FTrvoid = 2;            /* set don't calculate tax */
					}
			    }
			}
	    else if (ApplVar.RGRec.Key.Code < TEND + 100)    /* tender */
		{
	    	if (*((WORD *)ApplVar.RGRec.Qty.Value)) /* currency used ? */
			{
				ApplVar.CurrNumber = *((WORD *) ApplVar.RGRec.Qty.Value) - CURR;
			    ReadCurr();
				ApplVar.FCurr = 1;
			}
	    	ApplVar.TendNumber = ApplVar.RGRec.Key.Code - TEND - 1;
			ReadTender();
		    if (ApplVar.BufRec)    /* second receipt */
			{
				SetPrintLayOut(ApplVar.Tend.Print);
			    if (ApplVar.FCurr)
				{
					PrintStr(ApplVar.Curr.Name);
				    ApplVar.AmtDecimal = ApplVar.Curr.Options & BIT0;
					Prefix1 = ApplVar.Curr.Prefix1;
				    Prefix2 = ApplVar.Curr.Prefix2;
					Subtract(&ApplVar.SubTotal, &ApplVar.RGRec.Amt);                /* subtract for slip page */
				    GetCurrency(0, &ApplVar.RGRec.Amt);
			    }
				PrintAmt(ApplVar.Tend.Name, &ApplVar.RGRec.Amt);
				ApplVar.AmtDecimal = NO_DECIMAL;
			    Prefix1 = PREFIX_1;
				Prefix2 = PREFIX_2;
			}
	    	if (ApplVar.BufCC || ApplVar.FTrvoid)    /* reset CC bit ? */
			{
				if (ApplVar.FTrvoid)            /* Transaction void ? */
				{
					ApplVar.Qty = ONE;
					ApplVar.Qty.Sign ^= 0x80;
				}
				if (ApplVar.FCurr)
				    AddCurrTotal();
				else
					AddTenderTotal();
				if (ApplVar.PbNumber)
					AddPbtTotal();
			}
	    	ApplVar.FCurr = 0;
		}
		//ccr20130425 增加把小计(SUB)操作存入RGBuf的功能>>>>>>>>>>>>>>>>>>>>>>
		else if (ApplVar.RGRec.Key.Code == SUB)
		{
			if (ApplVar.BufRec)
			{
				SetPrintLayOut(0x0a);
				PrintAmt(Msg[XIAOJI].str, &ApplVar.RGRec.Amt);
			}
		}
		//<<<<<<<<<<<<<<<<<<
    	else if (ApplVar.RGRec.Key.Code == NUMBER)    /* number entry ? */
		{
			if (ApplVar.BufRec)
    		{
				SetPrintLayOut(0x0a);
				PrintNumber(&ApplVar.RGRec.Qty);
			}
		}
		else if (ApplVar.RGRec.Key.Code == SUSPEND)             /* suspend ? */
		{
			if (ApplVar.FTrvoid)
			{//如果此收据被挂起过,禁止取消交易(不合理)
				ApplVar.ErrorNumber=ERROR_ID(CWXXI01);
				return;
			}
			if (ApplVar.BufRec)
			{
				SetPrintLayOut(0x0b);
				//ccr20130425 PrintAmt(Msg[CWXXI21].str, &ApplVar.RGRec.Amt);
                PrintAmt(Msg[GUAQI].str, &ApplVar.RGRec.Amt);     //ccr20130425
			}
		}
	    else if (ApplVar.RGRec.Key.Code < SALPER + 256) /* SalesPerson ? */
		{
#if (salNumber)
			ApplVar.SalPerNumber = ApplVar.RGRec.Key.Code - SALPER;
		    ReadSalPer();
			if (ApplVar.BufRec)
			{
					if (TESTBIT(ApplVar.SalPer.Options, BIT0))      /* double ? */
						SetPrintLayOut(0x0f);
					else
						SetPrintLayOut(0x0a);
//					PrintStr(ApplVar.SalPer.Name);
			}
#endif
		    continue;
		}
	    else if (ApplVar.RGRec.Key.Code < CLERK + 1000) /* clerk ? */
		{
			ApplVar.ClerkNumber = ApplVar.RGRec.Key.Code - CLERK;
		    ReadClerk();
			if (TRAINING)   /* training clerk ? */
				ApplVar.FTrain = 1;
			else
				ApplVar.FTrain = 0;
		    continue;
		}
		}
		if (lastrefund == 2)
			lastrefund = 1;
		else
			lastrefund = 0;
	}
    ApplVar.RGRec.Key.Code = 0;     /* reset record to avoid doubling */
}

/* process transaction buffer according to BufXXX global flags */
/* types can be combined */
/* if ApplVar.BufCC then add customer count at end of transaction */
/* if ApplVar.BufRec then second receipt(复制收据) */
/* if ApplVar.BufKp then sent ApplVar.KP */
/* if BufVoid then TransAction void */

void ProcessBuffer()
{
    BYTE arrows, kperr;
    WORD saveclerk,savesalper;
    int i;

    ApplVar.RGRec.Key.Code = 0;     /* reset last record to avoid doubling */
    if (ApplVar.FProforma)
		ApplVar.CopyReceipt = ApplVar.RGItems.RGNumber = 0;    /* reset trans buffer */
	ApplVar.FProforma = 0;
    ApplVar.OldClerk = 0;     /* force clerk print start transaction */
	ApplVar.OldSalPer = 0;
    if (!ApplVar.RGItems.RGNumber)       /* not used */
    {
		ApplVar.FKpGroup = 0;
		ApplVar.BufCC = 0;
		return;
    }
    ApplVar.FBuffer = 0;         /* reset buffer full */
    saveclerk = ApplVar.ClerkNumber;
	savesalper = ApplVar.SalPerNumber;
    arrows = ARROWS;
    SETBIT(ARROWS, BIT0);   /* set on receipt for ApplVar.KP & second  */
    if (ApplVar.BufRec)    /* second receipt ? 为复制收据 */
    {
		ApplVar.SlipLines = 0;
		ApplVar.PrintLayOut = 0;
		ApplVar.SubTotal = ZERO;
		if (TESTBIT(SECOND, BIT1))   /* slip? */
		{
		    SETBIT(ApplVar.PrintLayOut, BIT3);
		    if (!TESTBIT(SECOND, BIT0))   /* no receipt then compulsory? */
		       SETBIT(ApplVar.PrintLayOut, BIT4);
		    ApplVar.SlipPage = 0;
		    if (CheckSlip())        /* check if slip paper there */
		    {
				ApplVar.BufRec = 0;
				return;
		    }
		}
		if (!ApplVar.SlipLines && TESTBIT(SECOND, BIT0))   /* receipt? */
		{
		    SETBIT(ApplVar.PrintLayOut, BIT1);
		    PrintHead1(PREHEADER);
		}
		if (!ApplVar.PrintLayOut)
		{
		    ApplVar.BufRec = 0;
		    return;
		}
		ApplVar.BufRec = ApplVar.PrintLayOut | 0x24;
		PrintMessage(FZHSHJU);
#if defined(CASE_PERU)//ccr2016-10-24>>>>>>>>>>>>>>>
        if (ApplVar.CustomerMessage.CopyOfMess)
        {
            ApplVar.CustomerMessage.Messages[0][0]=ApplVar.CustomerMessage.CopyOfMess;
            PrintCustomerMessage();
        }
#endif//ccr2016-10-24<<<<<<<<<<<<<<<<<
	    PrintStr(Msg[TITLE].str);
//20130709分割线没有多大的意义，客户反馈也浪费纸	   	PrintLine('=');
//		if (TESTBIT(ApplVar.BufRec, BIT1))  /* second receipt ? */
//		    RFeed(1);
    }
    kperr = 0;
    if (ApplVar.FCanc)
		ApplVar.BufKp = 0;
    if (ApplVar.BufKp == 1)
    {
		ApplVar.BufKp = 2;      /* set to 2 so no feed on ApplVar.KP receiptissue */
		ApplVar.KP.Used = 0;       /* see if kp used */
		ApplVar.KP.Error = 0;
		ApplVar.KP.Number = 0;
		ApplVar.KP.Next = 0;
		ApplVar.KP.Prt = (struct PRINTER *) &KPSTART;
		while(!(ApplVar.KP.Prt->type & 0x0f) && ApplVar.KP.Number < 8)
		{//select one Kitchen Printer
		    ApplVar.KP.Prt++;
		    ApplVar.KP.Number++;
		}
		if (ApplVar.KP.Number > 7)
		    ApplVar.BufKp = 0;
    }
    for(;;)
    {
		if (ApplVar.BufKp)
		{
		    ApplVar.KP.Prt = (struct PRINTER *) &KPSTART;
		    ApplVar.KP.Current = 0x01 << ApplVar.KP.Number;
		    if (kperr)
		    {
				while(ApplVar.KP.Number < 8)
				{
				    if (kperr & ApplVar.KP.Current)
						break;
				    ApplVar.KP.Number++;
				    ApplVar.KP.Current <<= 1;
				}
				if (ApplVar.KP.Number > 7)
				    break;
				ApplVar.KP.Prt += (ApplVar.KP.Prt + ApplVar.KP.Number)->alter;
		    }
		    else
				ApplVar.KP.Prt += ApplVar.KP.Number;
		    ApplVar.SubTotal = ZERO;    /* Clear Subtotal TALON bon  */
		}
		ProBuf();
		ApplVar.BufCC = 0;                  /* reset buffer flags */
		if (!ApplVar.BufKp)
		    break;
		if (ApplVar.KP.Used & ApplVar.KP.Current)
		{
		    if (!(ApplVar.KP.Error & ApplVar.KP.Current))    /* ApplVar.KP ? */
		    {
				if(!(ApplVar.KP.Prt->type & 0x80) || ApplVar.FKpGroup)  /* no single tickets */
				    KpEnd(1);
				if (ApplVar.KP.Prt->type & 0x20)        /* print TALON bon ? */
				{//在厨房打印机上打印汇总数据
				    ApplVar.PrintLayOut = 0x02;     /* for ApplVar.KP */
				    if (TESTBIT(KPPRICE, BIT1))
				    {
						//ccr2014-04-01 if (ApplVar.PbNumber || Msg[CWXXI19].str[0])
						{
						    PrintPbKp(1);
						    MemSet(SysBuf, sizeof(SysBuf), '=');
						    PrintKp(KP_NORMAL, SysBuf);
						}
				    }
//				    MemSet(SysBuf, sizeof(SysBuf), ' ');
//				    memcpy(SysBuf+4, ApplVar.TXT.Message[58], sizeof(ApplVar.TXT.Message[0]));
//				    SysBuf[20] = 0;
//				    PrintKp(KP_NORMAL, SysBuf);
				    if ((ApplVar.KP.Prt->type & 0x0f) == 1)
						PrintAmt(Msg[KPTOTAL].str, &ApplVar.SubTotal);
				    else
				    {
						MemSet(SysBuf, sizeof(SysBuf), ' ');
						CopyFrStr(SysBuf+10, Msg[KPTOTAL].str);
						FormatAmt(SysBuf + 29, &ApplVar.SubTotal);
						PrintKp(KP_NORMAL, SysBuf + 10);
				    }
				    KpEnd(1);
				}
		    }
		}
		ApplVar.KP.Number++;
		if (kperr)
		    ApplVar.KP.Next &= kperr;
		ApplVar.KP.Next &= (0xff << ApplVar.KP.Number);
		if (!ApplVar.KP.Next)
		{
		    if (kperr)      /* already alternative */
				break;
		    if (!ApplVar.KP.Error)
				break;
		    kperr = ApplVar.KP.Error;
		    ApplVar.KP.Current = 0x01;
		    ApplVar.KP.Number = 1;
		    ApplVar.PrintLayOut = 0x01;
		    PrintMessage(CWXXI37);
		    memset(ProgLineMes,' ',sizeof(ProgLineMes));
		    do
		    {
				if (ApplVar.KP.Error & ApplVar.KP.Current)
				{
                    i=strlen(Msg[CHFDYJHAO].str);
					memcpy(ProgLineMes+4,Msg[CHFDYJHAO].str,i);
					WORDtoASCL(ProgLineMes+i+4,ApplVar.KP.Number);
					ProgLineMes[i+4+2]= 0;
				    PrintRJ(ProgLineMes);
				}
				ApplVar.KP.Number++;
				ApplVar.KP.Current <<= 1;
		    } while(ApplVar.KP.Current);
		    ApplVar.KP.Number = 0;
		    ApplVar.KP.Error = 0;
		    ApplVar.KP.Used = 0;
		}
    }
    if (kperr)
		ApplVar.ErrorNumber=ERROR_ID(CWXXI37);
    if (ApplVar.BufRec)    /* second receipt ? */
    {
		SetPrintLayOut(0x0b);
		if (PVAT)
				CalculateTax(3);
		PrintSaleQty();
		ReceiptIssue(1);
		ApplVar.CopyReceipt = ApplVar.RGItems.RGNumber = 0;    /* reset trans buffer */
		ApplVar.FPb = 0;
		ApplVar.PbNumber = 0;
    }
    ARROWS = arrows;    /* restore arrows */
    ApplVar.FReceipt = 0;
    ApplVar.MultiplyCount = 0;
    ApplVar.BufKp = 0;
    ApplVar.FKpGroup = 0;
    ApplVar.BufRec = 0;
    if (ApplVar.FCanc)
    {
		ApplVar.FCanc = 0;
		ApplVar.CopyReceipt = ApplVar.RGItems.RGNumber = 0;    /* reset trans buffer */
    }
    if (TESTBIT(CLERKFIX, BIT0) && !ApplVar.ClerkLock)    /* clerk compulsory, no key */
    {
		ApplVar.ClerkNumber = 0;
		ApplVar.FTrain = 0;
    }
    else
    {
		ApplVar.ClerkNumber = saveclerk;
		ReadClerk();
		if (TRAINING)   /* training clerk ? */
		    ApplVar.FTrain = 1;
		else
		    ApplVar.FTrain = 0;
    }
#if (salNumber)
    if (TESTBIT(KEYTONE, BIT6))    /* salesperson compulsory */
		ApplVar.SalPerNumber = 0;
	else
	{
		if (TESTBIT(AM_PM, BIT4))    /* salesperson to 1 */
			ApplVar.SalPerNumber = 1;
	    else
		    ApplVar.SalPerNumber = savesalper;
    	ReadSalPer();
    }
#endif
}

/* store in buffer according to ApplVar.BufCmd */
/* if ApplVar.BufCmd = 0 just store no consolidation不合并 */
/* if ApplVar.BufCmd = 1 Store with consolidation合并 */
/* if ApplVar.BufCmd = 2 Check if used don't store */
/* if ApplVar.BufCmd = 3 then check PLU Mix/Match */

void StoreInBuffer()
{
    BCD TmpQty;
    BYTE i, savenum;
    short j;

    if (!ApplVar.RGRec.Key.Code || ApplVar.FProforma)
    {//没有需要保存的项目信息
		if(ApplVar.BufCmd)
		    ApplVar.BufCmd = 1 ;
		return;
    }
    if (ApplVar.BufCmd == 2)
		TmpQty = ZERO;       //==========================????????????????????????
	if (ApplVar.BufCmd == 1 && TESTBIT(AM_PM, BIT5))
		ApplVar.BufCmd = 0;             /* don't consolidate */
    if (ApplVar.BufCmd)
    {
		for(i = 0; i < ApplVar.RGItems.RGNumber; i++)
		{
		    if (ApplVar.RGItems.RGBuf[i].Key.Code == ApplVar.RGRec.Key.Code /* same ? */
				&& ApplVar.RGRec.Key.Code!=SUB)    /* ccr20130425+SUB,由于增加了把SUB操作存入RGBuf中,SUB不能合并 */
		    {
				if (ApplVar.RGRec.Key.Code > DEPT)  /* dept or plu ? */
				{
				    if (CompareBCD(&ApplVar.RGItems.RGBuf[i].Amt, &ApplVar.RGRec.Amt))
						continue;/* 价格不相同,不能合并 */
				    if (!CheckNotZero(&ApplVar.RGItems.RGBuf[i].Qty))
						continue;   /* 数量为0,不能合并 */
				    if (i)              /* not first entry */
					{
						if (ApplVar.RGItems.RGBuf[i - 1].Key.Code > CORREC &&
								ApplVar.RGItems.RGBuf[i - 1].Key.Code < CORREC+100)
						{/* 前一个项目为修改销售的功能码 */
							savenum = ApplVar.CorrecNumber;
							ApplVar.CorrecNumber = ApplVar.RGItems.RGBuf[i - 1].Key.Code - CORREC - 1;
							ReadCorrec();
							ApplVar.CorrecNumber = savenum;
							if ((ApplVar.Correc.Options & 0x07) == 2) /* refund */
							{
								ReadCorrec();
								continue;                               /* no consolidation */
							}
							ReadCorrec();
						}
					}
					j = CompareBCD(&ApplVar.RGItems.RGBuf[i].Qty, &ApplVar.RGRec.Qty);
				    if (ApplVar.BufCmd == 2)  /* correction */
				    {
						if (((ApplVar.RGRec.Qty.Sign & 0x03) ||
						    (ApplVar.RGItems.RGBuf[i].Qty.Sign & 0x03)) && j)  /* decimal point ? */
						    continue;
						else if (j < 0)
						    continue;
						Add(&TmpQty, &ApplVar.RGItems.RGBuf[i].Qty);    /* add temp qty */
						continue;                       /* for check if used */
				    }
				    else if ((ApplVar.RGRec.Qty.Sign & 0x03) || (ApplVar.RGItems.RGBuf[i].Qty.Sign & 0x03))
				    {
						if ((ApplVar.RGRec.Qty.Sign ^ ApplVar.RGItems.RGBuf[i].Qty.Sign) < 0x80)
						    continue; /* opposite sign and same qty ? */
						if (j)
						    continue;
				    }
				    if (i < (ApplVar.RGItems.RGNumber - 1))
				    {
						if (ApplVar.RGItems.RGBuf[i + 1].Key.Code > MODI &&
						    ApplVar.RGItems.RGBuf[i + 1].Key.Code < MODI + 1000)
						{
						    if (ApplVar.RGItems.RGBuf[i + 1].Key.Code != MODI + ApplVar.ModiCount)
							continue;
						    if (CompareBCDValue(&ApplVar.RGItems.RGBuf[i + 1].Qty, ApplVar.ModNum, ApplVar.ModiCount*sizeof(ApplVar.ModNum[0])))
							continue; /* not the same */
						}
						else if (ApplVar.ModiCount)
						    continue;
				    }
				    else if (ApplVar.ModiCount)
						continue;
				    ApplVar.OldModiCount = ApplVar.ModiCount;
				    ApplVar.ModiCount = 0;
				    if (ApplVar.BufCmd == 3)        /* Mix/Match Check */
						ApplVar.MixQty = ApplVar.RGItems.RGBuf[i].Qty;
				    else
                    {
						Add(&ApplVar.RGItems.RGBuf[i].Qty, &ApplVar.RGRec.Qty);/* 合并单品或部类的数量 */
						ApplVar.RGRec.DiscAmt.Sign = (ApplVar.RGRec.Qty.Sign);		/* cc 2006-10-24 for SubDisc */
//						Multiply(&ApplVar.RGRec.DiscAmt,&ApplVar.RGRec.Qty);		/* cc 2007-02-13 */
						Add(&ApplVar.RGItems.RGBuf[i].DiscAmt, &ApplVar.RGRec.DiscAmt);	/* cc 2006-10-24 for SubDisc */
                    }
				    return;
				}
				else if ((ApplVar.RGRec.Key.Code > CORREC && ApplVar.RGRec.Key.Code < CORREC+100)  /* Corrections? */
						|| (ApplVar.RGRec.Key.Code > PORA && ApplVar.RGRec.Key.Code < PORA+100))   /* Pora ? */
				{
					if(ApplVar.BufCmd == 2)         /* Correction Check !*/
						continue;
					else
					{
						Add(&ApplVar.RGItems.RGBuf[i].Qty, &ApplVar.RGRec.Qty);
						Add(&ApplVar.RGItems.RGBuf[i].Amt, &ApplVar.RGRec.Amt);
					}
					return;
				}
				else if (CompareBCD(&ApplVar.RGItems.RGBuf[i].Qty, &ApplVar.RGRec.Qty))
				    continue;    /* not the same function */
				else
				{
				    if (ApplVar.BufCmd == 2)
				    {
						if (CompareBCD(&ApplVar.RGItems.RGBuf[i].Amt, &ApplVar.RGRec.Amt) > -1)   /*  */
						    TmpQty = ONE;
						continue;
				    }
				    Add(&ApplVar.RGItems.RGBuf[i].Amt, &ApplVar.RGRec.Amt);
				    return;
				}
		    }
		}
    }
    if (ApplVar.BufCmd == 3)
    {/* not found */
		return;         /* only check if used */
    }
    if (ApplVar.BufCmd == 2)       /* correction ?*/
    {
		if (!CheckNotZero(&TmpQty))    /* total is zero then no correction */
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI16);
		ApplVar.BufCmd = 1;
		return;
    }
	/* 存储销售项目 */
    ApplVar.RGItems.RGBuf[ApplVar.RGItems.RGNumber] = ApplVar.RGRec;
    ApplVar.RGItems.RGNumber++;
    if (ApplVar.RGRec.Key.Code > DEPT && ApplVar.ModiCount)
    {/* 存储销售项目的口味信息 */
		ApplVar.RGItems.RGBuf[ApplVar.RGItems.RGNumber].Key.Code = MODI + ApplVar.ModiCount; /* store number of modi */
		memcpy(&ApplVar.RGItems.RGBuf[ApplVar.RGItems.RGNumber].Qty, ApplVar.ModNum, sizeof(ApplVar.ModNum));
		ApplVar.RGItems.RGNumber++;
		ApplVar.OldModiCount = ApplVar.ModiCount;
		ApplVar.ModiCount = 0;
		memset(ApplVar.ModNum, 0, sizeof(ApplVar.ModNum));
    }
    if (ApplVar.RGItems.RGNumber > (TRANSITEMS - 5))
		ApplVar.FBuffer = 1;/* 存储区满 */
	Appl_DispRGBuf.RGCount = ApplVar.RGItems.RGNumber;
	Appl_DispRGBuf.FirstRGNum = ApplVar.RGItems.RGNumber;
	Appl_DispRGBuf.LastRGNum = Appl_DispRGBuf.FirstRGNum;
    ApplVar.BufCmd = 1;
    return;
}

