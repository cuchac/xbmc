@ECHO OFF

PUSHD %~dp0\..
CALL vswhere.bat x86
IF ERRORLEVEL 1 (
  ECHO ERROR! make-addons.bat: Something went wrong when calling vswhere.bat
  POPD
  EXIT /B 1
)
CALL make-addons.bat %*
CALL make-addons.bat game.shader.presets
POPD
