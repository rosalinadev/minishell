#include "minishell.h"

bool	loop(t_ctx *ctx);

int	main(void)
{
	t_ctx	ctx;

	ctx = (t_ctx){.exitcode = 42};
	env_init(&ctx.env, environ);
	if (!loop(&ctx))
		return (env_clear(&ctx.env), EXIT_FAILURE);
	return (env_clear(&ctx.env), EXIT_SUCCESS);
}
