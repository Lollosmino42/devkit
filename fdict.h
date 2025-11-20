#pragma once

/* Quality macros that stdio.h should have 
 * to define file opening modes so you don't have
 * to memorize the string values */

#define	F_READ		"r"
#define	F_WRITE		"w"
#define F_APPEND	"a"

#define F_READ_WRITE	"r+"
#define	F_WRITE_READ	"w+"
#define	F_APPEND_READ	"a+"

#define F_READ_B	"rb"
#define	F_WRITE_B	"wb"
#define F_APPEND_B	"ab"

#define F_READ_WRITE_B	"rb+"
#define F_WRITE_READ_B	"wb+"
#define F_APPEND_READ_B	"ab+"

