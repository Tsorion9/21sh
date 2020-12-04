/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mphobos <mphobos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/24 20:15:38 by anton             #+#    #+#             */
/*   Updated: 2020/12/01 22:55:39 by mphobos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc21sh.h"
#include "fuck_norme_lexer_state.h"
#include "gayprompt.h"
#include "lex.h"

int			ret_winsize(int a)
{
	struct winsize	w;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	if (!a)
		return (w.ws_col);
	return (w.ws_row);
}

int			fuck_checklist_signal_state(int need_update, int new_value)
{
	static int	signal_arrived;

	if (need_update)
		signal_arrived = new_value;
	return (signal_arrived);
}

static void	processing_sigint(int signal_code)
{
	int	user_in_lines;

	(void)signal_code;
	if (have_children_global_request(0, 0))
		write(2, "\n", 1);
	if (rp(NULL)->in_read)
	{
		user_in_lines = str_n(rp(NULL)->prompt_len) - rp(NULL)->cur_pos[1] + 2;
		while (user_in_lines-- > 0)
			write(STDERR_FILENO, "\n", 1);
		ft_memdel((void **)&(rp(NULL)->user_in));
		gayprompt(get_prompt(PS1));
		reset_rp_to_start(get_prompt(PS1));
		fuck_checklist_signal_state(1, 1);
		fuck_norme_lexer_state(1, NULL, NULL, NULL);
		global_newline_erased(1, 0);
	}
	return ;
}

static void	processing_sigwinch(int signal_code)
{
	int	tmp_cur_pos[2];
	int	index;

	(void)signal_code;
	index = search_index(rp(NULL)->cur_pos, rp(NULL)->prompt_len);
	clear_all_line(rp(NULL)->prompt_len);
	rp(NULL)->ws_col = ret_winsize(0);
	rp(NULL)->ws_row = ret_winsize(1);
	clear_all_line(rp(NULL)->prompt_len);
	ft_putstr_fd(rp(NULL)->user_in, STDERR_FILENO);
	inverse_search_index(rp(NULL)->cur_pos, index, rp(NULL)->prompt_len);
	cur_pos_after_putstr(tmp_cur_pos, rp(NULL)->prompt_len);
	move_cursor_to_new_position(tmp_cur_pos, rp(NULL)->cur_pos);
}

void		set_signal(void)
{
	if (isatty(STDIN_FILENO))
	{
		signal(SIGINT, processing_sigint);
		signal(SIGWINCH, processing_sigwinch);
		signal(SIGTSTP, SIG_IGN);
	}
}
