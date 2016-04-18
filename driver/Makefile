subdirs = system com dio
MAKE=make --no-print-directory

all:	
	@for subdir in $(subdirs) ; do \
	(cd $$subdir && $(MAKE)) ;\
	done

modules_install:
	@for subdir in $(subdirs) ; do \
	(cd $$subdir && $(MAKE) modules_install ) ;\
	done

clean:
	@for subdir in $(subdirs) ; do \
	(cd $$subdir && $(MAKE) clean ) ;\
	done
