#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "module.h"
#include "module_list.h"
#include "dispatcher.h"

static void
dispatcher_fini (dispatcher_t *obj)
{
	fifo_fini(&obj->fifo);
	buf_fini(&obj->buf);

	memset((void *)obj, 0, sizeof(*obj));
}

bool
dispatcher_init (dispatcher_t *obj,
                 const char   *fifo_path)
{
	if (!obj || !fifo_path || !*fifo_path) {
		fprintf(stderr, "%s: invalid arguments\n", __func__);
		return false;
	}

	memset((void *)obj, 0, sizeof(*obj));

	if (!fifo_init(&obj->fifo, fifo_path)) {
		return false;
	}

	if (!buf_init(&obj->buf)) {
		fifo_fini(&obj->fifo);
		return false;
	}

	return true;
}

static int
dispatcher_dispatch (dispatcher_t  *obj __attribute__((__unused__)),
                     int            argc,
                     char         **argv)
{
	const module_t *module;

	if (0 == strcmp((const char *)argv[0], "dispatch")) {
		return EXIT_FAILURE;
	}

	module = module_list_find_builtin(argv[0]);
	if (!module) {
		return EXIT_FAILURE;
	}

	return module_invoke(module, argc, argv);
}

static void
dispatcher_handle_input (dispatcher_t *obj)
{
	buf_t *buf = dispatcher_get_buf(obj);
	uint8_t *d = buf_get_data_ptr(buf);
	size_t len = buf->used;
	size_t i = 0;
	size_t k = 0;
	int argc = 0;
	int c;

	// Count the number of null-terminated arguments
	for (;; ++i) {
		if (i >= len) {
			if (i > k) {
				argc++;
			}
			break;
		}
		if (d[i] == (uint8_t)'\0') {
			argc++;
			k = i + 1u;
		}
	}

	size_t argv_pos, need;

	// Offset for our arg vector to guarantee pointer alignment
	argv_pos = (len + (sizeof(char *) - 1u)) & ~(sizeof(char *) - 1u);

	// Alignment padding between current write offset and arg vector
	i = argv_pos - len;

	// Space needed for arg vector incl. padding and terminator
	need = i + ((size_t)(argc + 1) * sizeof(char *));

	// Reserve the space needed
	buf_reserve(buf, need);

	// Verify
	if (buf_get_free_space(buf) < need) {
		fprintf(stderr, "%s: buffer allocation failed\n", __func__);

	} else {
		// Set write offset to the beginning of the arg vector
		buf_consume(buf, i);

		char **argv = (char **)buf_get_write_ptr(buf);
		d = buf_get_data_ptr(buf);
		i = 0; // byte position
		c = 0; // arg vector index
		k = 0; // arg begin offset

		// Populate the arg vector
		for (;; ++i) {
			if (i >= len) {
				if (i > k) {
					argv[c++] = (char *)&d[k];
				}
				break;
			}
			if (d[i] == (uint8_t)'\0') {
				argv[c++] = (char *)&d[k];
				k = i + 1u;
			}
		}

		// null-terminate
		argv[c] = NULL;

		(void)dispatcher_dispatch(obj, argc, argv);
	}

	buf_release(buf);
}

static void
dispatcher_listen (dispatcher_t *obj)
{
	while (fifo_read(&obj->fifo, &obj->buf)) {
		dispatcher_handle_input(obj);
	}
}

int
dispatcher_run (dispatcher_t *obj)
{
	int err = EXIT_SUCCESS;

	if (!obj) {
		fprintf(stderr, "%s: invalid arguments\n", __func__);
		return EXIT_FAILURE;
	}

	if (!obj->fifo.path) {
		fprintf(stderr, "%s: module is not initialized\n", __func__);
		return EXIT_FAILURE;
	}

	if (fifo_mkfifo(&obj->fifo)) {
		dispatcher_listen(obj);
	} else {
		err = EXIT_FAILURE;
	}

	dispatcher_fini(obj);

	return err;
}
