#include <string>
#include "bnTaskGroup.h"

TaskGroup::TaskGroup(TaskGroup && other) noexcept
{
  std::swap(tasks, other.tasks);
  std::swap(currentTask, other.currentTask);
  std::swap(maxTasks, other.maxTasks);
  other.tasks.clear();
  other.currentTask = 0;
}

const bool TaskGroup::HasMore() const
{
  return tasks.size();
}

void TaskGroup::DoNextTask()
{
  if (tasks.size()) {
    tasks.begin()->second();
    tasks.erase(tasks.begin());
    currentTask++;
  }
}

const std::string & TaskGroup::GetTaskName() const
{
  return tasks.begin()->first;
}

const unsigned TaskGroup::GetTaskNumber() const
{
  return currentTask;
}

const unsigned TaskGroup::GetTotalTasks() const
{
  return maxTasks;
}

void TaskGroup::AddTask(const std::string & name, Callback<void()>&& task)
{
  tasks.insert(tasks.end(), std::make_pair(name, std::move(task)));
  maxTasks++;
}
