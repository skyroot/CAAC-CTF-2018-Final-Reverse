# 民航CTF决赛Reverse部分WP

## 1. FlagCounter（200pt）
- 主要考点：JAR逆向
- 工具：Java Decompiler / Luyten

- 题目给出JAR文件，直接使用JD或Luyten工具进行反编译
- 找到`cn.edu.cauc.isecca.fc.FlagCounter`类，定位到按钮点击事件方法：

```java
if (e.getSource() == this.btnSubmit) {
            final FlagChecker fc = new FlagChecker();
            if (fc.check(this.textFieldFlag.getText())) {
                this.textFieldFlag.setBorder(new LineBorder(Color.GREEN));
            }
            else {
                this.textFieldFlag.setBorder(new LineBorder(Color.RED));
            }
        }
```

- 其中答案正确与否是通过修改文本框边框颜色实现，不难看出只要方法`fc.check()`返回true即可通过，随后定位到该函数：

```java
public boolean check(final String flag) {
            if (flag.length() != 21) {
                return false;
            }
            final String encodeRow = "MCwyLDAsNiw4LDExLDExLDgsNyw4LDEzLDQsNiwxMSw4LDEyLDIsNiwwLDExLDE1";
            final String encodeColumn = "MTAsMTQsMiw4LDQsMywxMiw0LDIsNCwzLDgsOCwxMiw0LDEwLDE0LDQsNSwxMiwxMw==";
            final String indexRowString = new String(Base64.getDecoder().decode(encodeRow));
            final String indexColumnString = new String(Base64.getDecoder().decode(encodeColumn));
            final String[] indexRowArray = indexRowString.split(String.valueOf((char)this.s[4][2]));
            final String[] indexColumnArray = indexColumnString.split(String.valueOf((char)this.s[4][2]));
            if (indexRowArray.length != indexColumnArray.length || indexRowArray.length == 0) {
                return false;
            }
            for (int i = 0; i < indexRowArray.length; ++i) {
                if (flag.charAt(i) != this.s[Integer.valueOf(indexRowArray[i])][Integer.valueOf(indexColumnArray[i])]) {
                    return false;
                }
            }
            return true;
        }
```

- 该类中包含一个二维数组，通过代码推测，是从base64解码出一些二维数组坐标，再取出对应的值拼接成字符串即为答案，编写逆算法：

```java
int[][] s =  new int[][] { { 99, 124, 119, 123, 242, 107, 111, 197, 48, 1, 103, 43, 254, 215, 171, 118 }, 
{ 202, 130, 201, 125, 250, 89, 71, 240, 173, 212, 162, 175, 156, 164, 114, 192 }, 
{ 183, 253, 147, 38, 54, 63, 247, 204, 52, 165, 229, 241, 113, 216, 49, 21 }, 
{ 4, 199, 35, 195, 24, 150, 5, 154, 7, 18, 128, 226, 235, 39, 178, 117 }, 
{ 9, 131, 44, 26, 27, 110, 90, 160, 82, 59, 214, 179, 41, 227, 47, 132 }, 
{ 83, 209, 0, 237, 32, 252, 177, 91, 106, 203, 190, 57, 74, 76, 88, 207 }, 
{ 208, 239, 170, 251, 67, 77, 51, 133, 69, 249, 2, 127, 80, 60, 159, 168 }, 
{ 81, 163, 64, 143, 146, 157, 56, 245, 188, 182, 218, 33, 16, 255, 243, 210 }, 
{ 205, 12, 19, 236, 95, 151, 68, 23, 196, 167, 126, 61, 100, 93, 25, 115 }, 
{ 96, 129, 79, 220, 34, 42, 144, 136, 70, 238, 184, 20, 222, 94, 11, 219 }, 
{ 224, 50, 58, 10, 73, 6, 36, 92, 194, 211, 172, 98, 145, 149, 228, 121 }, 
{ 231, 200, 55, 109, 141, 213, 78, 169, 108, 86, 244, 234, 101, 122, 174, 8 }, 
{ 186, 120, 37, 46, 28, 166, 180, 198, 232, 221, 116, 31, 75, 189, 139, 138 }, 
{ 112, 62, 181, 102, 72, 3, 246, 14, 97, 53, 87, 185, 134, 193, 29, 158 }, 
{ 225, 248, 152, 17, 105, 217, 142, 148, 155, 30, 135, 233, 206, 85, 40, 223 }, 
{ 140, 161, 137, 13, 191, 230, 66, 104, 65, 153, 45, 15, 176, 84, 187, 22 } };
						
		final String encodeRow = "MCwyLDAsNiw4LDExLDExLDgsNyw4LDEzLDQsNiwxMSw4LDEyLDIsNiwwLDExLDE1";
		final String encodeColumn = "MTAsMTQsMiw4LDQsMywxMiw0LDIsNCwzLDgsOCwxMiw0LDEwLDE0LDQsNSwxMiwxMw==";
		final String indexRowString = new String(Base64.getDecoder().decode(encodeRow));
		final String indexColumnString = new String(Base64.getDecoder().decode(encodeColumn));
		final String[] indexRowArray = indexRowString.split(String.valueOf((char)s[4][2]));
		final String[] indexColumnArray = indexColumnString.split(String.valueOf((char)s[4][2]));
		StringBuilder flag = new StringBuilder();
		for (int i = 0; i < indexRowArray.length; ++i) {
			flag.append((char)s[Integer.valueOf(indexRowArray[i])][Integer.valueOf(indexColumnArray[i])]);
		}
		System.out.println(flag.toString());
```

- 程序输出：

```
g1wE_me_@_fREe_t1CkeT
```

- 答案：

```
flag{g1wE_me_@_fREe_t1CkeT}
```

## 2. RELF（200pt）
- 主要考点：Linux程序逆向
- 工具：IDA Pro

- 题目给出一ELF文件，64位，使用64位IDA Pro打开，定位到主函数：

```c
int __cdecl main(int argc, const char **argv, const char **envp)
```

- 容易得知函数`f`是关键函数，定位到该函数：

```c
int __fastcall f(__int64 a1, signed int a2)
```

- 分析可知，该函数限制了输入长度不得超过29，同时定位到函数`c`：

```c
int __fastcall c(__int64 a1, int a2)
```

- 先不看数据区，观察for循环可以得知，变量a1为输入的flag，而变量v3、v33、v63都是数组。代码可以转换为以下形式：

```c
for ( i = 0; i < a2; ++i )
```

- 可以看出，这是一个一元一次方程的形式，`v63[i] * a1[i] + v33[i] = v3[i]`，所以解此方程可得flag：

```
flag{E1F_FUnct10n_Math_S01vE}
```


## 3. SoPerfectApp（400pt）
- 主要考点：Android Java和So逆向、Xposed
- 工具：apktool、dex2jar、Luyten、IDA Pro

- 题目给出一apk文件，使用dex2jar工具将其转换为jar文件，工具报出错误：

```sh
$ d2j-dex2jar.sh SoPerfectApp.apk 
dex2jar SoPerfectApp.apk -> ./SoPerfectApp-dex2jar.jar
Detail Error Information in File ./SoPerfectApp-error.zip
Please report this file to http://code.google.com/p/dex2jar/issues/entry if possible.
```

- 但是实际上已经得到了正确的jar文件，可以继续查看，打开jar文件，定位到按钮点击事件方法：

```java
public void onClick(final View view) {
    MainActivity.this.check(MainActivity.this.flagText.getText().toString());
}
```

- 发现其调用了`check`方法，定位到该方法：

```java
public void check(String s) {
        if (this.realcheck(s)) {
            s = "RIGHT";
        }
        else {
            s = "WRONG";
        }
        Toast.makeText((Context)this, (CharSequence)s, 1).show();
}
```

- 调用了`realcheck`方法，定位到该方法：

```java
public boolean realcheck(final String s) {
        return false;
}
```

- 显然这个逻辑永远为假，我们必须找到真正的判断逻辑。经过观察，该apk中assets目录下包含`xposed_init`文件，说明这是一个Xposed模块。
- 根据其入口类，定位到`com.wrlus.caac.hook.HookEntry`类，发现该类实现了对`realcheck`方法的钩子：

```java
public void handleLoadPackage(final XC_LoadPackage$LoadPackageParam xc_LoadPackage$LoadPackageParam) throws Throwable {
        if (xc_LoadPackage$LoadPackageParam.packageName.equals("cn.edu.cauc.isecca.soperfectapp")) {
            XposedHelpers.findAndHookMethod("cn.edu.cauc.isecca.soperfectapp.MainActivity", xc_LoadPackage$LoadPackageParam.classLoader, "realcheck", new Object[] { String.class, new XC_MethodHook() {
                    protected void afterHookedMethod(final XC_MethodHook$MethodHookParam xc_MethodHook$MethodHookParam) throws Throwable {
                        super.afterHookedMethod(xc_MethodHook$MethodHookParam);
                        final Object[] args = xc_MethodHook$MethodHookParam.args;
                        final boolean b = false;
                        final String s = (String)args[0];
                        boolean b2 = b;
                        if (MainActivity.check1(s)) {
                            b2 = b;
                            if (MainActivity.check2(s)) {
                                b2 = true;
                            }
                        }
                        xc_MethodHook$MethodHookParam.setResult((Object)b2);
                    }
                    
                    protected void beforeHookedMethod(final XC_MethodHook$MethodHookParam xc_MethodHook$MethodHookParam) throws Throwable {
                        super.beforeHookedMethod(xc_MethodHook$MethodHookParam);
                        final Object[] args = xc_MethodHook$MethodHookParam.args;
                        final boolean b = false;
                        final String s = (String)args[0];
                        boolean b2 = b;
                        if (MainActivity.check1(s)) {
                            b2 = b;
                            if (MainActivity.check2(s)) {
                                b2 = true;
                            }
                        }
                        xc_MethodHook$MethodHookParam.setResult((Object)b2);
                    }
                } });
        }
    }
```

- 观察发现，其调用了`MainActivity`中的`check1`和`check2`方法，同时满足才可以通过检测，转到这两个方法发现其为native方法：

```java
public static native boolean check1(final String p0);
public static native boolean check2(final String p0);
```

- 解压该apk，寻找到该apk的so文件，使用32位的IDA Pro打开其ARM版本的so文件，定位到这两个方法：

```cpp
int __fastcall Java_cn_edu_cauc_isecca_soperfectapp_MainActivity_check1(_JNIEnv *a1, int a2, int a3)
```

```cpp
int __fastcall Java_cn_edu_cauc_isecca_soperfectapp_MainActivity_check2(_JNIEnv *a1, int a2, int a3)
```

-  发现其逻辑基本一致，规定其长度为29，并且检查了一些字符，同时因为存在`CallStaticBooleanMethod`，所以还调用了Java方法，定位到该方法：

```java
package cn.edu.cauc.isecca.soperfectapp;

import android.util.*;

public class Cpp2JavaBridge
{
    private static String[] keywords;
    
    static {
        Cpp2JavaBridge.keywords = new String[] { "ZmxhZ3tYcG9zZWQ=", "Y2Fu", "SG9vaw==" };
    }
    
    public static boolean chk(final String s) {
        try {
            final String[] split = s.split("_");
            for (int i = 0; i < 3; ++i) {
                if (!split[i].equals(new String(Base64.decode(Cpp2JavaBridge.keywords[i], 0)))) {
                    return false;
                }
            }
            return true;
        }
        catch (Exception ex) {
            ex.printStackTrace();
            return false;
        }
    }
}
```

- 结合以上代码，得知check1需要满足的条件是：

```
flag{Xposed_can_Hook_any?????}
```

- check2需要满足的条件是：

```
flag{Xposed_can_Hook_???thing}
```

- 综合考虑，flag为：

```
flag{Xposed_can_Hook_anything}
```

## 4. 打个气（400pt）
- 主要考点：Windows程序逆向、Windows程序动态调试
- 工具：IDA Pro/OllyDbg

- 题目给出一exe文件，运行之发现要求我们猜数字，如果猜错就退出，所以考虑使用静态分析结合动态调试进行绕过，使用32位的IDA Pro打开文件，定位到主函数：

```c
int __cdecl main(int argc, const char **argv, const char **envp)
```

- 发现需要连续调用六次`sub_402B20()`成功之后才可能显示flag，定位到该函数：

```c
signed int sub_402B20()
```

- 观察其逻辑发现，前五次猜测需要猜中1-6之间的一个随机数，而第六次猜测输入的数字是1-6，但是生成的数字却是7-12之间的，所以不可能猜对。先尝试直接逆向生成flag的算法，定位到`sub_401500()`：

```c
int sub_401500()
```

- 发现逻辑过于复杂，不便于分析，本题最佳解法不是分析这个函数，而是进行动态调试，观察`sub_402B20()`的汇编代码：

```asm
.text:00402C10 loc_402C10:                             ; CODE XREF: sub_402B20+D1↑j
```

- 发现程序通过cmp比较之后进行jnz跳转，所以我们只需要在jnz处下断点，并且修改标志寄存器中ZF标志位的值，即可实现每次都判断为成功的逻辑。

```asm
cmp     eax, [ebp+var_C]
```

- 经过动态调试绕过六次猜数字游戏后，得到输出的flag为：

```
flag{0llyDbg0rIDAPr0_To_DebUg_A5_weLL}
```