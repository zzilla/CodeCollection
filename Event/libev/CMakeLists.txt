########################################################################################
#############              ���� libev ���ļ�                               ############## 
########################################################################################
LIST (APPEND libev_LIB_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/ev.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ev_epoll.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ev_kqueue.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ev_poll.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ev_port.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ev_select.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ev_win32.c
    ${CMAKE_CURRENT_SOURCE_DIR}/event.c
    
#    ${CMAKE_CURRENT_SOURCE_DIR}/
    )



########################################################################################
#############               ���� libev ��汾��                          ############## 
########################################################################################
set(libev_LIB_VERSION     "1.0.0")
set(libev_LIB_SOVERSION   "1")



########################################################################################
#############              ����ͷ�ļ�Ŀ¼                                 ############## 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Messages
    ${PROJECT_SOURCE_DIR}/Messages/libev
)  



########################################################################################
#############               ���� ���ӿ��ļ�Ŀ¼                           ############## 
########################################################################################
link_directories(  
    /usr/local/lib   
    )  



########################################################################################
#############           ����Ŀ����ļ� �� �����汾��                      ############## 
########################################################################################
IF (MODULE_libev)

    #���ɶ�̬��  ��̬���� STATIC  
    add_library (libevlib     SHARED      ${libev_LIB_SRCS})  
    add_library (libevs       STATIC        ${libev_LIB_SRCS})  
    
    # ���ð汾�ţ�SOVERSIONΪAPI�汾��
    set_target_properties(libevlib    PROPERTIES 
                                        VERSION     ${libev_LIB_VERSION} 
                                        SOVERSION   ${libev_LIB_SOVERSION} )
ELSE (MODULE_libev)
    MESSAGE(STATUS "Not Include libev module.")
ENDIF (MODULE_libev)



########################################################################################
#############                 ����Ŀ�������ļ�                          ############## 
########################################################################################
IF (TEST_MODULE_FLAG AND MODULE_libev)
    add_executable(TestLibev.elf    libevTest.c)
    add_dependencies(TestLibev.elf        libevlib loglib)
    target_link_libraries(TestLibev.elf   libevlib loglib)

ELSE (TEST_MODULE_FLAG AND MODULE_libev)
    MESSAGE(STATUS "Not Include libev module.")
ENDIF (TEST_MODULE_FLAG AND MODULE_libev)



