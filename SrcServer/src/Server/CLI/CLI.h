#pragma once

#include <string>

class CLI
{
private:
	bool* done;

	struct command
	{

	};

public:
	CLI();
	~CLI();

	bool run(bool* running);
	bool process(const std::string& cmd);

	void handleHelpCommand();
	void handleExitCommand();
	void handleShutdownCommand();
	void handleConnectedCommand();
	void handleKickCommand(const std::string& cmd);

	void handlePrintGMSCommand();
	void handleReloadGMSCommand();
	void handleReloadConfigCommand();

public:
	static CLI* getInstance();


};



