#pragma once

#include <functional>

struct UndoableAction
{
	std::function<void()> undo;
	std::function<void()> redo;
};