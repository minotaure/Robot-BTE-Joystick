fichier_entree="code_internet_preprocessed.txt" #fichier html où l'on a rajouté de \ avant les ""
fichier_sortie="web_interface.h"

###########

print("Initialisation...")

f = open(fichier_entree)


lignes=["#define HTML_CODE " ]

t = f.readline()

while t:
    if t!= "\n":
        lignes.append("\""+t[:-1]+"\"\\"+"\n")
    t=f.readline()

##########

print("Ecriture .h ...")

fo = open(fichier_sortie,"w")

for ligne in lignes :
    fo.write(ligne)

f.close()
fo.close()

