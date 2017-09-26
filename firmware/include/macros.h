#ifndef MACROS_H
	#define MACROS_H

	//=================================================================================================
	// Useful macros
	//=================================================================================================
	#define JOIN2(a, b)					a ## b
	#define JOIN3(a, b, c)				a ## b ## c
	#define isnear(x,y,tol)				(((x)>((y)+(tol)))?FALSE:(((x)<((y)-(tol)))?FALSE:TRUE)) //returns true if x and y are near within +-tol
	#define Min( a, b )					( (a) < (b) ? (a) : (b) )
	#define Max( a, b )					( (a) > (b) ? (a) : (b) )
	#define Clamp( low, val, high )		( val = val < (low) ? (low) : ( val > (high) ? (high) : val ) )
	#define	ClampHigh( val, high )		( val = val > (high) ? (high) : val	)
	#define	ClampLow( val, low )		( val = val < (low) ? (low) : val )
	#define InRange( low, val, high )	( (val) < (low) ? FALSE : ( (val) > (high) ? FALSE : TRUE ) )
	#define	ELEMENT_COUNT(s, s_type)	(sizeof(s)/sizeof(s_type))

	#define BYTESWAP(a)	(a[1]+(a[0]<<8))
	#define BIT(x) (1 << (x))
	#define SETBITS(x,y) ((x) |= (y))
	#define CLEARBITS(x,y) ((x) &= (~(y)))
	#define SETBIT(x,y) SETBITS((x), (BIT((y))))
	#define CLEARBIT(x,y) CLEARBITS((x), (BIT((y))))
	#define GETBIT(x,y) ((x) & (BIT(y)))
	#define BITSET(x,y) ((x) & (BIT(y)))
	#define BITCLEAR(x,y) !BITSET((x), (y))
	#define BITSSET(x,y) (((x) & (y)) == (y))
	#define BITSCLEAR(x,y) (((x) & (y)) == 0)
	#define BITVAL(x,y) (((x)>>(y)) & 1)

	#define ONE_MS_DELAY				delay(1)
	#define SECONDS_PER_MINUTE			60
	#define MINUTES_PER_HOUR			60
	#define HOURS_PER_DAY				24
	#define TWENTY_FOUR_HOURS_SECONDS	(SECONDS_PER_MINUTE*MINUTES_PER_HOUR*HOURS_PER_DAY)

#endif
