#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <termios.h>
#include <term.h>
#include <fcntl.h>
#include "libft.h"
#include "get_next_line.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <signal.h>

#define BUFFSIZE 4096
#define HISTSIZE 1000
#define bool _Bool
#define true 1
#define false 0

struct termios	g_saved_attribute;
char            *g_user_in;
int             g_cur_pos[2];
char            g_flag;
int             g_line_shift;

typedef struct  s_history
{
    char                *str;
    struct s_history    *prev;
    struct s_history    *next;
}               t_history;

t_history       *g_history;

/*
** TOKEN
*/

typedef struct  s_token
{
    int         token_type;
    char        *attribute;
}               t_token;

#define TOKEN t_token

// --------------------------

/* 
** Defines for token
*/

#define NUMBER 0
#define WORD 1
#define ASSIGNMENT_WORD 9
#define PIPE 10
#define LINE_SEPARATOR 11
#define END_LINE 12

/*
** REDIRECTION
** '>' '<' '>>' '<<' '>&' '<&' '&>'
*/

#define GREATER 2
#define LESS 3
#define GREATER_GREATER 4
#define LESS_LESS 5
#define GREATER_AND 6
#define LESS_AND 7
#define AND_GREATOR 8

// --------------------------

/*
** Character key constants
*/

#define LEFT_ARROW 4479771 
#define RIGHT_ARROW 4414235 

#define UP_ARROW  4283163
#define DOWN_ARROW 4348699

#define ALT_LEFT_ARROW 74986827111195
#define ALT_RIGHT_ARROW 73887315483419

#if defined(__APPLE__) || defined(__NetBSD__)

#define ALT_LEFT_ARROW 25115
#define ALT_RIGHT_ARROW 26139

#endif

#ifdef ANTON_UNUSUAL_LAPTOP
#define BACKSPACE 8
#else
#define BACKSPACE 127
#endif

#define DEL 2117294875

#define CTRL_W 23
#define CTRL_Q 17
#define CTRL_R 18
#define CTRL_LEFT 74995417045787
#define CTRL_RIGHT 73895905418011

#define HOME 4738843
#define END  4607771

/* Sometimes 4*/
#define CTRL_D 0
#define CTRL_C 3
#define CTRL_S 19
#define CTRL_Z 26
#define CTRL_V 22

#define PROMPT_LEN 2

int			ft_putint(int c);
char        *readline(int tty_input);
int			ft_isspace(char c);
void        move_cursor(long c, int *cur_pos, char *user_in);
void        clear_line(int cup_pos, int end_cl);
void        delete_symbol(char *user_in, int *cur_pos);
void        insert_symbol(char *user_in, int *cur_pos, char c);
void        alt_left_right(long c, int *cur_pos, char *user_in);
void		home_end(long c, int *cur_pos, char *user_in);
void        delete_last_word(char *user_in, int *cur_pos);
void		wordmove_cursor(long c, int *cur_pos, char *user_in);
char		*input_under_cursor(int cursor_pos, char *user_input);
int			inside_boundaries(int cur_pos, char *user_input);
void		delete_symbol_forward(char *user_in, int *cur_pos);
int         str_n(char *user_in);
void        clear_all_line(char *user_in, int *cur_pos);
void        cur_pos_after_putstr(char *user_in, int *cur_pos);
int         last_cur_pos(char *user_in, int *cur_pos);
int         search_index(char *user_in, int *cur_pos);
void        ret_cur_to_original_pos(int *cur_pos, int *prev_cur_pos);
int			ret_winsize(int a);
int         check_backslash(char *user_in, int start_check);
char        *expansion(char *user_in, char **env);
t_history   *add_new_history(t_history *history, char *str);
void        add_to_start_history(t_history *history, char *str);
t_history   *create_history(char *str);
void        load_on_file_history(t_history *history);
void        save_in_file_history(t_history *history);
void        free_history_list(t_history *history);

TOKEN       get_next_token(char *user_in);
TOKEN       get_token_word(char *user_in, int *index, char *buf,\
    int *buf_index);
bool        is_ws(char c);
TOKEN       get_and_greator(char *user_in, int *index, char *buf,\
    int *buf_index);

/*
** Funcions that make termcaps interactions more readable
*/

void	    tc_cursor_up(int *cur_pos);
void        tc_cursor_down(int *cur_pos);
void		tc_cursor_left(int *cur_pos);
void		tc_cursor_right(int *cur_pos);
void		tc_clear_till_end(void);
