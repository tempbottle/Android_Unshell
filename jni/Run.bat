@echo off
color 0A
set base_dir=%~dp0
%
base_dir:~0,2%
pushd %base_dir%
CALL ndk-build.cmd
PAUSE