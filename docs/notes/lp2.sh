# sort -k3 coloana 3
# tr -s schimba caractere

#!/bin/sh 

#1)
# deschizi file
# usernames care se termina cu o litera intre m si z
# care folosesc ca și interpretor de login programul /sbin/nologin
# primii 15
# redirect
cut -d : -f 1,7 /etc/passwd | grep "/sbin/nologin" | grep -E "[.]*[m-z]:" | head -n 15 > max15.txt

#2)
# user - bash,
# terminale desemnate prin ID-uri (i.e., pts/XY) 
# ce încep cu cifrele 0, 1, 2 sau 5, 
# sort -r pid
# write procese
# redirect
ps -axo user,cmd,pid,tty | tr -s " " ":"| grep -e ":pts/[0|2|3|5][.]*" | sort -t: -nk3 | grep ":bash:" > output.txt
