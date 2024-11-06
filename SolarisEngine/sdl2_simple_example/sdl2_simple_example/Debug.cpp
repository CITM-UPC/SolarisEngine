//#include "App.h"
//#include "Debug.h"
//#include <iostream>
//
//
//
//Debug::Debug() {}
//Debug::~Debug() {}
//
//template<typename... Args>
//void Debug::Log(Args&&... args) {
//    std::ostringstream oss;
//    (oss << ... << args);
//    LogImpl(oss.str());
//}
//
//
//void Debug::LogImpl(const std::string& message) {
//
//    std::cout << "Log: " << message << std::endl;
//    app->windowEditor->GetImGuiWindow()->consolaPanel->AddLog(message);
//}
