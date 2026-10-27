cmake_minimum_required(VERSION 3.16)
project(PBL2_TutorCenter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

set(SOURCES
    src/code/entities/Person.cpp
    src/code/entities/Student.cpp
    src/code/entities/Tutor.cpp
    src/code/entities/Class.cpp
    src/code/entities/Contract.cpp
    src/code/entities/Account.cpp
    src/code/helper/Validator.cpp
    src/code/helper/IdGenerator.cpp
    src/code/services/CenterService.cpp
    src/code/services/AuthService.cpp
    src/code/ui/MainWindow.cpp
    src/code/ui/LoginWindow.cpp
    src/code/main_gui.cpp
)

set(HEADERS
    src/include/dsa/DoublyLinkedList.h
    src/include/dsa/HashTable.h
    src/include/entities/Person.h
    src/include/entities/Student.h
    src/include/entities/Tutor.h
    src/include/entities/Class.h
    src/include/entities/Contract.h
    src/include/entities/Account.h
    src/include/helper/Validator.h
    src/include/helper/IdGenerator.h
    src/include/services/CenterService.h
    src/include/services/AuthService.h
    src/include/ui/MainWindow.h
    src/include/ui/LoginWindow.h
)

add_executable(pbl2_gui WIN32 ${SOURCES} ${HEADERS})

target_include_directories(pbl2_gui PRIVATE
    src/include
    src/include/dsa
    src/include/entities
    src/include/helper
    src/include/services
    src/include/ui
)

target_link_libraries(pbl2_gui PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
