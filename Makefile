PROJECTS = $(shell (find ./projects -mindepth 1 -maxdepth 1 -type d ! -name "utils"; find ./projects/utils -mindepth 1 -maxdepth 1 -type d))

.PHONY: all new clean $(PROJECTS)

all: $(PROJECTS)

$(PROJECTS):
	@$(MAKE) test -C $@

new:
	cp -R ./projects/template ./projects/$(name)

clean:
	rm -rf ./projects/*/bin

