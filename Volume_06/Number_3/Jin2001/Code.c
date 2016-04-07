
/******************************************************************/
/*                                                                */ 
/*  Convolution surface field computation for line-skeleton with  */ 
/*  polynomial weight distributions using Cauchy kernel           */
/*                                                                */
/*                      By Xiaogang Jin, January 2001             */
/*                                                                */
/******************************************************************/

#include   <math.h>

typedef struct  {
	float   b[3] ; /* base vector */
	float   a[3] ; /* normalized axis */
	float   l ; /* length  */
	float   s ; /* s is used to control the width of the Cauchy kernel */
	float   q0, q1, q2, q3 ; /* the control points of Bezier profile curve */
} LINE ;

/* 
 *  This function calculates the field of point (px,py,pz) 
 *  generated by skeleton *line 
 */
float KernelCauchy(float px, float py,  float pz, LINE *line)
{
	float q0, q1, q2, q3 ;
	float d[3], h, s, l, p ;
	float d2, pp, qq, ww, h2, s2, ts2, ts4, term ; /* ts2: 2.0*s^2; ts4: 2.0*s^4 */
	float lmh, sdp, tpp, sp ; /* lms: l minus s; sdp: s divide by p; tpp: 2p^2 */
	float Fline1, Flinet, Flinet2, Flinet3 ;
	float p0, p1, p2, p3 ;

	q0 = line->q0 ; q1 = line->q1 ; q2 = line->q2 ; q3 = line->q3 ;
	s  = line->s ; l = line->l;

	d[0] = px - line->b[0] ;
	d[1] = py - line->b[1] ;
	d[2] = pz - line->b[2] ;  /* 3 "-" */

	d2 = d[0]*d[0] + d[1]*d[1] + d[2]*d[2] ; /* 3 "*", 2"+" */
	h  = d[0]*line->a[0] + d[1]*line->a[1] + d[2]*line->a[2] ; /* 3 "*", 2"+" */

	s2 = s*s ; h2 = h*h ; lmh = l - h ;  /* 2 "*", 1 "-" */
	pp = 1.0 + s2*(d2 - h2) ; /* 1 "*', 1 "+", 1"-" */
	ww = pp + s2*h2 ; qq = pp + s2*lmh*lmh ; /* 3 "*", 2 "+" */
	p = sqrt(pp) ; /* 1 sqrt */
	sdp = s/p ; /* 1 "/" */
	tpp = 2.0*pp, sp = s*p ; /* 2 "*" */
	term = atan(sdp*h)+atan(sdp*lmh) ; /* 2 atan, 2 "*", 1 "+" */

	Fline1  = (h/ww +lmh/qq)/tpp + term/(tpp*sp) ; /* 1 "*", 4 "/", 2 "+" */
        /* By now, the total operations are: 2 atan, 1 sqrt, 17 "*", 5 "/", 10 "+" , 5 "-" */

	ts2 = 2.0 * s2 ; 
	Flinet  = h*Fline1 + (1.0/ww - 1.0/qq) / ts2 ; /* 2 "*", 3 "/", 1 "+", 1 "-" */		
	Flinet2 = 2.0*h*Flinet - (h2+pp/s2)*Fline1 + term/(s2*sp) ; /* 4 "*", 2 "/", 2 "+", 1 "-" */
	ts4= ts2*s2 ; 
	Flinet3 = 3.0*h*Flinet2-3.0*h2*Flinet+h2*h*Fline1+log(qq/ww)/ts4-pp*(1.0/ww-1.0/qq)/ts4 ;
        /* 1 ln, 8 "*", 5 "/", 2 "+", 3 "-" */

	p0 = q0 ; 
	p1 = (-3.0*q0+3.0*q1) / l ;
	p2 = (3.0*q0-6.0*q1+3.0*q2) / (l*l) ;
	p3 = (-q0+3.0*q1-3.0*q2+q3) / (l*l*l) ;

	return( p0*Fline1 + p1*Flinet + p2*Flinet2 + p3*Flinet3 ) ;
}
