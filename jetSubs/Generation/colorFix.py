#!/usr/bin/env python
import sys
import os
import subprocess

if len(sys.argv) < 2:
    print "Give me a file plz"
    sys.exit()

#Things to be commented out: VMIX, NMIX, UMIX
#(17,5,5) number of lines to be commented out...

infile = sys.argv[1]
outfile= infile.split('.')[0]+'_fixed.lhe'
lines  = open(infile).read()
text   = lines.split('<event>')[0]
##Remove header##
text   = text.split('<header>')[0][:-1] + text.split('</header>')[1].split('<event>')[0]
##\Remove header#
orig   = text ##DD: take me out
events = lines.split('<event>')[1:]
ievent = 0 ##DD:Take me out
for event in events:
    ievent+=1
    #if ievent > 100: continue
    Color = [[],[],[]] #initial, #intermed, #final
    AColor= [[],[],[]] #initial, #intermed, #final
    ColorTrack = [[],[],[]] #initial, #intermed, #final
    AColorTrack= [[],[],[]] #initial, #intermed, #final    
    initState = 0
    finalState= 0
    interState= 0
    passedstop= False
    event = event.split('</event>')[0]
    particles = event.split('\n')[2:-1]
    countPart = 0
    combo = ''
    colorMatch=''
    stopplace   =[]
    stopReplace =[]
    stopPartID  =[]
    colorReplace=[]
    placeholder =[]
    partReplace =[]
    for particle in particles:
        if particle[0] == '#': continue
        allparts = particle.split(' ')
        allparts = filter(None, allparts)
        part = allparts[0]
        if not passedstop and '100000' not in part:
            Color[0].append(allparts[4])
            AColor[0].append(allparts[5])
            initState+=1
        elif '100000' in part:
            interState+=1
            Color[1].append(allparts[4])
            AColor[1].append(allparts[5])
            passedstop = True
            stopplace.append(countPart)
            stopPartID.append(int(part))
            if int(part)   >0: stopReplace.append(allparts[4])
            elif int(part) <0: stopReplace.append(allparts[5])            
        elif  (countPart > 3 and countPart < 8):
            combo+= str(part)
        else:
            finalState +=1
            Color[2].append(allparts[4])
            AColor[2].append(allparts[5])
        if  (countPart > 3 and countPart < 8) and countPart%2 == 0:
            if int(part) > 0: colorMatch = allparts[4]
            elif int(part) <0:
                placeholder.append(countPart)
                colorReplace.append(allparts[5])
                partReplace.append(0)
        if  (countPart > 3 and countPart < 8) and countPart%2 == 1:
            placeholder.append(countPart)
            partReplace.append(int(part))
            if int(part)   >0: colorReplace.append(allparts[4])
            elif int(part) <0: colorReplace.append(allparts[5])
        countPart += 1
    if not(combo == '-1-313' or combo == '13-1-3' or combo == '-3-535' or combo == '35-3-5'):
        print "Check charges of all particles in this event, something is fishy. and broken."
        print event
    if initState != 2 or interState != 2: print "initialState: "+str(initState)+", intermedState: "+str(interState)+", finalState: "+str(finalState)
    #Pruning for color-singlets in all states...
    countPart = 0
    for j in range(0,3):
        for i in range(0,len(Color[j])):
            if int(Color[j][i]) >0  and Color[j][i] not in AColor[j]:
                ColorTrack[j].append(Color[j][i])
            if int(AColor[j][i]) >0  and AColor[j][i] not in Color[j]:
                AColorTrack[j].append(AColor[j][i])


    foundColor = ''
    foundAColor= ''
    for color in ColorTrack[2]:
        if not(color in AColorTrack[2] or color in ColorTrack[0]):
            foundAColor+=color
    for color in AColorTrack[2]:
        if not(color in ColorTrack[2] or color in AColorTrack[0]):
            foundColor+=color
    if not foundColor:
        for color in ColorTrack[0]:
            if not (color in ColorTrack[2]):
                foundColor+=color
    if not foundAColor:
        for color in AColorTrack[0]:
            if not (color in AColorTrack[2]):
                foundAColor+=color

    foundstopColor = ''
    foundstopAColor= ''
    #if len(ColorTrack[1]) == 0: continue ##DD: Just for testing...
    if len(ColorTrack[1]) == 0:
        for color in Color[1]:
            if not int(color) > 0: continue
#            if color in ColorTrack[0] and color in ColorTrack[2]:
#                foundstopColor = color
            #elif not(color in ColorTrack[0] or color in ColorTrack[2]):
            #else: 
            foundstopColor = foundColor
        for color in AColor[1]:
            if not int(color) > 0: continue
            #if color in AColorTrack[0] and color in AColorTrack[2]:
            #    foundstopAColor = color
            #elif not(color in AColorTrack[0] or color in AColorTrack[2]):
            #else:
            foundstopAColor = foundAColor

    orig +='<event>\n'+event+'</event>\n'
    text +='<event>\n'
    text += event.split('\n')[1]+'\n'
    for particle in particles:
        if len(ColorTrack[1]) == 0:
            if countPart in stopplace:
                spot = stopplace.index(countPart)
                if stopPartID[spot] > 0:
                    particle = particle.replace(stopReplace[spot],foundstopColor,1)
                elif stopPartID[spot] < 0:
                    particle = particle.replace(stopReplace[spot],foundstopAColor,1)
        if countPart in placeholder:
            spot = placeholder.index(countPart)
            if partReplace[spot] > 0:
                particle = particle.replace(colorReplace[spot],foundColor,1)
            elif partReplace[spot] < 0:
                particle = particle.replace(colorReplace[spot],foundAColor,1)
            elif partReplace[spot]== 0:
                particle = particle.replace(colorReplace[spot],colorMatch,1)
        text+=particle+'\n'
        countPart +=1
    text+='</event>\n'

text+='</LesHouchesEvents>\n'
orig+='</LesHouchesEvents>\n'
#print text
outf = open(outfile,'w')
outf.write(text)
outf.close()
original = 'orig_'+outfile
outf2= open(original,'w')
outf2.write(orig)
outf2.close()
print ievent

#one if color in initial state, not in final state. select color
    
#two if color in transition state, check to see if in final state in acolor final state
#Read in event:
#For each event, separate lines into {initial state, intermediate state, final state}
#for initial state, check to be sure it only *always* includes two lines (stop should indicate intermed. phase)
#for intermediate state, check to be sure it's always stop/stop
#for intermediate state, check to be sure that +stop has color, -stop has anti-color
#for final state, check that particle pairs 1/3 are always made first... then other particles

#technical note: color-flow particles are indicated by 500's: int(number)/100 gives 5...

#next ID color flow integers#
#in intial state:
#store value of color and anti-color separately
#check to see if value is in both color/anti-color, if so, remove from list

#in intermediate state:
#store value of color anti-color separately

#in final state
#first, identify +- 1/3 pairs
#second, identify all other particles


