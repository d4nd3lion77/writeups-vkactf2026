obj-m += scoreboard.o

KDIR := $(PWD)/linux-6.6.16
PWD  := $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean