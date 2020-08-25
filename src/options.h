#ifndef TAPE__OPTIONS_H__INCLUDED_
#define TAPE__OPTIONS_H__INCLUDED_

#if defined(Q_CREATOR_RUN) && !defined(OPTION)
# define OPTION(tag_, chr_, str_, arg_, help_)
#endif /* Q_CREATOR_RUN && !OPTION */

/*     Tag     -x   --xxx     Arg. Help text           */
OPTION(Ascii,  'a',  "ascii",  TAPE_NO_ARG,         "Force ASCII output")
OPTION(Binary, 'b',  "binary", false,               "Force binary output")
OPTION(Color,  'c',  "color",  TAPE_OPTIONAL_ARG,   "Colorize output")
OPTION(Help,   'h',  "help",   0,                   "Show this help text")
OPTION(Out,    'o',  "out",    1,                   "Output file")

/* These are valid although they have the same character because
 * NUL is a special value indicating the lack of a short option.
 */
OPTION(NoChr1, '\0', "nochr1", 1,                   "Has no short option 1")
OPTION(NoChr2, '\0', "nochr2", true,                "Has no short option 2")
OPTION(NoChr3, '\0', "nochr3", TAPE_REQUIRED_ARG,   "Has no short option 3")

/* Will cause build to fail because the tag is already in use.
 */
//OPTION(Out,    'x',  "xout",   0,   "Definitely not --out, how dare you")

/* Will cause build to fail because the short option is already in use.
 */
//OPTION(Other,  'o',  "other",  1,   "Other file")

#endif /* TAPE__OPTIONS_H__INCLUDED_ */
