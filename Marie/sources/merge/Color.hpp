#pragma once

# define Yellow "\x1B[93m"
# define Red "\x1B[31m"

# define Green "\x1B[32m"
# define BlueP "\x1B[34"
# define Blue "\x1B[36m"
# define Colored "\033[3;44;30m"
# define Color "\033[0m"


/*COULEUR
	std::cout << "\x1B[31mTexting\033[0m\t\t" << std::endl; // rouge
	printf("\x1B[32mTexting\033[0m\t"); // vert
    printf("\x1B[33mTexting\033[0m"); // jaune
    printf("\x1B[34mTexting\033[0m"); // bleu violet
    
    printf("\x1B[36mTexting\033[0m\t"); // bleu
    printf("\x1B[93mTexting\033[0m\n"); // grand jaune
    
    printf("\033[3;44;30mTexting\033[0m\t"); // cool
*/