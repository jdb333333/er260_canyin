//---------------------------------------------------------------------------
#ifndef CHNMESS_H
#define CHNMESS_H

#if (defined(CHAR_6X8) && defined(CASE_FORHANZI))
#error "Not allowed same time:CHAR_6X8 & CASE_FORHANZI "
#endif
#if (defined(DEBUGBYPC)&& defined(STM32F10X_HD))
#error "Not allowed same time:DEBUGBYPC & STM32F10X_HD "
#endif

#if (!defined(CHAR_6X8) || defined(CASE_FORHANZI))
#define SETDATEDISPX	6
#define PROMDISPX	    4       //State1显示位置
#define INPUTWIDTH	    12		//RG模式下,输入区宽度,
#define SETUPDISPX 	    10      //SET模式下,输入内容显示位置
#else
#define SETDATEDISPX	8
#define PROMDISPX	    4       //State1显示位置
#define INPUTWIDTH	    12		//RG模式下,输入区宽度,
#define SETUPDISPX 	    13      //SET模式下,输入内容显示位置
#endif

#define INPUTDISPX	(SCREENWD-INPUTWIDTH)	// State2显示位置
#define PROMPTWD	(SCREENWD-PROMDISPX)	//12 hf 20060105

#define SETINPUTWD		(SCREENWD-SETUPDISPX)	//SET模式下,输入内容显示宽度


//----------------------------------------------
#define CursX		ApplVar.sCursor.x
#define CursY		ApplVar.sCursor.y
// MSg[]>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define LING			0				//WAIT
#define WAITING          LING
#define SHOUKUAN		(LING+1)		//营业
#define XBBIAO			(SHOUKUAN+1)	//读帐
#define ZBBIAO			(XBBIAO+1)		//清帐
#define SHEZHI			(ZBBIAO+1)		//设置
#define JINLI			(SHEZHI+1)		//经理

//vvvvvvvvvvvvvvvv设置档菜单相关的信息描述vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//设置档各级菜单的项目数存储在SubMenuItems中

/* 设置档一级菜单(主菜单) */
#define MENUFILES		(JINLI+1)		//资料管理
#define ZHGNSZ			(MENUFILES+1)	//系统设置
#define DEFKEYBOARD 	(ZHGNSZ+1)	    //键盘定义
#define PJYSSZ			(DEFKEYBOARD+1)	//票头票尾
#define COMMHOST        (PJYSSZ+1)      //通讯
#define TXKYWS			(COMMHOST+1)    //外接设备
#define CYGNSZ			(TXKYWS+1)		//餐饮设置
#define BBGNSZ			(CYGNSZ+1)		//报表管理
#define MENUDISC        (BBGNSZ+1)      //折扣促销
#define JYGLSZ			(MENUDISC+1)	//销售功能设置
#define ICKGLI			(JYGLSZ+1)		//IC卡管理
#if !defined(DD_LOCK)
#define QXSZHI			(ICKGLI+1)		//密码权限
#else
#define QXSZHI			(ICKGLI)		//密码权限
#endif
#define MENUSERVICE		(QXSZHI+1)		//维护(SERVICE)
//用于菜单管理,Msg[]中无信息>>>>>>>>>>>>>>>.........
#define MENUID_FILES	(1)		//资料管理
#define MENUID_SYS		(MENUID_FILES+1)	//系统设置
#define MENUID_DEFKB 	(MENUID_SYS+1)	    //键盘定义
#define MENUID_HEAD		(MENUID_DEFKB+1)	//票头票尾
#define MENUID_COMM     (MENUID_HEAD+1)      //通讯
#define MENUID_DEV		(MENUID_COMM+1)    //外接设备
#define MENUID_RES		(MENUID_DEV+1)		//餐饮设置
#define MENUID_REP		(MENUID_RES+1)		//报表管理
#define MENUID_DISC     (MENUID_REP+1)      //折扣促销
#define MENUID_OP		(MENUID_DISC+1)	//销售功能设置
#define MENUID_VIP		(MENUID_OP+1)		//IC卡管理
#if !defined(DD_LOCK)
#define MENUID_PWD		(MENUID_VIP+1)		//密码权限
#else
#define MENUID_PWD		(MENUID_VIP)		//密码权限
#endif
#define MENUID_SERVICE	(MENUID_PWD+1)		//维护(SERVICE)
#define SUBMENTMAX      (MENUSERVICE-MENUFILES+1)
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<........

//设置档二级菜单>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    #define SETIDXFROM      (MENUSERVICE+1)           //设置项目的开始序号

    /* ZHGNSZ下的综合功能设置菜单 */
    #define SETSYSFLAG      SETIDXFROM        //系统参数设置
    #define SETPOINTTYPE    (SETSYSFLAG+1)    //设置小数类型
    #define SETDATE		    (SETPOINTTYPE+1)	  //设置日期
    #define SETTIME 	    (SETDATE+1)  	  //设置时间
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
    #define SETEJOURNAL     (SETTIME+1)         //设置EJ
#else
    #define SETEJOURNAL     (SETTIME)         //设置EJ
#endif
    #define	MENUSYSMAX	    (SETEJOURNAL-SETSYSFLAG+1)		/* ccr2014-11-11 NEWSETUP Step-3 total items of sys menu */

    /* DEFKEYBOARD下的综合功能设置菜单 */
    #define SETKEYB   	    (SETEJOURNAL+1) 	  //设置键盘
    #define SETKEYMACRO     (SETKEYB+1)         //  MESSKEYMACRO            "键盘宏"
    #define SETKEYTYPE      (SETKEYMACRO+1)     //  MESSKEYTYPE             "键盘模式"
    #define SETPRINTKEYB    (SETKEYTYPE+1)      //打印键盘布局
    #define	MENUKEYBMAX	    (SETPRINTKEYB-SETKEYB+1)		/* ccr2014-11-11 NEWSETUP Step-3 total items of sys menu */

    /* 键盘模式选择　*/
    #define SETREST   	    (SETPRINTKEYB+1)		    //启用餐饮键盘
    #define SETMARKET       (SETREST+1)		        //启用超市键盘
    #define KEYTYPEMAX      (SETMARKET-SETREST+1)   //键盘模式选择

    #define SETGROUP   	    (SETMARKET+1)	  //柜组 //ccr2014-11-11 NEWSETUP Step-3<<<<<<
    #define SETSALER   	    (SETGROUP+1)	    //营业员

    /* SPGLSZ下的商品管理菜单 */
    #define SETPLU   	    (SETSALER+1)		 //单品
    #define SETDEPT   	    (SETPLU+1) 	 //部类
#if !defined(CASE_FORHANZI)
    #define SETTAX   	    (SETDEPT+1) 	    //税率
    #define SETCLERK   	    (SETTAX+1)	    //收款员
#else
    #define SETCLERK   	    (SETDEPT+1)	    //收款员
#endif
    #define	MENUPLUMAX	    (SETCLERK-SETPLU+1)	/* total items of plu manages */

    /* COMMHOST下的通讯菜单 */
    #define SETCOMPUTER   	(MENUPLUMAX+SETPLU)	    //"计算机"
    #define SETIP		    (SETCOMPUTER+1)        //IP地址
    //ccr2014-11-11 NEWSETUP Step-3>>>>>>
    #if defined(CASE_GPRS)
    #define SETGPRS         (SETIP+1)
    #else
    #define SETGPRS         (SETIP)
    #endif

    #define	MENUHOSTMAX	    (SETGPRS-SETCOMPUTER+1)		/* total items of comm.port setup */

    /* TXKYWS下的外设菜单 */
    #define SETBARCODE   	    (SETGPRS+1)        //"条码枪"
    #define SETBALANCE   	    (SETBARCODE+1)        //"电子秤"
    //ccr2016-01-22#define SETPORT4   	    (SETBALANCE+1)        //通讯口4
    #define SETSP           (SETBALANCE+1)		    //票据打印
    #define SETKP           (SETSP+1)           //厨房打印
    #define SETEXTPRINTER         (SETKP+1)          //"EXTERNAL PRINT"
    #define	MENUCOMMAX	    (SETEXTPRINTER-SETBARCODE+1)		/* total items of comm.port setup */

    #define SETPROM         (SETEXTPRINTER+1)          //中奖设置

    /* MENUDISC下的折扣促销功能设置*/
    #define SETOFF		    (SETPROM+1)           //优惠价
    #define SETDISC   	    (SETOFF+1)		    //折扣
    #define MENUDISCMAX     (SETDISC-SETOFF+1)

    /* JYGLSZ下的销售功能设置*/
    #define SETTEND   	    (SETDISC+1)		    //付款方式
    #define SETCORR   	    (SETTEND+1)		    //更正功能
    #define SETDRAWER       (SETCORR+1)		    //钱箱
    #define SETPORA   	    (SETDRAWER+1)		    //出金入金
    #define SETCURR   	    (SETPORA+1)		    //外币
    #define	MENUMANMAX	    (SETCURR-SETTEND+1)		/* total items of comsume manages */

    /* ICKGLI下的IC卡管理*/
    #define SETVIPIC           (SETCURR+1)          //IC卡和VIP设置
    #define SETADDNEWVIP       (SETVIPIC+1)  //SetAddNewECRVIP //"ADD NEW VIP"
    #define SETPRINTVIP       (SETADDNEWVIP+1)  //SetPrintECRVIP //"PRINT VIP"
    #define SETCLEANVIP       (SETPRINTVIP+1)   //"CLEAN VIP"
    #define SETBLOCKIC      (SETCLEANVIP+1)           //IC卡挂失
    #define SETPRINTIC	    (SETBLOCKIC+1)	    //打印卡信息
    #define	MENUICCMAX	    (SETPRINTIC-SETVIPIC+1)		/* total items of Chip Card */

    /* PJYSSZ下的票头票尾*/
    #define SETHEAD   	    (SETPRINTIC+1)	    //收据票头
    #define SETTRAIL   	    (SETHEAD+1)         //收据票尾
    #define SETSHEAD   	    (SETTRAIL+1) 	    //票据票头
    #define SETGRAP         (SETSHEAD+1)	    //票尾图片
    #define SETPRTGRAP      (SETGRAP+1)		    //打印图片
    #define	MENUTICMAX	    (SETPRTGRAP-SETHEAD+1)		/* total items of tictes */

    /* BBGNSZ下的报表功能*/
    #define SETREPORT       (SETPRTGRAP+1)	    //设置报表内容
    #define SETREPTYPE      (SETREPORT+1)	    //报表单元
    #define SETZONES   	    (SETREPTYPE+1)	    //报表时段设置
    #define SETCLRREP       (SETZONES+1)	    //总清报表
    #define SETPRTJOUR 	    (SETCLRREP+1)	    //打印流水
    #define	MENUREPMAX	    (SETPRTJOUR-SETREPORT+1)		/* total items of report */

    /* CYGNSZ下的餐饮功能*/
    #define SETPBINF   	    (SETPRTJOUR+1)	    //桌台
    #define SETPBF   	    (SETPBINF+1)	    //餐饮功能
    #define SETMODIF   	    (SETPBF+1) 	 	    //菜单说明
#if (agreeNumber)
xxx    #define SETAGREE        (SETMODIF+1)        //签单
#endif
    #define	MENUTBLMAX	    (SETMODIF-SETPBINF+1)		/* total items of table function */

    /* QXSZHI下的密码权限设置*/
    #if !defined(DD_LOCK)
    #define SETSETPWD       (SETMODIF+1)	    //设置档密码
    #define SETXPWD   	    (SETSETPWD+1)	    //只读报表密码
    #define SETZPWD   	    (SETXPWD+1)		    //清帐报表密码
    #define SETMGPWD   	    (SETZPWD+1)	    //经理档密码
    #define	MENUPWDMAX	    (SETMGPWD-SETSETPWD+1)		/* total items of password setup */
    #else
    #define SETMGPWD   	    (SETMODIF+1)	    //经理档密码
    #endif

    /* MENUSERVICE下的系统测试功能 */
    #define SETTEST        (SETMGPWD+1)      //系统测试
    #define SETCLEARPLU     (SETTEST+1)      //清除单品
    #define SETPLULIST      (SETCLEARPLU+1)		 //打印商品清单
    #define SETTSTVER	    (SETPLULIST+1)         //打印版本
    #define SETCONFIGPLU    (SETTSTVER+1)      //配置单品
    #define SETCONFIGDEPT   (SETCONFIGPLU+1)              //配置部类
#if CASE_USBDISK
    #define SETINSTALL      (SETCONFIGDEPT+1)    //安装/备份
    #define SETPRINTFAT     (SETINSTALL+1)      //打印文件分配表
#else
    #define SETPRINTFAT     (SETCONFIGDEPT+1)      //打印文件分配表
#endif
    #define SETPRNMENU	    (SETPRINTFAT+1)      //打印设置菜单
    #define MENUSERVICEMAX  (SETPRNMENU-SETTEST+1)

    /* SKJCSHI下的系统测试功能 */
    #define SETTSTDISP	    (SETPRNMENU+1)       //显示测试
    #define SETTSTCUST	    (SETTSTDISP+1)       //客显测试
    #define SETTSTPRT	    (SETTSTCUST+1)      //打印测试
    #define SETTSTCLK	    (SETTSTPRT+1)       //时钟测试
    #define SETTSTMEM	    (SETTSTCLK+1)       //RAM测试
    #define SETTSTBELL	    (SETTSTMEM+1)       //蜂鸣器测试
    #define SETTSTDRAW	    (SETTSTBELL+1)      //钱箱测试
    #define SETTSTKBD	    (SETTSTDRAW+1)      //键盘测试
    #define SETTSTCOMM	    (SETTSTKBD+1)       //串口测试
    #define SETTSTAUTO	    (SETTSTCOMM+1)      //自动测试

    #define	MENUTSTMAX	    (SETTSTAUTO-SETTSTDISP+1)		/* total items of test function */
    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    #define SETUPMAX	    SETTSTAUTO	//hf 20060105
    #if (SETUPMAX>254)
    #error "Error on setup IDX" //设置功能号不能>255
    #endif
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//ccr2016-02-04>>>>专用于ListMenu,Msg[]无信息>>>>>>>>>>>>>>>>>>>>>>>
#define LISTMGFUNC        (SETUPMAX+1)      //ccr2016-01-15 经理挡下选择小计功能ListItem序号
#define LISTKEYDES    (LISTMGFUNC+1)      //ccr2016-02-04 功能码描述ListItem序号
#define LISTKEYSID      (LISTKEYDES+1)  //ccr2016-02-04 功能码明细描述ListItem序号
#define LISTNUMRIC      (LISTKEYSID+1)  //ccr2016-02-25 数字码列表
//ccr2016-02-04<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#define JLHAO			(SETUPMAX+1)	//记录#
#define SHRDMA			(JLHAO+1)		//记录?
#define ZONGJI			(SHRDMA+1)		//总计
#define ZHHUAN			(ZONGJI+1)		//转换
#define XIAOJI			(ZHHUAN+1)		//小计
#define CHUJIN			(XIAOJI+1)		//出金
#define RUJIN			(CHUJIN+1)		//入金
#define BAOBIAO			(RUJIN+1)		//报表
#define ZENJIA			(BAOBIAO+1)		//增加
#define GUAQI			(ZENJIA+1)		//挂单
#define QUXIAO			(GUAQI+1)		//取消
#define MIMA			(QUXIAO+1)		//密码
#define MMQREN			(MIMA+1)		//密码确认
#define DANJIA			(MMQREN+1)		//单价:
#define JINEF			(DANJIA+1)		//金额:
#define CHAOSHI			(JINEF+1)		//超市版
#define CANYIN			(CHAOSHI+1)		//餐饮版
#define CEISHI			(CANYIN+1)		//测试
#define RAM				(CEISHI+1)		//  RAM:
#define YINGFU			(RAM+1)		//应付
#define JINGE			(YINGFU+1)		//净额
#define JIAGE			(JINGE+1)		//价格
#define JIAOYISHU		(JIAGE+1)		//交易数
#define SHULIANG		(JIAOYISHU+1)		//数量
#define JINE			(SHULIANG+1)		//金额
#define SHOUQU			(JINE+1)		//收取
#define ZHAOLING		(SHOUQU+1)		//找零
#define RI				(ZHAOLING+1)		//日
#define ZHOU			(RI+1)		//周
#define YUE				(ZHOU+1)		//月
#define NIAN			(YUE+1)		//年
#define BIAOZHUN		(NIAN+1)		//标准
#define SHKYUANS		(BIAOZHUN+1)		//收款员
#define SHI				(SHKYUANS+1)		//   时
#define RIS				(SHI+1)		//   日
#define YUES			(RIS+1)		//   月
#define TUIHUO			(YUES+1)		//退货
#define CHENBEN			(TUIHUO+1)		//成本
#define ZUOHAO			(CHENBEN+1)		//桌号#
#define ZHPHAO			(ZUOHAO+1)		//支票号#
#define XIANGSHU		(ZHPHAO+1)		//项数:
#define HSHXSHOU		(XIANGSHU+1)		//含税销售
#define SHUIE			(HSHXSHOU+1)		//税额
#define YEHAO			(SHUIE+1)		//页号
#define HAOMA			(YEHAO+1)		//号码:
#define GUAZHE			(HAOMA+1)		//挂帐
#define KAITAI			(GUAZHE+1)		//开台
#define QZHJSHU			(KAITAI+1)		//清帐计数
#define QINGCHU			(QZHJSHU+1)		//清除
#define SBAOBIAO		(QINGCHU+1)		//S-报表
#define MAOLI			(SBAOBIAO+1)		//毛利
#define KCZJIA			(MAOLI+1)		//库存增减
#define KCJIANSHAO	    (KCZJIA+1)		//库存增减
#define YZHZHANG		(KCJIANSHAO+1)		//原桌帐:
#define XYKHAO			(YZHZHANG+1)		//信用卡#
#define XZHZHANG		(XYKHAO+1)		//新桌帐:
#define RENSHU			(XZHZHANG+1)		//人数
#define HUANHUO			(RENSHU+1)		//换货
#define ZENJIAA			(HUANHUO+1)		//增加?
#define SHANCHU			(ZENJIAA+1)		//删除?ZENJIAA与SHANCHU不能间隔
#define ZHKOU1			(SHANCHU+1)		//折扣项 1
#define ZHKOU2			(ZHKOU1+1)		//折扣项 2
#define XSHOUE			(ZHKOU2+1)		//销售额
#define DBYHUI			(XSHOUE+1)		//大包优惠
#define KCSHLIANG		(DBYHUI+1)		//库存数量
#define TJYHUI			(KCSHLIANG+1)		//特价优惠
#define XUANXIANG		(TJYHUI+1)		//选项:
#define GDZHKOU			(XUANXIANG+1)		//固定
#define FDZHKOU1		(GDZHKOU+1)		//浮动
#define GFZHKOU1		(FDZHKOU1+1)		//固/浮
#define KTZHJIE			(GFZHKOU1+1)		//开台暂结
#define KTZJIA			(KTZHJIE+1)		//打印开台
#define ZHANJIE			(KTZJIA+1)		//暂结
#define QRJZHANG		(ZHANJIE+1)		//确认结帐
#define DYKTAI			(QRJZHANG+1)		//打印开台
#define DYZHDAN			(DYKTAI+1)		//打印帐单
#define QXQREN			(DYZHDAN+1)		//取消确认
#define FENZHANG		(QXQREN+1)		//分帐
#define ZHUANZHANG		(FENZHANG+1)		//转帐
#define GHSHKYUAN		(ZHUANZHANG+1)		//更换桌台
#define GKRSHU			(GHSHKYUAN+1)		//顾客人数
#define ZHJSHDING		(GKRSHU+1)		//中奖设定:
#define JIFEN			(ZHJSHDING+1)		//消费积分:
#define GBDPJGE			(JIFEN+1)		//改变单品价格
#define CHFDYJZU		(GBDPJGE+1)		//厨房打印机组
#define ZJKCUN			(CHFDYJZU+1)		//增加库存
#define JSHKCUN			(ZJKCUN+1)		//减少库存
#define SHURU			(JSHKCUN+1)		//输入
#define DAIFU			(SHURU+1)		//待付
#define RS232ERR		(DAIFU+1)		//RS232错
#define RS232OK			(RS232ERR+1)		//RS232正确
#define RTCERR			(RS232OK+1)		//RTC 错!
#define CTCERR			(RTCERR+1)		//CTC 错!
#define FPROMERR		(CTCERR+1)		//FPROM错!
#define CHSHHUA			(FPROMERR+1)		//初始化...
#define SHSHHJSHU		(CHSHHUA+1)		//初始化结束.
#define CHUCUO			(SHSHHJSHU+1)		//出错-
#define MMACUO			(CHUCUO+1)		//密码错 ！
#define BCMMA			(MMACUO+1)		//密码已保存
#define SHKYUANF		(BCMMA+1)		//收款员:
#define YYYUANF			(SHKYUANF+1)		//营业员:
#define SHKJHAO			(YYYUANF+1)		//收款机号:
#define WEIZHI			(SHKJHAO+1)		//位置:
#define QUERENF			(WEIZHI+1)		//确认?
#define WANCHENG		(QUERENF+1)		//完成
#define FXBBEN			(WANCHENG+1)		//发行版本:
#define BCXFEI			(FXBBEN+1)		//本次消费点
#define ZJDPIN			(BCXFEI+1)		//新增单品:
#define SHCHDP			(ZJDPIN+1)		//删除单品:
#define KSHJCE			(SHCHDP+1)		//开始检测>>
#define JCJSHU			(KSHJCE+1)		//<<检测结束
#define KUCUN			(JCJSHU+1)		//库存
#define BBQQING			(KUCUN+1)		//报表已全清!
#define GXBIOS			(BBQQING+1)		//更新程序切勿断电
#define WJKJYJIE		(GXBIOS+1)		//文件空间越界
#define JSHDPSHL		(WJKJYJIE+1)		//内存溢出！
#define RZQMAN			(JSHDPSHL+1)		//流水存储区满
#define GHDYZHI			(RZQMAN+1)		//请更换打印纸
#define KZNCCUO			(GHDYZHI+1)		//扩展内存出错！
#define SHUOMINF		(KZNCCUO+1)		//说明文字:

#define CWXXI01			(SHUOMINF+1)		//无效输入!
#define CWXXI02			(CWXXI01+1)		//无效日期!
#define CWXXI03			(CWXXI02+1)		//无效时间!
#define CWXXI04			(CWXXI03+1)		//禁止输入!
#define CWXXI05			(CWXXI04+1)		//PLU代码为空!
#define CWXXI06			(CWXXI05+1)		//PLU文件溢出!
#define CWXXI07			(CWXXI06+1)		//桌台被占用!
#define CWXXI08			(CWXXI07+1)		//桌台已打开!
#define CWXXI09			(CWXXI08+1)		//桌台号不对!
#define CWXXI10			(CWXXI09+1)		//请输入桌台号
#define CWXXI11			(CWXXI10+1)		//桌台没有开台
#define CWXXI12			(CWXXI11+1)		//桌台桌帐溢出
#define CWXXI13			(CWXXI12+1)		//禁止修改时间
#define CWXXI14			(CWXXI13+1)		//仍在销售中!
#define CWXXI15			(CWXXI14+1)		//销售缓冲区满
#define CWXXI16			(CWXXI15+1)		//商品没有售出
#define CWXXI17			(CWXXI16+1)		//正在结算中!
#define CWXXI18			(CWXXI17+1)		//输入数据超长
#define CWXXI19			(CWXXI18+1)		//正在付帐!
#define CWXXI20			(CWXXI19+1)		//人数溢出!
#define CWXXI21			(CWXXI20+1)		//没有确认结帐
#define CWXXI22			(CWXXI21+1)		//禁止取消付款
#define CWXXI23			(CWXXI22+1)		//无菜名!
#define CWXXI24			(CWXXI23+1)		//无此收款员!
#define CWXXI25			(CWXXI24+1)		//收款员改变
#define CWXXI26			(CWXXI25+1)		//无此类报表!
#define CWXXI27			(CWXXI26+1)		//报表打印中断
#define CWXXI28			(CWXXI27+1)		//必须在经理档
#define CWXXI29			(CWXXI28+1)		//不能分帐时用
#define CWXXI30			(CWXXI29+1)		//必须输入号码!
#define CWXXI31			(CWXXI30+1)		//转入到
#define CWXXI32			(CWXXI31+1)		//未授权!
#define CWXXI33			(CWXXI32+1)		//请指定收款员
#define CWXXI34			(CWXXI33+1)		//请指定营业员
#define CWXXI35			(CWXXI34+1)		//禁止PLU变价！
#define CWXXI36			(CWXXI35+1)		//密码不对!
#define CWXXI37			(CWXXI36+1)		//厨房打印机错
#define CWXXI38			(CWXXI37+1)		//票据打印机错
#define CWXXI39			(CWXXI38+1)		//打印压杆抬起
#define CWXXI40			(CWXXI39+1)		//打印纸用完!
#define CWXXI41			(CWXXI40+1)		//打印温度太高
#define CWXXI42			(CWXXI41+1)		//按键未定义!
#define CWXXI43			(CWXXI42+1)		//必须输入金额/* zero price not allowed enter amount */
#define CWXXI44			(CWXXI43+1)		//禁止多种付款
#define CWXXI45			(CWXXI44+1)		//记帐无此功能
#define CWXXI46			(CWXXI45+1)		//未按小计键!
#define CWXXI47			(CWXXI46+1)		//正在管理库存
#define CWXXI48			(CWXXI47+1)		//保存流水错
#define CWXXI49			(CWXXI48+1)		//MODEM通讯错
#define CWXXI50			(CWXXI49+1)		//卡操作出错!
#define CWXXI51			(CWXXI50+1)		//POS代码错!
#define CWXXI52			(CWXXI51+1)		//卡数据错!
#define CWXXI53			(CWXXI52+1)		//为过期卡!
#define CWXXI54			(CWXXI53+1)		//为挂失卡!
#define CWXXI55			(CWXXI54+1)		//客户类别错!
#define CWXXI56			(CWXXI55+1)		//为新卡!
#define CWXXI57			(CWXXI56+1)		//不是购物卡!
#define CWXXI58			(CWXXI57+1)		//写卡出错!
#define CWXXI59			(CWXXI58+1)		//卡号不对!
#define CWXXI60			(CWXXI59+1)		//禁用折扣卡!
#define CWXXI61			(CWXXI60+1)		//禁用现金卡!
#define CWXXI62			(CWXXI61+1)		//禁用赊帐卡!
#define CWXXI63			(CWXXI62+1)		//禁 IC 卡!
#define CWXXI64			(CWXXI63+1)		//清卡出错!
#define CWXXI65			(CWXXI64+1)		//数据溢出!
#define CWXXI66			(CWXXI65+1)		//IC充值出错!
#define CWXXI67			(CWXXI66+1)		//IC初始化出错
#define CWXXI68			(CWXXI67+1)		//禁止初始化!
#define CWXXI69			(CWXXI68+1)		//IC金额不足!
#define CWXXI70			(CWXXI69+1)		//IC付款出错!
#define CWXXI71			(CWXXI70+1)		//IP地址错!
#define CWXXI72			(CWXXI71+1)		//无IC卡!
#define CWXXI73			(CWXXI72+1)		//已有挂单

#define CWXXI74         (CWXXI73+1)         //  电池电量低"
#define CWXXI75         (CWXXI74+1)         //  没激活库存"
#define CWXXI76         (CWXXI75+1)         // 无U盘!     //
#define CWXXI77         (CWXXI76+1)         // 打开文件出错     //
#define CWXXI78         (CWXXI77+1)         // 读文件出错!     //
#define CWXXI79         (CWXXI78+1)         // 写文件出错!     //
#define CWXXI80         (CWXXI79+1)         //建新文件出错     //
#define CWXXI81         (CWXXI80+1)         // 关闭文件出错     //
                                            //GPRS>>>>>
#define CWXXI82    	    (CWXXI81+1)         //必须发送流水数据
#define CWXXI83         (CWXXI82+1)         //GPRS出错
#define CWXXI84         (CWXXI83+1)         //发送数据出错
#define CWXXI85         (CWXXI84+1)         //接收数据出错
#define CWXXI86         (CWXXI85+1)         //无法连接服务器
#define CWXXI87         (CWXXI86+1)         //无服务器IP地址
#define CWXXI88         (CWXXI87+1)         //无SIM卡
#define CWXXI89         (CWXXI88+1)          //GPRS未就绪
#define CWXXI90         (CWXXI89+1)          //已到最大数
#define CWXXI91         (CWXXI90+1)          //无此会员

//ccr2016-10-24 EJ >>>>>>>>>>>>>>>>>>>
#define CWXXI92 		(CWXXI91 +1)	    //"EJ DATA ERROR"
#define CWXXI93 		(CWXXI92 +1)         //"NO SD CARD"
#define CWXXI94 		(CWXXI93 +1)         //"EJ R/W ERROR"
#define CWXXI95 		(CWXXI94 +1)         //"NO EJ RECORDS"
#define CWXXI96 		(CWXXI95 +1)         //"EJ NEARLY FULL"
#define CWXXI97 		(CWXXI96 +1)         //"EJ IS FULL"
#define CWXXI98 		(CWXXI97 +1)         //"NEW EJ"
#define CWXXI99 		(CWXXI98 +1)         //"RESET EJ FIRST"
#define CWXXI100		(CWXXI99 +1)         //"EJ NOT MATCHED"

#define CWXXIMAX    	CWXXI100            //错误信息数目<127
//...........................................................

#define DKQXIANG		(CWXXIMAX+1)		//打开钱箱
#define YOUGUADAN		(DKQXIANG+1)	//有挂单
#define SHURUQTYAMT		(YOUGUADAN+1)	//  输入商品数量和商品价格
#define ZHUANRUDAO      (SHURUQTYAMT+1)    //ZHUANRUDAO "转入到"
#define ERRORMSG		(ZHUANRUDAO+1)	//Error-
#define CURRENCY		(ERRORMSG+1)	//CUR
#define TOTALMSG		(CURRENCY+1)	//TOT
#define CHANGE			(TOTALMSG+1)	//CHG
#define SUBTOTAL		(CHANGE+1)		//SUB
#define DISCOUNT		(SUBTOTAL+1)	//Dis
#define PUTOUT			(DISCOUNT+1)	//PO
#define RECIEVIN		(PUTOUT+1)		//RA
#define OPENDRAW		(RECIEVIN+1)	//DRAW
#define CANCEL			(OPENDRAW+1)	//CANC
#define ADDMODI			(CANCEL+1)		//ADD
#define TABLENUM		(ADDMODI+1)		//TABLE
#define SERVICE			(TABLENUM+1)	//SER
#define CHECKPAID		(SERVICE+1)		//CPA
#define CHECKCPAID		(CHECKPAID+1)	//CCP
#define COVERS			(CHECKCPAID+1)	//COV
#define CORRECT			(COVERS+1)		//COR
#define REFUND			(CORRECT+1)		//RET
#define CANCEL1			(REFUND+1)		//CANCEL 1
#define CANCEL2			(CANCEL1+1)		//CANCEL 2
#define ZZXZZKU			(CANCEL2+1)		//正在下载字库
#define ZZXZTXING		(ZZXZZKU+1)		//正在下载图形
#define DOWNLOAD		(ZZXZTXING+1)	//★
#define UPLOAD			(DOWNLOAD+1)	//☆
#define KAI				(UPLOAD+1)		//是
#define GUAN			(KAI+1)		    //否
#define MINCHENF		(GUAN+1)		//名称:
#define BULEIF			(MINCHENF+1)	//部类:
#define GUIZUF			(BULEIF+1)		//柜组:
#define HOLD			(GUIZUF+1)		//HOLD
#define DAYINF			(HOLD+1)		//打印:
#define XUANXIANGF		(DAYINF+1)		//选项:
#define LIANJIE			(XUANXIANGF+1)	//链接:
#define SHUILVF			(LIANJIE+1)		//税率:
#define WZHBHAOF		(SHUILVF+1)		//位置编号:
#define JIAGE1			(WZHBHAOF+1)	//价格1:
#define JIAGE2			(JIAGE1+1)		//价格2:
#define JIAGE3			(JIAGE2+1)		//价格3:
#define JIAGE4			(JIAGE3+1)		//价格4:
#define JINJIA			(JIAGE4+1)		//进价:
#define ZHKLVF			(JINJIA+1)		//折扣率(%):
#define ZDZHIF			(ZHKLVF+1)		//最大值:
#define FIXMSG			(ZDZHIF+1)		//折扣率:
#define MAXMSG			(FIXMSG+1)		//最大值:
#define HUILVF			(MAXMSG+1)		//税率:
#define MRJIAF			(HUILVF+1)		//买入价:
#define MCHJIAF			(MRJIAF+1)		//卖出价:
#define ZHUANGTAIF		(MCHJIAF+1)		//状态:
#define QIANXIANGF		(ZHUANGTAIF+1)		//钱箱:
#define OTDMSG			(QIANXIANGF+1)		//OTD
#define CHFDYINF		(OTDMSG+1)		//厨房打印:

#define POIMSG			(CHFDYINF+1)		//POI
#define PREMSG			(POIMSG+1)		//周期:
#define QIANZHUIFU		(PREMSG+1)		//PER
#define LIANJIEF		(QIANZHUIFU+1)		//连接:
#define GNMA			(LIANJIEF+1)		//输入功能码
#define AGNJIAN			(GNMA+1)		//按功能键:
#define LEIXING			(AGNJIAN+1)		//类型:
#define KSHRQIF			(LEIXING+1)		//开始日期:
#define JSHRQIF			(KSHRQIF+1)		//结束日期:
#define KSHSHJIANF		(JSHRQIF+1)		//开始时间:
#define JSHSHJIANF		(KSHSHJIANF+1)		//结束时间:
#define XINGQIFF		(JSHSHJIANF+1)		//星期:
#define ZHEKOUF			(XINGQIFF+1)		//折扣:
#define DBSHLIANGF		(ZHEKOUF+1)		//大包数量:
#define DANJIAF			(DBSHLIANGF+1)		//单价:
#define YHJIAF			(DANJIAF+1)		//优惠价:
#define TXXYIF			(YHJIAF+1)		//通讯协议:
#define DHHMAF			(TXXYIF+1)		//电话号码:
#define MIMAF			(DHHMAF+1)		//密码:
#define PINLV			(MIMAF+1)		//频率:
#define ZXJERF			(PINLV+1)		//最小金额:
#define TXKHAOF			(ZXJERF+1)		//通讯口:
#define JDJER			(TXKHAOF+1)		//计点金额:
#define TXXHAO			(JDJER+1)		//图象序号:
#define SHIJIANF		(TXXHAO+1)		//时间:
#define RIQIF			(SHIJIANF+1)	//日期:
                                        //
#define DYJXHAO			(RIQIF+1)		//打印机型号
#define DZCHXHAO        (DYJXHAO+1)     //ccr2017-03-16"秤型号:",

#define EUTRONMSG		(DZCHXHAO+1)		//创造优秀优创造就欢迎使用优创收款机
#define SHKJSDING		(EUTRONMSG+1)	//  收款机已锁定
#define AFSHSUO			(SHKJSDING+1)	//按【方式锁】开机
#define XSHFPIAO		(AFSHSUO+1)		//形式发票
#define CHFDYJHAO		(XSHFPIAO+1)	//厨房打印机#
#define PXZHTAI			(CHFDYJHAO+1)	//*** 培训状态 ***
#define JIADIAN			(PXZHTAI+1)		//***** 加电 *****
#define FKFZHANG		(JIADIAN+1)		//*** 分开付帐 ***
#define FZHSHJU			(FKFZHANG+1)	//*** 复制收据 ***
#define GBPLUJGE		(FZHSHJU+1)		//*** 商品变价 ***
#define ZJIAKUCUN		(GBPLUJGE+1)	//*** 增加减库存 ***
#define JSHAOKUCUN		(ZJIAKUCUN+1)	//*** 增加减库存 ***
#define QUXIAOP			(JSHAOKUCUN+1)		//***** 取消 *****
#define CHXCHSHHUA		(QUXIAOP+1)		//** 程序初始化 **
#define SELECT1HZ       (CHXCHSHHUA+1)  //"按<选择>或<确认>"
#define KPTOTAL			(SELECT1HZ+1)	//KPTOTAL
#define TITLE			(KPTOTAL+1)		//品名      数量     单价     金额
#define TITLEQTYAMT		(TITLE+1)	    //品名      数量        金额
#define TITLEAMT		(TITLEQTYAMT+1)	    //品名      数量        金额

#define JANUARY			(TITLEAMT+1)	//一月
#define FEBRUARY		(JANUARY+1)		//二月
#define MARCH			(FEBRUARY+1)	//三月
#define APRIL			(MARCH+1)		//四月
#define MAY			    (APRIL+1)		//五月
#define JUNE			(MAY+1)		    //六月
#define JULY			(JUNE+1)		//七月
#define AUGUST			(JULY+1)		//八月
#define SEPTMBER		(AUGUST+1)		//九月
#define OCTOBER			(SEPTMBER+1)	//十月
#define NOVEMBER		(OCTOBER+1)		//十一月
#define DECEMBER		(NOVEMBER+1)	//十二月
#define MONDAY			(DECEMBER+1)	//星期一
#define TUESDAY			(MONDAY+1)		//星期二
#define WEDNSDAY		(TUESDAY+1)		//星期三
#define THURSDAY		(WEDNSDAY+1)	//星期四
#define FRIDAY			(THURSDAY+1)	//星期五
#define SATURDAY		(FRIDAY+1)		//星期六
#define SUNDAY			(SATURDAY+1)		//星期日
#define AQRJIAN			(SUNDAY+1)		//按确认键...
#define KPDYZE			(AQRJIAN+1)		//打印总额:
#define KPCXTDY			(KPDYZE+1)		//菜相同打印
#define KPCBTDY			(KPCXTDY+1)		//菜不同打印
#define JZHLJGE			(KPCBTDY+1)		//禁止零价格
#define DYFGXIAN		(JZHLJGE+1)		//打印分隔线
#define DXXSHOU			(DYFGXIAN+1)		//单项销售:
#define ZHKXIANG1		(DXXSHOU+1)		//折扣项1:
#define ZHKXIANG2		(ZHKXIANG1+1)		//折扣项2:
#define DKQXIANGF		(ZHKXIANG2+1)		//打开钱箱:
#define SHRJE			(DKQXIANGF+1)		//输入金额:
#define JZHSHRU			(SHRJE+1)		//禁止输入:
#define SHRHMA			(JZHSHRU+1)		//输入号码:
#define ZHLXFEI			(SHRHMA+1)		//找零为小费
#define DKQXIANGP		(ZHLXFEI+1)		//打开钱箱:
#define FKFSHJIAN		(DKQXIANGP+1)		//付款方式键
#define CRJE			(FKFSHJIAN+1)		//存入金额:
#define RJCHJIN			(CRJE+1)		//入金/出金
#if defined(CASE_INDIA)
#define PORANUMBER      (RJCHJIN+1)     //ccr2016-05-31  输入卡号
#define DYZHKE			(PORANUMBER+1)		//打印折扣额
#else
#define DYZHKE			(RJCHJIN+1)		//打印折扣额
#endif
#define SHYZHK1			(DYZHKE+1)		//使用折扣1:
#define SHYZHK2			(SHYZHK1+1)		//使用折扣2:
#define MCHJZHLIN		(SHYZHK2+1)		//卖出价找零
#define WXSHDIAN		(MCHJZHLIN+1)		//无小数点:
#define DKQXIANGD		(WXSHDIAN+1)		//打开钱箱:
#define YYZZHI			(DKQXIANGD+1)		//营业/增值:
#define DYSHLXIANG		(YYZZHI+1)		//打印税率项
#define DYLSHXXI		(DYSHLXIANG+1)		//打印0税项
#define WGSTSHUI		(DYLSHXXI+1)		//为GST类税
#define DYSHE			(WGSTSHUI+1)		//打印税额:
#define QXXZHI			(DYSHE+1)		//取消限制:
#define GGXZHI			(QXXZHI+1)		//更改限制:
#define QXJYIXZHI		(GGXZHI+1)		//取消交易:
#define THXZHI			(QXJYIXZHI+1)		//退货限制:
#define BFBZHKXZHI		(THXZHI+1)		//%折扣限制:
#define BFBJCHXZHI		(BFBZHKXZHI+1)		//%加成限制:
#define JJJEXZHI		(BFBJCHXZHI+1)		//+-金额限制
#define PSHMSHI			(JJJEXZHI+1)		//培训模式:
#define mPRNINDEX       (PSHMSHI+1)         //Ccr "取菜号:",
                                            //
#define ECRIPADDRESS    (mPRNINDEX+1)       //"IP Address:"
#define ECRNETID        (ECRIPADDRESS+1)   //"收款机ID号:"   //

#define SHRLJHAO		(ECRNETID+1)		//<记录号>  新纪录<确认>  当前纪录
#define SHRDPDMA		(SHRLJHAO+1)		//<条码>增删改商品<确认>  当前商品
#define TCHBJI			(SHRDPDMA+1)		//<退出>      退出
#define BJDQJLU			(TCHBJI    +1)		//<确认>      继续
#define ANXZJIAN		(BJDQJLU+1)		//选择打印报表
#define XFJE			(ANXZJIAN+1)		//消费金额
#define AJGNDYI			(XFJE    +1)		//键码     //键
#define JLSHYONG		(AJGNDYI+1)		//只能在经理档用
#define YYSHYONG		(JLSHYONG+1)		//可在营业档用
#define DYWZI			(YYSHYONG+1)		//打印文字
#define BAOLIU			(DYWZI    +1)		//保留
#define SHGNENG			(BAOLIU    +1)		//文件密码测试
#define ZHZBHAO			(SHGNENG+1)		//拨号
#define MODEMLINK		(ZHZBHAO+1)		//MODEM 连接!
#define DAIMA			(MODEMLINK+1)		//代码:
#define PZHEKOU			(DAIMA+1)		//%折扣
#define PJIACHEN		(PZHEKOU+1)		//%加成
#define SHJHAOF			(PJIACHEN+1)		//收据号:
#define	WENJSHU			(SHJHAOF+1)		//文件数
#define	WENJKJ			(WENJSHU+1)		//文件空间
#define	TJWENJ			(WENJKJ+1)		//统计文件
#define	JISHUI			(TJWENJ+1)		//计税
#define	LIUSKJ			(JISHUI+1)		//流水空间
#define	RAMKB			(LIUSKJ+1)		//RAM(KB)
#define	CONIFOT0		(RAMKB+1)		//文件名      记录数      空间大小
#define	CONIFOT1		(CONIFOT0+1)		//串口名称    通信类别   通信协议
#define	DBKHSHU			(CONIFOT1+1)		//顶部空行数:
#define	MYEHSHU			(DBKHSHU+1)		//每页行数:
#define	DAYXXI			(MYEHSHU+1)		//打印信息:
#define	DEZPJU			(DAYXXI+1)		//第二章票据:
#define	ZUOBJU			(DEZPJU+1)		//左边距:
#define	PDJIXING		(ZUOBJU+1)		//票打机型:
#define DYJGUAN			(PDJIXING+1)		//打印机关
#define DYJKAI			(DYJGUAN+1)		//打印机开
#define ICSZMIMA		(DYJKAI+1)		//密码:
#define ICSZMIMAF		(ICSZMIMA+1)		//密码确认:
#define POSDAIMA		(ICSZMIMAF+1)		//POS代码:
#define POSDIAMAF		(POSDAIMA+1)		//代码确认:
#define BCDMA			(POSDIAMAF+1)		//代码已保存
#define DMACUO			(BCDMA+1)		//代码错 ！
#define ICCA			(DMACUO+1)		//启用IC卡:
#define ZHEKOUCA		(ICCA+1)		//折扣卡:
#define XIANJINKA		(ZHEKOUCA+1)		//现金卡:
#define SHEZHANGKA		(XIANJINKA+1)		//赊帐卡:
#define DAYINICKA		(SHEZHANGKA+1)		//打印IC卡
#define ZDZHEKOU		(DAYINICKA+1)		//自动折扣:
#define XFJIFEN		(ZDZHEKOU+1)		//消费积分:
#define XJJIAN			(XFJIFEN+1)		//用现金键:
#define QYXYQI			(XJJIAN + 1)		//启用有效期
#define GUASHIIC		(QYXYQI + 1)		//挂失卡
#define KAHAO			(GUASHIIC+1)		//卡号:
#define KLXING			(KAHAO+1)		//卡类型:
#define KNJE			(KLXING+1)		//余额:
#define KYJIN			(KNJE+1)		//卡押金:
#define XFZE			(KYJIN+1)		//消费:
#define CHZHZE			(XFZE+1)		//充值总额:
#define SHYCSHU			(CHZHZE+1)		//使用次数:
#define JGLBIE			(SHYCSHU+1)		//价格级别:
#define PINMA			(JGLBIE+1)		//PIN码:
#define BHJBIE			(PINMA+1)		//保护级别:
#define ZDJZHANG		(BHJBIE+1)		//自动结帐:
#define ZHKRQI			(ZDJZHANG+1)		//制卡日期:
#define KYXQI			(ZHKRQI+1)		//卡有效期:
#define KHMCHEN			(KYXQI+1)		//客户:
#define CHSHHICKA		(KHMCHEN+1)		//初始化IC
#define ICKCHZHI		(CHSHHICKA+1)		//IC卡充值
#define QCHICKA			(ICKCHZHI+1)		//清除IC卡
#define CHZHTKUAN		(QCHICKA+1)		//充值/退款?
#define JDSDIAN			(CHZHTKUAN+1)		//减点/送点?
#define QRCHZHI			(JDSDIAN+1)		//确认充值?
#define QRTKUAN			(QRCHZHI+1)		//确认退款?
#define QRSDIAN			(QRTKUAN+1)		//确认送点?
#define DJQREN			(QRSDIAN+1)		//兑奖确认?
#define XFJDDJIANG		(DJQREN+1)		//计点兑奖
#define ICKTKUAN		(XFJDDJIANG+1)		//IC卡退款
#define ICKDJDIANG		(ICKTKUAN+1)		//IC卡兑奖点数
#define ICKSDIAN		(ICKDJDIANG+1)		//IC卡送点数
#define QDDAI			(ICKSDIAN+1)		//请等待......
#define KNYJE			(QDDAI+1)		//请先退款...
#define TMHYHAO			(KNYJE+1)		//会员号
#define BARREPORT		(TMHYHAO+1)		//         会 员 消 费 表
#define CHXKJI			(BARREPORT+1)		//要启用新的IP,请重新开机.
#define ZQBBIAO			(CHXKJI+1)		//确认清报表?
#define WAITICARD		(ZQBBIAO+1)		//等待IC卡....
#define BGDYIN			(WAITICARD+1)		//倍高打印

//ccr2015-06-15>>>>>>>>>>>>>>>>>>>>>
#define CLEARIC         (BGDYIN+1)                //"清卡",
#define REFUNDtoIC      (CLEARIC+1)               //"退款",
#define PAYMENTbyIC     (REFUNDtoIC+1)            //"付款",
#define CHARGEtoIC      (PAYMENTbyIC+1)           //"充值",
#define INITIALIC       (CHARGEtoIC+1)            //"初始化",
#define TAKECAHSfrIC    (INITIALIC+1)             //"取现"
#define ADDVIPPOINTS    (TAKECAHSfrIC+1)             //"增加积分"
#define DECVIPPOINTS    (ADDVIPPOINTS+1)             //"减分兑奖"
#define VIPPOINTS       (DECVIPPOINTS+1)             //"累计积分"
//ccr2015-06-15<<<<<<<<<<<<<<<<<<<<<
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define MsgEJ_X_STORE      (VIPPOINTS+1)//EJ中存储X数据
#define MsgEJ_Z_STORE      (MsgEJ_X_STORE+1)//EJ中存储Z数据
#define MsgEJ_TR_STORE     (MsgEJ_Z_STORE+1)//EJ中存储TR数据
#else
#define MsgEJ_TR_STORE     (VIPPOINTS)//EJ中存储TR数据
#endif
//ccr2016-02-22>>>>>>>>>>>>>>>>>>>>>
#define MENUXREPORT     (MsgEJ_TR_STORE  +1)  // 打印X报表
#if (CASE_USBDISK)
#define MENUXEXPORT     (MENUXREPORT   +1)  //导出报表数据
#define MENUQUICKVIEW   (MENUXEXPORT   +1)  //Quick View
#else
#define MENUQUICKVIEW   (MENUXREPORT   +1)  //Quick View
#endif
#define MENUXEJFUNC     (MENUQUICKVIEW +1)  //LOG. OPERATION

#if defined(CASE_GPRS)
#define MENUSENDNEWLOG  (MENUXEJFUNC   +1)  //发送新流水数据
#define MENUXPASSWORD   (MENUSENDNEWLOG+1)  //设置X密码
#else
#define MENUXPASSWORD   (MENUXEJFUNC   +1)  //设置X密码
#endif

#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define MENUXEJOURNAL  (MENUXPASSWORD+1)//MsgMENUXEJOURNAL     //   "E Journal"
#else
#define MENUXEJOURNAL   MENUXPASSWORD   //MsgMENUXEJOURNAL    //   "E Journal"
#endif

#define MENUXITEMMAX    (MENUXEJOURNAL-MENUXREPORT+1)
//.................................
#define MENUZREPORT     (MENUXEJOURNAL +1)  //打印Z报表
#if (CASE_USBDISK)
#define MENUZEXPORT     (MENUZREPORT   +1)  //导出报表数据
#define MENUCLEARLOG     (MENUZEXPORT   +1)  //清除LOG
#else
#define MENUCLEARLOG     (MENUZREPORT   +1)  //清除LOG
#endif
#define MENUZGRAND      (MENUCLEARLOG   +1)  //是否清除Grand TOTAL
#define MENUZPASSWORD   (MENUZGRAND    +1)  //设置Z密码

#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define MENUZRESETEJ   (MENUZPASSWORD+1)//MsgMENUZRESETEJ  ,     //   "RESET EJ"
#else
#define MENUZRESETEJ    MENUZPASSWORD
#endif

#define MENUZITEMMAX    (MENUZRESETEJ-MENUZREPORT+1)
//...................................

//ccr2016-02-22<<<<<<<<<<<<<<<<<<<<<
#define XZTITLE			(MENUZRESETEJ  +1)
#define XZTITLE1		XZTITLE		//销售日报表
#define XZTITLE2		(XZTITLE1+1)      //销售月报表
#define XZTITLE3		(XZTITLE2+1)      //商品报表
#define XZTITLE4		(XZTITLE3+1)      //桌帐报表
#define XZTITLE5		(XZTITLE4+1)      //时段报表
#define XZTITLE6		(XZTITLE5+1)      //收款员日报表
#define XZTITLE7		(XZTITLE6+1)      //收款员月报表
#define XZTITLE8		(XZTITLE7+1)      //全收款员日报表
#define XZTITLE9		(XZTITLE8+1)      //全收款员月报表
#if (salNumber)
#define XZTITLE10		(XZTITLE9+1)      //营业员日报表
#define XZTITLE11		(XZTITLE10+1)      //营业员月报表
#define XZTITLE12		(XZTITLE11+1)      //全营业员日报表
#define XZTITLE13		(XZTITLE12+1)      //全营业员月报表
#else
#define XZTITLE13		(XZTITLE9)      //全营业员月报表
#endif
#if (BARCUSTOMER==1)
#define XZTITLE14		(XZTITLE13+1)      //条码会员报表
#define XZITEMSMAX      (XZTITLE14-XZTITLE1+1)
#else
#define XZITEMSMAX      (XZTITLE13-XZTITLE1+1)
#endif
//....................................
#if (CASE_EPOS)
#define ePOSTAC			(XZTITLE1+XZNUM)  //ePOS认证码:
#define WAITEPOS		(ePOSTAC+1)        //等待易通卡
#define ePOSCARNO		(WAITEPOS+1)       //易通卡号:
#define eCARREMAIN		(ePOSCARNO+1)      //易通卡余额:
#else
#define eCARREMAIN		(XZTITLE1+XZNUM-1)     //部类名称     交易数量       金额
#endif


#if (CASE_USBDISK)
//ccr090715>>> USB applications >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define usbDAOCHU     (eCARREMAIN+1)		//Ccr导出收款机文件//
#define usbDAORU      (usbDAOCHU+1)       //Ccr导入收款机文件 //
#define usbFLOW       (usbDAORU+1)       //Ccr导出销售流水 //

#define usbMAINITEMS  (usbFLOW-usbDAOCHU+1)
//;;;;;;;;;;;;;;;;;;;;

#define usbALL			(usbFLOW+1)         //Ccr全部文件  //
#define usbBULEI      (usbALL+1)          //Ccr"部类",
#define usbDANPIN     (usbBULEI+1)         //Ccr"单品",
#define usbSHUILV     (usbDANPIN+1)        //Ccr"税率",
#define usbPIAOTOU    (usbSHUILV+1)         //Ccr"票头",
#define usbZHEKOU     (usbPIAOTOU+1)       //Ccr"折扣",
#if (salNumber)
#define usbYYYUAN     (usbZHEKOU+1)        //Ccr"营业员",
#define usbSTSHZHI    (usbYYYUAN+1)         //Ccr"系统设置",
#else
#define usbSTSHZHI    (usbZHEKOU+1)         //Ccr"系统设置",
#endif
#define usbTXKOU1     (usbSTSHZHI+1)       //Ccr"通讯口1",
#define usbTUXIN      (usbTXKOU1+1)       //Ccr"图形打印",
#define usbGUIZU      (usbTUXIN+1)        //Ccr"柜组",
#define usbICKA       (usbGUIZU+1)       //Ccr"IC卡",
#define usbZHJIANG    (usbICKA+1)           //Ccr"中奖设置",
#define usbCANYINGN   (usbZHJIANG+1)         //Ccr"餐饮功能",
#define usbSHKYUAN    (usbCANYINGN+1)        //Ccr"收款员",
#define usbSHUOMIN    (usbSHKYUAN+1)        //Ccr"说明文字",
#define usbZHUOTAI    (usbSHUOMIN+1)        //Ccr"桌台",
#define usbSHIDUANG   (usbZHUOTAI+1)         //Ccr"时段",
#define usbJIANPAN    (usbSHIDUANG+1)       //Ccr"键盘",
#define usbTSHCHJIA   (usbJIANPAN+1)         //Ccr"特殊出价",
#define usbCHJRJIN    (usbTSHCHJIA+1)       //Ccr"出金入金",
#define usbPIAOWEI    (usbCHJRJIN+1)        //Ccr"票尾",
#define usbPIAOJTOU   (usbPIAOWEI+1)  //Lyq"票据票头",
#define usbICCAGUASHI (usbPIAOJTOU+1)    //ccr"IC卡挂失",

#define usbFILEITEMS	(usbICCAGUASHI-usbALL+1)
//;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
#define usbFLOWNEW      (usbICCAGUASHI+1)   // 导出新数据
#define usbFLOWALL      (usbFLOWNEW+1)      // 导出全部数据
#define usbFLOWALLDEL   (usbFLOWALL+1)      // 导出全部数据后清空 //

#define usbFLOWITEMS	(usbFLOWALLDEL-usbFLOWNEW+1)
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#define HDEPTREPORT		(usbFLOWALLDEL+1)   //部类名称     交易数量       金额
#else
#define HDEPTREPORT		(eCARREMAIN+1)  //部类名称     交易数量       金额
#endif

#define HPLUREPORT		(HDEPTREPORT+1)    //商品名称     交易数量       金额
#define PRESSANYKEY     (HPLUREPORT+1)     //按任意键 MESS_PRESSKEY,         //

#define PRICEBARCODE    (PRESSANYKEY+1)     //ccr2016-01-22 特殊条码

#define HEADCENTER       (PRICEBARCODE+1)    //SysFlags32,  "票头居中打印"
#define TRAILCENTER      (HEADCENTER  +1)    //SysFlags33, "票尾居中打印"
#define HEADDBLHIGH      (TRAILCENTER +1)    //SysFlags34, "票头倍高打印"
#define TRAILDBLHIGH     (HEADDBLHIGH +1)    //SysFlags35, "票尾倍高打印"

//ccr2016-02-17>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//经理档功能定义...................
#define MsgMGADDINV    (TRAILDBLHIGH +1)//   "增加单品库存"<(1+小计功能)
#define MsgMGDECINV    (MsgMGADDINV  +1)//   "减少单品库存"<(2+小计功能)
#define MsgMGSETPRICE  (MsgMGDECINV  +1)//   "修改单品价格"<(3+小计功能)
#define MsgMGCLEARIC   (MsgMGSETPRICE+1)//   "会员退卡"<(4+小计功能)
#define MsgMGINITIC    (MsgMGCLEARIC +1)//   "初始化会员卡"<(5+小计功能)
#define MsgMGCHARGE    (MsgMGINITIC  +1)//   "会员卡充值"<(6+小计功能)
#define MsgMGADDPOINT  (MsgMGCHARGE  +1)//   "会员兑送积分"<(7+小计功能)
#define MsgMGPRINTSTOCK  (MsgMGADDPOINT+1)//   "打印单品库存"//ccr2017-01-10
#define MsgMGPASSWORD  (MsgMGPRINTSTOCK+1)//    "设置经理密码"
#define MsgMGCLOSEPRN  (MsgMGPASSWORD+1)//    "关闭打印"
#if (!defined(DD_LOCK) || !defined(TR))
#define MsgMGTRAINING  (MsgMGCLOSEPRN+1)//    "培训模式"
#else
#define MsgMGTRAINING  (MsgMGCLOSEPRN)//    无"培训模式"
#endif
#define MsgMGGOTOMG    (MsgMGTRAINING+1)//    "经理档"
#define MsgMGMMAX      (MsgMGGOTOMG-MsgMGADDINV+1)
//ccr2016-03-09>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define MsgALLLOG         (MsgMGGOTOMG  +1)//MESSALLLOG       "全部收据"
#define MsgDATERANGE     (MsgALLLOG  +1)//MESSDATERANGE   "日期范围"
#define MsgMESSVIPLOG    (MsgDATERANGE  +1)               //MESSVIPLOG        "会员消费记录"
#define MsgPRNCASH       (MsgMESSVIPLOG  +1)      //"打印付款记录"
#define MsgPRNDEPART     (MsgPRNCASH    +1)    //"打印部类记录"
#define MsgPRNPLU        (MsgPRNDEPART  +1)    //"打印单品记录"
#define MsgPRNDISCOUNT   (MsgPRNPLU     +1)    //"打印折扣加成"
#define MsgPRNRAPO       (MsgPRNDISCOUNT+1)    //"打印出入金"
#define MsgPRNCORRECT    (MsgPRNRAPO    +1)    //"打印更正操作"
#define MsgPRNCANCEL     (MsgPRNCORRECT +1)  //"打印交易取消"
#define MsgPRNNOSALE     (MsgPRNCANCEL +1)    //"打印非销售"
#define MsgPRNLOGMAX      (MsgPRNNOSALE-MsgALLLOG+1)

#define MsgALLCASH	     (MsgPRNNOSALE  +1)    //"全部付款方式"
#define MsgSELCASH		 (MsgALLCASH	+1)    //"选择付款方式"
                                               //
#define MsgALLDEPT		 (MsgSELCASH	+1)    //"打印全部部类"
#define MsgSELDEPT		 (MsgALLDEPT	+1)    //"选择一个部类"
                                               //
#define MsgALLPLU		 (MsgSELDEPT	+1)    //"打印全部单品"
#define MsgSELPLU		 (MsgALLPLU	    +1)    //"选择一个单品"

#define MsgPLUNUMBER        (MsgSELPLU	+1)       // "记录数:"
#define MsgPLURANDOMSIZE    (MsgPLUNUMBER	+1)   // "编码位数:"
#define MsgPLUNAMESIZE		(MsgPLURANDOMSIZE +1) // "名称长度:"
#define MsgPLUPRICESIZE     (MsgPLUNAMESIZE   +1) // "价格位数:"
#define MsgPLUPRICELEVEL    (MsgPLUPRICESIZE  +1) // "会员价级别:"
#define MsgPLUCOSTSIZE      (MsgPLUPRICELEVEL +1) // "进价位数:"
#define MsgPLUINVENTSIZE    (MsgPLUCOSTSIZE   +1) // "库存位数:"
#define MsgPLUREPQTYSIZE    (MsgPLUINVENTSIZE +1) // "统计数量位数:"
#define MsgPLUREPAMTSIZE    (MsgPLUREPQTYSIZE +1) // "统计金额位数:"
#define MsgPLUREPDISCSIZE   (MsgPLUREPAMTSIZE +1) // "统计折扣位数:"
#define MsgPLUREPCOSTSIZE   (MsgPLUREPDISCSIZE+1) // "统计进价位数:"
//ccr2016-03-09<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//.......................................................
#define MsgDATETYPE     (MsgPLUREPCOSTSIZE+1)  //"FORMAT "
#define MsgDDMMYYYY     (MsgDATETYPE+1)    //"DDMMYYYY"
#define MsgMMDDYYYY     (MsgDDMMYYYY+1)    //"MMDDYYYY"
#define MsgYYYYMMDD     (MsgMMDDYYYY+1)    //"YYYYMMDD"

//ccr2016-04-05>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define MsgPOINTTYPE1       (MsgYYYYMMDD+1)//"1.000,00"  //两位小数
#define MsgPOINTTYPE2       (MsgPOINTTYPE1+1)// "1,000.00"  //两位小数
#define MsgPOINTTYPE3       (MsgPOINTTYPE2+1)// "1.000"     //无小数
#define MsgPOINTTYPE4       (MsgPOINTTYPE3+1)// "1,000"     //无小数
#if !defined(CASE_FORHANZI)
#define MsgPOINTTYPE5       (MsgPOINTTYPE4+1)// "1.000,000"  //三位小数
#define MsgPOINTTYPE6       (MsgPOINTTYPE5+1)// "1,000.000"  //三位小数
#else
#define MsgPOINTTYPE6       (MsgPOINTTYPE4)
#endif
//#define MsgPOINTTYPE7       (MsgPOINTTYPE6+1)// "1.000,0"  //一位小数
//#define MsgPOINTTYPE8       (MsgPOINTTYPE7+1)// "1,000.0"  //一位小数
#define MsgPOINTMAX         (MsgPOINTTYPE6-MsgPOINTTYPE1+1)
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//ccr2016-10-31>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))
#define MsgEJMESSAGES     (MsgPOINTTYPE6+1) //"EJ存储信息"
#define MsgEJALLEJ        (MsgEJMESSAGES+1)//"打印全部EJ"
#define MsgEJDATERANGE    (MsgEJALLEJ    +1)// "日期范围EJ"
#define MsgEJRECEIPT      (MsgEJDATERANGE+1)// "打印收据EJ"
#define MsgEJXREPORT      (MsgEJRECEIPT  +1)// "打印X-EJ"
#define MsgEJZREPORT      (MsgEJXREPORT  +1)// "打印Z-EJ"
#define MsgEJCLERK        (MsgEJZREPORT  +1)//"打印收款员EJ"    

#define MsgPRNEJMAX      (MsgEJCLERK-MsgEJMESSAGES+1)
#else
#define MsgEJCLERK         MsgPOINTTYPE6
#endif
//ccr2016-10-31<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

#if defined(CASE_QRCODE)
#define MsgQRCODE           (MsgEJCLERK+1)//"二维码"
#else
#define MsgQRCODE           MsgEJCLERK
#endif

#define GPRSSERVERIP	(MsgQRCODE+1)//
#define GPRSSERVERPORT  (GPRSSERVERIP+1)

#if (!defined(CASE_GPRS))		// ADD BY LQW 090827
#define HZMESSMAX (GPRSSERVERPORT+1)
#else
//ccr2014-11-11 NEWSETUP Step-4 子菜单项目信息编号>>>>>>>>>>>>>>
#define GPRSAPNNAME     (GPRSSERVERPORT+1)
#define GPRSUSERNAME    (GPRSAPNNAME+1)
#define GPRSPASSWORD    (GPRSUSERNAME+1)

#define gprsSETMODE  (GPRSPASSWORD+1)
#define gprsSendECRLogAll (gprsSETMODE+1)
#define gprsDownloadPLU     (gprsSendECRLogAll+1) //"下载单品文件"
#define gprsDownloadDEPT    (gprsDownloadPLU+1)     //"下载部类文件"
#define gprsDownloadCLERK   (gprsDownloadDEPT+1) //"下载收款员"
#define gprsDownloadHEAD    (gprsDownloadCLERK+1) //"下载票头"
#define gprsDownloadTRAIL   (gprsDownloadHEAD+1) //"下载票尾"
#define gprsDownloadALL   (gprsDownloadTRAIL+1) //"下载全部数据"
#define gprsRestart         (gprsDownloadALL+1)//  "复位GPRS"    //gprsRestart //ccr2016-08-26
#define gprsSENDMESS    (gprsRestart+1)

#define gprsMAINITEMS   (10)        //子菜单项目数,需要修改MENUGPRSFUNSMAX

#define HZMESSMAX	    (gprsSENDMESS+1) //<<<<<<<<
// MSg[]<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>added by lqw 09-08-02
#define G_xDATATYPE 0                                            //ccr"数据类型错"
#define G_GPRSOK                  (G_xDATATYPE+1)                //ccr"GPRS READY"
#define G_GPRSNOTOK               (G_GPRSOK+1)                   //ccr"GPRS ERROR"
#define G_CONFIRM                 (G_GPRSNOTOK+1)                //ccr"请确认"
#define G_SERVERIP_NULL           (G_CONFIRM+1)                 //ccr"服务器IP或端口不对:"
#define G_X_IPPORT                (G_SERVERIP_NULL+1)           //ccr"服务器IP或端口不对:"
#define G_WAITFOR                 (G_X_IPPORT+1)                 //ccr等待GPRS
#define G_SHUJUFASONGZHONG        (G_WAITFOR+1)                      //"数据发送中.."
#define G_SHUJUYIFACHU            (G_SHUJUFASONGZHONG+1)         //"数据已发送.."
#define G_FASONGCHENGGONG         (G_SHUJUYIFACHU+1)      //"发送成功.."
#define G_LIANJIESERVERSHIBAI     (G_FASONGCHENGGONG+1)   //"连接服务器失败"
#define G_JIESHOUSHIBAI           (G_LIANJIESERVERSHIBAI+1)      //"接收失败"
#define G_FASONGSHIBAI            (G_JIESHOUSHIBAI+1)       //"发送失败"
#define G_SERVERIP	              (G_FASONGSHIBAI+1)               //
#define G_SERVERPORT              (G_SERVERIP+1)                 //
#define G_CONNECTING              (G_SERVERPORT+1)          // "正在建立连接.."
#define G_TOCONNECTING            (G_CONNECTING+1)               // "准备建立连接.."
#define G_FUWEIMOKUAI             (G_TOCONNECTING+1)            // "正在复位模块.."
#define G_CHENGGONGTUICHU         (G_FUWEIMOKUAI+1)              // "发送完毕,请退出.."
#define G_GPRSMessMax             (G_CHENGGONGTUICHU+1)

extern CONST GPRSSTR GPRSMess[G_GPRSMessMax]; //ADD BY LQW. 090802
#endif
//<<<<<<<<<<<<<<<<<<<

//---------------------------------------------------------------------------
extern CONST FSTRING Msg[];

//---------------------------------------------------------------------------
#define ERROR_ID(err) (err-CWXXI01+1)

extern CONST char SPACE[2];

extern const char
    cMsg_NOSTOCK[],  //无库存数据
    cMsg_DISPSTOCK[],//显示/打印?
#if (defined(CASE_EJSD) || defined(CASE_EJFLASH))//ccr2016-10-25>>>>EJ
    cMsg_RECNUMFR[], //"Rec_FROM"                  //RECNUMFR p
    cMsg_RECNUMTO[], //"Rec_TO"                    //RECNUMTO  p
    cMsg_EJBBIAO[],  //	"~E~J ~R~E~P~O~R~T"		//EJBBIAO $
    cMsg_EJXINXI[],  //	"EJ INFORMATIONS"            //EJXINXI  
    cMsg_EJBHAO[],   //"CODE"						//EJBHAO
    cMsg_SIZEEJ[],   //"Size of EJ"				//SIZEEJ
    cMsg_EJCSHUA[],  //	"EJ INITIALIZED"	   //EJCSHUA O 
    cMsg_EJLEFT[],   //Msg_EJLEFT,
    cMsg_FORMAT[],
    cMsg_EJDATEEND[],// "DATE LAST EJ"
    cMsg_EJRECORDS[],// "LAST EJ#"

#endif//ccr2016-10-25<<<<<<<<
    cMESS_PAYALSO[],
    cDEPTLIST[],
    DEPTTITLE[],
    cPLULIST[],
    cPLUTITLE1[],
    cPLUTITLE2[],
    cMESSPLUPRINTEND[],
    cMESS_TABLEHOLD[],
    cMESSPRNMENU[],
    cPRTStyleN1[],
    cPRTStyleN2[],
    cPRTStyleN3[],
    cPRTStyleN4[],
    cComTestN1[],
    cComTestN2[],
    cComTestN3[],
    cComTestN4[],
    cCurrentTimeN1[],
    cMESSCONFIGERR[],
    cMESSCLEANFILES[],
    cMESSRAMOUT[],
    cMESSDELPLU[],
    cMESSCLEANLOG[],
    cMESSCLEANLOGA[],
    cSpecialSymbol3[],
    cRECALLFLOW[],
    cErrInfoN1[],
    cErrInfoN3[],
    cMessageE18[],
    cEXPORTCSV[],
    cDEPTCAP[],
    cPLUCAP[],
    cTAXCAP[],
    cHEADERCAP[],
    cDISCCAP[],
    cSALPERCAP[],
    cSYSTEMCAP[],
    cCOMMCAP[],
    cGRAPPRNCAP[],
    cGROUPCAP[],
    cICCARDCAP[],
    cPROMCAP[],
    cPBFUNCCAP[],
    cCLERKCAP[],
    cMODICAP[],
    cPBTOTALCAP[],
    cZONECAP[],
    cKEYBOARDCAP[],
    cOFFCAP[],
    cPORACAP[],
    cTRAILERCAP[],
    cSLIPHEADCAP[],
    cICBLOCKCAP[],
    cUSBREPNAME[],
    cSetupCaption38[],
    cSetupCaption39[],
    cReportType0[],
    cReportType1[],
    cReportType2[],
    cReportType3[],
    cReportType7[],
    cReportType8[],
    cReportType13[],
    cReportType17[],
    cReportType18[],
    cCSVREPTITL[],
    cDEPTTITLE[],
#if defined(CASE_FORHANZI)
    cInputMode1[],
    cInputMode2[],
    cInputMode3[],
    cInputMode4[],
#endif
    cBUTTONBATLOW[],
    cCHARGEBATNOW[],
    cREPRN_PWON[],
    cComTestN3ERR[],
    cComTestN3OK[],
    cSRAMNOTFOUND[],
    cCLERKPASS[],
    cMESSSETMENU[],
    cCLEARGRANDTOTAL[],
    cMsgNET[],
    cCLOSEPRINTER[],
    cCLEANALLECRVIP[],
    cECRVIPTITLE[],//ECRVIPTITLE;//"会员号      消费金额     计点";
    cCREATEDATE[],//CREATEDATE;//创建日期:
    cVIPLOGOUT[],//VIPLOGOUT 会员退出
    cVIPTOTAL[],//VIPTOTAL 总额
    cVIPTOTALA[],//VIPTOTALA 消费总额
    cDECVIPPOINT[],//兑奖
    cADDVIPPOINT[],//送分
    cINPUTVIPNAME[],//输入会员姓名:
#if defined(CASE_PERU)
    cMsgCUSTOMER[],//     "CUSTOMER"
#endif
    cPRINTKEYBOARD[];//打印键盘布局
#endif
