_SRC := $(if $(src),$(src),.)
include $(_SRC)/Makefile.vars # AKMOD can be annoying so preventing his silent context change

DKMS_ROOT_PATH  := /usr/src/msi-ec-ng-$(VERSION)

KERNELRELEASE := $(shell uname -r)
KERNEL_BUILD   := /lib/modules/$(KERNELRELEASE)/build

# Kernels built with clang (e.g. CachyOS) reject the gcc-only flags that kbuild
# passes to modules, so mirror the kernel toolchain automatically.
KBUILD_TOOLCHAIN := $(if $(shell grep -q '^CONFIG_CC_IS_CLANG=y' $(KERNEL_BUILD)/.config 2>/dev/null && echo 1),LLVM=1)

KMOD_DIR        := /lib/modules/$(KERNELRELEASE)/updates/drivers/platform/x86

ccflags-y := -std=gnu11 -Wno-declaration-after-statement

obj-m += $(MODNAME).o
$(MODNAME)-y := msi-ec.o

all: modules

modules:
	@$(MAKE) -C $(KERNEL_BUILD) M=$(CURDIR) $(KBUILD_TOOLCHAIN) modules

clean:
	@$(MAKE) -C $(KERNEL_BUILD) M=$(CURDIR) $(KBUILD_TOOLCHAIN) clean

load:
	insmod msi-ec-ng.ko

load-debug:
	insmod msi-ec-ng.ko debug=1

unload:
	-rmmod msi-ec-ng

reload: unload load

reload-debug: unload load-debug

install:
	mkdir -p $(KMOD_DIR)
	cp msi-ec-ng.ko $(KMOD_DIR)
	depmod -a
	echo msi-ec-ng > /etc/modules-load.d/msi-ec-ng.conf
	modprobe -v msi-ec-ng

uninstall:
	-modprobe -rv msi-ec-ng
	rm -f $(KMOD_DIR)/msi-ec-ng.ko
	-rmdir -p $(KMOD_DIR) > /dev/null 2>&1
	depmod -a
	rm -f /etc/modules-load.d/msi-ec-ng.conf

dkms-install:
	@dkms --version
	mkdir -p $(DKMS_ROOT_PATH)
	cp $(CURDIR)/dkms.conf $(DKMS_ROOT_PATH)
	cp $(CURDIR)/Makefile $(DKMS_ROOT_PATH)
	cp $(CURDIR)/Makefile.vars $(DKMS_ROOT_PATH)
	cp $(CURDIR)/msi-ec.c $(DKMS_ROOT_PATH)
	cp $(CURDIR)/ec_memory_configuration.h $(DKMS_ROOT_PATH)

	sed -e "s/@VERSION@/$(VERSION)/" \
	    -i $(DKMS_ROOT_PATH)/dkms.conf

	dkms add msi-ec-ng/$(VERSION)
	dkms build msi-ec-ng/$(VERSION)
	dkms install msi-ec-ng/$(VERSION)
	echo msi-ec-ng > /etc/modules-load.d/msi-ec-ng.conf

dkms-uninstall:
	-dkms remove msi-ec-ng/$(VERSION) --all
	rm -rf $(DKMS_ROOT_PATH)
	rm -f /etc/modules-load.d/msi-ec-ng.conf

dkms-update: dkms-uninstall dkms-install

dev: modules unload load

rpm:
	$(MAKE) -C packaging/rpm-akmod/ srpm
