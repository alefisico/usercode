####################################################
###
###  AWK Script to Check the Colour Conection of
###  particles generated in Madgraph before 
###  simulation using Pythia6 
###
###  Alejandro Gomez Espinosa
###  gomez@physics.rutgers.edu
###
###  How to run: awk -f thisFile.awk input.lhe > output.lhe
###
####################################################

BEGIN{
}
/<event>/ {  				# Search begining of each event
	linecount=-1;    		# To identify the particles
	initialcolour11=-999;   	# Initial colour of the first particle (11 12)
	initialcolour12=-999;
	initialcolour21=-999;   	# Initial colour of the second particle (21 22)
	initialcolour22=-999;
	initialcolour1=-999;   		# initial colour particle to be assign
	initialcolour2=-999;   		# initial colour ANTIparticle to be assign
	mother1=0;   			# last mother particle of the intermediate state
	mother2=0; 			# last mother antiparticle for the intermediate state
	daughters1=0; 			#number of daughets of mother1
	daughters2=0; 			#number of daughets of mother2
	colourmatch=0; 			#number of the colour remaind for matching
	checkID=0;  			#dummy variable

}

#Start processing event
{
	#Skip the first line after event
	#Check colour of initial partons
	if($2==-1){					# -1 for initial state particles
		if(linecount==1){
			initialcolour11=$5;		# Record initial value of colour
			initialcolour12=$6;
		}
		#print initialcolour11, initialcolour12;
		if(linecount==2){
			initialcolour21=$5;
			initialcolour22=$6;
		}
		#print initialcolour21, initialcolour22;
	}
	
	#Check which colour must be conserve at the end
	if(initialcolour11==initialcolour22){
		initialcolour1=initialcolour21;
		initialcolour2=initialcolour12;
	}
	else if(initialcolour12==initialcolour21){
		initialcolour1=initialcolour11;
		initialcolour2=initialcolour22;
	}
	#else print "Something is wrong";
	#print initialcolour1, initialcolour2;


	#Check colour in intermediate particles
	if($2==2){ 						# 2 is for intermediate particles
		if($3!=$4){					# For decaying particles right after initial state
			if($5!=$6){				# If the colour is the same something is wrong or if 0 we do not care about colour
				if($1>0){			# For particles
					mother1=linecount;	# Record number of mother particle
					$5=initialcolour1;	# Assign colour of initial particle
					$6=0;
				}
				else{				# Assign colour of initial ANTIparticle
					mother2=linecount;
					$6=initialcolour2;
					$5=0;
				}
			}
			else print "Intermediate particle does not have colour" 
		}
		else{						# If mothers are the same, particles come from an intermediate particle
			if($5!=$6){
				if($1>0){
					mother1=linecount;	# Record number of mother particle
					$5=initialcolour1;	# Assign colour of inital particle
					$6=0;
				}
				else{
					mother2=linecount;
					$6=initialcolour2;	# Assign colour of the initial ANTIparticle
					$5=0;
				}
			}
		}
	}

	# "Final state" particle
	else if($2==1){  					# 1 for "final" state particles
		#print mother1, mother2;
		if($3==mother1){				# For particles which mother is a particle
			daughters1++;
			if(daughters1==1){			# First daughter as a particle
				if($1<0) $1=-$1;
				$5=initialcolour1;
				$6=0;
			}
			if(daughters1==2){			# Second daughter as ANTIparticle
				if($1>0) $1=-$1;
				if($5!=0) colourmatch=$5;	# Record colour number for the "untouch" particle 
				else colourmatch=$6;
			}
			#print colourmatch1;
		}
		if($3==mother2){				# For particles which mother is a ANTIparticle
			daughters2++;
			if(daughters2==1){
				if($1>0) $1=-$1;
			       	#checkID=$1;		# Dummy variable, to check particle-ANTIparticle consistency
			       	$6=initialcolour2;
			       	$5=0;
		        }
			if(daughters2==2){
				#if($1>0 && checkID>0){			# If first is particle, second must be ANTIparticle
				if($1<0) $1=-$1;
				if($6!=0) $6=colourmatch;	# Record colour number for the "untouch" particle 
				else $5=colourmatch;
			}
		}
	}

	linecount++;				# Count the number of lines, i.e. check mothers
}

{
	if(lineprint==0) print $0;
}
END {
}
