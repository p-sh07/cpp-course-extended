//
//  04.Базовые понятия С++ и STL
//  Тема 07.Пары и кортежи
//  Задача 07.Вещественные числа и задача о задачах
//
//  Created by Pavel Sh on 28.11.2023.
//

#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
    NEW,          // новая
    IN_PROGRESS,  // в разработке
    TESTING,      // на тестировании
    DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

//for cerr:
string statusToStr(const TaskStatus& status) {
    switch (status) {
        default:
            return "";
            break;
        case TaskStatus::NEW:
            return "NEW";
            break;
        case TaskStatus::IN_PROGRESS:
            return "IN_PROGRESS";
            break;
        case TaskStatus::TESTING:
            return "TESTING";
            break;
        case TaskStatus::DONE:
            return "DONE";
            break;
    }
}

TaskStatus advanceTaskStatus(const TaskStatus& status) {
    TaskStatus new_status = TaskStatus::DONE;
    if(status != TaskStatus::DONE) {
        new_status = static_cast<TaskStatus>(static_cast<int>(status)+1);
    }
    return new_status;
}

class TeamTasks {
public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo& GetPersonTasksInfo(const string& person) const {
        return person_tasks_.at(person);
    }

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string& person) {
        ++person_tasks_[person][TaskStatus::NEW];
    }
    
    // Обновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count) {
        TasksInfo updated_tasks, unfinished_tasks;
        if( person_tasks_.count(person) == 0) {
            return {};
        }
        ///Т.к. у нас Map, статусы в нем уже будут отсортированы в нужном порядке. Поэтому можно просто:
        for(auto& [status, count] : person_tasks_[person]) {
            //case 0: ignore the DONE tasks
            if(status == TaskStatus::DONE) {
                break;
            } //case 1: there are still tasks to update
            else if(task_count > 0 && count > 0) { //->zeros will not be assigned with map[]=...;
                int tasks_to_update = count < task_count ? count : task_count;
                //remove tasks_to_update tasks with prev. status from person_tasks_
                count -= tasks_to_update;
                updated_tasks[advanceTaskStatus(status)] = tasks_to_update;
                task_count -= tasks_to_update;
                //case 1.5: finished updating, but there are still tasks of this status left
                if(count > 0) {
                    unfinished_tasks[status] = count;
                }
            } //case 2: store the remaining tasks in unfinished
            else if(count > 0) {
                unfinished_tasks[status] = count;
            }
        } //now update the person data with updated_tasks
        for(const auto& [upd_status, upd_count] : updated_tasks) {
            person_tasks_[person][upd_status] += upd_count;
        }
        return pair(updated_tasks, unfinished_tasks);
    }
    
private:
    map<string, TasksInfo> person_tasks_;
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь.
void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks"s
    << ", "s << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress"s
    << ", "s << tasks_info[TaskStatus::TESTING] << " tasks are being tested"s
    << ", "s << tasks_info[TaskStatus::DONE] << " tasks are done"s << endl;
}

int main() {
    TeamTasks tasks;
    tasks.AddNewTask("Ilia"s);
    for (int i = 0; i < 3; ++i) {
        tasks.AddNewTask("Ivan"s);
    }
    cout << "Ilia's tasks: "s;
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"s));
    cout << "Ivan's tasks: "s;
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));

    TasksInfo updated_tasks, untouched_tasks;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 2);
    cout << "Updated Ivan's tasks: "s;
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: "s;
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 2);
    cout << "Updated Ivan's tasks: "s;
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: "s;
    PrintTasksInfo(untouched_tasks);
}

