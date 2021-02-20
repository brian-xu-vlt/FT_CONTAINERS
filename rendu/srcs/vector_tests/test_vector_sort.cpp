/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_vector_sort.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvalette <bvalette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 09:51:20 by bvalette          #+#    #+#             */
/*   Updated: 2021/02/20 12:08:11 by bvalette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvaletteTester_vector.hpp"

int
test_vector_sort( void )	{
	std::cout << TITLE << "~~~~~~~~~~~ " << __func__ << " with ints ~~~~~~~~~~~" << RESET_COLOR << std::endl;
	std::cout << HEADER_TITLE << "vector FILLED WITH A RANDOM NUMBER OF RANDOM VALUES" << RESET_COLOR << std::endl;
	{
		ft::vector<int>		ft_c0;
		std::vector<int>		std_c0;
		srand(reinterpret_cast<long unsigned int>(&std_c0));
		size_t				testSize = rand() % 15000;

		std::cout << SUBTITLE << "[ pushback " << testSize << " random values into vector 0]" << RESET_COLOR << std::endl;

		for (size_t i = 0; i < testSize; i++)	{
			int val = rand() % 10000;
			ft_c0.push_back(val);
			std_c0.push_back(val);
		}
		testVector(ft_c0, std_c0, NOPRINT);
		std::cout << SUBTITLE << "[ sort vector 0 with greater_than Compare ]" << RESET_COLOR << std::endl;
		ft_c0.sort(greater_than);
		std_c0.sort(greater_than);
		testVector(ft_c0, std_c0, NOPRINT);

		std::cout << SUBTITLE << "[ sort vector 0 with not argument ]" << RESET_COLOR << std::endl;
		ft_c0.sort();
		std_c0.sort();
		testVector(ft_c0, std_c0, NOPRINT);
	}
	return (0);
}
