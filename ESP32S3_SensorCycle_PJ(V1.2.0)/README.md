| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C6 | ESP32-H2 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- | -------- |

# _Sample project_

(See the README.md file in the upper level 'examples' directory for more information about examples.)

This is the simplest buildable example. The example is used by command `idf.py create-project`
that copies the project to user specified path and set it's name. For more information follow the [docs page](https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html#start-a-new-project)



## How to use example
We encourage the users to use the example as a template for the new projects.
A recommended way is to follow the instructions on a [docs page](https://docs.espressif.com/projects/esp-idf/en/latest/api-guides/build-system.html#start-a-new-project).

## Example folder contents

The project **sample_project** contains one source file in C language [main.c](main/main.c). The file is located in folder [main](main).

ESP-IDF projects are built using CMake. The project build configuration is contained in `CMakeLists.txt`
files that provide set of directives and instructions describing the project's source files and targets
(executable, library, or both). 

Below is short explanation of remaining files in the project folder.

```
├── CMakeLists.txt
├── main
│   ├── CMakeLists.txt
│   └── main.c
└── README.md                  This is the file you are currently reading
```
Additionally, the sample project contains Makefile and component.mk files, used for the legacy Make based build system. 
They are not used or needed when building with CMake and idf.py.
 
# [RICKY NOTE]
## idf_component_register
idf_component_register 是 ESP-IDF 构建系统中的一个宏，用于注册和配置组件。
在使用 ESP-IDF 框架进行开发时，项目通常由多个组件组成。
每个组件可以包含源文件、头文件、依赖项等。
idf_component_register 宏帮助定义这些组件的属性和依赖关系，并将它们注册到构建系统中。

### idf_component_register 宏的语法：
```
idf_component_register(SRCS <source_files>                   --------指定组件的源文件列表。
                       INCLUDE_DIRS <include_directories>    --------指定组件的头文件目录列表。
                       REQUIRES <required_components>)       --------指定组件依赖的其他组件
```