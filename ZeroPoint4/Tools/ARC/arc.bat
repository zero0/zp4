
ant arc
rem dir "*.java" /B /S > compile.list
rem type nul > compile.txt
rem for /F "delims=" %%i in (compile.list) do "%%i" >> compile.txt

rem javac -classpath "./lib/*";"./lib/lib/*" -d "./bin" @compile.txt

rem java -cp "./bin/";"./lib/*";"./lib/lib/*" org.zero0.zeropoint.tools.arc.workspace.Main
