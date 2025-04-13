#pragma once

#include <string>

class ConsolePanel {
public:
	void Render();

private:
	std::string ExtractFileName(const std::string& filePath);
};