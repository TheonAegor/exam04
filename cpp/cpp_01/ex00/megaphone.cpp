#include <iostream>

using namespace std;

int	main(int argc, char **argv)
{
	int	i;
	int	k;

	i = 0;
	if (argc <= 1)
	{
		cout << "* LOUD AND UNBEARABLE FEEDBACK NOISE *";	
		return (0);
	}
	else
	{
		while (++i < argc)
		{
			k = -1;
			while (argv[i][++k])
				cout << (char)toupper(argv[i][k]);
		}
		return (0);
	}
}
