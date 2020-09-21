#ifndef TAPE__EXAMPLE__MODULE_PRIV_H__INCLUDED_
#define TAPE__EXAMPLE__MODULE_PRIV_H__INCLUDED_

#include "tape.h"

#include <limits.h>
#include <stddef.h>
#include <stdbool.h>

__attribute__((__always_inline__, __pure__))
static inline tape_option_tag_t
module_option_tag_max (void)
{
	long max = LONG_MIN;

	#define OPTION(tag_, chr_, str_, arg_, help_) if ((long)tag_ > max) { max = (long)tag_; }
	#include "options_priv.h"

	return (tape_option_tag_t)max;
}

__attribute__((__always_inline__, __pure__))
static inline tape_option_tag_t
module_option_tag_min (void)
{
	long min = LONG_MAX;

	#define OPTION(tag_, chr_, str_, arg_, help_) if ((long)tag_ < min) { min = (long)tag_; }
	#include "options_priv.h"

	return (tape_option_tag_t)min;
}

__attribute__((__unused__, __pure__))
static bool
module_option_tag_value_is_valid (long tag)
{
	const long min = (long)module_option_tag_min();
	const long max = (long)module_option_tag_max();
	return tag >= min && tag <= max;
}

__attribute__((__unused__))
static const char *
module_option_tag_name (tape_option_tag_t tag)
{
	switch (tag) {
	#define OPTION(tag_, chr_, str_, arg_, help_) case tag_: return #tag_;
	#include "options_priv.h"
	default: return NULL;
	}
}

__attribute__((__unused__, __pure__))
static char
module_option_char (tape_option_tag_t tag)
{
	switch (tag) {
	#define OPTION(tag_, chr_, str_, arg_, help_) case tag_: return chr_;
	#include "options_priv.h"
	default: return '\0';
	}
}

__attribute__((__unused__))
static const char *
module_option_string (tape_option_tag_t tag)
{
	switch (tag) {
	#define OPTION(tag_, chr_, str_, arg_, help_) case tag_: return str_;
	#include "options_priv.h"
	default: return NULL;
	}
}

__attribute__((__unused__, __pure__))
static tape_arg_type_t
module_option_arg_type (tape_option_tag_t tag)
{
	switch (tag) {
	#define OPTION(tag_, chr_, str_, arg_, help_) case tag_: return (tape_arg_type_t)(arg_);
	#include "options_priv.h"
	default: return TAPE_NO_ARG;
	}
}

__attribute__((__unused__, __pure__))
static inline bool
module_option_tag_from_char (tape_option_tag_t *tag,
                             char               c)
{
	switch (c) {
	#define OPTION(tag_, chr_, str_, arg_, help_) case chr_: *tag = tag_; break;
	#include "options_priv.h"
	default: return false;
	}
	return true;
}

__attribute__((__unused__))
static const char *
module_option_help (tape_option_tag_t tag)
{
	switch (tag) {
	#define OPTION(tag_, chr_, str_, arg_, help_) case tag_: return help_;
	#include "options_priv.h"
	default: return NULL;
	}
}

#endif /* TAPE__EXAMPLE__MODULE_PRIV_H__INCLUDED_ */
