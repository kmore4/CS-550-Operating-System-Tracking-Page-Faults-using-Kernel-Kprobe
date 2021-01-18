To execute part B of kprobe, my code needs to be run twice

Eg.

sudo insmod pf_probe_B1.ko processID=481
dmesg --(It wont show any output.)

to see output--

1) remove inserted modeule
 sudo rmmod pf_probe_B1

2) Again insert b module
	sudo insmod pf_probe_B1.ko processID=481

3) dmesg
	now you can see output on a console

4) Remove module and make clean
