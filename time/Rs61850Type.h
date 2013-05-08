/* Rs61850Type.h	*/
#ifndef _RS61850TYPE_H
#define _RS61850TYPE_H

#include "BaseType.h"

/* ====================================================================== */

#define IDENT_LEN			64 
#define NAME_LEN			65
#define FILENAME_LEN		255
#define VAL_LEN				96

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************************************************/

/* UTCTIME Quality	*/
#define	UTCQFLAG_FAILURE	0x00000040
#define	UTCQFLAG_NOTSYNCED	0x00000020

/* SQULITY	*/
#define	Q_LEN				13
/* Data0	*/
#define	QFLAG_GOOD			0x00
#define	QFLAG_INVLID		0x41		/* exp2val(6) & exp2val(0)	*/
/* Data1	*/
#define	QFLAG_PROCESS		0x00
#define	QFLAG_SUBST			0x20		/* exp2val(5)				*/

/* TrgOps and DataChg Reason	*/
#define	TRGOPS_DATA			0x40
#define	TRGOPS_QUALITY		0x20
#define	TRGOPS_FREEZE		0x10
#define	TRGOPS_INTEGRITY	0x08
#define	TRGOPS_GI			0x04

/* Rcb OptFlds	*/
#define	OPTFLDS_BITNO_SQNUM			1
#define	OPTFLDS_BITNO_TIMEOFENTRY	2
#define	OPTFLDS_BITNO_REASON		3
#define	OPTFLDS_BITNO_DATSET		4
#define	OPTFLDS_BITNO_DATAREF		5
#define	OPTFLDS_BITNO_BUFOVFL		6
#define	OPTFLDS_BITNO_ENTRYID		7
#define	OPTFLDS_BITNO_CONFREV		8
#define	OPTFLDS_BITNO_SUBSEQNUM		9

/* Log Data Form */
#define LOG_FORM_DATA				2
#define LOG_FORM_ANNOTATION 		3

/*************************************************************************************************/

typedef LONG	GTIME;

typedef struct
{
	INT32	ms;
	INT32	Days;
}BTIME6;

typedef struct
{
	INT32	Form;
	INT32	ms;
	INT32	Days;
}BTOD;

typedef struct
{
	UINT32	Secs;
	UINT32	Fraction;
	UINT32	Qflags;
}UTCTIME;

typedef UTCTIME	TIMESTAMP;

typedef UCHAR 	BSTR2[1];
typedef UCHAR 	BSTR6[1];
typedef UCHAR 	BSTR8[1];
typedef UCHAR 	BSTR9[2];
typedef UCHAR 	BSTR13[2];

/* ------------------------------------------------------------------  */
/* 注意：以下这些结构不能嵌在其他结构中 只能作为单个结构变量用 */
 typedef struct     
 {
 	INT16	Len;
 	UCHAR	Data[1];
 }BVSTR2;
 
 typedef struct
 {
 	INT16	Len;
 	UCHAR	Data[1];
 }BVSTR6;
 
 typedef struct
 {
 	INT16	Len;
 	UCHAR	Data[1];
 }BVSTR8; 
 
 typedef struct
 {
 	INT16	Len;
 	UCHAR	Data[2];
 }BVSTR9; 
 
 typedef struct
 {
 	INT16	Len;
 	UCHAR	Data[2];
 }BVSTR10; 
 
 typedef struct
 {
 	INT16	Len;
 	UCHAR	Data[3];
 }BVSTR24;

/* ------------------------------------------------------------------  */

typedef UCHAR 	OSTR8[8];

/* ------------------------------------------------------------------  */
/* 注意：以下这些结构不能嵌在其他结构中 只能作为单个结构变量用 */

 typedef struct 
 {
 	INT16	Len;
 	UCHAR	Data[64];
 }OVSTR64;
 
 typedef struct  
 {
 	INT16	Len;
 	UCHAR	Data[255];
 }OVSTR255;

 /* ------------------------------------------------------------------  */

typedef CHAR 	VSTR32[33];

typedef CHAR 	VSTR65[66];

typedef CHAR 	VSTR255[256];

typedef UCHAR 	UVSTR255[512];	/* Unicode Visible String */

typedef BTIME6	ENTRYTIME;

typedef VSTR65	OBJREF;

typedef OSTR8	ENTRYID;

/*  =======================================================================================================	*/

/* Enum Validity	*/
typedef enum {good, invalid, reserved, questionable} NVALIDITY;

/* Enum Range	*/
typedef enum {high_high, high, normal, low, low_low} NRANGE;

/* Enum DPS Val	*/
typedef enum {intermediate_state, off, on, bad_state} NDPSVAL;

/* Enum OrCat	*/
typedef enum {not_support, bay_control, station_control, remote_control, 
					automatic_bay, automatic_station, automatic_remote, maintenance, process} NORCAT;

/* Enum YkErr	*/
typedef enum {NoErr, UnknownErr, TimeoutTestNotOk, OperatorTestNotOk} NYKERR;

/* Enum Check	*/
typedef enum {no_check = 0x00, synchrocheck = 0x80, interlocking_check = 0x40, both = 0xC0} NCHECK;

/* Enum AddCause	*/
typedef enum {unknown, unsupport, blocked_by_switching_hierarchy, select_failed, invalid_position,
					parameter_change_in_execution, step_limit, blocked_by_Mode, blocked_by_process, blocked_by_interlocking,
					blocked_by_synchrocheck, command_already_in_execution, blocked_by_health, one_of_n_control, abortion_by_cancel,
					time_limit_over, abortion_by_trip, object_not_selected} NADDCAUSE;

/*  =======================================================================================================	*/

/*
typedef struct
{
	BOOL8		overflow;
	BOOL8		outOfRange;
	BOOL8		badReference;
	BOOL8		oscillatory;
	BOOL8		failure;
	BOOL8		oldData;
	BOOL8		inconsistent;
	BOOL8		inaccurate;
}SDETAILQUAL;

typedef struct
{
	typedef enum {process, substituted} NSOURCE;

	INT8		validity;			// M:
	SDETAILQUAL	detailQual;			// M:
	INT8		source;				// M:
	BOOL8		test;				// M:
	BOOL8		operatorBlocked;	// M:
}SQUALITY;

*/

typedef struct
{
	INT16		Len;				// M:
 	UCHAR		Data[2];			// M:
}SQUALITY;

/* Analog Value	*/
typedef struct 
{
/*	INT32		i;				// GC_1	*/
	FLOAT32		f;				/* GC_1	*/
}SANALOGVAL;

/* Analog Config	*/
typedef struct 
{
	FLOAT32		scaleFactor;	/* M	*/
	FLOAT32		offset;			/* M	*/
}SANALOGCFG;

/* Range Config	*/
typedef struct 
{
	SANALOGVAL	hhLim;			/* M	*/
	SANALOGVAL	hLim;			/* M	*/
	SANALOGVAL	lLim;			/* M	*/
	SANALOGVAL	llLim;			/* M	*/
	SANALOGVAL	min;			/* M	*/
	SANALOGVAL	max;			/* M	*/
}SRANGECFG;

/* Pulse Config	*/
typedef struct 
{
	ENUM8		cmdQual;		/* M	*/
	UINT32		onDur;			/* M	*/
	UINT32		offDur;			/* M	*/
	UINT32		numPls;			/* M	*/
}SPULSECFG;

/* Origin	*/
typedef struct
{
	ENUM8		orCat;			/* M:NORCAT	*/
	INT16		orIdentLen;		/* M		*/
	UCHAR		orIdentData[64];/* M		*/
}SORIGIN;

/* Unit		*/
typedef struct 
{
	ENUM8		SIUnit;			/* M	*/
	ENUM8		multiplier;		/* O	*/
}SUNIT;

/* VC		*/
typedef struct 
{
	FLOAT32		scaleFactor;	/* M	*/
	FLOAT32		offset;			/* O	*/
}SVC;

/* Vector	*/
typedef struct 
{
	SANALOGVAL	mag;			/* M	*/
	SANALOGVAL	ang;			/* O	*/
}SVECTOR;

/* Point	*/
typedef struct 
{
	FLOAT32		xVal;			/* M	*/
	FLOAT32		yVal;			/* M	*/
}SPOINT;

/*  =======================================================================================================	*/

/* SPS:单点状态信息	*/
/* SPS-ST			*/
typedef struct 
{
	BOOL8		stVal;			/* M	*/
	INT16		qLen;			/* M	*/
	UCHAR		qData[2];		/* M	*/
	TIMESTAMP	t;				/* M	*/
}SSPSST;

/* ---------------------------------------------------	*/
/* DPS:双点状态信息	*/
/* DPS-ST			*/
typedef struct 
{
	ENUM8		stVal;			/* M:NDPSVAL	*/
	INT16		qLen;			/* M	*/
	UCHAR		qData[2];		/* M	*/
	TIMESTAMP	t;				/* M	*/
}SDPSST;

/* ---------------------------------------------------	*/
/* INS:整数状态	*/
/* INS-ST		*/
typedef struct 
{
	INT32		stVal;			/* M	*/
	INT16		qLen;			/* M	*/
	UCHAR		qData[2];		/* M	*/
	TIMESTAMP	t;				/* M	*/
}SINSST;

/* ---------------------------------------------------	*/
/* MV:测量值	*/
/* MV-MX	*/
typedef struct 
{
/*	SANALOGVAL	instMag;		// O	*/
	SANALOGVAL	mag;			/* M	*/
/*	ENUM8		range;			// O:NRANGE	*/
	INT16		qLen;			/* M	*/
	UCHAR		qData[2];		/* M	*/
	TIMESTAMP	t;				/* M	*/
}SMVMX;

/* ---------------------------------------------------	*/
/* CMV:复数测量值	*/
/* CMV-MX			*/
typedef struct 
{
	SVECTOR		instCVal;		/* O	*/
	SVECTOR		cVal;			/* M	*/
	ENUM8		range;			/* O:NRANGE	*/
	INT16		qLen;			/* M	*/
	UCHAR		qData[2];		/* M	*/
	TIMESTAMP	t;				/* M	*/
}SCMVMX;

/* ---------------------------------------------------	*/
/* SAV:采样值	*/
/* SAV-MX		*/
typedef struct 
{
	SANALOGVAL	instMag;		/* M	*/
	INT16		qLen;			/* M	*/
	UCHAR		qData[2];		/* M	*/
	TIMESTAMP	t;				/* O	*/
}SSAVMX;

/* ---------------------------------------------------	*/
/* WYE:相对地测量值	*/
/* WYE				*/
typedef struct 
{
	SCMVMX		phsA;			/* GC_1	*/
	SCMVMX		phsB;			/* GC_1	*/
	SCMVMX		phsC;			/* GC_1	*/
	SCMVMX		nuet;			/* GC_1	*/
	SCMVMX		net;			/* GC_1	*/
	SCMVMX		res;			/* GC_1	*/
}SWYE;

/* ---------------------------------------------------	*/
/* DEL:相对相测量值	*/
/* DEL				*/
typedef struct 
{
	SCMVMX		phsAB;			/* GC_1	*/
	SCMVMX		phsBC;			/* GC_1	*/
	SCMVMX		phsCA;			/* GC_1	*/
}SDEL;

/* ---------------------------------------------------	*/
/* SPC:可控的单点	*/
/* SPC-ST			*/
typedef struct 
{
	BOOL8		stVal;			/* AC_ST	*/
	INT16		qLen;			/* M		*/
	UCHAR		qData[2];		/* M		*/
	TIMESTAMP	t;				/* AC_ST	*/
/*	BOOL8		stSeld;			// AC_CO_O	*/
}SSPCST;

/* SPC-CO	*/
typedef struct 
{
	BOOL8		ctlVal;
	SORIGIN		origin;
	UINT8		ctlNum;
	TIMESTAMP	t;
	BOOL8		test;
	BSTR2		check;
}SSPCCO;

/* SPC-CF	*/
typedef struct 
{
	SPULSECFG	PulseConfig;	/* AC_CO_O		*/
	ENUM8		CtlModels;		/* M:NCTLMODELS	*/
	UINT32		sboTimeout;		/* AC_CO_O		*/
	ENUM8		SboClasses;		/* AC_CO_O		*/
}SSPCCF;

/* ---------------------------------------------------	*/
/* APC:可控模拟设点信息	*/
/* APC-MX				*/
typedef struct 
{
	SANALOGVAL	setMag;			/* M	*/
	SORIGIN		origin;			/* O	*/
	INT16		qLen;			/* M	*/
	UCHAR		qData[2];		/* M	*/
	TIMESTAMP	t;				/* M	*/
}SAPCMX;

/* APC-SP		*/
typedef struct 
{
	SANALOGVAL	setMag;			/* M	*/
	SORIGIN		origin;			/* O	*/
	TIMESTAMP	operTm;			/* O	*/
}SAPCSP;

/* APC-CF		*/
typedef struct 
{
	ENUM8		CtlModels;		/* M	*/
	SUNIT		Unit;			/* O	*/
/*	SANALOGCFG	SVC;			// AC_SCAV	*/
	SANALOGVAL	minVal;			/* O	*/
	SANALOGVAL	maxVal;			/* O	*/
	UINT32		stepSize;		/* O	*/
}SAPCCF;

/* ---------------------------------------------------	*/
/* SPG:单点定值	*/
/* SPG-SP		*/
typedef struct 
{
	BOOL8		setVal;			/* AC_NSG_M	*/
}SSPGSP;

/* SPG-SG	*/
typedef struct 
{
	BOOL8		setVal;			/* AC_SG_M	*/
}SSPGSG;

/* SPG-SE	*/
typedef struct 
{
	BOOL8		setVal;			/* AC_SG_M	*/
}SSPGSE;

/* ---------------------------------------------------	*/
/* ING:整数状态定值	*/
/* ING-SP			*/
typedef struct 
{
	INT32		setVal;			/* AC_NSG_M	*/
}SINGSP;

/* INGG-SG	*/
typedef struct 
{
	INT32		setVal;			/* AC_SG_M	*/
}SINGSG;

/* ING-SE	*/
typedef struct 
{
	INT32		setVal;			/* AC_SG_M	*/
}SINGSE;

/* ---------------------------------------------------	*/
/* ASG:模拟定值	*/
/* ASG-SP		*/
typedef struct 
{
	SANALOGVAL	setMag;			/* AC_NSG_M	*/
}SASGSP;

/* ASG-SG	*/
typedef struct 
{
	SANALOGVAL	setMag;			/* AC_SG_M	*/
}SASGSG;

/* ASG-SE	*/
typedef struct 
{
	SANALOGVAL	setMag;			/* AC_SG_M	*/
}SASGSE;

/* ASG-CF	*/
typedef struct 
{
	SUNIT		units;			/* O	*/
/*	SANALOGCFG	sVC:			// AC_SCAV	*/
	SANALOGVAL	minVal;			/* O	*/
	SANALOGVAL	maxVal;			/* O	*/
	SANALOGVAL	stepSize;		/* O	*/
}SASGCF;

/* ---------------------------------------------------	*/
/* DPL:设备铭牌	*/
/* DPL-DC		*/
typedef struct 
{
	VSTR255		vendor;			/* M	*/
	VSTR255		hwRev;			/* O	*/
	VSTR255		swRev;			/* O	*/
/*	VSTR255		serNum;			// O	*/
/*	VSTR255		model;			// O	*/
/*	VSTR255		location;		// O	*/
}SDPLDC;

/* ---------------------------------------------------	*/
/* LPL:逻辑设备铭牌	*/
/* LPL-DC			*/
typedef struct 
{
	VSTR255		vendor;			/* M	*/
	VSTR255		swRev;			/* M	*/
	VSTR255		d;				/* M	*/
/*	UVSTR255	dU;				// M	*/
	VSTR255		configRev;		/* AC_LLN0_M	*/
}SLPLDC;

/*  ================================================================================	*/

/* Yk Sbo and Oper	*/
typedef struct 
{
	BOOL8		ctlVal;
	SORIGIN		origin;
	UINT8		ctlNum;
	TIMESTAMP	T;
	BOOL8		Test;
	INT16		CheckLen;
	UCHAR		CheckData[1];
}SYKOPER;

/* Yk Cancel	*/
typedef struct 
{
	BOOL8		ctlVal;
	SORIGIN		origin;
	UINT8		ctlNum;
	TIMESTAMP	T;
	BOOL8		Test;
}SYKCANCEL;

/* Control Sbo and Oper	*/

typedef struct 
{
	BOOL8		ctlVal;
	SORIGIN		origin;
	UINT8		ctlNum;
	TIMESTAMP	T;
	BOOL8		Test;
	INT16		CheckLen;
	UCHAR		CheckData[1];
}SSDPCOPER;

typedef struct 
{
	BOOL8		ctlVal;
	SORIGIN		origin;
	UINT8		ctlNum;
	TIMESTAMP	T;
	BOOL8		Test;
	INT16		CheckLen;
	UCHAR		CheckData[1];
}SSDPCCANCEL;

typedef struct 
{
	UINT8		ctlVal;
	SORIGIN		origin;
	UINT8		ctlNum;
	TIMESTAMP	T;
	BOOL8		Test;
	INT16		CheckLen;
	UCHAR		CheckData[1];
}SBSCOPER;

typedef struct 
{
	UINT8		ctlVal;
	SORIGIN		origin;
	UINT8		ctlNum;
	TIMESTAMP	T;
	BOOL8		Test;
}SBSCCANCEL;

typedef struct 
{
	INT8		ctlVal;
	SORIGIN		origin;
	UINT8		ctlNum;
	TIMESTAMP	T;
	BOOL8		Test;
	INT16		CheckLen;
	UCHAR		CheckData[1];
}SISCOPER;

typedef struct 
{
	INT8		ctlVal;
	SORIGIN		origin;
	UINT8		ctlNum;
	TIMESTAMP	T;
	BOOL8		Test;
}SISCANCEL;

typedef struct 
{
	INT32		ctlVal;
	SORIGIN		origin;
	UINT8		ctlNum;
	TIMESTAMP	T;
	BOOL8		Test;
	INT16		CheckLen;
	UCHAR		CheckData[1];
}SINCOPER;

typedef struct 
{
	INT32		ctlVal;
	SORIGIN		origin;
	UINT8		ctlNum;
	TIMESTAMP	T;
	BOOL8		Test;
}SINCANCEL;

/* ----------------------------------------------------------------- */

typedef struct 
{
	INT32		ctlVal;
	SORIGIN		origin;
	UINT8		ctlNum;
	TIMESTAMP	T;
	BOOL8		Test;
	INT16		CheckLen;
	UCHAR		CheckData[1];
}SCOOPER;

/* Control Cancel	*/
typedef struct 
{
	INT32		ctlVal;
	SORIGIN		origin;
	UINT8		ctlNum;
	TIMESTAMP	T;
	BOOL8		Test;
}SCOCANCEL;

/* Last App Error	*/
typedef struct 
{
	CHAR		ctlObj[NAME_LEN+1];
	ENUM8		err;
	SORIGIN		origin;
	UINT8		ctlNum;
	ENUM8		addCause;
}SLASTAPPERR;

/* ================================================================================	*/
/* Buffered Report Control Block	*/

typedef struct 
{
	CHAR		RptID[NAME_LEN+1];
	BOOL8		RptEna;
	CHAR		DatSet[NAME_LEN+1];
	UINT32		ConfRev;
	INT16		OptFldsLen;			/* BVSTR10	*/
	UCHAR		OptFldsData[2];
	UINT32		BufTm;
	UINT16		SqNum;
	INT16		TrgOpsLen;			/* BVSTR6	*/
	UCHAR		TrgOpsData[1];
	UINT32		IntgPd;
	BOOL8		GI;
	BOOL8		PurgeBuf;
	OSTR8		EntryID;
	BTIME6		TimeofEntry;
}SBRCB;

/* Unbuffered Report Control Block	*/

typedef struct 
{
	CHAR		RptID[NAME_LEN+1];
	BOOL8		RptEna;
	BOOL8		Resv;
	CHAR		DatSet[NAME_LEN+1];
	UINT32		ConfRev;
	INT16		OptFldsLen;			/* BVSTR10	*/
	UCHAR		OptFldsData[2];
	UINT32		BufTm;
	UINT8		SqNum;
	INT16		TrgOpsLen;			/* BVSTR6	*/
	UCHAR		TrgOpsData[1];
	UINT32		IntgPd;
	BOOL8		GI;
}SURCB;

/*  ================================================================================	*/
/*  Log Control Block	*/

typedef struct 
{
	BOOL8		LogEna;
	OBJREF		LogRef;
	OBJREF		DatSet;
	ENTRYTIME	OldEntrTm;
	ENTRYTIME	NewEntrTm;
	ENTRYID		OldEntr;
	ENTRYID		NewEntr;
	INT16		TrgOpsLen;			/* BVSTR6	*/
	UCHAR		TrgOpsData[1];
	UINT32		IntgPd;
}SLCB;

/*  ================================================================================	*/
/* Setting Group Control Block	*/

typedef struct 
{
	UINT8		NumOfSG;
	UINT8		ActSG;
	UINT8		EditSG;
	BOOL8		CnfEdit;
	TIMESTAMP	LActTm;
}SSGCB;

/*  ================================================================================	*/
/* Goose Control Block	*/

typedef UCHAR	MACADDR[6];

typedef struct 
{
	MACADDR		Addr;
	UINT8		PRIORITY;
	UINT16		VID;
	UINT16		APPID;
}SDstAddress;


typedef struct 
{
	BOOL8		GoEna;
	OBJREF		GoID;
	OBJREF		DatSet;
	UINT32		ConfRev;
	BOOL8		NdsCom;
	SDstAddress	DstAddress;
}SGOCB;

/*  ================================================================================	*/
/* Msv Control Block	*/

typedef struct 
{
	BOOL8		SvEna;
	OBJREF		MsvID;
	OBJREF		DatSet;
	UINT32		ConfRev;
	UINT16		SmpRate;
	INT16		OptFldsLen;			/* BVSTR3	*/
	UCHAR		OptFldsData[1];
}SMSVCB;

/* Usv Control Block	*/

typedef struct 
{
	BOOL8		SvEna;
	BOOL8		Resv;
	OBJREF		UsvID;
	OBJREF		DatSet;
	UINT32		ConfRev;
	UINT16		SmpRate;
	INT16		OptFldsLen;			/* BVSTR3	*/
	UCHAR		OptFldsData[1];
}SUSVCB;

/*  ================================================================================	*/

typedef struct
{
	UINT32		Size;
	BOOL8		mtimPres;
	TIMET		mtime;
	CHAR		FileName [FILENAME_LEN+1];
}SDIRENT;

/* -----------------------------------------------------------------------	*/

typedef struct
{
	INT32		Len;
	UCHAR		*Data;
}SVARDATA;

typedef struct
{
	CHAR		*VarTag;
	SVARDATA	VarData;
}SVARINFO;

/* -----------------------------------------------------------------------	*/

#ifdef __cplusplus
}
#endif


#endif  /* _RS61850TYPE_H	*/
