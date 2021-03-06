/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_io_redirect.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsance <jsance@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 22:09:19 by jsance            #+#    #+#             */
/*   Updated: 2021/02/17 22:09:20 by jsance           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_redirect	*init_redirects(void)
{
	t_redirect	*fresh;

	fresh = (t_redirect*)ft_memalloc(sizeof(t_redirect));
	if (!fresh)
		return (NULL);
	fresh->redirector = (t_redirector*)ft_memalloc(sizeof(t_redirector));
	fresh->redirector->need_quote_rm = 1;
	return (fresh);
}

t_redirect	*create_tail_redirects(t_redirect **head_redirects)
{
	if (*head_redirects == NULL)
	{
		*head_redirects = init_redirects();
		return (*head_redirects);
	}
	else
		return (create_tail_redirects(&(*head_redirects)->next));
}

int			need_set_default_fd(int action)
{
	static int	is_default_fd_needed;

	if (action == NEED_DEFAULT_FD)
		is_default_fd_needed = 1;
	else if (action == NOT_NEED_DEFAULT)
		is_default_fd_needed = 0;
	return (is_default_fd_needed);
}

/*
** [IO_NUMBER]? (io_file | DLESS WORD)
*/

int			match_io_redirect(t_redirect **head_redirects, t_deque **tokbuf_g)
{
	t_deque		*tokbuf_l;
	t_redirect	*tail_redir;

	tokbuf_l = NULL;
	tail_redir = create_tail_redirects(head_redirects);
	if (gett(g_parser_input_str, tokbuf_g, &tokbuf_l)->tk_type != IO_NUMBER)
	{
		ungett(tokbuf_g, &tokbuf_l);
		need_set_default_fd(NEED_DEFAULT_FD);
	}
	else
	{
		tail_redir->redirector->fd = ft_atoi(deque_n_th(tokbuf_l, 0)->value);
		need_set_default_fd(NOT_NEED_DEFAULT);
	}
	if (match_io_file(&tail_redir, tokbuf_g) == PARSER_SUCCES
		|| match_dless_word(&tail_redir, tokbuf_g) == PARSER_SUCCES)
	{
		erase_tokbuf(&tokbuf_l);
		return (PARSER_SUCCES);
	}
	return (return_err_and_flush_tokens(tokbuf_g, &tokbuf_l));
}
