set dest_path=%1
cd /d %~dp0

if "%dest_path%"=="" ( echo dest_path is null) else (
echo dest_path is "%dest_path%

echo f|xcopy /y /s /i /f .\textures\*                            %dest_path%\textures\
)

rem pause 