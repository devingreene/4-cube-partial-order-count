#include"headers.h"

#define MAG 8

#define SPLIT_UP \
	a = 0xff000000&graph; \
	b = 0x00ff0000&graph; \
	c = 0x0000ff00&graph; \
	graph &= 0x000000ff


/* We encode four cube graphs with unsigned 32-bit integers.  This function
   performs symmetry operations on the four-cube at the code level */

static inline ui effof01(ui n,ui shift){
	n >>= shift;
	ui a,b;
	a=n&0x44;
	b=n&0x22;
	n -= a+b;
	n += (a>>1) + (b<<1);
	return n<<shift;
}

static inline ui effof210(ui n,ui shift,ui inv){
	n >>= shift;
	ui a,b,c,d,e,f;
	a = 0x2&n;
	b = 0x10&n;
	c = 0x4&n;
	d = 0x8&n;
	e = 0x20&n;
	f = 0x40&n;
	n -= a+b+c+d+e+f;
	if(inv) n += (a<<1) + (c<<2) + (b>>3) +
		(d<<3) + (f>>1) + (e>>2);
	else n += (a << 3) + (b >> 2) + (c >> 1) +
		(d << 2) + (e << 1) + ( f >> 3);
	return n << shift;
}

ui symmetry(ui graph,ui pflip,ui cycle,ui tau,ui xorop){
	ui a,b,c;
	SPLIT_UP;
	switch(pflip){
		case 1:
			a = effof01(a,24);
			b = effof01(b,16);
			c = effof210(effof01(effof210(c,8,0),8),8,1);
			graph = effof210(effof01(effof210(graph,0,0),0),0,1);
			graph <<= 8;
			graph += (a>>8) + (b<<8) + (c>>8);
			break;
		case 2:
			a = effof210(a,24,1);
			b = effof210(b,16,1);
			c = effof210(c,8,0);
			graph = effof210(graph,0,0);
			graph <<= 16; 
			graph += (a>>16) + (b>>16) + (c<<16);
			break;
		case 3:
			a = effof210(effof01(effof210(a,24,1),24),24,0);
			b = effof210(effof01(effof210(b,16,1),16),16,0);
			c = effof210(effof01(effof210(c,8,1),8),8,0);
			graph = effof210(effof01(effof210(graph,0,1),0),0,0);
			graph <<= 24;
			graph += (a>>24) + (b>>8) + (c<<8);
			break;
		default:
			graph += a + b + c;
			break;
	}

	SPLIT_UP;
	switch(cycle){
		case 1:
			a=effof210(a,24,0);
			b=effof01(b,16);
			c=effof01(c,8);
			graph <<= 16;
			graph += a + (b>>8) + (c>>8);
			break;
		case 2:
			a=effof210(a,24,1);
			c=effof01(c,8);
			graph=effof01(graph,0);
			graph <<= 8;
			graph += a + (b>>16) + (c<<8);
			break;
		default:
			graph += a + b + c;
	}	

	SPLIT_UP;
	if(tau){
		c=effof210(effof01(effof210(c,8,0),8),8,1);	
		graph = effof210(effof01(effof210(graph,0,0),0),0,1);
		graph += c + (a>>8) + (b<<8);
	}
	else graph += a + b + c;

	ui d;
	if(xorop&0x1){
		SPLIT_UP;
		a >>= 24;
		d = a&0x55;
		a -= d;
		a = (a>>1) + (d<<1);
		a <<= 24;
		b >>= 16;
		d = b&0x55;
		b -= d;
		b = (b>>1) + (d<<1);
		b <<= 16;
		c >>= 8;
		d = c&0x55;
		c -= d;
		c = (c>>1) + (d<<1);
		c <<= 8;
		graph ^= 0xff;
		graph += a + b + c;
	}
	if(xorop&0x2){
		SPLIT_UP;
		a >>= 24;
		d = a&0x33;
		a -= d;
		a = (a>>2) + (d<<2);
		a <<= 24;
		b >>= 16;
		d = b&0x33;
		b -= d;
		b = (b>>2) + (d<<2);
		b <<= 16;
		c ^= 0xff00;
		d = graph&0x55;
		graph -= d;
		graph = (graph>>1) + (d<<1);
		graph += a + b + c;
	}
	if(xorop&0x4){
		SPLIT_UP;
		a >>= 24;
		d = a&0x0f;
		a -= d;
		a = (a>>4) + (d<<4);
		a <<= 24;
		b ^= 0x00ff0000;
		c >>= 8;
		d = c&0x33;
		c -= d;
		c = (c >> 2) + (d << 2);
		c <<= 8;
		d = graph&0x33;
		graph -= d;
		graph = (graph >> 2) + (d << 2);
		graph += a + b + c;
	}
	if(xorop&0x8){
		SPLIT_UP;
		a ^= 0xff000000;
		b >>= 16;
		d = b&0x0f;
		b -= d;
		b = (b>>4) + (d<<4);
		b <<= 16;
		c >>= 8;
		d = c&0x0f;
		c -= d;
		c = (c>>4) + (d<<4);
		c <<= 8;
		d = graph&0x0f;
		graph -= d;
		graph = (graph >> 4) + (d<<4);
		graph += a + b + c;
	}
	return graph;
}

/* static int _main(void) __attribute__((unused));
static int _main() {
	printf("%08x\n",symmetry(0x02000000,0,0,0,4));
	return 0;
}
*/
