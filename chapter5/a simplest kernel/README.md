#README

###过程中出现的 bug
1. `stosd` 弄成了 `stodb` ，导致 `RAM SIZE` 打印出一堆 `0` 出来～


------
###一些心得

1. 从实模式到保护模式的跳转之后，**记得记得** 一定要切换堆栈。
2.  `%include` 放置在错误的位置是会导致意外的错误的。例如，在 `loader.asm` 这个文件中，由于 `setuppaging` 和 `disp_meminfo` 这两个函数要调用 `disp_int` 和 `disp_al` 这两个显示函数，而这两个文件又都被放置在了 `lib.inc` 这个文件中。一开始，我为了方便和美观，把 `%include lib.inc` 这个头文件跟其他的头文件一起放在了 `loader.asm` 的开头处，结果一 `make` 就报 `warning` ：

         warning : word data exceeds bounds
         
    一开始我也很奇怪，`double word` 和 `edi` 怎么可能超出呢？后来想想，也是你把头文件放在了 `实模式` 下，所以 `edi` 是识别不出来的，只能识别出 `di` ，`di` 是十六位的，所以才会报 `waring` ～
**所以，`%include pm.inc` 一定要放在保护模式下的段中！**

3. 对于 `Makefile` ，**记得要把相关的头文件放在 `prerequisites` 的位置上 ！ **不然有可能你怎么该都是错的～