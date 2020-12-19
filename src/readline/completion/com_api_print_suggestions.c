/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   com_api_print_suggestions.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alexbuyanov <alexbuyanov@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 23:25:58 by alexbuyanov       #+#    #+#             */
/*   Updated: 2020/12/19 17:06:04 by alexbuyanov      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc21sh.h"
#include "gayprompt.h"

void 		com_api_clear_till_end(void)
{
	com_api_move_curs_to_end_line();
	ft_putchar('\n');
	tc_clear_till_end();
	rp(NULL)->competitions_raws = 1;
	com_api_return_curs_to_line();
	com_api_return_curs_to_position(rp(NULL)->column_end_of_line);
	com_api_move_curs_to_prev_pos();
}

void		com_api_print_many_suggestions(t_completion *matches)
{
	com_api_move_curs_to_end_line();
    if (check_big_list(complections_list_len(matches)))
    {
        ft_putchar('\n');
        rp(NULL)->competitions_raws = 2;
	    com_api_return_curs_to_line();
        com_api_return_curs_to_position(rp(NULL)->column_end_of_line);
        com_api_move_curs_to_prev_pos();
        return ;
    }
    ft_putchar('\n');
    com_api_print_lst(matches);
    gayprompt(rp(NULL)->prompt);
    ft_putstr(rp(NULL)->user_in);
    com_api_move_curs_to_prev_pos();
    inverse_search_index(rp(NULL)->cur_pos, rp(NULL)->index, rp(NULL)->prompt_len - 1);
}

void		com_api_print_normal_suggestions(t_completion *matches)
{
	com_api_move_curs_to_end_line();
    ft_putchar('\n');
    com_api_print_lst(matches);
    com_api_return_curs_to_line();
    com_api_return_curs_to_position(rp(NULL)->column_end_of_line);
    com_api_move_curs_to_prev_pos();
}

void		com_api_print_suggestion(t_completion *matches, char *remainder_word,
								char *path)
{
	int				size_of_matches;

	size_of_matches = complections_list_len(matches);
	if (size_of_matches == 1)
		complete_word(matches, remainder_word, path);
	else
	{
		if (size_of_matches > 20)
			com_api_print_many_suggestions(matches);
		else if (size_of_matches > 1)
			com_api_print_normal_suggestions(matches);
	}
}
