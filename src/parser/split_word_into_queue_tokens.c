#include "parser.h"
#include "lexer.h"


// TODO ONLY FOR DEBUG
void 	print_token_(t_token **token)
{
	ft_printf("%s\n", (*token)->value);
}

t_deque *split_word_into_queue_tokens(char *word)
{
	t_deque *deque = NULL;
	t_token *token;
	char 	*s;
	char	*str;

	str = NULL;
	s = ft_strdup(word);
	while (s != NULL)
	{
		token = lexer(&s);
		str = (char *)push_back(&deque, token);
		free(str);
	}
//	deque_apply_inplace(deque, &print_token_); // TODO ONLY FOR DEBUG
	return (deque);
}