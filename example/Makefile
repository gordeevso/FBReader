ROOTDIR = $(CURDIR)/..

MAKEFILESDIR = $(ROOTDIR)/makefiles

include $(MAKEFILESDIR)/config.mk

TARGET = Example
target = example


SUBDIRS = src

all:
	@for subdir in $(SUBDIRS); do \
		if ! $(MAKE) -C $$subdir -f $(MAKEFILESDIR)/subdir.mk; then \
			exit 1; \
		fi; \
	done;
	@echo -n 'Linking $(TARGET) ...'
	@$(LD) $(LDFLAGS) -o $(TARGET) `find src -name *.o` -L$(ROOTDIR)/zlibrary/text $(TEXT_LIBS) $(CORE_LIBS) -lsqlite3
	@echo ' OK'


clean:
	@for subdir in $(ALL_SUBDIRS); do \
		$(MAKE) -C $$subdir -f $(MAKEFILESDIR)/subdir.mk clean; \
	done;
	@for subdir in $(ALL_ARCHSUBDIRS); do \
		cd $$subdir; make clean; cd ..; \
	done;
	@$(RM) $(TARGET) err
