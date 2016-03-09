/**
 * FPA64b.h : Class header for the Fixed Point Arithmetic of 64 bits long

 * @Prephix = UGKFPA64b_

 * @Author: Ramón Mollá
 * @version: 2.0
 * @Date:	05/2014
 * Creative Commons: Attribution-NonCommercial 3.0 Unported (CC BY-NC 3.0) 2012-10 Valencia
 *
 * Update:
 * Date:
 * Version:
 * Changes:
 */

#ifndef UGK_FPA64b_h
#define UGK_FPA64b_h

#include <UGK.h>

#define Long64 long long

#ifdef	UGKOS_WINDOWS
	#include <windows.h>
	#include <stdint.h>
	#define UGKFPA64b_MAXVALUE INT64_MAX
	#pragma warning( push )
	#pragma warning( disable : 4244 )	// possible loss of data when converting a double to long long

#elif	UGKOS_LINUX
	#include <limits.h>
	#define UGKFPA64b_MAXVALUE LONG_MAX
#elif	UGKOS_OSX
#elif	UGKOS_ANDROID
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
#define UGKFPA64b_HI	1
#define UGKFPA64b_LO	0
#define UGKFPA64b_ONE	4294967296LL
#define UGKFPA64b_ZERO	0LL

namespace UGK
{
class UGK_API UGKFPA64b
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
	UGKFPA64b()			{LongLong = UGKFPA64b_ZERO;};
	UGKFPA64b(double a) {LongLong = a;};
	UGKFPA64b(Long64 a) {LongLong = a;};
	UGKFPA64b(int a)	{LongLong = a;};
	
	//Assignation
	inline void operator = (UGKFPA64b a)	{LongLong = a.LongLong;};
	inline void operator = (Long64 a)		{LongLong = a;};
	inline void operator = (double a)		{LongLong = a;};
	inline void operator = (int a)			{LongLong = a;};

	//Arithmetic
	inline UGKFPA64b operator + (UGKFPA64b a) {UGKFPA64b aux; aux.LongLong = LongLong + a.LongLong; return aux;};
	inline UGKFPA64b operator - (UGKFPA64b a) {UGKFPA64b aux; aux.LongLong = LongLong - a.LongLong; return aux;};
	
	inline void operator += (UGKFPA64b a) {LongLong = LongLong + a.LongLong;};
	inline void operator -= (UGKFPA64b a) {LongLong = LongLong - a.LongLong;};
	
	inline void operator += (float a) {LongLong = LongLong + a;};
	inline void operator *= (float a) {LongLong = LongLong * a;};
	
	inline double operator		* (double a)	{return double (LongLong)*a;};
	inline UGKFPA64b operator	* (UGKFPA64b a)	{UGKFPA64b aux; aux.LongLong = LongLong*a.LongLong; return aux;};

	inline UGKFPA64b operator / (UGKFPA64b a)	 {UGKFPA64b aux; aux.LongLong = LongLong/a.LongLong; return aux;};
	inline UGKFPA64b operator / (unsigned int a) {UGKFPA64b aux; aux.LongLong = LongLong/a; return aux;};

	//Logic
	inline bool operator < (UGKFPA64b a)	{return LongLong < a.LongLong;};
	inline bool operator > (UGKFPA64b a)	{return LongLong > a.LongLong;};
	inline bool operator <= (UGKFPA64b a)	{return LongLong <= a.LongLong;};
	inline bool operator >= (UGKFPA64b a)	{return LongLong >= a.LongLong;};
	inline bool operator == (UGKFPA64b a)	{return LongLong == a.LongLong;};
	inline bool operator != (UGKFPA64b a)	{return LongLong != a.LongLong;};
	inline bool operator == (Long64 a)		{return LongLong < a;};

	//32 bits comparison
	//┌────────────────┬────────────────┐
	//│     32 bits Hi │   32 bits Lo	│
	//└────────────────┴────────────────┘
	inline bool LEqHi32	(UGKFPA64b a)		{return Longs[UGKFPA64b_HI] <= a.Longs[UGKFPA64b_HI];};
	inline bool LEqHi32	(unsigned long a)	{return Longs[UGKFPA64b_HI] <= a;};
	inline bool HEqHi32	(UGKFPA64b a)		{return Longs[UGKFPA64b_HI] >= a.Longs[UGKFPA64b_HI];};
	inline bool HEqHi32	(unsigned long a)	{return Longs[UGKFPA64b_HI] >= a;};
	inline bool LHi32	(UGKFPA64b a)		{return Longs[UGKFPA64b_HI] <  a.Longs[UGKFPA64b_HI];};
	inline bool EqHi32	(UGKFPA64b a)		{return Longs[UGKFPA64b_HI] == a.Longs[UGKFPA64b_HI];};
	inline bool EqHi32	(unsigned long a)	{return Longs[UGKFPA64b_HI] == a;};
	inline bool NEqHi32	(unsigned long a)	{return Longs[UGKFPA64b_HI] != a;};
	
	//16 bits comparison
	//┌────────────────┬────────┬───────┐
	//│     32 bits Hi │  16 Hi │ 16 Lo │
	//└────────────────┴────────┴───────┘
	inline bool LEqHi16	(UGKFPA64b a)		{return Ints[UGKFPA64b_HI] <= a.Ints[UGKFPA64b_HI];};
	inline bool LEqHi16	(unsigned int a)	{return Ints[UGKFPA64b_HI] <= a;};
	inline bool HEqHi16	(UGKFPA64b a)		{return Ints[UGKFPA64b_HI] >= a.Ints[UGKFPA64b_HI];};
	inline bool LHi16	(UGKFPA64b a)		{return Ints[UGKFPA64b_HI] <  a.Ints[UGKFPA64b_HI];};
	inline bool EqHi16	(UGKFPA64b a)		{return Ints[UGKFPA64b_HI] == a.Ints[UGKFPA64b_HI];};
	
	//8 bits comparison
	//┌────────────────┬────────┬───┬───┐
	//│     32 bits Hi │  16 Hi │ Hi│Lo │
	//└────────────────┴────────┴───┴───┘
	inline bool LEq8	(UGKFPA64b a, unsigned char Pos)		{return Shorts[Pos] <= a.Shorts[Pos];};
	inline bool LEq8	(unsigned char  a, unsigned char Pos)	{return Shorts[Pos] <= a;};
	inline bool HEq8	(UGKFPA64b a, unsigned char Pos)		{return Shorts[Pos] >= a.Shorts[Pos];};
	inline bool HEq8	(unsigned char  a, unsigned char Pos)	{return Shorts[Pos] >= a;};
	inline bool L8		(UGKFPA64b a, unsigned char Pos)		{return Shorts[Pos] <  a.Shorts[Pos];};
	inline bool H8		(UGKFPA64b a, unsigned char Pos)		{return Shorts[Pos] >  a.Shorts[Pos];};
	inline bool Eq8		(UGKFPA64b a, unsigned char Pos)		{return Shorts[Pos] == a.Shorts[Pos];};
	inline bool Eq8		(unsigned char  a, unsigned char Pos)	{return Shorts[Pos] == a;};
};
} //namespace UGK

#ifdef	UGKOS_WINDOWS
	#pragma warning( pop ) 
#elif	UGKOS_LINUX
#elif	UGKOS_OSX
#elif	UGKOS_ANDROID
#endif

#endif