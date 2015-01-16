#Set this to @ to keep the makefile quiet
SILENCE = @

#---- Outputs ----#
COMPONENT_NAME = ControllerUnity
PROJECT_HOME_DIR = .
UNITY_HOME = ../unity.framework
CPP_PLATFORM = Gcc
UNITY_BUILD_HOME = ../unity.framework/extras/fixture/build

UNITY_CFLAGS += -DUNITY_OUTPUT_CHAR=UnityOutputCharSpy_OutputChar
LD_LIBRARIES += -lcrypto -lssl -lpthread -lrt

SRC_DIRS = \
	$(PROJECT_HOME_DIR)/src/Controller \

TEST_SRC_DIRS = \
	$(PROJECT_HOME_DIR)/unity\
	$(PROJECT_HOME_DIR)/unity/Controller\
	$(UNITY_HOME)/src\
    $(UNITY_HOME)/extras/fixture/src\
    $(UNITY_HOME)/extras/fixture/test\

INCLUDE_DIRS =\
  .\
  ../../build/i386/buildroot-2011.11/output/target/usr/xenomai/usr/include/xenomai\
  $(UNITY_HOME)/src\
  $(UNITY_HOME)/src\
  $(UNITY_HOME)/extras/fixture/src\
  $(UNITY_HOME)/extras/fixture/test\
  $(PROJECT_HOME_DIR)/include/Controller\

include $(UNITY_BUILD_HOME)/MakefileWorker.mk
