#ifndef TAPE__TAPE_H__INCLUDED_
#define TAPE__TAPE_H__INCLUDED_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef OPTION
# error "OPTION macro must not be defined"
#endif /* OPTION */

typedef enum tape_arg_type {
	TAPE_NO_ARG       = 0,
	TAPE_REQUIRED_ARG = 1,
	TAPE_OPTIONAL_ARG = 2
} tape_arg_type_t;

#define OPTION(tag_, chr_, str_, arg_, help_) tag_,

typedef enum tape_option_tag {
#include "options.h"
} tape_option_tag_t;

#undef OPTION

#include <stdbool.h>

extern tape_arg_type_t tape_option_get_arg_type (tape_option_tag_t);
extern char tape_option_get_char (tape_option_tag_t);
extern char const *tape_option_get_string (tape_option_tag_t);
extern char const *tape_option_get_help (tape_option_tag_t);

# ifdef __cplusplus
}
# endif /* __cplusplus */

#endif /* TAPE__TAPE_H__INCLUDED_ */
