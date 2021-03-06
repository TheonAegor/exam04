#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

enum e_type
{
	TYPE_NONE = 0,
	TYPE_PIPE,
	TYPE_BREAK,
};

enum e_stream
{
	STD_IN = 0,
	STD_OUT = 1,
	STD_ERR = 2,
};

typedef struct s_list
{
	char			**args;
	int				len;
	int				type;
	int				pipes[2];
	struct s_list	*next;	
	struct s_list	*prev;	
	
}				t_list;

int	ft_strlen(char *str)
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
	char	*new;

	i = -1;
	if (str == NULL)
		return (NULL);
	new = malloc(sizeof(char) * ft_strlen(str) + 1);
	while (str[++i])
		new[i] = str[i];
	new[i] = 0;
	return (new);
}

int	add_arguments(t_list **cmd, char *arg)
{
	char **args;
	int		i;

	args = malloc(sizeof(char*) * ((*cmd)->len + 2));
	i = -1;
	while (++i < (*cmd)->len)
		args[i] = (*cmd)->args[i];
	args[i] = ft_strdup(arg);	
	args[++i] = 0;
	free((*cmd)->args);
	(*cmd)->len++;
	(*cmd)->args = args;
	return (EXIT_SUCCESS);	
}

int	push_list(t_list **list, char *arg)
{
	t_list *new;

	new = malloc(sizeof(t_list));
	new->len = 0;
	new->type = TYPE_NONE;
	new->next = NULL;
	new->prev = NULL;
	if ((*list))
	{
		(*list)->next = new;
		new->prev = (*list);
	}
	*list = new;
	return (add_arguments(list, arg));
}

int	print_cmds(t_list *cmd)
{
	int i;

	printf("kdkfj=%s\n", cmd->args[0]);
	while (cmd)
	{
		i = -1;
		printf("type=%d\n", cmd->type);
		while (cmd->args[++i])	
			printf("%d)%s\n", i, cmd->args[i]);
		cmd = cmd->next;
	}
	return (EXIT_SUCCESS);
}

int rewind_lists(t_list **list)
{
	while ((*list)->prev)
	{
		(*list) = (*list)->prev;
	}
	return (EXIT_SUCCESS);
}


int	parcer(t_list **cmd, char *arg)
{
	int	is_break;

	is_break = (strcmp(";", arg) == 0);
	if (is_break && !(*cmd))
		return (EXIT_FAILURE);
	else if (!is_break && (!(*cmd) || ((*cmd)->prev && (*cmd)->prev->type == TYPE_PIPE)))
		push_list(cmd, arg);
	else if (strcmp("|", arg) == 0)
		(*cmd)->type = TYPE_PIPE;
	else if (is_break)
		(*cmd)->type = TYPE_BREAK;
	else if ((*cmd) && (*cmd)->type == TYPE_NONE)
		add_arguments(cmd, arg);
	else
		push_list(cmd, arg);
	return (EXIT_SUCCESS);
}

int	show_error(char *str)
{
	write(STD_ERR, str, ft_strlen(str));
	return (EXIT_SUCCESS);
}

void	exit_fatal(void)
{
	show_error("error: fatal\n");
	exit(EXIT_FAILURE);
}

int	exec_cmd(t_list *cmd, char **env)
{
	pid_t	pid;
	int		pipe_open;
	int		status;

	pipe_open = 0;
	if (cmd->type == TYPE_PIPE || (cmd->prev && cmd->prev->type == TYPE_PIPE))
	{
		pipe_open = 1;
		if(pipe(cmd->pipes))
			exit_fatal();
	}
	pid = fork();
	if (pid == -1)
		exit_fatal();
	else if (pid == 0)
	{
		if (cmd->type == TYPE_PIPE && dup2(cmd->pipes[STD_OUT], STD_OUT) < 0)
			exit_fatal();
		if (cmd->prev && cmd->prev->type == TYPE_PIPE && dup2(cmd->pipes[STD_IN], STD_IN) < 0)
			exit_fatal();
		if (execve(cmd->args[0], cmd->args, env) < 0)
		{
			show_error("error: cannot execute ");
			show_error(cmd->args[0]);
			show_error("\n");
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		waitpid(pid, &status, 0);	
		if (pipe_open)
		{
			close(cmd->pipes[STD_OUT]);
			if (!cmd->next || cmd->type == TYPE_BREAK)
				close(cmd->pipes[STD_IN]);
		}
		if (cmd->prev && cmd->prev->type == TYPE_PIPE)
			close(cmd->pipes[STD_OUT]);
	}
	return (EXIT_SUCCESS);
}

int	exec_cmds(t_list *cmd, char **env)
{
	int	ret;
	
	ret = EXIT_SUCCESS;
	while (cmd)
	{
		if (strcmp("cd", cmd->args[0]) == 0)
		{
			if (cmd->len != 1)
				return (show_error("error: cd: bad arguments\n"));
			if (chdir(cmd->args[1]) == 0)
			{
				show_error("error: cd: cannot change directory to ");
				show_error(cmd->args[1]);
				show_error("\n");
			}
		}
		else
		{
			ret = exec_cmd(cmd, env);
		}
		if (!cmd->next)
			break;
		cmd = cmd->next;
	}
	return (ret);
}

/*
void exit_execve(char *str)
{
	write(STD_ERR, "error: cannot execute ", ft_strlen("error: cannot execute "));
	write(STD_ERR, str, ft_strlen(str));
	write(STD_ERR, "\n", 1);
	exit(EXIT_FAILURE);
}

void exec_cmd(t_list *temp, char **env)
{
	pid_t pid;
	int status;
	int pipe_open;

	pipe_open = 0;
	if (temp->type == TYPE_PIPE || (temp->prev && temp->prev->type == TYPE_PIPE))
	{
		pipe_open = 1;
		if (pipe(temp->pipes))
			exit_fatal();
	}
	pid = fork();
	if (pid < 0)
		exit_fatal();
	else if (pid == 0) //child
	{
		if (temp->type == TYPE_PIPE && dup2(temp->pipes[STD_OUT], STD_OUT) < 0)
			exit_fatal();
		if (temp->prev && temp->prev->type == TYPE_PIPE && dup2(temp->prev->pipes[STD_IN], STD_IN) < 0)
			exit_fatal();
		if ((execve(temp->args[0], temp->args, env)) < 0)
			exit_execve(temp->args[0]);
		exit(EXIT_SUCCESS);
	}
	else //parent
	{
		waitpid(pid, &status, 0);
		if (pipe_open)
		{
			close(temp->pipes[STD_OUT]);
			if (!temp->next || temp->type == TYPE_BREAK)
				close(temp->pipes[STD_IN]);
		}
		if (temp->prev && temp->prev->type == TYPE_PIPE)
			close(temp->prev->pipes[STD_IN]);
	}
}

void exec_cmds(t_list *ptr, char **env)
{
	t_list *temp;

	temp = ptr;
	while (temp)
	{
		if (strcmp("cd", temp->args[0]) == 0)
		{
			if (temp->len < 2)
				show_error("lol");
			else if (chdir(temp->args[1]))
				show_error("lol");
		}
		else
			exec_cmd(temp, env);
		temp = temp->next;
	}
}
*/

int	main(int argc, char **argv, char **envp)
{
	int	i;
	int	ret;
	t_list *cmd;

	i = 0;
	cmd = NULL;
	ret = EXIT_FAILURE;
	if (argc <= 1)
		return (EXIT_SUCCESS);
	while (++i < argc)
	{
		printf("%s\n", argv[i]);
		parcer(&cmd, argv[i]);
	}
	if (cmd)
	{
		rewind_lists(&cmd);
		print_cmds(cmd);
		exec_cmds(cmd, envp);
	}
	return (ret);
}
