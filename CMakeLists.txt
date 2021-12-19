#cmake最低版本需求，不加入此行会受到警告信息
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

#项目名称
PROJECT(linuxservercode) 

#把当前目录(.)下所有源代码文件和头文件加入变量SRC_LIST
#AUX_SOURCE_DIRECTORY(. SRC_LIST)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/httpconn)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/mylock)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/pool)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/myconfig)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/mylog)

AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_LIST_DIR}/src/httpconn SOURCE_HTTPCONN)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_LIST_DIR}/src/mylock SOURCE_MYLOCK)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_LIST_DIR}/src/pool SOURCE_POOL)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_LIST_DIR}/src/myconfig SOURCE_MYCONFIG)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_LIST_DIR}/src/mylog SOURCE_MYLOG)

#add_definitions("-g -Wall -std=c++11")
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++11 -pthread")

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_LIST_DIR}/bin)

#生成应用程序 mylinuxserver
#ADD_EXECUTABLE(mylinuxserver ${SRC_LIST})
ADD_EXECUTABLE(mylinuxserver 
                    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
                    ${SOURCE_HTTPCONN}
                    ${SOURCE_MYLOCK}
                    ${SOURCE_POOL}
                    ${SOURCE_MYCONFIG}
                    ${SOURCE_MYLOG})