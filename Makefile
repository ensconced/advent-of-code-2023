PROJECTS = $(shell ls -d projects/*)

.PHONY: all new clean $(PROJECTS)

all: $(PROJECTS)

$(PROJECTS):
	$(MAKE) test -C $@

new:
	cp -R ./projects/template ./projects/$(name)

clean:
	rm -rf ./projects/*/bin

