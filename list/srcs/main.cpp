/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvalette <bvalette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/16 16:23:24 by bvalette          #+#    #+#             */
/*   Updated: 2021/02/17 09:58:55 by bvalette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * Compilation may change DEBUG_MODE setting to
 *
 * 	DEBUG_MODE=0 -> no special output
 * 	DEBUG_MODE=1 -> continue after test a failed (abort otherwise)
 * 				 	+ minimal informations (mainly constructor/destructor calls)
 * 	DEBUG_MODE=2 -> more indepth infos
*/

#include "bvaletteTester.hpp"

int main ( void )	{

	main_tester_list();
	// main_tester_vector();

	return (0);
}
