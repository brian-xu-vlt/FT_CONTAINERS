#ifndef MAP_ITERATOR_HPP
# define MAP_ITERATOR_HPP

# include "map_pair.hpp"
# include <iostream>
# include <cstddef>

#ifndef DEBUG_MODE
# define DEBUG_MODE 0
#endif


namespace ft	{

	/**
    *   Use a boolean to typedef either type 1 or type 2.
    */
    template <bool isConst, typename isFalse, typename isTrue>
    struct ft_enable_if {};

    /**
    *   Typedef: pointer, ref...
    */
    template <typename isFalse, typename isTrue>
    struct ft_enable_if<false, isFalse, isTrue>
    {
        typedef isFalse type;
    };

    /**
    *   Typedef: const pointer, const ref...
    */
    template <typename isFalse, typename isTrue>
    struct ft_enable_if<true, isFalse, isTrue>
    {
        typedef isTrue type;
    };


	template< 	class Key,
				class T,
				class Compare,
				typename map_node, bool B>
	class map_iterator	{

            typedef std::bidirectional_iterator_tag			iterator_category;

			typedef std::reverse_iterator< map_iterator<Key, T, Compare, map_node, false> >	reverse_iterator;
			typedef std::reverse_iterator< map_iterator<Key, T, Compare, map_node, true> >	const_reverse_iterator;

			typedef	Compare									key_compare;
			typedef typename ft::pair<const Key, T>			value_type;
			typedef typename ft_enable_if<B, value_type&, const value_type&>::type       reference;
            typedef typename ft_enable_if<B, value_type*, const value_type*>::type       pointer;

			// typedef	ft::pair<const Key, T>*					pointer;
			// typedef	ft::pair<const Key, T>&					reference;
			// typedef	const ft::pair<const Key, T>*			const_pointer;
			// typedef	const ft::pair<const Key, T>&			const_reference;
		public:

			map_iterator( map_node* ptr = NULL, map_node* dumbNode = NULL,
				const key_compare& comp = key_compare() ) :	_ptr(ptr),
															_btreeDumdNode(dumbNode),
															_comp(comp)		{}


			map_iterator(const map_iterator<Key, T, Compare, map_node, false>& itSrc) :	_ptr(itSrc._ptr),
																							_btreeDumdNode(itSrc._btreeDumdNode),
																							_comp(itSrc._comp)		{}

			~map_iterator( void )	{}

			map_iterator&
			operator++( void ) {

				if (_ptr == _btreeDumdNode)
					_ptr = _btreeDumdNode->left;
				else if (isLastNode(_ptr) == true)
					_ptr = _btreeDumdNode;
				else if (isLeaf(_ptr) == true)	{
					if (_ptr == _ptr->parent->left)
						_ptr = _ptr->parent;
					else
						getNextBranch();
				}
				else	{
					if (_ptr->right != NULL)
						_ptr = getFarLeft(_ptr->right);
					else
						getNextBranch();
				}
				return *this;
			}

			map_iterator
			operator++( int ) {
				map_iterator tmp(*this);
				operator++();
				return tmp;
			}

			map_iterator&
			operator--( void ) {

				if (_ptr == _btreeDumdNode)
					_ptr = _btreeDumdNode->right;
				else if (isFirstNode(_ptr) == true)
					_ptr = _btreeDumdNode;
				else if (isLeaf(_ptr) == true)	{
					if (_ptr == _ptr->parent->right)
						_ptr = _ptr->parent;
					else
						getPreviousBranch();
				}
				else	{
					if (_ptr->left != NULL)
						_ptr = getFarRight(_ptr->left);
					else
						getPreviousBranch();
				}
				return *this;
			}

			map_iterator
			operator--( int ) {
				map_iterator tmp(*this);
				operator--();
				return tmp;
			}

			bool
			operator==(const map_iterator& rhs) const	{ return _ptr==rhs._ptr; }

			bool
			operator!=(const map_iterator& rhs) const	{ return _ptr!=rhs._ptr; }

			bool
			operator<(const map_iterator& rhs) const	{ return _ptr< rhs._ptr; }

			pointer
			operator->()	const		{ return (&_ptr->item); }

			reference
			operator*()	const		{ return (_ptr->item); }

			/**
			 * @brief Pointer holding the address of the map_iterator element.
			*/
			map_node*			_ptr;
			map_node*			_btreeDumdNode;
			Compare				_comp;

		private:

			map_node*
			getFarLeft( map_node* cursor )	{

				while (cursor != NULL && cursor->left != NULL)
					cursor = cursor->left;
				return (cursor);
			}

			map_node*
			getFarRight( map_node* cursor )	{

				while (cursor != NULL && cursor->right != NULL)
					cursor = cursor->right;
				return (cursor);
			}

			void
			getNextBranch( void )	{

				Key				startKey = _ptr->item.first;
				map_node*		cursor = _ptr->parent;

				while (cursor != NULL && _comp(cursor->item.first, startKey) == true)
					cursor = cursor->parent;
				_ptr = cursor;
			}

			void
			getPreviousBranch( void )	{

				Key				startKey = _ptr->item.first;
				map_node*		cursor = _ptr->parent;

				while (cursor != NULL && _comp(startKey, cursor->item.first) == true)
					cursor = cursor->parent;
				_ptr = cursor;
			}

			bool
			isFirstNode( map_node* p )	{
				return (p == _btreeDumdNode->left);
			}

			bool
			isLastNode( map_node* p )	{
				return (p == _btreeDumdNode->right);
			}

			bool
			isLeaf(map_node* node)	{
				return (node->left == NULL && node->right == NULL);
			}


		}; //----------------- Class map_iterator


} // ----------------- ft namespace

#endif
