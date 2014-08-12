
dir *.java /B /S > compile.list
javac -classpath ./lib/swt-x64.jar;./lib/hsqldb.jar -d ./bin @compile.list
java -cp ./bin/;./lib/swt-x64.jar;./lib/hsqldb.jar org.zero0.zeropoint.tools.arc.workspace.Main
