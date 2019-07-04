# Caption

一个播放双语字幕的软件。采用 Qt 5.12 编写。

南京外国语学校2019届剑桥、IB毕业典礼上使用。

## 功能

* 通过 txt 读入双语字幕，方便快捷。
* 支持双显示器。在主屏幕上显示控制台，在副屏幕上显示字幕。
* 根据控制生成字幕。在操作字幕时，会在程序目录下生成一个 srt 字幕文件。字幕的时间轴与每个字幕的显示时间对应，可以用于后期视频制作。
* 支持自定义背景、字体等。
* 支持多平台。

## 安装
请在 [Releases](https://github.com/hqy2000/Caption/releases) 下载最新版本，解压运行 Caption.exe 即可。Linux、macOS版需自行编译。

程序也支持命令行参数：Caption.exe [config path]，自动加载指定的配置文件。

## 配置

1. 字幕文件请以 UTF-8 编码，双语字幕添加 .zh 扩展名为中文，添加 .en 扩展名为英文，其余请保持相同，例如 speech.en 与 speech.zh。
2. 双语字幕每一行互相对应。如 speech.en 的第一行内容对应 speech.zh 第一行内容，请确保双语文件行数一样。
3. 运行 Caption.exe，会生成 config_sample.ini， 根据该样例自行编写您自己的配置文件。
4. 使用命令行自动加载，或是手动通过对话框加载。

--------------------------

Qingyang Hu

Licensed under LGPL