/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_split_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/11 19:16:49 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/11 21:16:23 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdbool.h>
#include "libft.h"

static size_t	count_split(char *str, char const *sep)
{
	bool	is_quoted;
	size_t	i;
	size_t	count;

	is_quoted = false;
	i = 0;
	count = 1;
	if (!str || !str[0])
		return (0);
	while (str[i])
	{
		if (str[i] == '\'')
			is_quoted = !is_quoted;
		if (ft_strchr(sep, str[i]) && !is_quoted)
		{
			count++;
			while (ft_strchr(sep, str[i]))
				i++;
			continue ;
		}
		i++;
	}
	return (count);
}

static char	*substr_noquote(char *str, size_t len)
{
	size_t	i;
	size_t	new_i;
	char	*sub;

	i = 0;
	new_i = 0;
	sub = ft_calloc(len + 1, sizeof(char));
	if (!sub)
		return (NULL);
	while (i < len)
	{
		if (str[i] != '\'')
		{
			sub[new_i] = str[i];
			new_i++;
		}
		i++;
	}
	return (sub);
}

static bool	populate_split(char **split, char *str, char const *sep,
	size_t count)
{
	bool	is_quoted;
	size_t	i;
	size_t	len;

	i = 0;
	is_quoted = false;
	while (i < count && *str)
	{
		len = 0;
		while (str[len] && (!ft_strchr(sep, str[len]) || is_quoted))
		{
			if (str[len] == '\'')
				is_quoted = !is_quoted;
			len++;
		}
		split[i] = substr_noquote(str, len);
		if (!split[i])
			return (false);
		str += len;
		while (*str && ft_strchr(sep, *str))
			str++;
		i++;
	}
	return (true);
}

static void	free_split_str(char ***split)
{
	size_t	i;

	i = 0;
	while ((*split)[i])
	{
		free((*split)[i]);
		i++;
	}
	free(*split);
	*split = NULL;
}

/**
*	@param str	string to split
*	@param sep	null-terminated character array used as separator
*	@return Null-terminated array of strings 
*	@brief splits a string using any character in sep as separator
*
*/
char	**rt_parser_split(char *str, char const *sep)
{
	char	**split;
	char	*trimmed;
	size_t	count;
	bool	split_success;

	split_success = false;
	trimmed = ft_strtrim(str, sep);
	if (!trimmed)
		return (NULL);
	count = count_split(trimmed, sep);
	split = ft_calloc(count + 1, sizeof(char *));
	if (split)
		split_success = populate_split(split, trimmed, sep, count);
	if (!split_success)
		free_split_str(&split);
	free(trimmed);
	return (split);
}
