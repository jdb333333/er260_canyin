#define TAX_C 5
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"

#define FOR_CANADA  0   //ccr2014-08-25 当销售金额<=4.00时,不计税.

void GetTaxOffSet()
{
    RamOffSet = ApplVar.TaxNumber * ApplVar.AP.Tax.RecordSize + ApplVar.AP.StartAddress[AddrTax];
}

void AddTaxTotal()
{

    GetTaxOffSet();
    RamOffSet += ApplVar.AP.Tax.TotalOffSet;
    for (ApplVar.PointerType = 0; ApplVar.PointerType < REPDEFMAX; ApplVar.PointerType++)
    {
	    ApplVar.Size = ApplVar.AP.Tax.Size[ApplVar.PointerType];
        AddPointerTotal();
    }
}

void WriteTax()
{
    if (ApplVar.TaxNumber < ApplVar.AP.Tax.Number)
    {
	    GetTaxOffSet();
	    WriteRam(ApplVar.Tax.Name, ApplVar.AP.Tax.CapSize);
	    WriteRam(ApplVar.Tax.Rate, sizeof(ApplVar.Tax.Rate));
	    WriteRam(&ApplVar.Tax.Options, sizeof(ApplVar.Tax.Options));
    }
}
//Read ApplVar.Tax from area of TAX_C
void ReadTax()
{
    GetTaxOffSet();

    ReadRam(ApplVar.Tax.Name, ApplVar.AP.Tax.CapSize);
	ApplVar.Tax.Name[ApplVar.AP.Tax.CapSize]=0;
    ReadRam(ApplVar.Tax.Rate, sizeof(ApplVar.Tax.Rate));
    ReadRam(&ApplVar.Tax.Options, sizeof(ApplVar.Tax.Options));
}

void AddTaxItem(BYTE tax)
{
    BYTE t, i;

    t = 1;
    tax ^= ApplVar.TaxPerm;	/* shift tax 1 & 2 if needed */
    tax ^= ApplVar.TaxTemp;

    for (i = 0; i < 8; i++)
    {
	    if (tax & t)
	    {
	        Add(&ApplVar.TaxItem[i], &ApplVar.Amt);
	    }
	    t <<= 1;
    }
    Add(&ApplVar.SaleAmt, &ApplVar.Amt);
    Add(&ApplVar.SaleQty, &ApplVar.Qty);
    ApplVar.TaxTemp1 = ApplVar.TaxTemp;     /* save article tax shift */
    ApplVar.TaxTemp = 0;	    /* reset article tax shift */
}
//type=0:Total sales rounding;
//type=1:Rounding on MULT, TAX & Percentage discount

void AmtRound(BYTE type ,BCD *amt)
{
    BYTE b, sign;

    if ((!type && TESTBIT(ROUNDMASK, BIT0+BIT1+BIT2+BIT3)) ||
    	(type && TESTBIT(ROUNDMASK, BIT4+BIT5+BIT6+BIT7)))   /* Detail rounding?*/
	{

	    sign = amt->Sign;	/* save sign */
	    amt->Sign = 0;
	    b = ROUNDMASK & 0x0f;
	    if (!b)
	    	b = (ROUNDMASK>>4) & 0x0f;
	    switch (b){
	    case 1:
	    	if ((amt->Value[0] & 0x0f)>=0x05)
	    		Add(amt,(BCD*)&TEN);
	    	amt->Value[0] &= 0xf0;
	    	break;
	    case 2:
	    	b = amt->Value[0];
	    	amt->Value[0] = 0;		/* 00 - 24 -->0 */
            if (b > 0x74)			/* 75 - 99  --> 100 */
	            Add(amt, (BCD*)&HUNDRED);
            else if (b > 0x24)		/* 25 - 74 --> 50 */
                amt->Value[0] = 0x50;
	    	break;
	    case 3:
	    	b = amt->Value[0] & 0xf0;
	    	amt->Value[0] = 0;		/* 00 - 49 -->0 */
            if (b >= 0x50)
	            Add(amt, (BCD*)&HUNDRED);/* 50 - 99  --> 100 */
	    	break;
	    case 4:
	    	b = amt->Value[0] & 0xf0;
	    	amt->Value[0] = 0;		/* 00 - 49 -->0 */
            if (b > 0x00)
	            Add(amt, (BCD*)&HUNDRED);/* 50 - 99  --> 100 */
	    	break;
		}
	    amt->Sign = sign;	    /* restore sign */
	}
}

void RoundTaxable(BCD *amt)
{
	RoundBcd(amt, 0);
	AmtRound(1, amt);
}

/* if type = 0 then add in report and calculate add on tax */
/* if type = 1 then not add in report and calculate add on tax */
/* if type = 2 then calculate and print add on tax not report */
/* if type = 3 then print inclusive tax */
/* GST is a special tax which is always ADD ON and calculated as */
/* a percentage on (SalesAmt + TAX1 + TAX2 ..)	*/

#if FOR_CANADA
BCD BCD_4={0,{0,0x04,0,0,0,0,0,0}};
#endif

void CalculateTax(BYTE type )
{
    int len;
    BYTE printsub;
    BYTE calcgst;
    BCD gst,sVal1;//ccr20130922
#if FOR_CANADA
    BYTE sCAxTAX,sCAPrint;//加拿大<4.00不计税,但是打印
#endif
#if (defined(CASE_PERU)) //ccr2016-10-24>>>>>>>>>>>>>>
    BYTE   printline=0;
#endif
    printsub = 0;
    calcgst = 0;    /* checkif tax used for GST (tax on total sales amount)*/
	if (!type)
	    AmtRound(0, &ApplVar.SaleAmt);	/* sales amount rounding */

    ApplVar.SubTotal = ApplVar.SaleAmt;
#if FOR_CANADA
	if (CompareBCD(&ApplVar.SubTotal, &BCD_4) < 0)//为加拿大,销售金额<4.00时不计税
        sCAxTAX = 1;//		MemSet(ApplVar.TaxItem, sizeof(ApplVar.TaxItem), 0);//去掉计税统计
    else
        sCAxTAX = 0;
#endif
    if (ApplVar.AP.Tax.Number && !ApplVar.FNoTax)   /* if ApplVar.FNoTax == 1 then no tax */
    {
	    for(;;)
	    {
	        for (ApplVar.TaxNumber = 0; ApplVar.TaxNumber < ApplVar.AP.Tax.Number; ApplVar.TaxNumber++)
	        {
		        ReadTax();
		        if (TESTBIT(ApplVar.Tax.Options, BIT3))        /* GST tax ? */
		        {
		            if (calcgst == 2)
		            {
			            ApplVar.Amt = gst;		 /* Taxable is Saleamt + ApplVar.Tax */
			            ApplVar.TaxItem[ApplVar.TaxNumber] = ApplVar.Amt;   /* used for external invoice */
		            }
		            else
		            {
			            calcgst = 1;		/* set GST active */
			            continue;
		            }
		        }//if
		        else if (calcgst == 2)
	                continue;
	            else
	                ApplVar.Amt = ApplVar.TaxItem[ApplVar.TaxNumber];/* 读取税种销售金额累计 */
		        ApplVar.Cost = ZERO;	    /* clear calculated tax */
		        if (CheckNotZero(&ApplVar.Amt))
		        {
#if FOR_CANADA
                    if (sCAxTAX)//销售金额<4,不计税
                        ApplVar.Amt = ZERO;
                    sCAPrint = 1;//但是打印
#endif
		            ApplVar.Qty = ZERO;
		            memcpy(ApplVar.Qty.Value, ApplVar.Tax.Rate, sizeof(ApplVar.Tax.Rate));
		            if (!TESTBIT(ApplVar.Tax.Options, BIT0)) /* add on ? */
		            {
			            ApplVar.Cost = ApplVar.Amt;
			            ApplVar.Qty.Sign = 0x04;
			            Multiply(&ApplVar.Cost, &ApplVar.Qty);
			            RoundBcd(&ApplVar.Cost, 0);
			            AmtRound(1, &ApplVar.Cost);
			            Add(&ApplVar.SubTotal, &ApplVar.Cost);
		            }
		            else
		            {
//			            if (type == 3 )//ccr2015-08-19|| type == 2)//ccr20130922  if (type == 3)
			            if (type == 3 || type == 2)//ccr20130922  if (type == 3)
			            {
			                ApplVar.Cost = ApplVar.Amt; /* contains TAX */
							if (ApplVar.Qty.Value[0] == 0x99 &&
								ApplVar.Qty.Value[1] == 0x99 &&
								ApplVar.Qty.Value[2] == 0x99)
								ApplVar.DiscAmt = ZERO; 	/* NET is zero */
							else
							{
								ApplVar.DiscAmt = ApplVar.Amt;	/* contains NET */
				                Add(&ApplVar.Qty, (BCD*)&THOUSAND10);
				                ApplVar.Qty.Sign = 0x04;
				                Divide(&ApplVar.DiscAmt, &ApplVar.Qty);	/* calculate net */
								RoundTaxable(&ApplVar.DiscAmt);       //税额
				                Subtract(&ApplVar.Cost, &ApplVar.DiscAmt);	/* taxable - net =净额 */
				                Subtract(&gst, &ApplVar.Cost);	/* calculate net for */
				    /* add on tax for Argentina */
							}
			            }
		            }
		            if (!type || type == 4)		/* add report */
			            AddTaxTotal();
		        }//if
#if FOR_CANADA
                else
                    sCAPrint = 0;
#endif

		        if (type == 2)
		        {
                    if (TESTBIT(ApplVar.Tax.Options,BIT1))//ccr20130922 if (!TESTBIT(ApplVar.Tax.Options, BIT0) && TESTBIT(ApplVar.Tax.Options,BIT1))
		            {			      /* ADD ON and print */
			            if (CheckNotZero(&ApplVar.Cost)
#if FOR_CANADA
                            || sCAPrint
#endif
                            || TESTBIT(ApplVar.Tax.Options, BIT2))/* print also zero */
			            {
			                if (!printsub)
			                {
#if !defined(CASE_PERU)//ccr2017-01-12秘鲁客户要求不打印小计
				                if (calcgst == 2)   /* GST subtotal ?*/
				                    PrintAmt(Msg[XIAOJI].str, &gst);
				                else
				                    PrintAmt(Msg[XIAOJI].str, &ApplVar.SaleAmt);
#endif
				                printsub = 1;
			                }
//ccr20130922>>>>>>>>>>>>

                            Collect_Data(TAXLOG);//ccr2014-12-30,计税非0时才记录流水
#if (defined(CASE_PERU)) //ccr2016-10-24>>>>>>>>>>>>>>
                            if (!printline)
                            {
                                printline=1;
                                PrintLine('.');
                            }
#endif //ccr2016-10-24<<<<<<<<<<<<
			                if (TESTBIT(ApplVar.Tax.Options, BIT4))	   /* print taxable? */
                            {
                                strcpy(ProgLineMes,ApplVar.Tax.Name);
                                len = strlen(ProgLineMes);
                                ProgLineMes[len++] = ' ';
                                strcpy(ProgLineMes+len,Msg[XSHOUE].str);
                                if (!TESTBIT(ApplVar.Tax.Options, BIT0)) /*ccr20130927 add on ? */
    				                PrintAmt(ProgLineMes, &ApplVar.Amt);/* 打印税种的销售金额 */
                                else
    				                PrintAmt(ProgLineMes, &ApplVar.DiscAmt);/* 打印税种的销售净额 */
                            }
                            memset(ProgLineMes, ' ', sizeof(ProgLineMes));
                            sVal1 = ZERO;
                            CWORD(sVal1.Value[0]) = CWORD(ApplVar.Tax.Rate[0]);
                            CopyFrStr(ProgLineMes,ApplVar.Tax.Name);
                            sVal1.Sign = 2;
                            FormatQty(ProgLineMes+16,&sVal1);//cr070424
                            ProgLineMes[17] = '%';      //ProgLineMes[13] = '%';
                            ProgLineMes[18] = 0;        //ccr090113
                            PrintAmt(ProgLineMes,&ApplVar.Cost);/* 打印计税金额 */
//ccr20130922			                PrintAmt(ApplVar.Tax.Name, &ApplVar.Cost);/* 打印计税金额 */
//ccr20130922<<<<<<<<<<<<

			            }
		            }
                    /* 非ADD ON 税种不打印 ?*/
		        }
		        else if (type == 3)
	            {
	                if (TESTBIT(ApplVar.Tax.Options, BIT0) && TESTBIT(ApplVar.Tax.Options, BIT1))		/* print incl tax ? */
				    {
					    if (CheckNotZero(&ApplVar.Amt)
#if FOR_CANADA
                            || sCAPrint
#endif
                            || TESTBIT(ApplVar.Tax.Options, BIT2))/* print also zero */
					    {
						    PrintAmt(ApplVar.Tax.Name, &ApplVar.Cost);
						    if (TESTBIT(ApplVar.Tax.Options, BIT4)) 	/* print taxable ? */
			                    PrintAmt(Msg[XSHOUE].str, &ApplVar.DiscAmt);
					    }
	                }
	                ApplVar.Cost = ZERO;
	            }
            }//for
	        if (calcgst == 1)	    /* gst active ? */
	        {
		        AmtRound(0, &ApplVar.SubTotal);     /* sales amount rounding */
		        gst = ApplVar.SubTotal;
		        calcgst = 2;
		        printsub = 0;
	        }
	        else
		        break;
	    }//for /* also calculate gst */
    }//if
	if (!ApplVar.FPb)
	    AmtRound(0, &ApplVar.SubTotal);	/* sales amount rounding */
}


//cc 2006-10-28 for SubDisc>>>>>>>>>>>
void CalculateTax2()
{
	BCD d1,d2,EntryVal;
	BYTE i;

	if (!TESTBIT(ApplVar.Disc.Options, BIT2))    /* percentage ? */
	{
		EntryVal = ApplVar.Entry;
		Multiply(&EntryVal,(BCD*)&THOUSAND100);		/* cc 2006-10-24 for SubDisc */
		for (i = 0; i < 8; i++)
		{
//			if (ApplVar.Disc.Tax & (0x01 << i))
			if(ApplVar.Dept.Tax & (0x01 << i))
			{
				d1 = ApplVar.TaxItem[i];
				Multiply(&d1, &EntryVal);
				Divide(&d1,(BCD*)&MILLION10);	/* cc 2006-10-24 for SubDisc */
				RoundBcd(&d1, 0);
				AmtRound(1, &d1);
				if (!TESTBIT(ApplVar.Disc.Options, BIT3))    /* Positive or negative */
					d1.Sign ^= 0x80;
				if (ApplVar.FVoid)
					d1.Sign ^= 0x80;
				Add(&ApplVar.TaxItem[i], &d1);
			}
		}
	}
	else
	{
//		EntryVal = ApplVar.Amt;
//		d1 = ZERO;
		d1 = ApplVar.SUB_DiscTot;
//		for (i = 0; i < 8; i++)
//		{
//			if (ApplVar.Disc.Tax & (0x01 << i))
//			if (SalesTax & (0x01 << i))
//				Add(&d1, &ApplVar.TaxItem[i]);
//		}
		if (CheckNotZero(&d1))     /* active ? */
		{
			for (i = 0; i < 8; i++)
			{
//				if ((ApplVar.Disc.Tax & (0x01 << i)) && CheckNotZero(&ApplVar.TaxItem[i]))
				if ((ApplVar.Dept.Tax & (0x01 << i)) && CheckNotZero(&ApplVar.TaxItem[i]))
				{
					if (CheckNotZero(&ApplVar.TaxItem[i]))
					{
						d2 = ApplVar.TaxItem[i];
						Multiply(&d2,(BCD*)&MILLION10);	/* cc 2006-10-24 for SubDisc */
						Divide(&d2, &d1);
						Multiply(&d2, &ApplVar.Amt);
						Divide(&d2,(BCD*)&MILLION10);	/* cc 2006-10-24 for SubDisc */
						RoundBcd(&d2, 0);
						d2.Sign &= 0xfc;
						AmtRound(1, &d2);
						Add(&ApplVar.TaxItem[i], &d2);  /* sign is already oke */
					}
				}
			}
		}
	}
}
//cc 2006-10-28 for SubDisc<<<<<<<<<<<<<<
