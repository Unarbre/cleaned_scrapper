#ifndef PARAMSHELPER
#define PARAMSHELPER

#include "supervisor.h"

void print_action(action *act);
void add_action_to_params(param_container *params, action *act);
void print_full_params(param_container *params);
void print_task(task *ta);
void add_task_to_params(param_container *params, task *ta);
param_container *create_empty_params();
task *create_empty_task();
action *create_empty_action();
void report_errors(param_container *params);
void report_action_errors(action *act, int index);
void check_if_action_is_named(action *act, int index);
void report_tasks_errors(task *task, int index, action **actions, int action_amount);
void check_action_parameter_validity(task *task, int index, action **actions, int action_amount);
char *get_action_property(action *act, char *property_name);
true_action **generate_true_actions(param_container *params, int *amount);
true_action *fill_true_action(action *action);
char *get_action_option(action *act, char *option_name);
void fill_types(true_action *true_act, action *act);
char **get_types(scutted *scut, int *size);
true_parameters *generate_true_params(param_container *params);
void print_true_actions(true_action **true_actions, int amount);
void print_taction(true_action *taction);
true_task **generate_true_tasks(param_container *params, true_action **actions, int action_amount, int *tasks_amount);
true_task *fill_true_task(task *tas, true_action **true_actions, int action_amount);
long long get_task_timer(task *tas);
char *get_task_property(task *tas, char *property_name);
void fill_task_with_actions(task *tas, true_task *true_tas, true_action **true_actions, int action_amount);
true_action *get_action_by_action_name(char *action_name, true_action **true_actions, int action_amount);
void print_true_tasks(true_task **true_tasks, int tasks_amount);
void print_ttask(true_task *ttask);
#endif