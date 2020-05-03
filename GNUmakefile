TARGET:=compile
DIR_NAME:=$(HOME)/proyectos/scicrt/simulation/scicrt-fiber
PROJECT:=scibar
G4INSTALL:=/usr/lib/Geant4-10.2.2/
OPTICS=optical

all: $(TARGET)

compile:
	cd $(DIR_NAME) && mkdir $(PROJECT)-build;
	cd $(DIR_NAME)/$(PROJECT)-build && cmake -DGeant4_DIR=$(G4INSTALL) $(DIR_NAME)/$(PROJECT);
	cd $(DIR_NAME)/$(PROJECT)-build && mkdir $(OPTICS);
	cd $(DIR_NAME)/$(PROJECT)-build && make -j8;
	cp -R $(DIR_NAME)/$(PROJECT)/$(OPTICS)/* $(DIR_NAME)/$(PROJECT)-build/$(OPTICS);
clean:
	cd $(DIR_NAME) && rm -R $(PROJECT)-build
