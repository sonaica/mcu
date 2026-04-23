#include "protocol-task.h"
#include "pico/stdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

static api_t* api = {0};
static int commands_count = 0;

void protocol_task_init(api_t* device_api) {
    api = device_api;
    int ind = 0;
    int cnt = 0;
    while (api[ind].command_name != NULL && strcmp(api[ind].command_name, "0") != 0){
        cnt++;
        ind++;
    }
    commands_count = cnt;
}

void protocol_task_handle(char* command_string) {
    if (!command_string) {
        return;
    }

    // логика обработки полученной строки. Делим ее на команду и аргументы:
    const char* command_name = command_string;
    const char* command_args = NULL;

    char* space_symbol = strchr(command_string, ' ');

    if (space_symbol) {
        *space_symbol = '\0';
        command_args = space_symbol + 1;
    } else {
        command_args = "";
    }

    // Добавляем вывод найденных имени команды и ее аргументов;
    printf("command name: %s | command args: %s\n", command_name, command_args);

    for (int i = 0; i < commands_count; i++) {
        // определяем совпадает ли команда с именем команды в массиве `api`
        if (strcmp(api[i].command_name, command_name) != 0) {
            continue;
        }
        // если не совпадает, переходим к следующей итерации

        // ваш код

        // мы нашли команду, вызываем callback найденной команды
        api[i].command_callback(command_args);
        return;
    }
    // выводим ошибку, если команда не была найдена в списке команд
    printf("ERROR: command not found \n");
    return;
}