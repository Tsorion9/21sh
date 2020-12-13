/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mphobos <mphobos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/23 18:13:42 by mphobos           #+#    #+#             */
/*   Updated: 2020/12/06 11:15:39 by mphobos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "readline.h"
#include "gayprompt.h"

int			is_print(long c)
{
	if (c >= ' ' && c <= '~')
		return (1);
	return (0);
}

static void	handle_click_sup(long c)
{
	if (!now_search_history())
	{
		if (c == CTRL_LEFT || c == CTRL_RIGHT)
			wordmove_cursor(c);
		else if (c == CTRL_UP || c == CTRL_DOWN)
			strmove_cursor(c);
		else if (c == CTRL_W)
			delete_last_word();
		else if (c == TAB_ARROW)
			completion();
		else if (c == SHIFT_TAB)
			add_symbol(' ');
	}
}

static long	handle_click(void)
{
	long	c;

	c = 0;
	rp(NULL)->in_read = 1;
	read(STDIN_FILENO, &c, sizeof(long));
	rp(NULL)->in_read = 0;

	if (c == CTRL_R && !rp(NULL)->history_search.history_search_mode)
		set_history_search_mode();
	if (now_search_history())
		history_search_start(c);
	if (!now_search_history())
	{
		if (c == LEFT_ARROW || c == RIGHT_ARROW)
			move_cursor(c);
		else if (c == UP_ARROW || c == DOWN_ARROW)
			up_down_arrow(c);
		else if (c == BACKSPACE)
			delete_symbol();
		else if (c == DEL)
			delete_symbol_forward();
		else if (is_print(c))
			add_symbol(c);
		else if (c == ALT_LEFT_ARROW || c == ALT_RIGHT_ARROW)
			alt_left_right(c);
		else if (c == HOME || c == END)
			home_end(c);
		else if (c == CTRL_D && rp(NULL)->len == 0)
			return (-1);
	}
	else
		handle_click_sup(c);
	return (c);
}

static void	read_till_newline(int *user_in_len)
{
	long	c;

	c = 0;
	while (c != '\n' && c != -1)
		c = handle_click();
	*user_in_len = ft_strlen(rp(NULL)->user_in);
	rp(NULL)->user_in[*user_in_len] = (char)(c == -1 ? 0 : '\n');
	rp(NULL)->user_in[*user_in_len + 1] = 0;
}

/*
** Позволяет корректно работать со строкой ввода.
** Возвращает строку, введенную пользователем.
*/

char		*readline(char *prompt)
{
	char	*ret_user_in;
	int		user_in_len;
	int		user_in_lines;

	gayprompt(prompt);
	reset_rp_to_start(prompt);
	read_till_newline(&user_in_len);
	user_in_lines = str_n(rp(NULL)->prompt_len) - rp(NULL)->cur_pos[1];
	while (user_in_lines-- > 0)
		write(STDERR_FILENO, "\n", 1);
	write(STDERR_FILENO, "\n", 1);
	if (!(ret_user_in = ft_strdup(rp(NULL)->user_in)))
		reset_exit(1);
	ft_memdel((void **)&(rp(NULL)->prompt));
	ft_memdel((void **)&(rp(NULL)->user_in));
	rp(NULL)->in_readline = 0;
	return (ret_user_in);
}
