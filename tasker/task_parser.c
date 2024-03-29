#include "task_parser.h"

int fill_actions(param_container *params, char * file_name)
{
    int code_error = 0;
    size_t line_len = 0;
    ssize_t read;
    char *line = NULL;
    FILE *f = fopen(file_name, "r");

    if (f == NULL)
    {
        return 3;
    }

    while ((read = getline(&line, &line_len, f)) != -1)
    {
        char *fully_trimmed_parsed_line = special_character_remover(line);
        if (!strcmp("=", fully_trimmed_parsed_line))
        {
            fetch_action_parameters(params, f, line, line_len);
        }
        else if (!strcmp("==", fully_trimmed_parsed_line))
        {
            fetch_task(params, f, line, line_len);
        }

        if (fully_trimmed_parsed_line != NULL)
        {
            free(fully_trimmed_parsed_line);
        }
    }

    if (line != NULL)
    {
        free(line);
    }

    fclose(f);

    return code_error;
}

int get_file_char_amount(FILE *f)
{
    fseek(f, 0, SEEK_END);
    int character_counter = ftell(f);
    rewind(f);
    return character_counter;
}

void fetch_action_parameters(param_container *params, FILE *f, char *line, size_t line_len)
{
    action *action_to_add = create_empty_action();

    ssize_t read;
    while ((read = getline(&line, &line_len, f)) != -1)
    {
        char *fully_trimmed_parsed_line = special_character_remover(line);
        fully_trimmed_parsed_line = remove_all_after_character(fully_trimmed_parsed_line, '#');
        if (!strcmp("=", fully_trimmed_parsed_line) || !strcmp("==", fully_trimmed_parsed_line))
        {
            return;
        }

        if (!strcmp("+", fully_trimmed_parsed_line))
        {
            fill_action_option(action_to_add, f, line, line_len);
            add_action_to_params(params, action_to_add);
            return;
        }

        free(fully_trimmed_parsed_line);

        char *option_without_comment = remove_all_after_character(line, '#');
        option_without_comment = trim_before_after(option_without_comment);
        if (check_first_last_character('{', '}', option_without_comment))
        {
            fill_action_with_line_parameter(remove_first_last_character(option_without_comment), action_to_add, 1);
        }
    }
    add_action_to_params(params, action_to_add);
}

void fill_action_option(action *act, FILE *f, char *line, size_t line_len)
{
    ssize_t read;
    while ((read = getline(&line, &line_len, f)) != -1)
    {
        char *fully_trimmed_parsed_line = special_character_remover(line);
        if (!strcmp("=", fully_trimmed_parsed_line) || !strcmp("==", fully_trimmed_parsed_line))
        {
            fseek(f, 0 - strlen(line), SEEK_CUR);
            return;
        }

        free(fully_trimmed_parsed_line);

        char *option_trimmed_before_after = trim_before_after(line);
        if (check_first_last_character('{', '}', option_trimmed_before_after))
        {
            fill_action_with_line_parameter(remove_first_last_character(option_trimmed_before_after), act, 0);
        }
    }
}

void fill_action_with_line_parameter(char *line, action *action, int isProperties)
{
    int err = 0;
    scutted *cutted_line = string_cutter(&err, line, "->");
    if (err == 2 || cutted_line->size != 2)
    {
        easy_error_with_message(WRONG_PARAMETER_ERROR_MESSAGE);
        return;
    }

    add_parameter_to_action(action, cutted_line, isProperties);

    return;
}

void add_parameter_to_action(action *act, scutted *scut, int isProperty)
{
    keyvalue **keyvalue_array = NULL;
    int amount = 0;
    if (isProperty)
    {
        keyvalue_array = act->properties;
        amount = act->properties_amount;
    }
    else
    {
        keyvalue_array = act->options;
        amount = act->option_amount;
    }

    keyvalue *new_keyvalue = malloc(sizeof(keyvalue));
    new_keyvalue->name = scut->strings[0];
    new_keyvalue->value = scut->strings[1];

    keyvalue **new_keyvalues = malloc(sizeof(keyvalue *) * (amount + 1));
    for (int i = 0; i < amount; i++)
    {
        new_keyvalues[i] = keyvalue_array[i];
    }

    new_keyvalues[amount] = new_keyvalue;

    if (keyvalue_array != NULL)
    {
        free(keyvalue_array);
    }

    if (isProperty)
    {
        act->properties = new_keyvalues;
        act->properties_amount++;
    }
    else
    {
        act->options = new_keyvalues;
        act->option_amount++;
    }
}

void fetch_task(param_container *params, FILE *f, char *line, size_t line_len)
{
    task *task_to_add = create_empty_task();

    ssize_t read;
    while ((read = getline(&line, &line_len, f)) != -1)
    {
        char *fully_trimmed_parsed_line = special_character_remover(line);
        fully_trimmed_parsed_line = remove_all_after_character(fully_trimmed_parsed_line, '#');
        if (!strcmp("=", fully_trimmed_parsed_line) || !strcmp("==", fully_trimmed_parsed_line))
        {
            return;
        }

        if (!strcmp("+", fully_trimmed_parsed_line))
        {
            fill_task_option(task_to_add, f, line, line_len);
            add_task_to_params(params, task_to_add);
            return;
        }

        free(fully_trimmed_parsed_line);

        char *option_without_comment = remove_all_after_character(line, '#');
        option_without_comment = trim_before_after(option_without_comment);
        if (check_first_last_character('{', '}', option_without_comment))
        {
            fill_task_with_line_parameter(remove_first_last_character(option_without_comment), task_to_add);
        }
    }
    add_task_to_params(params, task_to_add);
}

void fill_task_with_line_parameter(char *string, task *ta)
{
    int err = 0;
    scutted *cutted_line = string_cutter(&err, string, "->");
    if (err == 2 || cutted_line->size != 2)
    {
        easy_error_with_message(WRONG_PARAMETER_ERROR_MESSAGE);
        return;
    }

    add_parameter_to_task(ta, cutted_line);

    if (cutted_line != NULL)
    {
        free(cutted_line);
    }

    return;
}

void add_parameter_to_task(task *ta, scutted *scut)
{
    keyvalue **keyvalue_array = ta->properties;
    int amount = ta->properties_amount;

    keyvalue *new_keyvalue = malloc(sizeof(keyvalue));
    new_keyvalue->name = scut->strings[0];
    new_keyvalue->value = scut->strings[1];

    keyvalue **new_keyvalues = malloc(sizeof(keyvalue *) * (amount + 1));
    for (int i = 0; i < amount; i++)
    {
        new_keyvalues[i] = keyvalue_array[i];
    }

    new_keyvalues[amount] = new_keyvalue;

    if (keyvalue_array != NULL)
    {
        free(keyvalue_array);
    }

    ta->properties = new_keyvalues;
    ta->properties_amount++;
}

void fill_task_option(task *ta, FILE *f, char *line, size_t line_len)
{
    ssize_t read;
    while ((read = getline(&line, &line_len, f)) != -1)
    {
        char *fully_trimmed_parsed_line = special_character_remover(line);
        fully_trimmed_parsed_line = remove_all_after_character(fully_trimmed_parsed_line, '#');
        if (!strcmp("=", fully_trimmed_parsed_line) || !strcmp("==", fully_trimmed_parsed_line))
        {
            fseek(f, 0 - strlen(line), SEEK_CUR);
            return;
        }

        free(fully_trimmed_parsed_line);

        char *option_without_comment = remove_all_after_character(line, '#');
        option_without_comment = trim_before_after(option_without_comment);
        if (check_first_last_character('(', ')', option_without_comment))
        {
            fill_task_with_line_option(remove_first_last_character(option_without_comment), ta);
        }
    }
}

void fill_task_with_line_option(char *string, task *ta)
{
    int err = 0;
    scutted *cutted_line = string_cutter(&err, string, ",");

    add_option_to_task(cutted_line, ta);

    if (cutted_line != NULL)
    {
        free(cutted_line);
    }

    return;
}

void add_option_to_task(scutted *scut, task *ta)
{
    int size = ta->action_amount;
    char **new_options = malloc(sizeof(char *) * (size + scut->size));

    for (int i = 0; i < size; i++)
    {
        new_options[i] = ta->actions_name[i];
    }

    for (int i = 0; i < scut->size; i++)
    {
        new_options[size + i] = scut->strings[i];
    }

    if (ta->actions_name != NULL)
    {
        free(ta->actions_name);
    }

    ta->actions_name = new_options;
    ta->action_amount += scut->size;
}
