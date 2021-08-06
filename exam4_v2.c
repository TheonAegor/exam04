#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>

#define TYPE_NONE 0
#define TYPE_PIPE 1
#define TYPE_BREAK 2

#define STDIN 0
#define STDOUT 1
#define STDERR 2 

typedef struct s_list
{
	char	**args;
	int		len;
	int		type;
	int		pipes[2];
	struct s_list *next;
	struct s_list *prev;
}				t_list;

int		ft_strlen(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		;
	return (i);
}

char	*ft_strdup(char *str)
{
	int		i;
	char	*res;

	if (!str)
		return (NULL);
	i = -1;	
	res = malloc(sizeof(char *) * ft_strlen(str) + 1);	
	while (str[++i])
		res[i] = str[i];
	res[i] = 0;
	return (res);
}

int	add_arguments(t_list **cmds, char *arg)
{
	char	**tmp;
	int		i;

	tmp = malloc(sizeof(char *) * ((*cmds)->len + 2));
	i = 0;
	while (i < (*cmds)->len)
	{
		tmp[i] = (*cmds)->args[i];
		i++;
	}
	if ((*cmds)->args)
		free((*cmds)->args);
	(*cmds)->args = tmp;
	(*cmds)->args[i] = ft_strdup(arg);
	(*cmds)->args[++i] = 0;
	(*cmds)->len++;
	return (EXIT_SUCCESS);
}

int	push_list(t_list **cmds, char *arg)
{
	t_list *new;

	new = malloc(sizeof(*new));
	new->len = 0;
	new->type = 0;
	new->next = NULL;
	new->prev = NULL;
	new->type = TYPE_NONE;
	if (*cmds)
	{
		(*cmds)->next = new;
		new->prev = (*cmds);
	}
	*cmds = new;
	add_arguments(cmds, arg);
	return (EXIT_SUCCESS);
}

int	parser(t_list **cmds, char *arg)
{
	int	is_break;

	is_break = (strcmp(";", arg) == 0);
	if (is_break && !(*cmds))
		return (EXIT_FAILURE);
	else if ((*cmds) && ((*cmds)->type == TYPE_BREAK || (*cmds)->type == TYPE_PIPE))
		push_list(cmds, arg);
	else if (is_break)
		(*cmds)->type == TYPE_BREAK;
	else if (strcmp("|", arg) == 0)
		(*cmds)->type = TYPE_PIPE;
	else if ((*cmds) && (*cmds)->type == TYPE_NONE)
		add_arguments(cmds, arg);
	else
		push_list(cmds, arg);
}

int	rewind_lists(t_list **cmds)
{
	while ((*cmds)->prev)
		(*cmds) = (*cmds)->prev;
	return (EXIT_SUCCESS);
}

int	print_chr_arr(char **strs, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		printf("%d)%s\n",i, strs[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	print_cmds(t_list *cmd)
{
	while (cmd)
	{
		printf("type = %d\n",cmd->type);
		print_chr_arr(cmd->args, cmd->len);
		cmd = cmd->next;
	}
	return (EXIT_SUCCESS);
}

int	show_error(char *err)
{
	if (err)
		write(STDERR, err, ft_strlen(err));
	return (EXIT_SUCCESS);
}

int	exit_fatal(void)
{
	show_error("error: fatal\n");
	exit(EXIT_FAILURE);
	return (EXIT_FAILURE);
}

int	exec_cmd(t_list *cmd, char **envp)
{
	int	pipe_open;
	int	status;
	int	ret;
	pid_t	pid;

	pipe_open = 0;
	ret = EXIT_FAILURE;
	if (cmd->type == TYPE_PIPE || (cmd->prev && cmd->prev->type == TYPE_PIPE))
	{
		pipe_open = 1;
		pipe(cmd->pipes);
	}
	pid = fork();
	if (pid < 0)
		return (exit_fatal());
	else if (pid == 0)
	{
		if (cmd->type == TYPE_PIPE && dup2(cmd->pipes[STDOUT], STDOUT))
			return (exit_fatal());
		if (cmd->type == TYPE_PIPE && dup2(cmd->pipes[STDIN], STDIN))
			return (exit_fatal());
		if (execve(cmd->args[0], cmd->args, envp))
		{
			show_error("error: cannot execute ");
			show_error(cmd->args[0]);
			show_error("\n");
		}
	}
	else	
	{
		waitpid(pid, &status, 0);
		if (cmd && cmd->type == TYPE_PIPE)
			close(cmd->pipes[STDOUT]);
		if (cmd->prev && cmd->prev->type == TYPE_PIPE)
			close(cmd->pipes[STDIN]);
		if (WIFEXITED(status))
			ret = WEXITSTATUS(status);
	}
	return (ret);
}

int	exec_cmds(t_list *cmds, char **envp)
{
	int	ret;

	ret = EXIT_SUCCESS;
	while (cmds)
	{
		if (strcmp("cd", cmds->args[0]) == 0)
		{
			if (cmds->len != 2)
				ret = show_error("error: cd: bad arguments\n");
			if (chdir(cmds->args[1]))
			{
				ret = show_error("error: cd: cannot change directory to ");
				show_error(cmds->args[1]);
				show_error("\n");
			}
		}
		else
		{
			ret = exec_cmd(cmds, envp);
		}
		cmds = cmds->next;
	}
	return (ret);
}

int	clear_cmds(t_list **cmds)
{
	int	i;
	t_list	*tmp;

	while ((*cmds))
	{
		tmp = *cmds;
		i = 0;
		while ((*cmds)->args[i])
		{
			free((*cmds)->args[i]);
			i++;
		}
		free((*cmds)->args);
		(*cmds) = (*cmds)->next;
		free(tmp);
	}
}

int main(int argc, char **argv, char **envp)
{
	t_list *cmds;
	int		i;

	i = 0;
	cmds = NULL;
	while (++i < argc)
		parser(&cmds, argv[i]);
	if (argc == 1)
		return (EXIT_SUCCESS);
	rewind_lists(&cmds);
//	print_cmds(cmds);
	if (cmds)
		exec_cmds(cmds, envp);
	clear_cmds(&cmds);
}