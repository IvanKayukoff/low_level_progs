# HelloWorldLKM

This is a very simple hello world project represented as loadable
kernel module

## Important!

You should keep in mind that using a `Loadable kernel module` can easily 
break down your system. Better to use it in a virtual machine, such as 
`VirtualBox`, `VMware` and so on.

Use it at your own risk

## Dependencies

To be able to make `HelloWorldLKM` you should install a few dependencies 
as following:

#### For ubuntu users:

```shell
sudo apt-get install linux-headers-$(uname -r) libelf-dev
```

## Usage

To make and load the `HelloWorldLKM`:
```shell
make

sudo insmod hello.ko
```

It's time to get a result! The `HelloWorldLKM` prints message to the 
`/var/log/kern.log`, so we need to print the last line of that log:
```shell
tail -n 1 /var/log/kern.log
```

To unload LKM:
```shell
sudo rmmod hello
```

To be sure that LKM has beed shutdowned, you can look to the last line of 
`/var/log/kern.log`:
```shell
tail -n 1 /var/log/kern.log
```

