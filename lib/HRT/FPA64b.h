/**
 * FPA64b.h : Class header for the Fixed Point Arithmetic of 64 bits long

 * @Prephix = FPA64b_

 * @Author: Ramón Mollá
 * @version: 2.0
 * @Date:	12/2013
 * Creative Commons: Attribution-NonCommercial 3.0 Unported (CC BY-NC 3.0) 2012-10 Valencia
 *
 * Update:
 * Date:
 * Version:
 * Changes:
 */

#ifndef FPA64b_h
#define FPA64b_h

#include <OS.h>

#define Long64 long long

#ifdef	OS_WINDOWS
	#include <windows.h>
	#include <stdint.h>
	#define FPA64b_MAXVALUE INT64_MAX
	#pragma warning( push )
	#pragma warning( disable : 4244 )	// possible loss of data when converting a double to long long

#elif	OS_LINUX
	#include <limits.h>
	#define FPA64b_MAXVALUE LONG_MAX
#elif	OS_OSX
#elif	OS_ANDROID
#endif


//Integer 32 bits high part and low part respectively
//┌────────────────┬────────────────┐
//│     32 bits Hi │   32 bits Lo	│
//└────────────────┴────────────────┘
//Integer 16 bits high part and low part respectively
//┌────────────────┬────────┬───────┐
//│     32 bits Hi │  16 Hi │ 16 Lo │
//└────────────────┴────────┴───────┘
//Byte high part and low part respectively
//┌────────────────┬────────┬───┬───┐
//│     32 bits Hi │  16 Hi │ Hi│Lo │
//└────────────────┴────────┴───┴───┘
#define FPA64b_HI	1
#define FPA64b_LO	0

class FPA64b_Integer
{
public:
	union
	{
		unsigned char	Shorts	[8];
		unsigned int	Ints	[4];
		unsigned long	Longs	[2];
		Long64			LongLong;
	};

	//Constructor
	FPA64b_Integer(){LongLong = 0LL;};
	FPA64b_Integer(double a) {LongLong = a;};
	FPA64b_Integer(Long64 a) {LongLong = a;};
	FPA64b_Integer(int a)	 {LongLong = a;};
	
	//Assignation
	inline void operator = (FPA64b_Integer a) {LongLong = a.LongLong;};
	inline void operator = (Long64 a)	{LongLong = a;};
	inline void operator = (double a)	{LongLong = a;};
	inline void operator = (int a)		{LongLong = a;};

	//Arithmetic
	inline FPA64b_Integer operator + (FPA64b_Integer a) {FPA64b_Integer aux; aux.LongLong = LongLong + a.LongLong; return aux;};
	inline FPA64b_Integer operator - (FPA64b_Integer a) {FPA64b_Integer aux; aux.LongLong = LongLong - a.LongLong; return aux;};
	
	inline void operator += (FPA64b_Integer a) {LongLong = LongLong + a.LongLong;};
	inline void operator -= (FPA64b_Integer a) {LongLong = LongLong - a.LongLong;};
	
	inline void operator += (float a) {LongLong = LongLong + a;};
	inline void operator *= (float a) {LongLong = LongLong * a;};
	
	inline double operator * (double a) {return double (LongLong)*a;};
	inline FPA64b_Integer operator * (FPA64b_Integer a) {FPA64b_Integer aux; aux.LongLong = LongLong*a.LongLong; return aux;};

	inline FPA64b_Integer operator / (FPA64b_Integer a) {FPA64b_Integer aux; aux.LongLong = LongLong/a.LongLong; return aux;};
	inline FPA64b_Integer operator / (unsigned int a) {FPA64b_Integer aux; aux.LongLong = LongLong/a; return aux;};

	//Logic
	inline bool operator < (FPA64b_Integer a)	{return LongLong < a.LongLong;};
	inline bool operator > (FPA64b_Integer a)	{return LongLong > a.LongLong;};
	inline bool operator <= (FPA64b_Integer a)	{return LongLong <= a.LongLong;};
	inline bool operator >= (FPA64b_Integer a)	{return LongLong >= a.LongLong;};
	inline bool operator == (FPA64b_Integer a)	{return LongLong == a.LongLong;};
	inline bool operator != (FPA64b_Integer a)	{return LongLong != a.LongLong;};
	inline bool operator == (Long64 a)			{return LongLong < a;};

	//32 bits comparison
	//┌────────────────┬────────────────┐
	//│     32 bits Hi │   32 bits Lo	│
	//└────────────────┴────────────────┘
	inline bool LEqHi32	(FPA64b_Integer a)	{return Longs[FPA64b_HI] <= a.Longs[FPA64b_HI];};
	inline bool LEqHi32	(unsigned long a)	{return Longs[FPA64b_HI] <= a;};
	inline bool HEqHi32	(FPA64b_Integer a)	{return Longs[FPA64b_HI] >= a.Longs[FPA64b_HI];};
	inline bool HEqHi32	(unsigned long a)	{return Longs[FPA64b_HI] >= a;};
	inline bool LHi32	(FPA64b_Integer a)	{return Longs[FPA64b_HI] <  a.Longs[FPA64b_HI];};
	inline bool EqHi32	(FPA64b_Integer a)	{return Longs[FPA64b_HI] == a.Longs[FPA64b_HI];};
	inline bool EqHi32	(unsigned long a)	{return Longs[FPA64b_HI] == a;};
	inline bool NEqHi32	(unsigned long a)	{return Longs[FPA64b_HI] != a;};
	
	//16 bits comparison
	//┌────────────────┬────────┬───────┐
	//│     32 bits Hi │  16 Hi │ 16 Lo │
	//└────────────────┴────────┴───────┘
	inline bool LEqHi16	(FPA64b_Integer a)	{return Ints[FPA64b_HI] <= a.Ints[FPA64b_HI];};
	inline bool LEqHi16	(unsigned int a)	{return Ints[FPA64b_HI] <= a;};
	inline bool HEqHi16	(FPA64b_Integer a)	{return Ints[FPA64b_HI] >= a.Ints[FPA64b_HI];};
	inline bool LHi16	(FPA64b_Integer a)	{return Ints[FPA64b_HI] <  a.Ints[FPA64b_HI];};
	inline bool EqHi16	(FPA64b_Integer a)	{return Ints[FPA64b_HI] == a.Ints[FPA64b_HI];};
	
	//8 bits comparison
	//┌────────────────┬────────┬───┬───┐
	//│     32 bits Hi │  16 Hi │ Hi│Lo │
	//└────────────────┴────────┴───┴───┘
	inline bool LEq8	(FPA64b_Integer a, unsigned char Pos)	{return Shorts[Pos] <= a.Shorts[Pos];};
	inline bool LEq8	(unsigned char  a, unsigned char Pos)	{return Shorts[Pos] <= a;};
	inline bool HEq8	(FPA64b_Integer a, unsigned char Pos)	{return Shorts[Pos] >= a.Shorts[Pos];};
	inline bool HEq8	(unsigned char  a, unsigned char Pos)	{return Shorts[Pos] >= a;};
	inline bool L8		(FPA64b_Integer a, unsigned char Pos)	{return Shorts[Pos] <  a.Shorts[Pos];};
	inline bool H8		(FPA64b_Integer a, unsigned char Pos)	{return Shorts[Pos] >  a.Shorts[Pos];};
	inline bool Eq8		(FPA64b_Integer a, unsigned char Pos)	{return Shorts[Pos] == a.Shorts[Pos];};
	inline bool Eq8		(unsigned char  a, unsigned char Pos)	{return Shorts[Pos] == a;};
};


#ifdef	OS_WINDOWS
	#pragma warning( pop ) 
#elif	OS_LINUX
#elif	OS_OSX
#elif	OS_ANDROID
#endif

#endif