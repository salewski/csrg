#ifndef lint
static	char *sccsid = "@(#)globals.c	1.3 83/05/20";
#endif
#include <stdio.h>
#include "externs.h"

struct scenario scene[NUMOFSCENES] = {
	5, 3, 5, 0, 1, 0, 2, "Ranger vs. Drake",
	{
		{ "Ranger",          0,  0, 7,  20, 4, 0 },
		{ "Drake",           1,  1, 7,  31, 5, 0 }
	},
	1, 3, 6, 0, 1,0, 2, "The Battle of Flamborough Head",
	{
		{ "Bonhomme Rich",   2,  0, 13, 40, 2, 0 },
		{ "Serapis",         3,  1,  2, 42, 2, 0 }
	},
	5, 5, 5, 0, 1, 0, 10, "Arbuthnot and Des Touches",
	{
		{ "America",         4,  1,  7, 37, 4, 0 },
		{ "Befford",         5,  1,  5, 35, 4, 0 },
		{ "Adamant",         6,  1,  3, 33, 4, 0 },
		{ "London",          7,  1,  1, 31, 4, 0 },
		{ "Royal Oak",       8,  1, -1, 29, 4, 0 },
		{ "Neptune",         9,  3,  6, 44, 4, 0 },
		{ "Duc Bougogne",    10, 3,  8, 46, 4, 0 },
		{ "Conquerant",      48, 3, 10, 48, 4, 0 },
		{ "Provence",        11, 3, 12, 50, 4, 0 },
		{ "Romulus",         12, 3, 20, 58, 4, 0 }
	},
	1, 3, 5, 0, 1, 0, 10, "Suffren and Hughes",
	{
		{ "Monmouth",        52, 1, 9,  45, 2, 0 },
		{ "Hero",            5,  1, 13, 49, 2, 0 },
		{ "Isis",            6,  1, 12, 48, 2, 0 },
		{ "Superb",          50, 1, 10, 46, 2, 0 },
		{ "Burford",	   48, 1, 11, 47, 2, 0 },
		{ "Flamband",        13, 3, 7,  59, 4, 0 },
		{ "Annibal",	    9, 3, 4,  56, 4, 0 },
		{ "Severe",          11, 3, 2,  54, 4, 0 },
		{ "Brilliant",       49, 3, -1, 51, 4, 0 },
		{ "Sphinx",          51, 3, -5, 47, 4, 0 }
	},
	1, 3, 4, 0, 1, 0, 2, "Nymphe vs. Cleopatre",
	{
		{ "Nymphe",          14, 1, 13, 30, 2, 0 },
		{ "Cleopatre",       15, 3, 3,  41, 2, 0 }
	},
	1, 3, 5, 0, 1, 0, 2, "Mars vs. Hercule",
	{
		{ "Mars",            16, 1, 13, 30, 2, 0 },
		{ "Hercule",         17, 3, 3,  41, 2, 0 }
	},
	5, 3, 5, 0, 1, 0, 2, "Ambuscade vs. Baionnaise",
	{
		{ "Ambuscade",       18, 1, 13, 30, 2, 0 },
		{ "Baionnaise",      19, 3, 3,  41, 2, 0 }
	},
	1, 5, 6, 0, 1, 0, 2, "Constellation vs. Insurgent",
	{
		{ "Constellation",   20, 0, 9,  50, 8, 0 },
		{ "Insurgent",       22, 3, 4,  24, 2, 0 }
	},
	1, 3, 5, 0, 1, 0, 2, "Constellation vs. Vengeance",
	{
		{ "Constellation",   20, 0, 12, 40, 2, 0 },
		{ "Vengeance",       21, 3, 1,  43, 2, 0 }
	},
	1, 3, 6, 0, 1, 0, 10, "The Battle of Lissa",
	{
		{ "Amphion",         23, 1, 8,  50, 4, 0 },
		{ "Active",          24, 1, 6,  48, 4, 0 },
		{ "Volage",          25, 1, 4,  46, 4, 0 },
		{ "Cerberus",        26, 1, 2,  44, 4, 0 },
		{ "Favorite",        27, 3, 9,  34, 2, 0 },
		{ "Flore",           21, 3, 13, 39, 2, 0 },
		{ "Danae",           64, 3, 15, 37, 2, 0 },
		{ "Bellona",         28, 3, 17, 35, 2, 0 },
		{ "Corona",          29, 3, 12, 31, 2, 0 },
		{ "Carolina",        30, 3, 15, 28, 2, 0 }
	},
	2, 5, 6, 0, 1, 0, 2, "Constitution vs. Guerriere",
	{
		{ "Constitution",    31, 0, 7,  35, 1, 0 },
		{ "Guerriere",       32, 1, 7,  47, 4, 0 }
	},
	1, 3, 5, 0, 1, 0, 2, "United States vs. Macedonian",
	{
		{ "United States",   33, 0, 1,  52, 6, 0 },
		{ "Macedonian",      34, 1, 14, 40, 1, 0 }
	},
	1, 3, 6, 0, 1, 0, 2, "Constitution vs. Java",
	{
		{ "Constitution",    31, 0, 1,  40, 2, 0 },
		{ "Java",            35, 1, 11, 40, 2, 0 }
	},
	1, 3, 5, 0, 1, 0, 2, "Chesapeake vs. Shannon",
	{
		{ "Chesapeake",      36, 0, 13, 40, 2, 0 },
		{ "Shannon",         37, 1, 1,  42, 2, 0 }
	},
	1, 1, 6, 0, 1, 0, 5, "The Battle of Lake Erie",
	{
		{ "Lawrence",        38, 0, 4,  55, 8, 0 },
		{ "Niagara",         42, 0, 7,  61, 8, 0 },
		{ "Lady Prevost",    39, 1, 4,  25, 2, 0,  },
		{ "Detroit",         40, 1, 7,  22, 2, 0 },
		{ "Q. Charlotte",    41, 1, 10, 19, 2, 0  }
	},
	1, 1, 5, 0, 1, 0, 2, "Wasp vs. Reindeer",
	{
		{ "Wasp",            42, 0, 3,  41, 2, 0 },
		{ "Reindeer",        43, 1, 10, 48, 2, 0 }
	},
	1, 2, 5, 0, 1, 0, 3, "Constitution vs. Cyane and Levant",
	{
		{ "Constitution",    31, 0, 10, 45, 2, 0 },
		{ "Cyane",           44, 1, 3,  37, 2, 0 },
		{ "Levant",          45, 1, 5,  35, 2, 0 }
	},
	5, 5, 5, 0, 1, 0, 3, "Pellew vs. Droits de L'Homme",
	{
		{ "Indefatigable",   46, 1, 12, 45, 6, 0 },
		{ "Amazon",          47, 1, 9,  48, 6, 0 },
		{ "Droits L'Hom",    48, 3, 3,  28, 5, 0 }
	},
	2, 2, 3, 0, 1, 0, 10, "Algeciras",
	{
		{ "Caesar",          49, 1, 7,  70, 6, 0 },
		{ "Pompee",          50, 1, 5,  72, 6, 0 },
		{ "Spencer",         5,  1, 3,  74, 6, 0 },
		{ "Hannibal",        7,  1, 1,  76, 6, 0 },
		{ "Real-Carlos",     53, 2, 9,  20, 3, 0 },
		{ "San Fernando",    54, 2, 11, 16, 3, 0 },
		{ "Argonauta",       55, 2, 10, 14, 4, 0 },
		{ "San Augustine",   56, 2, 6,  22, 4, 0 },
		{ "Indomptable",     51, 3, 7,  23, 5, 0 },
		{ "Desaix",          52, 3, 7,  27, 7, 0 }
	},
	5, 3, 6, 0, 1, 0, 7, "Lake Champlain",
	{
		{ "Saratoga",        60, 0, 8,  10, 1, 0 },
		{ "Eagle",           61, 0, 9,  13, 2, 0 },
		{ "Ticonderoga",     62, 0, 12, 17, 3, 0 },
		{ "Preble",          63, 0, 14, 20, 2, 0 },
		{ "Confiance",       57, 1, 4,  70, 6, 0 },
		{ "Linnet",          58, 1, 7,  68, 6, 0 },
		{ "Chubb",           59, 1, 10, 65, 6, 0 }
	},
	5, 3, 6, 0, 1, 0, 4, "Last Voyage of the USS President",
	{
		{ "President",       67, 0, 12, 42, 5, 0 },
		{ "Endymion",        64, 1, 5,  42, 5, 0 },
		{ "Pomone",          65, 1, 7,  82, 6, 0 },
		{ "Tenedos",         66, 1, 7,  -1, 4, 0 }
	},
	7, 5, 5, 0, 1, 0, 2, "Hornblower and the Natividad",
	{
		{ "Lydia",           68, 1, 12, 40, 2, 0 },
		{ "Natividad",       69, 2, 2,  40, 4, 0 }
	},
	1, 3, 6, 0, 1, 0, 2, "Curse of the Flying Dutchman",
	{
		{ "Piece of Cake",   19, 2, 7,  40, 2, 0 },
		{ "Flying Dutchy",   71, 3, 7,  41, 1, 0 }
	},
	1, 4, 1, 0, 1, 0, 4, "The South Pacific",
	{
		{ "USS Scurvy",      70, 0, 7,  40, 1, 0 },
		{ "HMS Tahiti",      71, 1, 12, 60, 1, 0 },
		{ "Australian",      18, 2, 5,  20, 8, 0 },
		{ "Bikini Atoll",    63, 3, 2,  60, 4, 0 }
	},
	7, 3, 6, 0, 1, 0, 5, "Hornblower and the battle of Rosas bay",
	{
		{ "Sutherland",      5,  1, 13, 30, 2, 0 },
		{ "Turenne",         10, 3, 9,  35, 6, 0 },
		{ "Nightmare",       9,  3, 7,  37, 6, 0 },
		{ "Paris",           53, 3, 3,  45, 4, 0 },
		{ "Napolean",        56, 3, 1,  40, 6, 0 }
	},
	6, 4, 7, 0, 1, 0, 5, "Cape Horn",
	{
		{ "Concord",         51, 0, 3,  20, 4, 0 },
		{ "Berkeley",        7,  0, 5,  50, 5, 0 },
		{ "Thames",          71, 1, 10, 40, 1, 0 },
		{ "Madrid",          53, 2, 13, 60, 8, 0 },
		{ "Musket",          10, 3, 10, 60, 7, 0 }
	},
	8, 3, 7, 0, 1, 0, 3, "New Orleans",
	{
		{ "Alligator",       71, 0, 13,  5, 1, 0 },
		{ "Firefly",         50, 1, 10, 20, 8, 0 },
		{ "Cypress",         46, 1, 5,  10, 6, 0 }
	},
	5, 3, 7, 0, 1, 0, 3, "Botany Bay",
	{
		{ "Shark",           11, 1, 6,  15, 4, 0 },
		{ "Coral Snake",     31, 3, 3,  30, 6, 0 },
		{ "Sea Lion",        33, 3, 13, 50, 8, 0 }
	},
	4, 3, 6, 0, 1, 0, 4, "Voyage to the Bottom of the Sea",
	{
		{ "Seaview",         71, 0, 6,  3,  3, 0 },
		{ "Flying Sub",      64, 0, 8,  3,  3, 0 },
		{ "Mermaid",         70, 1, 2,  5,  5, 0 },
		{ "Giant Squid",     53, 2, 10, 30, 8, 0 }
	},
	7, 3, 6, 0, 1, 0, 3, "Frigate Action",
	{
		{ "Killdeer",        21, 0, 7,  20, 8, 0 },
		{ "Sandpiper",       27, 1, 5,  40, 8, 0 },
		{ "Curlew",          34, 2, 10, 60, 8, 0 }
	},
	7, 2, 5, 0, 1, 0, 6, "The Battle of Midway",
	{
		{ "Enterprise",      49, 0, 10, 70, 8, 0 },
		{ "Yorktown",        51, 0, 3,  70, 7, 0 },
		{ "Hornet",          52, 0, 6,  70, 7, 0 },
		{ "Akagi",           53, 3, 6,  10, 4, 0 },
		{ "Kaga",            54, 3, 4,  12, 4, 0 },
		{ "Soryu",           55, 3, 2,  14, 4, 0 }
	},
	4, 3, 4, 0, 1, 0, 8, "Star Trek",
	{
		{ "Enterprise",      72, 0, 20,  4, 2, 0 },
		{ "Yorktown",        72, 0, 19,  2, 2, 0 },
		{ "Reliant",         73, 0, 18,  3, 2, 0 },
		{ "Galileo",         74, 0, 20,  6, 2, 0 },
		{ "Kobayashi Maru",  75, 3, 31, 11, 5, 0 },
		{ "Klingon two",     75, 3, 31, 11, 5, 0 },
		{ "Klingon three",   75, 3, 31, 11, 5, 0 },
		{ "Blue Orion",      63, 2, 31, 11, 5, 0 }
	}
};

/* happy */
struct shipspecs specs[] = {
	4, 7, 3,  19, 5,  5, 4,  2,  2,  2,  2,  2, 0, 0,  4, 4, 4, 4,  7,
	4, 7, 3,  17, 5,  5, 4,  2,  2,  2,  0,  0, 4, 4,  3, 3, 3, 3,  6,
	3, 5, 2,  42, 4,  7, 4,  2,  2,  2,  2,  2, 0, 0,  5, 5, 5, -1, 11,
	4, 6, 3,  44, 3,  7, 4,  2,  2,  2,  3,  3, 0, 0,  5, 5, 5, 5,  12,
	3, 5, 2,  64, 2, 17, 4,  8,  6,  6, 12, 12, 2, 2,  7, 7, 7, -1, 20,
	3, 5, 2,  74, 2, 20, 4,  8,  8,  8, 16, 16, 2, 2,  7, 7, 7, -1, 26,
	3, 5, 2,  50, 2, 12, 4,  6,  4,  4,  8,  8, 2, 2,  6, 6, 6, -1, 17,
	3, 5, 1,  98, 1, 23, 4, 10, 10, 10, 18, 18, 2, 2,  8, 8, 8, -1, 28,
	3, 5, 2,  74, 2, 20, 4,  8,  8,  8, 16, 16, 2, 2,  7, 7, 7, -1, 26,
	3, 5, 2,  74, 2, 21, 3, 10, 10,  8, 20, 20, 0, 0,  7, 7, 7, -1, 24,
	3, 5, 1,  80, 1, 23, 3, 12, 12, 10, 22, 22, 0, 0,  7, 7, 7, -1, 27,
	3, 5, 2,  64, 2, 18, 3,  8,  8,  6, 12, 12, 0, 0,  7, 7, 7, -1, 18,
	3, 5, 2,  44, 2, 11, 3,  4,  4,  4,  6,  6, 2, 2,  5, 5, 5, -1, 10,
	3, 5, 2,  50, 2, 14, 3,  6,  6,  4,  8,  8, 0, 0,  6, 6, 6, -1, 14,
	4, 6, 3,  36, 3, 11, 4,  4,  4,  2,  4,  4, 2, 2,  5, 5, 5, 5,  11,
	4, 6, 3,  36, 3, 11, 3,  4,  4,  4,  4,  4, 2, 2,  5, 5, 5, 5,  10,
	3, 5, 2,  74, 2, 21, 4, 10,  8,  8, 18, 18, 2, 2,  7, 7, 7, -1, 26,
	3, 5, 2,  74, 2, 21, 3, 10, 10,  8, 20, 20, 2, 2,  7, 7, 7, -1, 23,
	4, 6, 3,  32, 3,  8, 3,  4,  2,  2,  4,  4, 2, 2,  5, 5, 5, 5,  9,
	4, 6, 3,  24, 4,  6, 3,  4,  4,  4,  2,  2, 0, 0,  4, 4, 4, 4,  9,
	4, 7, 3,  38, 4, 14, 5,  6,  4,  4,  4,  4, 6, 6,  5, 5, 5, 5,  17,
	4, 6, 3,  40, 3, 15, 3,  8,  6,  6,  6,  6, 4, 4,  5, 5, 5, 5,  15,
	4, 7, 3,  36, 4, 11, 3,  6,  6,  4,  4,  4, 2, 2,  5, 5, 5, 5,  11,
	4, 6, 3,  32, 3, 11, 5,  4,  4,  2,  4,  4, 2, 2,  5, 5, 5, 5,  13,
	4, 6, 3,  38, 3, 14, 5,  4,  4,  4,  6,  6, 4, 4,  5, 5, 5, 5,  18,
	4, 6, 3,  22, 3,  6, 5,  2,  2,  2,  0,  0, 8, 8,  4, 4, 4, 4,  11,
	4, 6, 3,  32, 3, 11, 5,  4,  4,  2,  4,  4, 2, 2,  5, 5, 5, 5,  13,
	4, 6, 3,  40, 3, 14, 3,  6,  6,  4,  6,  6, 4, 4,  5, 5, 5, 5,  15,
	4, 6, 3,  32, 3, 11, 2,  4,  4,  4,  4,  4, 0, 0,  5, 5, 5, 5,  9,
	4, 6, 3,  40, 3, 14, 2,  6,  6,  4,  6,  6, 4, 4,  5, 5, 5, 5,  12,
	4, 6, 3,  32, 3,  8, 2,  4,  4,  1,  2,  2, 0, 0,  4, 4, 4, 4,  7,
	4, 7, 3,  44, 4, 18, 5,  6,  6,  6,  8,  8, 6, 6,  6, 6, 6, 6,  24,
	4, 6, 3,  38, 3, 14, 4,  4,  4,  2,  6,  6, 4, 4,  5, 5, 5, 5,  15,
	4, 5, 3,  44, 3, 18, 5,  8,  6,  6,  8,  8, 8, 8,  6, 6, 6, 6,  24,
	4, 6, 3,  38, 3, 14, 4,  4,  4,  4,  6,  6, 4, 4,  5, 5, 5, 5,  16,
	4, 7, 3,  38, 4, 14, 4,  6,  6,  6,  6,  6, 6, 6,  5, 5, 5, 5,  19,
	4, 6, 3,  38, 3, 14, 3,  6,  6,  4,  6,  6, 6, 6,  5, 5, 5, 5,  14,
	4, 6, 3,  38, 3, 14, 5,  6,  4,  4,  6,  6, 6, 6,  5, 5, 5, 5,  17,
	4, 7, 3,  20, 5,  6, 4,  4,  2,  2,  0,  0, 6, 6,  4, 4, 4, 4,  9,
	4, 7, 3,  13, 6,  3, 4,  0,  2,  2,  0,  0, 2, 2,  2, 2, 2, 2,  5,
	4, 7, 3,  19, 5,  5, 4,  2,  2,  2,  2,  2, 0, 0,  4, 4, 4, 4,  7,
	4, 7, 3,  17, 5,  5, 4,  2,  2,  2,  2,  2, 0, 0,  3, 3, 3, 3,  6,
	4, 7, 3,  20, 5,  6, 5,  4,  2,  2,  0,  0, 6, 6,  4, 4, 4, 4,  12,
	4, 7, 3,  18, 5,  5, 5,  2,  2,  2,  0,  0, 6, 6,  4, 4, 4, 4,  9,
	4, 7, 3,  24, 5,  6, 4,  4,  2,  2,  0,  0,10,10,  4, 4, 4, 4,  11,
	4, 7, 3,  20, 5,  6, 4,  2,  2,  2,  0,  0, 8, 8,  4, 4, 4, 4,  10,
	4, 6, 3,  44, 3, 11, 5,  4,  4,  4,  4,  4, 2, 2,  5, 5, 5, 5,  14,
	4, 6, 3,  36, 3, 12, 4,  4,  4,  4,  6,  6, 2, 2,  5, 5, 5, 5,  14,
	3, 5, 2,  74, 2, 21, 3, 10,  8,  8, 20, 20, 2, 2,  4, 4, 7, -1, 24,
	3, 5, 2,  80, 2, 24, 4, 10,  8,  8, 20, 20, 2, 2,  8, 8, 8, -1, 31,
	3, 5, 2,  74, 2, 21, 4,  8,  8,  6, 16, 16, 4, 4,  7, 7, 7, -1, 27,
	3, 5, 2,  80, 2, 24, 3, 12, 12, 10, 22, 22, 2, 2,  7, 7, 7, -1, 27,
	3, 5, 2,  74, 2, 21, 3, 10, 10,  8, 20, 20, 2, 2,  7, 7, 7, -1, 24,
	3, 5, 1, 112, 1, 27, 2, 12, 12, 12, 24, 24, 0, 0,  9, 9, 9, -1, 27,
	3, 5, 1,  96, 1, 24, 2, 12, 12, 10, 20, 20, 0, 0,  8, 8, 8, -1, 24,
	3, 5, 2,  80, 2, 23, 2, 10, 10,  8, 20, 20, 0, 0,  7, 7, 7, -1, 23,
	3, 5, 2,  74, 2, 21, 2, 10,  8,  8, 16, 16, 4, 4,  7, 7, 7, -1, 20,
	4, 6, 3,  37, 3, 12, 4,  4,  4,  2,  6,  6, 4, 4,  5, 5, 5, 5,  14,
	4, 7, 3,  16, 5,  5, 5,  2,  2,  2,  0,  0, 4, 4,  4, 4, 4, 4,  10,
	4, 7, 3,  11, 6,  3, 4,  2,  2,  2,  0,  0, 2, 2,  2, 2, 2, 2,  5,
	4, 7, 3,  26, 5,  6, 4,  4,  2,  2,  2,  2, 6, 6,  4, 4, 4, 4,  12,
	4, 7, 3,  20, 5,  6, 4,  4,  2,  2,  0,  0, 6, 6,  4, 4, 4, 4,  11,
	4, 7, 3,  17, 5,  5, 4,  2,  2,  2,  0,  0, 6, 6,  4, 4, 4, 4,  9,
	4, 7, 3,   7, 6,  3, 4,  0,  2,  2,  0,  0, 2, 2,  2, 2, 2, 2,  4,
	4, 6, 3,  40, 3, 15, 4,  4,  4,  4,  8,  8, 6, 6,  5, 5, 5, 5,  17,
	4, 6, 3,  44, 3, 15, 4,  8,  8,  6, 10, 10, 2, 2,  6, 6, 6, 6,  20,
	4, 6, 3,  38, 3, 14, 4,  4,  4,  4,  6,  6, 6, 6,  5, 5, 5, 5,  15,
	4, 5, 3,  44, 3, 18, 5,  8,  6,  6,  8,  8, 8, 8,  6, 6, 6, 6,  24,
	4, 6, 3,  36, 3,  9, 5,  4,  4,  2,  4,  4, 2, 2,  5, 5, 5, 5,  13,
	3, 5, 2,  50, 2, 14, 2,  6,  6,  6,  8,  8, 0, 0,  6, 6, 6, -1, 14,
	3, 5, 1, 136, 1, 30, 1,  8, 14, 14, 28, 28, 0, 0,  9, 9, 9, -1, 27,
	3, 5, 1, 120, 1, 27, 5, 16, 14, 14, 28, 28, 2, 2,  9, 9, 9, -1, 43,
/*72*/  3, 5, 1, 120, 2, 21, 5, 15, 17, 15, 25, 25, 7, 7,  9, 9, 9, -1, 36,
/*73*/  3, 5, 1,  90, 3, 18, 4, 13, 15, 13, 20, 20, 6, 6,  5, 5, 5,  5, 28,
/*74*/  4, 7, 3,   6, 6,  3, 4,  2,  2,  2, 20, 20, 6, 6,  2, 2, 3,  3,  5,
/*75*/  3, 5, 1, 110, 2, 20, 4, 14, 15, 11, 26, 26, 8, 8,  7, 8, 9, -1, 34,

/*int bs, fs, ta, guns, class, hull, qual, crew1, crew2, crew3, gunL, gunR, carL, carR, rig1, rig2, rig3, rig4, pts*/
};

struct windeffects WET[7][6] = {
	{ {9,9,9,9}, {9,9,9,9}, {9,9,9,9}, {9,9,9,9}, {9,9,9,9}, {9,9,9,9} },
	{ {3,2,2,0}, {3,2,1,0}, {3,2,1,0}, {3,2,1,0}, {2,1,0,0}, {2,1,0,0} },
	{ {1,1,1,0}, {1,1,0,0}, {1,0,0,0}, {1,0,0,0}, {1,0,0,0}, {1,0,0,0} },
	{ {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0} },
	{ {0,0,0,0}, {1,0,0,0}, {1,1,0,0}, {1,1,0,0}, {2,2,1,0}, {2,2,1,0} },
	{ {1,0,0,0}, {1,1,0,0}, {1,1,1,0}, {1,1,1,0}, {3,2,2,0}, {3,2,2,0} },
	{ {2,1,1,0}, {3,2,1,0}, {3,2,1,0}, {3,2,1,0}, {3,3,2,0}, {3,3,2,0} }
};
struct Tables RigTable[11][6] = {
	 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,1, 0,0,1,0 , 
	 0,0,0,0, 0,0,0,0, 0,0,0,1, 0,0,1,0, 1,0,0,1, 0,1,1,1 ,
	 0,0,0,0, 0,0,0,1, 0,0,1,1, 0,1,0,1, 0,1,0,1, 1,0,1,2 ,
	 0,0,0,0, 0,0,1,1, 0,1,0,1, 0,0,0,2, 0,1,0,2, 1,0,1,2 ,
	 0,1,0,1, 1,0,0,1, 0,1,1,2, 0,1,0,2, 0,0,1,3, 1,0,1,4 ,
	 0,0,1,1, 0,1,0,2, 1,0,0,3, 0,1,1,3, 1,0,0,4, 1,1,1,4 ,
	 0,0,1,2, 0,1,1,2, 1,1,0,3, 0,1,0,4, 1,0,0,4, 1,0,1,5 ,
	 0,0,1,2, 0,1,0,3, 1,1,0,3, 1,0,2,4, 0,2,1,5, 2,1,0,5 ,
	 0,2,1,3, 1,0,0,3, 2,1,0,4, 0,1,1,4, 0,1,0,5, 1,0,2,6 ,
	 1,1,0,4, 1,0,1,4, 2,0,0,5, 0,2,1,5, 0,1,2,6, 0,2,0,7 ,
	 1,0,1,5, 0,2,0,6, 1,2,0,6, 1,1,1,6, 2,0,2,6, 1,1,2,7 ,
};
struct Tables HullTable[11][6] = {
	 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,0,0, 0,1,0,0 , 
	 0,0,0,0, 0,0,0,0, 0,1,0,0, 1,1,0,0, 1,0,1,0, 1,0,1,1 ,
	 0,1,0,0, 1,0,0,0, 1,1,0,0, 1,0,1,0, 1,0,1,1, 2,1,0,0 ,
	 0,1,1,0, 1,0,0,0, 1,1,1,0, 2,0,0,1, 2,0,1,0, 2,2,0,0 ,
	 0,1,1,0, 1,0,0,1, 2,1,0,1, 2,2,1,0, 3,0,1,0, 3,1,0,0 ,
	 1,1,1,0, 2,0,2,1, 2,1,1,0, 2,2,0,0, 3,1,0,1, 3,1,1,0 ,
	 1,2,2,0, 2,0,2,1, 2,1,0,1, 2,2,0,0, 3,1,1,0, 4,2,1,0 ,
	 2,1,1,0, 2,0,1,1, 3,2,2,0, 3,2,0,0, 4,2,1,0, 4,2,1,1 ,
	 2,1,2,0, 3,1,1,1, 3,2,2,0, 4,2,1,0, 4,1,0,2, 4,2,2,0 ,
	 2,3,1,0, 3,2,2,0, 3,2,2,1, 4,2,2,0, 4,1,0,3, 5,1,2,0 ,
	 2,2,4,0, 3,3,1,1, 4,2,1,1, 5,1,0,2, 5,1,2,1, 6,2,2,0 ,
};

int AMMO[9][4] = {
	{-1,1,0,1},
	{-1,1,0,1},
	{-1,1,0,1},
	{-1,1,0,2},
	{-1,2,0,2},
	{-2,2,0,2},
	{-2,2,0,2},
	{-2,2,0,3},
	{-2,2,0,3}
};
	
int HDT[9][10] = {
	{1,0,-1,-2,-3,-3,-4,-4,-4,-4},
	{1,1,0,-1,-2,-2,-3,-3,-3,-3},
	{2,1,0,-1,-2,-2,-3,-3,-3,-3},
	{2,2,1,0,-1,-1,-2,-2,-2,-2},
	{3,2,1,0,-1,-1,-2,-2,-2,-2},
	{3,3,2,1,0,0,-1,-1,-1,-1},
	{4,3,2,1,0,0,-1,-1,-1,-1},
	{4,4,3,2,1,1,0,0,0,0},
	{5,4,3,2,1,1,0,0,0,0}
};

int HDTrake[9][10] = {
	{2,1,0,-1,-2,-2,-3,-3,-3,-3},
	{2,2,1,0,-1,-1,-2,-2,-2,-2},
	{3,2,1,0,-1,-1,-2,-2,-2,-2},
	{4,3,2,1,0,0,-1,-1,-1,-1},
	{5,4,3,2,1,1,0,0,0,0},
	{6,5,4,3,2,2,1,1,1,1},
	{7,6,5,4,3,3,2,2,2,2},
	{8,7,6,5,4,4,3,3,3,3},
	{9,8,7,6,5,5,4,4,4,4}
};

int QUAL[9][5] = {
	{-1,0,0,1,1},
	{-1,0,0,1,1},
	{-1,0,0,1,2},
	{-1,0,0,1,2},
	{-1,0,0,2,2},
	{-1,-1,0,2,2},
	{-2,-1,0,2,2},
	{-2,-1,0,2,2},
	{-2,-1,0,2,3}
};

int MT[9][3] = {
	{1,0,0},
	{1,1,0},
	{2,1,0},
	{2,1,1},
	{2,2,1},
	{3,2,1},
	{3,2,2},
	{4,3,2},
	{4,4,2}
};

int loaded, fired, changed, repaired, buffercount, xlast, ylast;
long lastsync;
int winddir, windspeed, turn, viewrow, viewcol;
int player = -1, nation[5], scroll = 18, game;
int MIGHTYCAPTAIN;
char Outbuf[BUFSIZE], movebuf[10], loadwith[20];
FILE *syncfile;
