#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "sys/types.h"
#include "sys/wait.h"


#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define TN 0
#define TP 1
#define TB 2

typedef struct s_list
{
	char		**args;
	int			len;
	int			type;
	int			fd[2];
	struct s_list	*next;
	struct s_list	*prev;
}				t_list;

int	ft_strlen(char *str)
{
	int	i;

	if (str == NULL)
		return (0);
	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	exit_fatal(void)
{
	write(STDERR, "error: fatal\n", 13);
	exit(EXIT_FAILURE);
}

char	*ft_strdup(char *str)
{
	int	i;
	char	*new;

	if (str == NULL)
		return (NULL);
	i = 0;
	new = malloc(sizeof(char) * ft_strlen(str) + 1);
	if (new == NULL)
		exit_fatal();
	while(str[i])
	{
		new[i] = str[i];
		i++;
	}
	new[i] = 0;
	return (new);
}

void	add_argument(t_list **cmd, char *arg)
{
	char	**args;
	int		i;

	args = malloc(sizeof(char *) * ((*cmd)->len + 2));
	i = 0;
	while (i < (*cmd)->len)
	{
		args[i] = (*cmd)->args[i];	
		i++;
	}
	args[i] = ft_strdup(arg);
	args[++i] = 0;
	(*cmd)->len += 1;
	free((*cmd)->args);
	(*cmd)->args = args;
}

void	push_list(t_list **cmd, char *arg)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (new == NULL)
		exit_fatal();
	new->type = TN;
	new->next = NULL;
	new->prev = NULL;
	new->fd[0] = 0;
	new->fd[1] = 1;
	new->len = 0;
	if (*cmd)
	{
		(*cmd)->next = new;
		new->prev = (*cmd);
	}
	(*cmd) = new;
	add_argument(cmd, arg);
}

void	parser(t_list **cmd, char *arg)
{
	int	is_break;

	is_break = (strcmp(";", arg) == 0);
	if (is_break && !(*cmd))
		return ;
	else if (!is_break && (!(*cmd) || ((*cmd)->type > TN)))
		push_list(cmd, arg);
	else if ((*cmd) && (strcmp("|", arg) == 0))
		(*cmd)->type = TP;
	else if (is_break)
		(*cmd)->type = TB;
	else
		add_argument(cmd, arg);
}

void	print_cmd(t_list *cmd)
{
	int	i;

	while (cmd)
	{
		printf("type = %d\n", cmd->type);
		printf("len = %d\n", cmd->len);
		i = -1;
		while(cmd->args[++i])
			printf("%d)%s\n", i, cmd->args[i]);
		cmd=cmd->next;
	}
}

void	rewind_cmd(t_list **cmd)
{
	while ((*cmd) && (*cmd)->prev)
	{
		(*cmd)=(*cmd)->prev;
	}
}

void	exec_cmd(t_list *cmd, char **envp)
{
	pid_t pid;
	int		status;
	int		ret;
	
	if (cmd->type == TP || (cmd->prev && cmd->prev->type == TP))
	{
		if (pipe(cmd->fd))
			return (exit_fatal());
	}
	pid = fork();
	if (pid < 0)
		return (exit_fatal());
	else if (pid == 0)
	{
		if (cmd->type == TP
			&& dup2(cmd->fd[STDOUT], STDOUT) < 0)
			return (exit_fatal());
		if (cmd->prev && cmd->prev->type == TP
			&& dup2(cmd->prev->fd[STDIN], STDIN) < 0)
			return (exit_fatal());
		if ((ret = execve(cmd->args[0], cmd->args, envp)) < 0)
		{
			printf("lol\n");
		}
		exit(ret);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (cmd->type == TP)
			close(cmd->fd[STDIN]);
		if (cmd->prev && cmd->prev->type == TP)
			close(cmd->fd[STDOUT]);
	}
	
}

void	exec_cmds(t_list *cmd, char **env)
{
	t_list *tmp;

	tmp = cmd;
	while (tmp)
	{
		if ((strcmp("cd", tmp->args[0]) == 0))
		{
			chdir(tmp->args[1]);
		}
		else
			exec_cmd(tmp, env);
		tmp = tmp->next;
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_list *cmd;
	int		i;	

	i = 1;
	cmd = NULL;
	if (argc <= 1)
		return (EXIT_SUCCESS);
	while (i < argc)
	{
		parser(&cmd, argv[i]);
		i++;
	}
	rewind_cmd(&cmd);
	print_cmd(cmd);
	exec_cmds(cmd, envp);
}
