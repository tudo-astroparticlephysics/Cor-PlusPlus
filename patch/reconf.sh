rm -f ./Makefile.in ./*/Makefile.in ./*/*/Makefile.in ./*/*/*/Makefile.in
rm -f ./configure
autoreconf -v $*
#libtoolize --force
#set name=`ls -l config/ltmain.sh | cut -d ">" -f 2-`
#rm -f config/ltmain.sh
#cp -f $name config/
#if(-d m4)then
#echo m4
#set name=`ls -l m4/libtool.m4 | cut -d ">" -f 2-`
#set name="$name "`ls -l m4/lt~obsolete.m4 | cut -d ">" -f 2-`
#set name="$name "`ls -l m4/ltoptions.m4 | cut -d ">" -f 2-`
#set name="$name "`ls -l m4/ltsugar.m4 | cut -d ">" -f 2-`
#set name="$name "`ls -l m4/ltversion.m4 | cut -d ">" -f 2-`
#rm -f m4/*
#cp -f $name m4/
#endif


