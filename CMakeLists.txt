cmake_minimum_required(VERSION 3.6)
project(QtLab)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        basemodel.cpp
        basemodel.h
        button.cpp
        button.h
        db.cpp
        db.h
        init.cpp
        init.h
        lineedit.cpp
        lineedit.h
        main.cpp
        mainwindow.cpp
        mainwindow.h
        settingmodel.cpp
        settingmodel.h
        tomato.cpp
        tomato.h
        unitlabel.cpp
        unitlabel.h RingModel.h ringmodel.cpp ringmodel.h)


find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Sql REQUIRED)

#add_executable(TomatoDownWidgets ${SOURCE_FILES})
add_executable(QtLab ${SOURCE_FILES})
target_link_libraries(QtLab Qt5::Core Qt5::Widgets Qt5::Sql)



