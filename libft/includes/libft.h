/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 12:50:03 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/01 20:15:26 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <string.h>
# include <sys/types.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}					t_list;

typedef struct s_array
{
	void	**array;
	size_t	len;
	size_t	size;
}	t_array;

int		ft_isalpha(int c);
int		ft_isdigit(int c);
int		ft_isalnum(int c);
int		ft_isascii(int c);
int		ft_isprint(int c);
int		ft_iswhitespace(int c);
int		is_numeric(char *nstr);
size_t	ft_strlen(const char *s);
void	*ft_memset(void *s, int c, size_t n);
void	ft_bzero(void *s, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
size_t	ft_strlcat(char *dest, const char *src, size_t size);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
int		ft_toupper(int c);
int		ft_tolower(int c);
char	*ft_strchr(const char *s, int c);
char	*ft_strrchr(const char *s, int c);
int		ft_strcmp(char *s1, char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
char	*ft_strnstr(const char *big, const char *little, size_t len);
int		ft_atoi(const char *nptr);
double	ft_atod(const char *nptr);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strdup(const char *s);
char	*ft_substr(const char *s, unsigned int start, size_t len);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strjoin_array(char const **str_array, char *sep);
char	*ft_strtrim(char const *s1, char const *set);
char	**ft_split(char const *s, char c);
char	**ft_split_keep_empty(char const *s, char c);
char	**ft_split_by_whitespace(char const *s);
char	*ft_itoa(int n);
char	*ft_dtoa(double number, int precision);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
void	ft_putchar_fd(char c, int fd);
void	ft_putstr_fd(char const *s, int fd);
void	ft_putendl_fd(char const *s, int fd);
void	ft_putnbr_fd(int n, int fd);

t_list	*ft_lstnew(void *content);
void	ft_lstadd_front(t_list **lst, t_list *new);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstdelone(t_list *lst, void (*del)(void*));
void	ft_lstclear(t_list **lst, void (*del)(void*));
void	ft_lstiter(t_list *lst, void (*f)(void*));
t_list	*ft_lstmap(t_list *lst, void *(*f)(void*), void (*del)(void*));
void	ft_lstsort(t_list **lst, int (*cmp)(void *, void *));
t_list	*ft_lstfilter(t_list *lst, int (*filter)(void *));
t_list	*ft_lstget_elem_index(t_list *lst, int index);

char	*get_next_line(int fd);
int		ft_printf(const char *s, ...);

// ft_math
double	ft_pow(double nb, int pow);

// ft_array

t_array	ft_arraynew(void);
void	ft_array_sort(t_array arr, int (*cmp)(void*, void*),
			void (*del)(void*));
void	ft_arrayclear(t_array *arr, void (*del)(void*));
void	ft_arrayadd_back(t_array *arr, void *new, void (*del)(void *));
t_array	ft_arraydup(t_array src, void (*dup)(void**, void*));
ssize_t	ft_arrayget_index(t_array arr, void *elem);
t_array	ft_array_map(t_array arr, void *(*f)(void *), void (*del)(void *));
t_array	ft_array_filter(t_array arr, int (*f)(void *), void (*del)(void *));

// ft_swap

void	ft_cswap(char *a, char *b);
void	ft_dswap(double *a, double *b);
void	ft_fswap(float *a, float *b);
void	ft_iswap(int *a, int *b);
void	ft_memswap(void *a, void *b, size_t size);

#endif
