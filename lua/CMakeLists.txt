CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

PROJECT(lua-sql-parse)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g")
SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -O0 -g")

option(USE_LUA "Use Lua (also called 'C' Lua) includes (default)" ON)
option(USE_LUAJIT "Use LuaJIT includes instead of 'C' Lua ones (recommended, if you're using LuaJIT, but disabled by default)")
set(USE_LUA_VERSION 5.1 CACHE STRING "Set the Lua version to use (default: 5.1)")

if(USE_LUAJIT)
# Find luajit
        find_package(LuaJIT REQUIRED)
        set(USE_LUA OFF)
# / Find lua
endif()

if(USE_LUA)
# Find lua
#        find_package(Lua ${USE_LUA_VERSION} EXACT REQUIRED)
# / Find lua
endif()


# Basic configurations
SET(INSTALL_CMOD share/lua/cmod CACHE PATH "Directory to install Lua binary modules (configure lua via LUA_CPATH)")
# / configs

SET(LUA_C_MODEL_FILE sql_parse.c table_util.c to_lua.c ../src/alter.c  ../src/callback.c  ../src/expr.c 
		../src/os_unix.c ../src/printf.c ../src/trigger.c  ../src/util.c
		../src/build.c ../src/delete.c   ../src/insert.c  ../src/os.c
		../src/parse.c   ../src/select.c  ../src/tokenize.c   ../src/update.c
		../src/where.c)

AUX_SOURCE_DIRECTORY(. ${CMAKE_SOURCE_DIR}/src/)

# Define how to build *.so:
  LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/)

  INCLUDE_DIRECTORIES((${CMAKE_SOURCE_DIR}/include/ ${LUA_INCLUDE_DIR}))
  ADD_LIBRARY(lua_cmod_sql_parse MODULE
    ${LUA_C_MODEL_FILE} )
  SET_TARGET_PROPERTIES(lua_cmod_sql_parse PROPERTIES PREFIX "")
  SET_TARGET_PROPERTIES(lua_cmod_sql_parse PROPERTIES OUTPUT_NAME "sql_parse")
  # / build sql_parse.so
