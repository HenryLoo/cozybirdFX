#pragma once

struct GLFWwindow;

class InputManager
{
public:
	InputManager();

	void update(GLFWwindow *window);

	// Check input states.
	//bool getKeyPressed() const;
	//bool getKeyReleased() const;
	//bool getKeyDown() const;
	bool getMousePressed() const;
	bool getMouseReleased() const;
	bool getMouseDown() const;

private:

};