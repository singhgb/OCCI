CC = /usr/bin/g++
ORACLE_HOME = /u01/app/oracle/product/12.1.0/dbhome_1
IDIR = -I$(ORACLE_HOME)/rdbms/demo -I$(ORACLE_HOME)/rdbms/public -I$(ORACLE_HOME)/plsql/public -I$(ORACLE_HOME)/network/public
CFLAGS=$(IDIR) -m64 -DLINUX -DORAX86_64 -D_GNU_SOURCE -D_LARGEFILE64_SOURCE=1 -D_LARGEFILE_SOURCE=1 -DSLTS_ENABLE -DSLMXMX_ENABLE -D_REENTRANT -DNS_THREADS -DLONG_IS_64 -DSS_64BIT_SERVER -DLDAP_CM

ALL = 	occiblob occiclob occidml
all: occiblob occiclob occidml

LIBS = -L$(ORACLE_HOME)/lib/ 
LIBFLAGS = $(LIBS) -locci -lclntsh -lclntshcore -ldl -lm -lpthread -m64
DEPS = 

OBJ = occidml.o occiclob.o occiblob.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

#$(ALL) : $(OBJ)
#	$(CC) -o $@ $^ $(LIBFLAGS)

occidml: occidml.o
	$(CC) -o $@ $^ $(LIBFLAGS)

occiblob: occiblob.o
	$(CC) -o $@ $^ $(LIBFLAGS)

occiclob: occiclob.o
	$(CC) -o $@ $^ $(LIBFLAGS)

dbblobsample: dbblobsample.o
	$(CC) -o $@ $^ $(LIBFLAGS)