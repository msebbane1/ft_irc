/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Color.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msebbane <msebbane@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/20 15:43:57 by msebbane          #+#    #+#             */
/*   Updated: 2023/06/17 14:21:09 by msebbane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

# define Yellow "\x1B[93m"
# define Red "\x1B[31m"
# define Grey "\x1B[37m"

# define Green "\x1B[32m"
# define BlueP "\x1B[34"
# define Blue "\x1B[36m"
# define BLUE "\033[0;34m"
# define CYAN "\033[0;36m"
# define RED "\033[0;31m"
# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
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