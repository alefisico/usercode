
import sys

MCline = sys.argv[1]
dataline = sys.argv[2]

listMC = MCline.split('&')
listMC.remove( listMC[0] )

tmplist = []

for i in listMC:

    a = i.split()
    #print a
    tmplist.append( a[0] )

listMC = tmplist

listdata = dataline.split('&')
listdata.remove( listdata[0] )


ratiolist = []

print listMC
print listdata

for i in range(len(listMC)):

    ratio = float(listMC[i])/float(listdata[i])

    ratiolist.append( round(ratio,3) )

print ratiolist

line = "MC/Data"
sp = " & "

for i in ratiolist:

    line += sp + str(i)

print line+" \\\\"
