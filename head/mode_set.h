#ifndef __mode_set_h__
#define __mode_set_h__

#include <stc12c5a60s2.h>

typedef enum
{
	pn0,
	pn1,
	pn2,
	pn3,
	pn4,
	pn5,
	pn6,
	pn7,

}com_select;

typedef enum
{
	general,
	output,
	input,
	opendrain
}mode_select;

typedef enum
{
	p0,
	p1,
	p2,
	p3,
	p4
}m_com_select;

extern void com_set(m_com_select p,com_select pn,mode_select mode);

#endif