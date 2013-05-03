int local(int argc, char* argv[]);

/**
 * @arg 
 * -f file name
 * -e erreur
 * -l nombre (de solutions à afficher)
 **/
#define SHOW_ARGS_NOMAL(x) {cerr<<x<<endl<<"Les arguments sont:\n\
    -f path (de fichier mgf),\n\
    -e erreur,\n\
    -l nombre (de solution à lister),\n\
    -m masse max d'un trou à considérer\n\
    -i ignorer les charges des peaks dans le fichier, \n\
    -bdd base de donnée (Harpe par défaut), \n\
    -pass mot de passe de l'utilisateur de la basse de donnée (root par défaut), \n\
    -user utilisateur de la basse de donnée (root par défaut), -host adresse ip du serveur (127.0.0.1 par défaut), \n\
    -port port de connextion à la base donnée (3306 par défaut)"<<endl;return 1;}

