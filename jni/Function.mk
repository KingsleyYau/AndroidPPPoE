# Copyright (C) 2014 The PPOE Project
# Makefile Common Function
#
# Created on: 2014年2月14日
# Author: Kingsley Yau
# Email: Kingsleyyau@gmail.com
#

# -----------------------------------------------------------------------------
# Macro    : all-cpp-files-under 查找目录所有cpp文件
# Arguments: 1:Real path 目录绝对路径，可以使用realpath或者abspath
# Usage    : $(call all-cpp-files-under)
# Return   : The list of cpp source file
# -----------------------------------------------------------------------------
define all-cpp-files-under
$(wildcard $1/*.cpp)
endef

# -----------------------------------------------------------------------------
# Macro    : all-cpp-files 查找目录(递归)所有cpp文件
# Arguments: 1:Real path 目录绝对路径，可以使用realpath或者abspath
# Usage    : $(call all-cpp-files-under)
# Return   : The list of cpp source file
# -----------------------------------------------------------------------------
define all-cpp-files
$(shell find $1 -name "*.cpp")
endef

# -----------------------------------------------------------------------------
# Macro    : all-c-files-under 查找目录所有cpp文件
# Arguments: 1:Real path 目录绝对路径，可以使用realpath或者abspath
# Usage    : $(call all-cpp-files-under)
# Return   : The list of cpp source file
# -----------------------------------------------------------------------------
define all-c-files-under
$(wildcard $1/*.c)
endef

# -----------------------------------------------------------------------------
# Macro    : all-c-files 查找目录(递归)所有cpp文件
# Arguments: 1:Real path 目录绝对路径，可以使用realpath或者abspath
# Usage    : $(call all-cpp-files-under)
# Return   : The list of cpp source file
# -----------------------------------------------------------------------------
define all-c-files
$(shell find $1 -name "*.c")
endef