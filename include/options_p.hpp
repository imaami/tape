/* Ensure that the public header has already been included. Notice that we
 * can't simply include tape.h here, as it requires that the OPTION macro
 * is undefined whilst this header demands the exact opposite.
 */
#ifndef TAPE__TAPE_H__INCLUDED_
# error "tape.h must be included before this header"
#endif /* TAPE__TAPE_H__INCLUDED_ */

#ifndef OPTION
# error "OPTION macro must be defined"
#endif /* OPTION */

/* Reset header guard to allow repeated inclusion */
#undef TAPE__OPTIONS_H__INCLUDED_

#include "options.h"

#undef OPTION
