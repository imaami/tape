/** \brief Forward declarations of various module API types.
 */
#ifndef TAPE__EXAMPLE__MODULE_TYPES_H__INCLUDED_
#define TAPE__EXAMPLE__MODULE_TYPES_H__INCLUDED_

/** Module descriptor type. */
typedef struct module module_t;

/** Module flags type. */
typedef enum module_flag module_flag_t;

/** Module iterator type. */
typedef struct module_iter module_iter_t;

/** Helper type for referencing a module descriptor pointer array. */
typedef const module_t *const module_list_t;

#endif // TAPE__EXAMPLE__MODULE_TYPES_H__INCLUDED_
