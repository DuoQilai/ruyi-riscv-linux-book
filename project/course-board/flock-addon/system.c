/* Minimal flock binding so dsh can lock session files on riscv64.
 * The published addon has no linux-riscv64 build. */
#include <errno.h>
#include <node_api.h>
#include <stdlib.h>
#include <sys/file.h>

typedef struct {
	int fd;
	int err;
	napi_async_work work;
	napi_ref cbref;
} job_t;

static void execute(napi_env env, void *data)
{
	job_t *j = data;
	(void)env;
	if (flock(j->fd, LOCK_EX | LOCK_NB) == 0)
		j->err = 0;
	else
		j->err = errno;
}

static void complete(napi_env env, napi_status status, void *data)
{
	job_t *j = data;
	napi_value cb, global, arg, result;
	(void)status;
	napi_get_reference_value(env, j->cbref, &cb);
	napi_get_global(env, &global);
	napi_create_int32(env, j->err, &arg);
	napi_call_function(env, global, cb, 1, &arg, &result);
	napi_delete_reference(env, j->cbref);
	napi_delete_async_work(env, j->work);
	free(j);
}

static napi_value try_lock(napi_env env, napi_callback_info info)
{
	size_t argc = 2;
	napi_value args[2];
	napi_value name;
	int32_t fd;
	job_t *j;

	napi_get_cb_info(env, info, &argc, args, NULL, NULL);
	napi_get_value_int32(env, args[0], &fd);
	j = calloc(1, sizeof *j);
	j->fd = (int)fd;
	napi_create_reference(env, args[1], 1, &j->cbref);
	napi_create_string_utf8(env, "flock", NAPI_AUTO_LENGTH, &name);
	napi_create_async_work(env, NULL, name, execute, complete, j, &j->work);
	napi_queue_async_work(env, j->work);
	return NULL;
}

static napi_value init(napi_env env, napi_value exports)
{
	napi_value fn;
	napi_create_function(env, "tryLock", NAPI_AUTO_LENGTH, try_lock, NULL, &fn);
	napi_set_named_property(env, exports, "tryLock", fn);
	return exports;
}

NAPI_MODULE(system, init)
