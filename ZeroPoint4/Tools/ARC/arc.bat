
dir *.java /B /S > compile.list
javac -classpath ./lib/*;./lib/lib/*; -d ./bin @compile.list

java -cp ./bin/;./lib/*;./lib/lib/* org.zero0.zeropoint.tools.arc.workspace.Main
