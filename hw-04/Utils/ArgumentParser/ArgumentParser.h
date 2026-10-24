#pragma once

#include <string>
#include <map>
#include <set>

class ArgumentParser{
public:
	ArgumentParser() = default;

	void addArgument(const std::string& name);
	void addFlag(const std::string& name);
	void parse(int argc, char* argv[]);
	
	bool has(const std::string& name) const;
	std::string get(const std::string& name) const;
private:
	std::map<std::string, std::string> m_regArgs;
	std::set<std::string> m_regFlags;
	std::set<std::string> m_activatedFlags;
	
	
};
