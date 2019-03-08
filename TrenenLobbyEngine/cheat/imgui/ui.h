#pragma once

class UI
{
public:
	void Setup();
	void Render();

private:
	Singleton(UI);
};