#define PBBUFFER_C 2
#include "king.h"                               /* Kingtron library and I/O routines */
#include "exthead.h"
#include "exth.h"

/****************** Define used Global variables ***********************/

extern WORD GetLinkNumber();
/***************************************************************/

BYTE UpdatePbRecord(BYTE *record, BYTE fnew)
{
    if(!fnew)        /* consolidate record ? */
    {
		if (!ApplVar.AP.Pb.ArtAmt)
		{
		    if (ApplVar.PBA.Flag && !TESTBIT(*(record + 2), BIT5)) /* amount stored ? */
			return 1;
		    if (!ApplVar.PBA.Flag && TESTBIT(*(record + 2), BIT5)) /* amount stored ? */
			return 1;
		}
		if (ApplVar.PBA.Rec.Key.Code < DEPT || ApplVar.PBA.Flag)
		{
		    if (ApplVar.PBA.Flag)
				memcpy(&ApplVar.Total.Amt, record + 2, ApplVar.AP.Pb.Qty + 1);
		    else
		    {
				memcpy(ApplVar.Total.Amt.Value, record + 3 + ApplVar.AP.Pb.Qty, ApplVar.AP.Pb.ArtAmt);
				ApplVar.Total.Amt.Sign = 0;
				if (TESTBIT(*(record + 2), BIT6))
				    SETBIT(ApplVar.Total.Amt.Sign, BIT7);
		    }
		    if (ApplVar.PbSplit)
		    {
				if (CompareBCD(&ApplVar.Total.Amt, &ApplVar.PBA.Total) < 0)
				    return 1;   /* must be smaller or he same */
				if (ApplVar.PbSplit == 1)     /* split bill */
				    Subtract(&ApplVar.Total.Amt, &ApplVar.PBA.Total);
				else                    /* refund */
				    Add(&ApplVar.Total.Amt, &ApplVar.PBA.Total);
		    }
		    else
				Add(&ApplVar.Total.Amt, &ApplVar.PBA.Total);    /* PORA or DISC */
		    if (ApplVar.PBA.Flag)
				memcpy(record + 2, &ApplVar.Total.Amt, ApplVar.AP.Pb.Qty + 1);
		    else
		    {
				memcpy(record + 3 + ApplVar.AP.Pb.Qty, ApplVar.Total.Amt.Value, ApplVar.AP.Pb.ArtAmt);
				if (TESTBIT(ApplVar.Total.Amt.Sign, BIT7))
				    SETBIT(*(record + 2) , BIT6);
		    }
		    SETBIT(*(record + 2), BIT5);  /* set amount */
		}
		else
		{
		    if (!ApplVar.PbSplit && ((ApplVar.PBA.Rec.Qty.Sign & 0x03) ||   /* don't consolidate decimal point */
				(*(record + 2) & 0x03)))
				return 1;
		    if (ApplVar.AP.Pb.ArtAmt)
		    {
				memcpy(ApplVar.Total.Amt.Value, record + 3 + ApplVar.AP.Pb.Qty, ApplVar.AP.Pb.ArtAmt);
				if (CompareBCD(&ApplVar.Total.Amt, &ApplVar.PBA.Rec.Amt))
				    return 1;       /* not same price */
		    }
		    memcpy(&ApplVar.Total.Qty, record + 2, ApplVar.AP.Pb.Qty + 1);
		    if (ApplVar.PbSplit)
		    {
				if ((ApplVar.PBA.Rec.Qty.Sign & 0x03) || (*(record + 2) & 0x03))
				{      /* decimal point ? */
				    if (CompareBCD(&ApplVar.Total.Qty, &ApplVar.PBA.Rec.Qty))
					return 1;   /* must be same */
				}
				else if (CompareBCD(&ApplVar.Total.Qty, &ApplVar.PBA.Rec.Qty) < 0)
				    return 1;       /* ApplVar.Qty to large ? */
				if (ApplVar.PbSplit == 1)   /* split bill */
				    Subtract(&ApplVar.Total.Qty, &ApplVar.PBA.Rec.Qty);
				else    /* refund */
				    Add(&ApplVar.Total.Qty, &ApplVar.PBA.Rec.Qty);
		    }
		    else
				Add(&ApplVar.Total.Qty, &ApplVar.PBA.Rec.Qty);

		    if (TESTBIT(*(record + 2), BIT6))
				SETBIT(ApplVar.Total.Qty.Sign , BIT6);
		    memcpy(record + 2, &ApplVar.Total.Qty, ApplVar.AP.Pb.Qty + 1);
		}
    }
    else
    {
		*((WORD *) record) = ApplVar.PBA.Rec.Key.Code;
		record += 2;
		if (ApplVar.PBA.Flag)       /* plu with other price */
		{
		    memcpy(record, &ApplVar.PBA.Total, ApplVar.AP.Pb.Qty + 1);    /* total amt + sign */
		    SETBIT(*record, BIT5);  /* set amount */
		    record += ApplVar.AP.Pb.Qty + 1;
		}
		else
		{
		    memcpy(record, &ApplVar.PBA.Rec.Qty, ApplVar.AP.Pb.Qty + 1);    /* qty + sign */
		    if (TESTBIT(ApplVar.PBA.Rec.Amt.Sign, BIT7))
			SETBIT(*record, BIT6);
		    record += ApplVar.AP.Pb.Qty + 1;
		    memcpy(record, ApplVar.PBA.Rec.Amt.Value, ApplVar.AP.Pb.ArtAmt);
				record += ApplVar.AP.Pb.ArtAmt;
		}
		if (TESTBIT(ApplVar.AP.Pb.Random, BIT5) && ApplVar.AP.Plu.Number)               /* Random Stored ? */
	    memcpy(record, ApplVar.PBA.Random, ApplVar.AP.Plu.RandomSize);
    }
    WriteRam(SysBuf, ApplVar.AP.Pb.BlockSize);
    return 0;
}

BYTE PbBuffer(WORD pbnum)
{
	BYTE i, *record;

    if (!pbnum)
	return 0;
	if (!ApplVar.AP.StartAddress[AddrTrack])	 /* no track buffer */
	return 0;
    PbTotal(pbnum, 4);      /* get offset */
    ReadRam((BYTE*)&ApplVar.PB.Block, sizeof(ApplVar.PB.Block));
    if (!ApplVar.PB.Block)      /* first (opened) block ? */
    {
		if (ApplVar.PbSplit)    /* if split or refund then bill must be open */
		{
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI16);
		    return 1;
		}
		if (ApplVar.EmptyBlock < ApplVar.AP.Pb.NumberOfBlocks)    /* buffer not full */
		{
			RamOffSet -= sizeof(ApplVar.PB.Block) ;
		    ApplVar.EmptyBlock++;
		    WriteRam((BYTE*)&ApplVar.EmptyBlock, sizeof(ApplVar.PB.Block));    /* write start block */
		    ApplVar.EmptyBlock--;
		}
		else
		{
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI12);
		    return 1;
		}
    }
    MemSet(&ApplVar.Total, sizeof(ApplVar.Total), 0);   /* clear working area */
    if (!ApplVar.AP.Pb.ArtAmt)
    {
		if (ApplVar.PBA.Rec.Key.Code < PLU1)
		    ApplVar.PBA.Flag = 1;       /* set amount stored */
    }
    else
		ApplVar.PBA.Flag = 0;
    for(;;)
    {
		RamOffSet = ApplVar.AP.StartAddress[AddrTrack] ;
		if (!ApplVar.PB.Block)        /* get new block */
		{
		    if (ApplVar.EmptyBlock < ApplVar.AP.Pb.NumberOfBlocks)    /* buffer not full */
			{
				RamOffSet += ((UnLong)ApplVar.EmptyBlock * ApplVar.AP.Pb.BlockSize);	/* start new block */
				ReadRam((char*)&ApplVar.PB.Block, sizeof(ApplVar.PB.Block));   /* read next free block */
				if (ApplVar.PB.Block)  /* next free block linked ? */
				    ApplVar.EmptyBlock = ApplVar.PB.Block - 1;
				else
				    ApplVar.EmptyBlock++;
				RamOffSet -=  sizeof(ApplVar.PB.Block) ;
				MemSet(SysBuf, sizeof(SysBuf), 0);
				UpdatePbRecord(SysBuf + 2, 1);
				return 0;
		    }
		    else
		    {
				ApplVar.ErrorNumber=ERROR_ID(CWXXI12);
				return 1;
		    }
		}
		ApplVar.PB.Block--; 		/* minus 1 for offset calculation */
		RamOffSet +=((UnLong)ApplVar.PB.Block * ApplVar.AP.Pb.BlockSize) ;
		ReadRam(SysBuf, ApplVar.AP.Pb.BlockSize);   /* max 128 bytes */
		RamOffSet -= ApplVar.AP.Pb.BlockSize ; /* reset pointer */
		record = (BYTE *)SysBuf + 2;
		for (i = 0; i < ApplVar.AP.Pb.NumberOfItems; i++)
		{
		    if (!*((WORD *) record))
		    {
				if (ApplVar.PbSplit)    /* split bill or refund error */
				{
				    ApplVar.ErrorNumber=ERROR_ID(CWXXI16);
				    return 1;
				}
				UpdatePbRecord(record, 1);  /* new record */
				return 0;
		    }
		    if (ApplVar.PBA.Rec.Key.Code > PLU1 && ApplVar.AP.Plu.RandomSize && TESTBIT(ApplVar.AP.Pb.Random, BIT5))
			{
				if (!CompareBCDValue(ApplVar.PBA.Random,
							record + 2 + 1 + ApplVar.AP.Pb.ArtAmt + ApplVar.AP.Pb.Qty,
								ApplVar.AP.Plu.RandomSize))     /* PLU Random Number ? */
			    {
				    if (!UpdatePbRecord(record, 0)) /* when different price next */
					    return 0;
			    }
			}
		    else if (*((WORD *) record) == ApplVar.PBA.Rec.Key.Code)
			{
				if (!UpdatePbRecord(record, 0)) /* when different price next */
					return 0;
			}
		    record += ApplVar.AP.Pb.ArtSize;
		}
		ApplVar.PB.Block = *((WORD *)SysBuf); /* next block */
		if (!ApplVar.PB.Block)
		{
		    if (ApplVar.PbSplit) /* if split bill or refund error */
		    {
			ApplVar.ErrorNumber=ERROR_ID(CWXXI16);
			return 1;
		    }
		    if (ApplVar.EmptyBlock < ApplVar.AP.Pb.NumberOfBlocks)
		    {
			ApplVar.EmptyBlock++;
			WriteRam((BYTE*)&ApplVar.EmptyBlock, sizeof(ApplVar.EmptyBlock));
			ApplVar.EmptyBlock--;
		    }
		    else
		    {
			ApplVar.ErrorNumber=ERROR_ID(CWXXI12);
			return 1;
		    }
		}
    }
}


void PrintPbItem(BYTE *printed, BYTE *group, BYTE *nextgroup)
{
    char *str, temp[18];
	BYTE saveprint;

	saveprint = ApplVar.PrintLayOut;  //20040326
	RESETBIT(ApplVar.PrintLayOut, BIT3); /* reset print on slip  20040326*/
	if (ApplVar.PBA.Rec.Key.Code > PLU1)  /* plu ? */
    {
		ApplVar.PluNumber = ApplVar.PBA.Rec.Key.Code - PLU1 - 1;
		ReadPlu();
		ApplVar.DeptNumber = ApplVar.PluDept;
	}
    else
		ApplVar.DeptNumber = ApplVar.PBA.Rec.Key.Code - DEPT - 1;
	ReadDept();
    if (ApplVar.Dept.Group > *group)
    {
		if (!(*nextgroup) || ApplVar.Dept.Group < *nextgroup)
			*nextgroup = ApplVar.Dept.Group;
		ApplVar.PrintLayOut	= saveprint;  //20040326
		return;
	}
    if (ApplVar.Dept.Group != *group)
	{
		ApplVar.PrintLayOut	= saveprint;  //20040326
		return;
	}
	if (!(*printed))   /* group descriptor printed? */
    {
		*printed = 1;
		if (TESTBIT(ApplVar.PbF.Options, BIT5+BIT6))
		{
			ApplVar.GroupNumber = *group;
		    ReadGroup();
		}
		if (TESTBIT(ApplVar.PbF.Options, BIT6))
			PrintStr(ApplVar.Group.Name);
	}
    if (!ApplVar.AP.Pb.ArtAmt)
    {
		if (TESTBIT(ApplVar.Qty.Sign, BIT5))    /* amount ? */
		{
			ApplVar.Amt = ApplVar.Qty;
		    ApplVar.Qty = ONE;
		    ApplVar.PBA.Flag = 1;   /* set amount */
		}
		else
		{
			ApplVar.Price = ZERO;   /* clear previous price */
		    memcpy(ApplVar.Price.Value, ApplVar.Plu.Price[0], ApplVar.AP.Plu.PriceSize);
		    ApplVar.Amt = ApplVar.Price;
		}
	}
    else
    {
		ApplVar.Amt.Sign = 0;
		if (ApplVar.Qty.Sign & 0x40)    /* negative amount */
			SETBIT(ApplVar.Amt.Sign, BIT7);
		ApplVar.Price = ApplVar.Amt;
	}
    Multiply(&ApplVar.Amt, &ApplVar.Qty);
    RoundBcd(&ApplVar.Amt, 0);
    if (ApplVar.Qty.Sign & 0x03)     /* ApplVar.Qty with decimal point? */
		AmtRound(1, &ApplVar.Amt);

    if (TESTBIT(PBPRINT, BIT3) && !CheckNotZero(&ApplVar.Amt))
	{
		ApplVar.PrintLayOut	= saveprint;  //20040326
		return;          /* print ZERO amounts? */
	}
	if (!TESTBIT(ApplVar.PbF.Options, BIT5))
	{
		if (ApplVar.PBA.Rec.Key.Code > PLU1)  /* plu ? */
			str = GetPluPrint(temp, KP_NORMAL);
		else
			str = ApplVar.Dept.Name;
		if (ApplVar.PBA.Flag)    /* amount ? */
			PrintAmt(str, &ApplVar.Amt);
		else
	    {
		    //PrintMultiply();
		    PrintQtyAmt(&ApplVar.Qty, str, &ApplVar.Amt,true);
		}
		if (TESTBIT(ApplVar.Dept.Options, BIT4))    /* print line ? */
		{
			MemSet(SysBuf, sizeof(SysBuf), '-');
		    PrintStr(SysBuf);
		}
    }
    else
	    Add(&ApplVar.SubTotal, &ApplVar.Amt);
	AddTaxItem(ApplVar.Dept.Tax);       /* add to tax itemizers */
	ApplVar.PrintLayOut	= saveprint;  //20040326
}


BYTE StorePbBuffer()
{
    BYTE i;

    if (ApplVar.FProforma)
		return 0;
    if (ApplVar.FSplit)
    {
		if (!ApplVar.FVoid && !ApplVar.FCorr)
	    	ApplVar.PbSplit = 1;
    }
    else if (ApplVar.FCorr)
		ApplVar.PbSplit = 2;        /* set correction */
    ApplVar.PBA.Rec = ApplVar.RGRec;  /* copy record */
	ApplVar.PBA.Rec.Amt.Value[BCDLEN-1]= 0; /* reset level for consolidate */
	if (ApplVar.RGRec.Key.Code > PLU1 && TESTBIT(ApplVar.AP.Pb.Random, BIT5))
		memcpy(ApplVar.PBA.Random, ApplVar.Plu.Random, ApplVar.AP.Plu.RandomSize);
    if (ApplVar.RGRec.Key.Code < DEPT)
		ApplVar.PBA.Total = ApplVar.RGRec.Amt;
    else
    {
		ApplVar.PBA.Total = ApplVar.Amt;
		if (ApplVar.FPrice || ApplVar.PluPriceLevel)
		    ApplVar.PBA.Flag = 1;       /* indicate plu other price */
		else
		    ApplVar.PBA.Flag = 0;
    }
    i = PbBuffer(ApplVar.PbNumber);
    ApplVar.PbSplit = 0;
    return i;
}

/* process ApplVar.PB Buffer, if to_pb is non zero then transfer else */
/* print buffer */

void ProcessPbBuffer(WORD to_pb)
{
    BYTE i, sort, group, nextgroup, printed;
	char *str,space[2];
    WORD startblock, currentblock;
	UnLong save, save1;

	if (!ApplVar.AP.StartAddress[AddrTrack])			/* no track buffer */
		return;
    if (!to_pb)
    {
		sort = 0;   /* if sort is 0 then articles */
			/* if sort is 1 then discounts */
			/* if sort is 2 then pora */
					/* if sort is 3 or 4 the Receiptnumber for Greece */
		ApplVar.SubTotal = ZERO;
		group = 0;
		nextgroup = 0;
		ApplVar.MultiplyCount = 1;
		printed = 0;
    }
    else
    {
		if (!ApplVar.PB.Block)  /* from not used yet */
		    return;
		if (ApplVar.EmptyBlock >= ApplVar.AP.Pb.NumberOfBlocks)    /* buffer full */
		{                       /* at least 1 free block for a transfer */
		    ApplVar.ErrorNumber=ERROR_ID(CWXXI12);
		    return;
		}
		MemSet(&ApplVar.PBA.Rec, sizeof(ApplVar.PBA.Rec), 0);
    }
    currentblock = ApplVar.PB.Block;
    startblock = ApplVar.PB.Block;
    for(;;)
    {
		currentblock--; 		/* minus 1 for offset calculation */
		RamOffSet = ApplVar.AP.StartAddress[AddrTrack] + (UnLong)currentblock * ApplVar.AP.Pb.BlockSize ;
		ReadRam((BYTE*)&currentblock, sizeof(currentblock));
		save = RamOffSet ;
		for (i = 0; i < ApplVar.AP.Pb.NumberOfItems; i++)
		{
			RamOffSet = save ;
		    ReadRam((BYTE*)&ApplVar.PBA.Rec.Key, sizeof(ApplVar.PBA.Rec.Key));   /* read keycode */
			save = RamOffSet + ApplVar.AP.Pb.Qty + ApplVar.AP.Pb.ArtAmt + 1;
			if (TESTBIT(ApplVar.AP.Pb.Random, BIT5))
			{
				if (ApplVar.PBA.Rec.Key.Code > PLU1 && ApplVar.AP.Plu.RandomSize)
				{
					save1 = RamOffSet ;
					RamOffSet = save ;
					ReadRam(ApplVar.PBA.Random, ApplVar.AP.Plu.RandomSize);
					ApplVar.PluNumber = GetPluNumber(0, ApplVar.PBA.Random);
					RamOffSet = save1 ;
					if (!ApplVar.PluNumber)
						ApplVar.PBA.Rec.Key.Code = DEPT + ApplVar.AP.Dept.Number;
								/* last department */
					else
						ApplVar.PBA.Rec.Key.Code = ApplVar.PluNumber + PLU1;
				}
				if (ApplVar.AP.Plu.Number)
					save += ApplVar.AP.Plu.RandomSize ;
			}
		    if (!ApplVar.PBA.Rec.Key.Code)
				continue;
		    ApplVar.PBA.Flag = 0;
		    if (to_pb)      /* transfer */
		    {
				ReadRam((BYTE*)&ApplVar.PBA.Rec.Qty, ApplVar.AP.Pb.Qty + 1);
				ReadRam(ApplVar.PBA.Rec.Amt.Value, ApplVar.AP.Pb.ArtAmt);
				if (TESTBIT(ApplVar.AP.Pb.Random,BIT5) && ApplVar.AP.Plu.Number)
					ReadRam(ApplVar.PBA.Random, ApplVar.AP.Plu.RandomSize);
				if (!ApplVar.AP.Pb.ArtAmt && TESTBIT(ApplVar.PBA.Rec.Qty.Sign, BIT5))
				{
				    ApplVar.PBA.Total = ApplVar.PBA.Rec.Qty;
				    ApplVar.PBA.Flag = 1;
				}
				else
				{
				    ApplVar.PBA.Rec.Amt.Sign = 0;
				    if (ApplVar.PBA.Rec.Qty.Sign & 0x40)    /* negative amount */
					SETBIT(ApplVar.PBA.Rec.Amt.Sign, BIT7);
				}
				ApplVar.PBA.Total = ApplVar.PBA.Rec.Amt;        /* used fo PORA and DISCOUNT */
				PbBuffer(to_pb);        /* transfer record */
				continue;
		    }
			ApplVar.Amt = ZERO;
		    ApplVar.Qty = ZERO;     /* clear amt and qty */
		    if (ApplVar.PBA.Rec.Key.Code > DEPT)    /* department or plu ? */
		    {
				if (sort)
			    	continue;
				ReadRam((BYTE*)&ApplVar.Qty, ApplVar.AP.Pb.Qty + 1);
				if (!CheckNotZero(&ApplVar.Qty))
					continue;
				if (ApplVar.AP.Pb.ArtAmt)
					ReadRam(ApplVar.Amt.Value, ApplVar.AP.Pb.ArtAmt);
				PrintPbItem(&printed, &group, &nextgroup);
			    if (ApplVar.PBA.Rec.Key.Code < PLU1 ||    /* department ? */
						!TESTBIT(ApplVar.AP.Options.Plu, BIT0)) /* no link */
					continue;
				if (TESTBIT(ApplVar.AP.Options.Plu, BIT2) ||
							!ApplVar.AP.Pb.ArtAmt) /* link stored seperate */
					continue;
				while(!ApplVar.ErrorNumber && ApplVar.Plu.Link)
				{
					ApplVar.PBA.Rec.Key.Code = GetLinkNumber();     /* number in ApplVar.Plu.Link */
					if (!ApplVar.PBA.Rec.Key.Code)  /* Random Code not found */
						break;
					ApplVar.PluNumber = ApplVar.PBA.Rec.Key.Code - PLU1 - 1;
					ReadPlu();
					ApplVar.Amt = ZERO;
				    memcpy(ApplVar.Amt.Value, ApplVar.Plu.Price[0], ApplVar.AP.Plu.PriceSize);
					PrintPbItem(&printed, &group, &nextgroup);
				}
		    }
		    else
		    {
				if (!ApplVar.AP.Pb.ArtAmt)
				    ReadRam((BYTE*)&ApplVar.Amt, ApplVar.AP.Pb.Qty + 1);
				else
				{
				    ReadRam((BYTE*)&ApplVar.Qty, ApplVar.AP.Pb.Qty + 1);
				    ReadRam(ApplVar.Amt.Value, ApplVar.AP.Pb.ArtAmt);
				    ApplVar.Amt.Sign = 0;
				    if (ApplVar.Qty.Sign & 0x40)    /* negative amount */
					SETBIT(ApplVar.Amt.Sign, BIT7);
				}
				if (!CheckNotZero(&ApplVar.Amt))
				    continue;
				if (ApplVar.PBA.Rec.Key.Code < DISC + 100)
				{
				    if (sort != 1)
					continue;
				    ApplVar.DiscNumber = ApplVar.PBA.Rec.Key.Code - DISC - 1;  /* discount */
				    ReadDisc();
				    str = ApplVar.Disc.Name;
				    if (!TESTBIT(ApplVar.Disc.Options, BIT4))    /* subtotal */
					AddTaxItem(ApplVar.Disc.Tax);
				    else
					Add(&ApplVar.SaleAmt, &ApplVar.Amt);
				}
				else
				{
				    if (sort != 2)
					continue;
				    ApplVar.PoRaNumber = ApplVar.PBA.Rec.Key.Code - PORA - 1;      /* pora */
				    ReadPoRa();
				    str = ApplVar.PoRa.Name;
				    Add(&ApplVar.SaleAmt, &ApplVar.Amt);
				}
				if (!printed)
				{
				    printed = 1;
				    if (TESTBIT(ApplVar.PbF.Options, BIT6))
				    {
				    	space[0]=' ';
				    	space[1]=0;
						PrintStr(space);      /* print empty line */
					}
				}
				PrintAmt(str, &ApplVar.Amt);
		    }
		}
		if (to_pb)      /* transfer ? */
		{
			startblock--;		  /* minus 1 for offset calculation */
			RamOffSet = (UnLong)startblock * ApplVar.AP.Pb.BlockSize +ApplVar.AP.StartAddress[AddrTrack] ;
		    ApplVar.EmptyBlock++;
		    WriteRam((BYTE*)&ApplVar.EmptyBlock, sizeof(ApplVar.PB.Block));  /* link free block */
		    ApplVar.EmptyBlock = startblock;
		    if (!currentblock)  /* end reached ? */
				return;
		    startblock = currentblock;
		}
		else if (!currentblock)  /* end reached */
		{
		    if (sort < 2)
		    {
				if (!sort)
				{
				    if (printed && TESTBIT(ApplVar.PbF.Options, BIT5))
				    {
						PrintAmt(ApplVar.Group.Name, &ApplVar.SubTotal);
						ApplVar.SubTotal = ZERO;
				    }
				    printed = 0;
				    if (group < nextgroup)
				    {
						group = nextgroup;
						nextgroup = 0;
				    }
				    else
						sort++;
				}
				else
				    sort++;
				currentblock = startblock;  /* reset start */
		    }
		    else
		    {
				ApplVar.MultiplyCount = 0;
				return;
		    }
		}
    }
}

