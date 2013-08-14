INET_PROJECT_DIR=../../inet

all: checkmakefiles
	cd src && $(MAKE)

clean: checkmakefiles
	cd src && $(MAKE) clean

cleanall: checkmakefiles
	cd src && $(MAKE) MODE=release clean
	cd src && $(MAKE) MODE=debug clean
	rm -f src/Makefile
	rm -rf out/

makefiles:
	cd src && opp_makemake -f --deep --make-so -o cni-os3 -O out -I$$\(INET_PROJ\)/src/world/radio -I$$\(INET_PROJ\)/src/mobility/models -I$$\(INET_PROJ\)/src/mobility -I$$\(INET_PROJ\)/src/util -I$$\(INET_PROJ\)/src/base -L/usr/local/lib -L$$\(INET_PROJ\)/out/$$\(CONFIGNAME\)/src -linet -lcurl -DINET_IMPORT -KINET_PROJ=$(INET_PROJECT_DIR)

checkmakefiles:
	@if [ ! -f src/Makefile ]; then \
	echo; \
	echo '======================================================================='; \
	echo 'src/Makefile does not exist. Please use "make makefiles" to generate it!'; \
	echo '======================================================================='; \
	echo; \
	exit 1; \
	fi
