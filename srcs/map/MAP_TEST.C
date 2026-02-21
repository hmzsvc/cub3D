
//#include "../../inc/game.h"

//static char	*skip_spaces(char *line)
//{
//    while (*line && (*line == ' ' || *line == '\t'))
//        line++;
//    return (line);
//}

//static int	parse_color(char *line)
//{
//    int		r;
//    int		g;
//    int		b;
//    char	**split;
//    int		i;

//    split = ft_split(line, ',');
//    if (!split || !split[0] || !split[1] || !split[2])
//        return (-1);
//    r = ft_atoi(split[0]);
//    g = ft_atoi(split[1]);
//    b = ft_atoi(split[2]);
//    i = 0;
//    while (split[i])
//        free(split[i++]);
//    free(split);
//    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
//        return (-1);
//    return ((r << 16) | (g << 8) | b);
//}

//static char	*trim_newline(char *str)
//{
//    int	len;

//    if (!str)
//        return (NULL);
//    len = ft_strlen(str);
//    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r'))
//    {
//        str[len - 1] = '\0';
//        len--;
//    }
//    return (str);
//}

//static int	parse_element(char *line, t_game *game)
//{
//    char	*trimmed;
//    char	*path;

//    trimmed = skip_spaces(line);
//    if (ft_strncmp(trimmed, "NO ", 3) == 0)
//    {
//        path = trim_newline(skip_spaces(trimmed + 3));
//        game->north_path = ft_strdup(path);
//        return (1);
//    }
//    if (ft_strncmp(trimmed, "SO ", 3) == 0)
//    {
//        path = trim_newline(skip_spaces(trimmed + 3));
//        game->south_path = ft_strdup(path);
//        return (1);
//    }
//    return (parse_element_continue(trimmed, game));
//}

//int	parse_element_continue(char *trimmed, t_game *game)
//{
//    char	*path;

//    if (ft_strncmp(trimmed, "WE ", 3) == 0)
//    {
//        path = trim_newline(skip_spaces(trimmed + 3));
//        game->west_path = ft_strdup(path);
//        return (1);
//    }
//    if (ft_strncmp(trimmed, "EA ", 3) == 0)
//    {
//        path = trim_newline(skip_spaces(trimmed + 3));
//        game->east_path = ft_strdup(path);
//        return (1);
//    }
//    return (parse_colors(trimmed, game));
//}

//int	parse_colors(char *trimmed, t_game *game)
//{
//    if (ft_strncmp(trimmed, "F ", 2) == 0)
//    {
//        game->floor_color = parse_color(skip_spaces(trimmed + 2));
//        if (game->floor_color == -1)
//            return (0);
//        return (1);
//    }
//    if (ft_strncmp(trimmed, "C ", 2) == 0)
//    {
//        game->ceiling_color = parse_color(skip_spaces(trimmed + 2));
//        if (game->ceiling_color == -1)
//            return (0);
//        return (1);
//    }
//    return (0);
//}

//int	is_map_line(char *line)
//{
//    char	*trimmed;

//    trimmed = skip_spaces(line);
//    if (*trimmed == '1' || *trimmed == '0')
//        return (1);
//    return (0);
//}

//int	is_empty_line(char *line)
//{
//    char	*trimmed;

//    trimmed = skip_spaces(line);
//    if (*trimmed == '\0' || *trimmed == '\n')
//        return (1);
//    return (0);
//}


//static int	count_map_lines(int fd)
//{
//    char	*line;
//    int		count;
//    int		map_started;

//    count = 0;
//    map_started = 0;
//    line = get_next_line(fd);
//    while (line)
//    {
//        if (is_map_line(line))
//        {
//            map_started = 1;
//            count++;
//        }
//        else if (map_started && !is_empty_line(line))
//            break ;
//        free(line);
//        line = get_next_line(fd);
//    }
//    while (line)
//    {
//        free(line);
//        line = get_next_line(fd);
//    }
//    return (count);
//}

//static char	**read_map_data(int fd, int line_count)
//{
//    char	**map;
//    char	*line;
//    int		i;

//    map = malloc(sizeof(char *) * (line_count + 1));
//    if (!map)
//        return (NULL);
//    i = 0;
//    line = get_next_line(fd);
//    while (line && i < line_count)
//    {
//        if (is_map_line(line))
//        {
//            map[i] = trim_newline(ft_strdup(line));
//            i++;
//        }
//        free(line);
//        line = get_next_line(fd);
//    }
//    map[i] = NULL;
//    return (map);
//}

//static int	parse_config(int fd, t_game *game)
//{
//    char	*line;
//    int		elements;

//    elements = 0;
//    line = get_next_line(fd);
//    while (line && elements < 6)
//    {
//        if (!is_empty_line(line) && !is_map_line(line))
//        {
//            if (parse_element(line, game))
//                elements++;
//        }
//        else if (is_map_line(line))
//        {
//            free(line);
//            break ;
//        }
//        free(line);
//        line = get_next_line(fd);
//    }
//    return (elements);
//}

//char	**read_map(char *path, t_game *game)
//{
//    int		fd;
//    int		map_lines;
//    int		elements;

//    fd = open(path, O_RDONLY);
//    if (fd < 0)
//        return (NULL);
//    elements = parse_config(fd, game);
//    close(fd);
//    if (elements != 6)
//        return (NULL);
//    fd = open(path, O_RDONLY);
//    map_lines = count_map_lines(fd);
//    close(fd);
//    if (map_lines == 0)
//        return (NULL);
//    fd = open(path, O_RDONLY);
//    game->map = read_map_data(fd, map_lines);
//    close(fd);
//    return (game->map);
//}
