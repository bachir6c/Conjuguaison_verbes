#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NOMBRE_MAX_VERBS 15000
#define LONGUEUR_MAX_CHAINE 200

void remplacer_chaine(char *ancienne_chaine, char *nouvelle_chaine, char *ancienne_sous_chaine, char *nouvelle_sous_chaine)
{
    int longueur = strlen(ancienne_chaine);
    int longueur_ancien_sous_chaine = strlen(ancienne_sous_chaine);
    int longueur_nouveau_sous_chaine = strlen(nouvelle_sous_chaine);
    int longueur_nouvelle_chaine = longueur + (longueur_nouveau_sous_chaine - longueur_ancien_sous_chaine) + 1;

    int i = 0, j = 0;
    while (i < longueur)
    {
        if (strncmp(&ancienne_chaine[i], ancienne_sous_chaine, longueur_ancien_sous_chaine) == 0)
        {
            memcpy(&nouvelle_chaine[j], nouvelle_sous_chaine, longueur_nouveau_sous_chaine);
            j += longueur_nouveau_sous_chaine;
            i += longueur_ancien_sous_chaine;
        }
        else
        {
            nouvelle_chaine[j] = ancienne_chaine[i];
            i++;
            j++;
        }
    }
    nouvelle_chaine[j] = '\0';
}

typedef struct
{
    char non_ascii[3];
    char ascii[3];
} AccentNonAsciiEnAscii;

AccentNonAsciiEnAscii accents[] = {
    {"é", "e'"},
    {"è", "e`"},
    {"ê", "e^"},
    {"ë", "e:"},
    {"â", "a^"},
    {"î", "i^"},
    {"ï", "i:"},
    {"ô", "o^"},
    {"ö", "o:"},
    {"û", "u^"},
    {"ü", "u:"},
    {"ç", "c,"}};

void remplacer_accents(char *ancienne_chaine, char *nouvelle_chaine, char *mode)
{
    int i, j;
    size_t longueur = strlen(ancienne_chaine);
    size_t nombre_paires = sizeof(accents) / sizeof(AccentNonAsciiEnAscii);
    bool non_ascii_vers_ascii = strcmp(mode, "non_ascii_vers_ascii") == 0;
    bool ascii_vers_non_ascii = strcmp(mode, "ascii_vers_non_ascii") == 0;
    if (!non_ascii_vers_ascii && !ascii_vers_non_ascii)
    {
        printf("Mode invalide\n");
        exit(1);
    }
    for (i = 0; i < longueur; i++)
    {
        int remplace = 0;
        for (j = 0; j < nombre_paires; j++)
        {
            if (non_ascii_vers_ascii && strncmp(&ancienne_chaine[i], accents[j].non_ascii, strlen(accents[j].non_ascii)) == 0)
            {
                strncpy(&nouvelle_chaine[i], accents[j].ascii, strlen(accents[j].ascii));
                i += strlen(accents[j].non_ascii) - 1;
                remplace = 1;
                break;
            }
            else if (ascii_vers_non_ascii && strncmp(&ancienne_chaine[i], accents[j].ascii, strlen(accents[j].ascii)) == 0)
            {
                strncpy(&nouvelle_chaine[i], accents[j].non_ascii, strlen(accents[j].non_ascii));
                i += strlen(accents[j].ascii) - 1;
                remplace = 1;
                break;
            }
        }
        if (!remplace)
        {
            nouvelle_chaine[i] = ancienne_chaine[i];
        }
    }
    nouvelle_chaine[longueur] = '\0';
}

void supprimer_derniers_characteres(char *ancienne_chaine, char *nouvelle_chaine, int nombre_de_caracteres_a_supprimer)
{
    strncpy(nouvelle_chaine, ancienne_chaine, strlen(ancienne_chaine) - nombre_de_caracteres_a_supprimer);
    nouvelle_chaine[strlen(ancienne_chaine) - nombre_de_caracteres_a_supprimer] = '\0';
}

void preparer_racine_pour_affichage(char *verbe_ascii, char *racine_ascii, char *racine, int nombre_de_caracteres_a_supprimer)
{
    supprimer_derniers_characteres(verbe_ascii, racine_ascii, nombre_de_caracteres_a_supprimer);
    remplacer_accents(racine_ascii, racine, "ascii_vers_non_ascii");
}

typedef struct
{
    char *verbe;
    char *format_conjugaison_present;
    char *format_conjugaison_imparfait;
    char *format_conjugaison_passe_simple;
    char *format_conjugaison_futur;
    int number_characters_to_remove;
} RegleDeConjugaison;

RegleDeConjugaison regles_de_conjugaison_premier_groupe[] = {
    {"ger",
     "%s%se\n%s%ses\n%s%se\n%s%seons\n%s%sez\n%s%sent\n",
     "%s%seais\n%s%seais\n%s%seait\n%s%sions\n%s%siez\n%s%saient\n",
     "%s%seai\n%s%seas\n%s%sea\n%s%seâmes\n%s%seâtes\n%s%sèrent\n",
     "%s%serai\n%s%seras\n%s%sera\n%s%serons\n%s%serez\n%s%seront\n",
     2},
    {"cer",
     "%s%sce\n%s%sces\n%s%sce\n%s%sçons\n%s%scez\n%s%scent\n",
     "%s%sçais\n%s%sçais\n%s%sçait\n%s%scions\n%s%sciez\n%s%sçaient\n",
     "%s%sçai\n%s%sças\n%s%sça\n%s%sçâmes\n%s%sçâtes\n%s%scèrent\n",
     "%s%scerai\n%s%sceras\n%s%scera\n%s%scerons\n%s %scerez\n%s%sceront\n",
     3},
    {"eler",
     "%s%selle\n%s%selles\n%s%selle\n%s%selons\n%s%selez\n%s%sellent\n",
     "%s%selais\n%s%selais\n%s%selait\n%s%selions\n%s%seliez\n%s%selaient\n",
     "%s%selai\n%s%selas\n%s%sela\n%s%selâmes\n%s%selâtes\n%s%selèrent\n",
     "%s%sellerai\n%s%selleras\n%s%sellera\n%s%sellerons\n%s%sellerez\n%s%selleront\n", 4},
    {"eter",
     "%s%sette\n%s%settes\n%s%sette\n%s%setons\n%s%setez\n%s%settent\n",
     "%s%setais\n%s%setais\n%s%setait\n%s%setions\n%s%setiez\n%s%setaient\n",
     "%s%setai\n%s%setas\n%s%seta\n%s%setâmes\n%s%setâtes\n%s%setèrent\n",
     "%s%setterai\n%s%setteras\n%s%settera\n%s%setterons\n%s%setterez\n%s%setteront\n",
     4},
    {"ayer",
     "%s%saye\n%s%sayes\n%s%saye\n%s%sayons\n%s%sayez\n%s%sayent\n",
     "%s%sayais\n%s%sayais\n%s%sayait\n%s%sayions\n%s%sayiez\n%s%sayaient\n",
     "%s%sayai\n%s%sayas\n%s%saya\n%s%s%sayâmes\n%s%s%sayâtes\n%s%sayèrent\n",
     "%s%saierai\n%s%saieras\n%s%saiera\n%s%saierons\n%s%saierez\n%s%saieront\n",
     4},
    {"oyer",
     "%s%sie\n%s%sies\n%s%sie\n%s%syons\n%s%syez\n%s%sient\n",
     "%s%syais\n%s%syais\n%s%syait\n%s%syions\n%s%syiez\n%s%syaient\n",
     "%s%syai\n%s%syas\n%s%sya\n%s%syâmes\n%s%syâtes\n%s%syèrent\n",
     "%s%sierai\n%s%sieras\n%s%siera\n%s%sierons\n%s%sierez\n%s%sieront\n",
     3},
    {"uyer",
     "%s%sie\n%s%sies\n%s%sie\n%s%syons\n%s%syez\n%s%sient\n",
     "%s%syais\n%s%syais\n%s%syait\n%s%syions\n%s%syiez\n%s%syaient\n",
     "%s%syai\n%s%syas\n%s%sya\n%s%syâmes\n%s%syâtes\n%s%syèrent\n",
     "%s%sierai\n%s%sieras\n%s%siera\n%s%sierons\n%s%sierez\n%s%sieront\n",
     3},
};

bool commence_en(const char *chaine, const char *prefixe)
{
    size_t longueur_chaine = strlen(chaine);
    size_t longueur_prefixe = strlen(prefixe);
    if (longueur_prefixe > longueur_chaine)
    {
        return 0;
    }
    return strncmp(chaine, prefixe, longueur_prefixe) == 0;
}

bool finit_en(const char *chaine, const char *suffixe)
{
    size_t longueur_chaine = strlen(chaine);
    size_t longueur_suffixe = strlen(suffixe);
    if (longueur_suffixe > longueur_chaine)
    {
        return 0;
    }
    return strcmp(chaine + longueur_chaine - longueur_suffixe, suffixe) == 0;
}

bool conjuguer_avec_des_regles(char *verbe, char *verbe_ascii, char *racine_ascii, char *racine, char pronoms[6][LONGUEUR_MAX_CHAINE], RegleDeConjugaison *regles, int nombre_regles)
{
    for (int i = 0; i < nombre_regles; i++)
    {
        if (finit_en(verbe, regles[i].verbe))
        {
            preparer_racine_pour_affichage(verbe_ascii, racine_ascii, racine, regles[i].number_characters_to_remove);

            char format_conjugaison_present[LONGUEUR_MAX_CHAINE] = "\nPresent:\n";
            strcat(format_conjugaison_present, regles[i].format_conjugaison_present);
            printf(format_conjugaison_present, pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);

            char format_conjugaison_imparfait[LONGUEUR_MAX_CHAINE] = "\nImparfait:\n";
            strcat(format_conjugaison_imparfait, regles[i].format_conjugaison_imparfait);
            printf(format_conjugaison_imparfait, pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);

            char format_conjugaison_passe_simple[LONGUEUR_MAX_CHAINE] = "\nPassé simple:\n";
            strcat(format_conjugaison_passe_simple, regles[i].format_conjugaison_passe_simple);
            printf(format_conjugaison_passe_simple, pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);

            char format_conjugaison_futur[LONGUEUR_MAX_CHAINE] = "\nFutur:\n";
            strcat(format_conjugaison_futur, regles[i].format_conjugaison_futur);
            printf(format_conjugaison_futur, pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);

            return true;
        }
    }
    return false;
}

bool conjuguer_cas_particuliers_premier_groupe(char *verbe, char *verbe_ascii, char *racine_ascii, char *racine, char pronoms[6][LONGUEUR_MAX_CHAINE])
{
    if (finit_en(verbe, "eler") && (strcmp(verbe, "agneler") == 0 || strcmp(verbe, "celer") == 0 || strcmp(verbe, "déceler") == 0 || strcmp(verbe, "receler") == 0 || strcmp(verbe, "ciseler") == 0 || strcmp(verbe, "démanteler") == 0 || strcmp(verbe, "écarteler") == 0 || strcmp(verbe, "encasteler") == 0 || strcmp(verbe, "geler") == 0 || strcmp(verbe, "dégeler") == 0 || strcmp(verbe, "congeler") == 0 || strcmp(verbe, "surgeler") == 0 || strcmp(verbe, "marteler") == 0 || strcmp(verbe, "modeler") == 0 || strcmp(verbe, "peler") == 0))
    {
        preparer_racine_pour_affichage(verbe_ascii, racine_ascii, racine, 4);
        printf("\nPrésent:\n%s%sèle\n%s%sèles\n%s%sèle\n%s%selons\n%s%selez\n%s%sèlent\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
        printf("\nImparfait:\n%s%selais\n%s%selais\n%s%selait\n%s%selions\n%s%seliez\n%s%selaient\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
        printf("\nPassé simple:\n%s%selai\n%s%selas\n%s%sela\n%s%selâmes\n%s%selâtes\n%s%selèrent\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
        printf("\nFutur simple:\n%s%sèlerai\n%s%sèleras\n%s%sèlera\n%s%sèlerons\n%s%sèlerez\n%s%sèleront\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
    }
    else if (finit_en(verbe, "eter") && (strcmp(verbe, "acheter") == 0 || strcmp(verbe, "racheter") == 0 || strcmp(verbe, "bégueter") == 0 || strcmp(verbe, "corseter") == 0 || strcmp(verbe, "crocheter") == 0 || strcmp(verbe, "fileter") == 0 || strcmp(verbe, "fureter") == 0 || strcmp(verbe, "haleter") == 0))
    {
        preparer_racine_pour_affichage(verbe_ascii, racine_ascii, racine, 4);
        printf("\nPrésent:\n%s%sète\n%s%sètes\n%s%sète\n%s%setons\n%s%setez\n%s%sètent\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
        printf("\nImparfait:\n%s%setais\n%s%setais\n%s%setait\n%s%setions\n%s%setiez\n%s%setaient\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
        printf("\nPassé simple:\n%s%setai\n%s%setas\n%s%seta\n%s%setâmes\n%s%setâtes\n%s%sèterent\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
        printf("\nFutur Simple:\n%s%sèterai\n%s%sèteras\n%s%sètera\n%s%sèterons\n%s%sèterez\n%s%sèteront\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
    }
    else if (finit_en(verbe, "ébrer") || finit_en(verbe, "écher") || finit_en(verbe, "écrer") || finit_en(verbe, "égrer") || finit_en(verbe, "éguer") || finit_en(verbe, "équer") || finit_en(verbe, "étrer") || finit_en(verbe, "évrer") || finit_en(verbe, "écer") || finit_en(verbe, "éder") || finit_en(verbe, "éger") || finit_en(verbe, "éler") || finit_en(verbe, "émer") || finit_en(verbe, "éner") || finit_en(verbe, "érer") || finit_en(verbe, "éser") || finit_en(verbe, "éter"))
    {
        preparer_racine_pour_affichage(verbe_ascii, racine_ascii, racine, 2);
        char racine_particuliere_1[LONGUEUR_MAX_CHAINE];
        remplacer_chaine(racine_ascii, racine_particuliere_1, "e'", "è");
        char racine_particuliere_2[LONGUEUR_MAX_CHAINE];
        remplacer_chaine(racine_ascii, racine_particuliere_2, "e'", "é");
        printf("\nPrésent:\n%s%se\n%s%ses\n%s%se\n%s%sons\n%s%sez\n%s%sent\n", pronoms[0], racine_particuliere_1, pronoms[1], racine_particuliere_1, pronoms[2], racine_particuliere_1, pronoms[3], racine_particuliere_2, pronoms[4], racine_particuliere_2, pronoms[5], racine_particuliere_1);
        printf("\nImparfait:\n%s%sais\n%s%sais\n%s%sait\n%s%sions\n%s%siez\n%s%saient\n", pronoms[0], racine_particuliere_2, pronoms[1], racine_particuliere_2, pronoms[2], racine_particuliere_2, pronoms[3], racine_particuliere_2, pronoms[4], racine_particuliere_2, pronoms[5], racine_particuliere_2);
        printf("\nPassé simple:\n%s%sai\n%s%sas\n%s%sa\n%s%sâmes\n%s%sâtes\n%s%sèrent\n", pronoms[0], racine_particuliere_2, pronoms[1], racine_particuliere_2, pronoms[2], racine_particuliere_2, pronoms[3], racine_particuliere_2, pronoms[4], racine_particuliere_2, pronoms[5], racine_particuliere_2);
        printf("\nFutur simple:\n%s%serai\n%s%seras\n%s%sera\n%s%serons\n%s%serez\n%s%seront\n", pronoms[0], racine_particuliere_2, pronoms[1], racine_particuliere_2, pronoms[2], racine_particuliere_2, pronoms[3], racine_particuliere_2, pronoms[4], racine_particuliere_2, pronoms[5], racine_particuliere_2);
    }
    else if (finit_en(verbe, "ecer") || finit_en(verbe, "emer") || finit_en(verbe, "eper") || finit_en(verbe, "erer") || finit_en(verbe, "eser") || finit_en(verbe, "ever") || finit_en(verbe, "evrer"))
    {
        preparer_racine_pour_affichage(verbe_ascii, racine_ascii, racine, 2);
        char racine_particuliere[LONGUEUR_MAX_CHAINE];
        remplacer_chaine(racine, racine_particuliere, "e", "è");
        printf("\nPrésent:\n%s%se\n%s%ses\n%s%se\n%s%sons\n%s%sez\n%s%sent\n", pronoms[0], racine_particuliere, pronoms[1], racine_particuliere, pronoms[2], racine_particuliere, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine_particuliere);
        printf("\nImparfait:\n%s%sais\n%s%sais\n%s%sait\n%s%sions\n%s%siez\n%s%saient\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
        printf("\nPassé simple:\n%s%sai\n%s%sas\n%s%sa\n%s%sâmes\n%s%sâtes\n%s%sèrent\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
        printf("\nFutur simple:\n%s%serai\n%s%seras\n%s%sera\n%s%serons\n%s%serez\n%s%seront\n", pronoms[0], racine_particuliere, pronoms[1], racine_particuliere, pronoms[2], racine_particuliere, pronoms[3], racine_particuliere, pronoms[4], racine_particuliere, pronoms[5], racine_particuliere);
    }
    else
    {
        return false;
    }
    return true;
}

void conjuguer_premier_groupe(char *verbe, char *verbe_ascii, char *racine_ascii, char *racine, char pronoms[6][LONGUEUR_MAX_CHAINE])
{
    if (conjuguer_cas_particuliers_premier_groupe(verbe, verbe_ascii, racine_ascii, racine, pronoms))
    {
        return;
    }

    int nombre_regles_conjugaison = sizeof(regles_de_conjugaison_premier_groupe) / sizeof(regles_de_conjugaison_premier_groupe[0]);
    if (conjuguer_avec_des_regles(verbe, verbe_ascii, racine_ascii, racine, pronoms, regles_de_conjugaison_premier_groupe, nombre_regles_conjugaison))
    {
        return;
    }

    preparer_racine_pour_affichage(verbe_ascii, racine_ascii, racine, 2);
    printf("\nPrésent:\n%s%se\n%s%ses\n%s%se\n%s%sons\n%s%sez\n%s%sent\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
    printf("\nImparfait:\n%s%sais\n%s%sais\n%s%sait\n%s%sions\n%s%siez\n%s%saient\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
    printf("\nPassé simple:\n%s%sai\n%s%sas\n%s%sa\n%s%sâmes\n%s%sâtes\n%s%sèrent\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
    printf("\nFutur simple:\n%s%serai\n%s%seras\n%s%sera\n%s%serons\n%s%serez\n%s%seront\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
}

void conjuguer_deuxieme_groupe(char *verbe, char *verbe_ascii, char *racine_ascii, char *racine, char pronoms[6][LONGUEUR_MAX_CHAINE])
{
    preparer_racine_pour_affichage(verbe_ascii, racine_ascii, racine, 2);
    printf("\nPrésent:\n%s%sis\n%s%sis\n%s%sit\n%s%sissons\n%s%sissez\n%s%sissent\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
    printf("\nImparfait:\n%s%sissais\n%s%sissais\n%s%sissait\n%s%sissions\n%s%sissiez\n%s%sissaient\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
    printf("\nPassé simple:\n%s%sis\n%s%sis\n%s%sit\n%s%sîmes\n%s%sîtes\n%s%sirent\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
    printf("\nPassé simple:\n%s%sirai\n%s%siras\n%s%sira\n%s%sirons\n%s%sirez\n%s%siront\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
}

RegleDeConjugaison regles_de_conjugaison_troisieme_groupe[] = {
    {"prendre",
     "%s%sprends\n%s%sprends\n%s%sprend\n%s%sprenons\n%s%sprenez\n%s%sprennent\n",
     "%s%sprenais\n%s%sprenais\n%s%sprenait\n%s%sprenions\n%s%spreniez\n%s%sprenaient\n",
     "%s%spris\n%s%spris\n%s%sprit\n%s%sprîmes\n%s%sprîtes\n%s%sprirent\n",
     "%s%sprendrai\n%s%sprendras\n%s%sprendra\n%s%sprendrons\n%s%sprendrez\n%s%sprendront\n",
     7},
    {"coudre",
     "%s%scouds\n%s%scouds\n%s%scoud\n%s%scousons\n%s%scousez\n%s%scousent\n",
     "%s%scousais\n%s%scousais\n%s%scousait\n%s%scousions\n%s%scousiez\n%s%scousaient\n",
     "%s%scousis\n%s%scousis\n%s%scousit\n%s%scousîmes\n%s%scousîtes\n%s%scousirent\n",
     "%s%scoudrai\n%s%scoudras\n%s%scoudra\n%s%scoudrons\n%s%scoudrez\n%s%scoudront\n",
     6},
    {"soudre",
     "%s%sous\n%s%sous\n%s%sout\n%s%solvons\n%s%solvez\n%s%solvent\n",
     "%s%solvais\n%s%solvais\n%s%solvait\n%s%solvions\n%s%solvez\n%s%solvent\n",
     "%s%solus\n%s%solus\n%s%solut\n%s%solûmes\n%s%solûtes\n%s%solurent\n",
     "%s%soudrai\n%s%soudras\n%s%soudra\n%s%soudrons\n%s%soudrez\n%s%soudront\n",
     5},
    {"moudre",
     "%s%souds\n%s%souds\n%s%soud\n%s%soulons\n%s%soulez\n%s%soulent\n",
     "%s%soulais\n%s%soulais\n%s%soulait\n%s%soulions\n%s%souliez\n%s%soulaient\n",
     "%s%soulus\n%s%soulus\n%s%soulut\n%s%soulûmes\n%s%soulûtes\n%s%soulurent\n",
     "%s%soudrai\n%s%soudras\n%s%soudra\n%s%soudrons\n%s%soudrez\n%s%soudront\n",
     5},
    {"dormir",
     "%s%ss\n%s%ss\n%s%st\n%s%smons\n%s%smez\n%s%sment\n",
     "%s%smais\n%s%smais\n%s%smait\n%s%smions\n%s%smiez\n%s%smaient\n",
     "%s%smis\n%s%smis\n%s%smit\n%s%smîmes\n%s%smîtes\n%s%smirent\n",
     "%s%smirai\n%s%smiras\n%s%smira\n%s%smirons\n%s%smirez\n%s%smiront\n",
     3},
    {"mourir",
     "%s%seurs\n%s%seurs\n%s%seurt\n%s%sourons\n%s%sourez\n%s%seurent\n",
     "%s%sourais\n%s%sourais\n%s%sourait\n%s%sourions\n%s%souriez\n%s%souraient\n",
     "%s%sourus\n%s%sourus\n%s%sourut\n%s%sourûmes\n%s%sourûtes\n%s%soururent\n",
     "%s%sourrai\n%s%sourras\n%s%sourra\n%s%sourrons\n%s%sourrez\n%s%sourront\n",
     5},
    {"indre",
     "%s%sins\n%s%sins\n%s%sint\n%s%signons\n%s%signez\n%s%signent\n",
     "%s%signais\n%s%signais\n%s%signait\n%s%signions\n%s%signiez\n%s%signaient\n",
     "%s%signis\n%s%signis\n%s%signit\n%s%signîmes\n%s%signîtes\n%s%signirent\n",
     "%s%sindrai\n%s%sindras\n%s%sindra\n%s%sindrons\n%s%sindrez\n%s%sindront\n",
     5},
    {"dre",
     "%s%ss\n%s%ss\n%s%s\n%s%sons\n%s%sez\n%s%sent\n",
     "%s%sais\n%s%sais\n%s%sait\n%s%sions\n%s%siez\n%s%saient\n",
     "%s%sis\n%s%sis\n%s%sit\n%s%sîmes\n%s%sîtes\n%s%sirent\n",
     "%s%srai\n%s%sras\n%s%sra\n%s%srons\n%s%srez\n%s%sront\n",
     2},
    {"mettre",
     "%s%sets\n%s%sets\n%s%set\n%s%settons\n%s%settez\n%s%settent\n",
     "%s%settais\n%s%settais\n%s%settait\n%s%settions\n%s%settiez\n%s%settaient\n",
     "%s%sis\n%s%sis\n%s%sit\n%s%sîmes\n%s%sîtes\n%s%sirent\n",
     "%s%settrai\n%s%settras\n%s%settra\n%s%settrons\n%s%settrez\n%s%settront\n",
     5},
    {"battre",
     "%s%sts\n%s%sts\n%s%st\n%s%sttons\n%s%sttez\n%s%sttent\n",
     "%s%sttais\n%s%sttais\n%s%sttait\n%s%sttions\n%s%sttiez\n%s%sttaient\n",
     "%s%sttis\n%s%sttis\n%s%sttit\n%s%sttîmes\n%s%sttîtes\n%s%sttirent\n",
     "%s%sttrai\n%s%sttras\n%s%sttra\n%s%sttrons\n%s%sttrez\n%s%sttront\n",
     4},
    {"cevoir",
     "%s%sçois\n%s%sçois\n%s%sçoit\n%s%scevons\n%s%scevez\n%s%sçoivent\n",
     "%s%scevais\n%s%scevais\n%s%scevait\n%s%scevions\n%s%sceviez\n%s%scevaient\n",
     "%s%sçus\n%s%sçus\n%s%sçut\n%s%sçûmes\n%s%sçûtes\n%s%sçurent\n",
     "%s%scevrai\n%s%scevras\n%s%scevra\n%s%scevrons\n%s%scevrez\n%s%scevront\n",
     6},
    {"enir",
     "%s%siens\n%s%siens\n%s%sient\n%s%senons\n%s%senez\n%s%sienent\n",
     "%s%senais\n%s%senais\n%s%senait\n%s%senions\n%s%seniez\n%s%senaient\n",
     "%s%sins\n%s%sins\n%s%sint\n%s%sînmes\n%s%sîntes\n%s%sinrent\n",
     "%s%siendrai\n%s%siendras\n%s%siendra\n%s%siendrons\n%s%siendrez\n%s%siendront\n",
     4},
    {"quérir",
     "%s%squiers\n%s%squiers\n%s%squiert\n%s%squérons\n%s%squérez\n%s%squièrent\n",
     "%s%squérais\n%s%squérais\n%s%squérait\n%s%squérions\n%s%squériez\n%s%squéraient\n",
     "%s%squis\n%s%squis\n%s%squit\n%s%squîmes\n%s%squîtes\n%s%squirent\n",
     "%s%squerrai\n%s%squerras\n%s%squerra\n%s%squerrons\n%s%squerrez\n%s%squerront\n",
     7},
    {"vêtir",
     "%s%svêts\n%s%svêts\n%s%svêt\n%s%svêtons\n%s%svêtez\n%s%svêtent\n",
     "%s%svêtais\n%s%svêtais\n%s%svêtait\n%s%svêtions\n%s%svêtiez\n%s%svêtaient\n",
     "%s%svêtis\n%s%svêtis\n%s%svêtit\n%s%svêtîmes\n%s%svêtîtes\n%s%svêtirent\n",
     "%s%svêtirai\n%s%svêtiras\n%s%svêtira\n%s%svêtirons\n%s%svêtirez\n%s%svêtiront\n",
     6},
    {"courir",
     "%s%ss\n%s%ss\n%s%st\n%s%sons\n%s%sez\n%s%sent\n",
     "%s%sais\n%s%sais\n%s%sait\n%s%sions\n%s%siez\n%s%saient\n",
     "%s%sus\n%s%sus\n%s%sut\n%s%sûmes\n%s%sûtes\n%s%surent\n",
     "%s%srai\n%s%sras\n%s%sra\n%s%srons\n%s%srez\n%s%sront\n",
     2},
    {"fuir",
     "%s%sis\n%s%sis\n%s%sit\n%s%syons\n%s%syez\n%s%sient\n",
     "%s%syais\n%s%syais\n%s%syait\n%s%syions\n%s%syiez\n%s%syaient\n",
     "%s%sis\n%s%sis\n%s%sit\n%s%sîmes\n%s%sîtes\n%s%sirent\n",
     "%s%sirai\n%s%siras\n%s%sira\n%s%sirons\n%s%sirez\n%s%siront\n",
     2},
    {"ir",
     "%s%ss\n%s%ss\n%s%st\n%s%sons\n%s%sez\n%s%sent\n",
     "%s%sais\n%s%sais\n%s%sait\n%s%sions\n%s%siez\n%s%saient\n",
     "%s%sis\n%s%sis\n%s%sit\n%s%sîmes\n%s%sîtes\n%s%sirent\n",
     "%s%sirai\n%s%siras\n%s%sira\n%s%sirons\n%s%sirez\n%s%siront\n",
     2},
    {"croître",
     "%s%scroîs\n%s%scroîs\n%s%scroît\n%s%scroissons\n%s%scroissez\n%s%scroissent\n",
     "%s%scroissais\n%s%scroissais\n%s%scroissait\n%s%scroissions\n%s%scroissiez\n%s%scroissaient\n",
     "%s%scrûs\n%s%scrûs\n%s%scrût\n%s%scrûmes\n%s%scrûtes\n%s%scrûrent\n",
     "%s%scroîtrai\n%s%scroîtras\n%s%scroîtra\n%s%scroîtrons\n%s%scroîtrez\n%s%scroîtront\n",
     8},
    {"aître",
     "%s%sais\n%s%sais\n%s%saît\n%s%saissons\n%s%saissez\n%s%saissent\n",
     "%s%saissais\n%s%saissais\n%s%saissait\n%s%saissions\n%s%saissiez\n%s%saissaient\n",
     "%s%sus\n%s%sus\n%s%sut\n%s%sûmes\n%s%sûtes\n%s%surent\n",
     "%s%saîtrai\n%s%saîtras\n%s%saîtra\n%s%saîtrons\n%s%saîtrez\n%s%saîtront\n",
     6},
    {"oître",
     "%s%sis\n%s%sis\n%s%sît\n%s%sissons\n%s%sissez\n%s%sissent\n",
     "%s%sis\n%s%sis\n%s%sît\n%s%sissons\n%s%sissez\n%s%sissent\n",
     "%s%sis\n%s%sis\n%s%sît\n%s%sissons\n%s%sissez\n%s%sissent\n",
     "%s%sis\n%s%sis\n%s%sît\n%s%sissons\n%s%sissez\n%s%sissent\n",
     5},
    {"tre",
     "%s%ss\n%s%ss\n%s%st\n%s%stons\n%s%stez\n%s%stent\n",
     "%s%stais\n%s%stais\n%s%stait\n%s%stions\n%s%stiez\n%s%staient\n",
     "\n",
     "%s%strai\n%s%stras\n%s%stra\n%s%strons\n%s%strez\n%s%stront\n",
     3},
    {"pre",
     "%s%ss\n%s%ss\n%s%st\n%s%sons\n%s%sez\n%s%sent\n",
     "%s%sais\n%s%sais\n%s%sait\n%s%sions\n%s%siez\n%s%sent\n",
     "%s%sis\n%s%sis\n%s%sit\n%s%sîmes\n%s%sîtes\n%s%sirent\n",
     "%s%srai\n%s%sras\n%s%sra\n%s%srons\n%s%srez\n%s%sront\n",
     2},
    {"plaire",
     "%s%splais\n%s%splais\n%s%splaît\n%s%splaisons\n%s%splaisez\n%s%splaisent\n",
     "%s%splaisais\n%s%splaisais\n%s%splaisait\n%s%splaisions\n%s%splaisiez\n%s%splaisaient\n",
     "%s%splus\n%s%splus\n%s%splut\n%s%splûmes\n%s%splûtes\n%s%splurent\n",
     "%s%splairai\n%s%splairas\n%s%splaira\n%s%splairons\n%s%splairez\n%s%splairont\n",
     6},
    {"taire",
     "%s%sais\n%s%sais\n%s%sait\n%s%saisons\n%s%saisez\n%s%saisent\n",
     "%s%saisais\n%s%saisais\n%s%saisait\n%s%saisions\n%s%saisiez\n%s%saisaient\n",
     "%s%sus\n%s%sus\n%s%sut\n%s%sûmes\n%s%sûtes\n%s%surent\n",
     "%s%sairai\n%s%sairas\n%s%saira\n%s%sairons\n%s%sairez\n%s%sairont\n",
     4},
    {"traire",
     "%s%sais\n%s%sais\n%s%sait\n%s%saiyons\n%s%saiyez\n%s%saient\n",
     "%s%saiyais\n%s%saiyais\n%s%saiyait\n%s%saiyions\n%s%saiyiez\n%s%saiyaient\n",
     "",
     "%s%sairai\n%s%sairas\n%s%saira\n%s%sairons\n%s%sairez\n%s%sairont\n",
     4},
    {"dire",
     "%s%sdis\n%s%sdis\n%s%sdit\n%s%sdisons\n%s%sdites\n%s%sdisent\n",
     "%s%sdisais\n%s%sdisais\n%s%sdisait\n%s%sdisions\n%s%sdisiez\n%s%sdisaient\n",
     "%s%sdis\n%s%sdis\n%s%sdit\n%s%sdîmes\n%s%sdîtes\n%s%sdirent\n",
     "%s%sdirai\n%s%sdiras\n%s%sdira\n%s%sdirons\n%s%sdirez\n%s%sdiront\n",
     4},
    {"crire",
     "%s%scris\n%s%scris\n%s%scrit\n%s%scrivons\n%s%scrivez\n%s%scrivent\n",
     "%s%scrivais\n%s%scrivais\n%s%scrivait\n%s%scrivions\n%s%scriviez\n%s%scrivaient",
     "%s%scrivis\n%s%scrivis\n%s%scrivit\n%s%scrivîmes\n%s%scrivîtes\n%s%scrivirent",
     "%s%scrirai\n%s%scriras\n%s%scrira\n%s%scrirons\n%s%scrirez\n%s%scriront",
     5},
    {"rire",
     "%s%sris\n%s%sris\n%s%srit\n%s%srions\n%s%sriez\n%s%srient\n",
     "%s%sriais\n%s%sriais\n%s%sriait\n%s%sriions\n%s%sriiez\n%s%sriaient\n",
     "%s%sris\n%s%sris\n%s%srit\n%s%srîmes\n%s%srîtes\n%s%srirent\n",
     "%s%srirai\n%s%sriras\n%s%srira\n%s%srirons\n%s%srirez\n%s%sriront\n",
     4},
    {"maudire",
     "%s%smaudis\n%s%smaudis\n%s%smaudit\n%s%smaudissons\n%s%smaudissez\n%s%smaudissent\n",
     "%s%smaudissais\n%s%smaudissais\n%s%smaudissait\n%s%smaudissions\n%s%smaudissiez\n%s%smaudissaient\n",
     "%s%smaudis\n%s%smaudis\n%s%smaudit\n%s%smaudîmes\n%s%smaudîtes\n%s%smaudirent\n",
     "%s%smaudirai\n%s%smaudiras\n%s%smaudira\n%s%smaudirons\n%s%smaudirez\n%s%smaudiront\n",
     4},
    {"oire",
     "%s%sois\n%s%sois\n%s%soit\n%s%soyons\n%s%soyez\n%s%soient\n",
     "%s%soyais\n%s%soyais\n%s%soyait\n%s%soyions\n%s%soyiez\n%s%soyaient\n",
     "%s%sus\n%s%sus\n%s%sut\n%s%sûmes\n%s%sûtes\n%s%surent\n",
     "%s%soirai\n%s%soiras\n%s%soira\n%s%soirons\n%s%soirez\n%s%soiront\n",
     4},
    {"ire",
     "%s%sis\n%s%sis\n%s%sit\n%s%sisons\n%s%sisez\n%s%sisent\n",
     "%s%sisais\n%s%sisais\n%s%sisait\n%s%sisions\n%s%sisiez\n%s%sisaient\n",
     "%s%sus\n%s%sus\n%s%sut\n%s%sûmes\n%s%sûtes\n%s%surent\n",
     "%s%sirai\n%s%siras\n%s%sira\n%s%sirons\n%s%sirez\n%s%siront\n",
     3},
    {"ure",
     "%s%sus\n%s%sus\n%s%sut\n%s%suons\n%s%suez\n%s%suent\n",
     "%s%suais\n%s%suais\n%s%suait\n%s%suions\n%s%suiez\n%s%suaient\n",
     "%s%sus\n%s%sus\n%s%sut\n%s%sûmes\n%s%sûtes\n%s%surent\n",
     "%s%surai\n%s%suras\n%s%sura\n%s%surons\n%s%surez\n%s%suront\n",
     3},
    {"ore",
     "%s%sos\n%s%sos\n%s%sôt\n%s%sosent\n",
     "\n",
     "\n",
     "%s%sorai\n%s%soras\n%s%sora\n%s%sorons\n%s%sorez\n%s%soront\n",
     3},
    {"vre",
     "%s%sis\n%s%sis\n%s%sit\n%s%sivons\n%s%sivez\n%s%sivent\n",
     "%s%sivais\n%s%sivais\n%s%sivait\n%s%sivions\n%s%siviez\n%s%sivaient\n",
     "%s%sécus\n%s%sécus\n%s%sécu\n%s%sécûmes\n%s%sécûtes\n%s%sécurent\n",
     "%s%sivrai\n%s%sivras\n%s%sivra\n%s%sivrons\n%s%sivrez\n%s%sivront\n",
     4},
    {"vaincre",
     "%s%svaincs\n%s%svaincs\n%s%svainc\n%s%svainquons\n%s%svainquez\n%s%svainquent\n",
     "%s%svainquais\n%s%svainquais\n%s%svainquait\n%s%svainquions\n%s%svainquiez\n%s%svainquaient\n",
     "%s%svainquis\n%s%svainquis\n%s%svainquit\n%s%svainquîmes\n%s%svainquîtes\n%s%svainquirent\n",
     "%s%svaincrai\n%s%svaincras\n%s%svaincra\n%s%svaincrons\n%s%svaincrez\n%s%svaincront\n",
     7}};

bool conjuguer_cas_particuliers_troisieme_groupe(char *verbe, char *verbe_ascii, char *racine_ascii, char *racine, char pronoms[6][LONGUEUR_MAX_CHAINE])
{
    if (strcmp(verbe, "pouvoir") == 0)
    {
        printf("\nPrésent:\n%speux\n%speux\n%speut\n%spouvons\n%spouvez\n%speuvent\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nImparfait:\n%spouvais\n%spouvais\n%spouvait\n%spouvions\n%spouviez\n%spouvaient\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nPassé simple:\n%spus\n%spus\n%sput\n%spûmes\\n%spûtes\\n%spurent\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nFutur simple:\n%spourrai\n%spourras\n%spourra\n%spourrons\\n%spourrez\\n%spourront\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
    }
    else if (strcmp(verbe, "vouloir") == 0)
    {
        printf("\nPrésent:\n%sveux\n%sveux\n%sveut\n%svoulons\n%svoulez\n%sveulent\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nImparfait:\n%svoulais\n%svoulais\n%svoulait\n%svoulions\n%svouliez\n%svoulaient\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nPassé simple:\n%svoulus\n%svoulus\n%svoulut\n%svoulûmes\n%svoulûtes\n%svoulurent\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nFutur simple:\n%svoudrai\n%svoudras\n%svoudra\n%svoudrons\n%svoudrez\n%svoudront\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
    }
    else if (strcmp(verbe, "en vouloir") == 0)
    {
        printf("\nPrésent:\n%sen veux\n%sen veux\n%sen veut\n%sen voulons\n%sen voulez\n%sen veulent\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nImparfait:\n%sen voulais\n%sen voulais\n%sen voulait\n%sen voulions\n%sen vouliez\n%sen voulaient\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nPassé simple:\n%sen voulus\n%sen voulus\n%sen voulut\n%sen voulûmes\n%sen voulûtes\n%sen voulurent\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nFutur simple:\n%sen voudrai\n%sen voudras\n%sen voudra\n%sen voudrons\n%sen voudrez\n%sen voudront\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
    }
    else if (finit_en(verbe, "faire"))
    {
        preparer_racine_pour_affichage(verbe_ascii, racine_ascii, racine, 5);
        printf("racine: %s", racine);
        printf("\nPrésent:\n%s%sfais\n%s%sfais\n%s%sfait\n%s%sfaisons\n%s%sfaites\n%s%sfont\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
        printf("\nImparfait:\n%s%sfaisais\n%s%sfaisais\n%s%sfaisait\n%s%sfaisions\n%s%sfaisiez\n%s%sfaisaient\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
        printf("\nPassé simple:\n%s%sfis\n%s%sfis\n%s%sfit\n%s%sfîmes\n%s%sfîtes\n%s%sfirent\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
        printf("\nFutur simple:\n%s%sferai\n%s%sferas\n%s%sfera\n%s%sferons\n%s%sferez\n%s%sferont\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
    }
    else if (strcmp(verbe, "asseoir") == 0)
    {
        printf("\nPrésent:\n%sassois\n%sassois\n%sassoit\n%sassoyons\n%sassoyez\n%sassoient\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nImparfait:\n%sassoyais\n%sassoyais\n%sassoyait\n%sassoyions\n%sassoyiez\n%sassoyaient\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nPassé simple:\n%sassis\n%sassis\n%sassit\n%sassîmes\n%sassîtes\n%sassirent\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nFutur simple:\n%sassoirai\n%sassoiras\n%sassoira\n%sassoirons\n%sassoirez\n%sassoiront\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
    }
    else if (strcmp(verbe, "aller") == 0 || strcmp(verbe, "en aller") == 0)
    {
        printf("\nPrésent:\n%svais\n%svas\n%sva\n%sallons\n%sallez\n%svont\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nImparfait:\n%sallais\n%sallais\n%sallait\n%sallions\n%salliez\n%sallaient\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nPassé simple:\n%sallai\n%sallas\n%salla\n%sallâmes\n%sallâtes\n%sallèrent\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nFutur simple:\n%sirai\n%siras\n%sira\n%sirons\n%sirez\n%siront\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
    }
    else if (strcmp(verbe, "être") == 0)
    {
        printf("\nPrésent:\n%ssuis\n%ses\n%sest\n%ssommes\n%sêtes\n%ssont\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nImparfait:\n%sétais\n%sétais\n%sétait\n%sétions\n%sétiez\n%sétaient\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nPassé simple:\n%sfus\n%sfus\n%sfut\n%sfûmes\n%sfûtes\n%sfurent\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nFutur simple:\n%sserai\n%sseras\n%ssera\n%sserons\n%sserez\n%sseront\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
    }
    else if (strcmp(verbe, "avoir") == 0)
    {
        printf("\nPrésent:\n%sai\n%sas\n%sa\n%savons\n%savez\n%sont\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nImparfait:\n%savais\n%savais\n%savait\n%savions\n%saviez\n%savaient\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nPassé simple:\n%seus\n%seus\n%seut\n%seûmes\n%seûtes\n%seurent\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nFutur simple:\n%saurai\n%sauras\n%saura\n%saurons\n%saurez\n%sauront\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
    }
    else if (strcmp(verbe, "savoir") == 0)
    {
        printf("\nPrésent:\n%ssais\n%ssais\n%ssait\n%ssavons\n%ssavez\n%ssavent\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nImparfait:\n%ssavais\n%ssavais\n%ssavait\n%ssavions\n%ssaviez\n%ssavaient\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nPassé simple:\n%ssus\n%ssus\n%ssut\n%ssûmes\n%ssûtes\n%ssurent\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nFutur simple:\n%ssaurai\n%ssauras\n%ssaura\n%ssaurons\n%ssaurez\n%ssauront\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
    }
    else if (strcmp(verbe, "falloir") == 0)
    {
        printf("\nPrésent:\nIl faut\n");
        printf("\nImparfait:\nIl fallait\n");
        printf("\nPassé simple:\nIl fallut\n");
        printf("\nFutur simple:\nIl faudra\n");
    }
    else if (strcmp(verbe, "en falloir") == 0)
    {
        printf("\nPrésent:\nIl s'en faut\n");
        printf("\nImparfait:\nIl s'en fallait\n");
        printf("\nPassé simple:\nIl s'en fallut\n");
        printf("\nFutur simple:\nIl s'en faudra\n");
    }
    else if (strcmp(verbe, "pleuvoir") == 0)
    {
        printf("\nPrésent:\nIl pleut\n");
        printf("\nImparfait:\nIl pleuvait\n");
        printf("\nPassé simple:\nIl plut\n");
        printf("\nFutur simple:\nIl pleuvra\n");
    }
    else if (strcmp(verbe, "valoir") == 0)
    {
        printf("\nPrésent:\n%svaux\n%svaux\n%svaut\n%svalons\n%svalez\n%svalent\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nImparfait:\n%svalais\n%svalais\n%svalait\n%svalions\n%svaliez\n%svalaient\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nPassé simple:\n%svalus\n%svalus\n%svalut\n%svalûmes\n%svalûtes\n%svalurent\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nFutur simple:\n%svaudrai\n%svaudras\n%svaudra\n%svaudrons\n%svaudrez\n%svaudront\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
    }
    else if (strcmp(verbe, "devoir") == 0)
    {
        printf("\nPrésent:\n%sdois\n%sdois\n%sdoit\n%sdevons\n%sdevez\n%sdoivent\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nImparfait:\n%sdevais\n%sdevais\n%sdevait\n%sdevions\n%sdeviez\n%sdevaient\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nPassé simple:\n%sdus\n%sdus\n%sdut\n%sdûmes\n%sdûtes\n%sdurent\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nFutur simple:\n%sdevrai\n%sdevras\n%sdevra\n%sdevrons\n%sdevrez\n%sdevront\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
    }
    else if (strcmp(verbe, "voir") == 0)
    {
        printf("\nPrésent:\n%svois\n%svois\n%svoit\n%svoyons\n%svoyez\n%svoient\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nImparfait:\n%svoyais\n%svoyais\n%svoyait\n%svoyions\n%svoyiez\n%svoyaient\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nPassé simple:\n%svis\n%svis\n%svit\n%svîmes\n%svîtes\n%svirent\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nFutur simple:\n%sverrai\n%sverras\n%sverra\n%sverrons\n%sverrez\n%sverront\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
    }
    else if (strcmp(verbe, "ouïr") == 0)
    {
        printf("\nPrésent:\n%sois\n%sois\n%soit\n%soyons\n%soyez\n%soient\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nImparfait:\n%soyais\n%soyais\n%soyait\n%soyions\n%soyiez\n%soyaient\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nPassé simple:\n%souïs\n%souïs\n%souït\n%souïmes\n%souïtes\n%souïrent\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
        printf("\nFutur simple:\n%souïrai\n%souïras\n%souïra\n%souïrons\n%souïrez\n%souïront\n", pronoms[0], pronoms[1], pronoms[2], pronoms[3], pronoms[4], pronoms[5]);
    }
    else if (strcmp(verbe, "naître") == 0 || strcmp(verbe, "renaître") == 0)
    {
        preparer_racine_pour_affichage(verbe_ascii, racine_ascii, racine, 7);
        printf("\nPrésent:\n%s%snais\n%s%snais\n%s%snaît\n%s%snaissons\n%s%snaissez\n%s%snaissent\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
        printf("\nImparfait:\n%s%snaissais\n%s%snaissais\n%s%snaissait\n%s%snaissions\n%s%snaissiez\n%s%snaissaient\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
        printf("\nPassé simple:\n%s%snaquis\n%s%snaquis\n%s%snaquit\n%s%snaquîmes\n%s%snaquîtes\n%s%snaquirent\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
        printf("\nFutur simple:\n%s%snaîtrai\n%s%snaîtras\n%s%snaîtra\n%s%snaîtrons\n%s%snaîtrez\n%s%snaîtront\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
    }
    else if (strcmp(verbe, "braire") == 0)
    {
        printf("\nPrésent:\n%sbrait\n%sbraient\n", pronoms[2], pronoms[5]);
        printf("\nFutur simple:\n%sbraira\n%sbrairont\n", pronoms[2], pronoms[5]);
    }
    else if (strcmp(verbe, "raire") == 0)
    {
        printf("\nPrésent:\n%srait\n", pronoms[2]);
        printf("\nFutur simple:\n%sraira\n", pronoms[2]);
    }
    else if (finit_en(verbe, "offrir") || finit_en(verbe, "ouvrir") || finit_en(verbe, "souffrir") || strcmp(verbe, "assaillir") == 0)
    {
        preparer_racine_pour_affichage(verbe_ascii, racine_ascii, racine, 2);
        printf("\nPrésent:\n%s%se\n%s%ses\n%s%se\n%s%sons\n%s%sez\n%s%sent\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
        printf("\nImparfait:\n%s%sais\n%s%sais\n%s%sait\n%s%sions\n%s%siez\n%s%saient\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
        printf("\nPassé simple:\n%s%sis\n%s%sis\n%s%sit\n%s%sîmes\n%s%sîtes\n%s%sirent\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
        printf("\nFutur simple:\n%s%sirai\n%s%siras\n%s%sira\n%s%sirons\n%s%sirez\n%s%siront\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
    }
    else if (finit_en(verbe, "cueillir"))
    {
        preparer_racine_pour_affichage(verbe_ascii, racine_ascii, racine, 2);
        printf("\nPrésent:\n%s%se\n%s%ses\n%s%se\n%s%sons\n%s%sez\n%s%sent\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
        printf("\nImparfait:\n%s%sais\n%s%sais\n%s%sait\n%s%sions\n%s%siez\n%s%saient\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
        printf("\nPassé simple:\n%s%sis\n%s%sis\n%s%sit\n%s%sîmes\n%s%sîtes\n%s%sirent\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
        printf("\nFutur simple:\n%s%serai\n%s%seras\n%s%sera\n%s%serons\n%s%serez\n%s%seront\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, pronoms[4], racine, pronoms[5], racine);
    }
    else if (finit_en(verbe, "partir") || finit_en(verbe, "servir") || finit_en(verbe, "sortir") || finit_en(verbe, "sentir") || finit_en(verbe, "mentir") || finit_en(verbe, "repentir") || finit_en(verbe, "bouillir"))
    {
        preparer_racine_pour_affichage(verbe_ascii, racine_ascii, racine, 3);
        char charactere_additionel = verbe[strlen(verbe_ascii) - 3];
        printf("\nPrésent:\n%s%ss\n%s%ss\n%s%st\n%s%s%cons\n%s%s%cez\n%s%s%cent\n", pronoms[0], racine, pronoms[1], racine, pronoms[2], racine, pronoms[3], racine, charactere_additionel, pronoms[4], racine, charactere_additionel, pronoms[5], racine, charactere_additionel);
        printf("\nImparfait:\n%s%s%cais\n%s%s%cais\n%s%s%cait\n%s%s%cions\n%s%s%ciez\n%s%s%caient\n", pronoms[0], racine, charactere_additionel, pronoms[1], racine, charactere_additionel, pronoms[2], racine, charactere_additionel, pronoms[3], racine, charactere_additionel, pronoms[4], racine, charactere_additionel, pronoms[5], racine, charactere_additionel);
        printf("\nPassé simple:\n%s%s%cis\n%s%s%cis\n%s%s%cit\n%s%s%cîmes\n%s%s%cîtes\n%s%s%cirent\n", pronoms[0], racine, charactere_additionel, pronoms[1], racine, charactere_additionel, pronoms[2], racine, charactere_additionel, pronoms[3], racine, charactere_additionel, pronoms[4], racine, charactere_additionel, pronoms[5], racine, charactere_additionel);
        printf("\nFutur simple:\n%s%s%cirai\n%s%s%ciras\n%s%s%cira\n%s%s%cirons\n%s%s%cirez\n%s%s%ciront\n", pronoms[0], racine, charactere_additionel, pronoms[1], racine, charactere_additionel, pronoms[2], racine, charactere_additionel, pronoms[3], racine, charactere_additionel, pronoms[4], racine, charactere_additionel, pronoms[5], racine, charactere_additionel);
    }
    else
    {
        return false;
    }
    return true;
}

void conjuguer_troixieme_groupe(char *verbe, char *verbe_ascii, char *racine_ascii, char *racine, char pronoms[6][LONGUEUR_MAX_CHAINE])
{
    if (conjuguer_cas_particuliers_troisieme_groupe(verbe, verbe_ascii, racine_ascii, racine, pronoms))
    {
        return;
    }
    int nombre_regles_conjugaison = sizeof(regles_de_conjugaison_troisieme_groupe) / sizeof(regles_de_conjugaison_troisieme_groupe[0]);
    if (conjuguer_avec_des_regles(verbe, verbe_ascii, racine_ascii, racine, pronoms, regles_de_conjugaison_troisieme_groupe, nombre_regles_conjugaison))
    {
        return;
    }

    printf("Pas de regle trouvée pour le verbe: %s\n", verbe);
}

bool est_voyelle(char c)
{
    switch (c)
    {
    case 'a':
    case 'e':
    case 'i':
    case 'o':
    case 'u':
        return true;
    default:
        return false;
    }
}

bool commence_par_accent(char *verbe)
{
    size_t longueur_accents = sizeof(accents) / sizeof(AccentNonAsciiEnAscii);
    for (int i = 0; i < longueur_accents; i++)
    {
        if (commence_en(verbe, accents[i].non_ascii))
        {
            return true;
        }
    }
    return false;
}

bool necessite_apostrophe(char *verbe)
{
    return est_voyelle(verbe[0]) || commence_par_accent(verbe) || commence_en(verbe, "h") || commence_en(verbe, "y ");
}

void recuperer_pronoms(char *verbe, char pronoms[6][LONGUEUR_MAX_CHAINE], bool pronominal)
{
    if (pronominal)
    {
        if (necessite_apostrophe(verbe))
        {
            strcpy(pronoms[0], "Je m'");
            strcpy(pronoms[1], "Tu t'");
            strcpy(pronoms[2], "Il/Elle/On s'");
            strcpy(pronoms[3], "Nous nous ");
            strcpy(pronoms[4], "Vous vous ");
            strcpy(pronoms[5], "Ils/Elles s'");
        }
        else
        {
            strcpy(pronoms[0], "Je me ");
            strcpy(pronoms[1], "Tu te ");
            strcpy(pronoms[2], "Il/Elle/On se ");
            strcpy(pronoms[3], "Nous nous ");
            strcpy(pronoms[4], "Vous vous ");
            strcpy(pronoms[5], "Ils/Elles se ");
        }
    }
    else
    {
        if (necessite_apostrophe(verbe))
        {
            strcpy(pronoms[0], "J'");
            strcpy(pronoms[1], "Tu ");
            strcpy(pronoms[2], "Il/Elle/On ");
            strcpy(pronoms[3], "Nous ");
            strcpy(pronoms[4], "Vous ");
            strcpy(pronoms[5], "Ils/Elles ");
        }
        else
        {
            strcpy(pronoms[0], "Je ");
            strcpy(pronoms[1], "Tu ");
            strcpy(pronoms[2], "Il/Elle/On ");
            strcpy(pronoms[3], "Nous ");
            strcpy(pronoms[4], "Vous ");
            strcpy(pronoms[5], "Ils/Elles ");
        }
    }
}

// Fonction pour conjuguer un verbe
void conjuguer(char *verbe, int groupe, bool pronominal)
{
    char racine[LONGUEUR_MAX_CHAINE];
    char racine_ascii[LONGUEUR_MAX_CHAINE];
    char verbe_ascii[LONGUEUR_MAX_CHAINE];
    remplacer_accents(verbe, verbe_ascii, "non_ascii_vers_ascii");

    char pronoms[6][LONGUEUR_MAX_CHAINE];
    recuperer_pronoms(verbe, pronoms, pronominal);

    switch (groupe)
    {
    case 1:
        conjuguer_premier_groupe(verbe, verbe_ascii, racine, racine_ascii, pronoms);
        break;
    case 2:
        conjuguer_deuxieme_groupe(verbe, verbe_ascii, racine, racine_ascii, pronoms);
        break;
    case 3:
        conjuguer_troixieme_groupe(verbe, verbe_ascii, racine, racine_ascii, pronoms);
        break;
    default:
        printf("Invalid groupe\n");
        break;
    }
}

// Une structure pour stocker les données d'un verbe
typedef struct
{
    char verbe[LONGUEUR_MAX_CHAINE];
    int groupe;
} Verbe;

// Fonction pour lire les données de verbes à partir d'un fichier et les stocker dans un tableau de structures Verbe
int lire_verbes(char *nom_fichier, Verbe *verbes)
{
    FILE *fp;
    char ligne[LONGUEUR_MAX_CHAINE];
    int nombre_verbes = 0;

    fp = fopen(nom_fichier, "r");
    if (fp == NULL)
    {
        printf("Erreur lors de l'ouverture du fichier %s\n", nom_fichier);
        exit(1);
    }

    while (fgets(ligne, LONGUEUR_MAX_CHAINE, fp) != NULL)
    {
        // Analyse du verbe et du groupe à partir de la ligne du fichier
        char verbe[LONGUEUR_MAX_CHAINE];
        int groupe;

        sscanf(ligne, "%[^0-9] %d", verbes[nombre_verbes].verbe, &verbes[nombre_verbes].groupe);
        verbes[nombre_verbes].verbe[strlen(verbes[nombre_verbes].verbe) - 1] = '\0';
        nombre_verbes++;
    }

    fclose(fp);

    return nombre_verbes;
}

bool demander_quelque_chose(char *question)
{
    char reponse[LONGUEUR_MAX_CHAINE];
    do
    {
        printf("\n%s (%s/%s)\n", question, "oui", "non");
        scanf("%s", reponse);
    } while (strcmp(reponse, "oui") != 0 && strcmp(reponse, "non") != 0);
    return strcmp(reponse, "oui") == 0;
}

int main()
{
    // Lire les verbess à partir du fichier
    Verbe verbes[NOMBRE_MAX_VERBS];
    int nombre_verbs = lire_verbes("verbes.txt", verbes);
    int groupe;

    // Loop until user chooses to stop
    bool continuer = true;
    while (continuer)
    {
        // Demander à l'utilisateur quel verbe il veut conjuguer
        char verbe[LONGUEUR_MAX_CHAINE];
        printf("\nQuel verbe voulez vous conjuguer?\n");
        fgets(verbe, sizeof(verbe), stdin);
        verbe[strcspn(verbe, "\n")] = '\0';

        // Trouver le verbe et son groupe dans le tableau de verbes
        int i, trouve = 0;
        for (i = 0; i < nombre_verbs; i++)
        {
            if (strcmp(verbe, verbes[i].verbe) == 0)
            {
                groupe = verbes[i].groupe;
                trouve = 1;
                break;
            }
        }

        if (!trouve && (commence_en(verbe, "s'") || commence_en(verbe, "se ")))
        {
            // Si on ne trouve pas le verbe, essayer de trouver le verbe sans le pronominal
            char vrai_verbe[LONGUEUR_MAX_CHAINE];
            if (commence_en(verbe, "s'"))
            {
                remplacer_chaine(verbe, vrai_verbe, "s'", "");
            }
            else // (commence_en(verbe, "se "))
            {
                remplacer_chaine(verbe, vrai_verbe, "se ", "");
            }

            for (i = 0; i < nombre_verbs; i++)
            {
                if (strcmp(vrai_verbe, verbes[i].verbe) == 0)
                {
                    groupe = verbes[i].groupe;
                    trouve = 1;
                    break;
                }
            }
        }

        // Si on ne trouve pas le verbe, demander à l'utilisateur s'il veut essayer à nouveau
        if (!trouve)
        {
            printf("Le verbe %s n'a pas été trouvé.\n", verbe);
            continue;
        }

        // Conjuguer le verbe
        if (commence_en(verbe, "s'") || commence_en(verbe, "se "))
        {
            printf("Le verbe \"%s\" est un verbe pronominal.\n", verbe);
            char vrai_verbe[LONGUEUR_MAX_CHAINE];
            if (commence_en(verbe, "s'"))
            {
                remplacer_chaine(verbe, vrai_verbe, "s'", "");
            }
            else // (commence_en(verbe, "se "))
            {
                remplacer_chaine(verbe, vrai_verbe, "se ", "");
            }
            printf("Le vrai verbe est \"%s\".\n", vrai_verbe);
            conjuguer(vrai_verbe, groupe, true);
        }
        else
        {
            conjuguer(verbe, groupe, false);
        }
    }

    return 0;
}