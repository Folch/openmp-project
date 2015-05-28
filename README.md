# openmp-project
It's very important add this line in *.pro file:

``LIBS += `pkg-config opencv --libs` ``

Copy names of files inside 'image.txt':
``for i in $(ls); do p=`pwd`; echo $p"/"$i;done``
