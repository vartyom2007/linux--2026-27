#include "ArgumentParser.h"

void ArgumentParser::addArgument(const std::string& name){
	m_regArgs[name] = "";
}

void ArgumentParser::addFlag(const std::string& name){
	m_regFlags.insert(name);
}

bool ArgumentParser::has(const std::string& name) const{
	return m_activatedFlags.find(name) != m_activatedFlags.end(); 
	       
}

std::string ArgumentParser::get(const std::string& name) const{
	auto it = m_regArgs.find(name);
	if(it != m_regArgs.end()) {
	return it->second;
	}

return "";
}

void ArgumentParser::parse(int argc, char* argv[]){
	for(int i = 1; i < argc; ++i){
		std::string currentArg = argv[i];
		if(m_regFlags.find(currentArg) != m_regFlags.end()){
			m_activatedFlags.insert(currentArg);
       }	else if(m_regArgs.find(currentArg) != m_regArgs.end()){
			if(i + 1 < argc){
				m_regArgs[currentArg] = argv[i + 1];
				i++;
			} else {
				m_regArgs[currentArg] =  "";
			}
		}
	}
}
