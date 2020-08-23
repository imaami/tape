#ifndef TAPE__OPTIONS_H__INCLUDED_
#define TAPE__OPTIONS_H__INCLUDED_

#if defined(Q_CREATOR_RUN) && !defined(OPTION)
# define OPTION(tag_, chr_, str_, arg_, help_)
#endif /* Q_CREATOR_RUN && !OPTION */

/*     Tag     -x   --xxx     Arg. Help text           */
OPTION(Ascii,  'a',  "ascii",  0,   "Force ASCII output")
OPTION(Binary, 'b',  "binary", 0,   "Force binary output")
OPTION(Color,  'c',  "color",  2,   "Colorize output")
OPTION(Help,   'h',  "help",   0,   "Show this help text")
OPTION(Out,    'o',  "out",    1,   "Output file")

#endif /* TAPE__OPTIONS_H__INCLUDED_ */
