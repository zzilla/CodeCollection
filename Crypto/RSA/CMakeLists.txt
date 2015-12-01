########################################################################################
#############               ���� rsa ���ļ�                            ############## 
########################################################################################
LIST (APPEND rsa_LIB_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/RSA.cpp
    )
    
      
           
########################################################################################
#############               ���� rsa ��汾��                          ############## 
########################################################################################
set(rsa_LIB_VERSION     "1.0.0")
set(rsa_LIB_SOVERSION   "1")



########################################################################################
#############              ����ͷ�ļ�Ŀ¼                                 ############## 
########################################################################################
include_directories(  
    ${PROJECT_SOURCE_DIR}/includes  
    ${PROJECT_SOURCE_DIR}/Crypto
    ${PROJECT_SOURCE_DIR}/Crypto/RSA
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
IF (MODULE_rsa)
    #���ɶ�̬��  ��̬���� STATIC  
    add_library (rsalib SHARED          ${rsa_LIB_SRCS})  
    add_library (rsas   STATIC          ${rsa_LIB_SRCS})  
    # ���ð汾�ţ�SOVERSIONΪAPI�汾��
    set_target_properties(rsalib    PROPERTIES 
                                    VERSION     ${rsa_LIB_VERSION} 
                                    SOVERSION   ${rsa_LIB_SOVERSION} )
ELSE (MODULE_rsa)
    MESSAGE(STATUS "Not Include rsa module.")
ENDIF (MODULE_rsa)




########################################################################################
#############                 ����Ŀ�������ļ�                          ############## 
########################################################################################
IF (TEST_MODULE_FLAG AND MODULE_rsa)
    add_executable(TestRsa.elf  rsaTest.cpp)
    add_dependencies(TestRsa.elf    rsalib  loglib)
    target_link_libraries(TestRsa.elf   loglib  rsalib )

ELSE (TEST_MODULE_FLAG AND MODULE_rsa)
    MESSAGE(STATUS "Not Include rsa module.")
ENDIF (TEST_MODULE_FLAG AND MODULE_rsa)
