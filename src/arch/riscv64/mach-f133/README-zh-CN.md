# 全志F133

***

## 编译源码，生成的目标文件在output目录下
```shell
make clean
make CROSS_COMPILE=/path/to/riscv64-unknown-elf- PLATFORM=riscv64-f133
```

## 烧写到RAM中并运行
```shell
sudo xfel ddr ddr2; sudo xfel write 0x40000000 xboot.bin; sudo xfel exec 0x40000000;
```

## 烧写普通镜像到SPI Nor Flash
```shell
sudo xfel spinor write 0 xboot.bin
```

## 烧写压缩镜像到SPI Nor Flash
```shell
sudo xfel spinor write 0 xboot.bin.z
```

## 烧写普通镜像到SPI Nand Flash
```shell
sudo xfel spinand splwrite 2048 1048576 xboot.bin
```

## 烧写压缩镜像到SPI Nand Flash
```shell
sudo xfel spinand splwrite 2048 1048576 xboot.bin.z
```

## 下载riscv工具链
```shell
http://pan.baidu.com/s/1dDtssIt

```
```
https://occ.t-head.cn/
Xuantie-900-gcc-elf-newlib-x86_64-V2.0.3-20210806.tar.gz
```

## 下载xfel工具源码
```shell
git clone https://github.com/xboot/xfel.git
```

## 编译安装xfel工具
```shell
make
sudo make install
```

# 相关网址

- XBOOT官方首页
  https://xboot.github.io/

- XBOOT源码(完全开源)，寄存在github，请直接`fork`一份代码，如需申请合并，直接`pull request`即可
  https://github.com/xboot/xboot

- XBOOT运行效果展示页面，首次接触的用户，建议观看，对XBOOT有个直观的感受
  https://github.com/xboot/xboot/blob/master/documents/xboot-show-case-zh-CN.md

- XBOOT系统开发指南
  https://github.com/xboot/xboot/blob/master/documents/xboot-system-development-guide-zh-CN.md

- XBOOT应用开发指南
  https://github.com/xboot/xboot/blob/master/documents/xboot-application-development-guide-zh-CN.md

- XBOOT官方QQ群，大佬聚集，请踊跃加入
  [658250248](https://jq.qq.com/?_wv=1027&k=5BOkXYO) (2000人)
