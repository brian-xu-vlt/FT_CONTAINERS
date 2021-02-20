/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_vector_iterator.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bvalette <bvalette@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/17 09:50:38 by bvalette          #+#    #+#             */
/*   Updated: 2021/02/20 12:08:11 by bvalette         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bvaletteTester_vector.hpp"

int
test_vector_iterator( void )	{
	std::cout << TITLE << "~~~~~~~~~~~ " << __func__ << " with ints ~~~~~~~~~~~" << RESET_COLOR << std::endl;
	std::cout << HEADER_TITLE << "ASCENDING ORDER VALUES" << RESET_COLOR << std::endl;
	{
		ft::vector<int>		ft_c0;
		size_t				testSize = 10;

		std::cout << SUBTITLE << "[ pushback " << testSize << " ASCENDING even values in vector 0 ]" << RESET_COLOR << std::endl;

		for (size_t i = 0; i < testSize; i++)	{
			ft_c0.push_back(i);
		}
		ft::vector<int>::iterator		ft_it = ft_c0.begin();
		ft::vector<int>::iterator		ft_it2 = ft_c0.begin();
		ft::vector<int>::iterator		ft_itend = ft_c0.end();

		std::cout << SUBTITLE << "[ test operator< with iterator to begin < end ]" << RESET_COLOR << std::endl;
		testBool(ft_it < ft_itend, __LINE__);
		std::cout << SUBTITLE << "[ loop test iterator increment vs. iterator + i ]" << RESET_COLOR << std::endl;
		for (size_t i = 0; i < ft_c0.size(); i++)
		{
			testBool((*ft_it == *(ft_it2 + i)), __LINE__);
			ft_it++;
		}
		std::cout << SUBTITLE << "[ test operator= ]" << RESET_COLOR << std::endl;
		ft_it2 = ft_it;
		testBool(ft_it == ft_it2, __LINE__);
		std::cout << SUBTITLE << "[ loop test iterator increment vs. iterator + i ]" << RESET_COLOR << std::endl;
		for (size_t i = 0; i < ft_c0.size(); i++)
		{
			testBool((*ft_it == *(ft_it2 - i)), __LINE__);
			ft_it--;
		}
		testBool(static_cast<size_t>(ft_itend - ft_it) == ft_c0.size(), __LINE__);
	}
	std::cout << HEADER_TITLE << "TEST ITERATOR ARITHMETIC" << RESET_COLOR << std::endl;
	{
		ft::vector<int>			l;
		l.push_back(0);
		l.push_back(1);
		l.push_back(2);
		l.push_back(3);
		l.push_back(4);
		l.push_back(5);
		ft::vector<int>::iterator it1 = ++l.begin();
		ft::vector<int>::iterator it2 = l.begin() + 2;
		it1++;
		ft::vector<int>::iterator ite1 = --l.end();
		ite1--;
		ft::vector<int>::iterator ite2 = l.end() - 2;

		testBool(*it1 == 2, __LINE__);
		testBool(*ite1 == 4, __LINE__);
		testBool(*it1 == *it2, __LINE__);
		testBool(*ite1 == *ite2, __LINE__);
	}

	return (0);
}