CXX = g++#-3.4.3#-4.0.2
DEBUG = #-pg #-g #-ggdb
WARNING = -Wall -W -Wno-unused-parameter #-w
OPTIMIZER = -O6#-finline-limit=1000

CXXFLAGS = -I/usr/local/Cellar/boost/1.49.0/include $(DEBUG) $(WARNING) $(OPTIMIZER)
LIBS = #/usr/lib/libpapi.a
LINK = -L/usr/local/Cellar/boost/1.49.0/lib -lboost_regex-mt -lboost_iostreams-mt
