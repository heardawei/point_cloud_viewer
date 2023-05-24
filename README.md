# 可视化点云

## 编译步骤

### 使用vcpkg安装PCL与Qt库（如果您已安装vcpkg、PCL和Qt，请跳过本步骤）

#### 类Unix

Linux平台前置条件:

* Git
* g++ >= 6

安装Linux开发者工具

* Debian，Ubuntu，popOS 或其他基于 Debian 的发行版:

```bash
$ sudo apt-get update
$ sudo apt-get install build-essential tar curl zip unzip
```

* CentOS

```bash
$ sudo yum install centos-release-scl
$ sudo yum install devtoolset-7
$ scl enable devtoolset-7 bash
```

macOS 平台前置条件:

* Apple Developer Tools

安装macOS开发者工具

```bash
$ xcode-select --install
```

安装vcpk

```bash
$ git clone https://github.com/microsoft/vcpkg
$ ./vcpkg/bootstrap-vcpkg.sh
```

安装PCL、Qt库

```bash
$ ./vcpkg/vcpkg install pcl[qt, visualization] --recurse
```

#### Windows

前置条件:

* Windows 7 或更新的版本
* Git
* Visual Studio 2015 Update 3 或更新的版本（包含英文语言包）

安装vcpk

```powershell
> git clone https://github.com/microsoft/vcpkg
> .\vcpkg\bootstrap-vcpkg.bat
```

安装PCL、Qt库

```powershell
$ .\vcpkg\vcpkg install pcl[qt, visualization] --recurse --triplet=x64-windows
```

### 在本项目中使用 vcpkg

> 注：请以`您的真实vcpkg目录`替换本章节中出现的`[vcpkg root]`

#### 使用`Visual Studio Code 中的 CMake Tools扩展` 编译本项目

将以下内容添加到您的工作区的 settings.json 中将使 CMake Tools 自动使用 vcpkg 中的第三方库:

```json
{
  "cmake.configureSettings": {
    "CMAKE_TOOLCHAIN_FILE": "[vcpkg root]/scripts/buildsystems/vcpkg.cmake"
  }
}
```

#### 使用`Visual Studio`编译本项目

打开 CMake 设置选项，将 vcpkg toolchain 文件路径在 CMake toolchain file 中：

```cmake
[vcpkg root]/scripts/buildsystems/vcpkg.cmake
```

#### 在`CLion`中编译本项目

打开 Toolchains 设置 (File > Settings on Windows and Linux, CLion > Preferences on macOS)， 并打开 CMake 设置 (Build, Execution, Deployment > CMake)。 最后在 CMake options 中添加以下行:

```bash
-DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake
```

您必须手动将此选项加入每个项目配置文件中。

#### 将 vcpkg 作为一个子模块

您仅需要将CMakeLists.txt中的第一行取消注释，并替换`[vcpkg root]`即可：

例

```cmake
#set(VCPKG_ROOT [vcpkg root])
```

改成

```cmake
set(VCPKG_ROOT "D:/vcpkg")
```

### 第一次编译完成后，运行可能会失败

![platform初始化失败](/docs/img/platform_not_initialized.png)

解决方案:

拷贝`[vcpkg root]/installed/[triplet]/Qt6/plugins/platforms`文件夹到exe文件所在目录
