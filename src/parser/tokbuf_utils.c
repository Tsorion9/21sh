#include "deque.h"
#include "parser.h"

/*
** Put the token from gloabl queue to local queue
** return it's value
*/

t_token	*gett(t_deque **tokbuf_g, t_deque **tokbuf_l)
{
	t_token	*next;

	if (!tokbuf_g || !*tokbuf_g || !(*tokbuf_g)->first)
		next = lex();
	else
		next = pop_front(*tokbuf_g);
	push_back(tokbuf_l, next);
	return (next);
}

/*
** Put the last read token back into the global buffer
*/

void	ungett(t_deque **tokbuf_g, t_deque **tokbuf_l)
{
	t_token	*last;

	last = pop_back(*tokbuf_l);
	if (!last)
		return ;
	push_front(tokbuf_g, last);
}

void	del_token(void *token)
{
	free(((t_token *)token)->attribute);
	free(token);
}

void	erase_tokbuf(t_deque **tokbuf)
{
	deque_del(tokbuf, del_token);
}

/*
** Put unknown quantity of tokens back tho the global buffer
** TODO: We should do it in constant time
*/

void	flush_tokbuf(t_deque **tokbuf_g, t_deque **tokbuf_l)
{
	if (!*tokbuf_l)
		return ;
	while ((*tokbuf_l)->first)
		ungett(tokbuf_g, tokbuf_l);
}