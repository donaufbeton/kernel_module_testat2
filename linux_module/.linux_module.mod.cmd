savedcmd_/home/test/Documents/testat2/kernel_module_testat2/linux_module/linux_module.mod := printf '%s\n'   linux_module.o | awk '!x[$$0]++ { print("/home/test/Documents/testat2/kernel_module_testat2/linux_module/"$$0) }' > /home/test/Documents/testat2/kernel_module_testat2/linux_module/linux_module.mod
